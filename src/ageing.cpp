using namespace std;

/****************************************************************************************//**
 * \brief ageing of seeds and trees
 *
 * 1. seed ageing and mortality of seeds reach their maximum age
 * 2. ageing of trees
 * 3. calculate maturation height of trees, that don't have one yet
 * 4. if tree is taller than its maturation height it grows cones
 * 5. if tree has cones already
 *
 *******************************************************************************************/
 
void Ageing( struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<Seed*> > &world_seed_list)
{
	for (vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<Seed*>& seed_list = *posw;

		for (list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pSeed=(*pos);
			pSeed->age++;
			
			// seeds older than gmelseedmaxage years (L.gmelinii) and 10 years (L.sibirica) die
			if ((pSeed->species==1) && (pSeed->age>parameter[0].gmelseedmaxage))
			{
				delete pSeed;
				pos=seed_list.erase(pos);
			}
			else if ((pSeed->species==2) && (pSeed->age>10)) 
			{
				delete pSeed;
				pos=seed_list.erase(pos);
			}
			else
			{
				++pos;	
			}
		}
	}

	int mat_age_length=183; // length of array maturationheight
	// height values in percent (0-99) computed using R
	double maturationheight[]={
								200,		201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	
								211,		212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	
								222,		223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	
								233,		234,	235,	236,	237,	238,	239,	240,	241,	242,	243,	
								244,		245,	246,	248,	249,	250,	251,	252,	253,	254,	256,	
								257,		258,	259,	260,	262,	263,	264,	266,	267,	269,	270,	
								271,		273,	274,	276,	277,	279,	281,	282,	284,	286,	287,	
								289,		291,	293,	295,	297,	299,	300,	303,	305,	307,	309,	
								311,		313,	316,	318,	320,	323,	325,	328,	330,	333,	336,	
								339,		341,	344,	347,	350,	354,	357,	360,	364,	367,	371,	
								374,		378,	382,	386,	390,	395,	399,	404,	408,	413,	418,	
								423,		428,	434,	439,	445,	451,	458,	464,	471,	478,	485,	
								493,		500,	509,	517,	526,	535,	545,	555,	566,	577,	588,	
								600,		613,	627,	641,	656,	672,	689,	707,	725,	746,	767,	
								790,		815,	841,	870,	900,	934,	970,	1010,	1053,	1100,	1153,	
								1212,		1277,	1350,	1434,	1529,	1639,	1767,	1919,	2100,	2323,	2600,	
								2958,		3434,	4100,	5100,	6767,	10100,	20100	
							};

	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;
		
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);
			pTree->age++;

			if (pTree->cone==0)
			{ 
				if (pTree->coneheight==99999)
				{  
		
					// trees reaching the maturation age are assigned a minimum height value for them to bear cones
					if (pTree->age>parameter[0].coneage)
					{
					
						// calculate random position in the array of maturation heights defined earlier
						// ... in this there are values between 0 and 182 (corresp. to (0,1) )
						int fraction= 0 + (int) floor( ( (mat_age_length-1) * rand()/(RAND_MAX + 1.0)) );
						
						// possibility for a tree <2m to maturate
						if (fraction==0)
						{ 
							pTree->coneheight= 100 +( (double) 100*rand()/(RAND_MAX + 1.0));
						}
						else
						{
							pTree->coneheight=maturationheight[fraction]; 
						}
					}
				}
				// tree already has a height of maturation assigned to it
				// ... if a tree is taller than this maturation height, he starts to produce seeds
				else if (pTree->coneheight!=99999)
				{  							
					if (pTree->height >= pTree->coneheight)
					{
						pTree->cone=1;
					}
				} 

				++pos;
			}
			// tree bears cones
			else if (pTree->cone==1)
			{ 
				pTree->seedproduced+=pTree->seednewly_produced; 
				pTree->seednewly_produced=0;

				++pos;
			}	
			else
			{	
				// check if cones were correctly assigned
				signed int exitageingerror; 
				
				printf("\n A tree has no value in the substructure cone in function ageing \n"); 
				printf("\n Continue the simulation with 1, stop it with any other key\n"); 

				scanf("%d", &exitageingerror); 
				
				if (exitageingerror!=1) 
				{
					printf("LAVESI was stopped after a failure in ageing.cpp\n\n");
					exit(1);
				}
		
				delete pTree;
				pos=tree_list.erase(pos);						
			}

		} 

	}

}
