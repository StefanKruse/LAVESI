using namespace std;

// reset of all lists to year of choice "resetyear"

/****************************************************************************************//**
 * \brief 
 *
 * backup all information about trees, seeds, maps and analysis values
 *
 *
 *
 *******************************************************************************************/
void SaveAllLists(void)
{

	// copy Trees
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;

		// opening the original tree list and the referring reset list ..._copy
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list_copy.begin()+aktort);
		list<Tree*>& tree_list_copy = *world_positon_b;
		
		
		aktort++;

		cout << endl;
		cout << "tree_list.size()=" << tree_list.size() << endl;
		cout << "tree_list_copy.size() before copying =" << tree_list_copy.size() << endl;
		
		int countertree=1;
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pTree_copy= new Tree();						// create new tree
			pTree_copy->yworldcoo=pTree->yworldcoo;
			pTree_copy->xworldcoo=pTree->xworldcoo;
			pTree_copy->xcoo=pTree->xcoo;		
			pTree_copy->ycoo=pTree->ycoo;
			pTree_copy->name=pTree->name;
			pTree_copy->namem=pTree->namem;
			pTree_copy->namep=pTree->namep;
			pTree_copy->line=pTree->line;		
			pTree_copy->generation=pTree->generation;
			pTree_copy->species=pTree->species;
			pTree_copy->yr_of_establishment=pTree->yr_of_establishment;		
			pTree_copy->yr_of_dying=pTree->yr_of_dying;		
			pTree_copy->dbasal=pTree->dbasal;				
			pTree_copy->dbasalrel=pTree->dbasalrel;			
			pTree_copy->dbrust=pTree->dbrust;
			pTree_copy->dbrustrel=pTree->dbrustrel;		
			pTree_copy->height=pTree->height;
			pTree_copy->age=pTree->age;
			pTree_copy->cone=pTree->cone;
			pTree_copy->coneheight=pTree->coneheight;
			pTree_copy->seednewly_produced=pTree->seednewly_produced;
			pTree_copy->seedproduced=pTree->seedproduced;
			pTree_copy->speicher=pTree->speicher;
			pTree_copy->densitywert=pTree->densitywert;
			pTree_copy->thawing_depthinfluence=pTree->thawing_depthinfluence;
			pTree_copy->longdispersed=pTree->longdispersed;
			pTree_copy->growing=pTree->growing;
				
			tree_list_copy.push_back(pTree_copy);				// push back tree to list

			
				if(countertree==1)
				{
					cout << pTree->height << " <-pTree ... Treeheight ... pTree_copy-> " << pTree_copy->height << endl;
				}

			++pos;
			++countertree;
		}
		
		cout << "tree_list_copy.size() after copying=" << tree_list_copy.size() << endl;
		cout << endl;
		cout << endl;
	}

	
	// copy Seeds
	aktort=0;
	for (vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<Seed*>& seed_list = *posw;

		// opening the original seed list and the referring reset list ..._copy		
		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<Seed*>& seed_list_copy = *world_positon_s;
		
		
		aktort++;

		cout << endl;
		cout << "seed_list.size()=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() before copying=" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// For displaying the first seed
		for (list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pSeed=(*pos);

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pSeed_copy= new Seed();						// create new seed
			pSeed_copy->xworldcoo=pSeed->xworldcoo;			
			pSeed_copy->yworldcoo=pSeed->yworldcoo;	
			pSeed_copy->xcoo=pSeed->xcoo;			
			pSeed_copy->ycoo=pSeed->ycoo;		
			pSeed_copy->namem=pSeed->namem;			
			pSeed_copy->namep=pSeed->namep;		
			pSeed_copy->line=pSeed->line;			
			pSeed_copy->generation=pSeed->generation;
			pSeed_copy->species=pSeed->species;
			pSeed_copy->imcone=pSeed->imcone;		
			pSeed_copy->gewicht=pSeed->gewicht;	
			pSeed_copy->age=pSeed->age;			
			pSeed_copy->longdispersed=pSeed->longdispersed;	
			seed_list_copy.push_back(pSeed_copy);				// push back seed to list

				if(counterseed==1)
				{
					cout << pSeed->age << " <-pSeed ... seedage ... pSeed_copy-> " << pSeed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list_copy.size() after copying=" << seed_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
	// copy Karten
	aktort=0;
	for (vector<vector<Karten*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
	{ 
		vector<Karten*>& plot_list = *posw;

		// opening the original plot list and the referring reset list ..._copy		
		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Karten*>& plot_list_copy = *posiweltk;
		
		
		aktort++;

		cout << endl;
		cout << "plot_list.size()=" << plot_list.size() << endl;
		cout << "plot_list_copy.size() vor Kopieren=" << plot_list_copy.size() << endl;
		
		int counterkarten=1;	// for displaying the first plot
		for (vector<Karten*>::iterator pos = plot_list.begin(); pos != plot_list.end(); )
		{ 
			pKarten=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pKarten_copy= new Karten();						// create new plot
			pKarten_copy->xworldcoo=pKarten->xworldcoo;	
			pKarten_copy->yworldcoo=pKarten->yworldcoo;	
			pKarten_copy->xcoo=pKarten->xcoo;		
			pKarten_copy->ycoo=pKarten->ycoo;		
			pKarten_copy->Treedensitywert=pKarten->Treedensitywert;
			pKarten_copy->Treeanzahl=pKarten->Treeanzahl;	
			pKarten_copy->maxthawing_depth=pKarten->maxthawing_depth;
			pKarten_copy->auflagenstaerke=pKarten->auflagenstaerke;	
			pKarten_copy->auflagenstaerke0=pKarten->auflagenstaerke0;
			pKarten_copy->auflagenstaerke1=pKarten->auflagenstaerke1;
			pKarten_copy->auflagenstaerke2=pKarten->auflagenstaerke2;
			pKarten_copy->auflagenstaerke3=pKarten->auflagenstaerke3;
			pKarten_copy->auflagenstaerke4=pKarten->auflagenstaerke4;
			pKarten_copy->auflagenstaerke5=pKarten->auflagenstaerke5;
			pKarten_copy->auflagenstaerke6=pKarten->auflagenstaerke6;
			pKarten_copy->auflagenstaerke7=pKarten->auflagenstaerke7;
			pKarten_copy->auflagenstaerke8=pKarten->auflagenstaerke8;
			pKarten_copy->auflagenstaerke9=pKarten->auflagenstaerke9;
			pKarten_copy->auflagenstaerkemittel=pKarten->auflagenstaerkemittel;
			plot_list_copy.push_back(pKarten_copy);

			
				if(counterkarten==1)
				{
					cout << pKarten->Treedensitywert << " <-pKarten ... KartenTreedensity ... pKarten_copy-> " << pKarten_copy->Treedensitywert << endl;
				}

			++pos;
			++counterkarten;
		}
		
		cout << "plot_list_copy.size() after copying=" << plot_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	// copy Evaluation 
	aktort=0;
	for (vector<vector<Evaluation*> >::iterator posw = world_evaluation_list.begin(); posw != world_evaluation_list.end(); ++posw)
	{ 
		vector<Evaluation*>& EvaluationListe = *posw;

		// opening the original evaluation list and the referring reset list ..._copy		
		vector<vector<Evaluation*> >::iterator posiwelta = (world_evaluation_list_copy.begin()+aktort);
		vector<Evaluation*>& EvaluationListe_copy = *posiwelta;
		
		
		aktort++;

		cout << endl;
		cout << "EvaluationListe.size()=" << EvaluationListe.size() << endl;
		cout << "EvaluationListe_copy.size() before copying=" << EvaluationListe_copy.size() << endl;
		
		int counterauswert=1;	// For displaying the first evaluation list element
		for (vector<Evaluation*>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end(); )
		{ 
			pEvaluation=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pEvaluation_copy= new Evaluation();						// create new evaluation
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;	
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;	
				for (vector<double>::iterator posval=pEvaluation->BAliste.begin(); posval<pEvaluation->BAliste.end(); )
				{ 
					pEvaluation_copy->BAliste.push_back((*posval));
					++posval;
				} 
				for (vector<double>::iterator posval=pEvaluation->BArunmeanliste.begin(); posval<pEvaluation->BArunmeanliste.end(); )
				{ 
					pEvaluation_copy->BArunmeanliste.push_back((*posval));
					++posval;
				} 
			pEvaluation_copy->maxincrementbasalarea=pEvaluation->maxincrementbasalarea;	
			pEvaluation_copy->countermaxincrementbasalarea=pEvaluation->countermaxincrementbasalarea;
			pEvaluation_copy->wendejahr=pEvaluation->wendejahr;	
			pEvaluation_copy->saettigungsjahr=pEvaluation->saettigungsjahr;	
			pEvaluation_copy->nachwendejahr=pEvaluation->nachwendejahr;

				for (vector<int>::iterator posval=pEvaluation->nheight0b40liste.begin(); posval<pEvaluation->nheight0b40liste.end(); )
				{
					pEvaluation_copy->nheight0b40liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->nheight0b40runmeanliste.begin(); posval<pEvaluation->nheight0b40runmeanliste.end(); )
				{
					pEvaluation_copy->nheight0b40runmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pEvaluation->nheight41b200liste.begin(); posval<pEvaluation->nheight41b200liste.end(); )
				{ 
					pEvaluation_copy->nheight41b200liste.push_back((*posval));
					++posval;
				} 
				for (vector<double>::iterator posval=pEvaluation->nheight41b200runmeanliste.begin(); posval<pEvaluation->nheight41b200runmeanliste.end(); )
				{ 
					pEvaluation_copy->nheight41b200runmeanliste.push_back((*posval));
					++posval;
				} 
				for (vector<int>::iterator posval=pEvaluation->nheight201b10000liste.begin(); posval<pEvaluation->nheight201b10000liste.end(); )
				{ 
					pEvaluation_copy->nheight201b10000liste.push_back((*posval));
					++posval;
				} 
				for (vector<double>::iterator posval=pEvaluation->nheight201b10000runmeanliste.begin(); posval<pEvaluation->nheight201b10000runmeanliste.end(); )
				{ 
					pEvaluation_copy->nheight201b10000runmeanliste.push_back((*posval));
					++posval;
				} 

				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterliste.begin(); posval<pEvaluation->meanbreastdiameterliste.end(); )
				{
					pEvaluation_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				} 
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterrunmeanliste.begin(); posval<pEvaluation->meanbreastdiameterrunmeanliste.end(); )
				{ 
					pEvaluation_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				} 

				for (vector<int>::iterator posval=pEvaluation->stemcountliste.begin(); posval<pEvaluation->stemcountliste.end(); )
				{
					pEvaluation_copy->stemcountliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pEvaluation->stemcountrunmeanliste.begin(); posval<pEvaluation->stemcountrunmeanliste.end(); )
				{
					pEvaluation_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				}

				for (vector<double>::iterator posval=pEvaluation->meantreeheightliste.begin(); posval<pEvaluation->meantreeheightliste.end(); )
				{ 
					pEvaluation_copy->meantreeheightliste.push_back((*posval));
					++posval;
				} 
				for (vector<double>::iterator posval=pEvaluation->meantreeheightrunmeanliste.begin(); posval<pEvaluation->meantreeheightrunmeanliste.end(); )
				{ 
					pEvaluation_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				} 
				for (vector<double>::iterator posval=pEvaluation->meantreeageliste.begin(); posval<pEvaluation->meantreeageliste.end(); )
				{
					pEvaluation_copy->meantreeageliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->meantreeagerunmeanliste.begin(); posval<pEvaluation->meantreeagerunmeanliste.end(); )
				{
					pEvaluation_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				} 
			EvaluationListe_copy.push_back(pEvaluation_copy);				// push back element to "..._copy"

			
				if(counterauswert==1)
				{
					cout << pEvaluation->maxincrementbasalarea << " <-pEvaluation ... Evaluationmaxincrementbasalarea ... pEvaluation_copy-> " << pEvaluation_copy->maxincrementbasalarea << endl;
				}

			++pos;
			++counterauswert;
		}
		
		cout << "EvaluationListe_copy.size() after copying=" << EvaluationListe_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
}



/****************************************************************************************//**
 * \brief 
 *
 * //delete all information about trees, seeds, maps and analysis values from current container
 *
 *
 *
 *******************************************************************************************/
void ClearAllLists(void)
{
	int aktort=0;
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{ // World weather list loop begin
		
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;

		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<Seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list = *posiweltk;
	
		vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list.begin()+aktort);
		vector<Evaluation*>& evaluation_list = *posiwelt;
	
		aktort++;
				
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{
			pTree=(*pos);
			delete pTree;
		}
		tree_list.clear();

		for (list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
		{
			pSeed= (*pos);
			delete pSeed;
		}
		seed_list.clear();

		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			 pKarten= plot_list.at(kartenpos);
			delete pKarten;
		}
		plot_list.clear();
		plot_list.shrink_to_fit();

		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{
			pEvaluation= (*pos);
			pEvaluation->BAliste.clear();
			pEvaluation->BAliste.shrink_to_fit();
			pEvaluation->BArunmeanliste.clear();
			pEvaluation->BArunmeanliste.shrink_to_fit();
			pEvaluation->nheight0b40liste.clear();
			pEvaluation->nheight0b40liste.shrink_to_fit();
			pEvaluation->nheight0b40runmeanliste.clear();
			pEvaluation->nheight0b40runmeanliste.shrink_to_fit();
			pEvaluation->nheight41b200liste.clear();
			pEvaluation->nheight41b200liste.shrink_to_fit();
			pEvaluation->nheight41b200runmeanliste.clear();
			pEvaluation->nheight41b200runmeanliste.shrink_to_fit();
			pEvaluation->nheight201b10000liste.clear();
			pEvaluation->nheight201b10000liste.shrink_to_fit();
			pEvaluation->nheight201b10000runmeanliste.clear();
			pEvaluation->nheight201b10000runmeanliste.shrink_to_fit();
			pEvaluation->meanbreastdiameterliste.clear();
			pEvaluation->meanbreastdiameterliste.shrink_to_fit();
			pEvaluation->meanbreastdiameterrunmeanliste.clear();
			pEvaluation->meanbreastdiameterrunmeanliste.shrink_to_fit();
			pEvaluation->stemcountliste.clear();
			pEvaluation->stemcountliste.shrink_to_fit();
			pEvaluation->stemcountrunmeanliste.clear();
			pEvaluation->stemcountrunmeanliste.shrink_to_fit();
			pEvaluation->meantreeheightliste.clear();
			pEvaluation->meantreeheightliste.shrink_to_fit();
			pEvaluation->meantreeheightrunmeanliste.clear();
			pEvaluation->meantreeheightrunmeanliste.shrink_to_fit();
			pEvaluation->meantreeageliste.clear();
			pEvaluation->meantreeageliste.shrink_to_fit();
			pEvaluation->meantreeagerunmeanliste.clear();
			pEvaluation->meantreeagerunmeanliste.shrink_to_fit();
			
			delete pEvaluation;
		}
		evaluation_list.clear();
		evaluation_list.shrink_to_fit();
	} // World weather list loop end
}



/****************************************************************************************//**
 * \brief 
 *
 * restore all information about trees, seeds, maps and analysis values from backup container
 *
 *
 *
 *******************************************************************************************/
void RestoreAllLists(void)
{

	// copying trees
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;
		
		// Opening the standard list to push back elements to it from the reset list "..._copy"
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list_copy = *world_positon_b;
		
		
		aktort++;

		cout << endl;
		cout << "tree_list.size() before copying =" << tree_list.size() << endl;
		cout << "tree_list_copy.size() before copying =" << tree_list_copy.size() << endl;
		
		int countertree=1;	// For displaying the first tree
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pTree_copy= new Tree();						// create new tree
			pTree_copy->yworldcoo=pTree->yworldcoo;
			pTree_copy->xworldcoo=pTree->xworldcoo;
			pTree_copy->xcoo=pTree->xcoo;		
			pTree_copy->ycoo=pTree->ycoo;
			pTree_copy->name=pTree->name;
			pTree_copy->namem=pTree->namem;
			pTree_copy->namep=pTree->namep;
			pTree_copy->line=pTree->line;			
			pTree_copy->generation=pTree->generation;
			pTree_copy->species=pTree->species;
			pTree_copy->yr_of_establishment=pTree->yr_of_establishment;		
			pTree_copy->yr_of_dying=pTree->yr_of_dying;		
			pTree_copy->dbasal=pTree->dbasal;				
			pTree_copy->dbasalrel=pTree->dbasalrel;			
			pTree_copy->dbrust=pTree->dbrust;
			pTree_copy->dbrustrel=pTree->dbrustrel;		
			pTree_copy->height=pTree->height;
			pTree_copy->age=pTree->age;
			pTree_copy->cone=pTree->cone;
			pTree_copy->coneheight=pTree->coneheight;
			pTree_copy->seednewly_produced=pTree->seednewly_produced;
			pTree_copy->seedproduced=pTree->seedproduced;
			pTree_copy->speicher=pTree->speicher;
			pTree_copy->densitywert=pTree->densitywert;
			pTree_copy->thawing_depthinfluence=pTree->thawing_depthinfluence;
			pTree_copy->longdispersed=pTree->longdispersed;
			pTree_copy->growing=pTree->growing;
			tree_list_copy.push_back(pTree_copy);				// push tree in list
				if(countertree==1)
				{
					cout << pTree->height << " <-pTree ... Treeheight ... pTree_copy-> " << pTree_copy->height << endl;
				}

			++pos;
			++countertree;
		}
		
		cout << "tree_list.size() after copying=" << tree_list.size() << endl;
		cout << "tree_list_copy.size() after copying=" << tree_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	// copying Seeds
	aktort=0;
	for (vector<list<Seed*> >::iterator posw = world_seed_list_copy.begin(); posw != world_seed_list_copy.end(); ++posw)
	{ 
		list<Seed*>& seed_list = *posw;

		// opening the original seed list and the referring reset list ..._copy		
		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<Seed*>& seed_list_copy = *world_positon_s;
		
		
		aktort++;

		cout << endl;
		cout << "seed_list.size() before copying =" << seed_list.size() << endl;
		cout << "seed_list_copy.size() before copying =" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// for displaying the first seed
		for (list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pSeed=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pSeed_copy= new Seed();						// create new seed
			pSeed_copy->xworldcoo=pSeed->xworldcoo;			
			pSeed_copy->yworldcoo=pSeed->yworldcoo;	
			pSeed_copy->xcoo=pSeed->xcoo;			
			pSeed_copy->ycoo=pSeed->ycoo;		
			pSeed_copy->namem=pSeed->namem;			
			pSeed_copy->namep=pSeed->namep;		
			pSeed_copy->line=pSeed->line;			
			pSeed_copy->generation=pSeed->generation;	
			pSeed_copy->species=pSeed->species;
			pSeed_copy->imcone=pSeed->imcone;		
			pSeed_copy->gewicht=pSeed->gewicht;	
			pSeed_copy->age=pSeed->age;			
			pSeed_copy->longdispersed=pSeed->longdispersed;	
			seed_list_copy.push_back(pSeed_copy);				// push back seed to list

			
				if(counterseed==1)
				{
					cout << pSeed->age << " <-pSeed ... seedage ... pSeed_copy-> " << pSeed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list.size() after copying=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() after copying=" << seed_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
	// copying Karten
	aktort=0;
	for (vector<vector<Karten*> >::iterator posw = world_plot_list_copy.begin(); posw != world_plot_list_copy.end(); ++posw)
	{ 
		vector<Karten*>& plot_list = *posw;

		// opening the original plot list and the referring reset list ..._copy		
		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list_copy = *posiweltk;
		
		
		aktort++;

		cout << endl;
		cout << "plot_list.size() before copying =" << plot_list.size() << endl;
		cout << "plot_list_copy.size() before copying =" << plot_list_copy.size() << endl;
		
		int counterkarten=1;	// for displaying the first plot list element
		for (vector<Karten*>::iterator pos = plot_list.begin(); pos != plot_list.end(); )
		{ 
			pKarten=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pKarten_copy= new Karten();						// create new plot list element
			pKarten_copy->xworldcoo=pKarten->xworldcoo;	
			pKarten_copy->yworldcoo=pKarten->yworldcoo;	
			pKarten_copy->xcoo=pKarten->xcoo;		
			pKarten_copy->ycoo=pKarten->ycoo;		
			pKarten_copy->Treedensitywert=pKarten->Treedensitywert;
			pKarten_copy->Treeanzahl=pKarten->Treeanzahl;	
			pKarten_copy->maxthawing_depth=pKarten->maxthawing_depth;
			pKarten_copy->auflagenstaerke=pKarten->auflagenstaerke;
			pKarten_copy->auflagenstaerke0=pKarten->auflagenstaerke0;
			pKarten_copy->auflagenstaerke1=pKarten->auflagenstaerke1;
			pKarten_copy->auflagenstaerke2=pKarten->auflagenstaerke2;
			pKarten_copy->auflagenstaerke3=pKarten->auflagenstaerke3;
			pKarten_copy->auflagenstaerke4=pKarten->auflagenstaerke4;
			pKarten_copy->auflagenstaerke5=pKarten->auflagenstaerke5;
			pKarten_copy->auflagenstaerke6=pKarten->auflagenstaerke6;
			pKarten_copy->auflagenstaerke7=pKarten->auflagenstaerke7;
			pKarten_copy->auflagenstaerke8=pKarten->auflagenstaerke8;
			pKarten_copy->auflagenstaerke9=pKarten->auflagenstaerke9;
			pKarten_copy->auflagenstaerkemittel=pKarten->auflagenstaerkemittel;
			plot_list_copy.push_back(pKarten_copy);				// push back element to plot list 

			
				if(counterkarten==1)
				{
					cout << pKarten->Treedensitywert << " <-pKarten ... KartenTreedensity ... pKarten_copy-> " << pKarten_copy->Treedensitywert << endl;
				}

			++pos;
			++counterkarten;
		}
		
		cout << "plot_list.size() after copying=" << plot_list.size() << endl;
		cout << "plot_list_copy.size() after copying=" << plot_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	// copying Evaluation
	aktort=0;
	for (vector<vector<Evaluation*> >::iterator posw = world_evaluation_list_copy.begin(); posw != world_evaluation_list_copy.end(); ++posw)
	{ 
		vector<Evaluation*>& EvaluationListe = *posw;

		// Opening the standard list to push back elements to it from the reset list "..._copy"
		vector<vector<Evaluation*> >::iterator posiwelta = (world_evaluation_list.begin()+aktort);
		vector<Evaluation*>& EvaluationListe_copy = *posiwelta;
		
		
		aktort++;

		cout << endl;
		cout << "EvaluationListe.size() before copying=" << EvaluationListe.size() << endl;
		cout << "EvaluationListe_copy.size() before copying=" << EvaluationListe_copy.size() << endl;
		
		int counterauswert=1;
		for (vector<Evaluation*>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end(); )
		{ 
			pEvaluation=(*pos);

			// Push back all data to a new object of the referring reset list "..._copy" 
			pEvaluation_copy= new Evaluation();						// create new evaluation list element
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;	
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;	
				for (vector<double>::iterator posval=pEvaluation->BAliste.begin(); posval<pEvaluation->BAliste.end(); )
				{
					pEvaluation_copy->BAliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->BArunmeanliste.begin(); posval<pEvaluation->BArunmeanliste.end(); )
				{
					pEvaluation_copy->BArunmeanliste.push_back((*posval));
					++posval;
				}
			pEvaluation_copy->maxincrementbasalarea=pEvaluation->maxincrementbasalarea;		
			pEvaluation_copy->countermaxincrementbasalarea=pEvaluation->countermaxincrementbasalarea;
			pEvaluation_copy->wendejahr=pEvaluation->wendejahr;	
			pEvaluation_copy->saettigungsjahr=pEvaluation->saettigungsjahr;			
			pEvaluation_copy->nachwendejahr=pEvaluation->nachwendejahr;

				for (vector<int>::iterator posval=pEvaluation->nheight0b40liste.begin(); posval<pEvaluation->nheight0b40liste.end(); )
				{
					pEvaluation_copy->nheight0b40liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->nheight0b40runmeanliste.begin(); posval<pEvaluation->nheight0b40runmeanliste.end(); )
				{
					pEvaluation_copy->nheight0b40runmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pEvaluation->nheight41b200liste.begin(); posval<pEvaluation->nheight41b200liste.end(); )
				{
					pEvaluation_copy->nheight41b200liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->nheight41b200runmeanliste.begin(); posval<pEvaluation->nheight41b200runmeanliste.end(); )
				{
					pEvaluation_copy->nheight41b200runmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pEvaluation->nheight201b10000liste.begin(); posval<pEvaluation->nheight201b10000liste.end(); )
				{
					pEvaluation_copy->nheight201b10000liste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->nheight201b10000runmeanliste.begin(); posval<pEvaluation->nheight201b10000runmeanliste.end(); )
				{
					pEvaluation_copy->nheight201b10000runmeanliste.push_back((*posval));
					++posval;
				}
				
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterliste.begin(); posval<pEvaluation->meanbreastdiameterliste.end(); )
				{
					pEvaluation_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterrunmeanliste.begin(); posval<pEvaluation->meanbreastdiameterrunmeanliste.end(); )
				{
					pEvaluation_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				}

				for (vector<int>::iterator posval=pEvaluation->stemcountliste.begin(); posval<pEvaluation->stemcountliste.end(); )
				{
					pEvaluation_copy->stemcountliste.push_back((*posval));
					++posval;
				}
				for (vector<int>::iterator posval=pEvaluation->stemcountrunmeanliste.begin(); posval<pEvaluation->stemcountrunmeanliste.end(); )
				{
					pEvaluation_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				}

				for (vector<double>::iterator posval=pEvaluation->meantreeheightliste.begin(); posval<pEvaluation->meantreeheightliste.end(); )
				{
					pEvaluation_copy->meantreeheightliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->meantreeheightrunmeanliste.begin(); posval<pEvaluation->meantreeheightrunmeanliste.end(); )
				{
					pEvaluation_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->meantreeageliste.begin(); posval<pEvaluation->meantreeageliste.end(); )
				{
					pEvaluation_copy->meantreeageliste.push_back((*posval));
					++posval;
				}
				for (vector<double>::iterator posval=pEvaluation->meantreeagerunmeanliste.begin(); posval<pEvaluation->meantreeagerunmeanliste.end(); )
				{
					pEvaluation_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				}
			EvaluationListe_copy.push_back(pEvaluation_copy);				// push back evaluation list element to reset list

			
				if(counterauswert==1)
				{
					cout << pEvaluation->maxincrementbasalarea << " <-pEvaluation ... Evaluationmaxincrementbasalarea ... pEvaluation_copy-> " << pEvaluation_copy->maxincrementbasalarea << endl;
				}

			++pos;
			++counterauswert;
		}
		
		cout << "EvaluationListe.size() after copying=" << EvaluationListe.size() << endl;
		cout << "EvaluationListe_copy.size() after copying=" << EvaluationListe_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
}



/****************************************************************************************//**
 * \brief 
 *
 * //delete all information about trees, seeds, maps and analysis values from current container
 *
 *
 *
 *******************************************************************************************/
void ClearAllLists_copy(void)
{

	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{  // world tree copy list loop BEGIN
		list<Tree*>& tree_list = *posw;

		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<Seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Karten*>& plot_list = *posiweltk;
	
		vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list_copy.begin()+aktort);
		vector<Evaluation*>& evaluation_list = *posiwelt;
	
		aktort++;


		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{
			pTree=(*pos);
			delete pTree;
		}
		tree_list.clear();

		// deleting seed list elements
		for (list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
		{
			pSeed=(*pos);
			delete pSeed;
		}
		seed_list.clear();

		// delete plot list elements
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{ 
			pKarten=plot_list[kartenpos];
			delete pKarten;
		} 
		plot_list.clear();

		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{
			pEvaluation=(*pos);
			pEvaluation->BAliste.clear();
			pEvaluation->BArunmeanliste.clear();
			pEvaluation->nheight0b40liste.clear();
			pEvaluation->nheight0b40runmeanliste.clear();
			pEvaluation->nheight41b200liste.clear();
			pEvaluation->nheight41b200runmeanliste.clear();
			pEvaluation->nheight201b10000liste.clear();
			pEvaluation->nheight201b10000runmeanliste.clear();
			pEvaluation->meanbreastdiameterliste.clear();
			pEvaluation->meanbreastdiameterrunmeanliste.clear();
			pEvaluation->stemcountliste.clear();
			pEvaluation->stemcountrunmeanliste.clear();
			pEvaluation->meantreeheightliste.clear();
			pEvaluation->meantreeheightrunmeanliste.clear();
			pEvaluation->meantreeageliste.clear();
			pEvaluation->meantreeagerunmeanliste.clear();
		}
		evaluation_list.clear();
	} // world tree copy list loop END
}

