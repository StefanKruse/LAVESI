#include <random>
#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

/****************************************************************************************/
/**
 * \brief calculate dispersal distance dependent on dispersal mode
 *(0-5)
 *
 *
 *
 *
 *******************************************************************************************/

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

/****************************************************************************************/
/**
 * \brief calculate seed dispersal
 *
 *
 *
 *
 *
 *******************************************************************************************/

void Seeddispersal(
					int jahr, 
					struct Parameter* parameter, 
					vector<VectorList<Seed>>& world_seed_list,
					vector<vector<Envirgrid*>>& world_plot_list) {
    int aktort = 0;

    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;

		vector<vector<Envirgrid*>>::iterator world_positon_b = (world_plot_list.begin() + aktort);
		vector<Envirgrid*>& plot_list = *world_positon_b;

        // determine the current location, so that in long distance dispersal the target can be determined
        aktort++;

        // variable for displaying seeds crossing the borders
        int rausgeflogenN = 0, rausgeflogenO = 0, rausgeflogenS = 0, rausgeflogenW = 0;

		std::random_device random_dev;
		
// pragma omp initializing
omp_set_dynamic(1); //enable dynamic teams
omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

#pragma omp parallel
{
		std::mt19937 rng(random_dev());
		std::uniform_real_distribution<double> uniform(0, 1);
#pragma omp for
		for (unsigned int i = 0; i < seed_list.size(); ++i) {
			auto& seed = seed_list[i];
			if (seed.dead) {
				continue;
			}

			if (seed.incone == true) {
				double flug = uniform(rng);

				if (flug <= parameter[0].seedflightrate) {
					double ratiorn = uniform(rng);

					if (ratiorn > 0.0) {
						seed.incone = false;

						// double dispersaldistance = 0.0;
						// double direction = 0.0;
						double velocity = 0.0;
						double wdirection = 0.0;
						double jquer = 0.0;
						double iquer = 0.0;

						double randomnumberwind = uniform(rng);
						Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, (double) seed.releaseheight/100, seed.species, randomnumberwind);

						// disperal limitation by elevation 
						if(parameter[0].demlandscape) {
							// TODO: add switch to parameter.txt
							// seed trajectory elevation sensing
							// 1. define releaseheight
							// 2. access elevation along path
							// 3. stop either when elevation>releaseheight or dispersal distance reached

							// get start coordinates in elevation input grid
								// bring to envirgrid resolution
									int ycoostartdem = (int)floor((double)seed.ycoo/1000 * parameter[0].sizemagnif);
									int xcoostartdem = (int)floor((double)seed.xcoo/1000 * parameter[0].sizemagnif);

								// get elevation of release + releaseheight
									double startele = (double)plot_list[(unsigned long long int) ycoostartdem * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif + (unsigned long long int) xcoostartdem]->elevation/10 + (double)seed.releaseheight/100;

								// get target position on input grid
									int ycootargetonelegrid = (int)floor(((double)seed.ycoo/1000 + iquer) * parameter[0].sizemagnif);
									int xcootargetonelegrid = (int)floor(((double)seed.xcoo/1000 + jquer) * parameter[0].sizemagnif);

								// go stepswise to next grid in direction of dispersal
									double ysteps = ycootargetonelegrid-ycoostartdem;
									double xsteps = xcootargetonelegrid-xcoostartdem;
									double ystepsize = ysteps / abs(ysteps);
									double xstepsize = xsteps / abs(xsteps);
									double maxsteps = abs(ysteps)+abs(xsteps);
									// for upslope calculation
									// double eleonestepfurther = 32767/10;
									// if( ((ycoostartdem + ysteps*150) < (parameter[0].sizemagnif*(treerows-1)))
											// & ((xcoostartdem + xsteps*150) < (parameter[0].sizemagnif*treecols-1))
											// & ((ycoostartdem + ysteps*150)>=0)
											// & ((xcoostartdem + xsteps*150)>=0)
										// ) {
										 // eleonestepfurther = plot_list[(unsigned long long int) (ycoostartdem + ysteps*150) * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif + (unsigned long long int) (xcoostartdem + xsteps*150)]->elevation/10;
									// }

									unsigned int stepi;
									unsigned int stepdistance = 150; //in m when divided by parameter[0].sizemagnif, as dem curently 30 m resolution and grid 0.2 m, 150 means all 30 m
									for(stepi=1; stepi <= maxsteps; stepi=stepi+stepdistance) {
										// check whether step in y or x direction
										if(abs(ysteps)>=abs(xsteps)){
											ycoostartdem = ycoostartdem + (int)stepdistance*ystepsize;
											ysteps = ysteps - ystepsize;
										} else {
											xcoostartdem = xcoostartdem + (int)stepdistance*xstepsize;
											xsteps = xsteps - xstepsize;
										}
										
										// only if still in plot
										if( (ycoostartdem < (parameter[0].sizemagnif*(treerows-1)))
											& (xcoostartdem < (parameter[0].sizemagnif*treecols-1))
											& (ycoostartdem>=0)
											& (xcoostartdem>=0)
										) {

											// assess current elevation
											double actele = (double)plot_list[(unsigned long long int) ycoostartdem * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif + (unsigned long long int) xcoostartdem]->elevation/10;

											
											if( (actele > startele) 
												& (actele!=32767/10) 
											) {// treat NA values (water areas) as last elevation values
												break;
											}
										} else {// seed leaves the plot
											stepi = maxsteps;
											break;
										}
									}
									// calculate dispersal distance reduced by altitudinal impact
									double dispfraction = stepi/maxsteps;
									if(dispfraction>1.0) {
										dispfraction=1.0;
									} else if (dispfraction<0.0) {
										dispfraction=0.0;
									}
									
									// shorten when upslope dispersal
									double upslopedispfact=1.0;
									// if( (eleonestepfurther!=32767/10)
										// & (eleonestepfurther>startele)
									// ) {
										// upslopedispfact = 1.0 - ((eleonestepfurther-startele)/30/2); // scale to 30 m steps as elevation input; if +30 m on 30 m value 0.5
										
										// if(upslopedispfact<0.0)
											// upslopedispfact=0.0;
									// }

									seed.xcoo = (unsigned int) floor(1000* ((double)seed.xcoo/1000 + upslopedispfact*dispfraction*jquer));
									seed.ycoo = (unsigned int) floor(1000* ((double)seed.ycoo/1000 + upslopedispfact*dispfraction*iquer));

						} else {
							seed.xcoo = (unsigned int) floor(1000* ((double)seed.xcoo/1000 + jquer));
							seed.ycoo = (unsigned int) floor(1000* ((double)seed.ycoo/1000 + iquer));
						}

						// check whether seed lands on plot or leaves the plot
						bool sameausserhalb = false;

						// Check if the seed is on the plot:
						if ((double)seed.ycoo/1000 > (double)(treerows - 1)) {
							if ((parameter[0].boundaryconditions == 1)) {
								seed.ycoo = (unsigned int) floor(1000* fmod((double)seed.ycoo/1000, (double)(treerows - 1)));
								// seed.namem = 0;
								// seed.namep = 0;
							} else if ((parameter[0].boundaryconditions == 3)) {
								sameausserhalb = true;
								rausgeflogenN++;
							} else {
								sameausserhalb = true;
								rausgeflogenN++;
							}
						} else if (seed.ycoo < 0) {
							if ((parameter[0].boundaryconditions == 1)) {
								seed.ycoo = (unsigned int) floor(1000* ((double)(treerows - 1) + fmod((double)seed.ycoo/1000, (double)(treerows - 1))));
								// seed.namem = 0;
								// seed.namep = 0;
							} else if ((parameter[0].boundaryconditions == 3)) {
								sameausserhalb = true;
								rausgeflogenS++;
							} else {
								sameausserhalb = true;
								rausgeflogenS++;
							}
						}

						if (seed.xcoo < 0) {
							if ((parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3)) {
								seed.xcoo = (unsigned int) floor(1000*fmod((double)seed.xcoo/1000, (double)(treecols - 1)) + (double)(treecols - 1));
								// seed.namem = 0;
								// seed.namep = 0;
							} else {
								sameausserhalb = true;
								rausgeflogenW++;
							}
						} else if ((double)seed.xcoo/1000 > (double)(treecols - 1)) {
							if (parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3) {
								seed.xcoo = (unsigned int) floor(1000*fmod((double)seed.xcoo/1000, (double)(treecols - 1)));
								// seed.namem = 0;
								// seed.namep = 0;

							} else if ((parameter[0].boundaryconditions == 2)
									   && (rand() < 0.5 * RAND_MAX)) {  // Reducing seed introduction on the western border:
								seed.xcoo = (unsigned int) floor(1000*fmod((double)seed.xcoo/1000, (double)(treecols - 1)));
							} else {
								sameausserhalb = true;
								rausgeflogenO++;
							}
						}

						if ((sameausserhalb == false)
							&& ((seed.ycoo < 0) | ((double)seed.ycoo/1000 > (double)(treerows - 1)) | (seed.xcoo < 0)
								| ((double)seed.xcoo/1000 > (double)(treecols - 1)))) {
							printf("\n\nLaVeSi was exited ");
							printf("in Seeddispersal.cpp\n");
							printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n",
								   (double)seed.ycoo/1000, (double)seed.xcoo/1000);
							exit(1);
						}

						if (sameausserhalb) {
							seed.dead = true;
							seed_list.remove(i);
						}
					}
				}
			}
		}
}  // pragma
	}
}

