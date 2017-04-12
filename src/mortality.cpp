﻿/****************************************************************************************//**
 * \brief calculate tree mortality
 *
 * depends on abiotic factors (temperature and number of days with temperatures above 20 degrees)
 * and on biotic factors
 *
 *
 *******************************************************************************************/
 
 
 
 
 
 #ifndef _distribution_H_///TODO: DELETE THIS!
  #define _distribution_H_
  #include "distribution.cpp"
  #endif
  
  
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
				if (pTree->age>parameter[0].hoechstage) 
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
					
					// if (pTree->coneheight!=99999)
					//{  
			
						///LIST OUTPUT NOW!
							/*
							if(parameter[0].pollenvert==2){
							
							FILE *dateizeiger;
							string dateiname;

							// Dateinamen zusammensetzen
							char dateinamesuf[10];
							sprintf(dateinamesuf, "%.4d", parameter[0].windsource);
							dateiname="output/data_genealogie_" + string(dateinamesuf) + ".csv";
					 
							// Datei versuchen zum Lesen und Schreiben zu oeffnen
							dateizeiger = fopen (dateiname.c_str(), "r+");
							// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
							if (dateizeiger == NULL)
							{
							  dateizeiger = fopen (dateiname.c_str(), "w");
								fprintf(dateizeiger, "name;");
								fprintf(dateizeiger, "maturationyear;")
								fprintf(dateizeiger, "dyingyear;");
								fprintf(dateizeiger, "NameM;");
								//fprintf(dateizeiger, "NameP");
								fprintf(dateizeiger, "xcoo;");
								fprintf(dateizeiger, "ycoo;");
								fprintf(dateizeiger, "art;");
								fprintf(dateizeiger, "\n");

								if (dateizeiger == NULL)
								{
									fprintf(stderr, "Error, could not open genealogy file\n");
									exit(1);
								}
							}

							// Die neuen Informationen werden ans Ende der Datei geschrieben
							fseek(dateizeiger,0,SEEK_END);

							// Datenaufbereiten und in die Datei schreiben
							fprintf(dateizeiger, "%d;", pTree->name);
							fprintf(dateizeiger, "%d;",0);
							fprintf(dateizeiger, "%d;", jahr);
							fprintf(dateizeiger, "%d;", pTree->namem);							//fprintf(dateizeiger, "%d;", pTree->namep);//nee
							fprintf(dateizeiger, "%4.5f;", pTree->xcoo);
							fprintf(dateizeiger, "%4.5f;", pTree->ycoo);	
							fprintf(dateizeiger, "%d;", pseed->species);
							fprintf(dateizeiger, "\n");

							fclose (dateizeiger);
							}
					}
							*/

					//vector<double>().swap(pTree->Dbrustliste);
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
 * first call TreeMort() then calculate seeds
 *
 *
 *******************************************************************************************/
 

 
void Mortalitaet(int treerows, int treecols, struct Parameter *parameter,int Jahr, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)
{
	/*
	signed int mortabbruch;
	// Abfrage ob das Programm beendet oder fortgesetzt werden soll
	printf("\n Beginn der Mortalitaet. Weiter mit 1, beenden mit irgendeiner Eingabe\n");
	scanf("%d", &mortabbruch);
	if (mortabbruch!=1) 
	{
		printf("LaVeSi wurde beendet\n\n"); 
		exit(0);
	}
	*/
	
	vector<int> Vname,cpSNP1,cpSNP2;
	int iran;
	int aktort=0;
	
	
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{
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

		//cout << "seed_list.size() vor Mortalität = " << seed_list.size() << endl;
		/// seedmortalität
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
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
		//


// loop around the loop for multi-core-processing
// before run a program parallel set the number of helpers by 
// ... export OMP_NUM_THREADS=4
// ... or explicitly overwrite the environmental variable by setting the helper number directly
// // --- some code to test the speed-up
// // --- 
if(parameter[0].ivort==1)
{
	
	// test with 1/2/4/8/16 helpers the speedup
	cout << " OMP current number of helpers=" << omp_get_num_procs() << endl;
	cout << " OMP current number of helpers=" << omp_get_num_threads() << endl << endl;
	

	omp_set_dynamic(0); //disable dynamic teams
	for(int helpernumi=1; helpernumi<3; helpernumi=helpernumi*2)
	{
		// record time before parallel execution
		double start_time=omp_get_wtime();
		
		omp_set_num_threads(helpernumi);
		#pragma omp parallel
		{
			#pragma omp for
			for(int i=0; i<100000000; ++i)
			{
				sqrt(sqrt(i^i));
			}
		}
		
		// print out the time used for execution
		double end_time=omp_get_wtime();
		cout << " -- OMP helper(N=" << helpernumi << " -> " << end_time-start_time << endl;
		
	}


	if(false)
	{// start if seed_list trial
	// try to for-loop parallel over tree_list
	// ... for each tree add 1000 new seeds to seed_list
	// ==> worked fine but with multiple helpers it was slower than with only one - probably because of to simple algorithms and the bottleneck that only one helper can access at a certain time the seed_list-object
		omp_set_dynamic(0); //disable dynamic teams
		for(int helpernumi=1; helpernumi<3; helpernumi=helpernumi*2)
		{
			// record time before parallel execution
			double start_time=omp_get_wtime();
		
			omp_set_num_threads(helpernumi);
			//base loop over different number of helpers

			cout << " - OMP - before -- seed_list_N=" << seed_list.size() << " -- tree_list_N=" << tree_list.size() << endl;
			#pragma omp parallel
			{//pragma parallel
			#pragma omp single//for task
			for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb)
			{
			#pragma omp task firstprivate(posb)//should work with iterators by task preset
				pTree=(*posb);

				for(int iii=0; iii<2; ++iii)
				{
					pseed= new seed();
					pseed->namem=pTree->name;

					// add some part of code that needs time to compute
					for(int i=0; i<100000000; ++i)
					{
						sqrt(sqrt(i^i));
					}

					// to guarantee that each process is accessing the seed list not simultaneously define it as critical
					#pragma omp critical(seed_list)
					{
						seed_list.push_back(pseed);// 3. add seed to seed_list
					}
				}
			}																																																																										
			}//end pragma parallel
			cout << " - OMP - after -- seed_list_N=" << seed_list.size() << " -- tree_list_N=" << tree_list.size()  << endl;

			// print out the time used for execution
			double end_time=omp_get_wtime();
			cout << " -- OMP/list helper(N=" << helpernumi << " -> " << end_time-start_time << endl;
		
		}// end for helpernumber
	}// end if seed_list trial


	///trial 22
	if(false)
	{// start if seed_list trial
	// try to for-loop parallel over tree_list
	// ... for each tree add 1000 new seeds to seed_list
	// ==> worked fine but with multiple helpers it was slower than with only one - probably because of to simple algorithms and the bottleneck that only one helper can access at a certain time the seed_list-object
		omp_set_dynamic(0); //disable dynamic teams
		for(int helpernumi=1; helpernumi<3; helpernumi=helpernumi*2)
		{
			// record time before parallel execution
			double start_time=omp_get_wtime();
		
			omp_set_num_threads(helpernumi);
			//base loop over different number of helpers

			#pragma omp parallel default(shared) private(pTree,pseed)
			#pragma omp for schedule(guided)
			//for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb)
			for (int pari=0; pari< tree_list.size(); ++pari)
			{
				list<Tree*>::iterator posb = tree_list.begin();
				advance(posb, pari);
				pTree=(*posb);

				for(int iii=0; iii<2; ++iii)
				{
					pseed= new seed();
					pseed->namem=pTree->name;

					// add some part of code that needs time to compute
					for(int i=0; i<100000000; ++i)
					{
						sqrt(sqrt(i^i));
					}

					// to guarantee that each process is accessing the seed list not simultaneously define it as critical
					#pragma omp critical(seed_list)
					{
						seed_list.push_back(pseed);// 3. add seed to seed_list
					}
				}
			}																																																																										

			// print out the time used for execution
			double end_time=omp_get_wtime();
			cout << " -- OMP/list helper(N=" << helpernumi << " -> " << end_time-start_time << endl;
		
		}// end for helpernumber
	}// end if seed_list trial



	// just for testing the OMP-helpers
	//cout << " exiting !" << endl;
	//exit(1);
}// end if ivort==1


		// loop with omp through each element of the list
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		#pragma omp parallel default(shared) private(pTree,pseed,Vname)
		#pragma omp for schedule(guided)
		for (unsigned int pari=0; pari< tree_list.size(); ++pari)
		{
			list<Tree*>::iterator posb = tree_list.begin();
			// since the iterator must be an int for omp, the iterator has to be 
			// ... constructed for each tree instance and advanced to the correct position
			advance(posb, pari);

			pTree=(*posb);			

			if(parameter[0].ivort==1)cout << " OMP set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << endl << endl;
			
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
				
				//if((parameter[0].einschwingen==false)&&(seedlebend>0)&& parameter[0].pollenvert==1){
				if((seedlebend>0)&& parameter[0].pollenvert==1)
				{
					BefrWahrsch(pTree->xcoo,pTree->ycoo,world_positon_b,Jahr,Vname);//;,cpSNP1,cpSNP2);
				}

				
				// gehe für Variable durch und erfinde seed		
				// seed wurden erstellt Beginn
				
				
				for (int sl=0; sl<seedlebend; sl++)
				{ // Neuen seed erstellen Beginn
					pseed= new seed();			// 1. Neuen seed erzeugen
					pseed->yworldcoo=aktortyworldcoo;	// 2. Werte dem seed zuweisen
					pseed->xworldcoo=aktortxworldcoo;
					pseed->xcoo=pTree->xcoo;
					pseed->ycoo=pTree->ycoo;
					pseed->namem=pTree->name;
					
					if((Vname.size()>0)&&(parameter[0].pollenvert==1)){
					iran=(int) rand()/(RAND_MAX+1.0)*Vname.size();
					pseed->namep	=Vname[iran];
					//pseed->cpSNP[0]=cpSNP1[iran];
					//pseed->cpSNP[1]=cpSNP2[iran];
					
					//pseed->descent=
					//pseed->pollenfall=
					//pseed->maxgrowth=
					
					}else{pseed->namep=0;}
					//pseed->cpSNP[0]=0;
					//pseed->cpSNP[1]=0;}
					/*cout<<pseed->namep<<endl;
					cout<<pseed->cpSNP[0]<<endl;
					cout<<pseed->cpSNP[1]<<endl;*/
					

					//pseed->mtSNP[0]=pTree->mtSNP[0];
					//pseed->mtSNP[1]=pTree->mtSNP[1];
					
					pseed->line=pTree->line;
					pseed->generation=pTree->generation+1;	// Generation=0 ist von außen eingebracht
					pseed->imcone=true;
					pseed->gewicht=1;
					pseed->age=0;
					pseed->species=pTree->species;//MutterTreespezies
					pseed->elternheight=pTree->height;


					// to guarantee that each process is accessing the seed list not simultaneously define it as critical
					#pragma omp critical(seed_list)
					{
						seed_list.push_back(pseed);// 3. add seed to seed_list
					}


				} // Neuen seed erstellen Ende

				Vname.clear();//cpSNP1.clear();cpSNP2.clear();
			} // seed wurden erstellt Ende
		}//Ende HauptTreeschleife
		
		/*!TreeMort(int yearposition_help,vector<weather*> &weather_list,list<Tree*> &tree_list)*/
		TreeMort(yearposition, weather_list, tree_list);
		
		//cout << "seed_list.size() nach Mortalität = " << seed_list.size() << endl;
	
	}//Ende weather_listnschleife
}



