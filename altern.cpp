/****************************************************************************************//**
 * \brief ageing of seeds and trees
 *
 * 1. seed ageing and mortality of seeds>3 years
 * 2. ageing of trees
 * 3. calculate maturation height of trees, that don't have one yet
 * 4. if tree is higher than its maturation height it gets cones
 *
 *******************************************************************************************/
void Altern(int baeumerows, int baeumecols, struct Parameter *parameter, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe)
{
	// seed
	for (vector<list<Samen*> >::iterator posw = WeltSamenListe.begin(); posw != WeltSamenListe.end(); ++posw)
	{ 
		list<Samen*>& SamenListe = *posw;

		for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); )
		{ 
			pSamen=(*pos);
			pSamen->alter++;

			// seeds older than 3 years die
			if (pSamen->alter>3) 
			{
				delete pSamen;
				pos=SamenListe.erase(pos);
			}

			else
			{
				++pos;
			}
		} 
	}


	// tree
	
	// declare the maturation height list
	int matalterlength=183; // length of array maturationshöhe
	double maturationshoehe[]={200,		201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	
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
							   1212,	1277,	1350,	1434,	1529,	1639,	1767,	1919,	2100,	2323,	2600,	
							   2958,	3434,	4100,	5100,	6767,	10100,	20100};

	for (vector<list<Baum*> >::iterator posw = WeltBaumListe.begin(); posw != WeltBaumListe.end(); ++posw)
	{ 
		list<Baum*>& BaumListe = *posw;

		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);
			
			// ageing of trees
			pBaum->alter++;

			
			// tree maturation
			if (pBaum->zapfen==0)
			{ 
				// if a tree does not bear cones yet calculate maturation height of trees 
				if (pBaum->zapfenhoehe==99999)
				{  
					// check if tree is old enough to get cones
					if (pBaum->alter>parameter[0].zapfenalter)
					{
						
						// randomly set the maturation height 
						int anteil= 0 + (int) floor( ( (matalterlength-1) * rand()/(RAND_MAX + 1.0)) );
						
						// add a change to maturate if a tree is smaller than 200 cm
						if (anteil==0)
						{ 
							pBaum->zapfenhoehe= 100 +( (double) 100*rand()/(RAND_MAX + 1.0));
						}
						else
						{
							pBaum->zapfenhoehe=maturationshoehe[anteil]; 
						}
					}
				}
				// if tree is higher than its maturation height it gets cones
				else if (pBaum->zapfenhoehe!=99999)
				{  
					if (pBaum->hoehe>=pBaum->zapfenhoehe)
					{
						pBaum->zapfen=1;
					}
				} 

				++pos;
			} 
			// ... or if cones are present
			else if (pBaum->zapfen==1)
			{ 
				// count seeds for later seed creation after seed mortality
				pBaum->samenproduziert+=pBaum->samenneuproduziert; 
				pBaum->samenneuproduziert=0;

				++pos;
			}	
			// ... or security check
			else
			{
				printf("\n\nLAVESI exited\n");
				printf("... altern.cpp\n");
				printf("... cause: a tree remains without the variable pBaum->zapfen\n"); 
				exit(1);
			}

		} 

	}

}

