void Etablierung( struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > & world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)
{
	//int startcpSNP=RAND_MAX-rand();//TEST!
	//int startmtSNP=RAND_MAX-rand();//TEST!

	int aktort=0;
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{ // Weltschleife Beginn

		vector<weather*>& weather_list = *posw;

		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;
	
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator world_positon_k = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list = *world_positon_k;

		aktort++;
		
			///go through seed list
			for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
			{ 

				pseed=(*pos);

				///if seed is not in a cone
				if (pseed->imcone==false)
				{ 
				
					//Determine if the seed germinates, depending on the density around it and...
					//
					
					
					
					//***german
					/// Ermitteln ob der seed keimt je
					/// nach der Auflagenstaerke am Ort und ...
					int i=(int) floor(pseed->ycoo*parameter[0].sizemagnif);//sizemagnif zB= 5, density grid tile.
					int j=(int) floor(pseed->xcoo*parameter[0].sizemagnif);
					
					// Formel aus PE Kapitel mit Feuer
					double keimungauflageinfluence=(1.0-0.01)/(200.0-600.0)*( (double) plot_list[i*treecols*parameter[0].sizemagnif+j]->auflagenstaerke )+1.495; 
					// (1-0.01)/(200-600)==Steigung damit durch bei 6 cm => 0.01 und 1.0 bei 2 cm, wichtig dabei: +1.495cm!!
					

					if (keimungauflageinfluence<0.01)
					{
						keimungauflageinfluence=0.01;	// minimal influence
					}
					
					/// ... and weather.
					
					
					//If this is switched on, the latest growth performance is calculated
					//***german
					// Falls eingestellt, die aktuelle Zuwachsleistung nach dem Ort ermitteln
					double maxbasalwachstum = 0;

					if (parameter[0].lineartransekt==true)
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
                                        //}
					}

					double zufallsz= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					
					///add new tree if seed germinates
					
					
					// Minimal germination rate was roughly estimated
					// keimungweathereinfluss= influence of current weather on germination (can be chosen between 0 and 1)
					//
					
					//***german:
					// Grundkeimungsrate nur geschätzt
					//keimungweathereinfluss=Einfluss von weather (maxbasalwachstum/gdbasal) auf Keimung (von 0-1 wählbar)
					
					if(pseed->species==1)
					{
						
						if (zufallsz< (parameter[0].keimungsrate+(parameter[0].keimungweathereinfluss*maxbasalwachstum/exp(parameter[0].gdbasalconstgmel))*keimungauflageinfluence) ) //original
						{  

							if (maxbasalwachstum>0.0) 
							{
								pTree= new Tree();						
								
								//***german:
								// Neuen Tree erzeugen
								
								
								//For SNP-assignment before it is inherited through the population:
								//first step: define mt and cpSNPs as random numbers < RAND_MAX+1 in the initialisation phase!
								//
								//	32 bit numbers are easily transformed to bits in a R analysation function!!!
								//
								//else mutationsrate: if rand()<= RAND_MAX*10^-4 for example
								// pTree->cpSNP+= or -= 2^rand()/RAND_MAX*sizeof(int)
								//
								
								/*//int choice=rand();
								
								if(pseed->generation==0)
								{//only with seed distribution as initialisation status...
								pTree->cpSNP[0]=startcpSNP;
								pTree->cpSNP[1]=startcpSNP+rand();//TEST!
								pTree->mtSNP[0]=startmtSNP;
								pTree->mtSNP[1]=startmtSNP+rand();//TEST!
								
								//loci=200 + SSRs=126 are test values, (different) parameters at some point
								for(int locus=0;locus<200;locus++) pTree->microsatellites.push_back(126);
								
								}
								else if((pseed->generation!=0)&&(choice>RAND_MAX*parameter[0].SNPMutationrate))
								{
									
								pTree->cpSNP[0]=pseed->cpSNP[0];
								pTree->cpSNP[1]=pseed->cpSNP[1];
								pTree->mtSNP[0]=pseed->mtSNP[0];
								pTree->mtSNP[1]=pseed->mtSNP[1];
								
								}
								else if((pseed->generation!=0)&&(choice<RAND_MAX*parameter[0].SNPMutationrate))
								{
									//produce integer random number in (0,+31)???
									//int random_number=rand()%(CHAR_BIT*sizeof(int));
									//cout<<"sizeof(int)="<<(CHAR_BIT*sizeof(int))<<endl;
									//cout<<"mutation at locus=="<<random_number<<endl;
									//int sign=(random_number% 2 ? 1 : -1);
								pTree->cpSNP[0]=pseed->cpSNP[0]+sign*pow(2,random_number);
								pTree->cpSNP[1]=pseed->cpSNP[1]+sign*pow(2,random_number);
								
								//both "SNPs" are mutated
								pTree->mtSNP[0]=pseed->mtSNP[0]+sign*pow(2,random_number);
								pTree->mtSNP[1]=pseed->mtSNP[1]+sign*pow(2,random_number);
								}//else if((pseed->generation!=0)&&(choice<RAND_MAX*parameter[0].microsatelliteMutationrate))
								{
									pTree->microsatellites[rand()%200]--;								
								}*/
								
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
								//pTree->Dbasalliste.push_back(dbasalneu);
								
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
								pTree->coneheight=99999.0;///??????
								pTree->seednewly_produced=0; 
								pTree->seedproduced=0; 
								pTree->speicher=1;
								pTree->densitywert=0;
								
								//First step for trait variation: the influence the thawing depth has on a tree
								//Through root growth factors pollen dependent inheritance?
								//This is just the establishment though
								
								// if(rand()%1000)
								//{
									//pTree->thawing_depthinfluence=NormverteilungRN(100, 0.10*100);
									//pTree->thawing_depthinfluence=100;//NormverteilungRN(100, 0.10*100);
								//cout<<"etablierung:"<<pTree->thawing_depthinfluence<<endl;
								// }
                                                                //else
                                                                //{pTree->thawing_depthinfluence=pseed->thawing_depthinfluence;}
								
								pTree->entfernung=pseed->entfernung;
								pTree->growing=true;
								pTree->species=pseed->species;
								
								//pTree->thawing_depthinfluence=100;
								
								tree_list.push_back(pTree);				// insert tree in the corresponding tree_list...

								delete pseed;							// ...and delete the seed the tree grew from
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
						if (zufallsz< (parameter[0].keimungsrate+(parameter[0].keimungweathereinfluss*maxbasalwachstum/exp(parameter[0].gdbasalconstsib))*keimungauflageinfluence) ) //original
						{  

							if (maxbasalwachstum>0.0) 
							{
								pTree= new Tree();						//a new tree is established
								
								//first step: define mt and cpSNPs as random numbers < RAND_MAX+1 in the initialisation phase!
								//
								//	32 bit (!) numbers are easily transformed to bits in a R analysation function!!!
								//
								//else mutationsrate: if rand()<= RAND_MAX*10^-4 for example
								// pTree->cpSNP+= or -= 2^rand()/RAND_MAX*sizeof(int)
								//
								/*//int choice=rand();
								
								if(pseed->generation==0)
								{//only with seed distribution as initialisation status...
								pTree->cpSNP[0]=startcpSNP;
								pTree->cpSNP[1]=startcpSNP+rand();//TEST!
								pTree->mtSNP[0]=startmtSNP;
								pTree->mtSNP[1]=startmtSNP+rand();//TEST!
								
								//loci=200 + SSRs=126 are test values, (different) parameters at some point
								for(int locus=0;locus<200;locus++) pTree->microsatellites.push_back(126);
								
								}
								else if((pseed->generation!=0)&&(choice>RAND_MAX*parameter[0].SNPMutationrate))
								{
									
								pTree->cpSNP[0]=pseed->cpSNP[0];
								pTree->cpSNP[1]=pseed->cpSNP[1];
								pTree->mtSNP[0]=pseed->mtSNP[0];
								pTree->mtSNP[1]=pseed->mtSNP[1];
								
								}
								else if((pseed->generation!=0)&&(choice<RAND_MAX*parameter[0].SNPMutationrate))
								{
									//produce integer random number in (0,+31)???
									//int random_number=rand()%(CHAR_BIT*sizeof(int));
									//cout<<"sizeof(int)="<<(CHAR_BIT*sizeof(int))<<endl;
									//cout<<"mutation at locus=="<<random_number<<endl;
									//int sign=(random_number% 2 ? 1 : -1);
								pTree->cpSNP[0]=pseed->cpSNP[0]+sign*pow(2,random_number);
								pTree->cpSNP[1]=pseed->cpSNP[1]+sign*pow(2,random_number);
								
								//noch mutiert er beides:
								pTree->mtSNP[0]=pseed->mtSNP[0]+sign*pow(2,random_number);
								pTree->mtSNP[1]=pseed->mtSNP[1]+sign*pow(2,random_number);
								}//else if((pseed->generation!=0)&&(choice<RAND_MAX*parameter[0].microsatelliteMutationrate))
								{
									pTree->microsatellites[rand()%200]--;								
								}*/
								
								
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
								
								//pTree->Dbasalliste.push_back(dbasalneu);
								
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
								tree_list.push_back(pTree);				// insert tree in the corresponding tree_list...

								delete pseed;							// ...and delete the seed the tree grew from
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
						
						//Safety routine: is the substructure species now defined?
						
						
						
						//***german:
						// Sicherheitsabfrage Art-Variable gesetzt?
						/* Abfrage ob das Programm beendet oder fortgesetzt werden soll */ 
						signed int abbrechenetabfehler; 
						
						printf("\n In establishment.cpp a tree was not assigned a new species value.\n");
						printf("\n continue typing 1, press any other key to exit\n");
						
						
						//***german:
						//printf("\n In der Etablierungsfunktion hat ein seed keinen Wert in der Variable pseed->species\n"); 
						//printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n"); 
						scanf("%d", &abbrechenetabfehler); 

						if (abbrechenetabfehler!=1) 
						{
							printf("LAVESI was exited after an error occuring in establishment.cpp\n");
							//printf("LaVeSi wurde nach einem Art-Fehler in der Etablierung beendet\n\n");
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

} // End of the function Etablierung
