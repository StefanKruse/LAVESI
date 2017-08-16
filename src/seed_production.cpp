
/****************************************************************************************//**
* \brief calculate  seed production
*
* depends on seed production factor, tree size, actual growth
*******************************************************************************************/
void seedproduktion(int treerows, int treecols, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list,int yearposition)
{

	//int aktort=0;
	
	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ //begin plot list iteration
		list<Tree*>& tree_list = *posw;

		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ //begin tree_list iteration
			pTree=(*pos);

			if (pTree->cone==1) // If a tree bears cones seeds will be produced.
			{
				pTree->seednewly_produced=(int) floor( parameter[0].seedprodfaktor	// seed production in dependence of a factor
												* pTree->height/100					// ... the tree's  height...
												* pTree->dbasalrel					// ... the tree's current growth...
												* (1.0-(1.0/(pTree->height/50))) );	// ... height.
			} 
			
			else
			{
				++pos;
			}
		} //end tree_list iteration
	}//end plot list iteration

}
