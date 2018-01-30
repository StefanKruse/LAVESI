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
	for (vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ // world seed list loop start
		list<seed*>& seed_list = *posw;

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
			if (parameter[0].jahremitseedeintrag<=0)
			{
				seednobuffer=parameter[0].etabbgpermanent;
			}
			else
			{
				seednobuffer=parameter[0].etabbg;
			}
			
						
			for (int n=0;n<seednobuffer;n++)//seednobuffer determines number of seeds (zB etabbg=1000 per year on the whole plot.)
			{
				// calculate the landing site for each seed
				double jseed, iseed;
				bool seedeintragen=false;

				/// seeddispmode = kind of seed introduction of all starter seeds 
				/// 1== exponentially from the southern border of each plot
				if (parameter[0].seeddispmode==1)//seeddispmode==1 =>randomly from the south border.
				{ 
					
					// random x position:
					// x-value
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
					
					// y-value
					double entfernung;
			
					do
					{
						double anteil;
				
						do
						{
							anteil= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));

						} while (anteil<=0.0);
						entfernung= (log(anteil)/(-0.2))/parameter[0].distanceratio;

					} while ( (entfernung>= (double) (treerows-1)) || (entfernung<0.0) );

					seedeintragen=true;
					iseed=entfernung;
							
				} 
						
					
				/// seeddispmode==2=>randomly all over the plot
				else if (parameter[0].seeddispmode==2)
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
					pseed= new seed();					// 1. generate new seed
					pseed->yworldcoo=aktortyworldcoo;
					pseed->xworldcoo=aktortxworldcoo;
					pseed->xcoo=jseed;					// 2. apply values to the seed
					pseed->ycoo=iseed;
					pseed->namem=0;
					pseed->namep=0;
					pseed->line=++parameter[0].lineakt;
					pseed->generation=0;
					pseed->imcone=false;
					pseed->gewicht=1;
					pseed->age=0;
					pseed->longdispersed=false;
					pseed->species=specieszufall;
					pseed->elternheight=0;
					
					pseed->thawing_depthinfluence=100;
					
					seed_list.push_back(pseed);			// 3. push back seed to the seed_list
					
					
//hier drunter die Abfrage ist vermutlich fehlerhaft
					if ( (pseed->yworldcoo<0.0) | (pseed->yworldcoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) )
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
 * \brief Set starting population to 11-CH-17/I parameters
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
		{// Set up of initial trees from different files
			
			// Go to each single site along Y-Transect
			int aktort=0;
			for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); posw++)
			{	// world tree list loop
		
				// Um auf die Inhalte in den tree_listn zuzugreifen muss eine tree_list als Referenz
				// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
				// realisiert ist weiterverfahren werden
				// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
				list<Tree*>& tree_list = *posw;	

				aktort++;
				// calculation of the current transect coordinate
				int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
				int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

				// Load datafile as set by the parameter file
				// If all sites should be simulated, the content to be invoked depends on aktort (1==10, 2=11, 3=12 & 4=13)
				if (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==999)
				{
					if (parameter[0].weatherchoice==111 && aktort==1)
						// ==CH06		
						f = fopen("CH06III_Treevert2011.csv","r");
					else if (parameter[0].weatherchoice==111 && aktort==2)
						// ==CH02		
						f = fopen("CH02II_Treevert2011.csv","r");
					else if (parameter[0].weatherchoice==111 && aktort==3)
						// ==CH17	
						f = fopen("input/CH17I_Treevert2011.csv","r");
					else if (parameter[0].weatherchoice==111 && aktort==4)
						// ==CH12		
						f = fopen("CH12II_Treevert2011.csv","r");
					// Jetzt die moeglichen auf 1911 zurueckgerechneten trees
					else if (parameter[0].weatherchoice==999 && aktort==1)
						// ==CH06		
						f = fopen("CH06III_Treevert2011_REDUCEDto1911.csv","r");
					else if (parameter[0].weatherchoice==999 && aktort==2)
						// ==CH02		
						f = fopen("CH02II_Treevert2011_REDUCEDto1911.csv","r");
					else if (parameter[0].weatherchoice==999 && aktort==3)
						// ==CH17	
						f = fopen("CH17I_Treevert2011_REDUCEDto1911.csv","r");
					else if (parameter[0].weatherchoice==999 && aktort==4)
						// ==CH12		
						f = fopen("CH12II_Treevert2011_REDUCEDto1911.csv","r");
				}
				else if (parameter[0].starttrees==10)
					// ==CH06		
					f = fopen("CH06III_Treevert2011.csv","r");
				else if (parameter[0].starttrees==11)
					// ==CH02		
					f = fopen("CH02II_Treevert2011.csv","r");
				else if (parameter[0].starttrees==12)
				{
					// ==CH17	
					f = fopen("input/CH17I_Treevert2011.csv","r");
					printf("load: input/CH17I_Treevert2011.csv");
				}
				else if (parameter[0].starttrees==120100)
				{
					// ==CH17	
					f = fopen("input/CH17I_Treevert2011_100_100.csv","r");
					printf("load: input/CH17I_Treevert2011_100_100.csv");
				}
				else if (parameter[0].starttrees==120500)
				{
					// ==CH17	
					f = fopen("input/CH17I_Treevert2011_500_500.csv","r");
					printf("load: input/CH17I_Treevert2011_500_500.csv");
				}
				else if (parameter[0].starttrees==121000)
				{
					// ==CH17	
					f = fopen("input/CH17I_Treevert2011_1000_1000.csv","r");
					printf("load: input/CH17I_Treevert2011_1000_1000.csv");
				}
				else if (parameter[0].starttrees==122000)
				{
					// ==CH17	
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
				else if (parameter[0].starttrees==13)
					// ==CH12		
					f = fopen("CH12II_Treevert2011.csv","r");
				//StefanC: Jetzt die moeglichen auf 1911 zurueckgerechneten trees
				else if (parameter[0].starttrees==910)
					// ==CH06		
					f = fopen("CH06III_Treevert2011_REDUCEDto1911.csv","r");
				else if (parameter[0].starttrees==911)
					// ==CH02		
					f = fopen("CH02II_Treevert2011_REDUCEDto1911.csv","r");
				else if (parameter[0].starttrees==912)
					// ==CH17	
					f = fopen("CH17I_Treevert2011_REDUCEDto1911.csv","r");
				else if (parameter[0].starttrees==913)
					// ==CH12		
					f = fopen("CH12II_Treevert2011_REDUCEDto1911.csv","r");
					
				
					
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

				
				//***german:
				// dann wird Zeilenweise ab der 2. Zeile (1. Zeile sind Ueberschriften) verfahren
				// und die jeweiligen Inhalte in das Treearray geschrieben
				while(fgets(puffer,maximal_word_length,f) !=NULL)
				{
					if (counter>=2)
					{
						strtok(puffer, ";");						//split puffer, delimiter is ";"
						sscanf(strtok(NULL, ";"), "%lg", &ybuffer);
						ycoobuf=ybuffer/*-1*/;
						sscanf(strtok(NULL, ";"), "%lg", &xbuffer); 
						xcoobuf=xbuffer/*-1*/; // comment not needed anymore??
						heightbuf=strtod(strtok(NULL, ";"),NULL);
						dbasalbuf=strtod(strtok(NULL, ";"),NULL);
						dbrustbuf=strtod(strtok(NULL, ";"),NULL);
						sscanf(strtok(NULL, ";"), "%d", &conebuf);
						sscanf(strtok(NULL, ";"), "%d", &agebuf);



							// add new trees
							pTree= new Tree();			// 1. generate new tree
							pTree->yworldcoo=aktortyworldcoo;				//
							pTree->xworldcoo=aktortxworldcoo;				//
							pTree->xcoo=(double) xcoobuf;				// 2. apply values to the new tree
							pTree->ycoo=(double) ycoobuf;
							pTree->name=++parameter[0].nameakt;
							pTree->namem=0;							
							pTree->namep=0;							//
							pTree->line=++parameter[0].lineakt;
							pTree->generation=0;
							pTree->yr_of_establishment=0;				//
							pTree->dbasal=dbasalbuf;
								pTree->dbasalrel=1.0;
							//pTree->Dbasalliste.push_back(dbasalbuf);
							pTree->dbrust=dbrustbuf;
								pTree->dbrustrel=1.0;
							//pTree->Dbrustliste.push_back(dbrustbuf);			
							pTree->height=heightbuf;
							pTree->age=agebuf; 
							pTree->cone=conebuf;				
							if (pTree->cone==0)
								pTree->coneheight=99999.0;
							pTree->seednewly_produced=0; 			//
							pTree->seedproduced=0; 				//
							pTree->speicher=1;						//
							pTree->densitywert=0;
							pTree->thawing_depthinfluence=100;		//
							pTree->growing=true;					//
							if(parameter[0].specpres==0 ||parameter[0].specpres==1)
							{
								pTree->species=1;
							}
							if (parameter[0].specpres==2)
							{
								pTree->species=2;
							}	
							tree_list.push_back(pTree);// 3. push back tree in tree_list
					}
					counter++;
				}

				fclose(f);
			}//world tree list end
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

