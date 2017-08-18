/****************************************************************************************//**
 * \brief ageing of seeds and trees
 *
 * 1. seed ageing and mortality of seeds>3 years
 * 2. ageing of trees
 * 3. calculate maturation height of trees, that don't have one yet
 * 4. if tree is taller than its maturation height it grows cones
 * 5. if tree has cones already
 *
 *******************************************************************************************/

 
void Ageing( struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list)
{

	for (vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		
		list<seed*>& seed_list = *posw;

		for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); )
		{ 
			pseed=(*pos);
			pseed->age++;
			
			///seeds older than 2 years (L.gmelinii) and 10 years (L.sibirica) die
			if ((pseed->species==1)&&(pseed->age>parameter[0].gmel_seedtod))
			{
				delete pseed;
				pos=seed_list.erase(pos);
			}
			else if ((pseed->species==2)&&(pseed->age>10)) 
			{
				delete pseed;
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

			//If tree does not bear cones
			if (pTree->cone==0)
			{ 
				
				//...and does not have a maturation height assigned to it
				if (pTree->coneheight==99999)
				{  
		
					//trees reaching the maturation age are assigned a minimum height value for them to bear cones
					//coneheight = this minimal height
					//coneage = 15 (from PE) //???

					///check if tree is old enough to get cones
					if (pTree->age>parameter[0].coneage)
					{
						/////**********************THIS IS SOMEWHAT DIFFERENT IN THE NEWEST VERSION:
						//calculate random position in array maturationheight 
						//in this there are ratios between 0 and 182 (corresp. to (0,1) )
						
						//***german:
						// berechne zufällige Position in maturationshöhe[]-array
						//im Array maturationsheight sind fuer Anteilswerte 0 bis 182 (entspr. (0,1) ) Werte vorhanden
						
						//calculate random position in the array of maturation heights defined earlier
						int anteil= 0 + (int) floor( ( (mat_age_length-1) * rand()/(RAND_MAX + 1.0)) );
						
						// Chance fuer kleiner 2 m zu maturieren
						if (anteil==0)
						{ 
							pTree->coneheight= 100 +( (double) 100*rand()/(RAND_MAX + 1.0));
						}
						///teile Tree zufällig die Höhe mit, ab der er cone trägt
						else
						{
							pTree->coneheight=maturationheight[anteil]; 
						}
					}
				}
				
				//Tree already has a height of maturation assigned to it
				
				//***german:
				//Tree hat bereits Maturierationsheight zugewiesen bekommen
				else if (pTree->coneheight!=99999)
				{  							
					//If a tree is taller than this maturation height, he grows cones
					
					//***german:
					/// Wenn ein Tree groeszer ist als diese Maturationshöhe, so bekommt er cone	
					
					
					if (pTree->height >= pTree->coneheight)
						pTree->cone=1;
					//cout<<"cone established \n";
				} 

				++pos;
			} 
			

			//Tree bears cones
			else if (pTree->cone==1)
			{ 
				/// counting seeds
				pTree->seedproduced+=pTree->seednewly_produced; 
				pTree->seednewly_produced=0;

				++pos;
			}	
			
			else
			{	
				//Check if the substructe cone was set
				//otherwise it is asked if the program should be interrupted
				//
				
				//***german:
				// Sicherheitsabfrage cone-Variable gesetzt? 
				/* Abfrage ob das Programm beendet oder fortgesetzt werden soll */ 
				signed int abbrechenAgeingfehler; 
				
				printf("\n A tree has no value in the substructure cone in function ageing \n"); 
				printf("\n Continue the simulation with 1, stop it with any other key\n"); 
				
				//***german:
				//printf("\n In der agesfunktion hat ein Tree keinen Wert in der Variable pTree->cone\n"); 
				//printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n"); 
				scanf("%d", &abbrechenAgeingfehler); 
				
				if (abbrechenAgeingfehler!=1) 
				{
					printf("LAVESI was stopped after a failure in ageing.cpp\n\n");
					exit(1);
					
					//***german:
					//printf("LaVeSi wurde nach einem Fehler in der agesfunktion beendet\n\n");
					//exit(1);
				}
		
				delete pTree;
				pos=tree_list.erase(pos);						
			}

		} 

	}

}
