#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void Seedoutput(int aktort, double dispersaldistance, float direction, int neueweltcoo) {
    FILE* filepointer;
    string dateiname;

    // assemble file names
    dateiname = "output/dataseed_LDSD.csv";

    // Trying to open file for reading
    filepointer = fopen(dateiname.c_str(), "r+");
    // if fopen fails create new data file + header
    if (filepointer == NULL) {
        filepointer = fopen(dateiname.c_str(), "w");
        fprintf(filepointer, "ivort;");
        fprintf(filepointer, "aktort;");
        fprintf(filepointer, "dispersaldistance;");
        fprintf(filepointer, "direction;");
        fprintf(filepointer, "neueweltcoo;");
        fprintf(filepointer, "\n");

        if (filepointer == NULL) {
            fprintf(stderr, "Error: long distance seed dispersal file could not be opened!\n");
            exit(1);
        }
    }

    // else: append data to file
    fseek(filepointer, 0, SEEK_END);

    // data evaluation and output
    fprintf(filepointer, "%d;", parameter[0].ivort);
    fprintf(filepointer, "%d;", aktort);
    fprintf(filepointer, "%4.5f;", dispersaldistance);
    fprintf(filepointer, "%4.5f;", direction);
    fprintf(filepointer, "%d;", neueweltcoo);
    fprintf(filepointer, "\n");

    fclose(filepointer);
}

void Seeddispersal(//int jahr, 
				   Parameter* parameter, 
				   vector<VectorList<Seed>>& world_seed_list, 
				   vector<vector<Envirgrid>>& world_plot_list) {
    int aktort = 0;

    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;

        vector<vector<Envirgrid>>::iterator world_positon_b = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *world_positon_b;

        // determine the current location, so that in long distance dispersal the target can be determined
        aktort++;

        // variable for displaying seeds crossing the borders // TODO: move to only used when debugging mode
        int seedleftN = 0, seedleftE = 0, seedleftS = 0, seedleftW = 0;

        RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
        for (unsigned int i = 0; i < seed_list.size(); ++i) {
            auto& seed = seed_list[i];
            if (!seed.dead && seed.incone) {
                if (uniform.draw() <= parameter[0].seedflightrate) {
                    double ratiorn = uniform.draw();

                    seed.incone = false;

                    // double dispersaldistance = 0.0;
                    // double direction = 0.0;
                    double velocity = 0.0;
                    double wdirection = 0.0;
                    double jquer = 0.0;
                    double iquer = 0.0;

                    double randomnumberwind = uniform.draw();
                    Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, (double)seed.releaseheight / 10, seed.species, randomnumberwind);

                    // disperal limitation by elevation
                    if (parameter[0].demlandscape) {
                        // seed trajectory elevation sensing
                        // 1. define releaseheight
                        // 2. access elevation along path
                        // 3. stop either when elevation>releaseheight or dispersal distance reached

                        // get start coordinates in elevation input grid
                        // bring to envirgrid resolution
                        int ycoostartdem = seed.ycoo * parameter[0].sizemagnif / 1000;
                        int xcoostartdem = seed.xcoo * parameter[0].sizemagnif / 1000;

                        // get elevation of release + releaseheight
                        double startele = (double)plot_list[(unsigned long long int)ycoostartdem * (unsigned long long int)treecols
                                                                * (unsigned long long int)parameter[0].sizemagnif
                                                            + (unsigned long long int)xcoostartdem]
                                                  .elevation
                                              / 10
                                          + (double)seed.releaseheight / 10;

                        // get target position on input grid
                        int ycootargetonelegrid = ((double)seed.ycoo / 1000 + iquer) * parameter[0].sizemagnif;
                        int xcootargetonelegrid = ((double)seed.xcoo / 1000 + jquer) * parameter[0].sizemagnif;

                        // go stepswise to next grid in direction of dispersal
                        double ysteps = ycootargetonelegrid - ycoostartdem;
                        double xsteps = xcootargetonelegrid - xcoostartdem;
                        double ystepsize = ysteps / abs(ysteps);
                        double xstepsize = xsteps / abs(xsteps);
                        double maxsteps = abs(ysteps) + abs(xsteps);

                        // for upslope calculation
                        unsigned int stepi;
                        unsigned int stepdistance =
                            150;  // in m when divided by parameter[0].sizemagnif, as dem curently 30 m resolution and grid 0.2 m, 150 means each 30 m
                        for (stepi = 1; stepi <= maxsteps; stepi += stepdistance) {
                            // check whether step in y or x direction
                            if (abs(ysteps) >= abs(xsteps)) {
                                ycoostartdem = ycoostartdem + (int)stepdistance * ystepsize;
                                ysteps = ysteps - ystepsize;
                            } else {
                                xcoostartdem = xcoostartdem + (int)stepdistance * xstepsize;
                                xsteps = xsteps - xstepsize;
                            }

                            // only if still in plot
                            if ((ycoostartdem < (int)(parameter[0].sizemagnif * (treerows - 1))) & (xcoostartdem < (int)(parameter[0].sizemagnif * treecols - 1))
                                & (ycoostartdem >= 0) & (xcoostartdem >= 0)) {
                                // assess current elevation
                                double actele = (double)plot_list[(unsigned long long int)ycoostartdem * (unsigned long long int)treecols
                                                                      * (unsigned long long int)parameter[0].sizemagnif
                                                                  + (unsigned long long int)xcoostartdem]
                                                    .elevation
                                                / 10;

                                if ((actele > startele) & (actele != 32767 / 10)) {  // treat NA values (water areas) as last elevation values
                                    break;
                                }
                            } else {  // seed leaves the plot
                                stepi = maxsteps;
                                break;
                            }
                        }
                        // calculate dispersal distance reduced by altitudinal impact
                        double dispfraction = stepi / maxsteps;
                        if (dispfraction > 1.0) {
                            dispfraction = 1.0;
                        } else if (dispfraction < 0.0) {
                            dispfraction = 0.0;
                        }

                        // shorten when upslope dispersal
                        double upslopedispfact = 1.0;
                        seed.xcoo = 1000 * ((double)seed.xcoo / 1000 + upslopedispfact * dispfraction * jquer);
                        seed.ycoo = 1000 * ((double)seed.ycoo / 1000 + upslopedispfact * dispfraction * iquer);
                    } else {
                        seed.xcoo = 1000 * ((double)seed.xcoo / 1000 + jquer);
                        seed.ycoo = 1000 * ((double)seed.ycoo / 1000 + iquer);
                    }

                    // check whether seed lands on plot or leaves the plot
                    bool sameausserhalb = false;
                    if (seed.ycoo > 1000 * ((int)treerows - 1)) {
                        if ((parameter[0].boundaryconditions == 1)) {
                            seed.ycoo = 1000 * fmod((double)seed.ycoo / 1000, (double)(treerows - 1));
                            // seed.namem = 0;
                            // seed.namep = 0;
                        } else if ((parameter[0].boundaryconditions == 3)) {
                            sameausserhalb = true;
                            seedleftN++;
                        } else {
                            sameausserhalb = true;
                            seedleftN++;
                        }
                    } else if (seed.ycoo < 0) {
                        if ((parameter[0].boundaryconditions == 1)) {
                            seed.ycoo = 1000 * ((double)(treerows - 1) + fmod((double)seed.ycoo / 1000, (double)(treerows - 1))); 
                            // seed.namem = 0;
                            // seed.namep = 0;
                        } else if ((parameter[0].boundaryconditions == 3)) {
                            sameausserhalb = true;
                            seedleftS++;
                        } else {
                            sameausserhalb = true;
                            seedleftS++;
                        }
                    }
                    if (seed.xcoo < 0) {
                        if ((parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3)) {
                            seed.xcoo = 1000 * fmod((double)seed.xcoo / 1000, (double)(treecols - 1)) + (double)(treecols - 1); 
                            // seed.namem = 0;
                            // seed.namep = 0;
                        } else {
                            sameausserhalb = true;
                            seedleftW++;
                        }
                    } else if ((double)seed.xcoo / 1000 > (double)(treecols - 1)) {
                        if (parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3) {
                            seed.xcoo = 1000 * fmod((double)seed.xcoo / 1000, (double)(treecols - 1)); 
                            // seed.namem = 0;
                            // seed.namep = 0;

                        } else if ((parameter[0].boundaryconditions == 2) && (uniform.draw() < 0.5)) {  // Reducing seed introduction on the western border:
                            seed.xcoo = 1000 * fmod((double)seed.xcoo / 1000, (double)(treecols - 1));
                        } else {
                            sameausserhalb = true;
                            seedleftE++;
                        }
                    }

                    if ((sameausserhalb == false)
                        && ((seed.ycoo < 0) || (seed.ycoo > 1000 * ((int)treerows - 1)) || (seed.xcoo < 0) || (seed.xcoo > 1000 * ((int)treecols - 1)))) {
                        printf("\n\nLaVeSi was exited ");
                        printf("in Seeddispersal.cpp\n");
                        printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n",
                               (double)seed.ycoo / 1000, (double)seed.xcoo / 1000);
                        exit(1);
                    }

                    if (sameausserhalb) {
                        seed.dead = true;
                        seed_list.remove(i);
                    }
                }
            }
        }
        seed_list.consolidate();
    }
}
