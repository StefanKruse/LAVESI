/****************************************************************************************//**
 * \brief calculate dispersal distance dependent on dispersal mode (0-4)
 *
 * 0=zufaellig, 1=exp, 2=fat tailed, 3=gaussian und 4=gaussian+fat tailed combined
 *
 *******************************************************************************************/
/*double getEntfernung(double ratiorn_help)	
{

	double entf_help;
	if (parameter[0].dispersalmode==0)
	{ // zufaellig in bestimmter Entfernung
		entf_help= ((ratiorn_help-1)*100.0)/(0.05-1);
	}
	else if (parameter[0].dispersalmode==1)
	{ // neg. exponential
		entf_help= parameter[0].distanceratio * ((log(ratiorn_help)/(-0.2))/0.16);
	}
	else if (parameter[0].dispersalmode==2)
	{ // fat tailed/power law
		double fatalpha=0.5;
		entf_help= parameter[0].distanceratio *  pow(ratiorn_help, (-1*(1+fatalpha)) );
	}
	else if (parameter[0].dispersalmode==3)
	{ // gaussian
		double gaussweite=40, gaussmaxh=1, gaussposcenter=0;
		entf_help= parameter[0].distanceratio *  sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter;
	}
	else if (parameter[0].dispersalmode==4 || parameter[0].dispersalmode==5)
	{ // gaussian combined with fat tailed
		double gaussfatratio=0.5;
		double gaussweite=20, gaussmaxh=1, gaussposcenter=0;	//gaussweite variieren??
																//oder unten 4500m?
		double fatalpha=0.5;
		double fatalpha=0.5;
		entf_help= parameter[0].distanceratio * 
		( 0.5*( gaussfatratio*(sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter) +
		(1/gaussfatratio)*(pow(ratiorn_help, (-1*(1+fatalpha)) )) ) );
		
		//
	}
	else 
	{
		printf("LaVeSi wurde beendet\n\n");
		printf("... Grund: choice der Ausbreitung (= %d) ausserhalb der verfuegbaren Modi!\n", parameter[0].dispersalmode);
		exit(1);
	}

	return(entf_help);


}*/


void Seedoutput(int aktort, double entfernung, float richtung, int neueweltcoo) 
{
	//StefanC: Ausgabe der LDSD-seed
	// Code-Bsp. hier http://www2.informatik.uni-halle.de/lehre/c/c_fopen.html
	FILE *dateizeiger;
	string dateiname;

		// Dateinamen zusammensetzen
		dateiname="output/dataseed_LDSD.csv";
 
		// Datei versuchen zum Lesen und Schreiben zu oeffnen
		dateizeiger = fopen(dateiname.c_str(), "r+");
			// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
			if (dateizeiger == NULL)
			{
			  dateizeiger = fopen(dateiname.c_str(), "w");
				fprintf(dateizeiger, "ivort;");
				fprintf(dateizeiger, "aktort;");
				fprintf(dateizeiger, "entfernung;");
				fprintf(dateizeiger, "richtung;");
				fprintf(dateizeiger, "neueweltcoo;");			
				fprintf(dateizeiger, "\n");

				if (dateizeiger == NULL)
				{
					fprintf(stderr, "Fehler: LDSD-Datei konnte nicht geoeffnet werden!\n");
					exit(1);
				}
			}

			// Die neuen Informationen werden ans Ende der Datei geschrieben
			fseek(dateizeiger,0,SEEK_END);

			// Datenaufbereiten und in die Datei schreiben
			fprintf(dateizeiger, "%d;", parameter[0].ivort);
			fprintf(dateizeiger, "%d;", aktort);
			fprintf(dateizeiger, "%4.5f;", entfernung);
			fprintf(dateizeiger, "%4.5f;", richtung);
			fprintf(dateizeiger, "%d;", neueweltcoo);		
			fprintf(dateizeiger, "\n");

		fclose(dateizeiger);
}



/****************************************************************************************//**
 * \brief calculate seed dispersal
 *
 * 
 *
 *******************************************************************************************/


//#ifndef _distribution_H_
//#define _distribution_H_
// #include "distribution.cpp"
//#endif

 void seedausbreitung(int treerows, int treecols,int jahr, int yearposition, struct Parameter *parameter, vector<list<seed*> > &world_seed_list)
{
		
	int aktort=0;
    double iquer=0,jquer=0;

	/*
	list<seed*> LDDseed_list; // Liste zum Zwischenspeichern der LDD ausgebreiteten seed
	*/
			


	
	
	

			
	///Loop around all Seed Lists
	for(vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<seed*>& seed_list = *posw;
		
		// den aktuellen Ort feststellen damit bei LDD davon ausgehend der Zielort festgestellt werden kann
		aktort++;

		// Anzeige Rausfliegender
		int rausgeflogenN=0, rausgeflogenO=0, rausgeflogenS=0, rausgeflogenW=0;

		if(parameter[0].omp_num_threads==1)
		{
			double cum_time_individual_seed=0;//timer for indi seed 
			double cum_time_seeddisp=0;
				
			///Loop around all Seeds
			for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
			{
				double time_start_individual_seed=omp_get_wtime();
				
				pseed=(*pos);

				///If Seed is in a cone
				if (pseed->imcone==true)
				{
					double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									
					///and random number < rate of emerging seeds
					if(flug <= parameter[0].seedflugrate)
					{ // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Beginn
						
						double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
						if(ratiorn>0.0)
						{ // RN Groeszer Null Beginn
							
							pseed->imcone=false;
							
							double entfernung = 0;
							double richtung=0.0;
							double geschwindigkeit=0;
							double wrichtung=0.0;
							
							double time_start_individual_seed_seeddisp=omp_get_wtime();
							seeddisp(ratiorn, jahr, jquer, iquer, geschwindigkeit, wrichtung);
							cum_time_seeddisp+=omp_get_wtime()-time_start_individual_seed_seeddisp;

							
							
													
							// seed dispersal output:
					if(parameter[0].ivort>1045 && parameter[0].ausgabemodus!=9)
					{							
							double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
						

							if(seedeinschreibzufall<0.01){
														entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
														richtung=atan2(iquer,jquer);
														
								//Nadja: seed geben ihre Entf-Daten aus - Ausgabe Anfang
								FILE *dateizeiger;
								string dateiname;

								// Dateinamen zusammensetzen
								char dateinamesuf[12];
								sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].wiederholung);
								dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
							
								// Datei versuchen zum Lesen und Schreiben zu oeffnen
								dateizeiger = fopen (dateiname.c_str(), "r+");
								// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
								if (dateizeiger == NULL)
								{
								  dateizeiger = fopen (dateiname.c_str(), "w");
									fprintf(dateizeiger,"IVORT;");
									fprintf(dateizeiger, "name;");
									fprintf(dateizeiger, "jahr;");
									fprintf(dateizeiger, "parentheight;");
									//fprintf(dateizeiger, "xentfernung;");
									//fprintf(dateizeiger, "yentfernung;");
									fprintf(dateizeiger, "distance;");
									fprintf(dateizeiger, "direction;");
									fprintf(dateizeiger, "xcoo;");
									fprintf(dateizeiger, "ycoo;");
									fprintf(dateizeiger, "art;");
									fprintf(dateizeiger, "weatherchoice;");
									fprintf(dateizeiger, "thawing_depth;");	
									fprintf(dateizeiger, "windspd;");
									fprintf(dateizeiger, "winddir;");
									fprintf(dateizeiger, "\n");

									if (dateizeiger == NULL)
									{
										fprintf(stderr, "Fehler: seedentfernungsdatei konnte nicht geoeffnet werden!\n");
										exit(1);
									}
								}

								fseek(dateizeiger,0,SEEK_END);

								fprintf(dateizeiger, "%d;",parameter[0].ivort);
								fprintf(dateizeiger, "%d;", pseed->namem);
								fprintf(dateizeiger, "%d;", jahr);
								fprintf(dateizeiger, "%4.3f;", pseed->elternheight);
								//fprintf(dateizeiger, "%f;", iquer);
								//fprintf(dateizeiger, "%f;", jquer);
								fprintf(dateizeiger, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
								fprintf(dateizeiger, "%4.5f;", richtung);
								fprintf(dateizeiger, "%4.5f;", pseed->xcoo);
								fprintf(dateizeiger, "%4.5f;", pseed->ycoo);	
								fprintf(dateizeiger, "%d;", pseed->species);
								fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
								fprintf(dateizeiger, "%d;", parameter[0].thawing_depth);
								fprintf(dateizeiger, "%lf;",geschwindigkeit);
								fprintf(dateizeiger, "%lf;", wrichtung);
								fprintf(dateizeiger, "\n");

								fclose(dateizeiger);
							}
					}//output end if
							
							
							pseed->xcoo=pseed->xcoo+jquer;
							pseed->ycoo=pseed->ycoo+iquer;
							pseed->entfernung=entfernung;	


							
							/****************************************************************************************//**
							* \brief calculate Long Distance Dispersal between transekts
							*
							*
							*******************************************************************************************/
							// Entfernungen zwischen Einzelpatches ermitteln
							// Umrechnung von Grad in Meter  "1 Grad = 60 x 1852 m = 111120 m"
							/*
							double abschnittslaenge=((parameter[0].Nposmax-parameter[0].Nposmin)*111120)/parameter[0].mapylength;
							*/
							
							/// DEPRECEATED!!! not supported in this version
							//SEEDTRAVELBETWEEN==TRUE-> mit periodischen RB verknüpfen!!!!!!!!!!!!!!!!!!
							/*
							if ( (parameter[0].mapylength>1 || parameter[0].mapxlength>1) && parameter[0].seedtravelbetween==true && entfernung>(abschnittslaenge/2))
							{  // Long Distance Dispersal in andere Flaechen Beginn

								// Feststellen ob der seed in eine andere Flaeche fliegt ...
								int richtungsfaktor=0;	// dieser Wert zeigt an in welche Richtung der LDDSeed fliegt: -1==S und +1==N
								char richtungsname = 'l';
								
								
								if ( (richtung > 0.8) && (richtung <= 2.35) )
								{
									richtungsname='N';
									richtungsfaktor=-1; // nach Norden
								}					
								else if ( (richtung > 3.95) && (richtung <= 5.5) )
								{	
									richtungsname='S';
									richtungsfaktor=1; // nach Sueden
								}
								else if ( (richtung > 2.35) && (richtung <= 3.95) )
								{	
									richtungsname='O';
									richtungsfaktor=1; // nach Osten
								}
								else if ( (richtung > 5.5) || (richtung <= 0.8) )//Westen ist von 5.5  bis 0.8->entspricht 2pi
								{	
									richtungsname='W';
									richtungsfaktor=-1; // nach Westen
								}
								cout << "aktort=" << aktort << " ... Richtung=" << richtungsname << " ... und Faktor =" << richtungsfaktor << " ... und Entfernung - halbe ABSCHNITT=" << (entfernung-(abschnittslaenge/2)) << " ... Abschnittlaenge=" << abschnittslaenge << endl;
								
								///if seed flies out of the plot direction
								// Eintragsflaeche ermitteln Beginn
									
									// Wie weit fliegt der Same, nur in das angrenzende Feld oder noch weiter darüber hinaus?
									int neueweltcoo=0;

									if ((entfernung-(abschnittslaenge/2))<abschnittslaenge)
										++neueweltcoo;
									else
									{
										bool eingetragen=false;
										do
										{
											++neueweltcoo;
											double teilentfernung=(entfernung-(abschnittslaenge/2))-neueweltcoo*abschnittslaenge;
											
											if (teilentfernung<abschnittslaenge) 
												eingetragen=true;

											if (neueweltcoo>=parameter[0].mapylength) 
												eingetragen=true;
										
										} while (eingetragen==false);

									}

	//cout << neueweltcoo << endl;
									
									Seedoutput(aktort, entfernung, richtung, neueweltcoo);
									
									// Neue yworldcoo oder xworldcoo des seeds festlegen und vergleich ob er wirklich in ein neues Feld gekommen ist
									int neueyworldcoo, neuexworldcoo;
									if (richtungsname=='N' || richtungsname=='S')
									{
										neueyworldcoo=pseed->yworldcoo+richtungsfaktor*(neueweltcoo);
	//cout << neueyworldcoo << endl;
										if (neueyworldcoo!=pseed->yworldcoo)
										{ // seed trifft anderes Feld 
											pseed->yworldcoo=neueyworldcoo;
										
											if (pseed->yworldcoo>=(parameter[0].mapylength-1)) 
												pseed->yworldcoo=(parameter[0].mapylength-1);
											
											if (pseed->yworldcoo<0) 
												pseed->yworldcoo=0;
											
											if ( (pseed->yworldcoo<0) || (pseed->yworldcoo>=parameter[0].mapylength) )
											{
												printf("\n\nLaVeSi wurde beendet\n");
												printf("Aktueller Schritt => seedausbreitung.cpp\n");
												printf("... Grund: Bei LDSD ist die neue Flaeche (= %d) ausserhalb der verfuegbaren Flaechen (max = %d)!\n", pseed->yworldcoo, parameter[0].mapylength);
												exit(1);
											}


											// ziehe Zufallsposition im neuem Feld
											pseed->xcoo= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
											pseed->ycoo= 0.0 + ( (double)  ( ((double) (treerows-1)) *rand()/(RAND_MAX + 1.0)));
			
											// Sicherheitsabfrage 
											if ((pseed->ycoo<0) || (pseed->ycoo> (double) (treerows-1)) || (pseed->xcoo<0) || (pseed->xcoo> (double) (treecols-1)))
											{
												printf("\n\nLaVeSi wurde beendet\n");
												printf("Aktueller Schritt => seedausbreitung.cpp\n");
												printf("... Grund: LDSD nach Nord-Sued ausgebreiteter seed ist ausserhalb der Flaeche gelandet (mit Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
												exit(1);
											}
	//cout << neueyworldcoo << endl;

											// seed in die Ausbreiteliste schreiben und aus der seed_list loeschen
											LDDseed_list.push_back(pseed);
											
											// delete pseed; // Dies wuerde den Speicher an der Position freigeben und somit weiteres Verwenden unmoeglich machen!!
											
											pos=seed_list.erase(pos);			// ATTENTION!!!! now the loop is rewritten to iterate in the head  ----> this will lead here to false advances!!
										} 
										else
										{ // Same erreicht nicht den nächsten Plot 
											if (pseed->ycoo> (double) (treerows-1)) 
											{
												rausgeflogenN++;
											}
											else if (pseed->ycoo<0.0) 
											{
												rausgeflogenS++;
											}

											delete pseed;
											//RAUSFLIEGENDE WERDEN GELÖSCHT!
											pos=seed_list.erase(pos);			// ATTENTION!!!! now the loop is rewritten to iterate in the head  ----> this will lead here to false advances!!
										} // seed bleibt im Feld Beginn	
									}
									else if (richtungsname=='O' || richtungsname=='W')
									{
										neuexworldcoo=pseed->xworldcoo+richtungsfaktor*(neueweltcoo);
										
										if (neuexworldcoo!=pseed->xworldcoo)
										{ // seed trifft anderes Feld 
											pseed->xworldcoo=neuexworldcoo;
										
											if (pseed->xworldcoo>=(parameter[0].mapxlength-1)) 
												pseed->xworldcoo=(parameter[0].mapxlength-1);
											
											if (pseed->xworldcoo<0) 
												pseed->xworldcoo=0;
											
											if ( (pseed->xworldcoo<0) || (pseed->xworldcoo>=parameter[0].mapxlength) )
											{
												printf("\n\nLaVeSi wurde beendet\n");
												printf("Aktueller Schritt => seedausbreitung.cpp\n");
												printf("... Grund: Bei LDSD ist die neue Flaeche (= %d) ausserhalb der verfuegbaren Flaechen (max = %d)!\n", pseed->xworldcoo, parameter[0].mapxlength);
												exit(1);
											}


											pseed->xcoo= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
											pseed->ycoo= 0.0 + ( (double)  ( ((double) (treerows-1)) *rand()/(RAND_MAX + 1.0)));
			
											// Sicherheitsabfrage
											if ((pseed->ycoo<0) || (pseed->ycoo> (double) (treerows-1)) || (pseed->xcoo<0) || (pseed->xcoo> (double) (treecols-1)))
											{
												printf("\n\nLaVeSi wurde beendet\n");
												printf("Aktueller Schritt => seedausbreitung.cpp\n");
												printf("... Grund: LDSD nach Ost-West ausgebreiteter seed ist ausserhalb der Flaeche gelandet (mit Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
												exit(1);
											}

											// seed in die Ausbreiteliste schreiben und aus der seed_list loeschen
											LDDseed_list.push_back(pseed);
											
											// delete pseed; // Dies wuerde den Speicher an der Position an der noch die Infos des seeds stehen freigeben und somit weiteres Verwenden unmoeglich machen!!
											
											pos=seed_list.erase(pos);			// ATTENTION!!!! now the loop is rewritten to iterate in the head  ----> this will lead here to false advances!!
										}
										else
										{ // Same erreicht nicht den nächsten Plot 
											if (pseed->xcoo> (double) (treerows-1)) 
												rausgeflogenO++; 
												 if(parameter[0].periodRB==1){
													 pseed->xcoo=fmod(pseed->xcoo,(double)(treecols-1));
													 pseed->namem=0;//rename it to 0
													 }
											else if (pseed->xcoo<0.0) 
												rausgeflogenW++;
												if(parameter[0].periodRB==1){pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));}
											if(parameter[0].periodRB==0){ delete pseed;
															  pos=seed_list.erase(pos);}			// ATTENTION!!!! now the loop is rewritten to iterate in the head  ----> this will lead here to false advances!!
										} // seed bleibt im Feld Beginn
									}


							} // Long Distance Dispersal in andere Flaechen Ende
							*/
							


							/****************************************************************************************//**
							* \brief calculate  Dispersal within a plot
							*
							*
							*******************************************************************************************/
							// else
							// { // "normales" Verhalten, also Same landet außerhalb des Plots aber erreicht nicht den nächsten Plot 
								//(dazu müsste er mind. die Hälfte der Strecke zurücklegen), mit Same Beginn
								bool sameausserhalb=false;
								
								// Ueberpruefung ob im Feld sonst fliegt dieser aus dem Feld
								if(pseed->ycoo > (double) (treerows-1)) //treerows bestimmt N-S-Ausbreitung des Areals
								{
									if((parameter[0].periodRB==1))
									{
									   pseed->ycoo=fmod(pseed->ycoo,(double)(treerows-1));
									   pseed->namem=0;
									   pseed->namep=0;
									 //  pseed->cpSNP[0]=rand();
									 //  pseed->mtSNP[0]=rand();
									 //  pseed->cpSNP[1]=rand();
									 //  pseed->mtSNP[1]=rand();
	// 								 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
	// 								   pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
									} else 
									{
										sameausserhalb=true;
										rausgeflogenN++;
									}
								} else if(pseed->ycoo<0.0)
								{
									if((parameter[0].periodRB==1))
									{
									   pseed->ycoo=(double)(treerows-1)+fmod(pseed->ycoo,(double)(treerows-1));
									   pseed->namem=0;
									   pseed->namep=0;
									//   pseed->cpSNP[0]=rand();
									//   pseed->mtSNP[0]=rand();
									//   pseed->cpSNP[1]=rand();
									//   pseed->mtSNP[1]=rand();
	// 								 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
	// 								   pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
									} else 
									{
										sameausserhalb=true;
										rausgeflogenS++;
									}
								} else if((pseed->ycoo< (double) (treerows-1))&&(pseed->xcoo< (double) (treecols-1))&&(pseed->ycoo>0.0)&&(pseed->xcoo>0.0)&&(parameter[0].defTreevert==1)&&(vegetationtype[ceil(pseed->ycoo)][ceil(pseed->xcoo)]<0.09))
								{
										//delete pseed;
										sameausserhalb=true;
										//cout<<"seed in a lake\n";
										//IN SEEN LANDENDE seed WERDEN GELÖSCHT!
										//pos=seed_list.erase(pos);
								}
								 
								if(pseed->xcoo<0.0)
								{
									if((parameter[0].periodRB==1))
									{
									   //pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
									   pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1))+(double)(treecols-1);
									   pseed->namem=0;
									   pseed->namep=0;
										 //  pseed->cpSNP[0]=rand();
										//   pseed->mtSNP[0]=rand();
										 //  pseed->cpSNP[1]=rand();
										//   pseed->mtSNP[1]=rand();
	// 									 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
	// 								   	pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
									} else
									{
										sameausserhalb=true;
										rausgeflogenW++;
									}
								} else if(pseed->xcoo > (double) (treecols-1)) //treecols best. W-O-Ausbr. des Areals
								{
									if(parameter[0].periodRB==1)
									{
										pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
										pseed->namem=0;
										pseed->namep=0;
									//   pseed->cpSNP[0]=rand();
									//   pseed->mtSNP[0]=rand();
									//   pseed->cpSNP[1]=rand();
									//   pseed->mtSNP[1]=rand();
									   
									} else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX))
									{ //Weniger Ausbreitung aus Osten nach Westen als andersherum
										pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
									} else
									{	
										sameausserhalb=true;
										rausgeflogenO++;
									}
								} else if((parameter[0].defTreevert==1)&&(vegetationtype[ceil(pseed->ycoo)][ceil(pseed->xcoo)]<0.09))
								{
										//delete pseed;
										sameausserhalb=true;
										//cout<<"seed in a lake\n";
										//IN SEEN LANDENDE seed WERDEN GELÖSCHT!
										//pos=seed_list.erase(pos);
								}
		
								if( (sameausserhalb==false) && ( (pseed->ycoo<0.0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) ) )
								{
									printf("\n\nLaVeSi wurde beendet\n");
									printf("Aktueller Schritt => seedausbreitung.cpp\n");
									printf("... Grund: Normal ausgebreiteter seed ist nach Loeschvorgang der zu weit geflogenen immernoch ausserhalb der Flaeche (mit Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
									exit(1);
								}

								if(sameausserhalb==true)
								{ // Loeschvorgang seed Beginn
									delete pseed;
									pos=seed_list.erase(pos);
								} else
								{
									++pos;
								}

							// } // "normales" Verhalten mit seed Ende
						} // RN Groeszer Null Ende

					} else  // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Ende
					{
						++pos;
					}
				} else //Imcone? Ende
				{
					++pos;
				}
				
				cum_time_individual_seed+=omp_get_wtime()-time_start_individual_seed;
				

			} // Ende seed_list ablaufen
			
			cout << endl << "All seeds:" << cum_time_individual_seed << " with seeddisp-function:" << cum_time_seeddisp << endl;
				
		} else
		{ // multi-core-processing

			// manually chose the implementation of multi-core-processing
			int mcorevariant=2;
				// 1 == only advance
				// ... ==> result: advance makes the computation really really slow!!
				// 2 == split list to X lists => had to outsource the delete precedure because otherwise it would lead to SEGFAULTs!
			if(mcorevariant==1)
			{// OMP==1
			// cout << endl << " OMP seed dispersal variant (1 was chosen)=" << mcorevariant << endl;
				///Loop around all Seeds
				for(unsigned int pari=0; pari<seed_list.size(); )//++pari)
				// for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
				{
					// double t0=omp_get_wtime();
					list<seed*>::iterator pos=seed_list.begin();
					advance(pos, pari);
					pseed=(*pos);

					// cout << "t(advance)=" << omp_get_wtime()-t0 << endl;
					
					///If Seed is in a cone
					if (pseed->imcone==true)
					{
						double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
										
						///and random number < rate of emerging seeds
						if(flug <= parameter[0].seedflugrate)
						{ // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Beginn
							
							double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
												
							if(ratiorn>0.0)
							{ // RN Groeszer Null Beginn
								
								pseed->imcone=false;
								
								double entfernung = 0;
								double richtung=0.0;
								double geschwindigkeit=0;
								double wrichtung=0.0;
								
								seeddisp(ratiorn, jahr, jquer, iquer, geschwindigkeit, wrichtung);
								
								pseed->xcoo=pseed->xcoo+jquer;
								pseed->ycoo=pseed->ycoo+iquer;
								pseed->entfernung=entfernung;	


							
								/****************************************************************************************//**
								* \brief calculate  Dispersal within a plot
								*
								*
								*******************************************************************************************/
									bool sameausserhalb=false;
									
									// Ueberpruefung ob im Feld sonst fliegt dieser aus dem Feld
									if(pseed->ycoo > (double) (treerows-1)) //treerows bestimmt N-S-Ausbreitung des Areals
									{
										if((parameter[0].periodRB==1))
										{
										   pseed->ycoo=fmod(pseed->ycoo,(double)(treerows-1));
										   pseed->namem=0;
										   pseed->namep=0;
										 //  pseed->cpSNP[0]=rand();
										 //  pseed->mtSNP[0]=rand();
										 //  pseed->cpSNP[1]=rand();
										 //  pseed->mtSNP[1]=rand();
		// 								 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
		// 								   pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
										} else 
										{
											sameausserhalb=true;
											rausgeflogenN++;
										}
									} else if(pseed->ycoo<0.0)
									{
										if((parameter[0].periodRB==1))
										{
										   pseed->ycoo=(double)(treerows-1)+fmod(pseed->ycoo,(double)(treerows-1));
										   pseed->namem=0;
										   pseed->namep=0;
										//   pseed->cpSNP[0]=rand();
										//   pseed->mtSNP[0]=rand();
										//   pseed->cpSNP[1]=rand();
										//   pseed->mtSNP[1]=rand();
		// 								 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
		// 								   pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
										} else 
										{
											sameausserhalb=true;
											rausgeflogenS++;
										}
									} else if((pseed->ycoo< (double) (treerows-1))&&(pseed->xcoo< (double) (treecols-1))&&(pseed->ycoo>0.0)&&(pseed->xcoo>0.0)&&(parameter[0].defTreevert==1)&&(vegetationtype[ceil(pseed->ycoo)][ceil(pseed->xcoo)]<0.09))
									{
											//delete pseed;
											sameausserhalb=true;
											//cout<<"seed in a lake\n";
											//IN SEEN LANDENDE seed WERDEN GELÖSCHT!
											//pos=seed_list.erase(pos);
									}
									 
									if(pseed->xcoo<0.0)
									{
										if((parameter[0].periodRB==1))
										{
										   //pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
										   pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1))+(double)(treecols-1);
										   pseed->namem=0;
										   pseed->namep=0;
											 //  pseed->cpSNP[0]=rand();
											//   pseed->mtSNP[0]=rand();
											 //  pseed->cpSNP[1]=rand();
											//   pseed->mtSNP[1]=rand();
		// 									 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
		// 								   	pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
										} else
										{
											sameausserhalb=true;
											rausgeflogenW++;
										}
									} else if(pseed->xcoo > (double) (treecols-1)) //treecols best. W-O-Ausbr. des Areals
									{
										if(parameter[0].periodRB==1)
										{
											pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
											pseed->namem=0;
											pseed->namep=0;
										//   pseed->cpSNP[0]=rand();
										//   pseed->mtSNP[0]=rand();
										//   pseed->cpSNP[1]=rand();
										//   pseed->mtSNP[1]=rand();
										   
										} else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX))
										{ //Weniger Ausbreitung aus Osten nach Westen als andersherum
											pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
										} else
										{	
											sameausserhalb=true;
											rausgeflogenO++;
										}
									} else if((parameter[0].defTreevert==1)&&(vegetationtype[ceil(pseed->ycoo)][ceil(pseed->xcoo)]<0.09))
									{
											//delete pseed;
											sameausserhalb=true;
											//cout<<"seed in a lake\n";
											//IN SEEN LANDENDE seed WERDEN GELÖSCHT!
											//pos=seed_list.erase(pos);
									}
			
									if( (sameausserhalb==false) && ( (pseed->ycoo<0.0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) ) )
									{
										printf("\n\nLaVeSi wurde beendet\n");
										printf("Aktueller Schritt => seedausbreitung.cpp\n");
										printf("... Grund: Normal ausgebreiteter seed ist nach Loeschvorgang der zu weit geflogenen immernoch ausserhalb der Flaeche (mit Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
										exit(1);
									}

									if(sameausserhalb==true)
									{ // Loeschvorgang seed Beginn
										delete pseed;
										pos=seed_list.erase(pos);
										++pari;
									} else
									{
										++pari;
									}

							} // RN Groeszer Null Ende

						} else  // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Ende
						{
							++pari;
						}
					} else //Imcone? Ende
					{
						++pari;
					}
				} // Ende seed_list ablaufen
			}// OMP==1
			if(mcorevariant==2)
			{// OMP==2
			// cout << endl << " OMP seed dispersal variant (2 was chosen)=" << mcorevariant << endl;
			
				
				double time_start_seeddisp=omp_get_wtime();
				
				omp_set_dynamic(0); //disable dynamic teams
				omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
					
				#pragma omp parallel private(pseed)
				{
					// initialize the info for each of the thread
					int thread_count = omp_get_num_threads();
					int thread_num   = omp_get_thread_num();
					size_t chunk_size= seed_list.size() / thread_count;
					auto begin = seed_list.begin();
					std::advance(begin, thread_num * chunk_size);
					auto end = begin;
					
					if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
					{
						end = seed_list.end();
						// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
					} else
					{
						std::advance(end, chunk_size);
						// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
					}
					
					
					// wait for all threads to initialize and then proceed
					#pragma omp barrier
					
					for(auto it = begin; it != end; ++it)
					{
						pseed=(*it);
					
					
					///Loop around all Seeds
					// for(unsigned int pari=0; pari<seed_list.size(); )//++pari)
					// for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
					// {
						// double t0=omp_get_wtime();
						// list<seed*>::iterator pos=seed_list.begin();
						// advance(pos, pari);
						// pseed=(*pos);

						// cout << "t(advance)=" << omp_get_wtime()-t0 << endl;
						
						///If Seed is in a cone
						if (pseed->imcone==true)
						{
							double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
							///and random number < rate of emerging seeds
							if(flug <= parameter[0].seedflugrate)
							{ // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Beginn
								
								double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
													
								if(ratiorn>0.0)
								{ // RN Groeszer Null Beginn
									
									pseed->imcone=false;
									
									double entfernung = 0;
									double richtung=0.0;
									double geschwindigkeit=0;
									double wrichtung=0.0;

									seeddisp(ratiorn, jahr, jquer, iquer, geschwindigkeit, wrichtung);
									
									pseed->xcoo=pseed->xcoo+jquer;
									pseed->ycoo=pseed->ycoo+iquer;
									pseed->entfernung=entfernung;	


								
									/****************************************************************************************//**
									* \brief calculate  Dispersal within a plot
									*
									*
									*******************************************************************************************/
										bool sameausserhalb=false;
										
										// Ueberpruefung ob im Feld sonst fliegt dieser aus dem Feld
										if(pseed->ycoo > (double) (treerows-1)) //treerows bestimmt N-S-Ausbreitung des Areals
										{
											if((parameter[0].periodRB==1))
											{
											   pseed->ycoo=fmod(pseed->ycoo,(double)(treerows-1));
											   pseed->namem=0;
											   pseed->namep=0;
											 //  pseed->cpSNP[0]=rand();
											 //  pseed->mtSNP[0]=rand();
											 //  pseed->cpSNP[1]=rand();
											 //  pseed->mtSNP[1]=rand();
			// 								 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
			// 								   pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
											} else if((parameter[0].periodRB==3))
											{
												sameausserhalb=true;
												rausgeflogenS++;
											} else 
											{
												sameausserhalb=true;
												rausgeflogenN++;
											}
										} else if(pseed->ycoo<0.0)
										{
											if((parameter[0].periodRB==1))
											{
											   pseed->ycoo=(double)(treerows-1)+fmod(pseed->ycoo,(double)(treerows-1));
											   pseed->namem=0;
											   pseed->namep=0;
											//   pseed->cpSNP[0]=rand();
											//   pseed->mtSNP[0]=rand();
											//   pseed->cpSNP[1]=rand();
											//   pseed->mtSNP[1]=rand();
			// 								 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
			// 								   pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
											} else if((parameter[0].periodRB==3))
											{
												sameausserhalb=true;
												rausgeflogenS++;
											} else 
											{
												sameausserhalb=true;
												rausgeflogenS++;
											}
										} else if((pseed->ycoo< (double) (treerows-1))&&(pseed->xcoo< (double) (treecols-1))&&(pseed->ycoo>0.0)&&(pseed->xcoo>0.0)&&(parameter[0].defTreevert==1)&&(vegetationtype[ceil(pseed->ycoo)][ceil(pseed->xcoo)]<0.09))
										{
												//delete pseed;
												sameausserhalb=true;
												//cout<<"seed in a lake\n";
												//IN SEEN LANDENDE seed WERDEN GELÖSCHT!
												//pos=seed_list.erase(pos);
										}
										 
										if(pseed->xcoo<0.0)
										{
											if((parameter[0].periodRB==1 || parameter[0].periodRB==3))
											{
											   //pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
											   pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1))+(double)(treecols-1);
											   pseed->namem=0;
											   pseed->namep=0;
												 //  pseed->cpSNP[0]=rand();
												//   pseed->mtSNP[0]=rand();
												 //  pseed->cpSNP[1]=rand();
												//   pseed->mtSNP[1]=rand();
			// 									 }else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX)){//reduzierte periodic BC
			// 								   	pseed->xcoo=(double)(treecols-1)+fmod(pseed->xcoo,(double)(treecols-1));
											} else
											{
												sameausserhalb=true;
												rausgeflogenW++;
											}
										} else if(pseed->xcoo > (double) (treecols-1)) //treecols best. W-O-Ausbr. des Areals
										{
											if(parameter[0].periodRB==1 || parameter[0].periodRB==3)
											{
												pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
												pseed->namem=0;
												pseed->namep=0;
											//   pseed->cpSNP[0]=rand();
											//   pseed->mtSNP[0]=rand();
											//   pseed->cpSNP[1]=rand();
											//   pseed->mtSNP[1]=rand();
											   
											} else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX))
											{ //Weniger Ausbreitung aus Osten nach Westen als andersherum
												pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
											} else
											{	
												sameausserhalb=true;
												rausgeflogenO++;
											}
										} else if((parameter[0].defTreevert==1)&&(vegetationtype[ceil(pseed->ycoo)][ceil(pseed->xcoo)]<0.09))
										{
												//delete pseed;
												sameausserhalb=true;
												//cout<<"seed in a lake\n";
												//IN SEEN LANDENDE seed WERDEN GELÖSCHT!
												//pos=seed_list.erase(pos);
										}
				
										if( (sameausserhalb==false) && ( (pseed->ycoo<0.0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) ) )
										{
											printf("\n\nLaVeSi wurde beendet\n");
											printf("Aktueller Schritt => seedausbreitung.cpp\n");
											printf("... Grund: Normal ausgebreiteter seed ist nach Loeschvorgang der zu weit geflogenen immernoch ausserhalb der Flaeche (mit Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
											exit(1);
										}

										if(sameausserhalb==true)
										{
											pseed->ycoo=-99999.9;
											// pseed->xcoo=-99999.9;
										}
								} // RN Groeszer Null Ende
							}   // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Ende
						} //Imcone? Ende
					} // Ende seed_list ablaufen
				}//END: pragma for
				
				// cout << " seed_list.size (before)=" << seed_list.size() << endl;
				// double time_start_delete=omp_get_wtime();
				for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
				{// START: delete seeds with sameausserhalb==true
					pseed=(*pos);

					if(pseed->ycoo==-99999.9)
					{ // Loeschvorgang seed Beginn
						delete pseed;
						pos=seed_list.erase(pos);
					} else
					{
						++pos;
					}
				}// END: delete seeds
				// cout << " seed_list.size (after)=" << seed_list.size() << endl;
				// cout << " Time(seeddisp)=" << time_start_delete-time_start_seeddisp << endl;
				// cout << " Time(deletion)=" << omp_get_wtime()-time_start_delete << endl;
			}// OMP==2

		}


		// Anzeige Rausfliegender
		if (parameter[0].seedausbranzeige==true) 
			printf("\n   Rausflieger(N/O/S/W)=(%d/%d/%d/%d) ", rausgeflogenN, rausgeflogenO, rausgeflogenS, rausgeflogenW);
	} // Weltschleife Ende




	/****************************************************************************************//**
	* \brief write dispersed seeds into the seedList of the respective site
	*
	*
	*******************************************************************************************/
	/*
	// seed die weit ausgebreitet wurden werden in die entsprechend seed_listn am neuen Ort geschrieben
	if ( (parameter[0].seedtravelbetween==true) & (LDDseed_list.size()>0) )
	{ // Seeds in WeltListen schreiben Beginn 
		
		// Wenn seed in der LDSD-Liste enthalten sind, so werde diese in die entsprechenden world_seed_listn geschoben
		for (list<seed*>::iterator pos = LDDseed_list.begin(); pos != LDDseed_list.end(); )
		{ // seed_list ablaufen Beginn
			pseed=(*pos);

			if ( (pseed->ycoo<0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0) | (pseed->xcoo> (double) (treecols-1)) )
			{
				printf("\n\nLaVeSi wurde beendet\n");
				printf("Aktueller Schritt => seedausbreitung.cpp\n");
				printf("... Grund: LDSD Ausgebreiteter seed in der LDDseed_list hat Koordinaten die ausserhalb der Flaeche liegen (mit Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
				exit(1);
			}

			// Ermittle die neue Position der Liste in die der LDD-Same eingetragen werden soll
			int aktortadvance=pseed->xworldcoo + (pseed->yworldcoo * parameter[0].mapxlength);// + 1;
//cout << "Advancing=" << aktortadvance << " xworldcooseed=" << pseed->xworldcoo << "   yworldcooseed=" << pseed->yworldcoo << endl; 		
			
			// Setze einen Iterator fuer die world_seed_list und gleich in die entsprechende Region
			vector<list<seed*> >::iterator posw=world_seed_list.begin();
			advance(posw, aktortadvance);	//LDDCG

			// Greife auf die an dem Ort befindende seed_list zu
			list<seed*>& seed_list = *posw;

			// Schreibe den seed in die seed_list und loesche ihn aus der urspruenglichen Liste
			seed_list.push_back(pseed);
			
			//delete pseed; // Dieser Aufruf wuerde abermals den Speicher des seeds freigeben und dadurch Fehler verursachen wenn dieser wieder genutzt wird!!
			
			pos=LDDseed_list.erase(pos);
		} // seed_list ablaufen Ende
	} // Seeds in WeltListen schreiben Ende

	*/
}































