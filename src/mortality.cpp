#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void TreeMort(int yearposition_help, vector<Weather>& weather_list, VectorList<Tree>& tree_list) {
    // abiotic influence: calculation of the factors in a function that adds a mortality rate impact
    double anstiegweathermortg = 160;
    double anstiegweathermorts = 160;
    double anstiegweathermortgmin = 160;
    double anstiegweathermortsmin = 160;

    anstiegweathermortg = (60 * weather_list[yearposition_help].janisothermrestriktiong + 60 * weather_list[yearposition_help].julisothermrestriktion
                           + 60 * weather_list[yearposition_help].nddrestriktion);
    anstiegweathermorts = (60 * weather_list[yearposition_help].janisothermrestriktions + 60 * weather_list[yearposition_help].julisothermrestriktion
                           + 60 * weather_list[yearposition_help].nddrestriktion);

    anstiegweathermortgmin = (60 * weather_list[yearposition_help].janisothermrestriktiongmin + 60 * weather_list[yearposition_help].julisothermrestriktionmin
                              + 60 * weather_list[yearposition_help].nddrestriktionmin);
    anstiegweathermortsmin = (60 * weather_list[yearposition_help].janisothermrestriktionsmin + 60 * weather_list[yearposition_help].julisothermrestriktionmin
                              + 60 * weather_list[yearposition_help].nddrestriktionmin);

    RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

        if (tree.growing == true) {
            double agesmort = 0.0;
            if (tree.age > parameter[0].maximumage) {  // if maximal age is exceeded an additional factor occurs
                agesmort = 1.0;
            }

            // height dependent influences
            double wachstumrel = 1.0;
            if ((double)tree.height / 100 < 130) {
                wachstumrel = (double)tree.dbasalrel / 1000;
            } else {
                wachstumrel = (double)tree.dbreastrel / 1000;
            }

            // extra competition effect
            double heightnkugeleinfluss = 1;
            if ((double)tree.height / 100 < (parameter[0].densityvaluemaximumatheight * 2)) {
                heightnkugeleinfluss =
                    heightnkugeleinfluss
                    + (sqrt(pow(parameter[0].densityvaluemaximumatheight, 2) - pow((double)tree.height / 100 - parameter[0].densityvaluemaximumatheight, 2))
                       / parameter[0].densityvaluemaximumatheight);
            }

            // TODO: merge gmel/sib sapl_mort calculations and add functionality for multiple species representation
            double sapl_mort_gmel = parameter[0].mortyouth * pow(exp((-1.0 * tree.dbasal) + (double)tree.dbasalmax / 1000), parameter[0].mortyouthinfluenceexp);
            double sapl_mort_sib = parameter[0].mortyouth * pow(exp((-1.0 * tree.dbasal) + (double)tree.dbasalmax / 1000), parameter[0].mortyouthinfluenceexp);
            double age_mort = parameter[0].mortage * agesmort * (10.0 * parameter[0].mortbg);
            double growth_mort = parameter[0].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
            double dens_mort = parameter[0].mdensity * heightnkugeleinfluss * tree.densitywert;

            double weathermortaddg;
            double weathermortadds;
            if (parameter[0].lineartransect) {
                weathermortaddg = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp((anstiegweathermortg
                                                  - ((anstiegweathermortg - anstiegweathermortgmin) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                                 * (weather_list[yearposition_help].weatherfactorg
                                                    + ((weather_list[yearposition_help].weatherfactorming - weather_list[yearposition_help].weatherfactorg)
                                                       * ((double)tree.ycoo / 1000) / ((double)treerows)))
                                                 * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                                                       + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)))));
                weathermortadds = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp((anstiegweathermorts
                                                  - ((anstiegweathermorts - anstiegweathermortsmin) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                                 * (weather_list[yearposition_help].weatherfactors
                                                    + ((weather_list[yearposition_help].weatherfactormins - weather_list[yearposition_help].weatherfactors)
                                                       * ((double)tree.ycoo / 1000) / ((double)treerows)))  // scaled effect along transect
                                                 * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal
                                                       + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)))));
            } else if (parameter[0].demlandscape) {
                weathermortaddg =
                    1.0
                    - (1.0
                       / (1.0
                          + (((1.0 - 0.5) / 0.5)
                             * exp((anstiegweathermortg
                                    - ((anstiegweathermortg - anstiegweathermortgmin) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                   * ((weather_list[yearposition_help].weatherfactorg * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                      + (weather_list[yearposition_help].weatherfactorming
                                         * (1
                                            - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                   * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                                         + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)))));
                weathermortadds =
                    1.0
                    - (1.0
                       / (1.0
                          + (((1.0 - 0.5) / 0.5)
                             * exp((anstiegweathermorts
                                    - ((anstiegweathermorts - anstiegweathermortsmin) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                   * ((weather_list[yearposition_help].weatherfactors * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                      + (weather_list[yearposition_help].weatherfactormins
                                         * (1
                                            - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                   * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal
                                         + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)))));
            } else {
                weathermortaddg = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp(anstiegweathermortg * weather_list[yearposition_help].weatherfactorg
                                                 * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                                                       + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)))));
                weathermortadds = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp(anstiegweathermorts * weather_list[yearposition_help].weatherfactors
                                                 * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal
                                                       + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)))));
            }

            double heightreduce = pow((1.0 / (double)tree.height / 100), parameter[0].heightweathermorteinflussexp);  // includes a minimun limit
            if (heightreduce < 0.001)
                heightreduce = 0.001;
            double weather_mort_gmel = parameter[0].mweather * weathermortaddg * heightreduce;
            double weather_mort_sib = parameter[0].mweather * weathermortadds * heightreduce;

            double dry_mort = 0.0;
            if (parameter[0].lineartransect) {
                dry_mort = parameter[0].mdrought
                           * (weather_list[yearposition_help].droughtmort
                              + ((weather_list[yearposition_help].droughtmortmin - weather_list[yearposition_help].droughtmort) * ((double)tree.ycoo / 1000)
                                 / ((double)treerows)))
                           * pow((1.0 / (double)tree.height / 100), 0.5);
            } else if (parameter[0].demlandscape) {
                dry_mort = parameter[0].mdrought
                           * ((weather_list[yearposition_help].droughtmort * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                               / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                              + (weather_list[yearposition_help].droughtmortmin
                                 * (1
                                    - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                          / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                           * pow((1.0 / (double)tree.height / 100), 0.5);
            } else {
                dry_mort = parameter[0].mdrought * weather_list[yearposition_help].droughtmort * pow((1.0 / (double)tree.height / 100), 0.5);
            }
			
            // calculating the mortality rate of the tree considering the factors of each mortality rate
            double Treemortg = 0.0 + parameter[0].mortbg + sapl_mort_gmel + age_mort + growth_mort + dens_mort + weather_mort_gmel + dry_mort;

            double Treemorts = 0.0 + parameter[0].mortbg + sapl_mort_sib + age_mort + growth_mort + dens_mort + weather_mort_sib + dry_mort;
			
			// Treemortg = Treemortg + (((double)tree.firedamage / 3) * (1 / ((double)tree.height / 100) / 200)); //fire damage
			Treemortg = Treemortg + ((double)tree.firedamage); // <--- high fire damage for testing
			
			Treemorts = Treemorts + (((double)tree.firedamage / 3) * (1 / ((double)tree.height / 100) / 200)); //fire damage
			
			// if (tree.firedamage > 0){
			// cout << " ###### tree.height= " << (double)tree.height/100 << " | sapl_mort_gmel=" << sapl_mort_gmel << " | age_mort=" << age_mort << " | growth_mort=" << growth_mort << " | dens_mort=" << dens_mort << " | weather_mort_gmel=" << weather_mort_gmel << " | dry_mort=" << dry_mort << " | Treemortg=" << Treemortg << " | +++++tree.firedamage=" << ((double)tree.firedamage) << endl;
			// }	
			
			// if (tree.firedamage > 0){
			// cout << " - tree.firedamage = " << ((double)tree.firedamage) << endl;
			// }
			
            if (Treemortg > 1.0) {
                Treemortg = 1.0;
            } else if (Treemortg < 0.0) {
                Treemortg = 0.0;
            }

            if (Treemorts > 1.0) {
                Treemorts = 1.0;
            } else if (Treemorts < 0.0) {
                Treemorts = 0.0;
            }

            // determine if a tree dies (deletion of said tree in the corresponding list)
            double zufallsz = uniform.draw();
            if ((((tree.species == 1) && (zufallsz < Treemortg)) || ((tree.species == 2) && (zufallsz < Treemorts))) || (tree.envirimpact <= 0)) {
                tree.growing = false;
				
				// if (tree.firedamage > 0)
					// cout << "tree_list[tree_i].growing = " << tree_list[tree_i].growing;
                tree_list.remove(tree_i);
                // TODO: alternatively set variables to dead and not growing: negative ages could be used for rotting deadwood
				// if (tree.firedamage > 0)
					// cout << " = after death => tree_list[tree_i].growing = " << tree_list[tree_i].growing << endl;
            }
        }
    }
    tree_list.consolidate();
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
                    if (uniform.draw() < (seed.incone ? parameter[0].seedconemort : parameter[0].seedfloormort)) {
                        seed.dead = true;
                        seed_list.remove(i);
                    }
                }
            }

#pragma omp for schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if(tree.growing == true) {
					++n_trees;  // for calculating mean of computation times // TODO still necessary

					if (tree.seednewly_produced > 0) {
						bool pollinated = false;
						for (int sna = 0; sna < tree.seednewly_produced; sna++) {
							if (uniform.draw() >= parameter[0].seedconemort) {
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
