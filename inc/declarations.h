
// Deklarationen globaler Variablen und Arrays wenn das Programm gestartet wird
	// Raster in entsprechender Groesze deklarieren
	const int treerows=250000;	// in metres
	const int treecols=100;

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
	
	vector<list<seed*> >	 world_seed_list;
	 seed *pseed;		// Pointer auf einen seed
	seed *pseed_copy;		// Pointer auf einen seed
	
	vector<vector<weather*> > world_weather_list;
	 weather *pweather;	// Pointer auf ein weather eines Jahres
	
	// densitykarten
	vector<vector<Karten*> > world_plot_list;
	 Karten *pKarten;	// Pointer auf ein Kartenelement im Kartenarray
	Karten *pKarten_copy;	// Pointer auf ein Kartenelement im Kartenarray
	
	// Evaluation/Ausgabe
	vector<vector<Evaluation*> > world_evaluation_list;
	 Evaluation *pEvaluation;	// Pointer auf ein Evaluationsobjekt
	Evaluation *pEvaluation_copy;	// Pointer auf ein Evaluationsobjekt

	
		// Listen für resetyear Kopien
		vector<list<Tree*> >	 world_tree_list_copy;
		vector<list<seed*> >	 world_seed_list_copy;
		vector<vector<Karten*> > world_plot_list_copy;
		vector<vector<Evaluation*> > world_evaluation_list_copy;
		
		// yearly pollen influx rate
		//list<double> influx;
		//lists are faster w push.back
			
		//wind data, associated years
		 vector<int> globalyears;
		 vector<vector<double> > windspd;
		 vector<vector<double> > winddir;
		 

		// Umweltdaten/Vegetationstypen:
		std::vector<std::vector<double> > vegetationtype;//( treecols , vector<double>( treerows ) );
		//double vegetationtype[treecols][treerows];
			//vectors are faster for iteration


