#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

double getMaxbasalwachstum(int yearposition, vector<Weather>& weather_list, Tree tree) {
    double maxbw_help = 0;
	double transectstart = treerows - parameter[0].locationshift;

    if (parameter[0].lineartransect) {
        if (parameter[0].thawing_depth) {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* (weather_list[yearposition-1].weatherfactor[0][tree.species]
				   + ((weather_list[yearposition-1].weatherfactormin[0][tree.species] - weather_list[yearposition-1].weatherfactor[0][tree.species]) * ((double)tree.ycoo / 1000)
					  / ((double)treerows)))
				* (((double)tree.thawing_depthinfluence) / (100*100));
        } else {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* (weather_list[yearposition-1].weatherfactor[0][tree.species]
				   + ((weather_list[yearposition-1].weatherfactormin[0][tree.species] - weather_list[yearposition-1].weatherfactor[0][tree.species]) * ((double)tree.ycoo / 1000)
					  / ((double)treerows)));
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
			if (parameter[0].n_weather_along_grid>0) {
				maxbw_help = pow( speciestrait[tree.species].mindiametergrowth + exp(-0.5 * speciestrait[tree.species].meangrowthq75p * pow(tree.dbasal,0.5)), 0.5 );
				maxbw_help = maxbw_help 
						* ((weighmeanweathervararray(weather_list[yearposition-1].weatherfactor,((double)tree.ycoo/1000)-transectstart,tree.species) * (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))) + (weighmeanweathervararray(weather_list[yearposition-1].weatherfactormin,((double)tree.ycoo/1000)-transectstart,tree.species) * (1 - (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
					* (((double)tree.thawing_depthinfluence) / (100*100) );
			} else {
				// maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal) + speciestrait[tree.species].meangrowthq75p;
				// maxbw_help = pow( speciestrait[tree.species].mindiametergrowth * exp(-0.5 * speciestrait[tree.species].meangrowthq75p * tree.dbasal), 0.5 );
				maxbw_help = pow( speciestrait[tree.species].mindiametergrowth + exp(-0.5 * speciestrait[tree.species].meangrowthq75p * pow(tree.dbasal,0.5)), 0.5 );
				// double maxbw_help_2 = maxbw_help;
					// if ( (tree.dbasal > 50) || (maxbw_help < speciestrait[tree.species].mindiametergrowth) ) {
					// if ( maxbw_help < speciestrait[tree.species].mindiametergrowth ) {
						// maxbw_help = speciestrait[tree.species].mindiametergrowth;
					// }
					
				maxbw_help = maxbw_help 
					// * (
						// (weather_list[yearposition-1].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
						// + (weather_list[yearposition-1].weatherfactormin[tree.species] * (1 -(((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))))
						// )
						* ((weather_list[yearposition-1].weatherfactor[0][tree.species] * (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))) + (weather_list[yearposition-1].weatherfactormin[0][tree.species] * (1 - (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
					* (((double)tree.thawing_depthinfluence) / (100*100) );
					
				// if(maxbw_help>1) {// write to file
				// #pragma omp critical 
				// cout << " species = " <<
					// tree.species
					// <<" grow1 = " <<
					// maxbw_help_2
					// <<" grow2 = " <<
					// maxbw_help
					// <<" dbasal = " <<
					// tree.dbasal
					// <<" elevation = " <<
					// (double)tree.elevation / 10
					// <<" wfac = " <<
					// weather_list[yearposition-1].weatherfactor[tree.species]
					// <<" fac1 = " <<
					// (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))
					// <<" wfacmin = " <<
					// weather_list[yearposition-1].weatherfactormin[tree.species]
					// <<" fac2 = " <<
					// (1 - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
					// <<" aldinfl = " <<
					// (((double)tree.thawing_depthinfluence) / (100*100))
					// << endl;
				// }
			}
        } else {
			maxbw_help =
				exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
				* ((weather_list[yearposition-1].weatherfactor[0][tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
					/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
				   + (weather_list[yearposition-1].weatherfactormin[0][tree.species]
					  * (1
						 - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
							   / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
        }
    } else {
        if (parameter[0].thawing_depth) {
                maxbw_help =
                    exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition-1].weatherfactor[0][tree.species] * (((double)tree.thawing_depthinfluence) / (100*100));
        } else {
                maxbw_help =
                    exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)
                    * weather_list[yearposition-1].weatherfactor[0][tree.species];
        }
    }

    return maxbw_help;
}

double getMaxbreastwachstum(int yearposition, vector<Weather>& weather_list, Tree tree) {
    double maxbrw_help = 0;
	double transectstart = treerows - parameter[0].locationshift;

    if (parameter[0].lineartransect == true) {
        if (parameter[0].thawing_depth == true) {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * (weather_list[yearposition-1].weatherfactor[0][tree.species]
                                 + ((weather_list[yearposition-1].weatherfactormin[0][tree.species] - weather_list[yearposition-1].weatherfactor[0][tree.species]) * ((double)tree.ycoo / 1000)
                                    / ((double)treerows)))
                              * (((double)tree.thawing_depthinfluence) / (100*100));
        } else {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * (weather_list[yearposition-1].weatherfactor[0][tree.species]
                                 + ((weather_list[yearposition-1].weatherfactormin[0][tree.species] - weather_list[yearposition-1].weatherfactor[0][tree.species]) * ((double)tree.ycoo / 1000)
                                    / ((double)treerows)));
        }
    } else if (parameter[0].demlandscape) {
        if (parameter[0].thawing_depth) {
			if (parameter[0].n_weather_along_grid>0) {
				maxbrw_help = pow( speciestrait[tree.species].mindiametergrowth + exp(-0.5 * speciestrait[tree.species].meangrowthq75p * pow(tree.dbreast,0.5)),0.5 );

				if ( (tree.dbreast > 50) || (maxbrw_help < speciestrait[tree.species].mindiametergrowth) ) {
					maxbrw_help = speciestrait[tree.species].mindiametergrowth;
				}
				
				maxbrw_help = maxbrw_help 
                    * ((weighmeanweathervararray(weather_list[yearposition-1].weatherfactor,((double)tree.ycoo/1000)-transectstart,tree.species) * (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))) + (weighmeanweathervararray(weather_list[yearposition-1].weatherfactormin,((double)tree.ycoo/1000)-transectstart,tree.species) * (1 - (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * (((double)tree.thawing_depthinfluence) / (100*100));
			} else {
				// maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast) + speciestrait[tree.species].meangrowthq75p;
				// maxbrw_help = pow( speciestrait[tree.species].mindiametergrowth * exp(-0.5 * speciestrait[tree.species].meangrowthq75p * tree.dbreast),0.5 );
				maxbrw_help = pow( speciestrait[tree.species].mindiametergrowth + exp(-0.5 * speciestrait[tree.species].meangrowthq75p * pow(tree.dbreast,0.5)),0.5 );

				if ( (tree.dbreast > 50) || (maxbrw_help < speciestrait[tree.species].mindiametergrowth) ) {
					maxbrw_help = speciestrait[tree.species].mindiametergrowth;
				}
				
				maxbrw_help = maxbrw_help 
                    * ((weather_list[yearposition-1].weatherfactor[0][tree.species] * (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000))) + (weather_list[yearposition-1].weatherfactormin[0][tree.species] * (1 - (((double)(tree.elevation-parameter[0].maxele) / 10) - (parameter[0].elevationoffset + 1000)) / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                    * (((double)tree.thawing_depthinfluence) / (100*100));
			}
        } else {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * ((weather_list[yearposition-1].weatherfactor[0][tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                 + (weather_list[yearposition-1].weatherfactormin[0][tree.species]
                                    * (1
                                       - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                             / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
        }
    } else {
        if (parameter[0].thawing_depth == true) {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition-1].weatherfactor[0][tree.species] * (((double)tree.thawing_depthinfluence) / (100*100));
        } else {
                maxbrw_help = exp(speciestrait[tree.species].gdbreastconst + speciestrait[tree.species].gdbreastfac * tree.dbreast
                                  + speciestrait[tree.species].gdbreastfacq * tree.dbreast * tree.dbreast)
                              * weather_list[yearposition-1].weatherfactor[0][tree.species];
        }
    }
    return (maxbrw_help);
}

bool IsFiniteNumber(double x) {
        return (x <= DBL_MAX && x >= -DBL_MAX); 
} 

void Growth(Parameter* parameter, int yearposition, vector<VectorList<Tree>>& world_tree_list, vector<vector<Weather>>& world_weather_list) {
	double gfac = 5.0;
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
				// maxbasalwachstum = getMaxbasalwachstum(yearposition, weather_list, tree);
				maxbasalwachstum = gfac * getMaxbasalwachstum(yearposition, weather_list, tree);
				// maxbasalwachstum = gfac * (speciestrait[tree.species].mindiametergrowth + getMaxbasalwachstum(yearposition, weather_list, tree));
// cout << "Basal dia = " << tree.dbasal << " -> " << maxbasalwachstum << endl;
				tree.dbasalmax = 1000 * maxbasalwachstum;

				double basalwachstum = maxbasalwachstum * (1.0 - (tree.densitywert*tree.heightsubordination));
				// basalwachstum = basalwachstum + basalwachstum * parameter[0].basalinfluenceoldyoung * tree.dbasal;

				if (parameter[0].demlandscape) {
					basalwachstum = basalwachstum * (double)tree.envirimpact / 10000;
// if(basalwachstum>10)
	// cout << " basalwachstum = " << basalwachstum << "  ... envirimp= " << (double)tree.envirimpact / 10000 << endl;
				}

				if (basalwachstum < 0.0) {
					basalwachstum = 0.0;
				}

				// check for real values
				if( (IsFiniteNumber( basalwachstum ) == false) || (basalwachstum > 10.0) || (IsFiniteNumber(tree.dbasal) == false) || (tree.dbasal > 5000.0) ) {
					if (parameter[0].qualiyearlyvis == true) {// output for a quick check
						cout << "tree growth unlimited = " << basalwachstum << "... for tree.dbasal = " << tree.dbasal << endl;
						cout << "tree.species = " << tree.species << "... for tree.envirimpact = " << (double)tree.envirimpact / 10000 << " + tree.densitywert = " <<  tree.densitywert << endl;
						basalwachstum = 0.0;
					}
				}

// #pragma omp critical
            // printf("%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\n",
					// (double)tree.age,
					// (double)tree.species,
					// (double)maxbasalwachstum,
					// (double)basalwachstum,
					// (double)tree.envirimpact / 10000,
					// (double)tree.dbasal,
					// (double)tree.densitywert,
					// (double)weather_list[yearposition-1].weatherfactor[tree.species],
					// (double)tree.thawing_depthinfluence
					// );


// #pragma omp critical
// {
			// FILE* fdir;
            // char filenamechar[50];
            // sprintf(filenamechar, "growthcheck");
            // string output = "output/" + string(filenamechar) + ".csv";
            // fdir = fopen(output.c_str(), "a+");

            // fprintf(fdir,
					// "%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\t%4.4f\n",
					// (double)tree.age,
					// (double)tree.species,
					// (double)maxbasalwachstum,
					// (double)basalwachstum,
					// (double)tree.envirimpact / 10000,
					// (double)tree.dbasal,
					// (double)tree.densitywert,
					// (double)weather_list[yearposition-1].weatherfactor[tree.species],
					// (double)tree.thawing_depthinfluence
					// );

            // fclose(fdir);
// }

				// snow impact
				if(parameter[0].snowcomputation == true && parameter[0].ivort>0) {
					if((((double)tree.height) / 10.0) < (((double)tree.snowdepth) / 10.0)) {
						basalwachstum = basalwachstum * 1.30;
					}
				}

				if (tree.growing == true) {
					tree.dbasal += basalwachstum;
				}
// cout << "Basal dia .... after = " << tree.dbasal << endl;

				tree.dbasalrel = 1000;
				if(parameter[0].relgrowthinfluence == 1) {
					if (maxbasalwachstum <= 0.0) {
						tree.dbasalrel = 0;
					} else {
						// tree.dbasalrel = 1000 * (basalwachstum / (maxbasalwachstum + maxbasalwachstum * parameter[0].basalinfluenceoldyoung * tree.dbasal)) * tree.soilhumidity;
						tree.dbasalrel = 1000 * (basalwachstum / maxbasalwachstum) * tree.soilhumidity;
					}
				}
// if(tree.dbasal > 100)
	// cout << " Basal dia = " << tree.dbasal << " -> dbasalrel= " << tree.dbasalrel << " <- " << basalwachstum << " / " << maxbasalwachstum << " * " << tree.soilhumidity << " ... " << tree.elevation/10 << " ... " << tree.soilhumidity << endl;


				// tree height update
				if (parameter[0].allometryfunctiontype == 3) {// logistic growth
					// tree.height = 10 * exp(speciestrait[tree.species].heightloga/(1+exp((speciestrait[tree.species].heightlogb-log(tree.dbasal*10))/speciestrait[tree.species].heightlogc)));
					double meandbasaly2 = 2.818182;	
					double slope = 260/meandbasaly2;
					if(tree.dbasal < meandbasaly2) {// weighted mean
						double weightfactor = tree.dbasal/meandbasaly2;
						double h1 = slope * tree.dbasal;
						double h2 = exp(speciestrait[tree.species].heightloga/(1+exp((speciestrait[tree.species].heightlogb-log(tree.dbasal*5*10))/speciestrait[tree.species].heightlogc)));
						tree.height = 10* ( (h1*(1-weightfactor)) + (h2*weightfactor) );
					} else {
						tree.height = 10 * exp(speciestrait[tree.species].heightloga/(1+exp((speciestrait[tree.species].heightlogb-log(tree.dbasal*5*10))/speciestrait[tree.species].heightlogc)));
					}

// #pragma omp critical
// cout << "maxbasalw= " << maxbasalwachstum << " => basalw = " << basalwachstum << "  ... envirimp= " << (double)tree.envirimpact / 10000 << " / densitywert = " << tree.densitywert << " / heightsubordination = " <<tree.heightsubordination << " / dbasalrel = " << tree.dbasalrel << " / soilhumidity = " << tree.soilhumidity << endl;

// cout << "H = " << tree.height << endl;
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
					// maxbreastwachstum = getMaxbreastwachstum(yearposition, weather_list, tree);
					maxbreastwachstum = gfac * getMaxbreastwachstum(yearposition, weather_list, tree);
					// maxbreastwachstum = gfac * (speciestrait[tree.species].mindiametergrowth + getMaxbreastwachstum(yearposition, weather_list, tree));

					// breastwachstum = maxbreastwachstum * (1.0 - tree.densitywert);
					breastwachstum = maxbreastwachstum * (1.0 - (tree.densitywert*tree.heightsubordination));

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
					}

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
// cout << "-> breasrel= " << tree.dbreastrel << " <- " << breastwachstum << " / " << maxbreastwachstum << " * " << tree.soilhumidity << endl;
				}
			}
		}
    }
}

