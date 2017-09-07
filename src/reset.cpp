using namespace std;


// reset of als lists to year of choice "resetyear"


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

	/// copy trees
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
		
		int countertree=1;	// For displaying the first tree
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			/// Push back all data to a new object of the referring reset list "..._copy" 
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
			//vector<double> Dbrustliste;	copy
			//	int counterbrustval=1;
				/*for (vector<double>::iterator posdbr=pTree->Dbrustliste.begin(); posdbr<pTree->Dbrustliste.end(); )
				{ // Dbrustliste Beginn
					pTree_copy->Dbrustliste.push_back((*posdbr));

					if(countertree==1 && counterbrustval==1)
					{
						cout << " ORIGINAL: " << (*posdbr) << endl;
					}

					++posdbr;
					++counterbrustval;
				} // Dbrustliste Ende	
				//vector<double> Dbrustliste;	copy
				counterbrustval=1;
				for (vector<double>::iterator posdbr=pTree_copy->Dbrustliste.begin(); posdbr<pTree_copy->Dbrustliste.end(); )
				{ // Dbrustliste Beginn
		
					if(countertree==1 && counterbrustval==1)
					{
						cout << " KOPIE: " << (*posdbr) << endl;
					}

					++posdbr;
					++counterbrustval;
				} // Dbrustliste Ende*/	
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

	
	/// copying seeds
	aktort=0;
	for (vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<seed*>& seed_list = *posw;

		// opening the original seed list and the referring reset list ..._copy		
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<seed*>& seed_list_copy = *world_positon_s;
		
		
		aktort++;

		cout << endl;
		cout << "seed_list.size()=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() before copying=" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// For displaying the first seed
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pseed=(*pos);

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pseed_copy= new seed();						// create new seed
			pseed_copy->xworldcoo=pseed->xworldcoo;			
			pseed_copy->yworldcoo=pseed->yworldcoo;	
			pseed_copy->xcoo=pseed->xcoo;			
			pseed_copy->ycoo=pseed->ycoo;		
			pseed_copy->namem=pseed->namem;			
			pseed_copy->namep=pseed->namep;		
			pseed_copy->line=pseed->line;			
			pseed_copy->generation=pseed->generation;
			pseed_copy->species=pseed->species;
			pseed_copy->imcone=pseed->imcone;		
			pseed_copy->gewicht=pseed->gewicht;	
			pseed_copy->age=pseed->age;			
			pseed_copy->longdispersed=pseed->longdispersed;	
			seed_list_copy.push_back(pseed_copy);				// push back seed to list

				if(counterseed==1)
				{
					cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list_copy.size() after copying=" << seed_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
	/// copying plots
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

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pKarten_copy= new Karten();						// create new plot
			pKarten_copy->xworldcoo=pKarten->xworldcoo;						//!<
			pKarten_copy->yworldcoo=pKarten->yworldcoo;						//!<
			pKarten_copy->xcoo=pKarten->xcoo;							//!<
			pKarten_copy->ycoo=pKarten->ycoo;							//!<
			pKarten_copy->Treedensitywert=pKarten->Treedensitywert;					//!<
			pKarten_copy->Treeanzahl=pKarten->Treeanzahl;						//!<
			pKarten_copy->maxthawing_depth=pKarten->maxthawing_depth; 			//!< in mm demnach max 65.535 m
			pKarten_copy->auflagenstaerke=pKarten->auflagenstaerke;			//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke0=pKarten->auflagenstaerke0;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke1=pKarten->auflagenstaerke1;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke2=pKarten->auflagenstaerke2;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke3=pKarten->auflagenstaerke3;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke4=pKarten->auflagenstaerke4;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke5=pKarten->auflagenstaerke5;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke6=pKarten->auflagenstaerke6;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke7=pKarten->auflagenstaerke7;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke8=pKarten->auflagenstaerke8;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke9=pKarten->auflagenstaerke9;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerkemittel=pKarten->auflagenstaerkemittel;	//!< in 0.1mm demnach max 6.5535 m
			plot_list_copy.push_back(pKarten_copy);				// Karten in Liste einfuegen

			
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

	
	/// Evaluation kopieren
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

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pEvaluation_copy= new Evaluation();						// create new evaluation
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;						//!<
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;						//!<
				//vector<double> BAliste;					//!<
				for (vector<double>::iterator posval=pEvaluation->BAliste.begin(); posval<pEvaluation->BAliste.end(); )
				{ // BAliste BEGIN
					pEvaluation_copy->BAliste.push_back((*posval));
					++posval;
				} // BAliste END
				//vector<double> BArunmeanliste;			//!<
				for (vector<double>::iterator posval=pEvaluation->BArunmeanliste.begin(); posval<pEvaluation->BArunmeanliste.end(); )
				{ // BArunmeanliste BEGIN
					pEvaluation_copy->BArunmeanliste.push_back((*posval));
					++posval;
				} // BArunmeanliste END
			pEvaluation_copy->maxsteigungbasalarea=pEvaluation->maxsteigungbasalarea;			//!<
			pEvaluation_copy->countermaxsteigungbasalarea=pEvaluation->countermaxsteigungbasalarea;	//!<
			pEvaluation_copy->wendejahr=pEvaluation->wendejahr;						//!<
			pEvaluation_copy->saettigungsjahr=pEvaluation->saettigungsjahr;				//!<
			pEvaluation_copy->nachwendejahr=pEvaluation->nachwendejahr;					//!<

				//vector<int> nheight0b40liste;
				for (vector<int>::iterator posval=pEvaluation->nheight0b40liste.begin(); posval<pEvaluation->nheight0b40liste.end(); )
				{ // nheight0b40liste BEGIN
					pEvaluation_copy->nheight0b40liste.push_back((*posval));
					++posval;
				} // nheight0b40liste END
				//vector<double> nheight0b40runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight0b40runmeanliste.begin(); posval<pEvaluation->nheight0b40runmeanliste.end(); )
				{ // nheight0b40runmeanliste BEGIN
					pEvaluation_copy->nheight0b40runmeanliste.push_back((*posval));
					++posval;
				} // nheight0b40runmeanliste END
				//vector<int> nheight41b200liste;
				for (vector<int>::iterator posval=pEvaluation->nheight41b200liste.begin(); posval<pEvaluation->nheight41b200liste.end(); )
				{ // nheight41b200liste BEGIN
					pEvaluation_copy->nheight41b200liste.push_back((*posval));
					++posval;
				} // nheight41b200liste END
				//vector<double> nheight41b200runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight41b200runmeanliste.begin(); posval<pEvaluation->nheight41b200runmeanliste.end(); )
				{ // nheight41b200runmeanliste BEGIN
					pEvaluation_copy->nheight41b200runmeanliste.push_back((*posval));
					++posval;
				} // nheight41b200runmeanliste END
				//vector<int> nheight201b10000liste;
				for (vector<int>::iterator posval=pEvaluation->nheight201b10000liste.begin(); posval<pEvaluation->nheight201b10000liste.end(); )
				{ // nheight201b10000liste BEGIN
					pEvaluation_copy->nheight201b10000liste.push_back((*posval));
					++posval;
				} // nheight201b10000liste END
				//vector<double> nheight201b10000runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight201b10000runmeanliste.begin(); posval<pEvaluation->nheight201b10000runmeanliste.end(); )
				{ // nheight201b10000runmeanliste BEGIN
					pEvaluation_copy->nheight201b10000runmeanliste.push_back((*posval));
					++posval;
				} // nheight201b10000runmeanliste END

				//vector<double> meanbreastdiameterliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterliste.begin(); posval<pEvaluation->meanbreastdiameterliste.end(); )
				{ // meanbreastdiameterliste BEGIN
					pEvaluation_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterliste END
				//vector<double> meanbreastdiameterrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterrunmeanliste.begin(); posval<pEvaluation->meanbreastdiameterrunmeanliste.end(); )
				{ // meanbreastdiameterrunmeanliste BEGIN
					pEvaluation_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterrunmeanliste END

				//vector<int> stemcountliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountliste.begin(); posval<pEvaluation->stemcountliste.end(); )
				{ // stemcountliste BEGIN
					pEvaluation_copy->stemcountliste.push_back((*posval));
					++posval;
				} // stemcountliste END
				//vector<int> stemcountrunmeanliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountrunmeanliste.begin(); posval<pEvaluation->stemcountrunmeanliste.end(); )
				{ // stemcountrunmeanliste BEGIN
					pEvaluation_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				} // stemcountrunmeanliste END

				//vector<double> meantreeheightliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightliste.begin(); posval<pEvaluation->meantreeheightliste.end(); )
				{ // meantreeheightliste BEGIN
					pEvaluation_copy->meantreeheightliste.push_back((*posval));
					++posval;
				} // meantreeheightliste END
				//vector<double> meantreeheightrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightrunmeanliste.begin(); posval<pEvaluation->meantreeheightrunmeanliste.end(); )
				{ // meantreeheightrunmeanliste BEGIN
					pEvaluation_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				} // meantreeheightrunmeanliste END
				//vector<double> meantreeageliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeageliste.begin(); posval<pEvaluation->meantreeageliste.end(); )
				{ // meantreeageliste BEGIN
					pEvaluation_copy->meantreeageliste.push_back((*posval));
					++posval;
				} // meantreeageliste END
				//vector<double> meantreeagerunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeagerunmeanliste.begin(); posval<pEvaluation->meantreeagerunmeanliste.end(); )
				{ // meantreeagerunmeanliste BEGIN
					pEvaluation_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				} // meantreeagerunmeanliste END
			EvaluationListe_copy.push_back(pEvaluation_copy);				// push back element to "..._copy"

			
				if(counterauswert==1)
				{
					cout << pEvaluation->maxsteigungbasalarea << " <-pEvaluation ... Evaluationmaxsteigungbasalarea ... pEvaluation_copy-> " << pEvaluation_copy->maxsteigungbasalarea << endl;
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
	cout << endl << "starting ClearAllLists!";

	int aktort=0;
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{	// World weather list loop begin
		
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;

		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list = *posiweltk;
	
		vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list.begin()+aktort);
		vector<Evaluation*>& evaluation_list = *posiwelt;
	
		aktort++;

				
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{ // tree list loop BEGIN
			 pTree=(*pos);
			//pTree->Dbrustliste.clear();
			delete pTree;
		} // tree list loop END
		tree_list.clear();
		//tree_list.shrink_to_fit();
		cout << endl << "           !!! tree_list deleted!         " << endl;

		
		// seed_listn loeschen
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
		{ // seed list loop BEGIN
			 pseed= (*pos);
			delete pseed;
		} // seed list loop END
		seed_list.clear();
		//seed_list.shrink_to_fit();
		cout << endl << "           !!! seed_list deleted!         " << endl;



		// Alle Kartenelemente loeschen
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{ // plot list loop BEGIN
			 pKarten= plot_list.at(kartenpos);
			delete pKarten;
		} // plot list loop END
		plot_list.clear();
		plot_list.shrink_to_fit();
		cout << endl << "           !!! plot_list deleted!         " << endl;


		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{ // evaluation list loop BEGIN
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

		} // evaluation list loop END
		evaluation_list.clear();
		evaluation_list.shrink_to_fit();
		cout << endl << "           !!! evaluation_list deleted!         " << endl;
		
	}	// World weather list loop end
	
	cout << "ClearAllLists ended!" << endl << endl;
		
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

	/// copying trees
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

			/// Push back all data to a new object of the referring reset list "..._copy" 
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
				//vector<double> Dbrustliste;	copy
				/*int counterbrustval=1;
				for (vector<double>::iterator posdbr=pTree->Dbrustliste.begin(); posdbr<pTree->Dbrustliste.end(); )
				{ // Dbrustliste Beginn
					pTree_copy->Dbrustliste.push_back((*posdbr));

					if(countertree==1 && counterbrustval==1)
					{
						cout << " ORIGINAL: " << (*posdbr) << endl;
					}

					++posdbr;
					++counterbrustval;
				} // Dbrustliste Ende	
				//vector<double> Dbrustliste;	copy
				counterbrustval=1;
				for (vector<double>::iterator posdbr=pTree_copy->Dbrustliste.begin(); posdbr<pTree_copy->Dbrustliste.end(); )
				{ // Dbrustliste Beginn
		
					if(countertree==1 && counterbrustval==1)
					{
						cout << " KOPIE: " << (*posdbr) << endl;
					}

					++posdbr;
					++counterbrustval;
				} // Dbrustliste Ende	*/
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
				if(countertree==1)
				{
				//	cout << pTree->height << " & " << pTree->Dbrustliste.size() << endl;
					//cout << pTree_copy->height << " & " << pTree_copy->Dbrustliste.size() << endl;
				}
			tree_list_copy.push_back(pTree_copy);				// Tree in Liste einfuegen

			
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

	
	/// copying seeds
	aktort=0;
	for (vector<list<seed*> >::iterator posw = world_seed_list_copy.begin(); posw != world_seed_list_copy.end(); ++posw)
	{ 
		list<seed*>& seed_list = *posw;

		// opening the original seed list and the referring reset list ..._copy		
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list_copy = *world_positon_s;
		
		
		aktort++;

		cout << endl;
		cout << "seed_list.size() before copying =" << seed_list.size() << endl;
		cout << "seed_list_copy.size() before copying =" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// for displaying the first seed
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pseed=(*pos);

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pseed_copy= new seed();						// create new seed
			pseed_copy->xworldcoo=pseed->xworldcoo;			
			pseed_copy->yworldcoo=pseed->yworldcoo;	
			pseed_copy->xcoo=pseed->xcoo;			
			pseed_copy->ycoo=pseed->ycoo;		
			pseed_copy->namem=pseed->namem;			
			pseed_copy->namep=pseed->namep;		
			pseed_copy->line=pseed->line;			
			pseed_copy->generation=pseed->generation;	
			pseed_copy->species=pseed->species;
			pseed_copy->imcone=pseed->imcone;		
			pseed_copy->gewicht=pseed->gewicht;	
			pseed_copy->age=pseed->age;			
			pseed_copy->longdispersed=pseed->longdispersed;	
			seed_list_copy.push_back(pseed_copy);				// push back seed to list

			
				if(counterseed==1)
				{
					cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list.size() after copying=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() after copying=" << seed_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
	/// copying plots
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

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pKarten_copy= new Karten();						// create new plot list element
			pKarten_copy->xworldcoo=pKarten->xworldcoo;						//!<
			pKarten_copy->yworldcoo=pKarten->yworldcoo;						//!<
			pKarten_copy->xcoo=pKarten->xcoo;							//!<
			pKarten_copy->ycoo=pKarten->ycoo;							//!<
			pKarten_copy->Treedensitywert=pKarten->Treedensitywert;					//!<
			pKarten_copy->Treeanzahl=pKarten->Treeanzahl;						//!<
			pKarten_copy->maxthawing_depth=pKarten->maxthawing_depth; 			//!< in mm demnach max 65.535 m
			pKarten_copy->auflagenstaerke=pKarten->auflagenstaerke;			//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke0=pKarten->auflagenstaerke0;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke1=pKarten->auflagenstaerke1;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke2=pKarten->auflagenstaerke2;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke3=pKarten->auflagenstaerke3;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke4=pKarten->auflagenstaerke4;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke5=pKarten->auflagenstaerke5;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke6=pKarten->auflagenstaerke6;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke7=pKarten->auflagenstaerke7;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke8=pKarten->auflagenstaerke8;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerke9=pKarten->auflagenstaerke9;		//!< in 0.1mm demnach max 6.5535 m
			pKarten_copy->auflagenstaerkemittel=pKarten->auflagenstaerkemittel;	//!< in 0.1mm demnach max 6.5535 m
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

	
	/// copying evaluations
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
		
		int counterauswert=1;	// Für die Ausgabe des ersten Treees
		for (vector<Evaluation*>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end(); )
		{ 
			pEvaluation=(*pos);

			/// Push back all data to a new object of the referring reset list "..._copy" 
			pEvaluation_copy= new Evaluation();						// create new evaluation list element
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;						//!<
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;						//!<
				//vector<double> BAliste;					//!<
				for (vector<double>::iterator posval=pEvaluation->BAliste.begin(); posval<pEvaluation->BAliste.end(); )
				{ // BAliste loop BEGIN
					pEvaluation_copy->BAliste.push_back((*posval));
					++posval;
				} // BAliste loop END
				//vector<double> BArunmeanliste;			//!<
				for (vector<double>::iterator posval=pEvaluation->BArunmeanliste.begin(); posval<pEvaluation->BArunmeanliste.end(); )
				{ // BArunmeanliste loop BEGIN
					pEvaluation_copy->BArunmeanliste.push_back((*posval));
					++posval;
				} // BArunmeanliste loop END
			pEvaluation_copy->maxsteigungbasalarea=pEvaluation->maxsteigungbasalarea;			//!<
			pEvaluation_copy->countermaxsteigungbasalarea=pEvaluation->countermaxsteigungbasalarea;	//!<
			pEvaluation_copy->wendejahr=pEvaluation->wendejahr;						//!<
			pEvaluation_copy->saettigungsjahr=pEvaluation->saettigungsjahr;				//!<
			pEvaluation_copy->nachwendejahr=pEvaluation->nachwendejahr;					//!<

				//vector<int> nheight0b40liste;
				for (vector<int>::iterator posval=pEvaluation->nheight0b40liste.begin(); posval<pEvaluation->nheight0b40liste.end(); )
				{ // nheight0b40liste loop BEGIN
					pEvaluation_copy->nheight0b40liste.push_back((*posval));
					++posval;
				} // nheight0b40liste loop END
				//vector<double> nheight0b40runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight0b40runmeanliste.begin(); posval<pEvaluation->nheight0b40runmeanliste.end(); )
				{ // nheight0b40runmeanliste loop BEGIN
					pEvaluation_copy->nheight0b40runmeanliste.push_back((*posval));
					++posval;
				} // nheight0b40runmeanliste loop END
				//vector<int> nheight41b200liste;
				for (vector<int>::iterator posval=pEvaluation->nheight41b200liste.begin(); posval<pEvaluation->nheight41b200liste.end(); )
				{ // nheight41b200liste loop BEGIN
					pEvaluation_copy->nheight41b200liste.push_back((*posval));
					++posval;
				} // nheight41b200liste loop END
				//vector<double> nheight41b200runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight41b200runmeanliste.begin(); posval<pEvaluation->nheight41b200runmeanliste.end(); )
				{ // nheight41b200runmeanliste loop BEGIN
					pEvaluation_copy->nheight41b200runmeanliste.push_back((*posval));
					++posval;
				} // nheight41b200runmeanliste loop END
				//vector<int> nheight201b10000liste;
				for (vector<int>::iterator posval=pEvaluation->nheight201b10000liste.begin(); posval<pEvaluation->nheight201b10000liste.end(); )
				{ // nheight201b10000liste loop BEGIN
					pEvaluation_copy->nheight201b10000liste.push_back((*posval));
					++posval;
				} // nheight201b10000liste loop END
				//vector<double> nheight201b10000runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight201b10000runmeanliste.begin(); posval<pEvaluation->nheight201b10000runmeanliste.end(); )
				{ // nheight201b10000runmeanliste loop BEGIN
					pEvaluation_copy->nheight201b10000runmeanliste.push_back((*posval));
					++posval;
				} // nheight201b10000runmeanliste loop END
				
				//vector<double> meanbreastdiameterliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterliste.begin(); posval<pEvaluation->meanbreastdiameterliste.end(); )
				{ // meanbreastdiameterliste loop BEGIN
					pEvaluation_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterliste loop END
				//vector<double> meanbreastdiameterrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterrunmeanliste.begin(); posval<pEvaluation->meanbreastdiameterrunmeanliste.end(); )
				{ // meanbreastdiameterrunmeanliste loop BEGIN
					pEvaluation_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterrunmeanliste loop END

				//vector<int> stemcountliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountliste.begin(); posval<pEvaluation->stemcountliste.end(); )
				{ // stemcountliste loop BEGIN
					pEvaluation_copy->stemcountliste.push_back((*posval));
					++posval;
				} // stemcountliste loop END
				//vector<int> stemcountrunmeanliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountrunmeanliste.begin(); posval<pEvaluation->stemcountrunmeanliste.end(); )
				{ // stemcountrunmeanliste loop BEGIN
					pEvaluation_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				} // stemcountrunmeanliste loop END

				//vector<double> meantreeheightliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightliste.begin(); posval<pEvaluation->meantreeheightliste.end(); )
				{ // meantreeheightliste loop BEGIN
					pEvaluation_copy->meantreeheightliste.push_back((*posval));
					++posval;
				} // meantreeheightliste loop END
				//vector<double> meantreeheightrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightrunmeanliste.begin(); posval<pEvaluation->meantreeheightrunmeanliste.end(); )
				{ // meantreeheightrunmeanliste loop BEGIN
					pEvaluation_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				} // meantreeheightrunmeanliste loop END
				//vector<double> meantreeageliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeageliste.begin(); posval<pEvaluation->meantreeageliste.end(); )
				{ // meantreeageliste loop BEGIN
					pEvaluation_copy->meantreeageliste.push_back((*posval));
					++posval;
				} // meantreeageliste loop END
				//vector<double> meantreeagerunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeagerunmeanliste.begin(); posval<pEvaluation->meantreeagerunmeanliste.end(); )
				{ // meantreeagerunmeanliste loop BEGIN
					pEvaluation_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				} // meantreeagerunmeanliste loop END
			EvaluationListe_copy.push_back(pEvaluation_copy);				// push back evaluation list element to reset list

			
				if(counterauswert==1)
				{
					cout << pEvaluation->maxsteigungbasalarea << " <-pEvaluation ... Evaluationmaxsteigungbasalarea ... pEvaluation_copy-> " << pEvaluation_copy->maxsteigungbasalarea << endl;
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

	cout << endl << "ClearAllLists_copy called!";

	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{  // world tree copy list loop BEGIN
		list<Tree*>& tree_list = *posw;

		vector<list<seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Karten*>& plot_list = *posiweltk;
	
		vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list_copy.begin()+aktort);
		vector<Evaluation*>& evaluation_list = *posiwelt;
	
		aktort++;

		
		
		

		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{ // tree list loop BEGIN
			pTree=(*pos);
		//	pTree->Dbrustliste.clear();
			delete pTree;
		} // tree list loop END
		tree_list.clear();
		cout << endl << "           !!! tree_list deleted!         " << endl;

		
		// deleting seed list elements
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
		{ // seed list loop BEGIN
			pseed=(*pos);
			delete pseed;
		} // seed list loop END
		seed_list.clear();
		cout << endl << "           !!! seed_list deleted!         " << endl;



		// delete plot list elements
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{ // plot list loop BEGIN
			pKarten=plot_list[kartenpos];
			delete pKarten;
		} // plot list loop END
		plot_list.clear();
		cout << endl << "           !!! plot_list deleted!         " << endl;


		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{ // evaluation list loop BEGIN
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
			//delete pEvaluation;
		} // Evaluation list loop END
		evaluation_list.clear();
		cout << endl << "           !!! evaluation_list deleted!         " << endl;
		
	}	// world tree copy list loop END
		

	cout << "ClearAllLists_copy ended!" << endl << endl;
		
}

