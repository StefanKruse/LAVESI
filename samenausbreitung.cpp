/****************************************************************************************//**
 * \brief Calculate dispersal distance dependent on dispersal mode (0-4)
 *
 * 0=zufaellig, 1=exp, 2=fat tailed, 3=gaussian und 4=gaussian+fat tailed combined
 *
 *
 *******************************************************************************************/
double getEntfernung(double ratiorn_help)	
{

	// calculate the dispersal distance
	double entf_help;
	// ... either randomly
	if (parameter[0].dispersalmode==0)
	{
		entf_help= ((ratiorn_help-1)*100.0)/(0.05-1);
	}
	// ... or neg. exponential
	else if (parameter[0].dispersalmode==1)
	{
		entf_help= parameter[0].entfernungsteiler * ((log(ratiorn_help)/(-0.2))/0.16);
	}
	// ... or fat tailed
	else if (parameter[0].dispersalmode==2)
	{ 
		double fatalpha=0.5;
		entf_help= parameter[0].entfernungsteiler *  pow(ratiorn_help, (-1*(1+fatalpha)) );
	}
	// ... or gaussian
	else if (parameter[0].dispersalmode==3)
	{ 
		double gaussweite=40, gaussmaxh=1, gaussposcenter=0;
		entf_help= parameter[0].entfernungsteiler *  sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter;
	}
	// ... or gaussian combined with fat tailed
	else if (parameter[0].dispersalmode==4)
	{ 
		double gaussfatratio=0.5;
		double gaussweite=20, gaussmaxh=1, gaussposcenter=0;
		double fatalpha=0.5;
		entf_help= parameter[0].entfernungsteiler * ( 0.5*( gaussfatratio*(sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter) + (1/gaussfatratio)*(pow(ratiorn_help, (-1*(1+fatalpha)) )) ) );
	}
	else 
	{
		printf("LAVESI exited\n\n");
		printf("... in samenausbreitung.cpp\n");
		printf("... cause: no valid seed dispersalmode specified\n");
		exit(1);
	}

	return(entf_help);
}

/****************************************************************************************//**
 * \brief Write the long dispersed seeds to a file
 *
 * 
 * 
 *
 *******************************************************************************************/
void Seedoutput(int aktort, double entfernung, double richtung, int neueweltcoo) 
{
	// write long dispersed seeds to a file
	
	// try to open the file stream
	FILE *dateizeiger;
	string dateiname;
	dateiname="dataseed_LDSD.csv";
	dateizeiger = fopen (dateiname.c_str(), "r+");

	// if not existing, create new with header
	if (dateizeiger == NULL)
	{
	  dateizeiger = fopen (dateiname.c_str(), "w");
		fprintf(dateizeiger, "ivort;");
		fprintf(dateizeiger, "aktort;");
		fprintf(dateizeiger, "entfernung;");
		fprintf(dateizeiger, "richtung;");
		fprintf(dateizeiger, "neueweltcoo;");
		fprintf(dateizeiger, "\n");

		if (dateizeiger == NULL)
		{
			fprintf(stderr, "Error: file for long dispersed seeds could not be opened!\n");
			exit(1);
		}
	}

	// append the new data
	fseek(dateizeiger,0,SEEK_END);

	fprintf(dateizeiger, "%d;", parameter[0].ivort);
	fprintf(dateizeiger, "%d;", aktort);
	fprintf(dateizeiger, "%4.5f;", entfernung);
	fprintf(dateizeiger, "%4.5f;", richtung);
	fprintf(dateizeiger, "%d;", neueweltcoo);
	fprintf(dateizeiger, "\n");

	// close the file stream
	fclose (dateizeiger);
}

/****************************************************************************************//**
 * \brief Calculate seed dispersal and if specified also long seed dispersal events
 *
 * 
 *
 *******************************************************************************************/
void Samenausbreitung(int baeumerows, int baeumecols, struct Parameter *parameter, vector<list<Samen*> > &WeltSamenListe)
{
	int aktort=0;
	
	// declare a buffer list for long dispersed seeds
	list<Samen*> LDDSamenListe; 

	for (vector<list<Samen*> >::iterator posw = WeltSamenListe.begin(); posw != WeltSamenListe.end(); ++posw)
	{ 
		list<Samen*>& SamenListe = *posw;
		
		aktort++;

		// declaration of variables for console output of seeds exiting the plot (==rausgeflogen) with the cardinal direction
		int rausgeflogenN=0, rausgeflogenO=0, rausgeflogenS=0, rausgeflogenW=0;

		// loop around all seeds
		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); )
		{ 
			pSamen=(*pos);

			// only disperse seeds that are still in the cones of the trees 
			if (pSamen->imzapfen==true)
			{ 
				// stochastically evaluate if a seed is released from the cone
				double flug = 0 +( (double) 1*rand()/(RAND_MAX + 1.0));
				if (flug <= parameter[0].samenflugrate)
				{
					// generate the new seeds coordinates at the ground if dispersed
					double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					if (ratiorn>0.0)
					{
						pSamen->imzapfen=false;
						double entfernung;

						// estimate the distance from the seed source tree
						entfernung = getEntfernung(ratiorn);

						// determine the direction of the seed dispersal
						double richtung= 0.0 +( (double) (2*M_PI)*rand()/(RAND_MAX + 1.0));
						double iquer=sin(richtung)*entfernung;
						double jquer=cos(richtung)*entfernung;
						pSamen->xcoo=pSamen->xcoo+jquer;
						pSamen->ycoo=pSamen->ycoo+iquer;

						// determine if a seed is reaching a plot further outside the current plot area ( named long dispersed seed dispersal LDSD)
						// ... either a seed reaches a further plot area which is specified by the user but only if the seed travelled half of the distance
						// ... ... to evaluate this calculate the distance between each simulated plot
						double abschnittslaenge=((parameter[0].Nposmax-parameter[0].Nposmin)*111120)/parameter[0].mapylength;
						if ( (parameter[0].mapylength>1 | parameter[0].mapxlength>1) & parameter[0].seedtravelbetween==true & entfernung>(abschnittslaenge/2))
						{

							// Feststellen ob der Samen in eine andere Flaeche fliegt ...
							int richtungsfaktor=0;	// dieser Wert zeigt an in welche Richtung der LDDSeed fliegt: -1==S und +1==N
							char richtungsname;
							
							if ( (richtung > 0.8) & (richtung <= 2.35) )
							{
								richtungsname='N';
								richtungsfaktor=-1;
							}					
							else if ( (richtung > 3.95) & (richtung <= 5.5) )
							{	
								richtungsname='S';
								richtungsfaktor=1;
							}
							else if ( (richtung > 2.35) & (richtung <= 3.95) )
							{	
								richtungsname='O';
								richtungsfaktor=1;
							}
							else if ( (richtung > 5.5) | (richtung <= 0.8) )
							{	
								richtungsname='W';
								richtungsfaktor=-1;
							}
							
								// determine how many plots the long dispersed seed passes
								int neueweltcoo=0;
								if ((entfernung-(abschnittslaenge/2))<abschnittslaenge)
								{
									++neueweltcoo;
								}
								else
								{
									bool eingetragen=false;
									do
									{
										++neueweltcoo;
										double teilentfernung=(entfernung-(abschnittslaenge/2))-neueweltcoo*abschnittslaenge;
										
										if (teilentfernung<abschnittslaenge)
										{
											eingetragen=true;
										}
										
										if (neueweltcoo>=parameter[0].mapylength) 
										{
											eingetragen=true;
										}
									} while (eingetragen==false);
								}

								Seedoutput(aktort, entfernung, richtung, neueweltcoo);
								
								// determine the new coordinates of the seed
								int neueyweltcoo, neuexweltcoo;
								// ... if North or South
								if (richtungsname=='N' | richtungsname=='S')
								{
									neueyweltcoo=pSamen->yweltcoo+richtungsfaktor*(neueweltcoo);
									
									if (neueyweltcoo!=pSamen->yweltcoo)
									{
										pSamen->yweltcoo=neueyweltcoo;
									
										if (pSamen->yweltcoo>=(parameter[0].mapylength-1)) 
										{
											pSamen->yweltcoo=(parameter[0].mapylength-1);
										}
										
										if (pSamen->yweltcoo<0) 
										{
											pSamen->yweltcoo=0;
										}
										
										// security check
										if ( (pSamen->yweltcoo<0) | (pSamen->yweltcoo>=parameter[0].mapylength) )
										{
											printf("\n\nLAVESI exited\n");
											printf("... samenausbreitung.cpp\n");
											printf("... cause: area of new long dispersed seed outside of available locations (= %d, max = %d)!\n", pSamen->yweltcoo, parameter[0].mapylength);
											exit(1);
										}

										// randomly determine the coordinates
										pSamen->xcoo= 0.0 + ( (double)  ( ((double) (baeumecols-1)) *rand()/(RAND_MAX + 1.0)));
										pSamen->ycoo= 0.0 + ( (double)  ( ((double) (baeumerows-1)) *rand()/(RAND_MAX + 1.0)));
		
										// security check 
										if ((pSamen->ycoo<0) || (pSamen->ycoo> (double) (baeumerows-1)) || (pSamen->xcoo<0) || (pSamen->xcoo> (double) (baeumecols-1)))
										{
											printf("\n\nLAVESI exited\n");
											printf("... samenausbreitung.cpp\n");
											printf("... cause: seed at north or south exited the new location area Pos(Y=%4.2f,X=%4.2f)\n", pSamen->ycoo, pSamen->xcoo);
											exit(1);
										}

										// add seed to the seed list and erase it from the current seed list
										LDDSamenListe.push_back(pSamen);
										pos=SamenListe.erase(pos);
									} 
									// ... or if a seed does not reach the next location it is removed
									else
									{
										if (pSamen->ycoo> (double) (baeumerows-1)) 
										{
											rausgeflogenN++;
										}
										else if (pSamen->ycoo<0.0) 
										{
											rausgeflogenS++;
										}

										delete pSamen;
										pos=SamenListe.erase(pos);
									}	
								}
								// ... for East and West
								else if (richtungsname=='O' | richtungsname=='W')
								{
									neuexweltcoo=pSamen->xweltcoo+richtungsfaktor*(neueweltcoo);
									
									if (neuexweltcoo!=pSamen->xweltcoo)
									{
										pSamen->xweltcoo=neuexweltcoo;
									
										if (pSamen->xweltcoo>=(parameter[0].mapxlength-1))
										{
											pSamen->xweltcoo=(parameter[0].mapxlength-1);
										}
										
										if (pSamen->xweltcoo<0) 
										{
											pSamen->xweltcoo=0;
										}
										
										// security check
										if ( (pSamen->xweltcoo<0) | (pSamen->xweltcoo>=parameter[0].mapxlength) )
										{
											printf("\n\nLAVESI exited\n");
											printf("... samenausbreitung.cpp\n");
											printf("... cause: area of new long dispersed seed outside of available locations (= %d, max = %d)!\n", pSamen->xweltcoo, parameter[0].mapxlength);
											exit(1);											
										}

										// randomly determine the new coordinates
										pSamen->xcoo= 0.0 + ( (double)  ( ((double) (baeumecols-1)) *rand()/(RAND_MAX + 1.0)));
										pSamen->ycoo= 0.0 + ( (double)  ( ((double) (baeumerows-1)) *rand()/(RAND_MAX + 1.0)));
		
										// security check
										if ((pSamen->ycoo<0) || (pSamen->ycoo> (double) (baeumerows-1)) || (pSamen->xcoo<0) || (pSamen->xcoo> (double) (baeumecols-1)))
										{
											printf("\n\nLAVESI exited\n");
											printf("... samenausbreitung.cpp\n");
											printf("... cause: seed at east or west exited the new location area Pos(Y=%4.2f,X=%4.2f)\n", pSamen->ycoo, pSamen->xcoo);
											exit(1);
										}

										// add seed to the seed list and erase it from the current seed list
										LDDSamenListe.push_back(pSamen);
										pos=SamenListe.erase(pos);
									} 
									// ... or if a seed does not reach the next location it is removed
									else
									{
										if (pSamen->xcoo> (double) (baeumerows-1)) 
										{
											rausgeflogenO++;
										}
										else if (pSamen->xcoo<0.0) 
										{
											rausgeflogenW++;
										}

										delete pSamen;
										pos=SamenListe.erase(pos);
									}
								}


						}
						// ... or the seed is deleted when it exited the plot area
						else
						{
							bool sameausserhalb=false;
							
							// check if the seed is exiting
							if (pSamen->ycoo> (double) (baeumerows-1)) 
							{
								sameausserhalb=true;
								rausgeflogenN++;
							}
							else if (pSamen->ycoo<0.0) 
							{
								sameausserhalb=true;
								rausgeflogenS++;
							}
							else 
							{
								if (pSamen->xcoo<0.0) 
								{
									sameausserhalb=true;
									rausgeflogenW++;
								}
								else if (pSamen->xcoo> (double) (baeumecols-1)) 
								{
									sameausserhalb=true;
									rausgeflogenO++;
								}
							}
	
							if ( (sameausserhalb==false) & ( (pSamen->ycoo<0) | (pSamen->ycoo> (double) (baeumerows-1)) | (pSamen->xcoo<0.0) | (pSamen->xcoo> (double) (baeumecols-1)) ) )
							{
								printf("\n\n LAVESI exited\n");
								printf("... in samenausbreitung.cpp\n");
								printf("... cause: normal dispersed seed was introduced non-validly into the surrounding of the simulated plot area Pos(Y=%4.2f,X=%4.2f)\n", pSamen->ycoo, pSamen->xcoo);
								exit(1);
							}

							// delete the seed
							if (sameausserhalb==true)
							{
								delete pSamen;
								pos=SamenListe.erase(pos);
							}
							else
							{
								++pos;
							}
						}
					}
					else if (ratiorn<=0.0)
					{
						++pos;
					}
				}
				else if (flug > parameter[0].samenflugrate)
				{
					++pos;
				}
			}
			else if (pSamen->imzapfen!=true)
			{
				++pos;
			}
		}

		// console ouput of seeds exiting the plot
		if (parameter[0].samenausbranzeige==true)
		{
			printf("\n   Long dispersed seeds (N/E/S/W)=(%d/%d/%d/%d) ", rausgeflogenN, rausgeflogenO, rausgeflogenS, rausgeflogenW);
		}
	}

	// write dispersed seeds into the SamenList of the respective site
	if ( (parameter[0].seedtravelbetween==true) & (LDDSamenListe.size()>0) )
	{
		for (list<Samen*>::iterator pos = LDDSamenListe.begin(); pos != LDDSamenListe.end(); )
		{
			pSamen=(*pos);

			// security check
			if ( (pSamen->ycoo<0) | (pSamen->ycoo> (double) (baeumerows-1)) | (pSamen->xcoo<0) | (pSamen->xcoo> (double) (baeumecols-1)) )
			{
				printf("\n\nLAVESI exited\n");
				printf("... in samenausbreitung.cpp\n");
				printf("... cause: LDSD seed in list with wrong coordinates Pos(Y=%4.2f,X=%4.2f)\n", pSamen->ycoo, pSamen->xcoo);
				exit(1);
			}

			// determine the new location into with the seed is placed
			int aktortadvance=pSamen->xweltcoo + (pSamen->yweltcoo * parameter[0].mapxlength);
			vector<list<Samen*> >::iterator posw=WeltSamenListe.begin();
			advance(posw, aktortadvance);

			// open the seed list at the new location
			list<Samen*>& SamenListe = *posw;

			// write the seed into the new list and delete it from the long dispersed seed list
			SamenListe.push_back(pSamen);
			pos=LDDSamenListe.erase(pos);
		}
	}
}


