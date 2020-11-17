#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

/****************************************************************************************/
/**
 * \brief calculate max. possible basal growth
 *
 * either just set to weather_list[yearposition]->weatherfactor *
 *exponential growth depending on dbh or, if transects are
 *calculated, calculated out of
 *weather_list[yearposition]->weatherfactormin (max basal growth at
 *most southern transekt and tree position in north-south transekt
 *
 *******************************************************************************************/

double getMaxbasalwachstum(int yearposition, vector<Weather*>& weather_list, Tree tree) {
    double maxbw_help = 0;
    if (parameter[0].lineartransect) {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                        + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
					* ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / (double)tree.ycoo/1000) ) )
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
 					* ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / (double)tree.ycoo/1000) ) )
					* ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                        + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
					* ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / (double)tree.ycoo/1000) ) );
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
					* ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / (double)tree.ycoo/1000) ) );
            }
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                        + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
					// * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / tree.elevation) ) )
					* ((weather_list[yearposition]->weatherfactorg*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( ((double)tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))))
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
 					// * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / tree.ycoo) ) )
					* ((weather_list[yearposition]->weatherfactors*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))))
					* ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
// cout << "ele=" << tree.elevation << " => rel infl " << ( tree.elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)) << endl;
        } else {
            if (tree.species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                        + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
					// * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / tree.ycoo) ) );
 					* ((weather_list[yearposition]->weatherfactorg*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))));
           } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
					// * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / tree.ycoo) ) );
 					* ((weather_list[yearposition]->weatherfactors*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))));
           }
        }
    } else {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                                 + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
                             * weather_list[yearposition]->weatherfactorg * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * tree.dbasal + parameter[0].gdbasalfacqsib * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition]->weatherfactors * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * tree.dbasal
                                 + parameter[0].gdbasalfacqgmel * tree.dbasal * tree.dbasal)
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

/****************************************************************************************/
/**
 * \brief calculate min. possible growth at breast height
 *
 * either taken from just set to
 *weather_list[yearposition]->maxbreastwachstumjahr \n or if
 *transects are calculated calculated out of
 *weather_list[yearposition]->maxbasalwachstumjahrmin and tree
 *position in north-south transect
 *
 *******************************************************************************************/
double getMaxbreastwachstum(int yearposition, vector<Weather*>& weather_list, Tree tree) {
    double maxbrw_help = 0;

    if (parameter[0].lineartransect == true) {
        if (parameter[0].thawing_depth == true) {
            if (tree.species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                        + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                    * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * (double)tree.ycoo/1000
                       + weather_list[yearposition]->weatherfactorming)
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast
                        + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * (double)tree.ycoo/1000
                       + weather_list[yearposition]->weatherfactormins)
                    * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                        + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                    * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * (double)tree.ycoo/1000
                       + weather_list[yearposition]->weatherfactorming);
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast
                        + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                    * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * (double)tree.ycoo/1000
                       + weather_list[yearposition]->weatherfactormins);
            }
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
            if (tree.species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbasal
                        + parameter[0].gdbreastfacqgmel * tree.dbasal * tree.dbasal)
					// * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / tree.elevation) ) )
					* ((weather_list[yearposition]->weatherfactorg*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))))
                    * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacgmel * tree.dbasal + parameter[0].gdbreastfacqsib * tree.dbasal * tree.dbasal)
 					// * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / tree.ycoo) ) )
					* ((weather_list[yearposition]->weatherfactors*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))))
					* ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
// cout << "ele=" << tree.elevation << " => rel infl " << ( tree.elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)) << endl;
        } else {
            if (tree.species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                        + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
						// * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * tree.ycoo + weather_list[yearposition]->weatherfactorming);
						* ((weather_list[yearposition]->weatherfactorg*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))));
            } else if (tree.species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast
                        + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
						// * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * tree.ycoo + weather_list[yearposition]->weatherfactormins);
						* ((weather_list[yearposition]->weatherfactors*( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( ((double) tree.elevation/10)-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)))));
            }
        }
	} else {
        if (parameter[0].thawing_depth == true) {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition]->weatherfactorg * (((double)tree.thawing_depthinfluence) / 100);
            } else if (tree.species == 2) {
                maxbrw_help = exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast
                                  + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition]->weatherfactors * ((((double)tree.thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (tree.species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * tree.dbreast
                                  + parameter[0].gdbreastfacqgmel * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition]->weatherfactorg;
            } else if (tree.species == 2) {
                maxbrw_help = exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * tree.dbreast
                                  + parameter[0].gdbreastfacqsib * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition]->weatherfactors;
            }
        }
    }
    return (maxbrw_help);
}

/****************************************************************************************/
/**
 * \brief calculate basal and breast height growth of each tree in
 *the simulation
 *
 * basalwachstum = maxbasalwachstum * densitywert *
 *thawing_depthinfluence/100;	\n basalwachstum = basalwachstum
 *+ basalwachstum * basalinfluenceoldyoung * dbasal; \n
 * breastwachstum = maxbreastwachstum * densitywert *
 *thawing_depthinfluence)/100;  \n
 *
 *******************************************************************************************/
void Growth(struct Parameter* parameter, int yearposition, vector<VectorList<Tree>>& world_tree_list, vector<vector<Weather*>>& world_weather_list) {
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;

        vector<vector<Weather*>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather*>& weather_list = *posiwelt;

        aktort++;
		
// pragma omp initializing
omp_set_dynamic(1); //enable dynamic teams
omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

#pragma omp parallel
{
#pragma omp for
		for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
			auto& tree = tree_list[tree_i];

            double maxbasalwachstum = 0.0;

            maxbasalwachstum = getMaxbasalwachstum(yearposition, weather_list, tree);
			tree.dbasalmax = (unsigned short int) floor(1000*maxbasalwachstum);

            // double basalwachstum = maxbasalwachstum * ((double)tree.buffer) * (1.0 - tree.densitywert);
            double basalwachstum = maxbasalwachstum * (1.0 - tree.densitywert);
            basalwachstum = basalwachstum + basalwachstum * parameter[0].basalinfluenceoldyoung * tree.dbasal;
			
			// slope impact with minimum threshold, maxima and maximum limit
			// TODO: insert switch
			// double peakslopegrowth = 10.0;
			// if ( (tree.slope<5.0) | (tree.slope>90.0)) // limits
				// basalwachstum=0.0;
			// else if (tree.slope<(5+(peakslopegrowth-5)))// assuming peak at 15
				// basalwachstum = basalwachstum * (1-fabs(tree.slope-peakslopegrowth)/(peakslopegrowth-5));
			// else if (tree.slope>=(5+(peakslopegrowth-5)))// assuming peak at 15
				// basalwachstum = basalwachstum * (1-(tree.slope-(5+(peakslopegrowth-5)))/(90-(5+(peakslopegrowth-5))) );
			if (parameter[0].demlandscape) {
				basalwachstum=basalwachstum * (double) tree.envirimpact/10000;
			}
				
            if (basalwachstum < 0.0) {
                basalwachstum = 0.0;
            }

            if (tree.growing == true) {
                tree.dbasal += basalwachstum;
            }
// cout << (double)tree.dbasalmax/1000 << " + " << tree.dbasal << " + " << (double)tree.elevation/10 << " + " << (double)tree.envirimpact/10000 << " + " << tree.densitywert << " => maxbasgrowth=" << maxbasalwachstum << " => ";
// cout << tree.dbasalmax << " + " << tree.dbasal << " + " << tree.elevation << " + " << tree.slope << " + " << tree.densitywert << " + " << tree.buffer << endl;

            if (parameter[0].relgrowthinfluence == 0) {
                tree.dbasalrel = (unsigned short int) floor(1000*1.0);
            } else if (parameter[0].relgrowthinfluence == 1) {
				if(maxbasalwachstum<=0.0)
					tree.dbasalrel = 0;
				else
					tree.dbasalrel = (unsigned short int) floor(1000* (basalwachstum / (maxbasalwachstum + (maxbasalwachstum * parameter[0].basalinfluenceoldyoung * tree.dbasal))) );
            }
// cout << " dbasalrel=" << tree.dbasalrel << endl;

            double maxbreastwachstum = 0;
            double breastwachstum = 0;

            if ((double)tree.height/100 >= 130) {
                maxbreastwachstum = getMaxbreastwachstum(yearposition, weather_list, tree);

                // breastwachstum = maxbreastwachstum * ((double)tree.buffer) * (1.0 - tree.densitywert);
                breastwachstum = maxbreastwachstum * (1.0 - tree.densitywert);

				// slope impact with minimum threshold, maxima and maximum limit
				// TODO: insert switch
				// if ( (tree.slope<5.0) | (tree.slope>90.0)) // limits
					// breastwachstum=0.0;
				// else if (tree.slope<(5+(peakslopegrowth-5)))// assuming peak at 15
					// breastwachstum = breastwachstum * (1-fabs(tree.slope-peakslopegrowth)/(peakslopegrowth-5));
				// else if (tree.slope>=(5+(peakslopegrowth-5)))// assuming peak at 15
					// breastwachstum = breastwachstum * (1-(tree.slope-(5+(peakslopegrowth-5)))/(90-(5+(peakslopegrowth-5))) );
				if (parameter[0].demlandscape) {
					breastwachstum=breastwachstum * (double) tree.envirimpact/10000;
				}
				
                if (breastwachstum < 0.0) {
                    breastwachstum = 0.0;
                }

                if (tree.growing == true) {
                    tree.dbreast += breastwachstum;
                }

                if (parameter[0].relgrowthinfluence == 0) {
                    tree.dbreastrel = (unsigned short int) floor(1000*1.0);
                } else if (parameter[0].relgrowthinfluence == 1) {
					if(maxbreastwachstum<=0.0)
						tree.dbreastrel = 0;
					else
						tree.dbreastrel = (unsigned short int) floor(1000*breastwachstum / maxbreastwachstum);
                }
            }

            // tree height update
            if ((double)tree.height/100 < 130) {
                if (parameter[0].allometryfunctiontype == 1) {
                    tree.height = (unsigned short int) floor(100* parameter[0].dbasalheightalloslope * pow(tree.dbasal, parameter[0].dbasalheightalloexp));
                } else {
                    tree.height = (unsigned short int) floor(100* parameter[0].dbasalheightslopenonlin * tree.dbasal);
                }
            } else if ((double)tree.height/100 >= 130) {
                if (parameter[0].allometryfunctiontype == 1) {
                    tree.height = (unsigned short int) floor(100* (parameter[0].dbreastheightalloslope * pow(tree.dbreast, parameter[0].dbreastheightalloexp) + 130.0) );
                } else {
                    tree.height = (unsigned short int) floor(100* (pow((parameter[0].dbreastheightslopenonlin * pow(tree.dbreast, 0.5)), 2) + 130.0) );
                }
            }
        }
} // pragma
    }
}
