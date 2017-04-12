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

	/// Bäume kopieren
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;

		// Öffne auch die _copy Liste um dann die Bäume dahin zu puschen
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list_copy.begin()+aktort);
		list<Tree*>& tree_list_copy = *world_positon_b;
		
		
		aktort++;

		cout << endl;
		cout << "tree_list.size()=" << tree_list.size() << endl;
		cout << "tree_list_copy.size() vor Kopieren=" << tree_list_copy.size() << endl;
		
		int countertree=1;	// Für die Ausgabe des ersten Treees
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			/// Alle Daten des Treees in ein neues Objekt in der ..._copy-Liste erzeugen
			pTree_copy= new Tree();						// Neuen Tree erzeugen
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
				//vector<double> Dbrustliste;	kopieren
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
				//vector<double> Dbrustliste;	kopieren
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
				/*if(countertree==1)
				{
					cout << pTree->height << " & " << pTree->Dbrustliste.size() << endl;
					cout << pTree_copy->height << " & " << pTree_copy->Dbrustliste.size() << endl;
				}*/
			tree_list_copy.push_back(pTree_copy);				// Tree in Liste einfuegen

			
				if(countertree==1)
				{
					cout << pTree->height << " <-pTree ... Treeheight ... pTree_copy-> " << pTree_copy->height << endl;
				}

			++pos;
			++countertree;
		}
		
		cout << "tree_list_copy.size() nach Kopieren=" << tree_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	/// seed kopieren
	aktort=0;
	for (vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<seed*>& seed_list = *posw;

		// Öffne auch die _copy Liste um dann die seed dahin zu puschen
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<seed*>& seed_list_copy = *world_positon_s;
		
		
		aktort++;

		cout << endl;
		cout << "seed_list.size()=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() vor Kopieren=" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// Für die Ausgabe des ersten Treees
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pseed=(*pos);

			/// Alle Daten des seeds in ein neues Objekt in der ..._copy-Liste erzeugen
			pseed_copy= new seed();						// Neuen seed erzeugen
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
			seed_list_copy.push_back(pseed_copy);				// seed in Liste einfuegen

			
				if(counterseed==1)
				{
					cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list_copy.size() nach Kopieren=" << seed_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
	/// Karten kopieren
	aktort=0;
	for (vector<vector<Karten*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
	{ 
		vector<Karten*>& plot_list = *posw;

		// Öffne auch die _copy Liste um dann die Karten dahin zu puschen
		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Karten*>& plot_list_copy = *posiweltk;
		
		
		aktort++;

		cout << endl;
		cout << "plot_list.size()=" << plot_list.size() << endl;
		cout << "plot_list_copy.size() vor Kopieren=" << plot_list_copy.size() << endl;
		
		int counterkarten=1;	// Für die Ausgabe des ersten Treees
		for (vector<Karten*>::iterator pos = plot_list.begin(); pos != plot_list.end(); )
		{ 
			pKarten=(*pos);

			/// Alle Daten des Kartens in ein neues Objekt in der ..._copy-Liste erzeugen
			pKarten_copy= new Karten();						// Neuen Karten erzeugen
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
		
		cout << "plot_list_copy.size() nach Kopieren=" << plot_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	/// Evaluation kopieren
	aktort=0;
	for (vector<vector<Evaluation*> >::iterator posw = world_evaluation_list.begin(); posw != world_evaluation_list.end(); ++posw)
	{ 
		vector<Evaluation*>& EvaluationListe = *posw;

		// Öffne auch die _copy Liste um dann die Evaluation dahin zu puschen
		vector<vector<Evaluation*> >::iterator posiwelta = (world_evaluation_list_copy.begin()+aktort);
		vector<Evaluation*>& EvaluationListe_copy = *posiwelta;
		
		
		aktort++;

		cout << endl;
		cout << "EvaluationListe.size()=" << EvaluationListe.size() << endl;
		cout << "EvaluationListe_copy.size() vor Kopieren=" << EvaluationListe_copy.size() << endl;
		
		int counterauswert=1;	// Für die Ausgabe des ersten Treees
		for (vector<Evaluation*>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end(); )
		{ 
			pEvaluation=(*pos);

			/// Alle Daten des Evaluations in ein neues Objekt in der ..._copy-Liste erzeugen
			pEvaluation_copy= new Evaluation();						// Neue Evaluation erzeugen
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;						//!<
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;						//!<
				//vector<double> BAliste;					//!<
				for (vector<double>::iterator posval=pEvaluation->BAliste.begin(); posval<pEvaluation->BAliste.end(); )
				{ // BAliste Beginn
					pEvaluation_copy->BAliste.push_back((*posval));
					++posval;
				} // BAliste Ende
				//vector<double> BArunmeanliste;			//!<
				for (vector<double>::iterator posval=pEvaluation->BArunmeanliste.begin(); posval<pEvaluation->BArunmeanliste.end(); )
				{ // BArunmeanliste Beginn
					pEvaluation_copy->BArunmeanliste.push_back((*posval));
					++posval;
				} // BArunmeanliste Ende
			pEvaluation_copy->maxsteigungbasalarea=pEvaluation->maxsteigungbasalarea;			//!<
			pEvaluation_copy->countermaxsteigungbasalarea=pEvaluation->countermaxsteigungbasalarea;	//!<
			pEvaluation_copy->wendejahr=pEvaluation->wendejahr;						//!<
			pEvaluation_copy->saettigungsjahr=pEvaluation->saettigungsjahr;				//!<
			pEvaluation_copy->nachwendejahr=pEvaluation->nachwendejahr;					//!<

				//vector<int> nheight0b40liste;
				for (vector<int>::iterator posval=pEvaluation->nheight0b40liste.begin(); posval<pEvaluation->nheight0b40liste.end(); )
				{ // nheight0b40liste Beginn
					pEvaluation_copy->nheight0b40liste.push_back((*posval));
					++posval;
				} // nheight0b40liste Ende
				//vector<double> nheight0b40runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight0b40runmeanliste.begin(); posval<pEvaluation->nheight0b40runmeanliste.end(); )
				{ // nheight0b40runmeanliste Beginn
					pEvaluation_copy->nheight0b40runmeanliste.push_back((*posval));
					++posval;
				} // nheight0b40runmeanliste Ende
				//vector<int> nheight41b200liste;
				for (vector<int>::iterator posval=pEvaluation->nheight41b200liste.begin(); posval<pEvaluation->nheight41b200liste.end(); )
				{ // nheight41b200liste Beginn
					pEvaluation_copy->nheight41b200liste.push_back((*posval));
					++posval;
				} // nheight41b200liste Ende
				//vector<double> nheight41b200runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight41b200runmeanliste.begin(); posval<pEvaluation->nheight41b200runmeanliste.end(); )
				{ // nheight41b200runmeanliste Beginn
					pEvaluation_copy->nheight41b200runmeanliste.push_back((*posval));
					++posval;
				} // nheight41b200runmeanliste Ende
				//vector<int> nheight201b10000liste;
				for (vector<int>::iterator posval=pEvaluation->nheight201b10000liste.begin(); posval<pEvaluation->nheight201b10000liste.end(); )
				{ // nheight201b10000liste Beginn
					pEvaluation_copy->nheight201b10000liste.push_back((*posval));
					++posval;
				} // nheight201b10000liste Ende
				//vector<double> nheight201b10000runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight201b10000runmeanliste.begin(); posval<pEvaluation->nheight201b10000runmeanliste.end(); )
				{ // nheight201b10000runmeanliste Beginn
					pEvaluation_copy->nheight201b10000runmeanliste.push_back((*posval));
					++posval;
				} // nheight201b10000runmeanliste Ende

				//vector<double> meanbreastdiameterliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterliste.begin(); posval<pEvaluation->meanbreastdiameterliste.end(); )
				{ // meanbreastdiameterliste Beginn
					pEvaluation_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterliste Ende
				//vector<double> meanbreastdiameterrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterrunmeanliste.begin(); posval<pEvaluation->meanbreastdiameterrunmeanliste.end(); )
				{ // meanbreastdiameterrunmeanliste Beginn
					pEvaluation_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterrunmeanliste Ende

				//vector<int> stemcountliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountliste.begin(); posval<pEvaluation->stemcountliste.end(); )
				{ // stemcountliste Beginn
					pEvaluation_copy->stemcountliste.push_back((*posval));
					++posval;
				} // stemcountliste Ende
				//vector<int> stemcountrunmeanliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountrunmeanliste.begin(); posval<pEvaluation->stemcountrunmeanliste.end(); )
				{ // stemcountrunmeanliste Beginn
					pEvaluation_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				} // stemcountrunmeanliste Ende

				//vector<double> meantreeheightliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightliste.begin(); posval<pEvaluation->meantreeheightliste.end(); )
				{ // meantreeheightliste Beginn
					pEvaluation_copy->meantreeheightliste.push_back((*posval));
					++posval;
				} // meantreeheightliste Ende
				//vector<double> meantreeheightrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightrunmeanliste.begin(); posval<pEvaluation->meantreeheightrunmeanliste.end(); )
				{ // meantreeheightrunmeanliste Beginn
					pEvaluation_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				} // meantreeheightrunmeanliste Ende
				//vector<double> meantreeageliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeageliste.begin(); posval<pEvaluation->meantreeageliste.end(); )
				{ // meantreeageliste Beginn
					pEvaluation_copy->meantreeageliste.push_back((*posval));
					++posval;
				} // meantreeageliste Ende
				//vector<double> meantreeagerunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeagerunmeanliste.begin(); posval<pEvaluation->meantreeagerunmeanliste.end(); )
				{ // meantreeagerunmeanliste Beginn
					pEvaluation_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				} // meantreeagerunmeanliste Ende
			EvaluationListe_copy.push_back(pEvaluation_copy);				// Evaluation in Liste einfuegen

			
				if(counterauswert==1)
				{
					cout << pEvaluation->maxsteigungbasalarea << " <-pEvaluation ... Evaluationmaxsteigungbasalarea ... pEvaluation_copy-> " << pEvaluation_copy->maxsteigungbasalarea << endl;
				}

			++pos;
			++counterauswert;
		}
		
		cout << "EvaluationListe_copy.size() nach Kopieren=" << EvaluationListe_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
}



/****************************************************************************************//**
 * \brief 
 *
 * delete all information about trees, seeds, maps and analysis values from current container
 *
 *
 *
 *******************************************************************************************/
void ClearAllLists(void)
{
	cout << endl << "ClearAllLists gestartet!";

	int aktort=0;
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
	{	// Weltschleife Beginn
		// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Refernenz
		// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
		// realisiert ist weiterverfahren werden
		// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
			//vector<weather*>& weather_list = *posw;

		// Um auf ein bestimmtes Element in der Welt zuzugreifen muss ein Iterator bis
		// zum entsprechenden Element justiert werden und dann eine tree_list als Referenz
		// erzeugt werden
		
				
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
		{ // Beginn tree_list ablaufen
			pTree=(*pos);
			//pTree->Dbrustliste.clear();
			delete pTree;
		} // Ende tree_list ableufen;
		tree_list.clear();
		cout << endl << "           !!! tree_list deleted!         " << endl;

		
		// seed_listn loeschen
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
		{ // Beginnn seed_list ablaufen
			pseed=(*pos);
			delete pseed;
		} // Ende seed_list ablaufen
		seed_list.clear();
		cout << endl << "           !!! seed_list deleted!         " << endl;



		// Alle Kartenelemente loeschen
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{ // Kartenschleife Beginn
			pKarten=plot_list[kartenpos];
			delete pKarten;
		} // Kartenschleife Ende
		plot_list.clear();
		cout << endl << "           !!! plot_list deleted!         " << endl;


		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{ // Beginn evaluation_list ablaufen
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
			delete pEvaluation;
			
			
		
		//vector<double> BAliste;					//!<
		//vector<double> BArunmeanliste;			//!<
				
		//vector<int> nheight0b40liste;
		//vector<double> nheight0b40runmeanliste;
		//vector<int> nheight41b200liste;
		//vector<double> nheight41b200runmeanliste;
		//vector<int> nheight201b10000liste;
		//vector<double> nheight201b10000runmeanliste;
		
		//vector<double> meanbreastdiameterliste;
		//vector<double> meanbreastdiameterrunmeanliste;
		
		//vector<int> stemcountliste;
		//vector<int> stemcountrunmeanliste;
		
		//vector<double> meantreeheightliste;
		//vector<double> meantreeheightrunmeanliste;
		//vector<double> meantreeageliste;
		//vector<double> meantreeagerunmeanliste;
		
		} // Ende evaluation_list ablaufen
		evaluation_list.clear();
		cout << endl << "           !!! evaluation_list deleted!         " << endl;
		
	}	// Weltschleife Ende
	
	//cout<<"clearing wind directions\n";
		winddir.clear();
	//cout<<"clearing wind speeds\n";
		windspd.clear();
	//cout<<"clearing years\n";
		globalyears.clear();
	//cout<<"clearing vegetypes\n";
		vegetationtype.clear();
		
		//delete[] winddir;

	cout << "ClearAllLists beendet!" << endl << endl;
		
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

	/// Bäume kopieren
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;

		// Öffne auch die normale Liste um dann die Bäume dahin zu puschen
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list_copy = *world_positon_b;
		
		
		aktort++;

		cout << endl;
		cout << "tree_list.size() vor dem Kopieren =" << tree_list.size() << endl;
		cout << "tree_list_copy.size() vor Kopieren =" << tree_list_copy.size() << endl;
		
		int countertree=1;	// Für die Ausgabe des ersten Treees
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			/// Alle Daten des Treees in ein neues Objekt in der ..._copy-Liste erzeugen
			pTree_copy= new Tree();						// Neuen Tree erzeugen
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
				//vector<double> Dbrustliste;	kopieren
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
				//vector<double> Dbrustliste;	kopieren
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
		
		cout << "tree_list.size() nach Kopieren=" << tree_list.size() << endl;
		cout << "tree_list_copy.size() nach Kopieren=" << tree_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	/// seed kopieren
	aktort=0;
	for (vector<list<seed*> >::iterator posw = world_seed_list_copy.begin(); posw != world_seed_list_copy.end(); ++posw)
	{ 
		list<seed*>& seed_list = *posw;

		// Öffne auch die _copy Liste um dann die seed dahin zu puschen
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list_copy = *world_positon_s;
		
		
		aktort++;

		cout << endl;
		cout << "seed_list.size() vor Kopieren =" << seed_list.size() << endl;
		cout << "seed_list_copy.size() vor Kopieren =" << seed_list_copy.size() << endl;
		
		int counterseed=1;	// Für die Ausgabe des ersten Treees
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pseed=(*pos);

			/// Alle Daten des seeds in ein neues Objekt in der ..._copy-Liste erzeugen
			pseed_copy= new seed();						// Neuen seed erzeugen
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
			seed_list_copy.push_back(pseed_copy);				// seed in Liste einfuegen

			
				if(counterseed==1)
				{
					cout << pseed->age << " <-pseed ... seedage ... pseed_copy-> " << pseed_copy->age << endl;
				}

			++pos;
			++counterseed;
		}
		
		cout << "seed_list.size() nach Kopieren=" << seed_list.size() << endl;
		cout << "seed_list_copy.size() nach Kopieren=" << seed_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
	/// Karten kopieren
	aktort=0;
	for (vector<vector<Karten*> >::iterator posw = world_plot_list_copy.begin(); posw != world_plot_list_copy.end(); ++posw)
	{ 
		vector<Karten*>& plot_list = *posw;

		// Öffne auch die _copy Liste um dann die Karten dahin zu puschen
		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list_copy = *posiweltk;
		
		
		aktort++;

		cout << endl;
		cout << "plot_list.size() vor Kopieren =" << plot_list.size() << endl;
		cout << "plot_list_copy.size() vor Kopieren =" << plot_list_copy.size() << endl;
		
		int counterkarten=1;	// Für die Ausgabe des ersten Treees
		for (vector<Karten*>::iterator pos = plot_list.begin(); pos != plot_list.end(); )
		{ 
			pKarten=(*pos);

			/// Alle Daten des Kartens in ein neues Objekt in der ..._copy-Liste erzeugen
			pKarten_copy= new Karten();						// Neuen Karten erzeugen
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
		
		cout << "plot_list.size() nach Kopieren=" << plot_list.size() << endl;
		cout << "plot_list_copy.size() nach Kopieren=" << plot_list_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

	
	/// Evaluation kopieren
	aktort=0;
	for (vector<vector<Evaluation*> >::iterator posw = world_evaluation_list_copy.begin(); posw != world_evaluation_list_copy.end(); ++posw)
	{ 
		vector<Evaluation*>& EvaluationListe = *posw;

		// Öffne auch die _copy Liste um dann die Evaluation dahin zu puschen
		vector<vector<Evaluation*> >::iterator posiwelta = (world_evaluation_list.begin()+aktort);
		vector<Evaluation*>& EvaluationListe_copy = *posiwelta;
		
		
		aktort++;

		cout << endl;
		cout << "EvaluationListe.size() vor Kopieren=" << EvaluationListe.size() << endl;
		cout << "EvaluationListe_copy.size() vor Kopieren=" << EvaluationListe_copy.size() << endl;
		
		int counterauswert=1;	// Für die Ausgabe des ersten Treees
		for (vector<Evaluation*>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end(); )
		{ 
			pEvaluation=(*pos);

			/// Alle Daten des Evaluations in ein neues Objekt in der ..._copy-Liste erzeugen
			pEvaluation_copy= new Evaluation();						// Neuen Evaluation erzeugen
			pEvaluation_copy->xworldcoo=pEvaluation->xworldcoo;						//!<
			pEvaluation_copy->yworldcoo=pEvaluation->yworldcoo;						//!<
				//vector<double> BAliste;					//!<
				for (vector<double>::iterator posval=pEvaluation->BAliste.begin(); posval<pEvaluation->BAliste.end(); )
				{ // BAliste Beginn
					pEvaluation_copy->BAliste.push_back((*posval));
					++posval;
				} // BAliste Ende
				//vector<double> BArunmeanliste;			//!<
				for (vector<double>::iterator posval=pEvaluation->BArunmeanliste.begin(); posval<pEvaluation->BArunmeanliste.end(); )
				{ // BArunmeanliste Beginn
					pEvaluation_copy->BArunmeanliste.push_back((*posval));
					++posval;
				} // BArunmeanliste Ende
			pEvaluation_copy->maxsteigungbasalarea=pEvaluation->maxsteigungbasalarea;			//!<
			pEvaluation_copy->countermaxsteigungbasalarea=pEvaluation->countermaxsteigungbasalarea;	//!<
			pEvaluation_copy->wendejahr=pEvaluation->wendejahr;						//!<
			pEvaluation_copy->saettigungsjahr=pEvaluation->saettigungsjahr;				//!<
			pEvaluation_copy->nachwendejahr=pEvaluation->nachwendejahr;					//!<

				//vector<int> nheight0b40liste;
				for (vector<int>::iterator posval=pEvaluation->nheight0b40liste.begin(); posval<pEvaluation->nheight0b40liste.end(); )
				{ // nheight0b40liste Beginn
					pEvaluation_copy->nheight0b40liste.push_back((*posval));
					++posval;
				} // nheight0b40liste Ende
				//vector<double> nheight0b40runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight0b40runmeanliste.begin(); posval<pEvaluation->nheight0b40runmeanliste.end(); )
				{ // nheight0b40runmeanliste Beginn
					pEvaluation_copy->nheight0b40runmeanliste.push_back((*posval));
					++posval;
				} // nheight0b40runmeanliste Ende
				//vector<int> nheight41b200liste;
				for (vector<int>::iterator posval=pEvaluation->nheight41b200liste.begin(); posval<pEvaluation->nheight41b200liste.end(); )
				{ // nheight41b200liste Beginn
					pEvaluation_copy->nheight41b200liste.push_back((*posval));
					++posval;
				} // nheight41b200liste Ende
				//vector<double> nheight41b200runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight41b200runmeanliste.begin(); posval<pEvaluation->nheight41b200runmeanliste.end(); )
				{ // nheight41b200runmeanliste Beginn
					pEvaluation_copy->nheight41b200runmeanliste.push_back((*posval));
					++posval;
				} // nheight41b200runmeanliste Ende
				//vector<int> nheight201b10000liste;
				for (vector<int>::iterator posval=pEvaluation->nheight201b10000liste.begin(); posval<pEvaluation->nheight201b10000liste.end(); )
				{ // nheight201b10000liste Beginn
					pEvaluation_copy->nheight201b10000liste.push_back((*posval));
					++posval;
				} // nheight201b10000liste Ende
				//vector<double> nheight201b10000runmeanliste;
				for (vector<double>::iterator posval=pEvaluation->nheight201b10000runmeanliste.begin(); posval<pEvaluation->nheight201b10000runmeanliste.end(); )
				{ // nheight201b10000runmeanliste Beginn
					pEvaluation_copy->nheight201b10000runmeanliste.push_back((*posval));
					++posval;
				} // nheight201b10000runmeanliste Ende

				//vector<double> meanbreastdiameterliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterliste.begin(); posval<pEvaluation->meanbreastdiameterliste.end(); )
				{ // meanbreastdiameterliste Beginn
					pEvaluation_copy->meanbreastdiameterliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterliste Ende
				//vector<double> meanbreastdiameterrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meanbreastdiameterrunmeanliste.begin(); posval<pEvaluation->meanbreastdiameterrunmeanliste.end(); )
				{ // meanbreastdiameterrunmeanliste Beginn
					pEvaluation_copy->meanbreastdiameterrunmeanliste.push_back((*posval));
					++posval;
				} // meanbreastdiameterrunmeanliste Ende

				//vector<int> stemcountliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountliste.begin(); posval<pEvaluation->stemcountliste.end(); )
				{ // stemcountliste Beginn
					pEvaluation_copy->stemcountliste.push_back((*posval));
					++posval;
				} // stemcountliste Ende
				//vector<int> stemcountrunmeanliste;
				for (vector<int>::iterator posval=pEvaluation->stemcountrunmeanliste.begin(); posval<pEvaluation->stemcountrunmeanliste.end(); )
				{ // stemcountrunmeanliste Beginn
					pEvaluation_copy->stemcountrunmeanliste.push_back((*posval));
					++posval;
				} // stemcountrunmeanliste Ende

				//vector<double> meantreeheightliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightliste.begin(); posval<pEvaluation->meantreeheightliste.end(); )
				{ // meantreeheightliste Beginn
					pEvaluation_copy->meantreeheightliste.push_back((*posval));
					++posval;
				} // meantreeheightliste Ende
				//vector<double> meantreeheightrunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeheightrunmeanliste.begin(); posval<pEvaluation->meantreeheightrunmeanliste.end(); )
				{ // meantreeheightrunmeanliste Beginn
					pEvaluation_copy->meantreeheightrunmeanliste.push_back((*posval));
					++posval;
				} // meantreeheightrunmeanliste Ende
				//vector<double> meantreeageliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeageliste.begin(); posval<pEvaluation->meantreeageliste.end(); )
				{ // meantreeageliste Beginn
					pEvaluation_copy->meantreeageliste.push_back((*posval));
					++posval;
				} // meantreeageliste Ende
				//vector<double> meantreeagerunmeanliste;
				for (vector<double>::iterator posval=pEvaluation->meantreeagerunmeanliste.begin(); posval<pEvaluation->meantreeagerunmeanliste.end(); )
				{ // meantreeagerunmeanliste Beginn
					pEvaluation_copy->meantreeagerunmeanliste.push_back((*posval));
					++posval;
				} // meantreeagerunmeanliste Ende
			EvaluationListe_copy.push_back(pEvaluation_copy);				// Evaluation in Liste einfuegen

			
				if(counterauswert==1)
				{
					cout << pEvaluation->maxsteigungbasalarea << " <-pEvaluation ... Evaluationmaxsteigungbasalarea ... pEvaluation_copy-> " << pEvaluation_copy->maxsteigungbasalarea << endl;
				}

			++pos;
			++counterauswert;
		}
		
		cout << "EvaluationListe.size() nach Kopieren=" << EvaluationListe.size() << endl;
		cout << "EvaluationListe_copy.size() nach Kopieren=" << EvaluationListe_copy.size() << endl;
		cout << endl;
		cout << endl;

	}

		
}



/****************************************************************************************//**
 * \brief 
 *
 * delete all information about trees, seeds, maps and analysis values from current container
 *
 *
 *
 *******************************************************************************************/
void ClearAllLists_copy(void)
{ // Weltschleife Beginn

	cout << endl << "ClearAllLists_copy gestartet!";

	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;

		vector<list<seed*> >::iterator world_positon_s = (world_seed_list_copy.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;

		vector<vector<Karten*> >::iterator posiweltk = (world_plot_list_copy.begin()+aktort);
		vector<Karten*>& plot_list = *posiweltk;
	
		vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list_copy.begin()+aktort);
		vector<Evaluation*>& evaluation_list = *posiwelt;
	
		aktort++;

		
		
		

		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{ // Beginn tree_list ablaufen
			pTree=(*pos);
		//	pTree->Dbrustliste.clear();
			delete pTree;
		} // Ende tree_list ableufen;
		tree_list.clear();
		cout << endl << "           !!! tree_list gelöscht!         " << endl;

		
		// seed_listn loeschen
		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
		{ // Beginnn seed_list ablaufen
			pseed=(*pos);
			delete pseed;
		} // Ende seed_list ablaufen
		seed_list.clear();
		cout << endl << "           !!! seed_list gelöscht!         " << endl;



		// Alle Kartenelemente loeschen
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{ // Kartenschleife Beginn
			pKarten=plot_list[kartenpos];
			delete pKarten;
		} // Kartenschleife Ende
		plot_list.clear();
		cout << endl << "           !!! plot_list gelöscht!         " << endl;


		for (vector<Evaluation*>::iterator pos = evaluation_list.begin(); pos != evaluation_list.end(); ++pos)
		{ // Beginn evaluation_list ablaufen
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
			delete pEvaluation;
		} // Ende evaluation_list ablaufen
		evaluation_list.clear();
		cout << endl << "           !!! evaluation_list gelöscht!         " << endl;
		
	}	// Weltschleife Ende
		

	cout << "ClearAllLists_copy beendet!" << endl << endl;
		
}

