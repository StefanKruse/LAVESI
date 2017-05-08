






/****************************************************************************************//**
 * \brief calculate tree mortality
 *
 * depends on abiotic factors (temperature and number of days with temperatures above 20 degrees)
 * and on biotic factors
 *
 *
 *******************************************************************************************/
void TreeMort(int yearposition_help,vector<weather*> &weather_list,list<Tree*> &tree_list)
{
		//console output:
		if (parameter[0].jahranzeige ==true) {
				cout << "tree_list.size() vor Mortalität = " << tree_list.size() << endl;
		}
		
		/// Verrechnung der Faktoren in einer Funktion, die einen Mortalitaetsratenaufschlag darstellt
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
				// Falls Hoechstage ueberschritten wird ein zusaetzlicher Wert aufgeschlagen
				double agesmort=0.0;
				if (pTree->age>parameter[0].maximalage) 
				{
					agesmort=1.0;
				}
		
				// heightnabhaengige Einfluesse
				double wachstumrel=1.0;
				if (pTree->height<130.0) 
				{
					wachstumrel=pTree->dbasalrel;
				}
				else 
				{
					wachstumrel=pTree->dbrustrel;
				}

				// ==> hat was mit Konkurrenzeffekt zu tun. 
				//     Kleiner Tree - kaum beeinflusst, 
				//	   150 cm  - am stärksten beeinflusst, 
				//	   300 cm und größer - kaum beeinflusst 
				//	   im Moment ausgeschaltet
				double heightnkugeleinfluss=1; //war bisher auf 0.  Dadurch wurde density in der Mortalität nicht berücksichtigt
				if (pTree->height<(parameter[0].densitywertmaximumbeiheight*2)) 
				{	
					heightnkugeleinfluss=heightnkugeleinfluss+(sqrt(pow(parameter[0].densitywertmaximumbeiheight,2)-pow(pTree->height-parameter[0].densitywertmaximumbeiheight, 2))/parameter[0].densitywertmaximumbeiheight);
				}	
				
				// Verrechnung der Mortalitaet von treesn
				// fuer die jugendmortalitaet
				double maxhg;
				double maxhs;
				double maxhpufg=weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);
				double maxhpufs=weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);

				///if maxbasalwachstumjahr <= 0
				if (maxhpufg<=0.0)
				{
					if (parameter[0].allometriefunktionstyp==1)
					{
						maxhg= parameter[0].dbasalheightallosteig * pow(exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
					}
					else 
					{
						maxhg= parameter[0].dbasalheightsteignonlin*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);	
					}
				}
				else 
				{
					if (parameter[0].allometriefunktionstyp==1)
					{
						maxhg= parameter[0].dbasalheightallosteig * pow(weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
					}
					else 
					{
						maxhg= parameter[0].dbasalheightsteignonlin*weather_list[yearposition_help]->weatherfactorg*exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal);
					}
				}
				
				if (maxhpufs<=0.0)
				{
					if (parameter[0].allometriefunktionstyp==1)
					{
						maxhs= parameter[0].dbasalheightallosteig * pow(exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
					}
					else 
					{
						maxhs= parameter[0].dbasalheightsteignonlin*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);	
					}
				}
				else 
				{
					if (parameter[0].allometriefunktionstyp==1)
					{
						maxhs= parameter[0].dbasalheightallosteig * pow(weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal), parameter[0].dbasalheightalloexp);
					}
					else 
					{
						maxhs= parameter[0].dbasalheightsteignonlin*weather_list[yearposition_help]->weatherfactors*exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal);
					}
				}
				
				double sapl_mort_gmel	= parameter[0].mjung * pow(exp((-1.0 * pTree->height) + maxhg), parameter[0].jugendmorteinflussexp);
				double sapl_mort_sib	= parameter[0].mjung * pow(exp((-1.0 * pTree->height) + maxhs), parameter[0].jugendmorteinflussexp);
				double age_mort		= parameter[0].malt	* agesmort * (10.0 * parameter[0].mortbg);
				double growth_mort	= parameter[0].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relwachstummorteinflussexp));
				double dens_mort	= parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert; // Treedensity innerhalb eines Umkreises um den aktuellen Tree;
				double weathermortaddg = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * 
											exp(anstiegweathermortg * weather_list[yearposition_help]->weatherfactorg*
												exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)))));
				double weathermortadds = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * 
											exp(anstiegweathermorts * weather_list[yearposition_help]->weatherfactors*
												exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)))));	
				double weather_mort_gmel	= parameter[0].mweather * weathermortaddg * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);
				double weather_mort_sib	= parameter[0].mweather * weathermortadds * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);				
				double dry_mort		= parameter[0].mtrockenheit * weather_list[yearposition_help]->trockenheitsmort * pow((1.0 / pTree->height), 0.5); // Verrechnung der Trockenheit aus Trockenheitsindex

				/// Calculating the mortality rate of the tree 
				// considering the factors of each mortality rate
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
								
				 //cout << endl << " Wachsenmort=" << growth_mort << " &density=" << dens_mort << " &Trocken=" << dry_mort << endl;
				 //cout << "Jung_gmel=" << sapl_mort_gmel << " &Jung_sib=" << sapl_mort_sib << " &weather_gmel=" << weather_mort_gmel << " &weather_sib=" << weather_mort_sib << endl;
						
				
				
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

				if (parameter[0].mortanzeige==true && pTree->height>200.0 && pTree->species==1) 
				{
						printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +FEUER=%4.4f =>%4.2f", 
						pTree->height, 
						pTree->dbasalrel,pTree->dbrustrel,
						parameter[0].mjung*pow(exp((-1.0*pTree->height)+maxhg),parameter[0].jugendmorteinflussexp),
						parameter[0].malt*(agesmort*(10.0*parameter[0].mortbg)), 
						+ (1.0-pow(wachstumrel,parameter[0].relwachstummorteinflussexp)),
						parameter[0].mdensity*heightnkugeleinfluss*pTree->densitywert, 
						parameter[0].mweather*weathermortaddg*pow((1.0/pTree->height),0.5), 
						parameter[0].mtrockenheit*(weather_list[yearposition_help]->trockenheitsmort*pow((1.0/pTree->height),0.5)), 
						Treemortg);
				}

				if (parameter[0].mortanzeige==true && pTree->height>200.0 && pTree->species==2) 
				{
						printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +FEUER=%4.4f =>%4.2f", 
						pTree->height, 
						pTree->dbasalrel,pTree->dbrustrel,
						parameter[0].mjung*pow(exp((-1.0*pTree->height)+maxhs),parameter[0].jugendmorteinflussexp),
						parameter[0].malt*(agesmort*(10.0*parameter[0].mortbg)), 
						+ (1.0-pow(wachstumrel,parameter[0].relwachstummorteinflussexp)),
						parameter[0].mdensity*heightnkugeleinfluss*pTree->densitywert, 
						parameter[0].mweather*weathermortadds*pow((1.0/pTree->height),0.5), 
						parameter[0].mtrockenheit*(weather_list[yearposition_help]->trockenheitsmort*pow((1.0/pTree->height),0.5)), 
						Treemorts);
				}


				// Ermitteln ob der aktuelle Tree stirbt, wenn ja, so wird der entsprechende ...
				// ... Eintrag in der tree_list geloescht
				///if random number < probability, tree dies
				double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
				if (((pTree->species==1) && (zufallsz<Treemortg)) || ((pTree->species==2) && (zufallsz<Treemorts))) 
				{
					//pTree->yr_of_dying=yearposition; Im Moment noch egal
					//pTree->Dbasalliste.clear();
					// http://www.cs.up.ac.za/cs/banguelov/blog/vectorMemLeak.cpp Dort hat jemand das Problem behandelt und eine Loesung vorgeschlagen. 
					//Wenn nur clear aufgerufen wird, so wird der allozierte Speicher nicht!! freigegeben. Daher auch soviel Speicherverbrauch?! 
					//Der Trick ist folgender. Der vector wird mit einem leeren vector geswapt (http://takinginitiative.net/2008/03/19/stl-vector-memory-deallocation-problems-memory-leak/)

					
					
					/* to reduce computation load, delete the following printout section
					//StefanC: Ausgabe der Infos des sterbenden Treees
					double toteinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					if ((pTree->age>10)&(toteinschreibzufall<0.0000001)) 
					{//StefanC: Bei Tod eines Treees Ausgabe
						FILE *dateizeiger;
						string dateiname;

							// Dateinamen zusammensetzen
							char dateinamesuf[10];
							sprintf(dateinamesuf, "%.4d", parameter[0].weatherchoice);
							dateiname="output/datatrees_death" + string(dateinamesuf) + ".csv";
					 
							// Datei versuchen zum Lesen und Schreiben zu oeffnen
							dateizeiger = fopen (dateiname.c_str(), "r+");
							// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
							if (dateizeiger == NULL)
							{
							  dateizeiger = fopen (dateiname.c_str(), "w");
								fprintf(dateizeiger, "yearposition_help;");
								fprintf(dateizeiger, "temp1monatmitteliso;");
								fprintf(dateizeiger, "janisothermrestriktiong;");
								fprintf(dateizeiger, "janisothermrestriktions;");								
								fprintf(dateizeiger, "julisothermrestriktion;");
								fprintf(dateizeiger, "nddrestriktion;");		
								fprintf(dateizeiger, "weatherchoice;");
								fprintf(dateizeiger, "agesmort;");
								fprintf(dateizeiger, "wachstumrel;");
								fprintf(dateizeiger, "growing;");
								fprintf(dateizeiger, "heightnkugeleinfluss;");	
								fprintf(dateizeiger, "height;");
								fprintf(dateizeiger, "age;");
								fprintf(dateizeiger, "species;");
								fprintf(dateizeiger, "mortbg;");	
								fprintf(dateizeiger, "weathermortadd;");	
								fprintf(dateizeiger, "maxh;");	
								fprintf(dateizeiger, "Treemort;");								
								fprintf(dateizeiger, "sapl_mort;");
								fprintf(dateizeiger, "weather_mort;");								
								fprintf(dateizeiger, "age_mort;");
								fprintf(dateizeiger, "growth_mort;");
								fprintf(dateizeiger, "dens_mort;");
								fprintf(dateizeiger, "dry_mort;");
								fprintf(dateizeiger, "zufallsz;");
								fprintf(dateizeiger, "\n");

								if (dateizeiger == NULL)
								{
									fprintf(stderr, "Fehler: Todesfalldatei konnte nicht geoeffnet werden!\n");
									exit(1);
								}
							}

							// Die neuen Informationen werden ans Ende der Datei geschrieben
							fseek(dateizeiger,0,SEEK_END);

							// Datenaufbereiten und in die Datei schreiben
							fprintf(dateizeiger, "%d;", yearposition_help);
							fprintf(dateizeiger, "%4.5f", weather_list[yearposition_help]->temp1monatmitteliso);
							fprintf(dateizeiger, "%4.5f;", weather_list[yearposition_help]->janisothermrestriktiong);
							fprintf(dateizeiger, "%4.5f;", weather_list[yearposition_help]->janisothermrestriktions);
							fprintf(dateizeiger, "%4.5f;", weather_list[yearposition_help]->julisothermrestriktion);
							fprintf(dateizeiger, "%4.5f;", weather_list[yearposition_help]->nddrestriktion);
							fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
							fprintf(dateizeiger, "%4.5f;", agesmort);
							fprintf(dateizeiger, "%4.5f;", wachstumrel);
							if (pTree->growing==true) 
							{
								fprintf(dateizeiger, "growing;");
							}
							else 
							{
								fprintf(dateizeiger, "static;");
							}	
							fprintf(dateizeiger, "%4.5f;", heightnkugeleinfluss);
							fprintf(dateizeiger, "%4.5f;", pTree->height);
							fprintf(dateizeiger, "%d;", pTree->age);
							fprintf(dateizeiger, "%d;", pTree->species);
							fprintf(dateizeiger, "%4.5f;", parameter[0].mortbg);
							if(pTree->species==1)
							{
								fprintf(dateizeiger, "%4.5f;", weathermortaddg);
								fprintf(dateizeiger, "%4.5f;", maxhg);			
								fprintf(dateizeiger, "%4.5f;", Treemortg);							
								fprintf(dateizeiger, "%4.5f;", sapl_mort_gmel);
								fprintf(dateizeiger, "%4.5f;", weather_mort_gmel);
							}
							else
							{
								fprintf(dateizeiger, "%4.5f;", weathermortadds);
								fprintf(dateizeiger, "%4.5f;", maxhs);			
								fprintf(dateizeiger, "%4.5f;", Treemorts);							
								fprintf(dateizeiger, "%4.5f;", sapl_mort_sib);
								fprintf(dateizeiger, "%4.5f;", weather_mort_sib);							
							}
							fprintf(dateizeiger, "%4.5f;", age_mort);
							fprintf(dateizeiger, "%4.5f;", growth_mort);
							fprintf(dateizeiger, "%4.5f;", dens_mort);
							fprintf(dateizeiger, "%4.5f;", dry_mort);
							fprintf(dateizeiger, "%4.5f;", zufallsz);
							fprintf(dateizeiger, "\n");

							fclose (dateizeiger);
					} //StefanC: Ende der Ausgabefunktion fuer tote Treee
					*/
					
					delete pTree;//LEGT yr_of_dying FEST
					pos=tree_list.erase(pos);
				}
				else
				{
					++pos;
				}

			}//growing?
			else
			{
				++pos;
			}

		} // Ende tree_list ablaufen
		
		// Zusaetzliche Ausgabe in Konsole
		if (parameter[0].jahranzeige ==true)
		{
				cout << "tree_list.size() nach Mortalität = " << tree_list.size() << endl;
		}


}



/****************************************************************************************//**
 * \brief calculate tree and seed mortality
 *
 * first create surviving seeds then call TreeMort() 
 *
 *
 *******************************************************************************************/
void Mortalitaet(int treerows, int treecols, struct Parameter *parameter,int Jahr, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)
{
	// vector<int> Vname,cpSNP1,cpSNP2;//moved down into omp-parallel-for part
	// int iran;
	int aktort=0;
	
	for(vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{// START: world loop
		// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Referenz
		// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
		// realisiert ist weiterverfahren werden
		// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
		vector<weather*>& weather_list = *posw;

		// Um auf ein bestimmtes Element in der Welt zuzugreifen muss ein Iterator bis
		// zum entsprechenden Element justiert werden und dann eine tree_list als Referenz
		// erzeugt werden
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;
		
		
		//world_positon_b_copy-dekl.??

		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;

		aktort++;

		clock_t start_time_mortpoll=clock();
		
		//cout << "seed_list.size() vor Mortalität = " << seed_list.size() << endl;
		/// seedmortalität
		for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pseed=(*pos);
			double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
			
			///seed is on ground && random number < probability (0.8)
			if (pseed->imcone==false)
			{
				if (zufallsz<parameter[0].seedbodenmort) 
				{
					delete pseed;
					pos=seed_list.erase(pos);
				}else
				{
					++pos;
				}
			}
			
			///seed in cone && random number < probability (0.8)
			else if (pseed->imcone==true)
			{
				if (zufallsz<parameter[0].seedTreemort) 
				{
					delete pseed;
					pos=seed_list.erase(pos);
				}
				
				else
				{
					++pos;
				}
			}
			
			else
			{	// Sicherheitsabfrage imcone-Variable gesetzt? 
				/* Abfrage ob das Programm beendet oder fortgesetzt werden soll */ 
				signed int abbrechenmortalitaetfehler; 
				printf("\n In der Mortalitaetsfunktion hat ein seed keinen Wert in der Variable pseed->imcone\n"); 
				printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n"); 
				scanf("%d", &abbrechenmortalitaetfehler); if (abbrechenmortalitaetfehler!=1) {printf("LaVeSi wurde nach einem Fehler in der Mortalitaetsfunktion beendet\n\n");exit(1);}
		
				delete pseed;
				pos=seed_list.erase(pos);						
			} 
		
		} 
		
		// Berechnung des aktuellen Ortes in Koordinaten
		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		// Loop for each seed that is to be produced for each tree
	
		// loop around the loop for MULTI-CORE-PROCESSING
		// before run a program parallel set the number of helpers by changing the environment variable
		// ... ... export OMP_NUM_THREADS=4
		// ... or explicitly overwrite the environmental variable by setting the helper number directly
		// ... ... omp_set_num_threads(int); // which is set in the parameter.txt file variable "omp_num_threads"
		// This implementation was formerly (21st April and backwards) named "Testversion 2" - give each thread a local seed_list and splice them together in the end

		// loop with omp through each element of the list
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		#pragma omp parallel default(shared) private(pTree,pseed)
		{ // START: parallel region
			// declare a local seed list to be filled by each thread
			list<seed*> newseed_list;

			#pragma omp for nowait schedule(guided) 
			for(unsigned int pari=0; pari<tree_list.size(); ++pari)
			{// START: main tree loop
				list<Tree*>::iterator posb=tree_list.begin();
				// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
				advance(posb, pari);

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pseed) and container (Vname)
				pTree=(*posb);			
				vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file

				if((parameter[0].ivort==1) & (pari==0))// check the number of used threads
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				
				if(pTree->seednewly_produced>0)
				{//START: tree produces seeds
					// ramdomly determine the number of surving seeds
					int seedlebend=0;
					for(int sna=0; sna < pTree->seednewly_produced; sna++)
					{
						double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
						if(zufallsz>=parameter[0].seedTreemort) 
						{
							++seedlebend;
						}
					}
					
					if(seedlebend>0)
					{// START: if seedlebend>0
						if(parameter[0].pollenvert==1)
						{
							BefrWahrsch(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,Jahr,Vname);//;,cpSNP1,cpSNP2);
						}
				
						// get the characteristics for each survining seed and push these back new to seed_list
						for(int sl=0; sl<seedlebend; sl++)
						{// START: create new seeds
							// create a new seed
							pseed= new seed();
							
							// add information
							pseed->yworldcoo=aktortyworldcoo;
							pseed->xworldcoo=aktortxworldcoo;
							pseed->xcoo=pTree->xcoo;
							pseed->ycoo=pTree->ycoo;
							pseed->namem=pTree->name;
							
							// if chosen, determine the father by pollination out of available (matured) trees
							if((Vname.size()>0)&&(parameter[0].pollenvert==1))
							{
								int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size();
								pseed->namep=Vname[iran];
								//pseed->cpSNP[0]=cpSNP1[iran];
								//pseed->cpSNP[1]=cpSNP2[iran];
								
								//pseed->descent=
								//pseed->pollenfall=
								//pseed->maxgrowth=
							} else
							{
								pseed->namep=0;
							}
							//pseed->cpSNP[0]=0;
							//pseed->cpSNP[1]=0;}
							/*cout<<pseed->namep<<endl;
							cout<<pseed->cpSNP[0]<<endl;
							cout<<pseed->cpSNP[1]<<endl;*/
							

							//pseed->mtSNP[0]=pTree->mtSNP[0];
							//pseed->mtSNP[1]=pTree->mtSNP[1];
							
							pseed->line=pTree->line;
							pseed->generation=pTree->generation+1;	// generation==0 introduced from outside
							pseed->imcone=true;
							pseed->gewicht=1;
							pseed->age=0;
							pseed->species=pTree->species;// species is inherited from the seed source
							pseed->elternheight=pTree->height;

							// add seed to seed_list
							newseed_list.push_back(pseed);
						}// END: create new seeds

						/*if(parameter[0].pollenvert==1)// maybe this is not needed because it is now locally constructed for each tree
						{
							Vname.clear();//cpSNP1.clear();cpSNP2.clear(); //  is this of use? in BefrWahrsch it is cleaned anyway!?
						}
						*/
					}// END: if seedlebend>0
				}// END: tree produces seeds
			}//END: main tree loop
			
			// append all newly created seed from each thread at once to the seed_list
			#pragma omp critical
			{
				seed_list.splice(seed_list.end(), newseed_list);
			}
		} // END: parallel region
			
		clock_t end_time_poll=clock();
		/*!TreeMort(int yearposition_help,vector<weather*> &weather_list,list<Tree*> &tree_list)*/
		TreeMort(yearposition, weather_list, tree_list);
		clock_t end_time_mortpoll=clock();
		
		if(parameter[0].computationtime==1)
		{
			openpoll:
			FILE *fp4;
			fp4=fopen("t_N_poll.txt","a+");
			if(fp4==0){goto openpoll;}
			fprintf(fp4,"%lu;%d;%10.2f;%10.2f\n",
					tree_list.size(),
					parameter[0].ivort, 
					((double) (end_time_poll - start_time_mortpoll))/ CLOCKS_PER_SEC,
					((double) (end_time_mortpoll - end_time_poll))/ CLOCKS_PER_SEC
				);
			fclose(fp4);
		}
	
	
	}// END: world loop

}



