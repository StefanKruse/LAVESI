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

		speciestrait[species_counter].yearlycalcofanstiegweathermort=(60*weather_list[yearposition_help].janisothermrestriktion[species_counter]+60*weather_list[yearposition_help].julisothermrestriktion+60*weather_list[yearposition_help].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
		speciestrait[species_counter].yearlycalcofanstiegweathermortmin=(60*weather_list[yearposition_help].janisothermrestriktionmin[species_counter]+60*weather_list[yearposition_help].julisothermrestriktion+60*weather_list[yearposition_help].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
	}

    RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

        if (tree.growing == true) {
            double agesmort = 0.0;
			agesmort = tree.age / speciestrait[tree.species].maximumage;
            // if (tree.age > speciestrait[tree.species].maximumage) {  // if maximal age is exceeded an additional factor occurs
                // agesmort = 1.0;
            // }
			
            double windthrowmort = 0.0;
			windthrowmort = speciestrait[tree.species].mwindthrow * ((double)tree.height / 10) / (65535 / 10);// scaled to maximum of height in model of 65 m

			double firecrowndamagemort = (double)tree.relcrowndamage / 1000;
			if(firecrowndamagemort < 0.0)
				firecrowndamagemort = 0.0;
			else if(firecrowndamagemort > 1.0)
				firecrowndamagemort = 1.0 - speciestrait[tree.species].resprouting*speciestrait[tree.species].relbarkthickness;
			tree.relcrowndamage = 0*1000;
// cout << firecrowndamagemort << " ... " << tree.relcrowndamage / 1000 << " | ";
			
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

            double sapl_mort = speciestrait[tree.species].mortyouth * pow(exp((-1.0 * tree.dbasal) + (double)tree.dbasalmax / 1000), speciestrait[tree.species].mortyouthinfluenceexp);
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
                                                 * (weather_list[yearposition_help].weatherfactor[tree.species]
                                                    + ((weather_list[yearposition_help].weatherfactormin[tree.species] - weather_list[yearposition_help].weatherfactor[tree.species])
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
                                   * ((weather_list[yearposition_help].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                      + (weather_list[yearposition_help].weatherfactormin[tree.species]
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
                                           * exp(speciestrait[tree.species].yearlycalcofanstiegweathermort * weather_list[yearposition_help].weatherfactor[tree.species]
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
                           * (weather_list[yearposition_help].droughtmort
                              + ((weather_list[yearposition_help].droughtmortmin - weather_list[yearposition_help].droughtmort) * ((double)tree.ycoo / 1000)
                                 / ((double)treerows)))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else if (parameter[0].demlandscape) {
                dry_mort = speciestrait[tree.species].mdrought
                           * ((weather_list[yearposition_help].droughtmort * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                               / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                              + (weather_list[yearposition_help].droughtmortmin
                                 * (1
                                    - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                          / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else {
                dry_mort = speciestrait[tree.species].mdrought * weather_list[yearposition_help].droughtmort * pow((1.0 / (double)tree.height / 10), 0.5);
            }

            // calculating the mortality rate of the tree considering the factors of each mortality rate
            double treemortality = 0.0 + speciestrait[tree.species].mortbg + sapl_mort + age_mort + growth_mort + dens_mort + weather_mort + dry_mort + windthrowmort + firecrowndamagemort;
			treemortality = treemortality + (((double)tree.firedamage / 3) * (1 / ((double)tree.height / 100) / 150)); //fire damage from before merging -> 10 cm tree firemort (fm) = fm*15, 100 cm = fm*1.5, 200 cm = fm*0.75
			//Treemortg = Treemortg + (((double)tree.firedamage / 3) * (1 / ((double)tree.height / 100) / 150)); //fire damage from before merging -> 10 cm tree firemort (fm) = fm*15, 100 cm = fm*1.5, 200 cm = fm*0.75

            if (treemortality > 1.0) {
                treemortality = 1.0;
            } else if (treemortality < 0.0) {
                treemortality = 0.0;
            }

// if((double)tree.height / 10 > 10)
		// cout << (double)tree.height / 10 << " ..... " << treemortality << " <<< " << wachstumrel << " + " << tree.dbasal << " - " << tree.dbasalrel<< "..." << tree.dbreast << " - " << tree.dbreastrel << " | " << sapl_mort  << " - " <<   age_mort  << " - " <<   growth_mort  << " - " <<   dens_mort  << " - " <<   weather_mort  << " - " <<   dry_mort <<  " - " << tree.soilhumidity << endl;

            // determine if a tree dies
			if ( ((double) uniform.draw() < treemortality) || (tree.envirimpact <= 0) ) {
                tree.growing = false;
				// if (tree.firedamage > 0)
					// cout << "tree_list[tree_i].growing = " << tree_list[tree_i].growing;
                tree_list.remove(tree_i);
                // TODO: alternatively set variables to dead and not growing: negative ages could be used for rotting deadwood
				// if (tree.firedamage > 0)
					// cout << " = after death => tree_list[tree_i].growing = " << tree_list[tree_i].growing << endl;				
			// cout << " ### tree.height= " << (double)tree.height/100 << " | sapl_mort_gmel=" << sapl_mort_gmel << " | age_mort=" << age_mort << " | growth_mort=" << growth_mort << " | dens_mort=" << dens_mort << " | weather_mort_gmel=" << weather_mort_gmel << " | dry_mort=" << dry_mort << " | tree.firedamage=" << (((double)tree.firedamage / 3) * (1 / ((double)tree.height / 100) / 150)) << " | Treemortg=" << Treemortg << endl;
            }
        }
    }
    tree_list.consolidate();
cout << endl;
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
					// if(!seed.incone)
						// cout << "seed on ground!!! " << endl;
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

