#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void TreeMort(int yearposition_help, vector<Weather>& weather_list, VectorList<Tree>& tree_list) {
    // abiotic influence: calculation of the factors in a function that adds a mortality rate impact
	// multiple species integration
	for (int species_counter = 1; species_counter < 99; species_counter++) {
		if(speciestrait[species_counter].number == 0)
			break;

		speciestrait[species_counter].yearlycalcofanstiegweathermort=(60*weather_list[yearposition_help-1].janisothermrestriktion[species_counter]+60*weather_list[yearposition_help-1].julisothermrestriktion+60*weather_list[yearposition_help-1].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
		speciestrait[species_counter].yearlycalcofanstiegweathermortmin=(60*weather_list[yearposition_help-1].janisothermrestriktionmin[species_counter]+60*weather_list[yearposition_help-1].julisothermrestriktion+60*weather_list[yearposition_help-1].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
	}

    RandomNumber<double> uniform(0, 1);
	unsigned int pestdeaths1 = 0;
	unsigned int pestdeaths2 = 0;
	unsigned int pestdeaths3 = 0;
	unsigned int pestdeaths4 = 0;
	unsigned int pestdeaths5 = 0;
	unsigned int pestdeaths6 = 0;
	unsigned int pestdeaths7 = 0;
	unsigned int pestcrowndamage1 = 0;
	unsigned int pestcrowndamage2 = 0;
	unsigned int pestcrowndamage3 = 0;
	unsigned int pestcrowndamage4 = 0;
	unsigned int pestcrowndamage5 = 0;
	unsigned int pestcrowndamage6 = 0;
	unsigned int pestcrowndamage7 = 0;
	unsigned int pestcrowndamagesurvivor1 = 0;
	unsigned int pestcrowndamagesurvivor2 = 0;
	unsigned int pestcrowndamagesurvivor3 = 0;
	unsigned int pestcrowndamagesurvivor4 = 0;
	unsigned int pestcrowndamagesurvivor5 = 0;
	unsigned int pestcrowndamagesurvivor6 = 0;
	unsigned int pestcrowndamagesurvivor7 = 0;

#pragma omp parallel for default(shared) private(uniform) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

        if (tree.growing == true) {
            double agesmort = 0.0;
			agesmort = tree.age / speciestrait[tree.species].maximumage;
			
            double windthrowmort = 0.0;
			windthrowmort = speciestrait[tree.species].mwindthrow * ((double)tree.height / 10) / (65535 / 10);// scaled to maximum of height in model of 65 m
			
			double firecrowndamagemort = 0.0;
			if ( (parameter[0].firemode == 112) || (parameter[0].fireintensitymode != 1.0) || (parameter[0].allow_pest_disturbances) ) {
				firecrowndamagemort = (double)tree.relcrowndamage / 1000;
				if(firecrowndamagemort < 0.0)
					firecrowndamagemort = 0.0;
				else if(firecrowndamagemort >= 1.0)
					firecrowndamagemort = 1.0 - speciestrait[tree.species].resprouting*speciestrait[tree.species].relbarkthickness;
			}

			double pestoutbreakmort = 0.0;
			if( parameter[0].allow_pest_disturbances ) {
				pestoutbreakmort = (double)tree.pestinfectancedamage / 1000;
				if(pestoutbreakmort < 0.0)
					pestoutbreakmort = 0.0;
				else if(pestoutbreakmort >= 1.0)
					pestoutbreakmort = 1.0;
				tree.pestinfectancedamage = 0*1000; // reset value
			}
		
            // height dependent influences
            double wachstumrel = 1.0;
            if ((double)tree.height / 10 < 130) {
                wachstumrel = (double)tree.dbasalrel / 1000;
            } else {
                wachstumrel = (double)tree.dbreastrel / 1000;
            }

            // extra competition effect
            double heightnkugeleinfluss = 1;
            if ((double)tree.height / 10 < (speciestrait[tree.species].densityvaluemaximumatheight * 2)) {
                heightnkugeleinfluss =
                    heightnkugeleinfluss
                    + (sqrt(pow(speciestrait[tree.species].densityvaluemaximumatheight, 2) - pow((double)tree.height / 10 - speciestrait[tree.species].densityvaluemaximumatheight, 2))
                       / speciestrait[tree.species].densityvaluemaximumatheight);
            }

			double sapl_mort_factor = 0.3948;
            double sapl_mort = (speciestrait[tree.species].mortyouth-sapl_mort_factor) * pow(exp((-1.0 * tree.dbasal) + (double)tree.dbasalmax / 1000), speciestrait[tree.species].mortyouthinfluenceexp);
            double age_mort = speciestrait[tree.species].mortage * agesmort * (10.0 * speciestrait[tree.species].mortbg);
            double growth_mort = speciestrait[tree.species].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
            double dens_mort = speciestrait[tree.species].mdensity * heightnkugeleinfluss * tree.densitywert;

            double weathermortadd;
            if (parameter[0].lineartransect) {
                weathermortadd = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp((speciestrait[tree.species].yearlycalcofanstiegweathermort
                                                  - ((speciestrait[tree.species].yearlycalcofanstiegweathermort - speciestrait[tree.species].yearlycalcofanstiegweathermort) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                                 * (weather_list[yearposition_help-1].weatherfactor[tree.species]
                                                    + ((weather_list[yearposition_help-1].weatherfactormin[tree.species] - weather_list[yearposition_help-1].weatherfactor[tree.species])
                                                       * ((double)tree.ycoo / 1000) / ((double)treerows)))
                                                 * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                                       + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            } else if (parameter[0].demlandscape) {
                weathermortadd =
                    1.0
                    - (1.0
                       / (1.0
                          + (((1.0 - 0.5) / 0.5)
                             * exp((speciestrait[tree.species].yearlycalcofanstiegweathermort
                                    - ((speciestrait[tree.species].yearlycalcofanstiegweathermort - speciestrait[tree.species].yearlycalcofanstiegweathermort) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                   * ((weather_list[yearposition_help-1].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                      + (weather_list[yearposition_help-1].weatherfactormin[tree.species]
                                         * (1
                                            - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                   * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                         + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            } else {
                weathermortadd = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp(speciestrait[tree.species].yearlycalcofanstiegweathermort * weather_list[yearposition_help-1].weatherfactor[tree.species]
                                                 * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                                       + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            }

            double heightreduce = pow((1.0 / (double)tree.height / 10), speciestrait[tree.species].heightweathermorteinflussexp);  // includes a minimun limit
            if (heightreduce < 0.001)
                heightreduce = 0.001;
            double weather_mort = speciestrait[tree.species].mweather * weathermortadd * heightreduce;

            double dry_mort = 0.0;
            if (parameter[0].lineartransect) {
                dry_mort = speciestrait[tree.species].mdrought
                           * (weather_list[yearposition_help-1].droughtmort
                              + ((weather_list[yearposition_help-1].droughtmortmin - weather_list[yearposition_help-1].droughtmort) * ((double)tree.ycoo / 1000)
                                 / ((double)treerows)))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else if (parameter[0].demlandscape) {
                dry_mort = speciestrait[tree.species].mdrought
                           * ((weather_list[yearposition_help-1].droughtmort * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                               / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                              + (weather_list[yearposition_help-1].droughtmortmin
                                 * (1
                                    - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                          / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else {
                dry_mort = speciestrait[tree.species].mdrought * weather_list[yearposition_help-1].droughtmort * pow((1.0 / (double)tree.height / 10), 0.5);
            }

            // calculating the mortality rate of the tree considering the factors of each mortality rate
            double treemortality = 0.0 + speciestrait[tree.species].mortbg + sapl_mort + age_mort + growth_mort + dens_mort + weather_mort + dry_mort + windthrowmort + firecrowndamagemort + pestoutbreakmort;
			
			// adding firedamage from cur_plot.fire (fire intensity), mediated by tree traits (only if firemode == 112)
			if (parameter[0].firemode == 112 || parameter[0].fireintensitymode != 1.0) {
			treemortality = treemortality + (double)tree.firedamage * pow((100*1/(double)tree.height), 0.5*0.5);
			
			} else if ((parameter[0].firemode != 0) & (parameter[0].firemode != 112)) {
				treemortality = treemortality + (double)tree.firedamage;
			} else if (parameter[0].firemode == 0) {
				treemortality = treemortality;
			}

            if (treemortality > 1.0) {
                treemortality = 1.0;
            } else if (treemortality < 0.0) {
                treemortality = 0.0;
            }

            // determine if a tree dies
			if ( ((double) uniform.draw() < treemortality) || (tree.envirimpact <= 0) ) {
				// add output for pest outbreak 
				if(tree.pestinfection > 0) {
#pragma omp critical
{
					if(tree.pestinfection ==        1) {pestdeaths1++;if(tree.relcrowndamage > 0) {pestcrowndamage1++;}}
					if(tree.pestinfection ==       10) {pestdeaths2++;if(tree.relcrowndamage > 0) {pestcrowndamage2++;}}
					if(tree.pestinfection ==      100) {pestdeaths3++;if(tree.relcrowndamage > 0) {pestcrowndamage3++;}}
					if(tree.pestinfection ==     1000) {pestdeaths4++;if(tree.relcrowndamage > 0) {pestcrowndamage4++;}}
					if(tree.pestinfection ==    10000) {pestdeaths5++;if(tree.relcrowndamage > 0) {pestcrowndamage5++;}}
					if(tree.pestinfection ==   100000) {pestdeaths6++;if(tree.relcrowndamage > 0) {pestcrowndamage6++;}}
					if(tree.pestinfection ==  1000000) {pestdeaths7++;if(tree.relcrowndamage > 0) {pestcrowndamage7++;}}
}
				}
                tree.growing = false;
                tree_list.remove(tree_i);		
            } else {
				if(tree.pestinfection > 0) {// count not dead but damaged trees
#pragma omp critical
{
					if(tree.pestinfection ==        1) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor1++;}}
					if(tree.pestinfection ==       10) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor2++;}}
					if(tree.pestinfection ==      100) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor3++;}}
					if(tree.pestinfection ==     1000) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor4++;}}
					if(tree.pestinfection ==    10000) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor5++;}}
					if(tree.pestinfection ==   100000) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor6++;}}
					if(tree.pestinfection ==  1000000) {if(tree.relcrowndamage > 0) {pestcrowndamagesurvivor7++;}}
}
				}
				tree.relcrowndamage = 0*1000;
				tree.pestinfection = 0;								 
            }
        }
    }
    tree_list.consolidate();
	
	// output mortality by pest outbreaks
		string filename;
		filename = "output/pestoutbreaks_mortality.csv";
		// trying to open the file for reading
		FILE* filepointer;
		filepointer = fopen(filename.c_str(), "r+");
		// if fopen fails, open a new file + header output
		if (filepointer == NULL) {
			filepointer = fopen(filename.c_str(), "w+");

			fprintf(filepointer, "Progress;");
			fprintf(filepointer, "Yearposition;");
			fprintf(filepointer, "Pest1deaths;");
			fprintf(filepointer, "Pest2deaths;");
			fprintf(filepointer, "Pest3deaths;");
			fprintf(filepointer, "Pest4deaths;");
			fprintf(filepointer, "Pest5deaths;");
			fprintf(filepointer, "Pest6deaths;");
			fprintf(filepointer, "Pest7deaths;");
			fprintf(filepointer, "Pest1defoliated;");
			fprintf(filepointer, "Pest2defoliated;");
			fprintf(filepointer, "Pest3defoliated;");
			fprintf(filepointer, "Pest4defoliated;");
			fprintf(filepointer, "Pest5defoliated;");
			fprintf(filepointer, "Pest6defoliated;");
			fprintf(filepointer, "Pest7defoliated;");
			fprintf(filepointer, "Pest1defoliatedSurvivor;");
			fprintf(filepointer, "Pest2defoliatedSurvivor;");
			fprintf(filepointer, "Pest3defoliatedSurvivor;");
			fprintf(filepointer, "Pest4defoliatedSurvivor;");
			fprintf(filepointer, "Pest5defoliatedSurvivor;");
			fprintf(filepointer, "Pest6defoliatedSurvivor;");
			fprintf(filepointer, "Pest7defoliatedSurvivor;");
			fprintf(filepointer, "\n");

			if (filepointer == NULL) {
				fprintf(stderr, "Error: output file for pest outbreaks is missing!\n");
				exit(1);
			}
		}

		// data utput		
		fseek(filepointer, 0, SEEK_END);
			fprintf(filepointer, "%d;", parameter[0].ivort);
			fprintf(filepointer, "%d;", yearposition_help-1);
			fprintf(filepointer, "%d;", pestdeaths1);
			fprintf(filepointer, "%d;", pestdeaths2);
			fprintf(filepointer, "%d;", pestdeaths3);
			fprintf(filepointer, "%d;", pestdeaths4);
			fprintf(filepointer, "%d;", pestdeaths5);
			fprintf(filepointer, "%d;", pestdeaths6);
			fprintf(filepointer, "%d;", pestdeaths7);
			fprintf(filepointer, "%d;", pestcrowndamage1);
			fprintf(filepointer, "%d;", pestcrowndamage2);
			fprintf(filepointer, "%d;", pestcrowndamage3);
			fprintf(filepointer, "%d;", pestcrowndamage4);
			fprintf(filepointer, "%d;", pestcrowndamage5);
			fprintf(filepointer, "%d;", pestcrowndamage6);
			fprintf(filepointer, "%d;", pestcrowndamage7);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor1);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor2);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor3);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor4);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor5);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor6);
			fprintf(filepointer, "%d;", pestcrowndamagesurvivor7);
			fprintf(filepointer, "\n");

		fclose(filepointer);
}

void Mortality(Parameter* parameter,
               // int Jahr,
               int yearposition,
               vector<VectorList<Tree>>& world_tree_list,
               vector<VectorList<Seed>>& world_seed_list,
               vector<vector<Weather>>& world_weather_list) {
    int aktort = 0;

    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather>& weather_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
        aktort++;

#pragma omp parallel default(shared)
        {
            RandomNumber<double> uniform(0, 1);
            double direction = 0.0;
            double velocity = 0.0;
            unsigned int ripm = 0;//, cntr = 0;
            double p = 0.0, kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2), phi = 0.0, dr = 0.0, dx = 0.0, dy = 0.0;
            double I0kappa = 0.0;
            double pe = 0.01;
            double C = parameter[0].pollengregoryc;
            double m = parameter[0].pollengregorym;
            vector<int> Vname;
            vector<double> Vthdpth;
            int n_trees = 0;

            // mortality of seeds
#pragma omp for schedule(guided)
            for (unsigned int i = 0; i < seed_list.size(); ++i) {
                auto& seed = seed_list[i];
                if (!seed.dead) {
                    if (uniform.draw() < (seed.incone ? speciestrait[seed.species].seedconemort : speciestrait[seed.species].seedfloormort)) {
                        seed.dead = true;
                        seed_list.remove(i);
                    } else {
					// if (!seed.incone)
					// cout << " seed survived on ground !!! " << endl;
					}
                }
            }

#pragma omp for schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					++n_trees;  // for calculating mean of computation times // TODO still necessary

					if (tree.seednewly_produced > 0) {
						bool pollinated = false;
						for (int sna = 0; sna < tree.seednewly_produced; sna++) {
							if (uniform.draw() >= speciestrait[tree.species].seedconemort) {
								if (!pollinated && ((parameter[0].pollination == 1 && parameter[0].ivort > 1045) || (parameter[0].pollination == 9))) {
									double randomnumberwind = uniform.draw();
									double randomnumberwindfather = uniform.draw();
									Pollinationprobability((double)tree.xcoo / 1000, 
														   (double)tree.ycoo / 1000, 
														   &parameter[0], 
														   world_positon_b, 
														   direction, 
														   velocity,
														   ripm, 
														   // cntr, 
														   p, 
														   kappa, 
														   phi, 
														   dr, 
														   dx, 
														   dy, 
														   I0kappa, 
														   pe, 
														   C, 
														   m, 
														   Vname, 
														   Vthdpth, 
														   n_trees, 
														   randomnumberwind,
														   randomnumberwindfather);
									pollinated = true;
								}

								Seed seed;

								// seed.yworldcoo = aktortyworldcoo;
								// seed.xworldcoo = aktortxworldcoo;
								seed.xcoo = tree.xcoo;
								seed.ycoo = tree.ycoo;
								// seed.namem = tree.name;
								seed.dead = false;

								// if chosen, determine the father by pollination out of available (matured) trees
								if ((Vname.size() > 0) && (parameter[0].pollination == 1 || parameter[0].pollination == 9)) {
									// int iran = uniform() * (Vname.size() - 1);
									// seed.namep = Vname.at(iran);
									// TODO: add here properties that are passed down from the father tree
									seed.thawing_depthinfluence = 100;
								} else {
									// seed.namep = 0;
									seed.thawing_depthinfluence = 100;
								}

								// seed.line = tree.line;
								// seed.generation = tree.generation + 1;
								seed.incone = true;
								// seed.weight = 1;
								seed.age = 0;
								seed.species = tree.species;
								seed.releaseheight = tree.height;

								seed_list.add(std::move(seed));
							}
						}
					}
				}
            }  // main tree loop on each core
        }      // parallel region
        seed_list.consolidate();

        TreeMort(yearposition, weather_list, tree_list);
    }
}

