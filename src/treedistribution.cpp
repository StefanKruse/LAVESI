#include "LAVESI.h"

using namespace std;

/****************************************************************************************//**
 * \brief Seedinput in the beginning 
 *
 * if seedinput==true, start simulation with seedinput \n
 * recall if sites are empty of trees 
 *
 *******************************************************************************************/
 
void Seedin()
{
	int aktort=0;
	for (vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{
		list<Seed*>& seed_list = *posw;

		aktort++;
		
		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		bool seedinput;

		// seedinput on all sites
		if (parameter[0].realseedconnect==false)
		{
			seedinput=true;
		}
		//seedinput on southern site only
		else if (parameter[0].realseedconnect==true && aktortyworldcoo==(parameter[0].mapylength-1))
		{
			seedinput=true;
		}
		// no seedinput
		else
		{
			seedinput=false;
		}


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
			
						
			for (int n=0;n<seednobuffer;n++)
			{
				// calculate post-dispersal position
				double jseed, iseed;
				bool seedeintragen=false;

				// seedwinddispersalmode==1 => randomly from the south border.
				if (parameter[0].seedwinddispersalmode==1)
				{ 
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
					
					double dispersaldistance;
			
					do
					{
						double fraction;
				
						do
						{
							fraction= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));

						} while (fraction<=0.0);
						dispersaldistance= (log(fraction)/(-0.2))/parameter[0].distanceratio;

					} while ( (dispersaldistance>= (double) (treerows-1)) || (dispersaldistance<0.0) );

					seedeintragen=true;
					iseed=dispersaldistance;
				} 
				// seedwinddispersalmode==2 => randomly all over the plot
				else if (parameter[0].seedwinddispersalmode==2)
				{ 
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
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
				
				double seedzufall=0.0;
				int specieszufall=0;
				if(parameter[0].specpres==0)
				{
					seedzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
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
				
				if (seedeintragen==true)
				{  
					pSeed= new Seed();
					
					pSeed->yworldcoo=aktortyworldcoo;
					pSeed->xworldcoo=aktortxworldcoo;
					pSeed->xcoo=jseed;
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
					pSeed->releaseheight=0;
					pSeed->thawing_depthinfluence=100;
					
					seed_list.push_back(pSeed);	
					
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
		FILE *f;
		f=NULL;
		
		// set up of initial trees from different files
		if (parameter[0].starttrees!=0)
		{
			int aktort=0;
			for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); posw++)
			{
				list<Tree*>& tree_list = *posw;	

				aktort++;

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
				else if (parameter[0].starttrees==120100050)
				{
					f = fopen("input/CH17I_Treevert2011_100_50000.csv","r");
					printf("load: input/CH17I_Treevert2011_100_50000.csv");
				}
					
				if (f==NULL)
				{
					printf("Tree input file not available!\n");
					exit(1);
				}

				char puffer[255];
				int counter=1;
				
				double ybuffer,ycoobuf, xbuffer,xcoobuf;
				int conebuf, agebuf;
				double heightbuf, dbasalbuf , dbreastbuf;
				
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
						dbreastbuf=strtod(strtok(NULL, ";"),NULL);
						sscanf(strtok(NULL, ";"), "%d", &conebuf);
						sscanf(strtok(NULL, ";"), "%d", &agebuf);

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
						pTree->dbreast=dbreastbuf;
						pTree->dbreastrel=1.0;
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
			}
		}
}




 /****************************************************************************************//**
 * \brief Seedinput from hinterland
 *
 * ... parameter settings
 * hinterland>0 -> 0 means no, otherwise the number is the length in meters that is south of the simultation area
 *
 
 * 1. estimate number for 20 m slices
 * 1.1. take "fly out" probability but not for each but generally the proportion
 * 1.2. define random x/y start for each seed
 * 2. estimate mean height for each 20 m slice
 * 3. estimate dx/dy by Seedwinddispersal()
 * 4. place new seed to seedlist
 *******************************************************************************************/
 
void Hinterlandseedintro( struct Parameter *parameter, int yearposition, vector<list<Seed*> > &world_seed_list, vector<vector<Weather*> > &world_weather_list)
{
	
	// general hinterland parameter
	// int hinterland_maxlength=200000; // maximum length in meters to the south for additional seed introduction from Hinterland
		// ... cut into 20 m-long pieces, start at 10 with steps of by 20 to determine the centre for each seed introduction nuclei
	
	// function parameters
	double logmodel_seeds_Po=7.8997307;
	double logmodel_seeds_r=-0.6616466;
	double logmodel_seeds_K=2700.0081918;
	
	double logmodel_heights_Po=6.0897390;
	double logmodel_heights_r=-0.5932627;
	double logmodel_heights_K=353.5688136;

	// set "trees" directly as seeds with the relevant information
	// ... for each tree the mean number of seeds and height at the position at the hinterland
	// ... jultemp based on position
	int aktort=0;
	for (vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{
		list<Seed*>& seed_list = *posw;

		// grant weather access
		vector<vector<Weather*> >::iterator world_positon_weather = (world_weather_list.begin()+aktort);
		vector<Weather*>& weather_list = *world_positon_weather;
		
		aktort++;
		
		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
		
#ifdef DEBUG
		cout << " ... seed_list.size=" << seed_list.size() << endl;
#endif
		
		if (parameter[0].hinterland_maxlength>0)
		{
			int seednobuffer; 
			
#ifdef DEBUG
			// debugging output
			int introduced=0;
			int exceedstoN=0;
			int exceedstoE=0;
			int exceedstoS=0;
			int exceedstoW=0;
#endif

			for(int yposhint=-10; yposhint>-1*parameter[0].hinterland_maxlength;yposhint=yposhint-20)
			{
				double jultempi=weather_list[yearposition]->temp7monthmean + ( -0.3508 * yposhint/(111120) );//conversion to degree latitude see...weatherinput.cpp
									
				double hinterheightsi=logmodel_heights_K/(1+exp(logmodel_heights_Po+logmodel_heights_r*jultempi));
				int hinterseedsi=(int) floor(parameter[0].seedflightrate * logmodel_seeds_K/(1+exp(logmodel_seeds_Po+logmodel_seeds_r*jultempi)));
				
				for (int n=0;n<hinterseedsi;n++)
				{
					// calculate post-dispersal position with the parameters set for the simulation run using the wind-dependent seed dispersal function
					
						// x y start random in field
						double xseed, yseed;
						bool introduceseed=true;
						
						xseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));//x coo start
						yseed= (yposhint-10) + ( (double)  ( ((double) (20)) *rand()/(RAND_MAX + 1.0)));// y coo start
				
						// define species
						double seedzufall=0.0;
						int specieszufall=0;
						if(parameter[0].specpres==0)
						{
							seedzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
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

						
						
						// estimation of new positions
						double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
						double dispersaldistance=0.0;
						double direction=0.0;
						double velocity=0.0;
						double wdirection=0.0;
						double jquer=0;
						double iquer=0;
						
						Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, hinterheightsi, specieszufall);

						xseed=xseed+jquer;
						yseed=yseed+iquer;
									
						// manipulate position according to boundary conditions if lands in simulated plot add to seedlist ...code adapted from seeddispersal.cpp
						if(yseed > (double) (treerows-1)) 
						{
							if((parameter[0].boundaryconditions==1))
							{
							   yseed=fmod(yseed,(double)(treerows-1));
							} 
							else if((parameter[0].boundaryconditions==3))
							{
#ifdef DEBUG
								exceedstoN++;
#endif
								introduceseed=false;
							} 
							else 
							{
#ifdef DEBUG
								exceedstoN++;
#endif
								introduceseed=false;
							}
						} 
						else if(yseed<0.0)
						{
							if((parameter[0].boundaryconditions==1))
							{
							   yseed=(double)(treerows-1)+fmod(yseed,(double)(treerows-1));
							} 
							else if((parameter[0].boundaryconditions==3))
							{
#ifdef DEBUG
								exceedstoS++;
#endif
								introduceseed=false;
							} 
							else 
							{
#ifdef DEBUG
								exceedstoS++;
#endif
								introduceseed=false;
							}
						}
						 
						if(xseed<0.0)
						{
							if((parameter[0].boundaryconditions==1 || parameter[0].boundaryconditions==3))
							{
								xseed = fmod(xseed,(double)(treecols-1))+(double)(treecols-1);
							} 
							else
							{
#ifdef DEBUG
								exceedstoW++;
#endif
								introduceseed=false;
							}
						} 
						else if(xseed > (double) (treecols-1))
						{
							if(parameter[0].boundaryconditions==1 || parameter[0].boundaryconditions==3)
							{
								xseed = fmod(xseed,(double)(treecols-1));
							} 
							else if((parameter[0].boundaryconditions==2) && (rand()<0.5*RAND_MAX))
							{ // Reducing seed introduction on the western border:
								xseed = fmod(xseed,(double)(treecols-1));
							} 
							else
							{	
#ifdef DEBUG
								exceedstoE++;
#endif
								introduceseed=false;
							}
						} 
									
						// add new seed to seedlist
						if (introduceseed==true)
						{  
#ifdef DEBUG
							introduced++;
#endif
							
							pSeed= new Seed();
							
							pSeed->yworldcoo=aktortyworldcoo;
							pSeed->xworldcoo=aktortxworldcoo;
							pSeed->xcoo=xseed;
							pSeed->ycoo=yseed;
							pSeed->namem=0;
							pSeed->namep=0;
							pSeed->line=++parameter[0].lineakt;
							pSeed->generation=0;
							pSeed->incone=false;
							pSeed->weight=1;
							pSeed->age=0;
							pSeed->longdispersed=false;
							pSeed->species=specieszufall;
							pSeed->releaseheight=hinterheightsi;
							pSeed->thawing_depthinfluence=100;
							pSeed->dispersaldistance=dispersaldistance;	
							
							seed_list.push_back(pSeed);	
							
							if ( (pSeed->yworldcoo<0.0) | (pSeed->yworldcoo> (double) (treerows-1)) | (pSeed->xcoo<0.0) | (pSeed->xcoo> (double) (treecols-1)) )
							{
								printf("\n\nLaVeSi was stopped\n");
								printf("=> Treedistribution.cpp\n");
								printf("... reason: new seed has coordinates beyond the plots borders (with Pos(Y=%4.2f,X=%4.2f))\n", yseed, xseed);
								exit(1);
							}
						}
				}
			
			}
			
#ifdef DEBUG
			cout << "C: introduced seeds=" << introduced << " ----> N=" << exceedstoN << "  E=" << exceedstoE << "  S=" << exceedstoS << "  W=" << exceedstoW << endl;
#endif
		}
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
	// either seed introduction...
	if ((parameter[0].starter==true))
	{
		Seedin();
	}			
	// ... or use initial tree input data files
	else
	{
		TreesIni(stringlengthmax);
	}
}

