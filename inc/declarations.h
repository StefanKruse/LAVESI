
// Deklarationen globaler Variablen und Arrays wenn das Programm gestartet wird
	// Raster in entsprechender Groesze deklarieren
	const int treerows=20;	// in metres
	const int treecols=20;

	// Parameter fuer den Startzustand deklarieren und initialisieren
		// Zum Einlesen von Parametern
		const int wortlaengemax=255;
		char trennzeichen[] = "=;";
		struct Parameter parameter[1];
		// Zum Einlesen der weatherdaten

	// Lists and pointers
	vector<list<Tree*> >	 world_tree_list;
	 Tree *pTree;		// Pointer auf einen Tree
	//*pTree=new Tree();
	Tree *pTree_copy;		// Pointer auf einen Tree
	
	vector<list<seed*> >	 world_seed_list(0);
	 seed *pseed;		// Pointer auf einen seed
	seed *pseed_copy;		// Pointer auf einen seed
	
	vector<vector<weather*> > world_weather_list(0);
	 weather *pweather;	// Pointer auf ein weather eines Jahres
	
	// densitykarten
	vector<vector<Karten*> > world_plot_list(0);
	 Karten *pKarten;	// Pointer auf ein Kartenelement im Kartenarray
	Karten *pKarten_copy;	// Pointer auf ein Kartenelement im Kartenarray
	
	// Evaluation/Ausgabe
	vector<vector<Evaluation*> > world_evaluation_list(0);
	 Evaluation *pEvaluation;	// Pointer auf ein Evaluationsobjekt
	Evaluation *pEvaluation_copy;	// Pointer auf ein Evaluationsobjekt

	
		// Listen für resetyear Kopien
		vector<list<Tree*> >	 world_tree_list_copy(0);
		vector<list<seed*> >	 world_seed_list_copy(0);
		vector<vector<Karten*> > world_plot_list_copy(0);
		vector<vector<Evaluation*> > world_evaluation_list_copy(0);
		

			
		//wind data, associated years
		 vector<int> globalyears(0);
		 vector<vector<double> > windspd(0);
		 vector<vector<double> > winddir(0);
		 int cntr;
		 vector<double> wdir(0),wspd(0);   