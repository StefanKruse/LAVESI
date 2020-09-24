#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

/****************************************************************************************/
/**
 * \brief calculate tree mortality
 *
 * depends on abiotic factors (temperature and number of days with temperatures above 20 degrees)
 * and on biotic factors
 *
 *
 *******************************************************************************************/
void TreeMort(int yearposition_help, vector<Weather*>& weather_list, list<Tree*>& tree_list) {
    // calculation of the factors in a function that adds a mortality rate impact
    double anstiegweathermortg = 160;
    double anstiegweathermorts = 160;
    double anstiegweathermortgmin = 160;
    double anstiegweathermortsmin = 160;										

    anstiegweathermortg = (60 * weather_list[yearposition_help]->janisothermrestriktiong + 60 * weather_list[yearposition_help]->julisothermrestriktion + 60 * weather_list[yearposition_help]->nddrestriktion);
    anstiegweathermorts = (60 * weather_list[yearposition_help]->janisothermrestriktions + 60 * weather_list[yearposition_help]->julisothermrestriktion + 60 * weather_list[yearposition_help]->nddrestriktion);

    anstiegweathermortgmin = (60 * weather_list[yearposition_help]->janisothermrestriktiongmin + 60 * weather_list[yearposition_help]->julisothermrestriktionmin + 60 * weather_list[yearposition_help]->nddrestriktionmin);
    anstiegweathermortsmin = (60 * weather_list[yearposition_help]->janisothermrestriktionsmin + 60 * weather_list[yearposition_help]->julisothermrestriktionmin + 60 * weather_list[yearposition_help]->nddrestriktionmin);

    // biotic influence:
    for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
        auto pTree = (*pos);

        if (pTree->growing == true) {
            // if maximal age is exceeded an additional factor occurs
            double agesmort = 0.0;
            if (pTree->age > parameter[0].maximumage) {
                agesmort = 1.0;
            }

            // height dependent influences
            double wachstumrel = 1.0;
            if (pTree->height < 130.0) {
                wachstumrel = pTree->dbasalrel;
            } else {
                wachstumrel = pTree->dbreastrel;
            }

            // extra competition effect
            double heightnkugeleinfluss = 1;
            if (pTree->height < (parameter[0].densityvaluemaximumatheight * 2)) {
                heightnkugeleinfluss =
                    heightnkugeleinfluss
                    + (sqrt(pow(parameter[0].densityvaluemaximumatheight, 2) - pow(pTree->height - parameter[0].densityvaluemaximumatheight, 2))
                       / parameter[0].densityvaluemaximumatheight);
            }

            // calculation of young tree's mortality
			/* // TODO: clean this calculation now replaced with simple version based on basal diameter
            double maxhg;
            double maxhs;
            double maxhpufg = weather_list[yearposition_help]->weatherfactorg
                              * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                    + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal);
            double maxhpufs =
                weather_list[yearposition_help]->weatherfactors
                * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal);

            if (maxhpufg <= 0.0) {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhg = parameter[0].dbasalheightalloslope
                            * pow(exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                      + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhg = parameter[0].dbasalheightslopenonlin
                            * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                  + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal);
                }
            } else {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhg = parameter[0].dbasalheightalloslope
                            * pow(weather_list[yearposition_help]->weatherfactorg
                                      * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                            + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhg = parameter[0].dbasalheightslopenonlin * weather_list[yearposition_help]->weatherfactorg
                            * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                  + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal);
                }
            }

            if (maxhpufs <= 0.0) {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhs = parameter[0].dbasalheightalloslope
                            * pow(exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                      + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhs = parameter[0].dbasalheightslopenonlin
                            * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                  + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal);
                }
            } else {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhs = parameter[0].dbasalheightalloslope
                            * pow(weather_list[yearposition_help]->weatherfactors
                                      * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                            + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhs = parameter[0].dbasalheightslopenonlin * weather_list[yearposition_help]->weatherfactors
                            * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                  + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal);
                }
            }
*/
			// TODO: merge gmel/sib sapl_mort calculations
            double sapl_mort_gmel = parameter[0].mortyouth * pow(exp((-1.0 * pTree->dbasal) + pTree->dbasalmax), parameter[0].mortyouthinfluenceexp);
            double sapl_mort_sib = parameter[0].mortyouth * pow(exp((-1.0 * pTree->dbasal) + pTree->dbasalmax), parameter[0].mortyouthinfluenceexp);
            double age_mort = parameter[0].mortage * agesmort * (10.0 * parameter[0].mortbg);
            double growth_mort = parameter[0].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
            double dens_mort = parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert;
			
			double weathermortaddg;
			double weathermortadds;
            if(parameter[0].lineartransect){
				weathermortaddg = 1.0
										 - (1.0
											/ (1.0
											   + (((1.0 - 0.5) / 0.5)
												  * exp(
												  ( anstiegweathermortg - ( ( anstiegweathermortg - anstiegweathermortgmin ) * 1.0/(((double)treerows) / pTree->ycoo) ) )
												  // ( anstiegweathermortg + ((pTree->ycoo / (double) treerows) * (anstiegweathermortg - anstiegweathermortgmin)) ) // scaled effect along transect
												  * ( weather_list[yearposition_help]->weatherfactorg - ( ( weather_list[yearposition_help]->weatherfactorg - weather_list[yearposition_help]->weatherfactorming ) * 1.0/(((double)treerows) / pTree->ycoo) ) ) // scaled effect along transect
														* exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
															  + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)))));
				weathermortadds = 1.0
										 - (1.0
											/ (1.0
											   + (((1.0 - 0.5) / 0.5)
												  * exp(
												  ( anstiegweathermorts - ( ( anstiegweathermorts - anstiegweathermortsmin ) * 1.0/(((double)treerows) / pTree->ycoo) ) )
												  // ( anstiegweathermorts + ((pTree->ycoo / (double) treerows) * (anstiegweathermorts - anstiegweathermortsmin)) ) // scaled effect along transect
												  * ( weather_list[yearposition_help]->weatherfactors - ( ( weather_list[yearposition_help]->weatherfactors - weather_list[yearposition_help]->weatherfactormins ) * 1.0/(((double)treerows) / pTree->ycoo) ) ) // scaled effect along transect
														* exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
															  + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)))));
			} else
			{
				weathermortaddg = 1.0
										 - (1.0
											/ (1.0
											   + (((1.0 - 0.5) / 0.5)
												  * exp(anstiegweathermortg * weather_list[yearposition_help]->weatherfactorg
														* exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
															  + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)))));
				weathermortadds = 1.0
										 - (1.0
											/ (1.0
											   + (((1.0 - 0.5) / 0.5)
												  * exp(anstiegweathermorts * weather_list[yearposition_help]->weatherfactors
														* exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
															  + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)))));
			}

			double heightreduce = pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp); // includes a minimun limit
			if(heightreduce < 0.001)
				heightreduce=0.001;
            double weather_mort_gmel = parameter[0].mweather * weathermortaddg * heightreduce;
            double weather_mort_sib = parameter[0].mweather * weathermortadds * heightreduce;

            double dry_mort=0.0;
			if(parameter[0].lineartransect){
				dry_mort = parameter[0].mdrought * 
							( weather_list[yearposition_help]->droughtmort - ( ( weather_list[yearposition_help]->droughtmort - weather_list[yearposition_help]->droughtmortmin ) * 1.0/(((double)treerows) / pTree->ycoo) ) )
							// (weather_list[yearposition_help]->droughtmort + ((pTree->ycoo / (double) treerows) * (weather_list[yearposition_help]->droughtmort - weather_list[yearposition_help]->droughtmortmin)))
							* pow((1.0 / pTree->height), 0.5);
			} else
			{
				dry_mort = parameter[0].mdrought * weather_list[yearposition_help]->droughtmort * pow((1.0 / pTree->height), 0.5);
			}
            // calculating the mortality rate of the tree considering the factors of each mortality rate
            double Treemortg = 0.0 + parameter[0].mortbg + sapl_mort_gmel + age_mort + growth_mort + dens_mort + weather_mort_gmel + dry_mort;

            double Treemorts = 0.0 + parameter[0].mortbg + sapl_mort_sib + age_mort + growth_mort + dens_mort + weather_mort_sib + dry_mort;
cout << pTree->dbasalmax << "; " << pTree->dbasalrel <<  " === " << parameter[0].mortbg << "; " << sapl_mort_gmel << "; " << age_mort << "; " << growth_mort << "; " << dens_mort << "; " << weather_mort_gmel << "; " << dry_mort << " => " << Treemortg;
// cout << weather_mort_gmel << " + " << dry_mort << " => " << Treemortg;
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

            // Determine if a tree dies (deletion of said tree in the corresponding list)
            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));
            if (((pTree->species == 1) && (zufallsz < Treemortg)) || ((pTree->species == 2) && (zufallsz < Treemorts))) {
                delete pTree;
                pos = tree_list.erase(pos);
cout << " killed " << endl;
            } else {
                ++pos;
cout << " lives " << endl;
            }
        } else {
            ++pos;
        }
    }
}

/****************************************************************************************/
/**
 * \brief calculate tree and seed mortality
 *
 * first create surviving seeds then call TreeMort()
 *
 *
 *******************************************************************************************/
void Mortality(struct Parameter* parameter,
               int Jahr,
               int yearposition,
               vector<list<Tree*>>& world_tree_list,
               vector<VectorList<Seed>>& world_seed_list,
               vector<vector<Weather*>>& world_weather_list) {
    int aktort = 0;

    for (vector<vector<Weather*>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather*>& weather_list = *posw;

        vector<list<Tree*>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        list<Tree*>& tree_list = *world_positon_b;

        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;

        aktort++;

        // mortality of seeds
        for (unsigned int i = 0; i < seed_list.size(); ++i) {
            auto& seed = seed_list[i];
            if (seed.dead) {
                continue;
            }
            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));

            if (zufallsz < seed.incone ? parameter[0].seedconemort : parameter[0].seedfloormort) {
                seed.dead = true;
                seed_list.remove(i);
            }
        }

        int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
        int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        // implementation of multi-core-processing == trees are ordered by age which is highly correlated with seedprodAKT so that only elements are considered untiil the last tree producing seeds
		omp_set_dynamic(0);                                 // disable dynamic teams ; TODO remove here
		omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers ; TODO remove here

		double direction = 0.0;
		double velocity = 0.0;
		unsigned int ripm = 0, cntr = 0;
		double p = 0.0, kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2), phi = 0.0, dr = 0.0, dx = 0.0, dy = 0.0;
		double I0kappa = 0.0;
		double pe = 0.01;
		double C = parameter[0].pollengregoryc;
		double m = parameter[0].pollengregorym;
		vector<int> Vname;
		vector<double> Vthdpth;

		// set end of the iterations split up to last tree with produced seeds
		list<Tree*>::iterator lasttreewithseeds_iter = tree_list.begin();
		int lasttreewithseeds_pos = 0;
		int treeiter = 0;
		for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb) {
			auto pTree = (*posb);

			treeiter = treeiter + 1;

			if (pTree->seednewly_produced > 0) {
				lasttreewithseeds_pos = treeiter;
			}
		}
		advance(lasttreewithseeds_iter, lasttreewithseeds_pos);

#pragma omp parallel default(shared) private(direction, velocity, ripm, cntr, p, kappa, phi, dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth)
		{
			direction = 0.0;
			velocity = 0.0;
			ripm = 0, cntr = 0;
			p = 0.0;
			kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2);
			phi = 0.0;
			dr = 0.0;
			dx = 0.0;
			dy = 0.0;
			I0kappa = 0.0;
			pe = 0.01;
			C = parameter[0].pollengregoryc;
			m = parameter[0].pollengregorym;

			int thread_count = omp_get_num_threads();
			int thread_num = omp_get_thread_num();
			size_t chunk_size = lasttreewithseeds_pos / thread_count;
			auto begin = tree_list.begin();
			std::advance(begin, thread_num * chunk_size);
			auto end = begin;

			if (thread_num == (thread_count - 1)) {
				if ((parameter[0].ivort == 1)) {
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads()
						 << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl
						 << endl;
				}

				end = lasttreewithseeds_iter;
			} else {
				std::advance(end, chunk_size);
			}

			// declare a local seed list to be filled by each thread
			VectorList<Seed> newseed_list;

			int n_trees = 0;

// wait for all threads to initialize and then proceed
#pragma omp barrier
			for (auto it = begin; it != end; ++it) {
				++n_trees;  // for calculating mean of computation times // TODO still necessary

				auto pTree = (*it);

				if (pTree->seednewly_produced > 0) {
					int seedlebend = 0;
					for (int sna = 0; sna < pTree->seednewly_produced; sna++) {
						double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));
						if (zufallsz >= parameter[0].seedconemort) {
							++seedlebend;
						}
					}

					if (seedlebend > 0) {
						if ((parameter[0].pollination == 1 && parameter[0].ivort > 1045) || (parameter[0].pollination == 9)) {
							Pollinationprobability(pTree->xcoo, pTree->ycoo, &parameter[0], world_positon_b, direction, velocity, ripm, cntr, p, kappa, phi,
												   dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth, n_trees);
						}

						for (int sl = 0; sl < seedlebend; sl++) {
							Seed seed;

							seed.yworldcoo = aktortyworldcoo;
							seed.xworldcoo = aktortxworldcoo;
							seed.xcoo = pTree->xcoo;
							seed.ycoo = pTree->ycoo;
							seed.namem = pTree->name;
							seed.dead = false;

							// if chosen, determine the father by pollination out of available (matured) trees
							if ((Vname.size() > 0) && (parameter[0].pollination == 1 || parameter[0].pollination == 9)) {
								int iran = (int)rand() / (RAND_MAX + 1.0) * Vname.size() - 1;
								seed.namep = Vname.at(iran);
								seed.thawing_depthinfluence = 100;
							} else {
								seed.namep = 0;
								seed.thawing_depthinfluence = 100;
							}

							seed.line = pTree->line;
							seed.generation = pTree->generation + 1;
							seed.incone = true;
							seed.weight = 1;
							seed.age = 0;
							seed.species = pTree->species;
							seed.releaseheight = pTree->height;

							seed_list.add(seed);
						}
					}
				}
			}  // main tree loop on each core
		}      // parallel region

		Vname.clear();
		Vname.shrink_to_fit();
		Vthdpth.clear();
		Vthdpth.shrink_to_fit();

        TreeMort(yearposition, weather_list, tree_list);
    }
}
