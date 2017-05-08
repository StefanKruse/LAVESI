// Grundfunktionen
 double NormverteilungRN(float mean, float sdev);
 double Parametereingabe(char *uebergabestring, int wortlaengemax, char *trennzeichen, struct Parameter *parameter);
 void weathereinlesen(int treerows, struct Parameter *parameter,  int wortlaengemax, vector<vector<weather*> > &world_weather_list);
 void Anzeige(int treerows, int treecols, vector<Tree*> &tree_list, vector<seed*> &seed_list);
 void Data_output(int treerows, int treecols, int t, int jahr, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list, vector<vector<Evaluation*> > &world_evaluation_list);

// Spezielle Funktionen
	// Treeverteilung aus Datei einlesen oder seedeintrag bei gutem weather
 void Treeverteilung(int treerows, int treecols, struct Parameter *parameter, int wortlaengemax, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list,vector<vector<double>> &vegetationtype);
 void Kartenupdate(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list);
 void Fire(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<vector<weather*> > &world_weather_list);
	// Jahresschritt
 void Wachstum(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list);
 void seedausbreitung(int treerows, int treecols,int Jahr,int yrpos, struct Parameter *parameter, vector<list<seed*> > &world_seed_list);
 void seedproduktion(int treerows, int treecols, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list,vector<vector<weather*> > &world_weather_list, int yrpos);
 void Etablierung(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list);
 void Mortalitaet(int treerows, int treecols, struct Parameter *parameter,int Jahr, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list);
 void Ageing(int treerows, int treecols, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list);

 void BefrWahrsch(double a , double b,struct Parameter *parameter, vector<list<Tree*> >::iterator f, int e, vector<int> &d);
 
 void seeddisp(double ad, int bd, double& cd, double& dd);
 
 void Parametereinlesen(void);
 void SaveAllLists(void);
 void ClearAllLists(void);
 void RestoreAllLists(void);
 void ClearAllLists_copy(void);
 
 
 void influx(int treerows, int treecols,int year, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<vector<double>> &vegtype);

