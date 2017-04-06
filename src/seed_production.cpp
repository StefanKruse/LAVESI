
/****************************************************************************************//**
* \brief calculate  seed production
*
* depends on seed production factor, tree size, actual growth
*******************************************************************************************/
void seedproduktion(int treerows, int treecols, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list,int yearposition)
{

	//int aktort=0;
	
	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ // Weltschleife
		list<Tree*>& tree_list = *posw;

		//vector<list<seed*> >::iterator posiwelt = (world_seed_list.begin()+aktort);
		//list<seed*>& seed_list = *posiwelt;

		//aktort++;

 
		// Berechnung des aktuellen Ortes in Koordinaten
		//int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		//int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);


		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ // Beginnn tree_list ablaufen
			pTree=(*pos);

			if (pTree->cone==1) // Wenn ein Tree cone traegt (GELOESCHT: "und einen Speicher von >= 0 aufweist") werden seed produced
			{ // Traegt der Tree cone? Beginn
				pTree->seednewly_produced=(int) floor( parameter[0].seedprodfaktor	// seedproduktion in Abh. von Faktor ...
												* pTree->height/100					// ... Groesze ...
												* pTree->dbasalrel					// ... aktuellem Zuwachs ...
												* (1.0-(1.0/(pTree->height/50))) );	// ... height.

												//cout<<"parameter seedprod (height,dbasalrel,seedprodfaktor)\n"<<pTree->height<<pTree->dbasalrel<<parameter[0].seedprodfaktor<<endl;
				++pos;				

				/*transferiert in mortalität um Speiche
				if (pTree->seednewly_produced>0)
				{ // seed wurden produced Beginn
					for (int sn=0; sn<pTree->seednewly_produced; sn++)
					{ // Neuen seed erstellen Beginn
						pseed= new seed();			// 1. Neuen seed erzeugen
						pseed->yworldcoo=aktortyworldcoo;	// 2. Werte dem seed zuweisen
						pseed->xworldcoo=aktortxworldcoo;
						pseed->xcoo=pTree->xcoo;
						pseed->ycoo=pTree->ycoo;
						pseed->namem=pTree->name;	// Im Moment wird nur der Name des MutterTreees ...
						pseed->namep=pTree->name;	// ... auf beide Elemente (namem + namep) uebergeben
						pseed->line=pTree->line;
						pseed->generation=pTree->generation+1;	// Generation=0 ist von außen eingebracht
						pseed->imcone=true;
						pseed->gewicht=1;
						pseed->age=0;
						pseed->species=pTree->species;
						pseed->elternheight=pTree->height;
						seed_list.push_back(pseed);// 3. seed in Liste einfuegen
					} // Neuen seed erstellen Ende
					++pos;
				} // seed wurden produced Ende
				else
				{
					++pos;
				}
				*/
			}  // Traegt der Tree cone? Ende
			
			else
			{
				++pos;
			}
		} // Ende tree_list ablaufen
	}//Weltschleife Ende

}
