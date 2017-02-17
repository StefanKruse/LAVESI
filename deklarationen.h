// declaration of global variables and arrays

	// determine plot area size in metres
	const int baeumerows=100;
	const int baeumecols=100;

	// for parameter read in
	const int wortlaengemax=255;
	char trennzeichen[] = "=;";
	struct Parameter parameter[1];
		
	// lists
	// ... trees, seeds and weather
	vector<list<Baum*> >	 WeltBaumListe;
	Baum *pBaum;		// pointer for each tree
	Baum *pBaum_copy;
	vector<list<Samen*> >	 WeltSamenListe;
	Samen *pSamen;		// pointer for each seed
	Samen *pSamen_copy;
	vector<vector<Wetter*> > WeltWetterListe;
	Wetter *pWetter;	// pointer for each weather data of each year
	
	// ... density map
	vector<vector<Karten*> > WeltKartenListe;
	Karten *pKarten;	// pointer for each sub-grid element
	Karten *pKarten_copy;
	
	// ... output
	vector<vector<Auswertung*> > WeltAuswertungsListe;
	Auswertung *pAuswertung;	// pointer for each output element
	Auswertung *pAuswertung_copy;

	
	// lists for resetyear copies
	vector<list<Baum*> >	 WeltBaumListe_copy;
	vector<list<Samen*> >	 WeltSamenListe_copy;
	vector<vector<Karten*> > WeltKartenListe_copy;
	vector<vector<Auswertung*> > WeltAuswertungsListe_copy;

	