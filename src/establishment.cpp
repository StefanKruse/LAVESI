#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void Treeestablishment(Parameter* parameter,
                       int yearposition,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<VectorList<Seed>>& world_seed_list,
                       vector<vector<Weather>>& world_weather_list,
                       vector<vector<Envirgrid>>& world_plot_list) {
    int aktort = 0;
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather>& weather_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
        vector<vector<Envirgrid>>::iterator world_positon_k = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *world_positon_k;
        aktort++;

        RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
        for (unsigned int i_seed = 0; i_seed < seed_list.size(); ++i_seed) {
            auto& seed = seed_list[i_seed];
            if (!seed.dead && !seed.incone) {
                // determine if the seed germinates, depending on the density around it and the litter layer
                int i = (seed.ycoo / 1000) * parameter[0].sizemagnif;
                int j = (seed.xcoo / 1000) * parameter[0].sizemagnif;

                const auto curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
                                     + static_cast<std::size_t>(j);

                if ((parameter[0].demlandscape) && (plot_list[curposi].elevation == 32767)) {
                    seed.dead = true;
                    seed_list.remove(i_seed);
                    continue;
                }

                double germinationlitterheightinfluence = (1.0 - 0.01) / (200.0 - 600.0) * 200 + 1.495;
                // (1.0 - 0.01) / (200.0 - 600.0) * ((double) plot_list[curposi].litterheight) + 1.495; // TODO: check litterheight implementation

                if (germinationlitterheightinfluence < 0.01) {
                    germinationlitterheightinfluence = 0.01;
                }

                // calculate the thawing depth influence on the tree growth
                double thawing_depthinfluence_help = 100;
                if ((plot_list[curposi].maxthawing_depth < 2000) && parameter[0].thawing_depth == true) {  // TODO: check calculation only during spinup
                    thawing_depthinfluence_help = (unsigned short)((200.0 / 2000.0) * (double)plot_list[curposi].maxthawing_depth);
                }

                // ... and weather.
                // calculate the latest growth performance
                // TODO: merge with general growth functions
                double maxbw_help = 0;


                if (parameter[0].n_weather_along_grid>0) {
// cout << ".... -> establishment with parameter[0].n_weather_along_grid>0" << endl;
// if (((double)seed.ycoo / (1000)) > (treerows)) {
	// cout << " y = " << ((double)seed.ycoo / (1000)) << endl;
	// exit(1);
// }
// cout << "     + calculation of weighted mean: " << weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo / (1000))) << endl;
// cout << "     + calculation of weighted mean: " << weighmeanweathervar(weather_list[yearposition].weatherfactorming, ((double)seed.ycoo / (1000))) << endl;

	// versatz so dass die n_number_grids das obere Ende sind
	// ... 
					double transectstart= treerows - parameter[0].locationshift;
// cout << " transectstart= " << transectstart  << " seed.ycoo= " << ((double)seed.ycoo/1000) << "     + calculation of weighted mean: " << weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo/1000)-transectstart) << endl;

                    if (seed.species == 1) {
                        maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                                     * weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo/1000)-transectstart)
                                     * (((double)thawing_depthinfluence_help) / 100);

                    } else if (seed.species == 2) {
                        maxbw_help = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                     * weighmeanweathervar(weather_list[yearposition].weatherfactors, ((double)seed.ycoo/1000)-transectstart)
                                     * (((double)thawing_depthinfluence_help) / 100);
                    }
// cout << " ... maxbw_help=" << maxbw_help << " from function=" << weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo/1000)-transectstart) << " ... posongrad=" << ((double)seed.ycoo/1000)-transectstart << "...fac1/2=" << weather_list[yearposition].weatherfactorg[0] << " / " << weather_list[yearposition].weatherfactorg[1] << endl;
                } else if (parameter[0].lineartransect == true) {
                    if (seed.species == 1) {
                        maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                                     * (weather_list[yearposition].weatherfactorg[0]
                                        + ((weather_list[yearposition].weatherfactorming[0] - weather_list[yearposition].weatherfactorg[0])
                                           * ((double)seed.ycoo / 1000) / ((double)treerows)))
                                     * (((double)thawing_depthinfluence_help) / 100);

                    } else if (seed.species == 2) {
                        maxbw_help = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                     * (weather_list[yearposition].weatherfactors[0]
                                        + ((weather_list[yearposition].weatherfactormins[0] - weather_list[yearposition].weatherfactors[0])
                                           * ((double)seed.ycoo / 1000) / ((double)treerows)))
                                     * (((double)thawing_depthinfluence_help) / 100);
                    }
                } else if (parameter[0].demlandscape) {  // copied from growth.cpp
                    if (parameter[0].thawing_depth) {
                        if (seed.species == 1) {
                            maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                                         * ((weather_list[yearposition].weatherfactorg[0]
                                             * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                            + (weather_list[yearposition].weatherfactorming[0]
                                               * (1
                                                  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                         * (((double)seed.thawing_depthinfluence) / 100);
                        } else if (seed.species == 2) {
                            maxbw_help = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                         * ((weather_list[yearposition].weatherfactors[0]
                                             * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                            + (weather_list[yearposition].weatherfactormins[0]
                                               * (1
                                                  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                         * ((((double)seed.thawing_depthinfluence * 0.8) / 100) - 0.6);
                        }
                    } else {
                        if (seed.species == 1) {
                            maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                                         * ((weather_list[yearposition].weatherfactorg[0]
                                             * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                            + (weather_list[yearposition].weatherfactorming[0]
                                               * (1
                                                  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
                        } else if (seed.species == 2) {
                            maxbw_help = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                         * ((weather_list[yearposition].weatherfactors[0]
                                             * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                            + (weather_list[yearposition].weatherfactormins[0]
                                               * (1
                                                  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
                        }
                    }
                } else {
                    if (seed.species == 1) {
                        maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                                     * weather_list[yearposition].weatherfactorg[0] 
									 * (((double)thawing_depthinfluence_help) / 100);
                    } else if (seed.species == 2) {
                        maxbw_help = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                     * weather_list[yearposition].weatherfactors[0] 
									 * (((double)thawing_depthinfluence_help) / 100);
                    }
                }

                // individual seedling growth depends on density
                // define seedlings density value
                double flaechengroesze = 0.0;
                if (parameter[0].calcinfarea == 1)  // linearly increasing
                    flaechengroesze = maxbw_help * parameter[0].incfac / 100.0;
                else if (parameter[0].calcinfarea == 2)  // linearly increasing
                    flaechengroesze = maxbw_help * (2 / 3) * parameter[0].incfac / 100.0;
                else if (parameter[0].calcinfarea == 3)  // linearly increasing
                    flaechengroesze = maxbw_help * (4 / 3) * parameter[0].incfac / 100.0;
                else if (parameter[0].calcinfarea == 4)  // logistic growth function with maximum at 8 m
                    flaechengroesze = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.1 * maxbw_help)))) - 1;
                else if (parameter[0].calcinfarea == 5)  // logistic growth function with maximum at 8 m
                    flaechengroesze = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.2 * maxbw_help)))) - 1;
                else if (parameter[0].calcinfarea == 6)  // logistic growth function with maximum at 8 m
                    flaechengroesze = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.5 * maxbw_help)))) - 1;

                double density_help = pow(
                    pow(maxbw_help, parameter[0].densitytreetile) * pow(flaechengroesze / (1.0 / parameter[0].sizemagnif), parameter[0].densitysmallweighing),
                    parameter[0].densityvaluemanipulatorexp);
                // get gridcell
                if (parameter[0].densitymode == 2) {
                    if (plot_list[curposi].Treedensityvalue > 0) {
                        if (parameter[0].densitytiletree == 1)  // sum of values
                        {
                            density_help = (1.0 - (density_help / ((double)plot_list[curposi].Treedensityvalue / 10000)));
                            //                           density_desired(at position) / density_currently(at position)
                        } else if (parameter[0].densitytiletree == 2)  // multiplication of values
                        {
                            density_help = (1.0 - (density_help / (((double)plot_list[curposi].Treedensityvalue / 10000) * density_help)));
                            //                           density_desired(at position) / density_currently(at position)
                        }
                    } else {
                        density_help = 0.0;  // no competition
                    }
                }
                // update growth
                double basalgrowth_help = maxbw_help * (1.0 - density_help);
                double zufallsz = uniform.draw();

                // minimal germination rate is roughly estimated // TODO: adjust for multiple species representation
                double germgmel = 0.0;
                double germsib = 0.0;
				if (parameter[0].n_weather_along_grid>0) {
// cout << ".... -> establishment with parameter[0].n_weather_along_grid>0" << endl;
// if (((double)seed.ycoo / (1000)) > (treerows)) {
	// cout << " y = " << ((double)seed.ycoo / (1000)) << endl;
	// exit(1);
// }
// cout << "     + calculation of weighted mean: " << weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo / (1000))) << endl;
// cout << "     + calculation of weighted mean: " << weighmeanweathervar(weather_list[yearposition].weatherfactorming, ((double)seed.ycoo / (1000))) << endl;

	// versatz so dass die n_number_grids das obere Ende sind
	// ... 
					double transectstart= treerows - parameter[0].locationshift;
// cout << " transectstart= " << transectstart  << " seed.ycoo= " << ((double)seed.ycoo/1000) << "     + calculation of weighted mean: " << weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo/1000)-transectstart) << endl;

					germgmel = parameter[0].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * parameter[0].germinatioweatherinfluence
                                     * pow(weighmeanweathervar(weather_list[yearposition].weatherfactorg, ((double)seed.ycoo/1000)-transectstart), 2.0)                           // weather influence
                                     * germinationlitterheightinfluence;  // litter layer dependency
                    germsib = parameter[0].germinationrate                // background germination rate
                              + (basalgrowth_help / maxbw_help)           // rel growth on position is density dependent
                                    * parameter[0].germinatioweatherinfluence
                                    * pow(weighmeanweathervar(weather_list[yearposition].weatherfactors, ((double)seed.ycoo/1000)-transectstart), 2.0)                           // weather influence
                                    * germinationlitterheightinfluence;  // litter layer dependency
                } else if (parameter[0].lineartransect == true) {
                    germgmel = parameter[0].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * parameter[0].germinatioweatherinfluence
                                     * pow((weather_list[yearposition].weatherfactorg[0]
                                            + ((weather_list[yearposition].weatherfactorming[0] - weather_list[yearposition].weatherfactorg[0])
                                               * ((double)seed.ycoo / 1000) / ((double)treerows))),
                                           2.0)                           // weather influence
                                     * germinationlitterheightinfluence;  // litter layer dependency
                    germsib = parameter[0].germinationrate                // background germination rate
                              + (basalgrowth_help / maxbw_help)           // rel growth on position is density dependent
                                    * parameter[0].germinatioweatherinfluence
                                    * pow((weather_list[yearposition].weatherfactors[0]
                                           + ((weather_list[yearposition].weatherfactormins[0] - weather_list[yearposition].weatherfactors[0])
                                              * ((double)seed.ycoo / 1000) / ((double)treerows))),
                                          2.0)                           // weather influence
                                    * germinationlitterheightinfluence;  // litter layer dependency
                } else if (parameter[0].demlandscape) {
                    germgmel = parameter[0].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * parameter[0].germinatioweatherinfluence
                                     * pow(((weather_list[yearposition].weatherfactorg[0]
                                             * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                            + (weather_list[yearposition].weatherfactorming[0]
                                               * (1
                                                  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))))),
                                           2.0)                           // weather influence
                                     * germinationlitterheightinfluence;  // litter layer dependency
                    germsib = parameter[0].germinationrate                // background germination rate
                              + (basalgrowth_help / maxbw_help)           // rel growth on position is density dependent
                                    * parameter[0].germinatioweatherinfluence
                                    * pow(((weather_list[yearposition].weatherfactors[0]
                                            * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                            / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                           + (weather_list[yearposition].weatherfactormins[0]
                                              * (1
                                                 - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))))),
                                          2.0)                           // weather influence
                                    * germinationlitterheightinfluence;  // litter layer dependency
                } else {
                    germgmel = parameter[0].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * parameter[0].germinatioweatherinfluence * pow(weather_list[yearposition].weatherfactorg[0], 2.0)  // weather influence
                                     * germinationlitterheightinfluence;                                                              // litter layer dependency
                    germsib = parameter[0].germinationrate       // background germination rate
                              + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                    * parameter[0].germinatioweatherinfluence * pow(weather_list[yearposition].weatherfactors[0], 2.0)  // weather influence
                                    * germinationlitterheightinfluence;                                                              // litter layer dependency
                }

                if (seed.species == 1) {
                    if (zufallsz < germgmel) {
                        if (maxbw_help > 0.0) {
                            Tree tree;

                            // tree.yworldcoo = seed.yworldcoo;
                            // tree.xworldcoo = seed.xworldcoo;
                            tree.xcoo = seed.xcoo;
                            tree.ycoo = seed.ycoo;
                            // tree.name = ++parameter[0].nameakt;
                            // tree.namem = seed.namem;
                            // tree.namep = seed.namep;
                            // tree.yr_of_establishment = yearposition;
                            // tree.line = seed.line;
                            // tree.generation = seed.generation;
                            tree.dbasal = basalgrowth_help;
                            tree.dbasalmax = 1000 * maxbw_help;
                            tree.dbasalrel = 1000;
                            tree.dbreast = 0.0;
                            tree.dbreastrel = 1000;

                            if (parameter[0].allometryfunctiontype == 1) {
                                tree.height = 100 * parameter[0].dbasalheightalloslope * pow(maxbw_help, parameter[0].dbasalheightalloexp);
                            } else {
                                tree.height = 100 * parameter[0].dbasalheightslopenonlin * maxbw_help;
                            }
                            tree.age = 0;
                            tree.cone = false;
                            tree.coneheight = 65535;
                            tree.seednewly_produced = 0;
                            // tree.seedproduced = 0;
                            // tree.buffer = 1;
                            tree.densitywert = density_help;
                            // tree.dispersaldistance = seed.dispersaldistance;
                            tree.growing = true;
                            tree.species = seed.species;
                            tree.thawing_depthinfluence = thawing_depthinfluence_help;
                            tree.envirimpact = 10000;

                            tree_list.add(std::move(tree));

                            seed.dead = true;
                            seed_list.remove(i_seed);
                        }
                    }
                } else if (seed.species == 2) {
                    if (zufallsz < germsib) {
                        if (maxbw_help > 0.0) {
                            Tree tree;

                            // tree.yworldcoo = seed.yworldcoo;
                            // tree.xworldcoo = seed.xworldcoo;
                            tree.xcoo = seed.xcoo;
                            tree.ycoo = seed.ycoo;
                            // tree.name = ++parameter[0].nameakt;
                            // tree.namem = seed.namem;
                            // tree.namep = seed.namep;
                            // tree.yr_of_establishment = yearposition;
                            // tree.line = seed.line;
                            // tree.generation = seed.generation;
                            tree.dbasal = basalgrowth_help;
                            tree.dbasalmax = 1000 * maxbw_help;
                            tree.dbasalrel = 1000;
                            tree.dbreast = 0.0;
                            tree.dbreastrel = 1000;

                            if (parameter[0].allometryfunctiontype == 1) {
                                tree.height = 100 * parameter[0].dbasalheightalloslope * pow(maxbw_help, parameter[0].dbasalheightalloexp);
                            } else {
                                tree.height = 100 * parameter[0].dbasalheightslopenonlin * maxbw_help;
                            }

                            tree.age = 0;
                            tree.cone = false;
                            tree.coneheight = 65535;
                            tree.seednewly_produced = 0;
                            // tree.seedproduced = 0;
                            // tree.buffer = 1;
                            tree.densitywert = density_help;
                            tree.thawing_depthinfluence = thawing_depthinfluence_help;
                            // tree.dispersaldistance = seed.dispersaldistance;
                            tree.growing = true;
                            tree.species = seed.species;
                            tree_list.add(std::move(tree));

                            seed.dead = true;
                            seed_list.remove(i_seed);
                        }
                    }
                } else {
                    seed.dead = true;
                    seed_list.remove(i_seed);
                }
            }
        }  // seed_list loop
        seed_list.consolidate();
        tree_list.consolidate();
    }
}
