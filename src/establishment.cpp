using namespace std;


void Etablierung( struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > & world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)
{
	int aktort=0;
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{

		vector<weather*>& weather_list = *posw;

		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;
	
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator world_positon_k = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list = *world_positon_k;

		aktort++;
		
			// go through seed list
			for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
			{
				pseed=(*pos);

				if (pseed->imcone==false)
				{ 
				
					// determine if the seed germinates, depending on the density around it and the litter layer
					int i=(int) floor(pseed->ycoo*parameter[0].sizemagnif);
					int j=(int) floor(pseed->xcoo*parameter[0].sizemagnif);
					
					double keimungauflageinfluence=(1.0-0.01)/(200.0-600.0)*( (double) plot_list[i*treecols*parameter[0].sizemagnif+j]->auflagenstaerke )+1.495; 
					
					if (keimungauflageinfluence<0.01)
					{
						keimungauflageinfluence=0.01;
					}
					
					// ... and weather.
					// calculate the latest growth performance
					double maxbasalwachstum = 0;

					if (parameter[0].lineartransect==true)
					{ 
						if(pseed->species==1)
						{
							maxbasalwachstum = exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*0+parameter[0].gdbasalfacqgmel*0*0)*
											(((weather_list[yearposition]->weatherfactorg
												-weather_list[yearposition]->weatherfactorming)
												/((double) treerows))*pseed->ycoo 
											+weather_list[yearposition]->weatherfactorming);				
						}
						else if(pseed->species==2)
						{
							maxbasalwachstum = exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*0+parameter[0].gdbasalfacqsib*0*0)*
											(((weather_list[yearposition]->weatherfactors
												-weather_list[yearposition]->weatherfactormins)
												/((double) treerows))*pseed->ycoo 
											+weather_list[yearposition]->weatherfactormins);
						}
					}
					else
					{
						if(pseed->species==1)
						{
							maxbasalwachstum = exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*0+parameter[0].gdbasalfacqgmel*0*0)*
											weather_list[yearposition]->weatherfactorg;
						}
						else if(pseed->species==2)
						{
							maxbasalwachstum = exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*0+parameter[0].gdbasalfacqsib*0*0)*
											weather_list[yearposition]->weatherfactors;
						}
					}

					double zufallsz= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					
					// add new tree if seed germinates
					// minimal germination rate is roughly estimated
					if(pseed->species==1)
					{
						
						if (zufallsz< (parameter[0].germinationrate+(parameter[0].keimungweathereinfluss*maxbasalwachstum/exp(parameter[0].gdbasalconstgmel))*keimungauflageinfluence) )
						{  

							if (maxbasalwachstum>0.0) 
							{
								pTree= new Tree();						
								
								pTree->yworldcoo=pseed->yworldcoo;
								pTree->xworldcoo=pseed->xworldcoo;
								pTree->xcoo=pseed->xcoo;				
								pTree->ycoo=pseed->ycoo;
								pTree->name=++parameter[0].nameakt;
								pTree->namem=pseed->namem;
								pTree->namep=pseed->namep;
								pTree->yr_of_establishment=yearposition;
								pTree->line=pseed->line;
								pTree->generation=pseed->generation;
								pTree->dbasal=maxbasalwachstum;
								pTree->dbasalrel=1.0;
								pTree->dbrust=0.0;
								pTree->dbrustrel=1.0;
								
								if (parameter[0].allometriefunktionstyp==1) 
								{
									pTree->height= parameter[0].dbasalheightallosteig * pow(maxbasalwachstum, parameter[0].dbasalheightalloexp);
								}
								else 
								{
									pTree->height= parameter[0].dbasalheightsteignonlin*maxbasalwachstum;	
								}
								pTree->age=0; 
								pTree->cone=0; 
								pTree->coneheight=99999.0;
								pTree->seednewly_produced=0; 
								pTree->seedproduced=0; 
								pTree->speicher=1;
								pTree->densitywert=0;
								pTree->entfernung=pseed->entfernung;
								pTree->growing=true;
								pTree->species=pseed->species;
								pTree->thawing_depthinfluence=100;
								
								tree_list.push_back(pTree);

								delete pseed;
								pos=seed_list.erase(pos);
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
					else if(pseed->species==2)
					{
						if (zufallsz< (parameter[0].germinationrate+(parameter[0].keimungweathereinfluss*maxbasalwachstum/exp(parameter[0].gdbasalconstsib))*keimungauflageinfluence) )
						{  

							if (maxbasalwachstum>0.0) 
							{
								pTree= new Tree();														
								
								pTree->yworldcoo=pseed->yworldcoo;
								pTree->xworldcoo=pseed->xworldcoo;
								pTree->xcoo=pseed->xcoo;
								pTree->ycoo=pseed->ycoo;
								pTree->name=++parameter[0].nameakt;
								pTree->namem=pseed->namem;
								pTree->namep=pseed->namep;
								pTree->yr_of_establishment=yearposition;
								pTree->line=pseed->line;
								pTree->generation=pseed->generation;
								pTree->dbasal=maxbasalwachstum;
								pTree->dbasalrel=1.0;
								pTree->dbrust=0.0;
								pTree->dbrustrel=1.0;
								
								if (parameter[0].allometriefunktionstyp==1) 
								{
									pTree->height= parameter[0].dbasalheightallosteig * pow(maxbasalwachstum, parameter[0].dbasalheightalloexp);
								}
								else 
								{
									pTree->height= parameter[0].dbasalheightsteignonlin*maxbasalwachstum;	
								}
								
								pTree->age=0; 
								pTree->cone=0; 
								pTree->coneheight=99999.0;
								pTree->seednewly_produced=0; 
								pTree->seedproduced=0; 
								pTree->speicher=1;
								pTree->densitywert=0;
								pTree->thawing_depthinfluence=100;
								pTree->entfernung=pseed->entfernung;						
								pTree->growing=true;
								pTree->species=pseed->species;
								tree_list.push_back(pTree);	

								delete pseed;
								pos=seed_list.erase(pos);
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
					else 
					{ 
						// safety routine: is the substructure species now defined?
						signed int abbrechenetabfehler; 
						printf("\n In establishment.cpp a tree was not assigned a new species value.\n");
						printf("\n continue typing 1, press any other key to exit\n");
						scanf("%d", &abbrechenetabfehler); 

						if (abbrechenetabfehler!=1) 
						{
							printf("LAVESI was exited after an error occurring in establishment.cpp\n");
							exit(1);
						}

						delete pseed;
						pos=seed_list.erase(pos);	
					}
				} 
				
				else if (pseed->imcone==true)
				{
					++pos;
				}
			} 
	} 

}

