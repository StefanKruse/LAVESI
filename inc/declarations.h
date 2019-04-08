#pragma once

	// plot dimensions rows==y, cols==x
 	const int treerows=200000;
 	const int treecols=20;

	// parameter input
 	const int stringlengthmax=255;
 	char divisionsign[] = "=;";
 	struct Parameter parameter[1];
	
	// lists and pointers
  	vector<list<Tree*>> world_tree_list;
  	Tree *pTree;
  	Tree *pTree_copy;
	
  	vector<list<Seed*>> world_seed_list(0);
  	Seed *pSeed;
  	Seed *pSeed_copy;
	
  	vector<vector<Weather*>> world_weather_list(0);
  	Weather *pWeather;
	
  	vector<vector<Envirgrid*>> world_plot_list(0);
  	Envirgrid *pEnvirgrid;
  	Envirgrid *pEnvirgrid_copy;
	
  	vector<vector<Evaluation*> > world_evaluation_list(0);
  	Evaluation *pEvaluation;
  	Evaluation *pEvaluation_copy;

	// lists for resetyear copies
	vector<list<Tree*>>	world_tree_list_copy(0);
	vector<list<Seed*>> world_seed_list_copy(0);
	vector<vector<Envirgrid*>> world_plot_list_copy(0);
	vector<vector<Evaluation*>> world_evaluation_list_copy(0);
		
	// wind data
	vector<int> globalyears(0);
	vector<vector<double>> windspd(0);
	vector<vector<double>> winddir(0);
	int cntr;
	vector<double> wdir(0),wspd(0);   
