#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

// TODO temporary here
extern vector<VectorList<Tree>> world_tree_list;
extern vector<VectorList<Seed>> world_seed_list;

void Seedin() {
    int aktort = 0;
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;
        aktort++;
        int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
        int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        bool seedinput;

        // seedinput on all sites
        if (parameter[0].realseedconnect == false) {
            seedinput = true;
        } else if (parameter[0].realseedconnect == true && aktort == 1) {  // seedinput on southern site only
            seedinput = true;
        } else {  // no seedinput
            seedinput = false;
        }

        if (seedinput == true) {
            int seednobuffer;
            if (parameter[0].yearswithseedintro <= 0) {
                seednobuffer = parameter[0].seedintronumberpermanent;
            } else {
                seednobuffer = parameter[0].seedintronumber;
            }

			RandomNumber<double> uniform(0, 1);
			// RandomNumber<double> uniformneutral(0, 999999);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
            for (int n = 0; n < seednobuffer; n++) {
                // calculate post-dispersal position
                double jseed, iseed;
                bool seedeintragen = false;

                // set limits
                double maxx = (double)(treecols - 1);
                if ( (parameter[0].seedintro_maxx > 0) & (parameter[0].seedintro_maxx < (treecols - 1)) )
                    maxx = (double)parameter[0].seedintro_maxx;

                double maxy = (double)(treerows - 1);
                if ( (parameter[0].seedintro_maxy > 0) & (parameter[0].seedintro_maxy < (treerows - 1)) )
                    maxy = (double)parameter[0].seedintro_maxy;

                // seedwinddispersalmode==1 => randomly from the south border.
                if (parameter[0].seedwinddispersalmode == 1) {
                    jseed = maxx * uniform.draw();

                    double dispersaldistance;

                    do {
                        double fraction;

                        do {
                            fraction = uniform.draw();

                        } while (fraction <= 0.0);
                        dispersaldistance = (log(fraction) / (-0.2)) / parameter[0].distanceratio;

                    } while ((dispersaldistance >= (double)(treerows - 1)) || (dispersaldistance < 0.0));

                    seedeintragen = true;
                    iseed = dispersaldistance;
                }
                // seedwinddispersalmode==2 => randomly all over the plot
                else if (parameter[0].seedwinddispersalmode == 2) {
                    do{
					jseed = maxx * uniform.draw();
                    iseed = maxy * uniform.draw();
					}while( (iseed > 1000 && iseed <=49800) || (iseed > 49800 && jseed >200 && jseed <=900));


                    seedeintragen = true;
                } else {
                    printf("\n\nLaVeSi was stopped\n");
                    printf("=> Treedistribution.cpp\n");
                    printf("... reason: no choice of a seed dispersal mode");
                    exit(1);
                }

                int rn_species = 0;
				// multiple species implementation
                if (parameter[0].specpres == 0) {
					rn_species = (int) 1 + (uniform.draw() * (parameter[0].species_max-1));
				} else {
					rn_species = parameter[0].specpres;
				}

                if (seedeintragen) {
                    Seed seed;
                    seed.yworldcoo = aktortyworldcoo;
                    seed.xworldcoo = aktortxworldcoo;
                    seed.xcoo = 1000 * jseed;
                    seed.ycoo = 1000 * iseed;
                    seed.namem = 0;
                    seed.namep = 0;
                    seed.line = ++parameter[0].lineakt;
                    seed.generation = 0;
					// if ((seed.ycoo/ 1000)<=500){
						// seed.origin=1;
						// }
					// else if((seed.xcoo/ 1000) <=200) {
						seed.origin=2;
						// }
					// else if((seed.xcoo/ 1000) >900){
						// seed.origin=3;
						// }
                    seed.incone = false;
                    // seed.weight = 1;
                    seed.age = 0;
                    seed.longdispersed = false;
                    seed.species = rn_species;
                    seed.releaseheight = 0;
                    seed.thawing_depthinfluence = 100;
                    seed.dead = false;
					if (parameter[0].variabletraits==1)
					{
						seed.seedweight=normrand(1,0.5,0.33,1.66);
						seed.droughtresist=normrand(50,20,0,100);
						//seed.seedweight=1;
						//seed.droughtresist=100;
						// seed.seednumber=normrand(1,0.5,0.33,1.66);

						// seed.selving=0;
						seed.selving=normrand(50,20,0,100);

						// vector<unsigned int> copyneutralmarkers(24, 0);
						// seed.neutralmarkers.resize(24);
						// generate(seed.neutralmarkers.begin(),seed.neutralmarkers.end(), uniformneutral.draw());
						for(unsigned int i=0; i < 24; i++) {
											// seed.neutralmarkers[i] = uniformneutral.draw();
											seed.neutralmarkers[i] = uniform.draw()*999999;
										 }
						// seed.neutralmarkers = copyneutralmarkers;
					}
					else
					{
						seed.seedweight=1;
						seed.droughtresist=100;
						// seed.seednumber=1;

						seed.selving=0;

					}
					seed.currentweight=seed.seedweight;
					if(seed.neutralmarkers.size() == 24 & seed.neutralmarkers[0] != 999999+1) { // filled vector and not the initialization value
						#pragma omp critical
						{
							seed_list.add_directly(std::move(seed));
							// seed_list.add(std::move(seed));
						}
					} else {
						cout << " not enough markers " << endl;
					}
                }
            }
            seed_list.consolidate();
        }
    }
}

void TreesIni(int maximal_word_length) {
    FILE* f;
    f = NULL;

    // set up of initial trees from different files
    if (parameter[0].starttrees != 0) {
        int aktort = 0;
        for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); posw++) {
            VectorList<Tree>& tree_list = *posw;
            aktort++;
            int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
             int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

            if (parameter[0].starttrees == 12) {
                f = fopen("input/CH17I_Treevert2011.csv", "r");
                printf("load: input/CH17I_Treevert2011.csv");
            } else if (parameter[0].starttrees == 120100) {
                f = fopen("input/CH17I_Treevert2011_100_100.csv", "r");
                printf("load: input/CH17I_Treevert2011_100_100.csv");
            } else if (parameter[0].starttrees == 120500) {
                f = fopen("input/CH17I_Treevert2011_500_500.csv", "r");
                printf("load: input/CH17I_Treevert2011_500_500.csv");
            } else if (parameter[0].starttrees == 121000) {
                f = fopen("input/CH17I_Treevert2011_1000_1000.csv", "r");
                printf("load: input/CH17I_Treevert2011_1000_1000.csv");
            } else if (parameter[0].starttrees == 120100050) {
                f = fopen("input/CH17I_Treevert2011_100_50000.csv", "r");
                printf("load: input/CH17I_Treevert2011_100_50000.csv");
            } else if (parameter[0].starttrees == 12345) {
                f = fopen("input/testingbothmiddle.csv", "r");
                printf("load: input/testingbothmiddle.csv");
            }

            if (f == NULL) {
                printf("Tree input file not available!\n");
                exit(1);
            }

            char puffer[25555];
            int counter = 1;

            double ybuffer, ycoobuf, xbuffer, xcoobuf;
            int conebuf, agebuf, originbuf;
            short unsigned int inbreedingbuf1, inbreedingbuf2,inbreedingbuf3,inbreedingbuf4,inbreedingbuf5,inbreedingbuf6,inbreedingbuf7,inbreedingbuf8,inbreedingbuf9,inbreedingbuf10,inbreedingbuf11,inbreedingbuf12,inbreedingbuf13,inbreedingbuf14,inbreedingbuf15,inbreedingbuf16,inbreedingbuf17,inbreedingbuf18,inbreedingbuf19,inbreedingbuf20,inbreedingbuf21,inbreedingbuf22,inbreedingbuf23,inbreedingbuf24;
            double heightbuf, dbasalbuf, dbreastbuf, seedweightbuf, droughtbuf, selvingbuf, inbreedingbuf;
			RandomNumber<double> uniform(0, 1);
			// RandomNumber<double> uniformneutral(0, 999999);

            // ignoring the header the contents are appended to the tree array line by line
            while (fgets(puffer, 255555, f) != NULL) {
                if (counter >= 2) {
                    strtok(puffer, ";");
                    sscanf(strtok(NULL, ";"), "%lg", &ybuffer);
                    ycoobuf = ybuffer;
                    sscanf(strtok(NULL, ";"), "%lg", &xbuffer);
                    xcoobuf = xbuffer;
                    heightbuf = strtod(strtok(NULL, ";"), NULL);
                    dbasalbuf = strtod(strtok(NULL, ";"), NULL);
                    dbreastbuf = strtod(strtok(NULL, ";"), NULL);
                    sscanf(strtok(NULL, ";"), "%d", &conebuf);
                    sscanf(strtok(NULL, ";"), "%d", &agebuf);
                    sscanf(strtok(NULL, ";"), "%d", &originbuf);
                    sscanf(strtok(NULL, ";"), "%lg", &seedweightbuf);
                    sscanf(strtok(NULL, ";"), "%lg", &droughtbuf);
                    sscanf(strtok(NULL, ";"), "%lg", &selvingbuf);
                    sscanf(strtok(NULL, ";"), "%lg", &inbreedingbuf);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf1);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf2);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf3);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf4);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf5);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf6);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf7);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf8);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf9);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf10);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf11);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf12);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf13);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf14);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf15);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf16);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf17);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf18);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf19);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf20);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf21);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf22);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf23);
                    sscanf(strtok(NULL, ";"), "%hu", &inbreedingbuf24);




                    Tree tree;

                    tree.yworldcoo = aktortyworldcoo;
                    tree.xworldcoo = aktortxworldcoo;
                    tree.xcoo = 1000 * xcoobuf;
                    tree.ycoo = 1000 * ycoobuf;
                    tree.name = ++parameter[0].nameakt;
                    tree.namem = 0;
                     tree.namep = 0;
                    tree.line = ++parameter[0].lineakt;
                    tree.generation = 0;
                    // tree.yr_of_establishment = 0;
                    tree.dbasal = dbasalbuf;
                    tree.dbasalrel = 1000;
                    tree.dbreast = dbreastbuf;
                    tree.dbreastrel = 1000;
                    tree.height = 10 * heightbuf;
                    tree.age = agebuf;
                    tree.cone = conebuf;
                    tree.origin=originbuf;
                    tree.seedweight=seedweightbuf;
                    tree.droughtresist=droughtbuf;
                    tree.selving=selvingbuf;
                    tree.inbreedingdepression=inbreedingbuf;
                        tree.neutralmarkers[0] = inbreedingbuf1;
                        tree.neutralmarkers[1] = inbreedingbuf2;
                        tree.neutralmarkers[2] = inbreedingbuf3;
                        tree.neutralmarkers[3] = inbreedingbuf4;
                        tree.neutralmarkers[4] = inbreedingbuf5;
                        tree.neutralmarkers[5] = inbreedingbuf6;
                        tree.neutralmarkers[6] = inbreedingbuf7;
                        tree.neutralmarkers[7] = inbreedingbuf8;
                        tree.neutralmarkers[8] = inbreedingbuf9;
                        tree.neutralmarkers[9] = inbreedingbuf10;
                        tree.neutralmarkers[10] = inbreedingbuf11;
                        tree.neutralmarkers[11] = inbreedingbuf12;
                        tree.neutralmarkers[12] = inbreedingbuf13;
                        tree.neutralmarkers[13] = inbreedingbuf14;
                        tree.neutralmarkers[14] = inbreedingbuf15;
                        tree.neutralmarkers[15] = inbreedingbuf16;
                        tree.neutralmarkers[16] = inbreedingbuf17;
                        tree.neutralmarkers[17] = inbreedingbuf18;
                        tree.neutralmarkers[18] = inbreedingbuf19;
                        tree.neutralmarkers[19] = inbreedingbuf20;
                        tree.neutralmarkers[20] = inbreedingbuf21;
                        tree.neutralmarkers[21] = inbreedingbuf22;
                        tree.neutralmarkers[22] = inbreedingbuf23;
                        tree.neutralmarkers[23] = inbreedingbuf24;

                    if (tree.cone == true) {
                        tree.coneheight = 65535;
                    }
                    tree.seednewly_produced = 0;
                    // tree.seedproduced = 0;
                    // tree.buffer = 1;
                    tree.densitywert = 0;
                    tree.thawing_depthinfluence = 100;
                    tree.growing = true;
					if (parameter[0].specpres == 0) {
						tree.species = (int) 1 + (uniform.draw() * (parameter[0].species_max-1));
					} else {
						tree.species = parameter[0].specpres;
					}
					if (parameter[0].variabletraits==1)
					{
						//tree.seedweight=normrand(1,0.5,0.33,1.66);
						//tree.droughtresist=normrand(50,20,0,100);
						//tree.seedweight=1;
						//tree.droughtresist=100;
						// tree.seednumber=normrand(1,0.5,0.33,1.66);


							//		tree.selving=0;
									//tree.selving=normrand(50,20,0,100);

									//double ranc = uniform.draw();
									//if (ranc*100<=tree.clonality && tree.clonality!=0){
									//tree.cloning = true;
									//} else {
									tree.cloning = false;
									//}
									tree.clonetimer=0;
									tree.cloned=false;
									tree.cloneboost=1;

									// vector<unsigned int> copyneutralmarkers(24, 0);
									 // generate(copyneutralmarkers.begin(),copyneutralmarkers.end(), uniformneutral);
									// tree.neutralmarkers=copyneutralmarkers;
									// generate(tree.neutralmarkers.begin(),tree.neutralmarkers.end(), uniformneutral.draw());
									//for(unsigned int i=0; i < 24; i++) {
										//	tree.neutralmarkers[i] = uniformneutral.draw();
										// }
								//	tree.inbreedingdepression=0;
						//for( unsigned int neutralcounter=0; neutralcounter<=tree.neutralmarkers.size();neutralcounter+=2){
							//if (tree.neutralmarkers[neutralcounter]==tree.neutralmarkers[neutralcounter+1]){
								//tree.inbreedingdepression=tree.inbreedingdepression+(100/(tree.neutralmarkers.size()/2));
							//}
						//}
					}
					else
					{
						tree.seedweight=1;
						tree.droughtresist=100;
						// tree.seednumber=1;

						tree.selving=0;

					}

					#pragma omp critical
					{
						tree_list.add_directly(std::move(tree));
					}
                }
                counter++;
            }
            tree_list.consolidate();

            fclose(f);
        }
    }
}

void Hinterlandseedintro(Parameter* parameter, int yearposition, vector<VectorList<Seed>>& world_seed_list, vector<vector<Weather>>& world_weather_list) {
    RandomNumber<double> uniform(0, 1);
	// RandomNumber<double> uniformneutral(0, 999999);
    // function parameters
    double logmodel_seeds_Po = 7.8997307;
    double logmodel_seeds_r = -0.6616466;
    double logmodel_seeds_K = 2700.0081918;

    double logmodel_heights_Po = 6.0897390;
    double logmodel_heights_r = -0.5932627;
    double logmodel_heights_K = 353.5688136;

    // set 'trees' directly as seeds with the relevant information
    // ... for each tree the mean number of seeds and height at the position at the hinterland
    // ... jultemp based on position
    int aktort = 0;
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;

        // grant weather access
        vector<vector<Weather>>::iterator world_positon_weather = (world_weather_list.begin() + aktort);
        vector<Weather>& weather_list = *world_positon_weather;
        aktort++;
        int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
        int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

#ifdef DEBUG
        cout << " ... seed_list.size=" << seed_list.size() << endl;
#endif

        if (parameter[0].hinterland_maxlength > 0) {
#ifdef DEBUG
            // debugging output
            int introduced = 0;
            int exceedstoN = 0;
            int exceedstoE = 0;
            int exceedstoS = 0;
            int exceedstoW = 0;
#endif

            // hinterland_maxlength is cut into 20 m-long pieces, start at 10 with steps of by 20 to determine the centre for each seed introduction nuclei
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
			for (int yposhint = -10; yposhint > -1 * parameter[0].hinterland_maxlength; yposhint = yposhint - 20) {
                double jultempi =
                    weather_list[yearposition].temp7monthmean + (-0.3508 * yposhint / (111120));  // conversion to degree latitude see...weatherinput.cpp

                double hinterheightsi = logmodel_heights_K / (1 + exp(logmodel_heights_Po + logmodel_heights_r * jultempi));
                int hinterseedsi = (parameter[0].seedflightrate * logmodel_seeds_K
                                    / (1 + exp(logmodel_seeds_Po + logmodel_seeds_r * jultempi))  // determine number of seeds produced at this nucleus
                                    )
                                   * (double)treecols / 20;  // scaling to the witdth of the simulated area
                for (int n = 0; n < hinterseedsi; n++) {
                    // calculate post-dispersal position with the parameters set for the simulation run using the wind-dependent seed dispersal function

                    // x y start random in field
                    double xseed, yseed;
                    bool introduceseed = true;

                    xseed = (treecols - 1) * uniform.draw();        // x coo start
                    yseed = (yposhint - 10) + 20 * uniform.draw();  // y coo start

                    // define species
					int rn_species = 0;
					// multiple species implementation
					if (parameter[0].specpres == 0) {
						rn_species = (int) 1 + (uniform.draw() * (parameter[0].species_max-1));
					} else {
						rn_species = parameter[0].specpres;
					}

                    // estimation of new positions
                    double ratiorn = uniform.draw();
                    // double dispersaldistance = 0.0;
                    double velocity = 0.0;
                    double wdirection = 0.0;
                    double jquer = 0;
                    double iquer = 0;

                    double randomnumberwind = uniform.draw();
                    Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, hinterheightsi, rn_species, randomnumberwind);

                    xseed = xseed + jquer;
                    yseed = yseed + iquer;

                    // manipulate position according to boundary conditions if lands in simulated plot add to seedlist ...code adapted from seeddispersal.cpp
                    if (yseed > (double)(treerows - 1)) {
                        if ((parameter[0].boundaryconditions == 1)) {
                            yseed = fmod(yseed, (double)(treerows - 1));
                        } else if ((parameter[0].boundaryconditions == 3)) {
#ifdef DEBUG
                            exceedstoN++;
#endif
                            introduceseed = false;
                        } else {
#ifdef DEBUG
                            exceedstoN++;
#endif
                            introduceseed = false;
                        }
                    } else if (yseed < 0.0) {
                        if ((parameter[0].boundaryconditions == 1)) {
                            yseed = (double)(treerows - 1) + fmod(yseed, (double)(treerows - 1));
                        } else if ((parameter[0].boundaryconditions == 3)) {
#ifdef DEBUG
                            exceedstoS++;
#endif
                            introduceseed = false;
                        } else {
#ifdef DEBUG
                            exceedstoS++;
#endif
                            introduceseed = false;
                        }
                    }

                    if (xseed < 0.0) {
                        if ((parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3)) {
                            xseed = fmod(xseed, (double)(treecols - 1)) + (double)(treecols - 1);
                        } else {
#ifdef DEBUG
                            exceedstoW++;
#endif
                            introduceseed = false;
                        }
                    } else if (xseed > (double)(treecols - 1)) {
                        if (parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3) {
                            xseed = fmod(xseed, (double)(treecols - 1));
                        } else if ((parameter[0].boundaryconditions == 2) && (uniform.draw() < 0.5)) {  // Reducing seed introduction on the western border:
                            xseed = fmod(xseed, (double)(treecols - 1));
                        } else {
#ifdef DEBUG
                            exceedstoE++;
#endif
                            introduceseed = false;
                        }
                    }

                    // add new seed to seedlist
                    if (introduceseed == true) {
#ifdef DEBUG
                        introduced++;
#endif

                        Seed seed;
                        seed.yworldcoo = aktortyworldcoo;
                        seed.xworldcoo = aktortxworldcoo;
                        seed.xcoo = 1000 * xseed;
                        seed.ycoo = 1000 * yseed;
                        seed.namem = 0;
                        seed.namep = 0;
                        seed.line = ++parameter[0].lineakt;
                        seed.generation = 0;

						seed.origin=1;

                        seed.incone = false;
                        // seed.weight = 1;
                        seed.age = 0;
                        seed.longdispersed = false;
                        seed.species = rn_species;
                        seed.releaseheight = 10 * hinterheightsi;
                        seed.thawing_depthinfluence = 100;
                        // seed.dispersaldistance = dispersaldistance;
                        seed.dead = false;
						if (parameter[0].variabletraits==1)
						{
						seed.seedweight=normrand(1,0.5,0.33,1.66);
						seed.droughtresist=normrand(50,20,0,100);
						//seed.seedweight=1;
						//seed.droughtresist=100;
						// seed.seednumber=normrand(1,0.5,0.33,1.66);

						//seed.selving=0;
						seed.selving=normrand(50,20,0,100);


						// vector<unsigned int> copyneutralmarkers(24, 0);
						 // generate(copyneutralmarkers.begin(),copyneutralmarkers.end(), uniformneutral);
						// seed.neutralmarkers=copyneutralmarkers;
						// generate(seed.neutralmarkers.begin(),seed.neutralmarkers.end(), uniformneutral.draw());
						for(unsigned int i=0; i < 24; i++) {
											seed.neutralmarkers[i] = uniform.draw()*999999;
										 }
						}
						else
						{
						seed.seedweight=1;
						seed.droughtresist=100;
						// seed.seednumber=1;

									seed.selving=0;

						}
						seed.currentweight=seed.seedweight;

						#pragma omp critical
						{
							seed_list.add_directly(std::move(seed));
							// seed_list.add(std::move(seed));
						}
                    }
                }
            }
            seed_list.consolidate();

#ifdef DEBUG
            cout << "C: introduced seeds=" << introduced << " ----> N=" << exceedstoN << "  E=" << exceedstoE << "  S=" << exceedstoS << "  W=" << exceedstoW
                 << endl;
#endif
        }
    }
}

void Treedistribution(Parameter* parameter, int stringlengthmax) {
    if ((parameter[0].starter == true)) {  // either seed introduction...
        Seedin();
    } else {  // ... or use initial tree input data files
        TreesIni(stringlengthmax);
    }
}
