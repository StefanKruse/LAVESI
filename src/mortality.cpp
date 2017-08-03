






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
		// if (parameter[0].jahranzeige ==true) {
				// cout << "tree_list.size() vor Mortalität = " << tree_list.size() << endl;
		// }
		
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
		// if (parameter[0].jahranzeige ==true)
		// {
				// cout << "tree_list.size() nach Mortalität = " << tree_list.size() << endl;
		// }


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

		double start_time_mortpoll=omp_get_wtime();
		
	// https://stackoverflow.com/questions/16777810/c-omp-omp-get-wtime-returning-time-0-00 
	// ... problem time() returns cpu-time not real time use omp_get_wtime()!!
	// printf... with "%.16" for double ini
	
	
	
	/*	// start: ORI CODE*/
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
				signed int abbrechenmortalitaetfehler; 
				printf("\n In der Mortalitaetsfunktion hat ein seed keinen Wert in der Variable pseed->imcone\n"); 
				printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n"); 
				scanf("%d", &abbrechenmortalitaetfehler); if (abbrechenmortalitaetfehler!=1) {printf("LaVeSi wurde nach einem Fehler in der Mortalitaetsfunktion beendet\n\n");exit(1);}
		
				delete pseed;
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
	
	
		### ist so einfach nicht möglich
		# im Moment ist es dadurch sehr langsam, vmtl. wegen des iterieren/advance!
		
	
	SEED MORTALITY PARALLELIZATION NO_2
			++ by hand
			https://stackoverflow.com/questions/8691459/how-do-i-parallelize-a-for-loop-through-a-c-stdlist-using-openmp
			
	SEED MORTALITY PARALLELIZATION NO_3
		based on NO_2 but with a local list to which the data is pushed (or copied??)
		
		including various versions to test the efficiency

	
	*/
		
	/* SEED MORTALITY PARALLELIZATION NO_1 			
		// time usage of program
			//TIME:
		// clock_t sm00; // start for each seed 
			//cumulativ Seconds:
		// clock_t sm01=0; // after advance
		// clock_t sm02=0; // after random number
		// clock_t sm99=0; // after if-else and copy

		// parallelization-1. 
		list<seed*> seed_list_global;

		// loop with omp through each element of the list
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		#pragma omp parallel default(shared) private(pseed)
		{ // START: parallel region
			// declare a local seed list to be filled by each thread
			// parallelization-2. 
			list<seed*> seed_list_local;
			
			#pragma omp for nowait schedule(guided) 
			// for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
			for(unsigned int pari=0; pari<seed_list.size(); ++pari)
			{// START: seed mortality
				// sm00=clock(); // start for each seed
				// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
				list<seed*>::iterator pos = seed_list.begin();
				advance(pos, pari);
				pseed=(*pos);
				
				// sm01+=clock() - sm00; // after advance

				double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
				
				// sm02+=clock() - sm00; // after random number
				
				///seed is on ground && random number < probability (0.8)
				if (pseed->imcone==false)
				{
					if (zufallsz<parameter[0].seedbodenmort) 
					{
						// delete pseed;
						// pos=seed_list.erase(pos);
					} else
					{
						// cout << endl << "-> seed list size before" << seed_list_local.size() << " -+- seed_list original list length " << seed_list.size();
						
						// parallelization-3. copy only surviving
						// is it sufficient to push_back? are the positions conserved from the original list or does this lead to errors?
						seed_list_local.push_back(pseed);
						
						// cout << endl << "-> seed list size after " << seed_list_local.size() << " -+- seed_list original list length " << seed_list.size();
					}
				}
				///seed in cone && random number < probability (0.8)
				else if (pseed->imcone==true)
				{
					if (zufallsz<parameter[0].seedTreemort) 
					{
						// delete pseed;
						// pos=seed_list.erase(pos);
					} else
					{
						// parallelization-3. copy only surviving
						seed_list_local.push_back(pseed);
					}

				}
				else
				{	// check imcone-variable set? 
					printf("\n no value of a seed at pseed->imcone / mortality.cpp\n"); 
					exit(1);
				}
				// clock_t sm99c=clock();
				// sm99+=sm99c - sm00; // after if-else and copy
			}//END: seed mortality
	
			// append all newly created seed from each thread at once to the seed_list
			#pragma omp critical
			{
				seed_list_global.splice(seed_list_global.end(), seed_list_local);
			}
		} // END: parallel region
		// cout << endl << endl << "TIMEs : advance(sm01)=" << sm01 << " : randomn(sm02)=" << sm02 <<  " : end(sm99)=" << sm99 << 	endl << endl;
	
		// parallelization-5.
		// cout << endl << "-> seed_list_global size before" << seed_list_global.size() << " -+- seed_list original list length " << seed_list.size();

		seed_list.swap(seed_list_global);
		// ... must the "seed_list_global" list be deleted or are all destructors called at the end of this scope?
		seed_list_global.clear();
		
		// cout << endl << "-> seed_list_global size after " << seed_list_global.size() << " -+- seed_list original list length " << seed_list.size();

		
	*/

	
	/* SEED MORTALITY PARALLELIZATION NO_2 		

// cout << " -> seed_list.size() = " << seed_list.size() << endl;	
if(parameter[0].ivort==2)
{
// copy list
// seed_list_copy=seed_list;
		list<seed*> seed_list_copy;
		cout << endl;
		cout << "seed_list.size()=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() vor Kopieren=" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// Für die Ausgabe des ersten Treees
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{
			pseed=(*pos);

			/// Alle Daten des seeds in ein neues Objekt in der ..._copy-Liste erzeugen
			pseed_copy= new seed();						// Neuen seed erzeugen
			pseed_copy->xworldcoo=pseed->xworldcoo;			
			pseed_copy->yworldcoo=pseed->yworldcoo;	
			pseed_copy->xcoo=pseed->xcoo;			
			pseed_copy->ycoo=pseed->ycoo;		
			pseed_copy->namem=pseed->namem;			
			pseed_copy->namep=pseed->namep;		
			pseed_copy->line=pseed->line;			
			pseed_copy->generation=pseed->generation;
			pseed_copy->species=pseed->species;
			pseed_copy->imcone=pseed->imcone;		
			pseed_copy->gewicht=pseed->gewicht;	
			pseed_copy->age=pseed->age;			
			pseed_copy->longdispersed=pseed->longdispersed;	
				// new
			pseed_copy->entfernung=pseed->entfernung;	
			pseed_copy->species=pseed->species;
			pseed_copy->elternheight=pseed->elternheight;
			pseed_copy->mtSNP[2]=pseed->mtSNP[2];
			pseed_copy->cpSNP[2]=pseed->cpSNP[2];
			pseed_copy->maxgrowth=pseed->maxgrowth;
			pseed_copy->pollenfall=pseed->pollenfall;
			pseed_copy->descent=pseed->descent;
			pseed_copy->thawing_depthinfluence=pseed->thawing_depthinfluence;
				// new end
			seed_list_copy.push_back(pseed_copy);				// seed in Liste einfuegen
			
				if(counterseed==1)
				{
					cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list_copy.size() nach Kopieren=" << seed_list_copy.size() << endl;
		cout << endl;
		
		
// parallele loop
	// for(int helperi=1; helperi<9; helperi=helperi*2)
	for(int helperi=2; helperi<9; helperi=helperi*2)
	{
	double comptime=0;
	int repeats=100;
	for(int wdh=0; wdh<repeats; wdh++)
	{// arbitr loop to check comp time for general loop
		cout << ".";
	
		// clean list first
		// fill list with buffer-list
			// cout << "seed_list.size()=" << seed_list.size() << endl;
			for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
			{
				pseed=(*pos);
				delete pseed;
				pos=seed_list.erase(pos);
			}	
			// cout << "seed_list.size() nach loeschen=" << seed_list.size() << endl;
			// cout << "seed_list_copy.size() vor Kopieren=" << seed_list_copy.size() << endl;
		
			// int counterseed=1;	// Für die Ausgabe des ersten Treees
			for (list<seed*>::iterator pos = seed_list_copy.begin(); pos != seed_list_copy.end(); )
			{
				pseed=(*pos);

				/// Alle Daten des seeds in ein neues Objekt in der ..._copy-Liste erzeugen
				pseed_copy= new seed();						// Neuen seed erzeugen
				pseed_copy->xworldcoo=pseed->xworldcoo;			
				pseed_copy->yworldcoo=pseed->yworldcoo;	
				pseed_copy->xcoo=pseed->xcoo;			
				pseed_copy->ycoo=pseed->ycoo;		
				pseed_copy->namem=pseed->namem;			
				pseed_copy->namep=pseed->namep;		
				pseed_copy->line=pseed->line;			
				pseed_copy->generation=pseed->generation;
				pseed_copy->species=pseed->species;
				pseed_copy->imcone=pseed->imcone;		
				pseed_copy->gewicht=pseed->gewicht;	
				pseed_copy->age=pseed->age;			
				pseed_copy->longdispersed=pseed->longdispersed;	
					// new
				pseed_copy->entfernung=pseed->entfernung;	
				pseed_copy->species=pseed->species;
				pseed_copy->elternheight=pseed->elternheight;
				pseed_copy->mtSNP[2]=pseed->mtSNP[2];
				pseed_copy->cpSNP[2]=pseed->cpSNP[2];
				pseed_copy->maxgrowth=pseed->maxgrowth;
				pseed_copy->pollenfall=pseed->pollenfall;
				pseed_copy->descent=pseed->descent;
				pseed_copy->thawing_depthinfluence=pseed->thawing_depthinfluence;
					// new end
				seed_list.push_back(pseed_copy);				// seed in Liste einfuegen
				
					// if(counterseed==1)
					// {
						// cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
					// }

				++pos;
				++counterseed;
			}
			
			// cout << "seed_list_copy.size() nach Kopieren=" << seed_list_copy.size() << endl;
			// cout << "seed_list.size() nach copy=" << seed_list.size() << endl;
			// cout << endl;
	
	
	
	double startt = omp_get_wtime();
	
		omp_set_dynamic(0); //disable dynamic teams
		// omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		omp_set_num_threads(helperi); //set the number of helpers
		
		// if(parameter[0].ivort==1)// check the number of used threads
			// cout << endl << endl << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
					
		// seed_list	
		#pragma omp parallel private(pseed)
		{
			int thread_count = omp_get_num_threads();
			int thread_num   = omp_get_thread_num();
			size_t chunk_size= seed_list.size() / thread_count;
			auto begin = seed_list.begin();
			std::advance(begin, thread_num * chunk_size);
			auto end = begin;
			
			// if(thread_num==0)
				// cout << endl << endl << " -- OMP -- set current number of helpers to =" << helperi << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl;
			
			if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
			{
				end = seed_list.end();
				// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
			} else
			{
				std::advance(end, chunk_size);
				// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
			}
			
			#pragma omp barrier
			for(auto it = begin; it != end; )
			{
				// it->process();
				pseed=(*it);
				// cout << pseed->imcone << "  ";
				double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
				///seed is on ground && random number < probability (0.8)
				if (pseed->imcone==false)
				{
					if (zufallsz<parameter[0].seedbodenmort) 
					{
						delete pseed;
						it=seed_list.erase(it);
						// ++it;
					} else
					{
						++it;
					}
				} else if (pseed->imcone==true)
				{
					if (zufallsz<parameter[0].seedTreemort) 
					{
						delete pseed;
						it=seed_list.erase(it);
						// ++it;
					} else
					{
						++it;
					}
				} else
				{	// check imcone-variable set? 
					cout << "->" << pseed->imcone << "<-";
					printf("\n no value of a seed at pseed->imcone / mortality.cpp\n"); 
					exit(1);
				}
			
			}
		}
	comptime+=(double) omp_get_wtime()-startt;
	}
	printf("%d:::%20.20f",helperi,comptime/((double) repeats));
	cout << "         -> seed_list.size() = " << seed_list.size() << endl;	

	}

	exit(1);
}


	*/

	
	/* SEED MORTALITY PARALLELIZATION NO_3 			

// cout << " -> seed_list.size() = " << seed_list.size() << endl;	
if(parameter[0].ivort==2)
{
// copy list
// seed_list_copy=seed_list;
		list<seed*> seed_list_copy;
		cout << endl;
		cout << "seed_list.size()=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() vor Kopieren=" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// Für die Ausgabe des ersten Treees
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{
			pseed=(*pos);

			/// Alle Daten des seeds in ein neues Objekt in der ..._copy-Liste erzeugen
			pseed_copy= new seed();						// Neuen seed erzeugen
			pseed_copy->xworldcoo=pseed->xworldcoo;			
			pseed_copy->yworldcoo=pseed->yworldcoo;	
			pseed_copy->xcoo=pseed->xcoo;			
			pseed_copy->ycoo=pseed->ycoo;		
			pseed_copy->namem=pseed->namem;			
			pseed_copy->namep=pseed->namep;		
			pseed_copy->line=pseed->line;			
			pseed_copy->generation=pseed->generation;
			pseed_copy->species=pseed->species;
			pseed_copy->imcone=pseed->imcone;		
			pseed_copy->gewicht=pseed->gewicht;	
			pseed_copy->age=pseed->age;			
			pseed_copy->longdispersed=pseed->longdispersed;	
				// new
			pseed_copy->entfernung=pseed->entfernung;	
			pseed_copy->species=pseed->species;
			pseed_copy->elternheight=pseed->elternheight;
			pseed_copy->mtSNP[2]=pseed->mtSNP[2];
			pseed_copy->cpSNP[2]=pseed->cpSNP[2];
			pseed_copy->maxgrowth=pseed->maxgrowth;
			pseed_copy->pollenfall=pseed->pollenfall;
			pseed_copy->descent=pseed->descent;
			pseed_copy->thawing_depthinfluence=pseed->thawing_depthinfluence;
				// new end
			seed_list_copy.push_back(pseed_copy);				// seed in Liste einfuegen
			
				if(counterseed==1)
				{
					cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list_copy.size() nach Kopieren=" << seed_list_copy.size() << endl;
		cout << endl;
		
		
// parallele loop
	// for(int helperi=1; helperi<9; helperi=helperi*2)
	for(int helperi=0; helperi<9; )
	{
		double comptime=0;
		int repeats=10;
		for(int wdh=0; wdh<repeats; wdh++)
		{// arbitr loop to check comp time for general loop
			cout << ".";
		
			// clean list first
			// fill list with buffer-list
				// cout << "seed_list.size()=" << seed_list.size() << endl;
				for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
				{
					pseed=(*pos);
					delete pseed;
					pos=seed_list.erase(pos);
				}	
				// cout << "seed_list.size() nach loeschen=" << seed_list.size() << endl;
				// cout << "seed_list_copy.size() vor Kopieren=" << seed_list_copy.size() << endl;
			
				// int counterseed=1;	// Für die Ausgabe des ersten Treees
				for (list<seed*>::iterator pos = seed_list_copy.begin(); pos != seed_list_copy.end(); )
				{
					pseed=(*pos);

					/// Alle Daten des seeds in ein neues Objekt in der ..._copy-Liste erzeugen
					pseed_copy= new seed();						// Neuen seed erzeugen
					pseed_copy->xworldcoo=pseed->xworldcoo;			
					pseed_copy->yworldcoo=pseed->yworldcoo;	
					pseed_copy->xcoo=pseed->xcoo;			
					pseed_copy->ycoo=pseed->ycoo;		
					pseed_copy->namem=pseed->namem;			
					pseed_copy->namep=pseed->namep;		
					pseed_copy->line=pseed->line;			
					pseed_copy->generation=pseed->generation;
					pseed_copy->species=pseed->species;
					pseed_copy->imcone=pseed->imcone;		
					pseed_copy->gewicht=pseed->gewicht;	
					pseed_copy->age=pseed->age;			
					pseed_copy->longdispersed=pseed->longdispersed;	
						// new
					pseed_copy->entfernung=pseed->entfernung;	
					pseed_copy->species=pseed->species;
					pseed_copy->elternheight=pseed->elternheight;
					pseed_copy->mtSNP[2]=pseed->mtSNP[2];
					pseed_copy->cpSNP[2]=pseed->cpSNP[2];
					pseed_copy->maxgrowth=pseed->maxgrowth;
					pseed_copy->pollenfall=pseed->pollenfall;
					pseed_copy->descent=pseed->descent;
					pseed_copy->thawing_depthinfluence=pseed->thawing_depthinfluence;
						// new end
					seed_list.push_back(pseed_copy);				// seed in Liste einfuegen
					
						// if(counterseed==1)
						// {
							// cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
						// }

					++pos;
					++counterseed;
				}
				
				// cout << "seed_list_copy.size() nach Kopieren=" << seed_list_copy.size() << endl;
				// cout << "seed_list.size() nach copy=" << seed_list.size() << endl;
				// cout << endl;
		
		
		
			double startt = omp_get_wtime();
			
			// linear old
			if(helperi==0)
			{ // linear
			// start: ORI CODE
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
						signed int abbrechenmortalitaetfehler; 
						printf("\n In der Mortalitaetsfunktion hat ein seed keinen Wert in der Variable pseed->imcone\n"); 
						printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n"); 
						scanf("%d", &abbrechenmortalitaetfehler); if (abbrechenmortalitaetfehler!=1) {printf("LaVeSi wurde nach einem Fehler in der Mortalitaetsfunktion beendet\n\n");exit(1);}
				
						delete pseed;
						pos=seed_list.erase(pos);						
					}
				}
			// end: ORI CODE
			} else
			{ // parallel
				omp_set_dynamic(0); //disable dynamic teams
				// omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
				omp_set_num_threads(helperi); //set the number of helpers
				
				// if(parameter[0].ivort==1)// check the number of used threads
					// cout << endl << endl << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
							
				// create global seed list for surviving seeds found by all threads
				list<seed*> seed_list_surviving_global;
				
				
				// seed_list	
				#pragma omp parallel private(pseed)
				{
					// initialize the info for each of the thread
					int thread_count = omp_get_num_threads();
					int thread_num   = omp_get_thread_num();
					size_t chunk_size= seed_list.size() / thread_count;
					auto begin = seed_list.begin();
					std::advance(begin, thread_num * chunk_size);
					auto end = begin;
					
					// if(thread_num==0)
						// cout << endl << endl << " -- OMP -- set current number of helpers to =" << helperi << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl;
					
					if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
					{
						end = seed_list.end();
						// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
					} else
					{
						std::advance(end, chunk_size);
						// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
					}
					
					// create a local list for each thread to copy all surviving seed into
					list<seed*> seed_list_surviving_local;

					// wait for all threads to initialize and then proceed
					#pragma omp barrier
					
					for(auto it = begin; it != end; ++it)
					{
						// it->process();
						pseed=(*it);
						// cout << pseed->imcone << "  ";
						double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
						///seed is on ground && random number < probability (0.8)
						if (pseed->imcone==false)
						{
							if (zufallsz<parameter[0].seedbodenmort) 
							{
								// delete pseed;
								// it=seed_list.erase(it);
								// ++it;
								
								seed_list_surviving_local.push_back(pseed);
							} else
							{
								// ++it;
							}
						} else if (pseed->imcone==true)
						{
							if (zufallsz<parameter[0].seedTreemort) 
							{
								// delete pseed;
								// it=seed_list.erase(it);
								// ++it;
								
								seed_list_surviving_local.push_back(pseed);
							} else
							{
								// ++it;
							}
						} else
						{	// check imcone-variable set? 
							cout << "->" << pseed->imcone << "<-";
							printf("\n no value of a seed at pseed->imcone / mortality.cpp\n"); 
							exit(1);
						}
					}
					
					// append all newly created seed from each thread at once to the seed_list_surviving_global
					// vielleicht 
					#pragma omp critical
					{
						seed_list_surviving_global.splice(seed_list_surviving_global.end(), seed_list_surviving_local);
					}
					
				}
				// if(wdh<=3)cout << "seed_list_surviving_global=" << seed_list_surviving_global.size() << " +++ seed_list=" << seed_list.size();
				
				seed_list.swap(seed_list_surviving_global);
					
				// if(wdh<=3)cout << "seed_list(after swap)=" << seed_list.size() << endl;
			}
			
			comptime+=(double) omp_get_wtime()-startt;
		}
		
		printf("%d:::%20.20f",helperi,comptime/((double) repeats));
		cout << "         -> seed_list.size() = " << seed_list.size() << endl;	

		if(helperi==0)
			helperi=1;
		else
			helperi=helperi*2;
	}

	exit(1);
}

*/

	
	
	
	
		double end_time_seedsuviving=omp_get_wtime();

		
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
	/*if(parameter[0].pollenvert==0)
	{ // only one kernel specified by parameter[0].omp_num_threads
// start ORI


		// loop with omp through each element of the list
		// omp_set_dynamic(0); //disable dynamic teams
		// omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
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
			  double  p=0.0,kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].GregoryC;
			  double  m=parameter[0].Gregorym;
			  
///
			  	vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				vector<double> Vthdpth;
///
		/// #####################################
		/// #####################################
		for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb)
		{
			pTree=(*posb);
			// vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
			// vector<double> Vthdpth;
				
			if (pTree->seednewly_produced>0)
			{
				// gehe durch Anzahl der seed pro Tree und erwürfel Tod/Leben
				int seedlebend=0;
				for(int sna=0; sna < pTree->seednewly_produced; sna++)
				{
					double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					if (zufallsz>=parameter[0].seedTreemort) 
					{
						++seedlebend;
					}
				}
				
				if(seedlebend>0)
				{
						if( (parameter[0].pollenvert==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollenvert==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							BefrWahrsch(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,Jahr,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth);//;,cpSNP1,cpSNP2);
						}

					for (int sl=0; sl<seedlebend; sl++)
					{ // Neuen seed erstellen Beginn
						pseed= new seed();			// 1. Neuen seed erzeugen
						pseed->yworldcoo=aktortyworldcoo;	// 2. Werte dem seed zuweisen
						pseed->xworldcoo=aktortxworldcoo;
						pseed->xcoo=pTree->xcoo;
						pseed->ycoo=pTree->ycoo;
						pseed->namem=pTree->name;
						
						// if chosen, determine the father by pollination out of available (matured) trees
						if((Vname.size()>0) && (parameter[0].pollenvert==1 || parameter[0].pollenvert==9))
							{
								int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
								pseed->namep=Vname.at(iran);
								
								pseed->thawing_depthinfluence=100;////Vthdpth.at(iran);
								
								//cout<<"samenproduktion:"<<pseed->thawing_depthinfluence<<endl;
								//pseed->cpSNP[0]=cpSNP1[iran];
								//pseed->cpSNP[1]=cpSNP2[iran];
								
								//pseed->descent=
								//pseed->pollenfall=
								//pseed->maxgrowth=
							} else
							{
								pseed->namep=0;
								
								pseed->thawing_depthinfluence=100;
							}
						//pseed->cpSNP[0]=0;
						//pseed->cpSNP[1]=0;}
						//cout<<pseed->namep<<endl;
						//cout<<pseed->cpSNP[0]<<endl;
						//cout<<pseed->cpSNP[1]<<endl;
						

						//pseed->mtSNP[0]=pTree->mtSNP[0];
						//pseed->mtSNP[1]=pTree->mtSNP[1];
						
						pseed->line=pTree->line;
						pseed->generation=pTree->generation+1;	// Generation=0 ist von außen eingebracht
						pseed->imcone=true;
						pseed->gewicht=1;
						pseed->age=0;
						pseed->species=pTree->species;//MutterTreespezies
						pseed->elternheight=pTree->height;
						
						seed_list.push_back(pseed);// 3. seed in Liste einfuegen
					} // Neuen seed erstellen Ende
				
				// Vname.clear();//cpSNP1.clear();cpSNP2.clear();
				}// END: if seedlebend>0

			} // seed wurden erstellt Ende

		}//Ende HauptTreeschleife

// end ORI
	} else
	{
	*/

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
			  // vector<int> SNP1,SNP2;
			  double  p=0.0,kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].GregoryC;
			  double  m=parameter[0].Gregorym;
			  
///
			  	vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				vector<double> Vthdpth;
///
		/// #####################################
		/// #####################################
		#pragma omp parallel default(shared) private(pTree,pseed,       pTree_copy,    richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
		{ // START: parallel region
			// declare a local seed list to be filled by each thread
			list<seed*> newseed_list;
				// timers
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;// from here only surviving seeds
				double timer_eachtree_seedadd=0;// from here only surviving seeds
				double timer_eachtree_total=0;// from here only surviving seeds
			
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

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pseed) and container (Vname)
				pTree=(*posb);			
				// vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				// vector<double> Vthdpth;
				
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
						if(zufallsz>=parameter[0].seedTreemort) 
						{
							++seedlebend;
						}
					}
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;

					
					if(seedlebend>0)
					{// START: if seedlebend>0
						if( (parameter[0].pollenvert==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollenvert==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							BefrWahrsch(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,Jahr,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth);//;,cpSNP1,cpSNP2);
						}
				
						// get the characteristics for each surviving seed and push these back new to seed_list
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
							if((Vname.size()>0) && (parameter[0].pollenvert==1 || parameter[0].pollenvert==9))
							{
								int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
								pseed->namep=Vname.at(iran);
								pseed->thawing_depthinfluence=100;////Vthdpth.at(iran);
								
								//cout<<"samenproduktion:"<<pseed->thawing_depthinfluence<<endl;
								//pseed->cpSNP[0]=cpSNP1[iran];
								//pseed->cpSNP[1]=cpSNP2[iran];
								
								//pseed->descent=
								//pseed->pollenfall=
								//pseed->maxgrowth=
							} else
							{
								pseed->namep=0;
								pseed->thawing_depthinfluence=100;
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
		
		
				if((parameter[0].ivort==1))// check the number of used threads
				{
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				}
				
				
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
			  double  p=0.0,kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].GregoryC;
			  double  m=parameter[0].Gregorym;
			  
///
			  	vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				vector<double> Vthdpth;
///
		/// #####################################
		/// #####################################
		#pragma omp parallel default(shared) private(pTree,pseed,       pTree_copy,    richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
		{ // START: parallel region
		

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
			} else
			{
				std::advance(end, chunk_size);
				// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
			}
					
					
					
					
			// declare a local seed list to be filled by each thread
			list<seed*> newseed_list;
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
			
			// #pragma omp for nowait schedule(guided) 
			// for(unsigned int pari=0; pari<tree_list.size(); ++pari)
			// {// START: main tree loop
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for later calculating mean of computation times
				
				// list<Tree*>::iterator posb=tree_list.begin();
				// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
				// advance(posb, pari);
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pseed) and container (Vname)
				// pTree=(*posb);			
				pTree=(*it);			
				// vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				// vector<double> Vthdpth;
				
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;

				// if((parameter[0].ivort==1) & (pari==0))// check the number of used threads
					// cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				
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
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;

					
					if(seedlebend>0)
					{// START: if seedlebend>0
						if( (parameter[0].pollenvert==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollenvert==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							BefrWahrsch(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,Jahr,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth);//;,cpSNP1,cpSNP2);
						}
				
						// get the characteristics for each surviving seed and push these back new to seed_list
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
							if((Vname.size()>0) && (parameter[0].pollenvert==1 || parameter[0].pollenvert==9))
							{
								int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
								pseed->namep=Vname.at(iran);
								pseed->thawing_depthinfluence=100;////Vthdpth.at(iran);
								
								//cout<<"samenproduktion:"<<pseed->thawing_depthinfluence<<endl;
								//pseed->cpSNP[0]=cpSNP1[iran];
								//pseed->cpSNP[1]=cpSNP2[iran];
								
								//pseed->descent=
								//pseed->pollenfall=
								//pseed->maxgrowth=
							} else
							{
								pseed->namep=0;
								pseed->thawing_depthinfluence=100;
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
		// directly modified as the parralelization is slowing the computations
		
		
				if((parameter[0].ivort==1))// check the number of used threads
				{
					cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				}
				
				
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
			  double  p=0.0,kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			  double  I0kappa=0.0;
			  double pe=0.01;
			  double  C=parameter[0].GregoryC;
			  double  m=parameter[0].Gregorym;
			  
///
			  	vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				vector<double> Vthdpth;
///
		/// #####################################
		/// #####################################
		
						
				// set end to last tree with produced seeds
				list<Tree*>::iterator lasttreewithseeds_iter=tree_list.begin();
				int lasttreewithseeds_pos=0;
				int treeiter=0;
				for(list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb)
				{
					pTree=(*posb);
					
					treeiter=treeiter+1;
					
					// cout << treeiter << "/ ";
					
					if(pTree->seednewly_produced>0)
					{
						lasttreewithseeds_pos=treeiter;
					}
				}
				advance(lasttreewithseeds_iter, lasttreewithseeds_pos);
				cout << endl << "---- lasttreewithseeds_pos=" << lasttreewithseeds_pos << " ==> tree_list.size()=" << tree_list.size() << endl << endl;
				
				// if(lasttreewithseeds_pos>10)
					// exit(1);
				
				
				
				
				
		#pragma omp parallel default(shared) private(pTree,pseed,       pTree_copy,    richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth)
		{// START: parallel region
		

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
				// end = tree_list.end();
				end = lasttreewithseeds_iter;
				// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
			} else
			{
				std::advance(end, chunk_size);
				// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
			}
					
					
					
					
			// declare a local seed list to be filled by each thread
			list<seed*> newseed_list;
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
			// for(unsigned int pari=0; pari<tree_list.size(); ++pari)
			// {// START: main tree loop
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for later calculating mean of computation times
				
				// list<Tree*>::iterator posb=tree_list.begin();
				// since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
				// advance(posb, pari);
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;

				// to test the functionality of mutli-cores test to define only local pointers (pTree+pseed) and container (Vname)
				// pTree=(*posb);			
				pTree=(*it);			
				// vector<int> Vname;//,cpSNP1,cpSNP2; // moved here from the top of this file
				// vector<double> Vthdpth;
				
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;

				// if((parameter[0].ivort==1) & (pari==0))// check the number of used threads
					// cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
				
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
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;

					
					if(seedlebend>0)
					{// START: if seedlebend>0
						double start_timer_tresedliv=omp_get_wtime();	
						if( (parameter[0].pollenvert==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollenvert==9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
						{
							BefrWahrsch(pTree->xcoo,pTree->ycoo,&parameter[0],world_positon_b,Jahr,        
												richtung,geschwindigkeit,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m,       
											Vname,Vthdpth);//;,cpSNP1,cpSNP2);
						}
						double end_timer_tresedliv=omp_get_wtime();	
						timer_tresedliv+=end_timer_tresedliv-start_timer_tresedliv;
						// get the characteristics for each surviving seed and push these back new to seed_list
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
							if((Vname.size()>0) && (parameter[0].pollenvert==1 || parameter[0].pollenvert==9))
							{
								int iran=(int) rand()/(RAND_MAX+1.0)*Vname.size()-1;
								pseed->namep=Vname.at(iran);
								pseed->thawing_depthinfluence=100;////Vthdpth.at(iran);
								
								//cout<<"samenproduktion:"<<pseed->thawing_depthinfluence<<endl;
								//pseed->cpSNP[0]=cpSNP1[iran];
								//pseed->cpSNP[1]=cpSNP2[iran];
								
								//pseed->descent=
								//pseed->pollenfall=
								//pseed->maxgrowth=
							} else
							{
								pseed->namep=0;
								pseed->thawing_depthinfluence=100;
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
						double end_timer_createseeds=omp_get_wtime();	
						timer_createseeds+=end_timer_createseeds-end_timer_tresedliv;

						/*if(parameter[0].pollenvert==1)// maybe this is not needed because it is now locally constructed for each tree
						{
							Vname.clear();//cpSNP1.clear();cpSNP2.clear(); //  is this of use? in BefrWahrsch it is cleaned anyway!?
						}
						*/
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

		

	
}// OMP==3
		
		
		// neue Ausgabe um zu beschleunigen
		// ... seed list: only if age==0 &&namep!=0
		// ... Ivort/X/Y/
		if( ( (parameter[0].pollenvert==1 && Jahr>1978 && Jahr<2013 && parameter[0].einschwingen==false && parameter[0].ivort>1045) || (parameter[0].pollenvert==9) ) && (parameter[0].ausgabemodus!=9))//ivort 1045 bei 1000yrspinup and 80yrsim is 1979:2013
		{
						//print data in the most probable cases:
						char output[50];

						FILE *fdir;
						sprintf(output,"output/windgen_YR%.4d_REP%.3d.txt",Jahr,parameter[0].wiederholung);
						
						fdir=fopen(output,"r+");
						
						if(fdir==NULL){
						fdir=fopen(output,"w+");
						// fprintf(fdir,"IVORT \t distance \t rel_angle \t windspd \t winddir\n");
						fprintf(fdir,"IVORT \t X0 \t Y0 \t namep  \t namem \n");
						}
						
						fseek(fdir,0,SEEK_END);

						// # print data
								for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
								{
									pseed=(*pos);
									
									if(pseed->age==0 && pseed->namep!=0)
									{
										fprintf(fdir,"%d \t %lf \t %lf \t %d \t %d \n",parameter[0].ivort,
										pseed->xcoo,pseed->ycoo,pseed->namep,pseed->namem);
									}
								}
								
								
						fclose(fdir);

		}// file output
			
			
			
		// write timers to file
		// cout << endl;
		// cout << timer_eachtree_advance_all << endl;
		// cout << timer_eachtree_vectini_all << endl;
		// cout << timer_eachtree_seedsurv_all << endl;
		// cout << timer_eachtree_seedadd_all << endl;
		// cout << timer_eachtree_total_all << endl;
		// cout << endl;
		// printf("%10.20f\n",timer_eachtree_advance_all);
		// printf("%10.20f\n",timer_eachtree_vectini_all);
		// printf("%10.20f\n",timer_eachtree_seedsurv_all);
		// printf("%10.20f\n",timer_eachtree_seedadd_all);
		// printf("%10.20f\n",timer_eachtree_total_all);
	//}// end if omp choice
		
		
			
		double end_time_poll=omp_get_wtime();
		/*!TreeMort(int yearposition_help,vector<weather*> &weather_list,list<Tree*> &tree_list)*/
		TreeMort(yearposition, weather_list, tree_list);
		double end_time_mortpoll=omp_get_wtime();
		
		if(parameter[0].computationtime==1)
		{
			openpoll:
			FILE *fp4;
			fp4=fopen("t_N_poll.txt","a+");
			if(fp4==0){goto openpoll;}
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
			
			/* script to analyse computation times in R
			
				# read output data of general computation times
				dain=read.table(paste0("M:/Documents/Programmierung/git/LAVESI_wind/LAVESI/","/t_N_mort.txt"), header=FALSE, sep=";", dec=".")
				names(dain)=c("N_tree", "time", "mort", "kartenup", "wachstum", "seeddisp", "seedprod", "treedistribu", "etablier", "fire", "output", "mortality", "ageing", "all")
				str(dain)
				
				# read specifically mortality/pollination computation times
				dain_p=read.table(paste0("M:/Documents/Programmierung/git/LAVESI_wind/LAVESI/","/t_N_poll.txt"), header=FALSE, sep=";", dec=".")
				names(dain_p)=c("time", "N_seed","N_tree", "pollseedmort", "treemort", "seedmort", "advanc","vecti","seedsurv","seedadd","eachtree", "BefrW", "SeedCreate")
				str(dain_p)
				summary(dain_p)
				#names(dain_p)=c("N_tree", "time", "pollseedmort", "treemort")
				# .. redo and calc when all three output variables are included
								dev.new();pie(apply(dain_p[,c("advanc","vecti","seedsurv","BefrW", "SeedCreate")], 2, function(x)sum(na.omit(x))))
								
				dain=merge(dain, dain_p, sort=FALSE)
				str(dain)
				
				# subset/postprocess
					damat=dain[c(4:11,13, 16:17)]
					rm=apply(damat,1,function(x)sum(na.omit(x)))
					for(rowi in 1:dim(damat)[1])
					{
						damat[rowi,]=damat[rowi,]/rm[rowi]
					}
					clbp=rainbow(s=0.5,n=dim(damat)[2])
					# add nas for missing fill up to 100 yrs
					if(dim(damat)[1]<100)
					{
						dfiad=as.data.frame(matrix(NA, nrow=100-dim(damat)[1], ncol=dim(damat)[2]))
						names(dfiad)=names(damat)
						damat=rbind(damat,dfiad)
					}
		
		
				# plot
				dev.new(height=400*1,width=1000)
				par(mar=c(4,0,1,0))
				layout(matrix(c(1:(2*1)), ncol=2, nrow=1, byrow=TRUE), width=c(1,0.2), height=1)
				layout.show(2)

					par(mar=c(4,0,1,0))
						barplot(t(as.matrix(damat)), main=paste0(" -> mean time for one year (sim years=", max(dain$time),"): ",round(mean(rm),0)," sec"), border=NA, yaxt="n", col=clbp, axes=FALSE)#, names.arg=rep(NA,100))
							# overlay full time for computation
							par(new=TRUE)
							with(dain, plot(all~time, type="l", lwd=2, xaxt="n", yaxt="n", ylab="", xlab="", bty="n", xlim=c(0,100)))
							# axis(2,line=-2)
							# with(dain, points(rm~time, type="l", col="red", lty=2, lwd=2, xaxt="n", yaxt="n", ylab="", xlab="", bty="n"))

					par(mar=c(8,0,1,3))
						barplot(apply(damat[1:dim(dain[4:13])[1],],2,mean), border=NA, yaxt="n", las=2, col=clbp)
						# pie(apply(damat,2,mean), border=NA, yaxt="n", las=2, col=clbp)
						axis(side=4,las=1)
				
				
				
			*/
		}
	
	
	}// END: world loop

}



