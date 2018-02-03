using namespace std;

/****************************************************************************************//**
 * \brief calculate tree mortality
 *
 * depends on abiotic factors (temperature and number of days with temperatures above 20 degrees)
 * and on biotic factors
 *
 *
 *******************************************************************************************/
void TreeMort(int yearposition_help,vector<Weather*> &weather_list,list<Tree*> &tree_list)
{
	// calculation of the factors in a function that adds a mortality rate impact
	double anstiegweathermortg=160;
	double anstiegweathermorts=160;
	
	anstiegweathermortg=(60*weather_list[yearposition_help]->janisothermrestriktiong+60*weather_list[yearposition_help]->julisothermrestriktion+60*weather_list[yearposition_help]->nddrestriktion);
	anstiegweathermorts=(60*weather_list[yearposition_help]->janisothermrestriktions+60*weather_list[yearposition_help]->julisothermrestriktion+60*weather_list[yearposition_help]->nddrestriktion);

	// biotic influence:
	for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
	{
		pTree=(*pos);
		
		if(pTree->growing==true)
		{
			// if maximal age is exceeded an additional factor occurs
			double agesmort=0.0;
			if (pTree->age>parameter[0].maximumage) 
			{
				agesmort=1.0;
			}
	
			// height dependent influences
			double wachstumrel=1.0;
			if (pTree->height<130.0) 
			{
				wachstumrel=pTree->dbasalrel;
			}
			else 
			{
				wachstumrel=pTree->dbreastrel;
			}
			
			// extra competition effect
			double heightnkugeleinfluss=1;
			if (pTree->height<(parameter[0].densityvaluemaximumatheight*2)) 
			{	
				heightnkugeleinfluss=heightnkugeleinfluss+(sqrt(pow(parameter[0].densityvaluemaximumatheight,2)-pow(pTree->height-parameter[0].densityvaluemaximumatheight, 2))/parameter[0].densityvaluemaximumatheight);
			}	
			
			// calculation of young tree's mortality
			double maxhg;
			double maxhs;
			double maxhpufg=weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);
			double maxhpufs=weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);

			if (maxhpufg<=0.0)
			{
				if (parameter[0].allometryfunctiontype==1)
				{
					maxhg= parameter[0].dbasalheightalloslope * pow(exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
				}
				else 
				{
					maxhg= parameter[0].dbasalheightslopenonlin*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);	
				}
			}
			else 
			{
				if (parameter[0].allometryfunctiontype==1)
				{
					maxhg= parameter[0].dbasalheightalloslope * pow(weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
				}
				else 
				{
					maxhg= parameter[0].dbasalheightslopenonlin*weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);
				}
			}
			
			if (maxhpufs<=0.0)
			{
				if (parameter[0].allometryfunctiontype==1)
				{
					maxhs= parameter[0].dbasalheightalloslope * pow(exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
				}
				else 
				{
					maxhs= parameter[0].dbasalheightslopenonlin*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);	
				}
			}
			else 
			{
				if (parameter[0].allometryfunctiontype==1)
				{
					maxhs= parameter[0].dbasalheightalloslope * pow(weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
				}
				else 
				{
					maxhs= parameter[0].dbasalheightslopenonlin*weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);
				}
			}
			
			double sapl_mort_gmel	= parameter[0].mortyouth * pow(exp((-1.0 * pTree->height) + maxhg), parameter[0].mortyouthinfluenceexp);
			double sapl_mort_sib	= parameter[0].mortyouth * pow(exp((-1.0 * pTree->height) + maxhs), parameter[0].mortyouthinfluenceexp);
			double age_mort		= parameter[0].mortage	* agesmort * (10.0 * parameter[0].mortbg);
			double growth_mort	= parameter[0].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
			double dens_mort	= parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert;
			double weathermortaddg = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * 
										exp(anstiegweathermortg * weather_list[yearposition_help]->weatherfactorg*
										exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)))));
			double weathermortadds = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * 
										exp(anstiegweathermorts * weather_list[yearposition_help]->weatherfactors*
										exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)))));	
			double weather_mort_gmel	= parameter[0].mweather * weathermortaddg * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);
			double weather_mort_sib	= parameter[0].mweather * weathermortadds * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);				
			double dry_mort		= parameter[0].mdrought * weather_list[yearposition_help]->droughtmort * pow((1.0 / pTree->height), 0.5);
			
			// calculating the mortality rate of the tree considering the factors of each mortality rate
			double Treemortg = 0.0 
							+ parameter[0].mortbg
							+ sapl_mort_gmel
							+ age_mort
							+ growth_mort 
							+ dens_mort
							+ weather_mort_gmel 
							+ dry_mort;
							
			double Treemorts = 0.0 
							+ parameter[0].mortbg
							+ sapl_mort_sib
							+ age_mort
							+ growth_mort 
							+ dens_mort
							+ weather_mort_sib
							+ dry_mort;

			
			if (Treemortg>1.0)
			{
				Treemortg = 1.0;
			}
			else if (Treemortg<0.0) 
			{
				Treemortg = 0.0;
			}

			if(Treemorts>1.0)
			{
				Treemorts = 1.0;
			}
			else if (Treemorts<0.0) 
			{
				Treemorts = 0.0;
			}				

			if (parameter[0].mortvis==true && pTree->height>200.0 && pTree->species==1) 
			{
					printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +drought=%4.4f =>%4.2f", 
					pTree->height, 
					pTree->dbasalrel,pTree->dbreastrel,
					parameter[0].mortyouth*pow(exp((-1.0*pTree->height)+maxhg),parameter[0].mortyouthinfluenceexp),
					parameter[0].mortage*(agesmort*(10.0*parameter[0].mortbg)), 
					+ (1.0-pow(wachstumrel,parameter[0].relgrowthmortinfluenceexp)),
					parameter[0].mdensity*heightnkugeleinfluss*pTree->densitywert, 
					parameter[0].mweather*weathermortaddg*pow((1.0/pTree->height),0.5), 
					parameter[0].mdrought*(weather_list[yearposition_help]->droughtmort*pow((1.0/pTree->height),0.5)), 
					Treemortg);
			}

			if (parameter[0].mortvis==true && pTree->height>200.0 && pTree->species==2) 
			{
					printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +drought=%4.4f =>%4.2f", 
					pTree->height, 
					pTree->dbasalrel,pTree->dbreastrel,
					parameter[0].mortyouth*pow(exp((-1.0*pTree->height)+maxhs),parameter[0].mortyouthinfluenceexp),
					parameter[0].mortage*(agesmort*(10.0*parameter[0].mortbg)), 
					+ (1.0-pow(wachstumrel,parameter[0].relgrowthmortinfluenceexp)),
					parameter[0].mdensity*heightnkugeleinfluss*pTree->densitywert, 
					parameter[0].mweather*weathermortadds*pow((1.0/pTree->height),0.5), 
					parameter[0].mdrought*(weather_list[yearposition_help]->droughtmort*pow((1.0/pTree->height),0.5)), 
					Treemorts);
			}

			// Determine if a tree dies (deletion of said tree in the corresponding list)
			double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
			if (((pTree->species==1) && (zufallsz<Treemortg)) || ((pTree->species==2) && (zufallsz<Treemorts))) 
			{
				delete pTree;
				pos=tree_list.erase(pos);
			}
			else
			{
				++pos;
			}
		}
		else
		{
			++pos;
		}

	}
}



/****************************************************************************************//**
 * \brief calculate tree and seed mortality
 *
 * first create surviving seeds then call TreeMort() 
 *
 *
 *******************************************************************************************/
void Mortality( struct Parameter *parameter,int Jahr, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<Seed*> > &world_seed_list, vector<vector<Weather*> > &world_weather_list)
{
	int aktort=0;
	
	for(vector<vector<Weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{
		vector<Weather*>& weather_list = *posw;

		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;
		
		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<Seed*>& seed_list = *world_positon_s;

		aktort++;

		double start_time_mortpoll=omp_get_wtime();
		
		// mortality of seeds
		for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{
			pSeed=(*pos);
			double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
			
			if (pSeed->incone==false)
			{
				if (zufallsz<parameter[0].seedfloormort) 
				{
					delete pSeed;
					pos=seed_list.erase(pos);
				}
				else
				{
					++pos;
				}
			}
			else if (pSeed->incone==true)
			{
				if (zufallsz<parameter[0].seedconemort) 
				{
					delete pSeed;
					pos=seed_list.erase(pos);
				}
				else
				{
					++pos;
				}
			}
			else
			{	
				// safety procedure: has the variable incone been set?
				signed int exiterrormortality; 
				printf("\n In the mortality determining function a seed has no value applied to the variable incone\n"); 
				printf("\n To continue the simulation, press 1, stop it with any other key\n"); 
				
				scanf("%d", &exiterrormortality); 
                                if (exiterrormortality!=1) 
                                {
                                    printf("LaVeSi has been stopped after a failure in mortality.cpp\n\n");exit(1);
                                }
		
				delete pSeed;
				pos=seed_list.erase(pos);						
			}
		}
		
		double end_time_seedsuviving=omp_get_wtime();

		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		// timers for parallel processing 
		double timer_eachtree_advance_all=0;
		double timer_eachtree_vectini_all=0;
		double timer_eachtree_seedsurv_all=0;
		double timer_eachtree_seedadd_all=0;
		double timer_eachtree_total_all=0;
		double timer_tresedliv_all=0;
		double timer_createseeds_all=0;

		// chose the implementation of multi-core-processing
		int mcorevariant=3;
			// 1 == use advance to iterate through lists, but this makes the computation really slow
			// 2 == split list to X lists of the same length
			// 3 == trees are ordered by age which is highly correlated with seedprodAKT so that only elements are considered untiil the last tree producing seeds
		if(mcorevariant==1)
		{// OMP==1
			// loop around the loop for MULTI-CORE-PROCESSING
			// before run a program parallel set the number of helpers by changing the environment variable
			// ... ... export OMP_NUM_THREADS=4
			// ... or explicitly overwrite the environmental variable by setting the helper number directly
			// ... ... omp_set_num_threads(int); // which is set in the parameter.txt file variable "omp_num_threads"

			// loop with omp through each element of the list
			omp_set_dynamic(0); //disable dynamic teams
			omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
			
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

			#pragma omp parallel default(shared) private(pTree,pSeed,       pTree_copy,    direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
			{
				// declare a local seed list to be filled by each thread
				list<Seed*> newseed_list;
				
				direction=0.0;velocity=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);
				I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;phi=0.0;dr=0.0;dx=0.0;dy=0.0;
				
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;
				double timer_eachtree_seedadd=0;
				double timer_eachtree_total=0;
				
				#pragma omp for nowait schedule(guided) 
				for(unsigned int pari=0; pari<tree_list.size(); ++pari)
				{
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for later calculating mean of computation times
				
					list<Tree*>::iterator posb=tree_list.begin();
					// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
					advance(posb, pari);
					
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

					pTree=(*posb);			
					
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;

					if((parameter[0].ivort==1) & (pari==0))
					{
						cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
					}
					
					if(pTree->seednewly_produced>0)
					{
						int seedlebend=0;
						for(int sna=0; sna < pTree->seednewly_produced; sna++)
						{
							double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
							if(zufallsz>=parameter[0].seedconemort) 
							{
								++seedlebend;
							}
						}
						
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;

						if(seedlebend>0)
						{
							if( (parameter[0].pollination==1 && Jahr>1978 && Jahr<2013 && parameter[0].spinupphase==false && parameter[0].ivort>1045) || (parameter[0].pollination==9))
							{
								Pollinationprobability(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,        
													direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
													Vname,Vthdpth,
													n_trees
													);
							}
					
							// get the characteristics for each surviving seed and push these back new to seed_list
							for(int sl=0; sl<seedlebend; sl++)
							{
								pSeed= new Seed();
								
								pSeed->yworldcoo=aktortyworldcoo;
								pSeed->xworldcoo=aktortxworldcoo;
								pSeed->xcoo=pTree->xcoo;
								pSeed->ycoo=pTree->ycoo;
								pSeed->namem=pTree->name;
								
								// if chosen, determine the father by pollination out of available (matured) trees
								if((Vname.size()>0) && (parameter[0].pollination==1 || parameter[0].pollination==9))
								{
									int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
									pSeed->namep=Vname.at(iran);
									pSeed->thawing_depthinfluence=Vthdpth.at(iran);
								} 
								else
								{
									pSeed->namep=0;
									pSeed->thawing_depthinfluence=100;
								}
															
								pSeed->line=pTree->line;
								pSeed->generation=pTree->generation+1;
								pSeed->incone=true;
								pSeed->weight=1;
								pSeed->age=0;
								pSeed->species=pTree->species;
								pSeed->releaseheight=pTree->height;

								newseed_list.push_back(pSeed);
							}

						}
						
						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;
					}
					
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;
				}
				
				// append all newly created seed from each thread at once to the seed_list
				#pragma omp critical
				{
					seed_list.splice(seed_list.end(), newseed_list);
					
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
				}
			}// parallel region

			Vname.clear();Vname.shrink_to_fit();
			Vthdpth.clear();Vthdpth.shrink_to_fit();
		}// OMP==1
		if(mcorevariant==2)
		{// OMP==2
			omp_set_dynamic(0); //disable dynamic teams
			omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

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
	
			#pragma omp parallel default(shared) private(pTree,pSeed,       pTree_copy,    direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
			{
				if((parameter[0].ivort==1))
				{
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				}

				direction=0.0;velocity=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);
				I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;phi=0.0;dr=0.0;dx=0.0;dy=0.0;
			
				// slit the lists manually
				int thread_count=omp_get_num_threads();
				int thread_num=omp_get_thread_num();
				size_t chunk_size=tree_list.size()/thread_count;
				auto begin=tree_list.begin();
				std::advance(begin, thread_num*chunk_size);
				auto end=begin;
				if(thread_num==(thread_count-1)) // the last thread iterates through the remaining elements
				{
					end = tree_list.end();
				} 
				else
				{
					std::advance(end, chunk_size);
				}
						
				// declare a local seed list to be filled by each thread
				list<Seed*> newseed_list;

				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;
				double timer_eachtree_seedadd=0;
				double timer_eachtree_total=0;
				
				// wait for all threads to initialize and then proceed
				#pragma omp barrier
				for(auto it = begin; it != end; ++it)
				{
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;
					
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

					pTree=(*it);			
					
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;
					
					if(pTree->seednewly_produced>0)
					{								
						int seedlebend=0;
						for(int sna=0; sna < pTree->seednewly_produced; sna++)
						{
							double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
							if(zufallsz>=parameter[0].seedconemort) 
							{
								++seedlebend;
							}
						}
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;
						
						if(seedlebend>0)
						{
							if( (parameter[0].pollination==1 && Jahr>1978 && Jahr<2013 && parameter[0].spinupphase==false && parameter[0].ivort>1045) || (parameter[0].pollination==9))
							{
								Pollinationprobability(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,        
												direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
												Vname,Vthdpth,
												n_trees
											);
							}
					
							// get the characteristics for each surviving seed and push these back new to seed_list
							for(int sl=0; sl<seedlebend; sl++)
							{
								pSeed= new Seed();
								
								pSeed->yworldcoo=aktortyworldcoo;
								pSeed->xworldcoo=aktortxworldcoo;
								pSeed->xcoo=pTree->xcoo;
								pSeed->ycoo=pTree->ycoo;
								pSeed->namem=pTree->name;
								
								// if chosen, determine the father by pollination out of available (matured) trees
								if((Vname.size()>0) && (parameter[0].pollination==1 || parameter[0].pollination==9))
								{
									int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
									pSeed->namep=Vname.at(iran);
									pSeed->thawing_depthinfluence=100;
								} 
								else
								{
									pSeed->namep=0;
									pSeed->thawing_depthinfluence=100;
								}
								
								pSeed->line=pTree->line;
								pSeed->generation=pTree->generation+1;
								pSeed->incone=true;
								pSeed->weight=1;
								pSeed->age=0;
								pSeed->species=pTree->species;
								pSeed->releaseheight=pTree->height;

								newseed_list.push_back(pSeed);
							}
						}

						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;

					}
					
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;

				}// main tree loop on each core
				
				// append all newly created seed from each thread at once to the seed_list
				#pragma omp critical
				{
					seed_list.splice(seed_list.end(), newseed_list);
					
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
				}
			}// parallel region
			
			Vname.clear();Vname.shrink_to_fit();
			Vthdpth.clear();Vthdpth.shrink_to_fit();
		}// OMP==2
		if(mcorevariant==3)
		{// OMP==3
			omp_set_dynamic(0); //disable dynamic teams
			omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

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
			
			// set end of the iterations split up to last tree with produced seeds
			list<Tree*>::iterator lasttreewithseeds_iter=tree_list.begin();
			int lasttreewithseeds_pos=0;
			int treeiter=0;
			for(list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb)
			{
				pTree=(*posb);
				
				treeiter=treeiter+1;
				
				if(pTree->seednewly_produced>0)
				{
					lasttreewithseeds_pos=treeiter;
				}
			}
			advance(lasttreewithseeds_iter, lasttreewithseeds_pos);
					
			#pragma omp parallel default(shared) private(pTree,pSeed,       pTree_copy,    direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
			{
				direction=0.0;velocity=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);phi=0.0;dr=0.0;dx=0.0;dy=0.0;
				I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;

				int thread_count=omp_get_num_threads();
				int thread_num=omp_get_thread_num();
				size_t chunk_size=lasttreewithseeds_pos/thread_count;
				auto begin=tree_list.begin();
				std::advance(begin, thread_num*chunk_size);
				auto end=begin;
				
				if(thread_num==(thread_count-1))
				{
					if((parameter[0].ivort==1))
					{
						cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
					}
				
					end=lasttreewithseeds_iter;
				} 
				else
				{
					std::advance(end,chunk_size);
				}
		
				// declare a local seed list to be filled by each thread
				list<Seed*> newseed_list;
				
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;
				double timer_eachtree_seedadd=0;
				double timer_eachtree_total=0;
				double timer_tresedliv=0;
				double timer_createseeds=0;

				// wait for all threads to initialize and then proceed
				#pragma omp barrier
				for(auto it = begin; it != end; ++it)
				{
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for calculating mean of computation times
				
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

					pTree=(*it);			
					
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;
		
					if(pTree->seednewly_produced>0)
					{
						int seedlebend=0;
						for(int sna=0; sna < pTree->seednewly_produced; sna++)
						{
							double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
							if(zufallsz>=parameter[0].seedconemort) 
							{
								++seedlebend;
							}
						}
						
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;
						
						if(seedlebend>0)
						{
							double start_timer_tresedliv=omp_get_wtime();	

							if( (parameter[0].pollination==1 && parameter[0].ivort>1045) || (parameter[0].pollination==9))
							{
								Pollinationprobability(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,        
												direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
												Vname,Vthdpth,
												n_trees
											);
							}
							
							double end_timer_tresedliv=omp_get_wtime();	
							timer_tresedliv+=end_timer_tresedliv-start_timer_tresedliv;

							for(int sl=0; sl<seedlebend; sl++)
							{
								pSeed= new Seed();
								
								pSeed->yworldcoo=aktortyworldcoo;
								pSeed->xworldcoo=aktortxworldcoo;
								pSeed->xcoo=pTree->xcoo;
								pSeed->ycoo=pTree->ycoo;
								pSeed->namem=pTree->name;
								
								// if chosen, determine the father by pollination out of available (matured) trees
								if((Vname.size()>0) && (parameter[0].pollination==1 || parameter[0].pollination==9))
								{
									int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
									pSeed->namep=Vname.at(iran);
									pSeed->thawing_depthinfluence=100;
								} 
								else
								{
									pSeed->namep=0;
									pSeed->thawing_depthinfluence=100;
								}
								
								pSeed->line=pTree->line;
								pSeed->generation=pTree->generation+1;
								pSeed->incone=true;
								pSeed->weight=1;
								pSeed->age=0;
								pSeed->species=pTree->species;
								pSeed->releaseheight=pTree->height;

								newseed_list.push_back(pSeed);
							}
							
							double end_timer_createseeds=omp_get_wtime();	
							timer_createseeds+=end_timer_createseeds-end_timer_tresedliv;
						}
						
						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;
					}
					
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;
				}// main tree loop on each core
				
				// append all newly created seed from each thread at once to the seed_list
				#pragma omp critical
				{
					seed_list.splice(seed_list.end(), newseed_list);
					
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
					
					timer_tresedliv_all+=timer_tresedliv/n_trees;
					timer_createseeds_all+=timer_createseeds/n_trees;
				}
			} // parallel region
			
			Vname.clear();Vname.shrink_to_fit();
			Vthdpth.clear();Vthdpth.shrink_to_fit();
		}// OMP==3
				
				
		// new output to speed up:
		// ... seed list: only if age==0 &&namep!=0
		// ... Ivort/X/Y/
		if(parameter[0].ivort>1045)
		{

						//print data in the most probable cases:
						char output[50];

						FILE *fdir;
						sprintf(output,"output/windgen_IVORT%.4d_REP%.3d.txt",parameter[0].ivort,parameter[0].repeati);
						
						fdir=fopen(output,"a+");
						
						if(fdir==NULL)
						{
						fdir=fopen(output,"a+");
						fprintf(fdir,"IVORT \t X0 \t Y0 \t namep  \t namem \n");
						}
						
						// # print data
								for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
								{
									pSeed=(*pos);
									
									if(pSeed->age==0)
									{
										fprintf(fdir,"%lf \t %lf \t %d \t %d \n",pSeed->xcoo,pSeed->ycoo,pSeed->namep,pSeed->namem);
									}
								}

						fclose(fdir);

		}// file output
			
		double end_time_poll=omp_get_wtime();
		TreeMort(yearposition, weather_list, tree_list);
		double end_time_mortpoll=omp_get_wtime();
		
		if(parameter[0].computationtimevis==1)
		{
			openpoll:
			FILE *fp4;
			fp4=fopen("t_N_poll.txt","a+");
			if(fp4==0)
			{goto openpoll;}
			fprintf(fp4,"%d;%lu;%lu;%10.10f;%10.10f;%10.10f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f\n",
					parameter[0].ivort, 
					seed_list.size(),
					tree_list.size(),
					(end_time_poll - end_time_seedsuviving), // pollination total
					(end_time_mortpoll - end_time_poll), // only tree mortality
					(end_time_seedsuviving - start_time_mortpoll), // seed mortality
					
					//timers each tree
					timer_eachtree_advance_all,
					timer_eachtree_vectini_all,
					timer_eachtree_seedsurv_all,
					timer_eachtree_seedadd_all,
					timer_eachtree_total_all,
					
					timer_tresedliv_all,
					timer_createseeds_all
				);
			fclose(fp4);
		}
	
	
	}// END: world loop

}



