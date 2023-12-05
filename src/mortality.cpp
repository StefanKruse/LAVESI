#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void TreeMort(int yearposition_help, 
				vector<Weather>& weather_list, 
				VectorList<Tree>& tree_list//, 
				// vector<Pollengrid> &pollen_list
				) {
    // abiotic influence: calculation of the factors in a function that adds a mortality rate impact
	// multiple species integration
	for (int species_counter = 1; species_counter < 99; species_counter++) {
		if(speciestrait[species_counter].number == 0)
			break;

		speciestrait[species_counter].yearlycalcofanstiegweathermort=(60*weather_list[yearposition_help].janisothermrestriktion[species_counter]+60*weather_list[yearposition_help].julisothermrestriktion+60*weather_list[yearposition_help].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
		speciestrait[species_counter].yearlycalcofanstiegweathermortmin=(60*weather_list[yearposition_help].janisothermrestriktionmin[species_counter]+60*weather_list[yearposition_help].julisothermrestriktion+60*weather_list[yearposition_help].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
	}

    RandomNumber<double> uniform(0, 1);
	
#pragma omp parallel for default(shared) private(uniform) schedule(static)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

        if (tree.growing == true) {
            double agesmort = 0.0;
			agesmort = tree.age / speciestrait[tree.species].maximumage;
            // if (tree.age > speciestrait[tree.species].maximumage) {  // if maximal age is exceeded an additional factor occurs
                // agesmort = 1.0;
            // }
			
            double windthrowmort = 0.0;
			windthrowmort = speciestrait[tree.species].mwindthrow * ((double)tree.height / 10) / (65535 / 10);// scaled to maximum of height in model of 65 m
			
			double firecrowndamagemort = 0.0;

			if (parameter[0].firemode == 112 || parameter[0].fireintensitymode != 1.0) {
				firecrowndamagemort = (double)tree.relcrowndamage / 1000;
				if(firecrowndamagemort < 0.0)
					firecrowndamagemort = 0.0;
				else if(firecrowndamagemort >= 1.0)
					firecrowndamagemort = 1.0 - speciestrait[tree.species].resprouting*speciestrait[tree.species].relbarkthickness;
				tree.relcrowndamage = 0*1000;
			}
			
			
            // height dependent influences
            double wachstumrel = 1.0;
            if ((double)tree.height / 10 < 130) {
                wachstumrel = (double)tree.dbasalrel / 1000;
            } else {
                wachstumrel = (double)tree.dbreastrel / 1000;
            }

            // extra competition effect
            double heightnkugeleinfluss = 1;
            if ((double)tree.height / 10 < (speciestrait[tree.species].densityvaluemaximumatheight * 2)) {
                heightnkugeleinfluss =
                    heightnkugeleinfluss
                    + (sqrt(pow(speciestrait[tree.species].densityvaluemaximumatheight, 2) - pow((double)tree.height / 10 - speciestrait[tree.species].densityvaluemaximumatheight, 2))
                       / speciestrait[tree.species].densityvaluemaximumatheight);
            }

            double sapl_mort = speciestrait[tree.species].mortyouth * pow(exp((-1.0 * tree.dbasal) + (double)tree.dbasalmax / 1000), speciestrait[tree.species].mortyouthinfluenceexp);
            double age_mort = speciestrait[tree.species].mortage * agesmort * (10.0 * speciestrait[tree.species].mortbg);
            double growth_mort = speciestrait[tree.species].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
            double dens_mort = speciestrait[tree.species].mdensity * heightnkugeleinfluss * tree.densitywert;

            double weathermortadd;
            if (parameter[0].lineartransect) {
                weathermortadd = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp((speciestrait[tree.species].yearlycalcofanstiegweathermort
                                                  - ((speciestrait[tree.species].yearlycalcofanstiegweathermort - speciestrait[tree.species].yearlycalcofanstiegweathermort) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                                 * (weather_list[yearposition_help].weatherfactor[tree.species]
                                                    + ((weather_list[yearposition_help].weatherfactormin[tree.species] - weather_list[yearposition_help].weatherfactor[tree.species])
                                                       * ((double)tree.ycoo / 1000) / ((double)treerows)))
                                                 * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                                       + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            } else if (parameter[0].demlandscape) {
                weathermortadd =
                    1.0
                    - (1.0
                       / (1.0
                          + (((1.0 - 0.5) / 0.5)
                             * exp((speciestrait[tree.species].yearlycalcofanstiegweathermort
                                    - ((speciestrait[tree.species].yearlycalcofanstiegweathermort - speciestrait[tree.species].yearlycalcofanstiegweathermort) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                   * ((weather_list[yearposition_help].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                      + (weather_list[yearposition_help].weatherfactormin[tree.species]
                                         * (1
                                            - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                   * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                         + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            } else {
                weathermortadd = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp(speciestrait[tree.species].yearlycalcofanstiegweathermort * weather_list[yearposition_help].weatherfactor[tree.species]
                                                 * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                                       + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            }

            double heightreduce = pow((1.0 / (double)tree.height / 10), speciestrait[tree.species].heightweathermorteinflussexp);  // includes a minimun limit
            if (heightreduce < 0.001)
                heightreduce = 0.001;
            double weather_mort = speciestrait[tree.species].mweather * weathermortadd * heightreduce;

            double dry_mort = 0.0;
            if (parameter[0].lineartransect) {
                dry_mort = speciestrait[tree.species].mdrought
                           * (weather_list[yearposition_help].droughtmort
                              + ((weather_list[yearposition_help].droughtmortmin - weather_list[yearposition_help].droughtmort) * ((double)tree.ycoo / 1000)
                                 / ((double)treerows)))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else if (parameter[0].demlandscape) {
                dry_mort = speciestrait[tree.species].mdrought
                           * ((weather_list[yearposition_help].droughtmort * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                               / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                              + (weather_list[yearposition_help].droughtmortmin
                                 * (1
                                    - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                          / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else {
                dry_mort = speciestrait[tree.species].mdrought * weather_list[yearposition_help].droughtmort * (pow((1.0 / (double)tree.height / 10), 0.5)+0.5)*(tree.droughtresist/100);
            }

            // calculating the mortality rate of the tree considering the factors of each mortality rate
            double treemortality = 0.0 + speciestrait[tree.species].mortbg + sapl_mort + age_mort + growth_mort + dens_mort + weather_mort + dry_mort + windthrowmort + firecrowndamagemort;
			
			// Adding firedamage from cur_plot.fire (fire intensity), mediated by tree traits (only if firemode == 112)
			if (parameter[0].firemode == 112 || parameter[0].fireintensitymode != 1.0) {
			// treemortality = treemortality + (((double)tree.firedamage) * (1 / ((double)tree.height / 100) / 150)); // -> 10 cm tree firemort (fm) = fm*15, 100 cm = fm*1.5, 200 cm = fm*0.75
			treemortality = treemortality + (double)tree.firedamage * pow((100*1/(double)tree.height), 0.5*0.5); //adapted version
			
			} else if ((parameter[0].firemode != 0) & (parameter[0].firemode != 112)) {
				treemortality = treemortality + (double)tree.firedamage;
			} else if (parameter[0].firemode == 0) {
				treemortality = treemortality;
			}

			if(tree.soilhumidity == 0.0)
				treemortality += 0.1;
				
            if (treemortality > 1.0) {
                treemortality = 1.0;
            } else if (treemortality < 0.0) {
                treemortality = 0.0;
            }

            // determine if a tree dies
			if ( ((double) uniform.draw() < treemortality) || (tree.envirimpact <= 0) ) {
                tree.growing = false;
                tree_list.remove(tree_i);				
            }
        }
    }
    tree_list.consolidate();
cout << endl;
}

void Mortality(Parameter* parameter,
               // int Jahr,
               int yearposition,
               vector<VectorList<Tree>>& world_tree_list,
               vector<VectorList<Seed>>& world_seed_list,
               vector<vector<Weather>>& world_weather_list,
			   vector<vector<Pollengrid>>& world_pollen_list,
			   vector<vector<Pollencalcs>>& world_pollcalcs,
			   vector<vector<Envirgrid>>& world_plot_list) {
    int aktort = 0;
	
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather>& weather_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
		vector<vector<Pollengrid> >::iterator world_positon_p = (world_pollen_list.begin()+aktort);
		vector<Pollengrid>& pollen_list = *world_positon_p;
		vector<vector<Pollencalcs> >::iterator world_positon_c = (world_pollcalcs.begin()+aktort);
		vector<Pollencalcs>& pollcalcs = *world_positon_c;
		vector<vector<Envirgrid>>::iterator world_positon_k = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *world_positon_k;
        aktort++;

		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		RandomNumber<double> uniform(0, 1);

		// mortality of seeds
#pragma omp parallel for default(shared) private(uniform) schedule(static)
		for (unsigned int i = 0; i < seed_list.size(); ++i) {
			auto& seed = seed_list[i];
			if (!seed.dead) {
				if (uniform.draw() < (seed.incone ? speciestrait[seed.species].seedconemort : speciestrait[seed.species].seedfloormort)) {
					seed.dead = true;
					seed_list.remove(i);
				}
			}
		}
		seed_list.consolidate();


// #pragma omp parallel for default(shared) schedule(guided)
#pragma omp parallel for default(shared) schedule(static)
		for(int kartenpos=0;kartenpos<(parameter[0].pollengridxpoints*parameter[0].pollengridypoints);kartenpos++) {
			pollen_list[kartenpos].Treenames.clear();
			pollen_list[kartenpos].seedweight=0;
			pollen_list[kartenpos].droughtresist=0;
			// pollen_list[kartenpos].seednumber=0;
			pollen_list[kartenpos].selving=0;
			pollen_list[kartenpos].neutralmarkers.clear();
			
			pollcalcs[kartenpos].neutral.clear();
			pollcalcs[kartenpos].name.clear();
			pollcalcs[kartenpos].seedweight.clear();
			pollcalcs[kartenpos].droughtresist.clear();
			pollcalcs[kartenpos].selving.clear();
			pollcalcs[kartenpos].pname.clear();
			pollcalcs[kartenpos].filled = 0;
		}

#pragma omp parallel for default(shared) schedule(static)
		for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
			auto& tree = tree_list[tree_i];
			
			if(tree.cone!=0){			
				//...because the grid is updated here in every time step. This way dying trees are filtered.
				//double lent=sqrt(parameter[0].pollengridpoints); //marked as unused variable
				double lentx=(parameter[0].pollengridxpoints);
				double lenty=(parameter[0].pollengridypoints);
				
				int kartenpos=(floor(((tree.ycoo/1000)*0.9999999)/(treerows/lentx))*lenty)+(floor(((tree.xcoo/1000)*0.9999999)/(treecols/lenty)));
				
				pollen_list[kartenpos].Treenames.push_back(tree.name);
				//tree.subgridVECpos=//pollen_list2[kartenpos].Treenames.begin()+
				//(pollen_list[kartenpos].Treenames.size()-1);
				pollen_list[kartenpos].seedweight+=tree.seedweight;
				pollen_list[kartenpos].droughtresist+=tree.droughtresist;
				// pollen_list[kartenpos].seednumber+=tree.seednumber;
				pollen_list[kartenpos].selving+=tree.selving;
				// unsigned int pollen_i_presize = 24* (pollen_list[kartenpos].Treenames.size()-1);
				for (unsigned int i=0; i < 24; i++ ){
					pollen_list[kartenpos].neutralmarkers.push_back( tree.neutralmarkers[i]);
				}
				tree.subgridpos=kartenpos+1;
			}
		}

#pragma omp parallel for default(shared) schedule(static)
		// TODO make independent from parameter just based on size
		for(int kartenpos=0;kartenpos<(parameter[0].pollengridxpoints*parameter[0].pollengridypoints);kartenpos++) {
			// TODO declare once size reduces to 1 time calculation rather than 4 times
			if(pollen_list[kartenpos].Treenames.size()>1){
				pollen_list[kartenpos].seedweight/=pollen_list[kartenpos].Treenames.size();
				pollen_list[kartenpos].droughtresist/=pollen_list[kartenpos].Treenames.size();
				// pollen_list[kartenpos].seednumber/=pollen_list[kartenpos].Treenames.size();

				pollen_list[kartenpos].selving/=pollen_list[kartenpos].Treenames.size();

				// pollen_list[kartenpos].seedweightvar+=(tree.seedweight-pollen_list[kartenpos].seedweight)*(tree.seedweight-pollen_list[kartenpos].seedweight);
			}
		}
			
		double lentx=(parameter[0].pollengridxpoints);
		double lenty=(parameter[0].pollengridypoints);				

		// first a loop filling the arriving polleninformation around pollengrid
		// ... followed by a loop around trees assessing the information
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
		for (unsigned int pos = 0; pos < pollen_list.size(); ++pos) {
			auto& pPollengrid = pollen_list[pos];
			
			if(pPollengrid.Treenames.size()!=0) {// only if in the gridcell are trees bearing cones
				double direction=0.0;
				double velocity=0.0;
				unsigned int ripm=0,cntr=0;
				double p=0.0,kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
				double I0kappa=0.0;
				double pe=0.01;
				double C=parameter[0].pollengregoryc;
				double m=parameter[0].pollengregorym;
				vector<int> Vname;
				vector<double> Vthdpth;
				vector<double> Vdrought;
				// vector<double> Vnumber;
				vector<double> Vselve;
				vector<unsigned int> Vneutral;
				vector<int> pollname;
				vector<unsigned int> copyneutralmarkers(24, 0);
				vector<unsigned int> fathvector(24, 0);
				vector<vector<unsigned int>> storevector;

				if( (parameter[0].pollination==1 && parameter[0].ivort>1045) || (parameter[0].pollination==9)) { // calculate and fill structure for later assessment				
					Pollinationprobability(pPollengrid.xcoo,pPollengrid.ycoo,&parameter[0],world_positon_p,direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m, Vname,Vthdpth,Vdrought,Vselve,Vneutral,pollname);

					// filling assuming pollcalcs has the same size and structure as pollen_list
					copy(Vname.begin(), Vname.end(), back_inserter(pollcalcs[pos].name));
					copy(Vthdpth.begin(), Vthdpth.end(), back_inserter(pollcalcs[pos].seedweight));
					copy(Vdrought.begin(), Vdrought.end(), back_inserter(pollcalcs[pos].droughtresist));
					copy(Vselve.begin(), Vselve.end(), back_inserter(pollcalcs[pos].selving));
					copy(Vneutral.begin(), Vneutral.end(), back_inserter(pollcalcs[pos].neutral));
					copy(pollname.begin(), pollname.end(), back_inserter(pollcalcs[pos].pname));
					pollcalcs[pos].filled = 1;
				}
			}
		}

#pragma omp parallel for default(shared) private(uniform) schedule(guided)
		for (unsigned int it = 0; it < tree_list.size(); ++it) {

			auto& tree = tree_list[it];
			
				if(tree.seednewly_produced>0) {
					// declare structures to copy locally arriving pollen information
					// TODO this is computationally intensive, better nested around gridpoints and copy once all data from the pollencalcs structure for all trees in this
					vector<int> Vname;
					vector<double> Vthdpth;
					vector<double> Vdrought;
					vector<double> Vselve;
					vector<unsigned int> Vneutral;
					vector<int> pollname;
					
					// TODO PARAL could be a direct check and if portion > seedconemort just let it survive or save this and use for selving evaluation, saving 1 RNG
					vector<unsigned short> seedliving;
					for(int sna=0; sna < tree.seednewly_produced; sna++) {
						double zufallsz = 0.0 +( (double) 1.0*uniform.draw() );
						if(zufallsz>=parameter[0].seedconemort) {
							seedliving.push_back(zufallsz*100);
						}
					}
					
					if(seedliving.size()>0) {
						int position=(floor(((tree.ycoo/1000)*0.9999999)/(treerows/lentx))*lenty)+(floor(((tree.xcoo/1000)*0.9999999)/(treecols/lenty)));
						copy(pollcalcs[position].neutral.begin(), pollcalcs[position].neutral.end(), back_inserter(Vneutral));
						copy(pollcalcs[position].name.begin(), pollcalcs[position].name.end(), back_inserter(Vname));
						copy(pollcalcs[position].seedweight.begin(), pollcalcs[position].seedweight.end(), back_inserter(Vthdpth));
						copy(pollcalcs[position].droughtresist.begin(), pollcalcs[position].droughtresist.end(), back_inserter(Vdrought));
						copy(pollcalcs[position].selving.begin(), pollcalcs[position].selving.end(), back_inserter(Vselve));
						copy(pollcalcs[position].pname.begin(), pollcalcs[position].pname.end(), back_inserter(pollname));
						pollcalcs[position].filled = 1;
						
						for(unsigned int sl=0; sl<seedliving.size(); sl++) {
							Seed seed;
							seed.yworldcoo=aktortyworldcoo;
							seed.xworldcoo=aktortxworldcoo;
							seed.xcoo=tree.xcoo;
							seed.ycoo=tree.ycoo;
							seed.dead = false;
							seed.generation = tree.generation+1;
							seed.namem=tree.name;
							if ( seedliving[sl] < tree.selving && (parameter[0].pollination==1 || parameter[0].pollination==9) ) {
								// if no fathering pollen grid cell is found....
								seed.namep=0;
								seed.seedweight=normrand(tree.seedweight,0.05,0.33,1.66);	
								seed.droughtresist=normrand(tree.droughtresist,8,0,100);
								seed.selving=normrand(tree.selving,8,0,100);
								mixvector(tree.neutralmarkers,tree.neutralmarkers,seed.neutralmarkers);
							}
							// if chosen, determine the father by pollination out of available (matured) trees
							
							else if((Vname.size()>0) && (parameter[0].pollination==1 || parameter[0].pollination==9)) {
								double randecide=uniform.draw();
								int iran= floor (randecide *(Vname.size()-0.00001)); //at the end -1 was deleteed as it is suspected to be causing the bug
								//Vname.at(iran) is the chosen pollen grid cell number returned from the pollination function
								//Vthdpth.at(iran) is the chosen trait (seed weight) value returned from the pollination function
								seed.namep=Vname.at(iran); 

								seed.seedweight=mixrand(Vthdpth.at(iran),0.05,tree.seedweight,0.05,0.33,1.66); // changed the std to be a lot 
								seed.droughtresist=mixrand(Vdrought.at(iran),8,tree.droughtresist,8,0,100);
								seed.selving=mixrand(Vselve.at(iran),8,tree.selving,8,0,100);
								
								std::array<unsigned int, 24> storearray;
								for (unsigned int i=0; i < 24; i++ ){
									int	point_i= i+(24*iran);
									storearray[i] = Vneutral[point_i];
								}
								
								mixvector(tree.neutralmarkers,storearray,seed.neutralmarkers);
							} else if (parameter[0].pollination ==0) { ///TODO should be further upstream to avoid unnecessary computation. no need to keep pollenlist when there is no pollination 
								if (parameter[0].variabletraits == 1) {
									seed.namep=0;
									seed.seedweight=1;
									seed.droughtresist=100;
									seed.selving=normrand(50,20,0,100);
									for(unsigned int i=0; i < 24; i++) {
										seed.neutralmarkers[i] = uniform.draw()*999999;
									 }
								} else {
									seed.namep=0;
									seed.seedweight=1;
									seed.droughtresist=100;
									seed.selving=0;
								}
							} else {
								seed.dead = true;
							}
							
							if(seed.dead == false) {
								seed.currentweight=tree.seedweight;
								seed.line=tree.line;
								seed.generation=tree.generation+1;
								seed.origin=tree.origin;
								seed.incone=true;
								seed.age=0;
								seed.species=tree.species;
								seed.releaseheight=tree.height;
								seed.thawing_depthinfluence=100;
#pragma omp critical
								{
									seed_list.add_directly(std::move(seed));
								}
							}
						}
					}
				}
				
				if (tree.cloningactive==true) {
					if (tree.cloned!=true){
						tree.clonetimer=6;
						tree.cloned=true;
						Tree treenew;
						int i = tree.ycoo * parameter[0].sizemagnif / 1000;
						int j = tree.xcoo * parameter[0].sizemagnif / 1000;

						const auto curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif) + static_cast<std::size_t>(j);

						double thawing_depthinfluence_help = 100;
						if ( (parameter[0].thawing_depth == true) && (plot_list[curposi].maxthawing_depth < (speciestrait[tree.species].minactivelayer*10)) ) {  // TODO: check calculation only during spinup
							thawing_depthinfluence_help = (unsigned short)((200.0 / (speciestrait[tree.species].minactivelayer*10)) * (double)plot_list[curposi].maxthawing_depth);
						}

					// ... and weather.
					// calculate the latest growth performance
					// TODO: merge with general growth functions
					double maxbw_help = 0;

					if (parameter[0].lineartransect == true) {
						maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
									 * (weather_list[yearposition].weatherfactor[tree.species]
										+ ((weather_list[yearposition].weatherfactormin[tree.species] - weather_list[yearposition].weatherfactor[tree.species])
										   * ((double)tree.ycoo / 1000) / ((double)treerows)))
									 * (((double)thawing_depthinfluence_help) / 100);
					} else if (parameter[0].demlandscape) {  // copied from growth.cpp
						if (parameter[0].thawing_depth) {
							maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
										 * ((weather_list[yearposition].weatherfactor[tree.species]
											 * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
											 / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
											+ (weather_list[yearposition].weatherfactormin[tree.species]
											   * (1
												  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
														/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
										 * (((double)tree.thawing_depthinfluence) / 100);
						} else {
							maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
										 * ((weather_list[yearposition].weatherfactor[tree.species]
											 * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
											 / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
											+ (weather_list[yearposition].weatherfactormin[tree.species]
											   * (1
												  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
														/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
						}
					} else {
							maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
										 * weather_list[yearposition].weatherfactor[tree.species] * (((double)thawing_depthinfluence_help) / 100);
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
							} else if (parameter[0].densitytiletree == 2)  // multiplication of values
							{
								density_help = (1.0 - (density_help / (((double)plot_list[curposi].Treedensityvalue / 10000) * density_help)));
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
								
					// create new tree
					treenew.yworldcoo = tree.yworldcoo;
					treenew.xworldcoo = tree.xworldcoo;
				
					
					double moveranx=uniform.draw()*4000-2000;
					double moverany=uniform.draw()*4000-2000;
					
					if (tree.ycoo+moverany > 1000 * ((int)treerows - 1)) {
						treenew.ycoo = 1000 * ((int)treerows - 1);
					}
					else if (tree.ycoo+moverany <0) {
						treenew.ycoo =0.0;
					}
					else{
						treenew.ycoo = tree.ycoo+moverany; 
					}
					if (treenew.ycoo>1000 * ((int)treerows - 1)){
						printf("cloned a tree position %d", tree.ycoo);
						printf("new position %d", treenew.ycoo);
						printf("mover %f", moverany);
					}
					if (tree.xcoo+moveranx > 1000 * ((int)treecols - 1)) {
						treenew.xcoo = 1000 * ((int)treecols - 1);
					}
					else if (tree.xcoo+moveranx < 0) {
						treenew.xcoo = 0.0;
					}
					else {
						treenew.xcoo = tree.xcoo+moveranx;
					}
					
					
					treenew.name = ++parameter[0].nameakt;
					treenew.namem = tree.namem;
					treenew.namep = 0;
					treenew.line = tree.line;
					treenew.generation = tree.generation+1;
					treenew.origin=tree.origin;
					treenew.dbasal = basalgrowth_help;
					treenew.dbasalmax = 1000 * maxbw_help;
					treenew.dbasalrel = 1000;
					treenew.dbreast = 0.0;
					treenew.dbreastrel = 1000;

					// tree height update
					if (parameter[0].allometryfunctiontype == 3) {// logistic growth
						treenew.height = 10 * exp(speciestrait[tree.species].heightloga/(1+exp((speciestrait[tree.species].heightlogb-log(treenew.dbasal*10))/speciestrait[tree.species].heightlogc)));
					} else if (parameter[0].allometryfunctiontype == 1) {
						treenew.height = 10 * speciestrait[tree.species].dbasalheightalloslope * pow(maxbw_help, speciestrait[tree.species].dbasalheightalloexp);
					} else {
						treenew.height = 10 * speciestrait[tree.species].dbasalheightslopenonlin * maxbw_help;
					}
					treenew.age = 0;
					treenew.cone = false;
					treenew.coneheight = 65535;
					treenew.seednewly_produced = 0;
					treenew.densitywert = density_help;
					treenew.growing = true;
					treenew.species = tree.species;
					treenew.thawing_depthinfluence = thawing_depthinfluence_help;
					treenew.seedweight = tree.seedweight;
					treenew.droughtresist = tree.droughtresist;
					treenew.cloning = false;
					treenew.cloningactive=true;
					treenew.clonetimer=10;
					treenew.cloned=true;
					treenew.selving = tree.selving;
					treenew.neutralmarkers=tree.neutralmarkers;
					treenew.inbreedingdepression=tree.inbreedingdepression;
					treenew.envirimpact = 10000;
					treenew.twi = 6.25*100;
					treenew.soilhumidity = 1;
					treenew.crownstart = 0*10;
					treenew.relcrowndamage = 0*1000;
					//adding factor for growth here. should make this dependend on height maybe. although treeas are maturation height when the clone, so straight might be fine.
					treenew.cloneboost=2;
					tree.cloneboost=0.95;

					tree_list.add(std::move(treenew));
					
					}
					if (tree.clonetimer<=0){
						tree.cloned=false;
						tree.cloneboost=1;
						tree.cloningactive=false;
					}
				}
		}// main tree loop for each tree
		tree_list.consolidate();
		
		TreeMort(yearposition, 
					weather_list, 
					tree_list//, 
					// pollen_list
					);
    } // main world loop
}
