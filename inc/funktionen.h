// Grundfunktionen
 double NormverteilungRN(float mean, float sdev);
 double Parametereingabe(char *uebergabestring, int wortlaengemax, char *trennzeichen, struct Parameter *parameter);
 void weathereinlesen( struct Parameter *parameter,  int wortlaengemax, vector<vector<weather*> > &world_weather_list);
 void Anzeige( vector<Tree*> &tree_list, vector<seed*> &seed_list);
 void Data_output( int t, int jahr, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list, vector<vector<Evaluation*> > &world_evaluation_list);

// Spezielle Funktionen
	// Treeverteilung aus Datei einlesen oder seedeintrag bei gutem weather
 void Treeverteilung( struct Parameter *parameter, int wortlaengemax, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list);
 void Kartenupdate( struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list);
 void Fire( struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<vector<weather*> > &world_weather_list);
	// Jahresschritt
 void Wachstum( struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list);
 void seedausbreitung(int Jahr, struct Parameter *parameter, vector<list<seed*> > &world_seed_list);
 void seedproduktion( struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list,vector<vector<weather*> > &world_weather_list, int yrpos);
 void Etablierung( struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list);
 void Mortalitaet( struct Parameter *parameter,int Jahr, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list);
 void Ageing( struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list);

 void BefrWahrsch(	//double a , double b,struct Parameter *parameter, vector<list<Tree*> >::iterator f, int e, vector<int> &d,vector<double> &thawingdepthinher);
 
					double x, double y,struct Parameter *parameter, vector<list<Tree*> >::iterator world_positon_b, 
				double richtung,double geschwindigkeit,unsigned int ripm,unsigned int cntr,double p,double kappa,double phi,double dr,double dx,double dy,double I0kappa,double pe,double C,double m,
			vector<int> &pName, vector<double>  &thdpthinfl,
			int outputtreesiter
			);
 void seeddisp(double ad, double& cd, double& dd, double &vv, double &vd, double parhei, int seedspec);
 
 void Parametereinlesen(void);
 void SaveAllLists(void);
 void ClearAllLists(void);
 void RestoreAllLists(void);
 void ClearAllLists_copy(void);