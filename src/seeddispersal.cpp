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

void Seeddispersal(int jahr, struct Parameter* parameter, vector<VectorList<Seed>>& world_seed_list) {
    int aktort = 0;

    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;

        // determine the current location, so that in long distance dispersal the target can be determined
        aktort++;

        // variable for displaying seeds crossing the borders
        int rausgeflogenN = 0, rausgeflogenO = 0, rausgeflogenS = 0, rausgeflogenW = 0;

		// implementation of multi-core-processing == split list to X lists
		omp_set_dynamic(0);                                 // disable dynamic teams ; TODO remove later
		omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers ; TODO remove later
		std::random_device random_dev;

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
							double direction = 0.0;
							double velocity = 0.0;
							double wdirection = 0.0;
							double jquer = 0;
							double iquer = 0;

							Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, (double) seed.releaseheight, seed.species);

							if (parameter[0].ivort > 1045 && parameter[0].outputmode != 9 && parameter[0].omp_num_threads == 1) {
								double seedeinschreibzufall = uniform(rng);

								if (seedeinschreibzufall < 0.01) {
									// dispersaldistance = sqrt(pow(iquer, 2) + pow(jquer, 2));
									direction = atan2(iquer, jquer);

									FILE* filepointer;
									string dateiname;

									// assemble file name
									char dateinamesuf[12];
									sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice, parameter[0].repeati);
									dateiname = "output/dataseed_distance" + string(dateinamesuf) + ".csv";

									filepointer = fopen(dateiname.c_str(), "r+");
									if (filepointer == NULL) {
										filepointer = fopen(dateiname.c_str(), "w");

										fprintf(filepointer, "IVORT;");
										// fprintf(filepointer, "name;");
										fprintf(filepointer, "year;");
										fprintf(filepointer, "parentheight;");
										// fprintf(filepointer, "distance;");
										fprintf(filepointer, "direction;");
										fprintf(filepointer, "xcoo;");
										fprintf(filepointer, "ycoo;");
										fprintf(filepointer, "species;");
										fprintf(filepointer, "weatherchoice;");
										fprintf(filepointer, "thawing_depth;");
										fprintf(filepointer, "windspd;");
										fprintf(filepointer, "winddir;");

										fprintf(filepointer, "\n");

										if (filepointer == NULL) {
											fprintf(stderr, "Error: seed distance file could not be opened!\n");
											exit(1);
										}
									}

									fseek(filepointer, 0, SEEK_END);

									fprintf(filepointer, "%d;", parameter[0].ivort);
									// fprintf(filepointer, "%d;", seed.namem);
									fprintf(filepointer, "%d;", jahr);
									fprintf(filepointer, "%4.3f;", (double) seed.releaseheight);
									// fprintf(filepointer, "%4.5f;", dispersaldistance);
									fprintf(filepointer, "%4.5f;", direction);
									fprintf(filepointer, "%4.5f;", (double) seed.xcoo/1000);
									fprintf(filepointer, "%4.5f;", (double) seed.ycoo/1000);
									fprintf(filepointer, "%d;", seed.species);
									fprintf(filepointer, "%d;", parameter[0].weatherchoice);
									fprintf(filepointer, "%d;", parameter[0].thawing_depth);
									fprintf(filepointer, "%lf;", velocity);
									fprintf(filepointer, "%lf;", wdirection);
									fprintf(filepointer, "\n");

									fclose(filepointer);
								}
							}

							seed.xcoo = (unsigned int) floor(1000* ((double)seed.xcoo/1000 + jquer));
							seed.ycoo = (unsigned int) floor(1000* ((double)seed.ycoo/1000 + iquer));
							// seed.dispersaldistance = dispersaldistance;

							/****************************************************************************************/
							/**
							 * \brief calculate  Dispersal
							 *within a plot
							 *
							 *
							 *******************************************************************************************/
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

