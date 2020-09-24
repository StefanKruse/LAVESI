#include <cassert>
#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

void Treeestablishment(struct Parameter* parameter,
                       int yearposition,
                       vector<list<Tree*>>& world_tree_list,
                       vector<VectorList<Seed>>& world_seed_list,
                       vector<vector<Weather*>>& world_weather_list,
                       vector<vector<Envirgrid*>>& world_plot_list) {
    int aktort = 0;
    for (vector<vector<Weather*>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather*>& weather_list = *posw;

        vector<list<Tree*>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        list<Tree*>& tree_list = *world_positon_b;

        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;

        vector<vector<Envirgrid*>>::iterator world_positon_k = (world_plot_list.begin() + aktort);
        vector<Envirgrid*>& plot_list = *world_positon_k;

        aktort++;

        // go through seed list
        for (unsigned int i_seed = 0; i_seed < seed_list.size(); ++i_seed) {
            auto& seed = seed_list[i_seed];
            if (seed.dead) {
                continue;
            }
			
            if (!seed.incone) {
                // determine if the seed germinates, depending on the density around it and the litter layer
                int i = (int)floor(seed.ycoo * parameter[0].sizemagnif);
                int j = (int)floor(seed.xcoo * parameter[0].sizemagnif);

                assert(i * treecols * parameter[0].sizemagnif + j >= 0);

				if( (parameter[0].demlandscape) & ((plot_list[i * treecols * parameter[0].sizemagnif + j]->elevation==9999) | (plot_list[i * treecols * parameter[0].sizemagnif + j]->slope<5)) ){
					seed.dead = true;
					// seed_list.remove(i);
					continue;
				}

                double keimungauflageinfluence =
                    (1.0 - 0.01) / (200.0 - 600.0) * ((double)plot_list[i * treecols * parameter[0].sizemagnif + j]->litterheight) + 1.495;

                if (keimungauflageinfluence < 0.01) {
                    keimungauflageinfluence = 0.01;
                }




// maxthawing_depth
// calculate the thawing depth influence on the tree growth
double thawing_depthinfluence_help = 100;
if((plot_list[i * treecols * parameter[0].sizemagnif + j]->maxthawing_depth < 2000)
	&& parameter[0].thawing_depth == true) {//TODO: check calculation only during spinup 
	thawing_depthinfluence_help =
		(unsigned short)((200.0 / 2000.0) * (double)plot_list[i * treecols * parameter[0].sizemagnif + j]->maxthawing_depth);
}
				
					
                // ... and weather.
                // calculate the latest growth performance
				// TODO: clean code 
				// TODO: merge with growth functions
                double maxbw_help = 0;

                if (parameter[0].lineartransect == true) {
                    if (seed.species == 1) {
                        maxbw_help =
                            exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                            * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / seed.ycoo) ) )
							* (((double)thawing_depthinfluence_help) / 100);

                    } else if (seed.species == 2) {
                        maxbw_help =
                            exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                            * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / seed.ycoo) ) )
							* (((double)thawing_depthinfluence_help) / 100);
                    }
                } else {
                    if (seed.species == 1) {
                        maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
											* weather_list[yearposition]->weatherfactorg
											* (((double)thawing_depthinfluence_help) / 100);
                    } else if (seed.species == 2) {
                        maxbw_help = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                           * weather_list[yearposition]->weatherfactors
											* (((double)thawing_depthinfluence_help) / 100);
                    }
                }

// TODO: clean code and merge with functions from plotupdate.cpp/growth.cpp
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
	if (plot_list[i * treecols * parameter[0].sizemagnif + j]->Treedensityvalue > 0.0) {
		if (parameter[0].densitytiletree == 1)  // sum of values
		{
			density_help = (1.0 - (density_help / plot_list[i * treecols * parameter[0].sizemagnif + j]->Treedensityvalue));
			//                           density_desired(at position) / density_currently(at position)
		} else if (parameter[0].densitytiletree == 2)  // multiplication of values
		{
			density_help =
				(1.0
				 - (density_help / (plot_list[i * treecols * parameter[0].sizemagnif + j]->Treedensityvalue * density_help)));
			//                           density_desired(at position) / density_currently(at position)
		}
	} else {
		density_help = 0.0;  // no competition
	}
}					
// update growth
double basalgrowth_help = maxbw_help * (1.0 - density_help);
// if(basalgrowth_help < maxbw_help)
	// cout << maxbw_help << " - > " << basalgrowth_help << endl;
                double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));

                // add new tree if seed germinates
// cout << maxbw_help << " - " << " - " << basalgrowth_help << " - " << density_help << " - " << parameter[0].germinatioweatherinfluence * basalgrowth_help/maxbw_help << " - " << keimungauflageinfluence << endl;
// cout << basalgrowth_help << " - " << exp(parameter[0].gdbasalconstgmel) << " - " << ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / seed.ycoo) ) ) << " y==" << seed.ycoo << endl;

// cout << parameter[0].germinationrate // background germination rate
// + (basalgrowth_help/maxbw_help) // rel growth on position is density dependent
// * parameter[0].germinatioweatherinfluence * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / seed.ycoo) ) ) // weather influence
// * keimungauflageinfluence // litter layer dependency
// << " ";
				// minimal germination rate is roughly estimated
                if (seed.species == 1) {
                    if (zufallsz
                        < (
							parameter[0].germinationrate // background germination rate
							+ (basalgrowth_help/maxbw_help) // rel growth on position is density dependent
							* parameter[0].germinatioweatherinfluence * pow(( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / seed.ycoo) ) ), 2.0) // weather influence
							* keimungauflageinfluence // litter layer dependency
						)) {
                        if (maxbw_help > 0.0) {
                            auto pTree = new Tree();

                            pTree->yworldcoo = seed.yworldcoo;
                            pTree->xworldcoo = seed.xworldcoo;
                            pTree->xcoo = seed.xcoo;
                            pTree->ycoo = seed.ycoo;
                            pTree->name = ++parameter[0].nameakt;
                            pTree->namem = seed.namem;
                            pTree->namep = seed.namep;
                            pTree->yr_of_establishment = yearposition;
                            pTree->line = seed.line;
                            pTree->generation = seed.generation;
                            pTree->dbasal = basalgrowth_help;
                            pTree->dbasalmax = maxbw_help;
                            pTree->dbasalrel = 1.0;
                            pTree->dbreast = 0.0;
                            pTree->dbreastrel = 1.0;

                            if (parameter[0].allometryfunctiontype == 1) {
                                pTree->height = parameter[0].dbasalheightalloslope * pow(maxbw_help, parameter[0].dbasalheightalloexp);
                            } else {
                                pTree->height = parameter[0].dbasalheightslopenonlin * maxbw_help;
                            }
                            pTree->age = 0;
                            pTree->cone = 0;
                            pTree->coneheight = 99999.0;
                            pTree->seednewly_produced = 0;
                            pTree->seedproduced = 0;
                            pTree->buffer = 1;
                            pTree->densitywert = density_help;
                            pTree->dispersaldistance = seed.dispersaldistance;
                            pTree->growing = true;
                            pTree->species = seed.species;
                            pTree->thawing_depthinfluence = thawing_depthinfluence_help;

                            tree_list.push_back(pTree);

                            seed.dead = true;
                            seed_list.remove(i_seed);
                        }
                    }
                } else if (seed.species == 2) {
                    if (zufallsz
                        < (
							parameter[0].germinationrate // background germination rate
							+ (basalgrowth_help/maxbw_help) // rel growth on position is density dependent
							* parameter[0].germinatioweatherinfluence * pow(( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / seed.ycoo) ) ), 2.0) // weather influence
							* keimungauflageinfluence // litter layer dependency
						)) {
                        if (maxbw_help > 0.0) {
                            auto pTree = new Tree();

                            pTree->yworldcoo = seed.yworldcoo;
                            pTree->xworldcoo = seed.xworldcoo;
                            pTree->xcoo = seed.xcoo;
                            pTree->ycoo = seed.ycoo;
                            pTree->name = ++parameter[0].nameakt;
                            pTree->namem = seed.namem;
                            pTree->namep = seed.namep;
                            pTree->yr_of_establishment = yearposition;
                            pTree->line = seed.line;
                            pTree->generation = seed.generation;
                            pTree->dbasal = basalgrowth_help;
                            pTree->dbasalmax = maxbw_help;
							pTree->dbasalrel = 1.0;
                            pTree->dbreast = 0.0;
                            pTree->dbreastrel = 1.0;

                            if (parameter[0].allometryfunctiontype == 1) {
                                pTree->height = parameter[0].dbasalheightalloslope * pow(maxbw_help, parameter[0].dbasalheightalloexp);
                            } else {
                                pTree->height = parameter[0].dbasalheightslopenonlin * maxbw_help;
                            }

                            pTree->age = 0;
                            pTree->cone = 0;
                            pTree->coneheight = 99999.0;
                            pTree->seednewly_produced = 0;
                            pTree->seedproduced = 0;
                            pTree->buffer = 1;
                            pTree->densitywert = density_help;
                            pTree->thawing_depthinfluence = thawing_depthinfluence_help;
                            pTree->dispersaldistance = seed.dispersaldistance;
                            pTree->growing = true;
                            pTree->species = seed.species;
                            tree_list.push_back(pTree);

                            seed.dead = true;
                            seed_list.remove(i_seed);
                        }
                    }
                } else {
                    seed.dead = true;
                    seed_list.remove(i_seed);
                }
            }
        
		}
    }
}
