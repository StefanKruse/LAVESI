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

                double keimungauflageinfluence =
                    (1.0 - 0.01) / (200.0 - 600.0) * ((double)plot_list[i * treecols * parameter[0].sizemagnif + j]->litterheight) + 1.495;

                if (keimungauflageinfluence < 0.01) {
                    keimungauflageinfluence = 0.01;
                }

                // ... and weather.
                // calculate the latest growth performance
                double maxbasalwachstum = 0;

                if (parameter[0].lineartransect == true) {
                    if (seed.species == 1) {
                        maxbasalwachstum =
                            exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                            * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * seed.ycoo
                               + weather_list[yearposition]->weatherfactorming);
                    } else if (seed.species == 2) {
                        maxbasalwachstum =
                            exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                            * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * seed.ycoo
                               + weather_list[yearposition]->weatherfactormins);
                    }
                } else {
                    if (seed.species == 1) {
                        maxbasalwachstum = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * 0 + parameter[0].gdbasalfacqgmel * 0 * 0)
                                           * weather_list[yearposition]->weatherfactorg;
                    } else if (seed.species == 2) {
                        maxbasalwachstum = exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * 0 + parameter[0].gdbasalfacqsib * 0 * 0)
                                           * weather_list[yearposition]->weatherfactors;
                    }
                }

                double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));

                // add new tree if seed germinates
                // minimal germination rate is roughly estimated
                if (seed.species == 1) {
                    if (zufallsz
                        < (parameter[0].germinationrate
                           + (parameter[0].germinatioweatherinfluence * maxbasalwachstum / exp(parameter[0].gdbasalconstgmel)) * keimungauflageinfluence)) {
                        if (maxbasalwachstum > 0.0) {
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
                            pTree->dbasal = maxbasalwachstum;
                            pTree->dbasalrel = 1.0;
                            pTree->dbreast = 0.0;
                            pTree->dbreastrel = 1.0;

                            if (parameter[0].allometryfunctiontype == 1) {
                                pTree->height = parameter[0].dbasalheightalloslope * pow(maxbasalwachstum, parameter[0].dbasalheightalloexp);
                            } else {
                                pTree->height = parameter[0].dbasalheightslopenonlin * maxbasalwachstum;
                            }
                            pTree->age = 0;
                            pTree->cone = 0;
                            pTree->coneheight = 99999.0;
                            pTree->seednewly_produced = 0;
                            pTree->seedproduced = 0;
                            pTree->buffer = 1;
                            pTree->densitywert = 0;
                            pTree->dispersaldistance = seed.dispersaldistance;
                            pTree->growing = true;
                            pTree->species = seed.species;
                            pTree->thawing_depthinfluence = 100;

                            tree_list.push_back(pTree);

                            seed.dead = true;
                            seed_list.remove(i_seed);
                        }
                    }
                } else if (seed.species == 2) {
                    if (zufallsz
                        < (parameter[0].germinationrate
                           + (parameter[0].germinatioweatherinfluence * maxbasalwachstum / exp(parameter[0].gdbasalconstsib)) * keimungauflageinfluence)) {
                        if (maxbasalwachstum > 0.0) {
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
                            pTree->dbasal = maxbasalwachstum;
                            pTree->dbasalrel = 1.0;
                            pTree->dbreast = 0.0;
                            pTree->dbreastrel = 1.0;

                            if (parameter[0].allometryfunctiontype == 1) {
                                pTree->height = parameter[0].dbasalheightalloslope * pow(maxbasalwachstum, parameter[0].dbasalheightalloexp);
                            } else {
                                pTree->height = parameter[0].dbasalheightslopenonlin * maxbasalwachstum;
                            }

                            pTree->age = 0;
                            pTree->cone = 0;
                            pTree->coneheight = 99999.0;
                            pTree->seednewly_produced = 0;
                            pTree->seedproduced = 0;
                            pTree->buffer = 1;
                            pTree->densitywert = 0;
                            pTree->thawing_depthinfluence = 100;
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
