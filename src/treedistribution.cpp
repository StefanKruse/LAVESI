using namespace std;



/****************************************************************************************//**
 * \brief Seedinput in the beginnig 
 *
 * if seedinput==true, start simulation with seedinput \n
 * recall if sites are empty of trees 
 *
 *******************************************************************************************/
void Seedin()
{	

	int aktort=0;
	for (vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ // world seed list loop start
		list<Seed*>& seed_list = *posw;

		aktort++;
		
		// Calculation of the transect coordinates
		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		bool seedinput;
		

		// realseedconnect is a bool (1/0) 
		// if realseedconnect==1: seed introduction only on the most southern plot

		//seedinput on all sites
		if (parameter[0].realseedconnect==false) 
			seedinput=true;


		//seedinput on southern site only
		else if (parameter[0].realseedconnect==true && aktortyworldcoo==(parameter[0].mapylength-1)) seedinput=true;


		// no seedinput
		else seedinput=false;
			



		///start seedinput
		if (seedinput==true)
		{
			int seednobuffer; 
			if (parameter[0].yearswithseedintro<=0)
			{
				seednobuffer=parameter[0].seedintronumberpermanent;
			}
			else
			{
				seednobuffer=parameter[0].seedintronumber;
			}
			
						
			for (int n=0;n<seednobuffer;n++)//seednobuffer determines number of seeds (zB seedintronumber=1000 per year on the whole plot.)
			{
				// calculate the landing site for each seed
				double jseed, iseed;
				bool seedeintragen=false;

				/// Seedwinddispersalmode = kind of seed introduction of all starter seeds 
				/// 1== exponentially from the southern border of each plot
				if (parameter[0].Seedwinddispersalmode==1)//Seedwinddispersalmode==1 =>randomly from the south border.
				{ 
					
					// random x position:
					// x-value
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
					
					// y-value
					double dispersaldistance;
			
					do
					{
						double anteil;
				
						do
						{
							anteil= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));

						} while (anteil<=0.0);
						dispersaldistance= (log(anteil)/(-0.2))/parameter[0].distanceratio;

					} while ( (dispersaldistance>= (double) (treerows-1)) || (dispersaldistance<0.0) );

					seedeintragen=true;
					iseed=dispersaldistance;
							
				} 
						
					
				/// Seedwinddispersalmode==2=>randomly all over the plot
				else if (parameter[0].Seedwinddispersalmode==2)
				{ 
					// x value
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
				
					// y value
					iseed= 0.0 + ( (double)  ( ((double) (treerows-1)) *rand()/(RAND_MAX + 1.0))); 
					
					seedeintragen=true;
                }
                 else 
				{
					printf("\n\nLaVeSi was stopped\n");
					printf("=> Treedistribution.cpp\n");
					printf("... reason: no choice of a seed dispersal mode");
					exit(1);
				}
				
				double seedzufall = 0.0;
				int specieszufall = 0;
				if(parameter[0].specpres==0)
				{
					seedzufall = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					if ((seedzufall >= 0.0) && (seedzufall <= 0.5))
					{
						specieszufall = 1;
					}
					else if ((seedzufall > 0.5) && (seedzufall <= 1.0))
					{
						specieszufall = 2;
					}
				}
				else if (parameter[0].specpres==1)
				{
					specieszufall = 1;
				}
				else if (parameter[0].specpres==2)
				{
					specieszufall = 2;
				}
				
				///place new seed
				if (seedeintragen==true)
				{  
					// add new seed
					pSeed= new Seed();					// 1. generate new seed
					pSeed->yworldcoo=aktortyworldcoo;
					pSeed->xworldcoo=aktortxworldcoo;
					pSeed->xcoo=jseed;					// 2. apply values to the seed
					pSeed->ycoo=iseed;
					pSeed->namem=0;
					pSeed->namep=0;
					pSeed->line=++parameter[0].lineakt;
					pSeed->generation=0;
					pSeed->incone=false;
					pSeed->weight=1;
					pSeed->age=0;
					pSeed->longdispersed=false;
					pSeed->species=specieszufall;
					pSeed->elternheight=0;
					
					pSeed->thawing_depthinfluence=100;
					
					seed_list.push_back(pSeed);			// 3. push back seed to the seed_list
					
					
//hier drunter die Abfrage ist vermutlich fehlerhaft
					if ( (pSeed->yworldcoo<0.0) | (pSeed->yworldcoo> (double) (treerows-1)) | (pSeed->xcoo<0.0) | (pSeed->xcoo> (double) (treecols-1)) )
					{
						printf("\n\nLaVeSi was stopped\n");
						printf("=> Treedistribution.cpp\n");
						printf("... reason: new seed has coordinates beyond the plots borders (with Pos(Y=%4.2f,X=%4.2f))\n", iseed, jseed);
						exit(1);
					}
				
				} 

			} 	
		
		}
	
	} 

}






/****************************************************************************************//**
 * \brief Set starting population to CH-17/I and variations
 *
 * read file and place trees according to their coordinates
 *
 *
 *******************************************************************************************/
void TreesIni(int maximal_word_length)
{
		// open input file
		FILE *f;
		f=NULL;
		
		if (parameter[0].starttrees!=0)
		{// set up of initial trees from different files
			
			int aktort=0;
			for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); posw++)
			{
				list<Tree*>& tree_list = *posw;	

				aktort++;
				// calculation of the current transect coordinate
				int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
				int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

				if (parameter[0].starttrees==12)
				{
					f = fopen("input/CH17I_Treevert2011.csv","r");
					printf("load: input/CH17I_Treevert2011.csv");
				}
				else if (parameter[0].starttrees==120100)
				{
					f = fopen("input/CH17I_Treevert2011_100_100.csv","r");
					printf("load: input/CH17I_Treevert2011_100_100.csv");
				}
				else if (parameter[0].starttrees==120500)
				{
					f = fopen("input/CH17I_Treevert2011_500_500.csv","r");
					printf("load: input/CH17I_Treevert2011_500_500.csv");
				}
				else if (parameter[0].starttrees==121000)
				{
					f = fopen("input/CH17I_Treevert2011_1000_1000.csv","r");
					printf("load: input/CH17I_Treevert2011_1000_1000.csv");
				}
				else if (parameter[0].starttrees==122000)
				{
					f = fopen("input/CH17I_Treevert2011_2000_2000.csv","r");
					printf("load: input/CH17I_Treevert2011_2000_2000.csv");
				}
				else if (parameter[0].starttrees==125000)
				{
					f = fopen("input/CH17I_Treevert2011_5000_5000.csv","r");
					printf("load: input/CH17I_Treevert2011_5000_5000.csv");
				}
				else if (parameter[0].starttrees==120100050)
				{
					f = fopen("input/CH17I_Treevert2011_100_50000.csv","r");
					printf("load: input/CH17I_Treevert2011_100_50000.csv");
				}
				
					
				if (f == NULL)
				{
					printf("Tree input file not available!\n");
					exit(1);
				}

				char puffer[255];
				int counter=1;
				
				double ybuffer,ycoobuf, xbuffer,xcoobuf;
				int conebuf, agebuf;
				double heightbuf, dbasalbuf , dbrustbuf;
				
				// ignoring the header the contents are appended to the tree array line by line
				while(fgets(puffer,maximal_word_length,f) !=NULL)
				{
					if (counter>=2)
					{
						strtok(puffer, ";");
						sscanf(strtok(NULL, ";"), "%lg", &ybuffer);
						ycoobuf=ybuffer;
						sscanf(strtok(NULL, ";"), "%lg", &xbuffer); 
						xcoobuf=xbuffer;
						heightbuf=strtod(strtok(NULL, ";"),NULL);
						dbasalbuf=strtod(strtok(NULL, ";"),NULL);
						dbrustbuf=strtod(strtok(NULL, ";"),NULL);
						sscanf(strtok(NULL, ";"), "%d", &conebuf);
						sscanf(strtok(NULL, ";"), "%d", &agebuf);

						// add new trees
						pTree= new Tree();
						
						pTree->yworldcoo=aktortyworldcoo;
						pTree->xworldcoo=aktortxworldcoo;	
						pTree->xcoo=(double) xcoobuf;
						pTree->ycoo=(double) ycoobuf;
						pTree->name=++parameter[0].nameakt;
						pTree->namem=0;							
						pTree->namep=0;	
						pTree->line=++parameter[0].lineakt;
						pTree->generation=0;
						pTree->yr_of_establishment=0;
						pTree->dbasal=dbasalbuf;
						pTree->dbasalrel=1.0;
						pTree->dbrust=dbrustbuf;
						pTree->dbrustrel=1.0;
						pTree->height=heightbuf;
						pTree->age=agebuf; 
						pTree->cone=conebuf;				
						if (pTree->cone==0)
						{
							pTree->coneheight=99999.0;
						}
						pTree->seednewly_produced=0; 
						pTree->seedproduced=0; 	
						pTree->buffer=1;	
						pTree->densitywert=0;
						pTree->thawing_depthinfluence=100;	
						pTree->growing=true;		
						if(parameter[0].specpres==0 || parameter[0].specpres==1)
						{
							pTree->species=1;
						}
						if (parameter[0].specpres==2)
						{
							pTree->species=2;
						}	
						
						tree_list.push_back(pTree);
					}
					counter++;
				}

				fclose(f);
			}// world tree list
		}
		else
		{
			// without initial trees
		}
}










/****************************************************************************************//**
 * \brief Start either with empty site and seed input or with 11-CH-17/I tree coordinates
 *
 * 
 *
 *******************************************************************************************/
void Treedistribution(struct Parameter *parameter, int stringlengthmax)
{
	
	// Either seed introduction...
	if ((parameter[0].starter==true))
		Seedin();
			
	// ... or use initial tree input data files
	else
		TreesIni(stringlengthmax);

}

