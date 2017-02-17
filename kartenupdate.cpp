/****************************************************************************************//**
 * \brief 
 *
 * loop around every tree to determine it's individual density value \n
 * that's the value with which it influences other trees
 *
 * 3 kinds of density calculation are possible \n
 *    1: no density calculated and dichtewert=0.0 \n
 *    2: ZOI-kind of type
 *    3: random field of ZOI-type values
 *******************************************************************************************/
void IndividualTreeDensity(list<Baum*> &BaumListe, vector<Karten*> &KartenListe)
{
	// go through tree list
	for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
	{ 
			pBaum=(*pos);

			// determine the trees position in the density grid
			// 				ycoo/xcoo = exact position
			//				ycoo/xcoo* sizemagnif = cell number in density grid
			int i = (int) floor(pBaum->ycoo * parameter[0].sizemagnif);
			int j = (int) floor(pBaum->xcoo * parameter[0].sizemagnif);

			// handling of three different modes of density calculation 
			// ... DENSITY 1 without any density
			if (parameter[0].dichteart == 1)
			{
				pBaum->dichtewert = 0.0;
			}
			// ... or DENSITY 2 or DENSITY 3 with a zone-of-influence (ZOI) or random determined density value calculated for each tree
			else 
			{
				// iterate through the surrounding of each tree and assess the density value
				
				// local declared variables
				double flaechengroesze=0.0;
				
				// manipulation of the area that is influenced by a tree
				if (parameter[0].calcinfarea == 1) //linearly increasing
				{
					flaechengroesze = pBaum->dbasal * parameter[0].incfac/100.0; 
				}
				
				// if a tree is only influencing one tile
				if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
				{
					// ... for DENSITY 2 ZOI
					if (parameter[0].dichteart==2)
					{
						if (KartenListe[i*baeumecols*parameter[0].sizemagnif+j]->baumdichtewert > 0.0) 
						{
							// calculation of the density values
							// ... either the sum of values
							if (parameter[0].densitytiletree==1)	
							{
								pBaum->dichtewert = (1.0 - (pBaum->dichtewert/ KartenListe[i*baeumecols*parameter[0].sizemagnif+j]->baumdichtewert));
							}
							// ... or the multiplication of the values
							else if (parameter[0].densitytiletree==2)	
							{
								pBaum->dichtewert =	(1.0 - (pBaum->dichtewert
															/ 
															(KartenListe[i*baeumecols*parameter[0].sizemagnif+j]->baumdichtewert*pBaum->dichtewert) 
															)
													);
							}
						}
						else
						{
							pBaum->dichtewert=0.0; 
						}
					}
					// ... or DENSITY 3 random
					else if (parameter[0].dichteart==3)
					{
						// sample a random tile
						int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (baeumerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (baeumecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						
						if (KartenListe[izuf*baeumecols*parameter[0].sizemagnif+jzuf]->baumdichtewert > 0.0) 
						{
							// calculation of the density values
							// ... either the sum of values
							if (parameter[0].densitytiletree==1)
							{
								pBaum->dichtewert = (1.0 - (pBaum->dichtewert/ KartenListe[izuf*baeumecols*parameter[0].sizemagnif+jzuf]->baumdichtewert));
							}
							// ... or the multiplication of the values
							else if (parameter[0].densitytiletree==2)
							{
								pBaum->dichtewert=(1.0 - (pBaum->dichtewert
															/ 
															(KartenListe[i*baeumecols*parameter[0].sizemagnif+j]->baumdichtewert*pBaum->dichtewert) 
															)
													);
							}
						}
						else
						{
							pBaum->dichtewert=0.0;
						}
					}
					
				}
				// ... or if a tree is influencing more than one tile
				else
				{
					// determine the distance to the most distant sub grid tile that is influenced by the tree 
					int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
					double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

					double sumdichtewert=0;
					unsigned int anzahlflaechen=0;
					
					// go through all tiles that are influenced by the tree
					for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
					{
						for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
						{
							if ((rastposi<=( (baeumerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (baeumecols-1)*parameter[0].sizemagnif) && rastposj>=0))
							{
								// calculate the current distance from the tree
								double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
								
								if (entfrastpos<= (double) xyquerrastpos)
								{
									int icurr=rastposi;
									int jcurr=rastposj;

									// randomization of i/j vals only if DENSITY 3
									if (parameter[0].dichteart==3)
									{
										icurr= (int) floor( 0.0 + ( (double)  ( ((double) (baeumerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (baeumecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
									}
									
									// different modes for processing the density values for each tree
									if (parameter[0].densitytiletree==1)
									{
										sumdichtewert+=KartenListe[icurr*baeumecols*parameter[0].sizemagnif+jcurr]->baumdichtewert
														* (1-entfrastpos/maxdist);
									}
									else if (parameter[0].densitytiletree==2)
									{
										// first weighing of individual influencing values and then addtion of the ground level
										sumdichtewert+=(KartenListe[icurr*baeumecols*parameter[0].sizemagnif+jcurr]->baumdichtewert
														-pow(pBaum->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0) )
														* pow(pBaum->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)
														+ pow(pBaum->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0);
									}

									anzahlflaechen++;
								}
							}
						}
					}
					
					// calculate a density value fraction a tree experienced between 0 and 1
					if (sumdichtewert > 0.0)
					{
						pBaum->dichtewert = 1.0 - (pBaum->dichtewert/sumdichtewert);
					}
					else
					{
						pBaum->dichtewert=0.0;
					}
				}

				// modifie the density value
				// ... generally if it should be depending on the tree diameter
				pBaum->dichtewert= pBaum->dichtewert *pow((1.0-(0.01/pBaum->dbasal)),parameter[0].dichtewertdbasaleinfluss);
				// ... or by different modes as specified
				// ... ... either constant density values for the three height classes
				if (parameter[0].dichtheightrel==1)
				{
					if (pBaum->hoehe<40)
						pBaum->dichtewert=1.0;
					else if (pBaum->hoehe>=40 & pBaum->hoehe<200)
						pBaum->dichtewert=0.5;
					else if (pBaum->hoehe>=200)
						pBaum->dichtewert=0.0;
				}
				// ... ... or linearly decreasing with age
				else if (parameter[0].dichtheightrel==2) 
				{
					double dichtewertminimum=0.1;
					double alterseinfluss=( -1*(1-dichtewertminimum)/50.0* (double) pBaum->alter + 1.0 );
					
					if(alterseinfluss<dichtewertminimum)
					{
						alterseinfluss=dichtewertminimum;
					}
					
					pBaum->dichtewert= pBaum->dichtewert*alterseinfluss;
				}
				
				// securtiy check
				// ... lower limit
				if (pBaum->dichtewert<0)
				{
					pBaum->dichtewert=0.0;
				}
				// ... upper limit
				if (pBaum->dichtewert>parameter[0].desitymaxreduction) 
				{
					pBaum->dichtewert=parameter[0].desitymaxreduction;
				}
			} 

			++pos;
	}
}




/****************************************************************************************//**
 * \brief Reset density maps "Karten"
 *
 * 
 *
 *
 *******************************************************************************************/
void ResetMaps(int jahrposition, vector<Karten*> &KartenListe, vector<Wetter*> &WetterListe)
{
	// iterate through the density grid
	for (int kartenpos=0; kartenpos< (baeumerows*parameter[0].sizemagnif*baeumecols*parameter[0].sizemagnif); kartenpos++)
		{ 
			pKarten=KartenListe[kartenpos];
			pKarten->baumdichtewert=0;
			pKarten->baumanzahl=0;
		} 
}





/****************************************************************************************//**
 * \brief Calculate simple density value for each tree
 *
 * 
 *
 *
 *
 *******************************************************************************************/
void AddTreeDensity(list<Baum*> &BaumListe, vector<Karten*> &KartenListe)
{

		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);
			
			int i=(int) floor(pBaum->ycoo*parameter[0].sizemagnif);
			int j=(int) floor(pBaum->xcoo*parameter[0].sizemagnif);

			// trees influence the tiles in their environment by a distance that is depending on their basal diameter
			double flaechengroesze=0.0;
			if (parameter[0].calcinfarea == 1) //linearly increasing
			{
				flaechengroesze = pBaum->dbasal * parameter[0].incfac/100.0; // reformat radius to meters: dbasal*parameter[0].incfac/100.0;
			}
			
			// if a tree is only influencing one tile
			if ( flaechengroesze<(1.0/parameter[0].sizemagnif) )
			{
				KartenListe[i*baeumecols*parameter[0].sizemagnif+j]->baumdichtewert += 
										pow(
													pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing)
													//weighing with diameter
													*
													pow(pBaum->dbasal, parameter[0].densitytreetile),
											parameter[0].dichtewertmanipulatorexp);
				KartenListe[i*baeumecols*parameter[0].sizemagnif+j]->baumanzahl++;
				
				pBaum->dichtewert=pow(
								pow(pBaum->dbasal, parameter[0].densitytreetile)*pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing),
								parameter[0].dichtewertmanipulatorexp);
			}
			// ... or if a tree is influencing more than one tile
			else
			{
				// determine the distance the tree influences its surrounding density tiles
				int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );

				double sumdichtewert=0;
				
				// go through grid tiles that are influenced by the tree
				for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
				{
					for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
					{
						if ( ( rastposi<=( (baeumerows-1)*parameter[0].sizemagnif) && rastposi>=0 ) && ( rastposj<=( (baeumecols-1)*parameter[0].sizemagnif) && rastposj>=0 ) )
						{
							// calculation of the density value for the current tile which is decreasing with increasing distance to the centre of the tree
							double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));

							if (entfrastpos<= (double) xyquerrastpos)
							{
								KartenListe[rastposi*baeumecols*parameter[0].sizemagnif+rastposj]->baumdichtewert 
									+= 
									pow(
										pow(pBaum->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
										parameter[0].dichtewertmanipulatorexp);
									
								KartenListe[rastposi*baeumecols*parameter[0].sizemagnif+rastposj]->baumanzahl++;

								sumdichtewert+=pow(
													pow(pBaum->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
													parameter[0].dichtewertmanipulatorexp);

							}
						}
					}
				}

				pBaum->dichtewert=sumdichtewert;
			}

			++pos;
		}
}





/****************************************************************************************//**
 * \brief Update density maps
 *
 * 
 *
 *
 *******************************************************************************************/
void Kartenupdate(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<vector<Karten*> > &WeltKartenListe, vector<list<Baum*> > &WeltBaumListe, vector<vector<Wetter*> > &WeltWetterListe)
{
	int aktort=0;
	
	for (vector<vector<Karten*> >::iterator posw = WeltKartenListe.begin(); posw != WeltKartenListe.end(); ++posw)
	{ 
		vector<Karten*>& KartenListe = *posw;

		vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe.begin()+aktort);
		list<Baum*>& BaumListe = *posiweltb;

		vector<vector<Wetter*> >::iterator posiwelt = (WeltWetterListe.begin()+aktort);
		vector<Wetter*>& WetterListe = *posiwelt;

		aktort++;

		// reset lists
		ResetMaps(jahrposition, KartenListe, WetterListe);

		// add tree density in list
		AddTreeDensity(BaumListe, KartenListe);

		// processing tree density values
		IndividualTreeDensity(BaumListe, KartenListe);
	}
}













