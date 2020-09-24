#include "LAVESI.h"

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

double getMaxbasalwachstum(int yearposition, vector<Weather*>& weather_list, Tree* pTree) {
    double maxbw_help = 0;
    if (parameter[0].lineartransect) {
        if (parameter[0].thawing_depth) {
            if (pTree->species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                        + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)
					* ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / pTree->ycoo) ) )
                    * (((double)pTree->thawing_depthinfluence) / 100);
            } else if (pTree->species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)
 					* ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / pTree->ycoo) ) ) * ((((double)pTree->thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (pTree->species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                        + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)
					* ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / pTree->ycoo) ) );
            } else if (pTree->species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)
					* ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / pTree->ycoo) ) );
            }
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
            if (pTree->species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                        + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)
					// * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / pTree->elevation) ) )
					* (weather_list[yearposition]->weatherfactorg*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))))
                    * (((double)pTree->thawing_depthinfluence) / 100);
            } else if (pTree->species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)
 					// * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / pTree->ycoo) ) )
					* (weather_list[yearposition]->weatherfactors*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))))
					* ((((double)pTree->thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
// cout << "ele=" << pTree->elevation << " => rel infl " << ( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)) << endl;
        } else {
            if (pTree->species == 1) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                        + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)
					// * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / pTree->ycoo) ) );
 					* (weather_list[yearposition]->weatherfactorg*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))));
           } else if (pTree->species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)
					// * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / pTree->ycoo) ) );
 					* (weather_list[yearposition]->weatherfactors*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))));
           }
        }
		
/* // moved to growth
		// slope impact with minimum threshold, maxima and maximum limit
		// TODO: insert switch
		double peakslopegrowth = 10.0;
		if ( (pTree->slope<5.0) | (pTree->slope>90.0)) // limits
			maxbw_help=0.0;
		// else
			// maxbw_help = maxbw_help * (1-fabs(pTree->slope-47.5)/47.5);
		else if (pTree->slope<(5+(peakslopegrowth-5)))// assuming peak at 15
			maxbw_help = maxbw_help * (1-fabs(pTree->slope-peakslopegrowth)/(peakslopegrowth-5));
		else if (pTree->slope>=(5+(peakslopegrowth-5)))// assuming peak at 15
			maxbw_help = maxbw_help * (1-(pTree->slope-(5+(peakslopegrowth-5)))/(90-(5+(peakslopegrowth-5))) );
// cout << "slope=" << pTree->slope << " => factor (5min..90max..via47.5max)=" << (1-fabs(pTree->slope-47.5)/47.5) << endl;	
*/
    } else {
        if (parameter[0].thawing_depth) {
            if (pTree->species == 1) {
                maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                 + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)
                             * weather_list[yearposition]->weatherfactorg * (((double)pTree->thawing_depthinfluence) / 100);
            } else if (pTree->species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)
                    * weather_list[yearposition]->weatherfactors * ((((double)pTree->thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (pTree->species == 1) {
                maxbw_help = exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                 + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)
                             * weather_list[yearposition]->weatherfactorg;
            } else if (pTree->species == 2) {
                maxbw_help =
                    exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)
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
double getMaxbreastwachstum(int yearposition, vector<Weather*>& weather_list, Tree* pTree) {
    double maxbrw_help = 0;

    if (parameter[0].lineartransect == true) {
        if (parameter[0].thawing_depth == true) {
            if (pTree->species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * pTree->dbreast
                        + parameter[0].gdbreastfacqgmel * pTree->dbreast * pTree->dbreast)
                    * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * pTree->ycoo
                       + weather_list[yearposition]->weatherfactorming)
                    * (((double)pTree->thawing_depthinfluence) / 100);
            } else if (pTree->species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * pTree->dbreast
                        + parameter[0].gdbreastfacqsib * pTree->dbreast * pTree->dbreast)
                    * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * pTree->ycoo
                       + weather_list[yearposition]->weatherfactormins)
                    * ((((double)pTree->thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (pTree->species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * pTree->dbreast
                        + parameter[0].gdbreastfacqgmel * pTree->dbreast * pTree->dbreast)
                    * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * pTree->ycoo
                       + weather_list[yearposition]->weatherfactorming);
            } else if (pTree->species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * pTree->dbreast
                        + parameter[0].gdbreastfacqsib * pTree->dbreast * pTree->dbreast)
                    * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * pTree->ycoo
                       + weather_list[yearposition]->weatherfactormins);
            }
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
            if (pTree->species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * pTree->dbasal
                        + parameter[0].gdbreastfacqgmel * pTree->dbasal * pTree->dbasal)
					// * ( weather_list[yearposition]->weatherfactorg - ( ( weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming ) * 1.0/(((double)treerows) / pTree->elevation) ) )
					* (weather_list[yearposition]->weatherfactorg*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))))
                    * (((double)pTree->thawing_depthinfluence) / 100);
            } else if (pTree->species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacgmel * pTree->dbasal + parameter[0].gdbreastfacqsib * pTree->dbasal * pTree->dbasal)
 					// * ( weather_list[yearposition]->weatherfactors - ( ( weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins ) * 1.0/(((double)treerows) / pTree->ycoo) ) )
					* (weather_list[yearposition]->weatherfactors*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))))
					* ((((double)pTree->thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
// cout << "ele=" << pTree->elevation << " => rel infl " << ( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000)) << endl;
        } else {
            if (pTree->species == 1) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * pTree->dbreast
                        + parameter[0].gdbreastfacqgmel * pTree->dbreast * pTree->dbreast)
                    // * (((weather_list[yearposition]->weatherfactorg - weather_list[yearposition]->weatherfactorming) / ((double)treerows)) * pTree->ycoo + weather_list[yearposition]->weatherfactorming);
					* (weather_list[yearposition]->weatherfactorg*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactorming*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))));
            } else if (pTree->species == 2) {
                maxbrw_help =
                    exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * pTree->dbreast
                        + parameter[0].gdbreastfacqsib * pTree->dbreast * pTree->dbreast)
                    // * (((weather_list[yearposition]->weatherfactors - weather_list[yearposition]->weatherfactormins) / ((double)treerows)) * pTree->ycoo + weather_list[yearposition]->weatherfactormins);
					* (weather_list[yearposition]->weatherfactors*( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))) + (weather_list[yearposition]->weatherfactormins*(1-( pTree->elevation-(parameter[0].elevationoffset+1000))/(parameter[0].elevationoffset-(parameter[0].elevationoffset+1000))));
            }
        }
/* // moved to growth
		// slope impact with minimum threshold, maxima and maximum limit
		// TODO: insert switch
		double peakslopegrowth = 10.0;
		if ( (pTree->slope<5.0) | (pTree->slope>90.0)) // limits
			maxbrw_help=0.0;
		else if (pTree->slope<(5+(peakslopegrowth-5)))// assuming peak at 15
			// maxbrw_help = maxbrw_help * (1-fabs(pTree->slope-47.5)/47.5);
			maxbrw_help = maxbrw_help * (1-fabs(pTree->slope-peakslopegrowth)/(peakslopegrowth-5));
		else if (pTree->slope>=(5+(peakslopegrowth-5)))// assuming peak at 15
			// maxbrw_help = maxbrw_help * (1-fabs(pTree->slope-47.5)/47.5);
			maxbrw_help = maxbrw_help * (1-(pTree->slope-(5+(peakslopegrowth-5)))/(90-(5+(peakslopegrowth-5))) );
*/
	} else {
        if (parameter[0].thawing_depth == true) {
            if (pTree->species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * pTree->dbreast
                                  + parameter[0].gdbreastfacqgmel * pTree->dbreast * pTree->dbreast)
                              * weather_list[yearposition]->weatherfactorg * (((double)pTree->thawing_depthinfluence) / 100);
            } else if (pTree->species == 2) {
                maxbrw_help = exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * pTree->dbreast
                                  + parameter[0].gdbreastfacqsib * pTree->dbreast * pTree->dbreast)
                              * weather_list[yearposition]->weatherfactors * ((((double)pTree->thawing_depthinfluence * 0.8) / 100) - 0.6);
            }
        } else {
            if (pTree->species == 1) {
                maxbrw_help = exp(parameter[0].gdbreastconstgmel + parameter[0].gdbreastfacgmel * pTree->dbreast
                                  + parameter[0].gdbreastfacqgmel * pTree->dbreast * pTree->dbreast)
                              * weather_list[yearposition]->weatherfactorg;
            } else if (pTree->species == 2) {
                maxbrw_help = exp(parameter[0].gdbreastconstsib + parameter[0].gdbreastfacsib * pTree->dbreast
                                  + parameter[0].gdbreastfacqsib * pTree->dbreast * pTree->dbreast)
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
void Growth(struct Parameter* parameter, int yearposition, vector<list<Tree*>>& world_tree_list, vector<vector<Weather*>>& world_weather_list) {
    int aktort = 0;
    for (vector<list<Tree*>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        list<Tree*>& tree_list = *posw;

        vector<vector<Weather*>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather*>& weather_list = *posiwelt;

        aktort++;

        for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
            auto pTree = (*pos);

            double maxbasalwachstum = 0.0;

            maxbasalwachstum = getMaxbasalwachstum(yearposition, weather_list, pTree);
			pTree->dbasalmax = maxbasalwachstum;

            double basalwachstum = maxbasalwachstum * ((double)pTree->buffer) * (1.0 - pTree->densitywert);
            basalwachstum = basalwachstum + basalwachstum * parameter[0].basalinfluenceoldyoung * pTree->dbasal;
			
			// slope impact with minimum threshold, maxima and maximum limit
			// TODO: insert switch
			double peakslopegrowth = 10.0;
			if ( (pTree->slope<5.0) | (pTree->slope>90.0)) // limits
				basalwachstum=0.0;
			else if (pTree->slope<(5+(peakslopegrowth-5)))// assuming peak at 15
				basalwachstum = basalwachstum * (1-fabs(pTree->slope-peakslopegrowth)/(peakslopegrowth-5));
			else if (pTree->slope>=(5+(peakslopegrowth-5)))// assuming peak at 15
				basalwachstum = basalwachstum * (1-(pTree->slope-(5+(peakslopegrowth-5)))/(90-(5+(peakslopegrowth-5))) );
			
            if (basalwachstum < 0.0) {
                basalwachstum = 0.0;
            }

            if (pTree->growing == true) {
                pTree->dbasal += basalwachstum;
            }
// cout << pTree->dbasalmax << " + " << pTree->dbasal << " + " << pTree->elevation << " + " << pTree->slope << " + " << pTree->densitywert << " + " << pTree->buffer << endl;

            if (parameter[0].relgrowthinfluence == 0) {
                pTree->dbasalrel = 1.0;
            } else if (parameter[0].relgrowthinfluence == 1) {
				if(maxbasalwachstum<=0.0)
					pTree->dbasalrel = 0.0;
				else
					pTree->dbasalrel = basalwachstum / (maxbasalwachstum + (maxbasalwachstum * parameter[0].basalinfluenceoldyoung * pTree->dbasal));
            }

            double maxbreastwachstum = 0;
            double breastwachstum = 0;

            if (pTree->height >= 130.0) {
                maxbreastwachstum = getMaxbreastwachstum(yearposition, weather_list, pTree);

                breastwachstum = maxbreastwachstum * ((double)pTree->buffer) * (1.0 - pTree->densitywert);

				// slope impact with minimum threshold, maxima and maximum limit
				// TODO: insert switch
				if ( (pTree->slope<5.0) | (pTree->slope>90.0)) // limits
					breastwachstum=0.0;
				else if (pTree->slope<(5+(peakslopegrowth-5)))// assuming peak at 15
					breastwachstum = breastwachstum * (1-fabs(pTree->slope-peakslopegrowth)/(peakslopegrowth-5));
				else if (pTree->slope>=(5+(peakslopegrowth-5)))// assuming peak at 15
					breastwachstum = breastwachstum * (1-(pTree->slope-(5+(peakslopegrowth-5)))/(90-(5+(peakslopegrowth-5))) );

                if (breastwachstum < 0.0) {
                    breastwachstum = 0.0;
                }

                if (pTree->growing == true) {
                    pTree->dbreast += breastwachstum;
                }

                if (parameter[0].relgrowthinfluence == 0) {
                    pTree->dbreastrel = 1.0;
                } else if (parameter[0].relgrowthinfluence == 1) {
					if(maxbreastwachstum<=0.0)
						pTree->dbreastrel = 0.0;
					else
						pTree->dbreastrel = breastwachstum / maxbreastwachstum;
                }
            }

            // tree height update
            if (pTree->height < 130.0) {
                if (parameter[0].allometryfunctiontype == 1) {
                    pTree->height = parameter[0].dbasalheightalloslope * pow(pTree->dbasal, parameter[0].dbasalheightalloexp);
                } else {
                    pTree->height = parameter[0].dbasalheightslopenonlin * pTree->dbasal;
                }
            } else if (pTree->height >= 130.0) {
                if (parameter[0].allometryfunctiontype == 1) {
                    pTree->height = parameter[0].dbreastheightalloslope * pow(pTree->dbreast, parameter[0].dbreastheightalloexp) + 130.0;
                } else {
                    pTree->height = pow((parameter[0].dbreastheightslopenonlin * pow(pTree->dbreast, 0.5)), 2) + 130.0;
                }
            } else {
                // check if the substructure height defined
                delete pTree;
                pos = tree_list.erase(pos);
            }

            ++pos;
        }
    }
}
