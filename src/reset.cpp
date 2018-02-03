using namespace std;

/****************************************************************************************//**
 * \brief 
 *
 * backup all information about trees, seeds, maps and analysis values at year of choice "resetyear"
 *
 *
 *
 *******************************************************************************************/
 
void Savealllists(void)
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

			pTree_copy= new Tree();
			
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
			pTree_copy->dbreast=pTree->dbreast;
			pTree_copy->dbreastrel=pTree->dbreastrel;		
			pTree_copy->height=pTree->height;
			pTree_copy->age=pTree->age;
			pTree_copy->cone=pTree->cone;
			pTree_copy->coneheight=pTree->coneheight;
			pTree_copy->seednewly_produced=pTree->seednewly_produced;
			pTree_copy->seedproduced=pTree->seedproduced;
			pTree_copy->buffer=pTree->buffer;
			pTree_copy->densitywert=pTree->densitywert;
			pTree_copy->thawing_depthinfluence=pTree->thawing_depthinfluence;
			pTree_copy->longdispersed=pTree->longdispersed;
			pTree_copy->growing=pTree->growing;
				
			tree_list_copy.push_back(pTree_copy);

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
		
		int counterseed=1;	// for displaying the first seed
		for (list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pSeed=(*pos);

			pSeed_copy= new Seed();	
			
			pSeed_copy->xworldcoo=pSeed->xworldcoo;			
			pSeed_copy->yworldcoo=pSeed->yworldcoo;	
			pSeed_copy->xcoo=pSeed->xcoo;			
			pSeed_copy->ycoo=pSeed->ycoo;		
			pSeed_copy->namem=pSeed->namem;			
			pSeed_copy->namep=pSeed->namep;		
			pSeed_copy->line=pSeed->line;			
			pSeed_copy->generation=pSeed->generation;
			pSeed_copy->species=pSeed->species;
			pSeed_copy->incone=pSeed->incone;		
			pSeed_copy->weight=pSeed->weight;	
			pSeed_copy->age=pSeed->age;			
			pSeed_copy->longdispersed=pSeed->longdispersed;	
			seed_list_copy.push_back(pSeed_copy);	

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

		
	// copy Envirgrid
	aktort=0;
	for (vector<vector<Envirgrid*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
	{ 
		vector<Envirgrid*>& plot_list = *posw;

		// opening the original plot list and the referring reset list ..._copy		
		vector<vector<Envirgrid*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Envirgrid*>& plot_list_copy = *posiweltk;
		
		
		aktort++;

		cout << endl;
		cout << "plot_list.size()=" << plot_list.size() << endl;
		cout << "plot_list_copy.size() vor Kopieren=" << plot_list_copy.size() << endl;
		
		int counterkarten=1;	// for displaying the first plot
		for (vector<Envirgrid*>::iterator pos = plot_list.begin(); pos != plot_list.end(); )
		{ 
			pEnvirgrid=(*pos);

			pEnvirgrid_copy= new Envirgrid();
			
			pEnvirgrid_copy->xworldcoo=pEnvirgrid->xworldcoo;	
			pEnvirgrid_copy->yworldcoo=pEnvirgrid->yworldcoo;	
			pEnvirgrid_copy->xcoo=pEnvirgrid->xcoo;		
			pEnvirgrid_copy->ycoo=pEnvirgrid->ycoo;		
			pEnvirgrid_copy->Treedensityvalue=pEnvirgrid->Treedensityvalue;
			pEnvirgrid_copy->Treenumber=pEnvirgrid->Treenumber;	
			pEnvirgrid_copy->maxthawing_depth=pEnvirgrid->maxthawing_depth;
			pEnvirgrid_copy->litterheight=pEnvirgrid->litterheight;	
			pEnvirgrid_copy->litterheight0=pEnvirgrid->litterheight0;
			pEnvirgrid_copy->litterheight1=pEnvirgrid->litterheight1;
			pEnvirgrid_copy->litterheight2=pEnvirgrid->litterheight2;
			pEnvirgrid_copy->litterheight3=pEnvirgrid->litterheight3;
			pEnvirgrid_copy->litterheight4=pEnvirgrid->litterheight4;
			pEnvirgrid_copy->litterheight5=pEnvirgrid->litterheight5;
			pEnvirgrid_copy->litterheight6=pEnvirgrid->litterheight6;
			pEnvirgrid_copy->litterheight7=pEnvirgrid->litterheight7;
			pEnvirgrid_copy->litterheight8=pEnvirgrid->litterheight8;
			pEnvirgrid_copy->litterheight9=pEnvirgrid->litterheight9;
			pEnvirgrid_copy->litterheightmean=pEnvirgrid->litterheightmean;
			plot_list_copy.push_back(pEnvirgrid_copy);

			if(counterkarten==1)
			{
				cout << pEnvirgrid->Treedensityvalue << " <-pEnvirgrid ... EnvirgridTreedensity ... pEnvirgrid_copy-> " << pEnvirgrid_copy->Treedensityvalue << endl;
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
		
		int counterauswert=1;	// for displaying the first evaluation list element
		for (vector<Evaluation*>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end(); )
		{ 
			pEvaluation=(*pos);

			pEvaluation_copy= new Evaluation();	
			
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;	
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;	
			for (vector<double>::iterator posval=pEvaluation->basalarealist.begin(); posval<pEvaluation->basalarealist.end(); )
			{ 
				pEvaluation_copy->basalarealist.push_back((*posval));
				++posval;
			} 
			for (vector<double>::iterator posval=pEvaluation->basalarearunmeanlist.begin(); posval<pEvaluation->basalarearunmeanlist.end(); )
			{ 
				pEvaluation_copy->basalarearunmeanlist.push_back((*posval));
				++posval;
			} 
			pEvaluation_copy->maxincrementbasalarea=pEvaluation->maxincrementbasalarea;	
			pEvaluation_copy->countermaxincrementbasalarea=pEvaluation->countermaxincrementbasalarea;
			pEvaluation_copy->yearofturningpoint=pEvaluation->yearofturningpoint;	
			pEvaluation_copy->yearofequilibrium=pEvaluation->yearofequilibrium;	
			pEvaluation_copy->postyearofturningpoint=pEvaluation->postyearofturningpoint;
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
			
			EvaluationListe_copy.push_back(pEvaluation_copy);

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
 
void Clearalllists(void)
{
	int aktort=0;
	for (vector<vector<Weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{
		
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;

		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<Seed*>& seed_list = *world_positon_s;

		vector<vector<Envirgrid*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
		vector<Envirgrid*>& plot_list = *posiweltk;
	
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
			 pEnvirgrid= plot_list.at(kartenpos);
			delete pEnvirgrid;
		}
		plot_list.clear();
		plot_list.shrink_to_fit();

		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{
			pEvaluation= (*pos);
			
			pEvaluation->basalarealist.clear();
			pEvaluation->basalarealist.shrink_to_fit();
			pEvaluation->basalarearunmeanlist.clear();
			pEvaluation->basalarearunmeanlist.shrink_to_fit();
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
	}
}



/****************************************************************************************//**
 * \brief 
 *
 * restore all information about trees, seeds, maps and analysis values from backup container
 *
 *
 *
 *******************************************************************************************/
 
void Restorealllists(void)
{
	// copying Trees
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;
		
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list_copy = *world_positon_b;
		
		aktort++;

		cout << endl;
		cout << "tree_list.size() before copying =" << tree_list.size() << endl;
		cout << "tree_list_copy.size() before copying =" << tree_list_copy.size() << endl;
		
		int countertree=1;	// for displaying the first tree
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			pTree_copy= new Tree();
			
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
			pTree_copy->dbreast=pTree->dbreast;
			pTree_copy->dbreastrel=pTree->dbreastrel;		
			pTree_copy->height=pTree->height;
			pTree_copy->age=pTree->age;
			pTree_copy->cone=pTree->cone;
			pTree_copy->coneheight=pTree->coneheight;
			pTree_copy->seednewly_produced=pTree->seednewly_produced;
			pTree_copy->seedproduced=pTree->seedproduced;
			pTree_copy->buffer=pTree->buffer;
			pTree_copy->densitywert=pTree->densitywert;
			pTree_copy->thawing_depthinfluence=pTree->thawing_depthinfluence;
			pTree_copy->longdispersed=pTree->longdispersed;
			pTree_copy->growing=pTree->growing;
			tree_list_copy.push_back(pTree_copy);
			
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

			pSeed_copy= new Seed();	
			
			pSeed_copy->xworldcoo=pSeed->xworldcoo;			
			pSeed_copy->yworldcoo=pSeed->yworldcoo;	
			pSeed_copy->xcoo=pSeed->xcoo;			
			pSeed_copy->ycoo=pSeed->ycoo;		
			pSeed_copy->namem=pSeed->namem;			
			pSeed_copy->namep=pSeed->namep;		
			pSeed_copy->line=pSeed->line;			
			pSeed_copy->generation=pSeed->generation;	
			pSeed_copy->species=pSeed->species;
			pSeed_copy->incone=pSeed->incone;		
			pSeed_copy->weight=pSeed->weight;	
			pSeed_copy->age=pSeed->age;			
			pSeed_copy->longdispersed=pSeed->longdispersed;	
			seed_list_copy.push_back(pSeed_copy);
			
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
		
	// copying Envirgrid
	aktort=0;
	for (vector<vector<Envirgrid*> >::iterator posw = world_plot_list_copy.begin(); posw != world_plot_list_copy.end(); ++posw)
	{ 
		vector<Envirgrid*>& plot_list = *posw;

		// opening the original plot list and the referring reset list ..._copy		
		vector<vector<Envirgrid*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
		vector<Envirgrid*>& plot_list_copy = *posiweltk;
				
		aktort++;

		cout << endl;
		cout << "plot_list.size() before copying =" << plot_list.size() << endl;
		cout << "plot_list_copy.size() before copying =" << plot_list_copy.size() << endl;
		
		int counterkarten=1;	// for displaying the first plot list element
		for (vector<Envirgrid*>::iterator pos = plot_list.begin(); pos != plot_list.end(); )
		{ 
			pEnvirgrid=(*pos);

			pEnvirgrid_copy= new Envirgrid();
			
			pEnvirgrid_copy->xworldcoo=pEnvirgrid->xworldcoo;	
			pEnvirgrid_copy->yworldcoo=pEnvirgrid->yworldcoo;	
			pEnvirgrid_copy->xcoo=pEnvirgrid->xcoo;		
			pEnvirgrid_copy->ycoo=pEnvirgrid->ycoo;		
			pEnvirgrid_copy->Treedensityvalue=pEnvirgrid->Treedensityvalue;
			pEnvirgrid_copy->Treenumber=pEnvirgrid->Treenumber;	
			pEnvirgrid_copy->maxthawing_depth=pEnvirgrid->maxthawing_depth;
			pEnvirgrid_copy->litterheight=pEnvirgrid->litterheight;
			pEnvirgrid_copy->litterheight0=pEnvirgrid->litterheight0;
			pEnvirgrid_copy->litterheight1=pEnvirgrid->litterheight1;
			pEnvirgrid_copy->litterheight2=pEnvirgrid->litterheight2;
			pEnvirgrid_copy->litterheight3=pEnvirgrid->litterheight3;
			pEnvirgrid_copy->litterheight4=pEnvirgrid->litterheight4;
			pEnvirgrid_copy->litterheight5=pEnvirgrid->litterheight5;
			pEnvirgrid_copy->litterheight6=pEnvirgrid->litterheight6;
			pEnvirgrid_copy->litterheight7=pEnvirgrid->litterheight7;
			pEnvirgrid_copy->litterheight8=pEnvirgrid->litterheight8;
			pEnvirgrid_copy->litterheight9=pEnvirgrid->litterheight9;
			pEnvirgrid_copy->litterheightmean=pEnvirgrid->litterheightmean;
			plot_list_copy.push_back(pEnvirgrid_copy);
		
			if(counterkarten==1)
			{
				cout << pEnvirgrid->Treedensityvalue << " <-pEnvirgrid ... EnvirgridTreedensity ... pEnvirgrid_copy-> " << pEnvirgrid_copy->Treedensityvalue << endl;
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

		// opening the standard list to push back elements to it from the reset list "..._copy"
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

			pEvaluation_copy= new Evaluation();
			
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;	
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;	
			for (vector<double>::iterator posval=pEvaluation->basalarealist.begin(); posval<pEvaluation->basalarealist.end(); )
			{
				pEvaluation_copy->basalarealist.push_back((*posval));
				++posval;
			}
			for (vector<double>::iterator posval=pEvaluation->basalarearunmeanlist.begin(); posval<pEvaluation->basalarearunmeanlist.end(); )
			{
				pEvaluation_copy->basalarearunmeanlist.push_back((*posval));
				++posval;
			}
			pEvaluation_copy->maxincrementbasalarea=pEvaluation->maxincrementbasalarea;		
			pEvaluation_copy->countermaxincrementbasalarea=pEvaluation->countermaxincrementbasalarea;
			pEvaluation_copy->yearofturningpoint=pEvaluation->yearofturningpoint;	
			pEvaluation_copy->yearofequilibrium=pEvaluation->yearofequilibrium;			
			pEvaluation_copy->postyearofturningpoint=pEvaluation->postyearofturningpoint;
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
			
			EvaluationListe_copy.push_back(pEvaluation_copy);
	
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

void Clearalllists_copy(void)
{
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{
		list<Tree*>& tree_list = *posw;

		vector<list<Seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<Seed*>& seed_list = *world_positon_s;

		vector<vector<Envirgrid*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Envirgrid*>& plot_list = *posiweltk;
	
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
			pEnvirgrid=plot_list[kartenpos];
			delete pEnvirgrid;
		} 
		plot_list.clear();

		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{
			pEvaluation=(*pos);
			pEvaluation->basalarealist.clear();
			pEvaluation->basalarearunmeanlist.clear();
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
	}
}

