#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

double getMaxbasalwachstum(int yearposition, 
						   vector<Weather>& weather_list, 
						   Tree tree) {
    double maxbw_help = 0;
    if (parameter[0].lineartransect) {
        if (parameter[0].thawing_depth) {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* (weather_list[yearposition].weatherfactor[tree.species]
				   + ((weather_list[yearposition].weatherfactormin[tree.species] - weather_list[yearposition].weatherfactor[tree.species]) * ((double)tree.ycoo / 1000)
					  / ((double)treerows)))
				* (((double)tree.thawing_depthinfluence) / 100);
        } else {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* (weather_list[yearposition].weatherfactor[tree.species]
				   + ((weather_list[yearposition].weatherfactormin[tree.species] - weather_list[yearposition].weatherfactor[tree.species]) * ((double)tree.ycoo / 1000)
					  / ((double)treerows)));
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* ((weather_list[yearposition].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
					/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
				   + (weather_list[yearposition].weatherfactormin[tree.species]
					  * (1
						 - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
							   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
				* (((double)tree.thawing_depthinfluence) / 100);
        } else {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* ((weather_list[yearposition].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
					/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
				   + (weather_list[yearposition].weatherfactormin[tree.species]
					  * (1
						 - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
							   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
        }
    } else {
        if (parameter[0].thawing_depth) {
                maxbw_help =
                    exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition].weatherfactor[tree.species] * (((double)tree.thawing_depthinfluence) / 100);
        } else {
                maxbw_help =
                    exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition].weatherfactor[tree.species];
        }
    }

    return maxbw_help;
}

double getMaxbreastwachstum(int yearposition, 
							vector<Weather>& weather_list, 
							Tree tree) {
    double maxbrw_help = 0;

    if (parameter[0].lineartransect == true) {
        if (parameter[0].thawing_depth == true) {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * (weather_list[yearposition].weatherfactor[tree.species]
                                 + ((weather_list[yearposition].weatherfactormin[tree.species] - weather_list[yearposition].weatherfactor[tree.species]) * ((double)tree.ycoo / 1000)
                                    / ((double)treerows)))
                              * (((double)tree.thawing_depthinfluence) / 100);
        } else {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * (weather_list[yearposition].weatherfactor[tree.species]
                                 + ((weather_list[yearposition].weatherfactormin[tree.species] - weather_list[yearposition].weatherfactor[tree.species]) * ((double)tree.ycoo / 1000)
                                    / ((double)treerows)));
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
                maxbrw_help =
                    exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                    * ((weather_list[yearposition].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                        / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                       + (weather_list[yearposition].weatherfactormin[tree.species]
                          * (1
                             - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * (((double)tree.thawing_depthinfluence) / 100);
        } else {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * ((weather_list[yearposition].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                 + (weather_list[yearposition].weatherfactormin[tree.species]
                                    * (1
                                       - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
        }
    } else {
        if (parameter[0].thawing_depth == true) {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition].weatherfactor[tree.species] * (((double)tree.thawing_depthinfluence) / 100);
        } else {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition].weatherfactor[tree.species];
        }
    }
    return (maxbrw_help);
}

bool IsFiniteNumber(double x) {
        return (x <= DBL_MAX && x >= -DBL_MAX); 
} 
						   
void Growth(Parameter* parameter, int yearposition, vector<VectorList<Tree>>& world_tree_list, vector<vector<Weather>>& world_weather_list) {
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;
        vector<vector<Weather>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather>& weather_list = *posiwelt;
        aktort++;

#pragma omp parallel for default(shared) schedule(guided)
        for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
            auto& tree = tree_list[tree_i];
			
			if (tree.growing == true) {
				double maxbasalwachstum = 0.0;
				maxbasalwachstum = getMaxbasalwachstum(yearposition, weather_list, tree);

				tree.dbasalmax = 1000 * maxbasalwachstum;

				double basalwachstum = maxbasalwachstum * (1.0 - tree.densitywert);

				if (parameter[0].demlandscape) {
					basalwachstum = basalwachstum * (double)tree.envirimpact / 10000;
				}

				if (basalwachstum < 0.0) {
					basalwachstum = 0.0;
				}

				// check for real values
				if( (IsFiniteNumber( basalwachstum ) == false) || (basalwachstum > 10.0) || (IsFiniteNumber(tree.dbasal) == false) || (tree.dbasal > 5000.0) ) {
					cout << "tree growth unlimited" << basalwachstum << endl;
					cout << "tree.dbasal = " << tree.dbasal << endl;
					basalwachstum = 0.0;
				if (tree.growing == true) {
					tree.dbasal += basalwachstum;
				}

				tree.dbasalrel = 1000;
				if(parameter[0].relgrowthinfluence == 1) {
					if (maxbasalwachstum <= 0.0) {
						tree.dbasalrel = 0;
					} else {
						tree.dbasalrel = 1000 * (basalwachstum / maxbasalwachstum) * tree.soilhumidity;
					}
				}

				// tree height update
				if (parameter[0].allometryfunctiontype == 3) {// logistic growth
					tree.height = 10 * exp(speciestrait[tree.species].heightloga/(1+exp((speciestrait[tree.species].heightlogb-log(tree.dbasal*10))/speciestrait[tree.species].heightlogc)));
				} else {
					if ((double)tree.height / 10 < 130) {
						if (parameter[0].allometryfunctiontype == 1) {
							tree.height = 10 * speciestrait[tree.species].dbasalheightalloslope * pow(tree.dbasal, speciestrait[tree.species].dbasalheightalloexp);
						} else {
							tree.height = 10 * speciestrait[tree.species].dbasalheightslopenonlin * tree.dbasal;
						}
					} else if ((double)tree.height / 10 >= 130) {
						if (parameter[0].allometryfunctiontype == 1) {
							tree.height = 10 * (speciestrait[tree.species].dbreastheightalloslope * pow(tree.dbreast, speciestrait[tree.species].dbreastheightalloexp) + 130.0);
						} else {
							tree.height = 10 * (pow((speciestrait[tree.species].dbreastheightslopenonlin * pow(tree.dbreast, 0.5)), 2) + 130.0);
						}
					}
				}

				double maxbreastwachstum = 0;
				double breastwachstum = 0;

				if ((double)tree.height / 10 >= 130) {
					maxbreastwachstum = getMaxbreastwachstum(yearposition, weather_list, tree);

					breastwachstum = maxbreastwachstum * (1.0 - tree.densitywert);

					if (parameter[0].demlandscape) {
						breastwachstum = breastwachstum * (double)tree.envirimpact / 10000;
					}

					if (breastwachstum < 0.0) {
						breastwachstum = 0.0;
					}

					// check for real values
					if( (IsFiniteNumber( breastwachstum ) == false) || (breastwachstum > 10.0) || (IsFiniteNumber(tree.dbreast) == false) || (tree.dbreast > 5000.0) ) {
						cout << "tree growth breast unlimited = " << breastwachstum << endl;
						cout << "tree.dbreast = " << tree.dbreast << endl;
						breastwachstum = 0.0;
					if (tree.growing == true) {
						tree.dbreast += breastwachstum;
					}

					tree.dbreastrel = 1000;
					if (parameter[0].relgrowthinfluence == 1) {
						if (maxbreastwachstum <= 0.0)
							tree.dbreastrel = 0;
						else
							tree.dbreastrel = 1000 * (breastwachstum / maxbreastwachstum) * tree.soilhumidity;
					}
				}
			}
		}
    }
}

