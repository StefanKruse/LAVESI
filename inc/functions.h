double Parameterinput(char *uebergabestring, int stringlengthmax, char *divisionsign, struct Parameter *parameter);
void Weatherinput( struct Parameter *parameter,  int stringlengthmax, vector<vector<Weather*> > &world_weather_list);
void Dataoutput( int t, int jahr, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<Seed*> > &world_seed_list, vector<vector<Weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list, vector<vector<Evaluation*> > &world_evaluation_list);
void Treedistribution( struct Parameter *parameter, int stringlengthmax);
void Environmentupdate( struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<Weather*> > &world_weather_list);
void Growth( struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<vector<Weather*> > &world_weather_list);
void Seeddispersal(int Jahr, struct Parameter *parameter, vector<list<Seed*> > &world_seed_list);
void Seedproduction( struct Parameter *parameter, vector<list<Tree*> > &world_tree_list);
void Treeestablishment( struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<Seed*> > &world_seed_list, vector<vector<Weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list);
void Mortalitaet( struct Parameter *parameter,int Jahr, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<Seed*> > &world_seed_list, vector<vector<Weather*> > &world_weather_list);
void Ageing( struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<Seed*> > &world_seed_list);
void BefrWahrsch(
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
