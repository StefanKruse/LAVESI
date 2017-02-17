/****************************************************************************************//**
 * \brief Calculate max. possible basal growth
 *
 * either just set to WetterListe[jahrposition]->maxbasalwachstumjahr (trees <130 cm)
 * or WetterListe[jahrposition]->maxbasalsaplwachstumjahr (trees >130 cm)\n
 * or if transekts are calculated calculated out of WetterListe[jahrposition]->maxbasalwachstumjahrmin 
 * (max basal growth at most southern transekt) and tree position in north-south transekt
 *
 *******************************************************************************************/
double getMaxbasalwachstum(int jahrposition, list<Baum*> &BaumListe, vector<Wetter*> &WetterListe)
{
	double maxbw_help;
	if (parameter[0].lineartransekt==true)
	{
		if (pBaum->hoehe<130.0)
		{
			maxbw_help=((WetterListe[jahrposition]->maxbasalwachstumjahr
								-WetterListe[jahrposition]->maxbasalwachstumjahrmin)
								/((double) baeumerows))*pBaum->ycoo 
							+WetterListe[jahrposition]->maxbasalwachstumjahrmin;
		}				
		else
		{
			maxbw_help=((WetterListe[jahrposition]->maxbasalsaplwachstumjahr
								-WetterListe[jahrposition]->maxbasalsaplwachstumjahrmin)
								/((double) baeumerows))*pBaum->ycoo 
							+WetterListe[jahrposition]->maxbasalwachstumjahrmin;
		}
	}
	else
	{
		if (pBaum->hoehe<130.0)
		{
			maxbw_help= WetterListe[jahrposition]->maxbasalwachstumjahr;
		}
		else
		{
			maxbw_help = WetterListe[jahrposition]->maxbasalsaplwachstumjahr;
		}
	}

	return (maxbw_help);
}

/****************************************************************************************//**
 * \brief Calculate min. possible basal growth
 *
 * either taken from just set to WetterListe[jahrposition]->maxbrustwachstumjahr \n
 * or if transekts are calculated calculated out of WetterListe[jahrposition]->maxbasalwachstumjahrmin 
 * and tree position in north-south transekt
 *
 *******************************************************************************************/
double getMaxbrustwachstum(int jahrposition, list<Baum*> &BaumListe, vector<Wetter*> &WetterListe)
{	
	double maxbrw_help;
	
	if (parameter[0].lineartransekt==true)
	{
			maxbrw_help = ((WetterListe[jahrposition]->maxbrustwachstumjahr
									-WetterListe[jahrposition]->maxbrustwachstumjahrmin)
									/((double) baeumerows))
								*pBaum->ycoo 
								+WetterListe[jahrposition]->maxbasalwachstumjahrmin;
	}
	else
	{
		maxbrw_help = WetterListe[jahrposition]->maxbrustwachstumjahr;
	}

	return(maxbrw_help);
}

/****************************************************************************************//**
 * \brief Calculate basal and breast height growth of each tree in the simulation
 *
 *
 *
 *
 *******************************************************************************************/
void Wachstum(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe)
{
	int aktort=0;
	
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe.begin(); posw != WeltBaumListe.end(); ++posw)
	{ 
		list<Baum*>& BaumListe = *posw;
	
		vector<vector<Wetter*> >::iterator posiwelt = (WeltWetterListe.begin()+aktort);
		vector<Wetter*>& WetterListe = *posiwelt;

		aktort++;

		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);

			// declare the basal growth variable
			double maxbasalwachstum=0;

			// evaluate the maximum growth of the current year
			maxbasalwachstum =getMaxbasalwachstum(jahrposition, BaumListe, WetterListe);
			
			// calculate the current trees growth
			double basalwachstum = maxbasalwachstum	* (1.0-pBaum->dichtewert);
			basalwachstum  = basalwachstum
							+basalwachstum*parameter[0].basaleinflussaltneu
							*pBaum->dbasal;

			// make sure the value is within a valid range
			if (basalwachstum<0.0)
			{
				basalwachstum=0.0;
			}
			
			if (pBaum->growing==true)
			{
				pBaum->dbasal+= basalwachstum;
			}

			// further processing of the diameter growth
			if (parameter[0].relwachstumeinfluss==0)
			{
				pBaum->dbasalrel = 1.0;
			}
			else if (parameter[0].relwachstumeinfluss==1)
			{
				pBaum->dbasalrel = basalwachstum
									/(maxbasalwachstum
									  +(maxbasalwachstum
									*parameter[0].basaleinflussaltneu
									*pBaum->dbasal));
			}


			// evaluation of the breast diameter growth if a tree is already at that height
			double maxbrustwachstum=0;
			double brustwachstum=0;
			if (pBaum->hoehe>=130.0)
			{ 
				maxbrustwachstum = getMaxbrustwachstum(jahrposition, BaumListe, WetterListe);

				brustwachstum = maxbrustwachstum * (1.0-pBaum->dichtewert);
				
				if (brustwachstum<0.0)
				{
					brustwachstum=0.0;
				}
				
				if (pBaum->growing==true)
				{
					pBaum->dbrust+= brustwachstum;
					pBaum->Dbrustliste.push_back(brustwachstum);
				}

				if (parameter[0].relwachstumeinfluss==0)
				{
					pBaum->dbrustrel=1.0;
				}		
				else if (parameter[0].relwachstumeinfluss==1)
				{
					pBaum->dbrustrel=brustwachstum/maxbrustwachstum;
				}
			} 

			// update the tree height
			if (pBaum->hoehe<130.0)
			{
				if (parameter[0].allometriefunktionstyp==1)
				{
					pBaum->hoehe= parameter[0].dbasalhoeheallosteig * pow(pBaum->dbasal, parameter[0].dbasalhoehealloexp);
				}
				else 
				{
					pBaum->hoehe= parameter[0].dbasalhoehesteignonlin*pBaum->dbasal;
				}
			}
			else if (pBaum->hoehe>=130.0)
			{
				if (parameter[0].allometriefunktionstyp==1)
				{
					pBaum->hoehe= parameter[0].dbrusthoeheallosteig * pow(pBaum->dbrust, parameter[0].dbrusthoehealloexp) + 130.0;
				}
				else 
				{
					pBaum->hoehe= pow( (parameter[0].dbrusthoehesteignonlin*pow(pBaum->dbrust,0.5)), 2)+130.0; 
				}
			}
			// security check
			else
			{
				printf("\n LAVESI exited\n"); 
				printf("\n ... cause: a tree has no valid height value pBaum->hoehe\n"); 
				exit(1);				
			} 

			++pos;
		}
	}
}
















