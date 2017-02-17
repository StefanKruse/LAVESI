/****************************************************************************************//**
 * \brief Calculate tree mortality
 *
 * depends on abiotic factors (temperature and vegetation length)
 * and on biotic factors
 *
 *
 *******************************************************************************************/
void TreeMort(int jahrposition_help,vector<Wetter*> &WetterListe,list<Baum*> &BaumListe)
{
		// ABIOTIC FACTORS
		// declaration of variables
		double janisothermrestriktion=0;
		double julisothermrestriktion=0;
		double nddrestriktion=0;

		// evaluation of mean January temperature
		if (WetterListe[jahrposition_help]->temp1monatmitteliso<(-40.0))
		{
			janisothermrestriktion=1.0;
		}
		else
		{
			janisothermrestriktion=1.0-fabs(8.0*(WetterListe[jahrposition_help]->temp1monatmitteliso+40.0)/40.0);
		}

		// evaluation of mean July temperature
		if (WetterListe[jahrposition_help]->temp7monatmitteliso<10.0) 
		{
			julisothermrestriktion=1.0;
		}
		else 
		{
			julisothermrestriktion=1.0-fabs(2.0*(WetterListe[jahrposition_help]->temp7monatmitteliso-10.0)/10.0);
		}

		// evaluation of the vegetation period length
		if (WetterListe[jahrposition_help]->vegetationslaengeiso<60)
		{
			nddrestriktion=1.0; 
		}
		else 
		{
			nddrestriktion=1.0-fabs(( ((double) WetterListe[jahrposition_help]->vegetationslaengeiso)-60.0)/60.0);
		}
	
		// calculation of the addition of mortality rate due to climate factors
		double anstiegwettermort=160;
		anstiegwettermort-=(60*janisothermrestriktion+60*julisothermrestriktion+60*nddrestriktion);
		double wettermortadd = 1.0  -(1.0 / (1.0 + (((1.0 - 0.5) / 0.5) * exp(-anstiegwettermort * WetterListe[jahrposition_help]->maxbasalwachstumjahr))));

		
		
		// BIOTIC FACTORS
		double maxh;
		double maxhpuf=WetterListe[jahrposition_help]->maxbasalwachstumjahr;

		// estimation of the maximum growth for small individuals later used in the determination of the youth mortality
		if (maxhpuf<=0.0)
		{
			if (parameter[0].allometriefunktionstyp==1)
			{
				maxh= parameter[0].dbasalhoeheallosteig * pow(parameter[0].gdbasal, parameter[0].dbasalhoehealloexp);
			}
			else 
			{
				maxh= parameter[0].dbasalhoehesteignonlin*parameter[0].gdbasal;	
			}
		}
		else 
		{
			if (parameter[0].allometriefunktionstyp==1)
			{
				maxh= parameter[0].dbasalhoeheallosteig * pow(WetterListe[jahrposition_help]->maxbasalwachstumjahr, parameter[0].dbasalhoehealloexp);
			}
			else 
			{
				maxh= parameter[0].dbasalhoehesteignonlin*WetterListe[jahrposition_help]->maxbasalwachstumjahr;
			}
		}

		
		// MORTALITY RATE ESTIMATION
		// estimate the actual mortality rate for each tree in the tree list
		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);
	
			if(pBaum->growing==true)
			{
				// if a tree exceeds the observed maximum tree age for the species its mortality is increased
				double altersmort=0.0;
				if (pBaum->alter>parameter[0].hoechstalter) 
				{
					altersmort=1.0;
				}
		
				// preparation for growth-dependent influences
				double wachstumrel=1.0;
				if (pBaum->hoehe<130.0) 
				{
					wachstumrel=pBaum->dbasalrel;
				}
				else 
				{
					wachstumrel=pBaum->dbrustrel;
				}

				// if specified trees within a certain height range experience a higher mortality rate
				double hoehenkugeleinfluss=1;
				if (pBaum->hoehe<(parameter[0].dichtewertmaximumbeihoehe*2))
				{
					hoehenkugeleinfluss=hoehenkugeleinfluss+(sqrt(pow(parameter[0].dichtewertmaximumbeihoehe,2)-pow(pBaum->hoehe-parameter[0].dichtewertmaximumbeihoehe, 2))/parameter[0].dichtewertmaximumbeihoehe);
				}

				// declaration and calculation of the different parts that form together the mortality rate of a tree
				double sapl_mort	= parameter[0].mjung * pow(exp((-1.0 * pBaum->hoehe) + maxh), parameter[0].jugendmorteinflussexp);
				double age_mort		= parameter[0].malt	* altersmort * (10.0 * parameter[0].mortbg);
				double growth_mort	= parameter[0].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relwachstummorteinflussexp));
				double dens_mort	= parameter[0].mdichte * hoehenkugeleinfluss * pBaum->dichtewert;
				double weather_mort	= parameter[0].mwetter * wettermortadd * pow((1.0 / pBaum->hoehe), parameter[0].hoehewettermorteinflussexp);
				double dry_mort		= parameter[0].mtrockenheit * WetterListe[jahrposition_help]->trockenheitsmort * pow((1.0 / pBaum->hoehe), 0.5);

				double baummort = 0.0 
								+ parameter[0].mortbg
								+ sapl_mort
								+ age_mort
								+ growth_mort 
								+ dens_mort
								+ weather_mort 
								+ dry_mort;
				
				// make sure the tree mortality rate is between 0 and 1
				if (baummort>1.0)
				{
					baummort = 1.0;
				}
				else if (baummort<0.0) 
				{
					baummort = 0.0;
				}

				
				if (parameter[0].mortanzeige==true && pBaum->hoehe>200.0) 
				{
						printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => YOUTH=%4.3f +AGED=%4.2f +GROWTH=%4.2f +DENSITY=%4.4f +WEATHER=%4.3f +DROUGHT=%4.4f =>%4.2f", 
						pBaum->hoehe, 
						pBaum->dbasalrel,pBaum->dbrustrel,
						parameter[0].mjung*pow(exp((-1.0*pBaum->hoehe)+maxh),	parameter[0].jugendmorteinflussexp), 
						parameter[0].malt*(altersmort*(10.0*parameter[0].mortbg)), 
						+ (1.0-pow(wachstumrel,parameter[0].relwachstummorteinflussexp)),
						parameter[0].mdichte*hoehenkugeleinfluss*pBaum->dichtewert, 
						parameter[0].mwetter*wettermortadd*pow((1.0/pBaum->hoehe),0.5), 
						parameter[0].mtrockenheit*(WetterListe[jahrposition_help]->trockenheitsmort*pow((1.0/pBaum->hoehe),0.5)), 
						baummort);
				}

				// evaluate if a tree dies and if so delete it
				double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
				if (zufallsz<baummort) 
				{
					vector<double>().swap(pBaum->Dbrustliste);
					delete pBaum;
					pos=BaumListe.erase(pos);
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
}




/****************************************************************************************//**
 * \brief Calculate tree and seed mortality rate
 *
 * first call TreeMort() then calculate seeds mortality rates
 *
 *
 *******************************************************************************************/
void Mortalitaet(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe)
{
	int aktort=0;
	
	for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); ++posw)
	{ 
		vector<Wetter*>& WetterListe = *posw;

		vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe.begin()+aktort);
		list<Baum*>& BaumListe = *posiweltb;

		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe.begin()+aktort);
		list<Samen*>& SamenListe = *posiwelts;

		aktort++;

		// mortality evaluation for trees
		TreeMort(jahrposition, WetterListe, BaumListe);

		// mortality evaluation for seeds
		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); )
		{ 
			pSamen=(*pos);
			
			// determine stochastically if a seed dies
			// ... draw a random number
			double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
			
			// ... either for seeds on the ground
			if (pSamen->imzapfen==false)
			{
				if (zufallsz<parameter[0].samenbodenmort) 
				{
					delete pSamen;
					pos=SamenListe.erase(pos);
				}
				else
				{
					++pos;
				}
			}
			// ... or for seeds in the cones of trees
			else if (pSamen->imzapfen==true)
			{
				if (zufallsz<parameter[0].samenbaummort) 
				{
					delete pSamen;
					pos=SamenListe.erase(pos);
				}
				else
				{
					++pos;
				}
			}
			// ... security check
			else
			{
				printf("\nLAVESI exited\n"); 
				printf("... in mortalitaet.cpp\n"); 
				printf("... pSamen->imzapfen not valid of the current tree\n"); 
			} 
		} 
	}
}

