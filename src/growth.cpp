#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

double getMaxbasalwachstum(int yearposition, vector<Weather*>& weather_list, Tree tree) {
    double maxbw_help = 0;
    if (parameter[0].lineartransect) {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                    * (weather_list[yearposition]->weatherfactorg
                       + ((weather_list[yearposition]->weatherfactorming - weather_list[yearposition]->weatherfactorg) * ((double)tree.ycoo / 1000)
                          / ((double)treerows)))
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * (weather_list[yearposition]->weatherfactors
                       + ((weather_list[yearposition]->weatherfactormins - weather_list[yearposition]->weatherfactors) * ((double)tree.ycoo / 1000)
                          / ((double)treerows)))
                    * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                    * (weather_list[yearposition]->weatherfactorg
                       + ((weather_list[yearposition]->weatherfactorming - weather_list[yearposition]->weatherfactorg) * ((double)tree.ycoo / 1000)
                          / ((double)treerows)));
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * (weather_list[yearposition]->weatherfactors
                       + ((weather_list[yearposition]->weatherfactormins - weather_list[yearposition]->weatherfactors) * ((double)tree.ycoo / 1000)
                          / ((double)treerows)));
            }
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                    * ((weather_list[yearposition]->weatherfactorg * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactorming
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * ((weather_list[yearposition]->weatherfactors * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactormins
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                    * ((weather_list[yearposition]->weatherfactorg * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactorming
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * ((weather_list[yearposition]->weatherfactors * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactormins
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
            }
        }
    } else {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition]->weatherfactorg * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition]->weatherfactors * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition]->weatherfactorg;
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition]->weatherfactors;
            }
        }
    }

    return maxbw_help;
}

double getMaxbreastwachstum(int yearposition, vector<Weather*>& weather_list, Tree tree) {
    double maxbrw_help = 0;

    if (parameter[0].lineartransect == true) {
        if (parameter[0].thawing_depth == true) {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * (weather_list[yearposition]->weatherfactorg
                                 + ((weather_list[yearposition]->weatherfactorming - weather_list[yearposition]->weatherfactorg) * ((double)tree.ycoo / 1000)
                                    / ((double)treerows)))
                              * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * (weather_list[yearposition]->weatherfactors
                       + ((weather_list[yearposition]->weatherfactormins - weather_list[yearposition]->weatherfactors) * ((double)tree.ycoo / 1000)
                          / ((double)treerows)))
                    * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * (weather_list[yearposition]->weatherfactorg
                                 + ((weather_list[yearposition]->weatherfactorming - weather_list[yearposition]->weatherfactorg) * ((double)tree.ycoo / 1000)
                                    / ((double)treerows)));
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * (weather_list[yearposition]->weatherfactors
                       + ((weather_list[yearposition]->weatherfactormins - weather_list[yearposition]->weatherfactors) * ((double)tree.ycoo / 1000)
                          / ((double)treerows)));
            }
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbasal + parameter[0].gdbreastfacqgmel * tree.dbasal * tree.dbasal)
                    * ((weather_list[yearposition]->weatherfactorg * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactorming
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacgmel * tree.dbasal + parameter[0].gdbreastfacqsib * tree.dbasal * tree.dbasal)
                    * ((weather_list[yearposition]->weatherfactors * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactormins
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * ((weather_list[yearposition]->weatherfactorg * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                 + (weather_list[yearposition]->weatherfactorming
                                    * (1
                                       - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * ((weather_list[yearposition]->weatherfactors * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition]->weatherfactormins
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
            }
        }
    } else {
        if (parameter[0].thawing_depth == true) {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition]->weatherfactorg * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * weather_list[yearposition]->weatherfactors * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition]->weatherfactorg;
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * weather_list[yearposition]->weatherfactors;
            }
        }
    }
    return (maxbrw_help);
}

void Growth(struct Parameter* parameter, int yearposition, vector<VectorList<Tree>>& world_tree_list, vector<vector<Weather*>>& world_weather_list) {
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;
        vector<vector<Weather*>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather*>& weather_list = *posiwelt;
        aktort++;

#pragma omp parallel for default(shared) schedule(guided)
        for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
            auto& tree = tree_list[tree_i];
            double maxbasalwachstum = 0.0;
            maxbasalwachstum = getMaxbasalwachstum(yearposition, weather_list, tree);
            tree.dbasalmax = 1000 * maxbasalwachstum;

            double basalwachstum = maxbasalwachstum * (1.0 - tree.densitywert);
            basalwachstum = basalwachstum + basalwachstum * parameter[0].basalinfluenceoldyoung * tree.dbasal;

            if (parameter[0].demlandscape) {
                basalwachstum = basalwachstum * (double)tree.envirimpact / 10000;
            }

            if (basalwachstum < 0.0) {
                basalwachstum = 0.0;
            }

            if (tree.growing == true) {
                tree.dbasal += basalwachstum;
            }

            if (parameter[0].relgrowthinfluence == 0) {
                tree.dbasalrel = 1000;
            } else if (parameter[0].relgrowthinfluence == 1) {
                if (maxbasalwachstum <= 0.0)
                    tree.dbasalrel = 0;
                else
                    tree.dbasalrel = 1000 * basalwachstum / (maxbasalwachstum + maxbasalwachstum * parameter[0].basalinfluenceoldyoung * tree.dbasal);
            }

            double maxbreastwachstum = 0;
            double breastwachstum = 0;

            if ((double)tree.height / 100 >= 130) {
                maxbreastwachstum = getMaxbreastwachstum(yearposition, weather_list, tree);

                breastwachstum = maxbreastwachstum * (1.0 - tree.densitywert);

                if (parameter[0].demlandscape) {
                    breastwachstum = breastwachstum * (double)tree.envirimpact / 10000;
                }

                if (breastwachstum < 0.0) {
                    breastwachstum = 0.0;
                }

                if (tree.growing == true) {
                    tree.dbreast += breastwachstum;
                }

                if (parameter[0].relgrowthinfluence == 0) {
                    tree.dbreastrel = 1000;
                } else if (parameter[0].relgrowthinfluence == 1) {
                    if (maxbreastwachstum <= 0.0)
                        tree.dbreastrel = 0;
                    else
                        tree.dbreastrel = 1000 * breastwachstum / maxbreastwachstum;
                }
            }

            // tree height update
            if ((double)tree.height / 100 < 130) {
                if (parameter[0].allometryfunctiontype == 1) {
                    tree.height = 100 * parameter[0].dbasalheightalloslope * pow(tree.dbasal, parameter[0].dbasalheightalloexp);
                } else {
                    tree.height = 100 * parameter[0].dbasalheightslopenonlin * tree.dbasal;
                }
            } else if ((double)tree.height / 100 >= 130) {
                if (parameter[0].allometryfunctiontype == 1) {
                    tree.height = 100 * (parameter[0].dbreastheightalloslope * pow(tree.dbreast, parameter[0].dbreastheightalloexp) + 130.0);
                } else {
                    tree.height = 100 * (pow((parameter[0].dbreastheightslopenonlin * pow(tree.dbreast, 0.5)), 2) + 130.0);
                }
            }
        }
    }
}
