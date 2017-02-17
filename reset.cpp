/****************************************************************************************//**
 * \brief Backup all information about trees, seeds, maps and analysisvalues
 *
 *
 *
 *
 *******************************************************************************************/
void SaveAllLists(void)
{
	// copy trees list
	int aktort=0;
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe.begin(); posw != WeltBaumListe.end(); ++posw)
	{ 
		list<Baum*>& BaumListe = *posw;

		// open the ..._copy list for the actual location to copy all trees to
		vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe_copy.begin()+aktort);
		list<Baum*>& BaumListe_copy = *posiweltb;
		
		aktort++;

		int countertree=1;
		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);

			// create a clone of the current tree
			pBaum_copy= new Baum();
			pBaum_copy->yweltcoo=pBaum->yweltcoo;
			pBaum_copy->xweltcoo=pBaum->xweltcoo;
			pBaum_copy->xcoo=pBaum->xcoo;		
			pBaum_copy->ycoo=pBaum->ycoo;
			pBaum_copy->name=pBaum->name;
			pBaum_copy->namem=pBaum->namem;
			pBaum_copy->namep=pBaum->namep;
			pBaum_copy->linie=pBaum->linie;			
			pBaum_copy->generation=pBaum->generation;				
			pBaum_copy->etablierungsjahr=pBaum->etablierungsjahr;		
			pBaum_copy->sterbejahr=pBaum->sterbejahr;		
			pBaum_copy->dbasal=pBaum->dbasal;				
			pBaum_copy->dbasalrel=pBaum->dbasalrel;			
			pBaum_copy->dbrust=pBaum->dbrust;
				// copy diameter at breast height list vector<double> Dbrustliste
				for (vector<double>::iterator posdbr=pBaum->Dbrustliste.begin(); posdbr<pBaum->Dbrustliste.end(); )
				{
					pBaum_copy->Dbrustliste.push_back((*posdbr));
					++posdbr;
				}
			pBaum_copy->dbrustrel=pBaum->dbrustrel;		
			pBaum_copy->hoehe=pBaum->hoehe;
			pBaum_copy->alter=pBaum->alter;
			pBaum_copy->zapfen=pBaum->zapfen;
			pBaum_copy->zapfenhoehe=pBaum->zapfenhoehe;
			pBaum_copy->samenneuproduziert=pBaum->samenneuproduziert;
			pBaum_copy->samenproduziert=pBaum->samenproduziert;
			pBaum_copy->dichtewert=pBaum->dichtewert;
			pBaum_copy->longdispersed=pBaum->longdispersed;
			pBaum_copy->growing=pBaum->growing;
			BaumListe_copy.push_back(pBaum_copy);

			++pos;
		}
	}

	// copy seed list
	aktort=0;
	for (vector<list<Samen*> >::iterator posw = WeltSamenListe.begin(); posw != WeltSamenListe.end(); ++posw)
	{ 
		list<Samen*>& SamenListe = *posw;

		// open the ..._copy list for the actual location to copy all seeds to
		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe_copy.begin()+aktort);
		list<Samen*>& SamenListe_copy = *posiwelts;
		
		aktort++;

		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); )
		{ 
			pSamen=(*pos);

			// create a clone of the current seed
			pSamen_copy= new Samen();
			pSamen_copy->xweltcoo=pSamen->xweltcoo;			
			pSamen_copy->yweltcoo=pSamen->yweltcoo;	
			pSamen_copy->xcoo=pSamen->xcoo;			
			pSamen_copy->ycoo=pSamen->ycoo;		
			pSamen_copy->namem=pSamen->namem;			
			pSamen_copy->namep=pSamen->namep;		
			pSamen_copy->linie=pSamen->linie;			
			pSamen_copy->generation=pSamen->generation;	
			pSamen_copy->imzapfen=pSamen->imzapfen;		
			pSamen_copy->gewicht=pSamen->gewicht;	
			pSamen_copy->alter=pSamen->alter;			
			pSamen_copy->longdispersed=pSamen->longdispersed;	
			SamenListe_copy.push_back(pSamen_copy);

			++pos;
		}
	}

	// copy density grid
	aktort=0;
	for (vector<vector<Karten*> >::iterator posw = WeltKartenListe.begin(); posw != WeltKartenListe.end(); ++posw)
	{ 
		vector<Karten*>& KartenListe = *posw;

		// open the ..._copy list for the actual location to copy all density grid tiles to
		vector<vector<Karten*> >::iterator posiweltk = (WeltKartenListe_copy.begin()+aktort);
		vector<Karten*>& KartenListe_copy = *posiweltk;
		
		aktort++;

		for (vector<Karten*>::iterator pos = KartenListe.begin(); pos != KartenListe.end(); )
		{ 
			pKarten=(*pos);

			// create a clone of the current density grid tile
			pKarten_copy= new Karten();
			pKarten_copy->xweltcoo=pKarten->xweltcoo;
			pKarten_copy->yweltcoo=pKarten->yweltcoo;
			pKarten_copy->xcoo=pKarten->xcoo;
			pKarten_copy->ycoo=pKarten->ycoo;
			pKarten_copy->baumdichtewert=pKarten->baumdichtewert;
			pKarten_copy->baumanzahl=pKarten->baumanzahl;
			KartenListe_copy.push_back(pKarten_copy);

			++pos;
		}
	}

	// copy output list
	aktort=0;
	for (vector<vector<Auswertung*> >::iterator posw = WeltAuswertungsListe.begin(); posw != WeltAuswertungsListe.end(); ++posw)
	{ 
		vector<Auswertung*>& AuswertungListe = *posw;

		// open the ..._copy list for the actual location to copy all output lists to
		vector<vector<Auswertung*> >::iterator posiwelta = (WeltAuswertungsListe_copy.begin()+aktort);
		vector<Auswertung*>& AuswertungListe_copy = *posiwelta;

		aktort++;

		for (vector<Auswertung*>::iterator pos = AuswertungListe.begin(); pos != AuswertungListe.end(); )
		{ 
			pAuswertung=(*pos);

			// create a clone of the current output list
			pAuswertung_copy= new Auswertung();
			pAuswertung_copy->xweltcoo=pAuswertung->xweltcoo;
			pAuswertung_copy->yweltcoo=pAuswertung->yweltcoo;
				// copy the basal diameter list vector<double> BAliste
				for (vector<double>::iterator posval=pAuswertung->BAliste.begin(); posval<pAuswertung->BAliste.end(); )
				{
					pAuswertung_copy->BAliste.push_back((*posval));
					++posval;
				}
				// copy the basal diameter mean value list vector<double> BArunmeanliste
				for (vector<double>::iterator posval=pAuswertung->BArunmeanliste.begin(); posval<pAuswertung->BArunmeanliste.end(); )
				{
					pAuswertung_copy->BArunmeanliste.push_back((*posval));
					++posval;
				}
			pAuswertung_copy->maxsteigungbasalarea=pAuswertung->maxsteigungbasalarea;
			pAuswertung_copy->countermaxsteigungbasalarea=pAuswertung->countermaxsteigungbasalarea;
			pAuswertung_copy->wendejahr=pAuswertung->wendejahr;
			pAuswertung_copy->saettigungsjahr=pAuswertung->saettigungsjahr;
			pAuswertung_copy->nachwendejahr=pAuswertung->nachwendejahr;

				// copy the number of seedlings list vector<int> nhoehe0b40liste
				for (vector<int>::iterator posval=pAuswertung->nhoehe0b40liste.begin(); posval<pAuswertung->nhoehe0b40liste.end(); )
				{
					pAuswertung_copy->nhoehe0b40liste.push_back((*posval));
					++posval;
				}
				// copy the mean number of seedlings list vector<double> nhoehe0b40runmeanliste
				for (vector<double>::iterator posval=pAuswertung->nhoehe0b40runmeanliste.begin(); posval<pAuswertung->nhoehe0b40runmeanliste.end(); )
				{
					pAuswertung_copy->nhoehe0b40runmeanliste.push_back((*posval));
					++posval;
				}
				// copy the number of saplings list vector<int> nhoehe41b200liste
				for (vector<int>::iterator posval=pAuswertung->nhoehe41b200liste.begin(); posval<pAuswertung->nhoehe41b200liste.end(); )
				{
					pAuswertung_copy->nhoehe41b200liste.push_back((*posval));
					++posval;
				}
				// copy the mean number of saplings list vector<double> nhoehe41b200runmeanliste
				for (vector<double>::iterator posval=pAuswertung->nhoehe41b200runmeanliste.begin(); posval<pAuswertung->nhoehe41b200runmeanliste.end(); )
				{
					pAuswertung_copy->nhoehe41b200runmeanliste.push_back((*posval));
					++posval;
				}
				// copy the number of trees list vector<int> nhoehe201b10000liste
				for (vector<int>::iterator posval=pAuswertung->nhoehe201b10000liste.begin(); posval<pAuswertung->nhoehe201b10000liste.end(); )
				{
					pAuswertung_copy->nhoehe201b10000liste.push_back((*posval));
					++posval;
				}
				// copy the mean number of trees list vector<double> nhoehe201b10000runmeanliste
				for (vector<double>::iterator posval=pAuswertung->nhoehe201b10000runmeanliste.begin(); posval<pAuswertung->nhoehe201b10000runmeanliste.end(); )
				{
					pAuswertung_copy->nhoehe201b10000runmeanliste.push_back((*posval));
					++posval;
				}

				// copy the diameter at breast height list vector<double> meanbreastdiameterliste
				for (vector<double>::iterator posval=pAuswertung->meanbreastdiameterliste.begin(); posval<pAuswertung->meanbreastdiameterliste.end(); )
				{
					pAuswertung_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				}
				// copy the mean diameter at breast height list vector<double> meanbreastdiameterrunmeanliste
				for (vector<double>::iterator posval=pAuswertung->meanbreastdiameterrunmeanliste.begin(); posval<pAuswertung->meanbreastdiameterrunmeanliste.end(); )
				{
					pAuswertung_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				}

				// copy the stemcount list vector<int> stemcountliste
				for (vector<int>::iterator posval=pAuswertung->stemcountliste.begin(); posval<pAuswertung->stemcountliste.end(); )
				{
					pAuswertung_copy->stemcountliste.push_back((*posval));
					++posval;
				}
				// copy the mean stemcount list vector<int> stemcountrunmeanliste
				for (vector<int>::iterator posval=pAuswertung->stemcountrunmeanliste.begin(); posval<pAuswertung->stemcountrunmeanliste.end(); )
				{
					pAuswertung_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				}

				// copy the tree height list vector<double> meantreeheightliste;
				for (vector<double>::iterator posval=pAuswertung->meantreeheightliste.begin(); posval<pAuswertung->meantreeheightliste.end(); )
				{
					pAuswertung_copy->meantreeheightliste.push_back((*posval));
					++posval;
				}
				// copy the mean tree height list vector<double> meantreeheightrunmeanliste
				for (vector<double>::iterator posval=pAuswertung->meantreeheightrunmeanliste.begin(); posval<pAuswertung->meantreeheightrunmeanliste.end(); )
				{
					pAuswertung_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				}
				// copy the tree age list vector<double> meantreeageliste
				for (vector<double>::iterator posval=pAuswertung->meantreeageliste.begin(); posval<pAuswertung->meantreeageliste.end(); )
				{
					pAuswertung_copy->meantreeageliste.push_back((*posval));
					++posval;
				}
				// copy the mean tree age list vector<double> meantreeagerunmeanliste
				for (vector<double>::iterator posval=pAuswertung->meantreeagerunmeanliste.begin(); posval<pAuswertung->meantreeagerunmeanliste.end(); )
				{
					pAuswertung_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				}
				
			AuswertungListe_copy.push_back(pAuswertung_copy);

			++pos;
		}
	}
}

/****************************************************************************************//**
 * \brief Delete all information about trees, seeds, maps and analysis values from current container
 *
 *
 *
 *
 *******************************************************************************************/
void ClearAllLists(void)
{
	int aktort=0;
	for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); ++posw)
	{
		vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe.begin()+aktort);
		list<Baum*>& BaumListe = *posiweltb;

		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe.begin()+aktort);
		list<Samen*>& SamenListe = *posiwelts;

		vector<vector<Karten*> >::iterator posiweltk = (WeltKartenListe.begin()+aktort);
		vector<Karten*>& KartenListe = *posiweltk;
	
		vector<vector<Auswertung*> >::iterator posiwelt = (WeltAuswertungsListe.begin()+aktort);
		vector<Auswertung*>& AuswertungsListe = *posiwelt;
	
		aktort++;

		// delete tree list
		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); ++pos)
		{
			pBaum=(*pos);
			pBaum->Dbrustliste.clear();
			delete pBaum;
		}
		BaumListe.clear();
		
		// delete seed list
		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); ++pos)
		{
			pSamen=(*pos);
			delete pSamen;
		}
		SamenListe.clear();

		// delete density grid
		for (int kartenpos=0; kartenpos< (baeumerows*parameter[0].sizemagnif*baeumecols*parameter[0].sizemagnif); kartenpos++)
		{
			pKarten=KartenListe[kartenpos];
			delete pKarten;
		}
		KartenListe.clear();

		// delete output list
		for (vector<Auswertung*>::iterator pos = AuswertungsListe.begin(); pos != AuswertungsListe.end(); ++pos)
		{
			pAuswertung=(*pos);
			pAuswertung->BAliste.clear();
			pAuswertung->BArunmeanliste.clear();
			pAuswertung->nhoehe0b40liste.clear();
			pAuswertung->nhoehe0b40runmeanliste.clear();
			pAuswertung->nhoehe41b200liste.clear();
			pAuswertung->nhoehe41b200runmeanliste.clear();
			pAuswertung->nhoehe201b10000liste.clear();
			pAuswertung->nhoehe201b10000runmeanliste.clear();
			pAuswertung->meanbreastdiameterliste.clear();
			pAuswertung->meanbreastdiameterrunmeanliste.clear();
			pAuswertung->stemcountliste.clear();
			pAuswertung->stemcountrunmeanliste.clear();
			pAuswertung->meantreeheightliste.clear();
			pAuswertung->meantreeheightrunmeanliste.clear();
			pAuswertung->meantreeageliste.clear();
			pAuswertung->meantreeagerunmeanliste.clear();
			delete pAuswertung;
		}
		AuswertungsListe.clear();
	}
}

/****************************************************************************************//**
 * \brief Restore all information about trees, seeds, maps and analysisvalues from backup container
 *
 * 
 *
 *
 *******************************************************************************************/
void RestoreAllLists(void)
{

	// restore trees
	int aktort=0;
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe_copy.begin(); posw != WeltBaumListe_copy.end(); ++posw)
	{ 
		list<Baum*>& BaumListe = *posw;

		// open the saved and the current list at the actual location
		vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe.begin()+aktort);
		list<Baum*>& BaumListe_copy = *posiweltb;
		
		aktort++;

		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);

			pBaum_copy= new Baum();
			pBaum_copy->yweltcoo=pBaum->yweltcoo;
			pBaum_copy->xweltcoo=pBaum->xweltcoo;
			pBaum_copy->xcoo=pBaum->xcoo;		
			pBaum_copy->ycoo=pBaum->ycoo;
			pBaum_copy->name=pBaum->name;
			pBaum_copy->namem=pBaum->namem;
			pBaum_copy->namep=pBaum->namep;
			pBaum_copy->linie=pBaum->linie;			
			pBaum_copy->generation=pBaum->generation;				
			pBaum_copy->etablierungsjahr=pBaum->etablierungsjahr;		
			pBaum_copy->sterbejahr=pBaum->sterbejahr;		
			pBaum_copy->dbasal=pBaum->dbasal;				
			pBaum_copy->dbasalrel=pBaum->dbasalrel;			
			pBaum_copy->dbrust=pBaum->dbrust;
				// copy list of breast diameters vector<double> Dbrustliste
				for (vector<double>::iterator posdbr=pBaum->Dbrustliste.begin(); posdbr<pBaum->Dbrustliste.end(); )
				{
					pBaum_copy->Dbrustliste.push_back((*posdbr));
					++posdbr;
				}
			pBaum_copy->dbrustrel=pBaum->dbrustrel;		
			pBaum_copy->hoehe=pBaum->hoehe;
			pBaum_copy->alter=pBaum->alter;
			pBaum_copy->zapfen=pBaum->zapfen;
			pBaum_copy->zapfenhoehe=pBaum->zapfenhoehe;
			pBaum_copy->samenneuproduziert=pBaum->samenneuproduziert;
			pBaum_copy->samenproduziert=pBaum->samenproduziert;
			pBaum_copy->dichtewert=pBaum->dichtewert;
			pBaum_copy->longdispersed=pBaum->longdispersed;
			pBaum_copy->growing=pBaum->growing;
			BaumListe_copy.push_back(pBaum_copy);

			++pos;
		}
	}

	
	// restore seeds
	aktort=0;
	for (vector<list<Samen*> >::iterator posw = WeltSamenListe_copy.begin(); posw != WeltSamenListe_copy.end(); ++posw)
	{ 
		list<Samen*>& SamenListe = *posw;

		// open the saved and the current list at the actual location
		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe.begin()+aktort);
		list<Samen*>& SamenListe_copy = *posiwelts;
		
		aktort++;

		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); )
		{ 
			pSamen=(*pos);

			pSamen_copy= new Samen();
			pSamen_copy->xweltcoo=pSamen->xweltcoo;			
			pSamen_copy->yweltcoo=pSamen->yweltcoo;	
			pSamen_copy->xcoo=pSamen->xcoo;			
			pSamen_copy->ycoo=pSamen->ycoo;		
			pSamen_copy->namem=pSamen->namem;			
			pSamen_copy->namep=pSamen->namep;		
			pSamen_copy->linie=pSamen->linie;			
			pSamen_copy->generation=pSamen->generation;	
			pSamen_copy->imzapfen=pSamen->imzapfen;		
			pSamen_copy->gewicht=pSamen->gewicht;	
			pSamen_copy->alter=pSamen->alter;			
			pSamen_copy->longdispersed=pSamen->longdispersed;	
			SamenListe_copy.push_back(pSamen_copy);

			++pos;
		}
	}

		
	// restore density grid
	aktort=0;
	for (vector<vector<Karten*> >::iterator posw = WeltKartenListe_copy.begin(); posw != WeltKartenListe_copy.end(); ++posw)
	{ 
		vector<Karten*>& KartenListe = *posw;

		// open the saved and the current list at the actual location
		vector<vector<Karten*> >::iterator posiweltk = (WeltKartenListe.begin()+aktort);
		vector<Karten*>& KartenListe_copy = *posiweltk;
		
		aktort++;

		for (vector<Karten*>::iterator pos = KartenListe.begin(); pos != KartenListe.end(); )
		{ 
			pKarten=(*pos);

			pKarten_copy= new Karten();	
			pKarten_copy->xweltcoo=pKarten->xweltcoo;
			pKarten_copy->yweltcoo=pKarten->yweltcoo;
			pKarten_copy->xcoo=pKarten->xcoo;
			pKarten_copy->ycoo=pKarten->ycoo;
			pKarten_copy->baumdichtewert=pKarten->baumdichtewert;
			pKarten_copy->baumanzahl=pKarten->baumanzahl;
			KartenListe_copy.push_back(pKarten_copy);

			++pos;
		}
	}

	
	// restore output list
	aktort=0;
	for (vector<vector<Auswertung*> >::iterator posw = WeltAuswertungsListe_copy.begin(); posw != WeltAuswertungsListe_copy.end(); ++posw)
	{ 
		vector<Auswertung*>& AuswertungListe = *posw;

		// open the saved and the current list at the actual location
		vector<vector<Auswertung*> >::iterator posiwelta = (WeltAuswertungsListe.begin()+aktort);
		vector<Auswertung*>& AuswertungListe_copy = *posiwelta;
		
		aktort++;

		for (vector<Auswertung*>::iterator pos = AuswertungListe.begin(); pos != AuswertungListe.end(); )
		{ 
			pAuswertung=(*pos);

			pAuswertung_copy= new Auswertung();
			pAuswertung_copy->xweltcoo=pAuswertung->xweltcoo;
			pAuswertung_copy->yweltcoo=pAuswertung->yweltcoo;
				for (vector<double>::iterator posval=pAuswertung->BAliste.begin(); posval<pAuswertung->BAliste.end(); )
				{
					pAuswertung_copy->BAliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->BArunmeanliste.begin(); posval<pAuswertung->BArunmeanliste.end(); )
				{
					pAuswertung_copy->BArunmeanliste.push_back((*posval));
					++posval;
				}
			pAuswertung_copy->maxsteigungbasalarea=pAuswertung->maxsteigungbasalarea;
			pAuswertung_copy->countermaxsteigungbasalarea=pAuswertung->countermaxsteigungbasalarea;
			pAuswertung_copy->wendejahr=pAuswertung->wendejahr;	
			pAuswertung_copy->saettigungsjahr=pAuswertung->saettigungsjahr;
			pAuswertung_copy->nachwendejahr=pAuswertung->nachwendejahr;
				for (vector<int>::iterator posval=pAuswertung->nhoehe0b40liste.begin(); posval<pAuswertung->nhoehe0b40liste.end(); )
				{
					pAuswertung_copy->nhoehe0b40liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->nhoehe0b40runmeanliste.begin(); posval<pAuswertung->nhoehe0b40runmeanliste.end(); )
				{
					pAuswertung_copy->nhoehe0b40runmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pAuswertung->nhoehe41b200liste.begin(); posval<pAuswertung->nhoehe41b200liste.end(); )
				{
					pAuswertung_copy->nhoehe41b200liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->nhoehe41b200runmeanliste.begin(); posval<pAuswertung->nhoehe41b200runmeanliste.end(); )
				{
					pAuswertung_copy->nhoehe41b200runmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pAuswertung->nhoehe201b10000liste.begin(); posval<pAuswertung->nhoehe201b10000liste.end(); )
				{
					pAuswertung_copy->nhoehe201b10000liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->nhoehe201b10000runmeanliste.begin(); posval<pAuswertung->nhoehe201b10000runmeanliste.end(); )
				{
					pAuswertung_copy->nhoehe201b10000runmeanliste.push_back((*posval));
					++posval;
				}

				for (vector<double>::iterator posval=pAuswertung->meanbreastdiameterliste.begin(); posval<pAuswertung->meanbreastdiameterliste.end(); )
				{
					pAuswertung_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->meanbreastdiameterrunmeanliste.begin(); posval<pAuswertung->meanbreastdiameterrunmeanliste.end(); )
				{
					pAuswertung_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				}

				for (vector<int>::iterator posval=pAuswertung->stemcountliste.begin(); posval<pAuswertung->stemcountliste.end(); )
				{
					pAuswertung_copy->stemcountliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pAuswertung->stemcountrunmeanliste.begin(); posval<pAuswertung->stemcountrunmeanliste.end(); )
				{
					pAuswertung_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				}

				for (vector<double>::iterator posval=pAuswertung->meantreeheightliste.begin(); posval<pAuswertung->meantreeheightliste.end(); )
				{
					pAuswertung_copy->meantreeheightliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->meantreeheightrunmeanliste.begin(); posval<pAuswertung->meantreeheightrunmeanliste.end(); )
				{
					pAuswertung_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->meantreeageliste.begin(); posval<pAuswertung->meantreeageliste.end(); )
				{
					pAuswertung_copy->meantreeageliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pAuswertung->meantreeagerunmeanliste.begin(); posval<pAuswertung->meantreeagerunmeanliste.end(); )
				{
					pAuswertung_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				}
			AuswertungListe_copy.push_back(pAuswertung_copy);

			++pos;
		}
	}
}

/****************************************************************************************//**
 * \brief Delete all information about trees, seeds, maps and analysisvalues from current container
 * 
 *
 *
 *
 *******************************************************************************************/
void ClearAllLists_copy(void)
{

	cout << endl << "ClearAllLists_copy started!";

	int aktort=0;
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe_copy.begin(); posw != WeltBaumListe_copy.end(); ++posw)
	{ 
		list<Baum*>& BaumListe = *posw;

		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe_copy.begin()+aktort);
		list<Samen*>& SamenListe = *posiwelts;

		vector<vector<Karten*> >::iterator posiweltk = (WeltKartenListe_copy.begin()+aktort);
		vector<Karten*>& KartenListe = *posiweltk;
	
		vector<vector<Auswertung*> >::iterator posiwelt = (WeltAuswertungsListe_copy.begin()+aktort);
		vector<Auswertung*>& AuswertungsListe = *posiwelt;
	
		aktort++;

		// empty tree list
		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); ++pos)
		{
			pBaum=(*pos);
			pBaum->Dbrustliste.clear();
			delete pBaum;
		}
		BaumListe.clear();
		
		// empty seed list
		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); ++pos)
		{
			pSamen=(*pos);
			delete pSamen;
		}
		SamenListe.clear();

		// empty density grid
		for (int kartenpos=0; kartenpos< (baeumerows*parameter[0].sizemagnif*baeumecols*parameter[0].sizemagnif); kartenpos++)
		{
			pKarten=KartenListe[kartenpos];
			delete pKarten;
		}
		KartenListe.clear();
		
		// empty output list
		for (vector<Auswertung*>::iterator pos = AuswertungsListe.begin(); pos != AuswertungsListe.end(); ++pos)
		{
			pAuswertung=(*pos);
			pAuswertung->BAliste.clear();
			pAuswertung->BArunmeanliste.clear();
			pAuswertung->nhoehe0b40liste.clear();
			pAuswertung->nhoehe0b40runmeanliste.clear();
			pAuswertung->nhoehe41b200liste.clear();
			pAuswertung->nhoehe41b200runmeanliste.clear();
			pAuswertung->nhoehe201b10000liste.clear();
			pAuswertung->nhoehe201b10000runmeanliste.clear();
			pAuswertung->meanbreastdiameterliste.clear();
			pAuswertung->meanbreastdiameterrunmeanliste.clear();
			pAuswertung->stemcountliste.clear();
			pAuswertung->stemcountrunmeanliste.clear();
			pAuswertung->meantreeheightliste.clear();
			pAuswertung->meantreeheightrunmeanliste.clear();
			pAuswertung->meantreeageliste.clear();
			pAuswertung->meantreeagerunmeanliste.clear();
			delete pAuswertung;
		}
		AuswertungsListe.clear();
	}
}

