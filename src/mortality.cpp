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
		// Calculation of the factors in a function, that adds a mortality rate impact
		
		//***german:
		/// Verrechnung der Faktoren in einer Funktion, die einen Mortalitysratenaufschlag darstellt
		double anstiegweathermortg=160;
		double anstiegweathermorts=160;
		
		anstiegweathermortg=(60*weather_list[yearposition_help]->janisothermrestriktiong+60*weather_list[yearposition_help]->julisothermrestriktion+60*weather_list[yearposition_help]->nddrestriktion);
		anstiegweathermorts=(60*weather_list[yearposition_help]->janisothermrestriktions+60*weather_list[yearposition_help]->julisothermrestriktion+60*weather_list[yearposition_help]->nddrestriktion);

		
		// biotic influence:
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{//begin tree_list iteration
			pTree=(*pos);
	
			
			if(pTree->growing==true)
			{
				//If maximal age is exceeded an additional factor occurs
				
				//***german:
				// Falls Hoechstage ueberschritten wird ein zusaetzlicher Wert aufgeschlagen
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
					wachstumrel=pTree->dbrustrel;
				}
				
				// Competition effect:
				// small 	     Tree => barely influenced by surrounding trees
				// 150cm tall    Tree => strongest influence
				// height >300cm Tree => barely influenced by surrounding trees
				// switched off at the moment
				
				
				//***german:
				// ==> hat was mit Konkurrenzeffekt zu tun. 
				//     Kleiner Baum - kaum beeinflusst, 
				//	   150 cm  - am stärksten beeinflusst, 
				//	   300 cm und größer - kaum beeinflusst 
				//	   im Moment ausgeschaltet
				double heightnkugeleinfluss=1; //war bisher auf 0.  Dadurch wurde density in der Mortalität nicht berücksichtigt
				if (pTree->height<(parameter[0].densityvaluemaximumatheight*2)) 
				{	
					heightnkugeleinfluss=heightnkugeleinfluss+(sqrt(pow(parameter[0].densityvaluemaximumatheight,2)-pow(pTree->height-parameter[0].densityvaluemaximumatheight, 2))/parameter[0].densityvaluemaximumatheight);
				}	
				
				// Calculation of the young trees mortalities
				
				//***german:
				// Verrechnung der Mortality von Bäumen
				// fuer die jugendmortalitaet
				double maxhg;
				double maxhs;
				double maxhpufg=weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);
				double maxhpufs=weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);

				///if maxbasalwachstumjahr <= 0
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
				double dens_mort	= parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert; // Treedensity innerhalb eines Umkreises um den aktuellen Tree;
				double weathermortaddg = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * 
											exp(anstiegweathermortg * weather_list[yearposition_help]->weatherfactorg*
												exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)))));
				double weathermortadds = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * 
											exp(anstiegweathermorts * weather_list[yearposition_help]->weatherfactors*
												exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)))));	
				double weather_mort_gmel	= parameter[0].mweather * weathermortaddg * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);
				double weather_mort_sib	= parameter[0].mweather * weathermortadds * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);				
				double dry_mort		= parameter[0].mdrought * weather_list[yearposition_help]->trockenheitsmort * pow((1.0 / pTree->height), 0.5); // Verrechnung der Trockenheit aus Trockenheitsindex
				
				/// Calculating the mortality rate of the tree 
				/// considering the factors of each mortality rate
				double Treemortg = 0.0 
								+ parameter[0].mortbg //Background Mortality 0.0005
								+ sapl_mort_gmel
								+ age_mort
								+ growth_mort 
								+ dens_mort
								+ weather_mort_gmel 
								+ dry_mort;
								
				double Treemorts = 0.0 
								+ parameter[0].mortbg //Background Mortality 0.0005
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
						pTree->dbasalrel,pTree->dbrustrel,
						parameter[0].mortyouth*pow(exp((-1.0*pTree->height)+maxhg),parameter[0].mortyouthinfluenceexp),
						parameter[0].mortage*(agesmort*(10.0*parameter[0].mortbg)), 
						+ (1.0-pow(wachstumrel,parameter[0].relgrowthmortinfluenceexp)),
						parameter[0].mdensity*heightnkugeleinfluss*pTree->densitywert, 
						parameter[0].mweather*weathermortaddg*pow((1.0/pTree->height),0.5), 
						parameter[0].mdrought*(weather_list[yearposition_help]->trockenheitsmort*pow((1.0/pTree->height),0.5)), 
						Treemortg);
				}

				if (parameter[0].mortvis==true && pTree->height>200.0 && pTree->species==2) 
				{
						printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +drought=%4.4f =>%4.2f", 
						pTree->height, 
						pTree->dbasalrel,pTree->dbrustrel,
						parameter[0].mortyouth*pow(exp((-1.0*pTree->height)+maxhs),parameter[0].mortyouthinfluenceexp),
						parameter[0].mortage*(agesmort*(10.0*parameter[0].mortbg)), 
						+ (1.0-pow(wachstumrel,parameter[0].relgrowthmortinfluenceexp)),
						parameter[0].mdensity*heightnkugeleinfluss*pTree->densitywert, 
						parameter[0].mweather*weathermortadds*pow((1.0/pTree->height),0.5), 
						parameter[0].mdrought*(weather_list[yearposition_help]->trockenheitsmort*pow((1.0/pTree->height),0.5)), 
						Treemorts);
				}

				// Determine if a tree dies (deletion of said tree in the corresponding list)
				
				//***german:
				// Ermitteln ob der aktuelle Tree stirbt, wenn ja, so wird der entsprechende ...
				// ... Eintrag in der tree_list geloescht
				
				
				///if random number < probability, tree dies
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

			}//is the tree growing?
			else
			{
				++pos;
			}

		} // End tree_list iteration
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
	{// START: world loop
		//To access the weather_list content, a referential weather list has to be created, to represent the structure of it.
		//
		//To access a certain element a corresponding iterator has to be adjusted to fit the current element and creation of a referential tree list
		//
		
		//***german:
		// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Referenz
		// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
		// realisiert ist weiterverfahren werden
		// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
		vector<Weather*>& weather_list = *posw;

		// Um auf ein bestimmtes Element in der Welt zuzugreifen muss ein Iterator bis
		// zum entsprechenden Element justiert werden und dann eine tree_list als Referenz
		// erzeugt werden
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;
		


		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<Seed*>& seed_list = *world_positon_s;

		aktort++;

		double start_time_mortpoll=omp_get_wtime();
		
	// https://stackoverflow.com/questions/16777810/c-omp-omp-get-wtime-returning-time-0-00 
	// ... problem time() returns cpu-time not real time use omp_get_wtime()!!
	// printf... with "%.16" for double ini
	
	
	/*	// start: ORI CODE  */
		//cout << "seed_list.size() vor Mortalität = " << seed_list.size() << endl;
		/// seedmortalität
		for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{
			pSeed=(*pos);
			double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
			
			///seed is on ground && random number < probability (0.8)
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
			
			///seed in cone && random number < probability (0.8)
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
				//safety procedure: has the variable incone been set?
				signed int abbrechenmortalitaetfehler; 
				printf("\n In the mortality determining function a seed has no value applied to the variable incone\n"); 
				printf("\n To continue the simulation, press 1, stop it with any other key\n"); 
				
				scanf("%d", &abbrechenmortalitaetfehler); 
                                if (abbrechenmortalitaetfehler!=1) 
                                {
                                    printf("LaVeSi has been stopped after a failure in mortality.cpp\n\n");exit(1);
                                }
		
				delete pSeed;
				pos=seed_list.erase(pos);						
			}
		}
	// end: ORI CODE
	
	


	/* PARALLELIZATION OF THE SEED MORTALITY LOOP 
	
	
	SEED MORTALITY PARALLELIZATION NO_1 		
		x1.create dummy_seed_list_globally => "seed_list_global"
		x2.create dummy_seed_list_locally for each thread => "seed_list_local"
		x3.iter over orig_seed_list parallel
			copy only surviving seeds to dummy_seed_list_locally
		x4.splice dummy_seed_list_locally to dummy_seed_list_globally
		5.swap content of dummy_seed_list_globally to orig_seed_list "seed_list"
	
	
	SEED MORTALITY PARALLELIZATION NO_2
			++ by hand
			https://stackoverflow.com/questions/8691459/how-do-i-parallelize-a-for-loop-through-a-c-stdlist-using-openmp
			
	SEED MORTALITY PARALLELIZATION NO_3
		based on NO_2 but with a local list to which the data is pushed (or copied??)
		
		including various versions to test the efficiency

	
	*/
		
	double end_time_seedsuviving=omp_get_wtime();

		//Calculation of the current location in transect coordinates
		
		//***german:
		// Berechnung des aktuellen Ortes in Koordinaten
	int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
	int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		// Loop for each seed that is to be produced for each tree
		
		// timers for parallel processing 
	double timer_eachtree_advance_all=0;
	double timer_eachtree_vectini_all=0;
	double timer_eachtree_seedsurv_all=0;// from here only surviving seeds
	double timer_eachtree_seedadd_all=0;// from here only surviving seeds
	double timer_eachtree_total_all=0;// from here only surviving seeds
		
	double timer_tresedliv_all=0;//find pollenfather
	double timer_createseeds_all=0;//creates seeds

// manually chose the implementation of multi-core-processing
int mcorevariant=3;
	// 1 == only advance
	// ... ==> result: advance makes the computation really really slow!!
	// 2 == split list to X lists => had to outsource the delete precedure because otherwise it would lead to SEGFAULTs!
	// 3 == trees are ordered by age which is highly correlated with seedprodAKT
	// ... try to find last tree with seeds and then split lists over first until last tree
if(mcorevariant==1)
{// OMP==1
	// more than one kernel specified by parameter[0].omp_num_threads
	
	
		// loop around the loop for MULTI-CORE-PROCESSING
		// before run a program parallel set the number of helpers by changing the environment variable
		// ... ... export OMP_NUM_THREADS=4
		// ... or explicitly overwrite the environmental variable by setting the helper number directly
		// ... ... omp_set_num_threads(int); // which is set in the parameter.txt file variable "omp_num_threads"
		// This implementation was formerly (21st April and backwards) named "Testversion 2" - give each thread a local seed_list and splice them together in the end

		// loop with omp through each element of the list
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		// omp_set_num_threads(1); //set the number of helpers
		// directly modified as the parralelization is slowing the computations
		
		/// #####################################
		/// #####################################
		/// new paralellisierung for BefrWarh!!
		// ... also include in private:
		// pTree_copy
		///
			  // list<Tree*>& tree_list = *world_positon_b;
			  double  richtung=0.0;
			  double  geschwindigkeit=0.0;
			  unsigned int    ripm=0,cntr=0;
			  double  p=0.0,kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].pollengregoryc;
			  double  m=parameter[0].pollengregorym;
			  
///
			  	vector<int> Vname; // moved here from the top of this file
				vector<double> Vthdpth;
///
		/// #####################################
		/// #####################################
		#pragma omp parallel default(shared) private(pTree,pSeed,       pTree_copy,    richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
		{ // START: parallel region
			// declare a local seed list to be filled by each thread
			list<Seed*> newseed_list;
			
		richtung=0.0;geschwindigkeit=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);
		I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;phi=0.0;dr=0.0;dx=0.0;dy=0.0;
		
				// timers
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;	// from here on only surviving seeds
				double timer_eachtree_seedadd=0;	// from here on only surviving seeds
				double timer_eachtree_total=0;		// from here on only surviving seeds
			
			#pragma omp for nowait schedule(guided) 
			for(unsigned int pari=0; pari<tree_list.size(); ++pari)
			{// START: main tree loop
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for later calculating mean of computation times
				
				list<Tree*>::iterator posb=tree_list.begin();
				// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
				advance(posb, pari);
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pSeed) and container (Vname)
				pTree=(*posb);			
				
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;

				if((parameter[0].ivort==1) & (pari==0))// check the number of used threads
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				
				if(pTree->seednewly_produced>0)
				{//START: tree produces seeds
					// ramdomly determine the number of surving seeds
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
					{// START: if seedlebend>0
						if( (parameter[0].pollination==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollination==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							Pollinationprobability(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth,
											n_trees);
						}
				
						// get the characteristics for each surviving seed and push these back new to seed_list
						for(int sl=0; sl<seedlebend; sl++)
						{// START: create new seeds
						 // create a new seed
							pSeed= new Seed();
							
							// add information
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
							pSeed->generation=pTree->generation+1;	// generation==0 introduced from outside
							pSeed->incone=true;
							pSeed->weight=1;
							pSeed->age=0;
							pSeed->species=pTree->species;// species is inherited from the seed source
							pSeed->releaseheight=pTree->height;

							// add seed to seed_list
							newseed_list.push_back(pSeed);
						}// END: create new seeds

					}// END: if seedlebend>0
						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;

				}// END: tree produces seeds
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;

			}//END: main tree loop
			
			// append all newly created seed from each thread at once to the seed_list
			#pragma omp critical
			{
				seed_list.splice(seed_list.end(), newseed_list);
				
				// timing calculations
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
			}
		} // END: parallel region

		Vname.clear();Vname.shrink_to_fit();
		Vthdpth.clear();Vthdpth.shrink_to_fit();
		
}// OMP==1
if(mcorevariant==2)
{// OMP==2

// more than one kernel specified by parameter[0].omp_num_threads
	
	
		// loop around the loop for MULTI-CORE-PROCESSING
		// before run a program parallel set the number of helpers by changing the environment variable
		// ... ... export OMP_NUM_THREADS=4
		// ... or explicitly overwrite the environmental variable by setting the helper number directly
		// ... ... omp_set_num_threads(int); // which is set in the parameter.txt file variable "omp_num_threads"
		// This implementation was formerly (21st April and backwards) named "Testversion 2" - give each thread a local seed_list and splice them together in the end

		// loop with omp through each element of the list
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		// omp_set_num_threads(1); //set the number of helpers
		// directly modified as the parralelization is slowing the computations
		

		/// #####################################
		/// #####################################
		/// new paralellisierung for BefrWarh!!
		// ... also include in private:
		// pTree_copy
		///
			  // list<Tree*>& tree_list = *world_positon_b;
			  double  richtung=0.0;
			  double  geschwindigkeit=0.0;
			  unsigned int    ripm=0,cntr=0;
			  // vector<int> SNP1,SNP2;
			  double  p=0.0,kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].pollengregoryc;
			  double  m=parameter[0].pollengregorym;
			  	vector<int> Vname;// moved here from the top of this file
				vector<double> Vthdpth;
///
		/// #####################################
		/// #####################################
		#pragma omp parallel default(shared) private(pTree,pSeed,       pTree_copy,    richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
		{ // START: parallel region
			if((parameter[0].ivort==1))// check the number of used threads
			{
				cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
			}

			richtung=0.0;geschwindigkeit=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);
			I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;phi=0.0;dr=0.0;dx=0.0;dy=0.0;
		

			// initialize the info for each of the thread
			int thread_count = omp_get_num_threads();
			int thread_num   = omp_get_thread_num();
			size_t chunk_size= tree_list.size() / thread_count;
			auto begin = tree_list.begin();
			std::advance(begin, thread_num * chunk_size);
			auto end = begin;
			
			if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
			{
				end = tree_list.end();
				// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
			} 
			else
			{
				std::advance(end, chunk_size);
				// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
			}
					
					
					
					
			// declare a local seed list to be filled by each thread
			list<Seed*> newseed_list;
				// timers
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;// from here only surviving seeds
				double timer_eachtree_seedadd=0;// from here only surviving seeds
				double timer_eachtree_total=0;// from here only surviving seeds
			
			
			
			// wait for all threads to initialize and then proceed
			#pragma omp barrier
			for(auto it = begin; it != end; ++it)
			{// START: main tree loop
			
			
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;
					
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pSeed) and container (Vname)
				// pTree=(*posb);			
				pTree=(*it);			
				
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;

				
				if(pTree->seednewly_produced>0)
				{//START: tree produces seeds
						
					// ramdomly determine the number of surving seeds
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
					{// START: if seedlebend>0
						if( (parameter[0].pollination==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollination==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							Pollinationprobability(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth,
											n_trees);//;
						}
				
						// get the characteristics for each surviving seed and push these back new to seed_list
						for(int sl=0; sl<seedlebend; sl++)
						{// START: create new seeds
							// create a new seed
							pSeed= new Seed();
							
							// add information
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
							pSeed->generation=pTree->generation+1;	// generation==0 introduced from outside
							pSeed->incone=true;
							pSeed->weight=1;
							pSeed->age=0;
							pSeed->species=pTree->species;// species is inherited from the seed source
							pSeed->releaseheight=pTree->height;

							// add seed to seed_list
							newseed_list.push_back(pSeed);
						}// END: create new seeds

					
					}// END: if seedlebend>0
						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;

				}// END: tree produces seeds
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;

			}//END: main tree loop on each core
			

			
			// append all newly created seed from each thread at once to the seed_list
			#pragma omp critical
			{
				seed_list.splice(seed_list.end(), newseed_list);
				
				// timing calculations
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
			}
		} // END: parallel region
		Vname.clear();Vname.shrink_to_fit();
		Vthdpth.clear();Vthdpth.shrink_to_fit();
		

	
}// OMP==2
if(mcorevariant==3)
{// OMP==3

// more than one kernel specified by parameter[0].omp_num_threads
	
		// loop around the loop for MULTI-CORE-PROCESSING
		// before run a program parallel set the number of helpers by changing the environment variable
		// ... ... export OMP_NUM_THREADS=4
		// ... or explicitly overwrite the environmental variable by setting the helper number directly
		// ... ... omp_set_num_threads(int); // which is set in the parameter.txt file variable "omp_num_threads"
		// This implementation was formerly (21st April and backwards) named "Testversion 2" - give each thread a local seed_list and splice them together in the end

		// loop with omp through each element of the list
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		// omp_set_num_threads(1); //set the number of helpers
		// directly modified as the parallelisation is slowing the computations
		
		

				
				
		/// new paralellisierung for BefrWarh!!
		// ... also include in private:
		// pTree_copy
		///
			  // list<Tree*>& tree_list = *world_positon_b;
			  double  richtung=0.0;
			  double  geschwindigkeit=0.0;
			  unsigned int    ripm=0,cntr=0;
			  // vector<int> SNP1,SNP2;
			  double  p=0.0,kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].pollengregoryc;
			  double  m=parameter[0].pollengregorym;
			  
			  	vector<int> Vname;//// moved here from the top of this file
				vector<double> Vthdpth;
		
						
				// set end to last tree with produced seeds
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
			
				
		#pragma omp parallel default(shared) private(pTree,pSeed,       pTree_copy,    richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
		{// START: parallel region
		

				
		  richtung=0.0;geschwindigkeit=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);phi=0.0;dr=0.0;dx=0.0;dy=0.0;
			    I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;

			// initialize the info for each of the thread
			int thread_count = omp_get_num_threads();
			int thread_num   = omp_get_thread_num();
			// size_t chunk_size= tree_list.size() / thread_count;
			size_t chunk_size= lasttreewithseeds_pos / thread_count;
			auto begin = tree_list.begin();
			std::advance(begin, thread_num * chunk_size);
			auto end = begin;
			
			if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
			{
				if((parameter[0].ivort==1))// check the number of used threads
				{
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				}
			
				end = lasttreewithseeds_iter;
			} 
			else
			{
				std::advance(end, chunk_size);
			}
	
			// declare a local seed list to be filled by each thread
			list<Seed*> newseed_list;
			
				// timers
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;// from here only surviving seeds
				double timer_eachtree_seedadd=0;// from here only surviving seeds
				double timer_eachtree_total=0;// from here only surviving seeds
				
				double timer_tresedliv=0;//find pollenfather
				double timer_createseeds=0;//creates seeds

			
			
			
			// wait for all threads to initialize and then proceed
			#pragma omp barrier
			for(auto it = begin; it != end; ++it)
			{// START: main tree loop
			
			// #pragma omp for nowait schedule(guided) 
			
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for later calculating mean of computation times
				
				// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
				// advance(posb, pari);
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pSeed) and container (Vname)
		
				pTree=(*it);			

				
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;

				
				if(pTree->seednewly_produced>0)
				{//START: tree produces seeds
						
					// ramdomly determine the number of surving seeds
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
					{// START: if seedlebend>0
						double start_timer_tresedliv=omp_get_wtime();	

						if( (parameter[0].pollination==1 && parameter[0].ivort>1045) || (parameter[0].pollination==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							Pollinationprobability(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth,
											n_trees);
						}
						double end_timer_tresedliv=omp_get_wtime();	
						timer_tresedliv+=end_timer_tresedliv-start_timer_tresedliv;
						// get the characteristics for each surviving seed and push these back new to seed_list
						for(int sl=0; sl<seedlebend; sl++)
						{// START: create new seeds
							// create a new seed
							pSeed= new Seed();
							
							// add information
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
							pSeed->generation=pTree->generation+1;	// generation==0 introduced from outside
							pSeed->incone=true;
							pSeed->weight=1;
							pSeed->age=0;
							pSeed->species=pTree->species;// species is inherited from the seed source
							pSeed->releaseheight=pTree->height;

							// add seed to seed_list
							newseed_list.push_back(pSeed);
						}// END: create new seeds
						double end_timer_createseeds=omp_get_wtime();	
						timer_createseeds+=end_timer_createseeds-end_timer_tresedliv;
					}// END: if seedlebend>0
						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;

				}// END: tree produces seeds
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;

			}//END: main tree loop on each core
			
			
			
			
			
			// append all newly created seed from each thread at once to the seed_list
			#pragma omp critical
			{
				seed_list.splice(seed_list.end(), newseed_list);
				
				// timing calculations
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
					
					timer_tresedliv_all+=timer_tresedliv/n_trees;
					timer_createseeds_all+=timer_createseeds/n_trees;
			}
		} // END: parallel region
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



