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
                int i = seed.ycoo * parameter[0].sizemagnif / 1000;
                int j = seed.xcoo * parameter[0].sizemagnif / 1000;

                const auto curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif) + static_cast<std::size_t>(j);

                if ((parameter[0].demlandscape) && (plot_list[curposi].elevation == 32767)) {
                    seed.dead = true;
                    seed_list.remove(i_seed);
                    continue;
                }

                double germinationlitterheightinfluence = (1.0 - 0.01) / (0 - 1000.0) * (double) plot_list[curposi].litterheight0 + 1;
                // double germinationlitterheightinfluence = (1.0 - 0.01) / (200.0 - 600.0) * 200 + 1.495;
                // (1.0 - 0.01) / (200.0 - 600.0) * ((double) plot_list[curposi].litterheight) + 1.495; // TODO: check litterheight implementation
// cout << plot_list[curposi].litterheight0 << " => " << germinationlitterheightinfluence << endl;
                if (germinationlitterheightinfluence < 0.01) {// minimum 1%
                    germinationlitterheightinfluence = 0.01;
                }
// if(plot_list[curposi].litterheight0!=1000)
	// cout << " ... after: " << plot_list[curposi].litterheight0 << " => " << germinationlitterheightinfluence << endl;

                // calculate the thawing depth influence on the tree growth
                double thawing_depthinfluence_help = 100;
                if ( (parameter[0].thawing_depth == true) && (plot_list[curposi].maxthawing_depth < (speciestrait[seed.species].minactivelayer*10)) ) {  // TODO: check calculation only during spinup
                    thawing_depthinfluence_help = (unsigned short)((200.0 / (speciestrait[seed.species].minactivelayer*10)) * (double)plot_list[curposi].maxthawing_depth);
                }

                // ... and weather.
                // calculate the latest growth performance
                // TODO: merge with general growth functions
                double maxbw_help = 0;

                if (parameter[0].lineartransect == true) {
					maxbw_help = exp(speciestrait[seed.species].gdbasalconst + speciestrait[seed.species].gdbasalfac * 0 + speciestrait[seed.species].gdbasalfacq * 0 * 0)
								 * (weather_list[yearposition].weatherfactor[seed.species]
									+ ((weather_list[yearposition].weatherfactormin[seed.species] - weather_list[yearposition].weatherfactor[seed.species])
									   * ((double)seed.ycoo / 1000) / ((double)treerows)))
								 * (((double)thawing_depthinfluence_help) / 100);
                } else if (parameter[0].demlandscape) {  // copied from growth.cpp
                    if (parameter[0].thawing_depth) {
						maxbw_help = exp(speciestrait[seed.species].gdbasalconst + speciestrait[seed.species].gdbasalfac * 0 + speciestrait[seed.species].gdbasalfacq * 0 * 0)
									 * ((weather_list[yearposition].weatherfactor[seed.species]
										 * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
										 / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
										+ (weather_list[yearposition].weatherfactormin[seed.species]
										   * (1
											  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
													/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
									 * (((double)seed.thawing_depthinfluence) / 100);
                    } else {
						maxbw_help = exp(speciestrait[seed.species].gdbasalconst + speciestrait[seed.species].gdbasalfac * 0 + speciestrait[seed.species].gdbasalfacq * 0 * 0)
									 * ((weather_list[yearposition].weatherfactor[seed.species]
										 * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
										 / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
										+ (weather_list[yearposition].weatherfactormin[seed.species]
										   * (1
											  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
													/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
                    }
                } else {
                        maxbw_help = exp(speciestrait[seed.species].gdbasalconst + speciestrait[seed.species].gdbasalfac * 0 + speciestrait[seed.species].gdbasalfacq * 0 * 0)
                                     * weather_list[yearposition].weatherfactor[seed.species] * (((double)thawing_depthinfluence_help) / 100);
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
				if (density_help < 0.0)
					density_help = 0.0;
				if (density_help > 1.0)
					density_help = 1.0;
                // update growth
                double basalgrowth_help = maxbw_help * (1.0 - density_help);
// if(basalgrowth_help>10)
	// cout << "basalgrowth_help=" << basalgrowth_help << " ... maxbw= " << maxbw_help << " ... " << seed.species << " ... " << ((double)plot_list[curposi].elevation / 10) << " density_help= " << density_help << endl;
	
                // minimal germination rate is roughly estimated // TODO: adjust for multiple species representation
                double germinationprobability = 0.0;
                if (parameter[0].lineartransect == true) {
                    germinationprobability = speciestrait[seed.species].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * speciestrait[seed.species].germinationweatherinfluence
                                     * pow((weather_list[yearposition].weatherfactor[seed.species]
                                            + ((weather_list[yearposition].weatherfactormin[seed.species] - weather_list[yearposition].weatherfactor[seed.species])
                                               * ((double)seed.ycoo / 1000) / ((double)treerows))),
                                           2.0)                           // weather influence
                                     * germinationlitterheightinfluence;  // litter layer dependency
                 } else if (parameter[0].demlandscape) {
                    germinationprobability = speciestrait[seed.species].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * speciestrait[seed.species].germinationweatherinfluence
                                     * pow(((weather_list[yearposition].weatherfactor[seed.species]
                                             * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                            + (weather_list[yearposition].weatherfactormin[seed.species]
                                               * (1
                                                  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))))),
                                           2.0)                           // weather influence
                                     * germinationlitterheightinfluence;  // litter layer dependency
               } else {
                    germinationprobability = speciestrait[seed.species].germinationrate       // background germination rate
                               + (basalgrowth_help / maxbw_help)  // rel growth on position is density dependent
                                     * speciestrait[seed.species].germinationweatherinfluence * pow(weather_list[yearposition].weatherfactor[seed.species], 2.0)  // weather influence
                                     * germinationlitterheightinfluence;                                                              // litter layer dependency
               }

				double rn = uniform.draw();
				if (rn < germinationprobability) {
// cout << "germ prob = " << germinationprobability << " --> random number = " << rn << endl;
					if (maxbw_help > 0.0) { // otherwise can survive
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

						// tree height update
						if (parameter[0].allometryfunctiontype == 3) {// logistic growth
							tree.height = 10 * exp(speciestrait[seed.species].heightloga/(1+exp((speciestrait[seed.species].heightlogb-log(tree.dbasal*10))/speciestrait[seed.species].heightlogc)));
		// cout << "H = " << tree.height << endl;
						} else if (parameter[0].allometryfunctiontype == 1) {
							tree.height = 10 * speciestrait[seed.species].dbasalheightalloslope * pow(maxbw_help, speciestrait[seed.species].dbasalheightalloexp);
						} else {
							tree.height = 10 * speciestrait[seed.species].dbasalheightslopenonlin * maxbw_help;
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
						tree.twi = 6.25*100;
						tree.soilhumidity = 1;
						tree.crownstart = 0*10;
						tree.relcrowndamage = 0*1000;

						tree_list.add(std::move(tree));

						seed.dead = true;
						seed_list.remove(i_seed);
					}
                } //else {
                    // seed.dead = true;
                    // seed_list.remove(i_seed);
                // }
            }
        }  // seed_list loop
        seed_list.consolidate();
        tree_list.consolidate();
    }
}

