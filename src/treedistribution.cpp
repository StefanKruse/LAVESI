/****************************************************************************************//**
 * \brief Seedinput in the beginnig 
 *
 * if seedinput==true, start simulation with seedinput \n
 * recall if sites are empty of trees 
 *
 *******************************************************************************************/
void Seedin()//für die klassifizierung nach vegetationstypen arbeite ich wieder mit seedin
{	

	int aktort=0;
	for (vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ // Weltschleife Start
		list<seed*>& seed_list = *posw;

		//vector<vector<weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
		//vector<weather*>& weather_list = *posiwelt;

		aktort++;
		
		// Berechnung des aktuellen Ortes
		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		bool seedinput;
		

		// realseedconnect ist logisch (1/0) 
		// ==> Wenn 1, dann werden nur in der suedlichsten Site seed (N=Grundetab) ausgebracht

		//seedinput on all sites
		if (parameter[0].realseedconnect==false) 
			seedinput=true;


		//seedinput on southern site only
		else if (parameter[0].realseedconnect==true && aktortyworldcoo==(parameter[0].mapylength-1)) 
			seedinput=true;


		// no seedinput
		else 
			seedinput=false;



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
			
			// etabbg = amount of seeds put on the site (independent of site-size)
			
			
			for (int n=0;n<seednobuffer;n++)//seednobuffer bestimmt Anzahl an seed (zB etabbg=1000 pro Jahr auf dem gesamten Plot.)
			{
				// fuer jeden seed den Zielort berechnen
				double jseed, iseed;
				bool seedeintragen=false;

				/// seeddispmode = Art des seedeinbringens der Starterseed 
				/// 1==auf jeder Site vom Sueden einbringen
				if (parameter[0].defTreevert==0 && parameter[0].seeddispmode==1)//seeddispmode==1 =>randomly from the south border.
				{ 
					
					// ziehe Zufallsposition und bringe x seed die im Feld landen von Sueden her ein 
					// x-Wert
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
					
					// y-Wert
					double entfernung;
			
					do
					{
						double anteil;
				
						// Stelle sicher, dass die Zufallszahl >0
						do
						{
							anteil= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));

						} while (anteil<=0.0);//invertiert denken, ist richtig.
						// sonst fuehrt der Wert 0 zu unendlich groszen Zahlen!

						entfernung= (log(anteil)/(-0.2))/parameter[0].distanceratio;

					} while ( (entfernung>= (double) (treerows-1)) || (entfernung<0.0) );

					seedeintragen=true;
					iseed=entfernung;
							
				} 
						
					
				/// 2==zufaellig ueberall einbringen
				else if (parameter[0].seeddispmode==2)//seeddispmode==2 =>randomly all over the plot.
				{ 
					// ziehe Zufallsposition und bringe x seed die im Feld landen von Sueden her ein 
					// x-Wert
					jseed= 0.0 + ( (double)  ( ((double) (treecols-1)) *rand()/(RAND_MAX + 1.0)));
				
					// y-Wert
					iseed= 0.0 + ( (double)  ( ((double) (treerows-1)) *0.2*rand()/(RAND_MAX + 1.0))); //the southern 20% of the plot
				
					seedeintragen=true;
				 if (parameter[0].defTreevert==1){
									  
				  iseed=(double) rand()/(RAND_MAX+1.0)*(double)(treerows-1);//*0.2;//maximum ist 0.2 mal länge
				  jseed=0.0+(double) rand()/(RAND_MAX+1.0)*(double)(treecols-1);//so überall 
				  
				  if((rand()<vegetationtype[(int)(iseed)][(int)(jseed)]*RAND_MAX)){//"taiga" mit Probedensitywert 0.5
																					 // oder"tundra" mit Probedensitywert 0.05
				  seedeintragen=true;//iseed+jseed evtl vertauschen
				  
				  }else{seedeintragen=false;}//"lake"
				  //
				  
 				  //n=seednobuffer;//entspricht weniger seed->schneller rechnen.
				  
				  //seedeintragen=true;
				}
			}else 
				{
					printf("\n\nLaVeSi wurde beendet\n");
					printf("Aktueller Schritt => Treeverteilung.cpp\n");
					printf("... Grund: Keine Auschoice eines Seeddispersalmodes");
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
					// neue seed hinzufuegen
					pseed= new seed();					// 1. Neuen seed erzeugen
					pseed->yworldcoo=aktortyworldcoo;
					pseed->xworldcoo=aktortxworldcoo;
					pseed->xcoo=jseed;					// 2. Werte dem seed zuweisen
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
					
					seed_list.push_back(pseed);			// 3. seed in Liste einfuegen
					
					
					
					
					
					
													//hier drunter die Abfrage ist vermutlich fehlerhaft
					if ( (pseed->yworldcoo<0.0) | (pseed->yworldcoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) )
					{
						printf("\n\nLaVeSi wurde beendet\n");
						printf("Aktueller Schritt => Treeverteilung.cpp\n");
						printf("... Grund: NEUER seed hat Koordinaten die ausserhalb der Flaeche liegen (mit Pos(Y=%4.2f,X=%4.2f))\n", iseed, jseed);
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
void TreesIni()
{
		// Zuerst wird die Datei geoeffnet, aus der gelesen werden soll
		FILE *f;
		f=NULL;
		
		if (parameter[0].starttrees!=0)
		{// Set up of initial trees from different files
			
			// Go to each single Site along Y-Transect
			int aktort=0;
			for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); posw++)
			{	// Weltschleife
				// Um auf die Inhalte in den tree_listn zuzugreifen muss eine tree_list als Referenz
				// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
				// realisiert ist weiterverfahren werden
				// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
				list<Tree*>& tree_list = *posw;	

				aktort++;
				// Berechnung des aktuellen Ortes
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
					
				//else if (parameter[0].starttrees==2012)
				//	VegeTypeIni();

					
				if (f == NULL)
				{
					printf("Treeverteilungsdatei nicht vorhanden!\n");
					exit(1);
				}

				
				char puffer[255];
				int counter=1;
				
				double ybuffer,ycoobuf, xbuffer,xcoobuf;
				int conebuf, agebuf;
				double heightbuf, dbasalbuf , dbrustbuf;

				// dann wird Zeilenweise ab der 2. Zeile (1. Zeile sind Ueberschriften) verfahren
				// und die jeweiligen Inhalte in das Treearray geschrieben
				while(fgets(puffer,wortlaengemax,f) !=NULL)//Hier bestimmt fgets die Anzahl an treesn..
				{
					if (counter>=2)
					{
						strtok(puffer, ";");						//split puffer, delimiter is ";"
						sscanf(strtok(NULL, ";"), "%lg", &xbuffer); //sscanf->
						xcoobuf=xbuffer/*-1*/; // -1 auskommentiert für Fläche CH17*7*4, da die Koordinaten bei 0 beginnen und %d in %lg (double) geändert
						sscanf(strtok(NULL, ";"), "%lg", &ybuffer);
						ycoobuf=ybuffer/*-1*/;
						heightbuf=strtod(strtok(NULL, ";"),NULL);
						dbasalbuf=strtod(strtok(NULL, ";"),NULL);
						dbrustbuf=strtod(strtok(NULL, ";"),NULL);
						sscanf(strtok(NULL, ";"), "%d", &conebuf);
						sscanf(strtok(NULL, ";"), "%d", &agebuf);



							// neue trees hinzufuegen
							pTree= new Tree();			// 1. Neuen Tree erzeugen
							pTree->yworldcoo=aktortyworldcoo;				//
							pTree->xworldcoo=aktortxworldcoo;				//
							pTree->xcoo=(double) xcoobuf;				// 2. Werte dem Tree zuweisen
							pTree->ycoo=(double) ycoobuf;
							pTree->name=++parameter[0].nameakt;
							pTree->namem=0;							// "//" means it stays the same in the vegetationtype-routine
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
							pTree->densitywert=0;//Nochmal nachschauen
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
							tree_list.push_back(pTree);// 3. Tree in Liste einfuegen
					}
					counter++;//um den header zu überspringen
				}

				fclose(f);
			}// Weltschleife Ende
		}
		else
		{
			// without initial trees
		}
}




void VegeTypeIni()
{
	//int aktort=0;??
	
	int rowtest=treecols-1,coltest=treerows-1;
	int i,j,k,l,rowcntr=0,colcntr=0;
	
	vector<vector<double>> values;
	vector<double> valueline ;
	
	string item;
	
	string filename="umweltdaten/test.txt";
	
	ifstream input(filename.c_str());
	
	
	for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//initialization w dummy 66
			vegetationtype[j][i]=66;
		}
	}
	
	for(string line;getline(input,line);){
		istringstream input(line);
		colcntr=0;
		while(getline(input,item,';')){
			valueline.push_back(atof(item.c_str()));
			colcntr++;
		}
		values.push_back(valueline);
		valueline.clear();
		rowcntr++;
	}
	input.close();
	
	/*for(k=0;k<rowcntr;k++){
	for(l=0;l<colcntr;l++){
				cout<<values[k][l]<<" ";
	}
	cout<<endl;
	}*/
	
	cout<<rowcntr<<"\t"<<colcntr<<endl;
	
	
if(rowcntr<rowtest){//kleine Umweltdatenmatrix auf große Datenmatrix (30*30  sieht richtig aus..)
	//cout<<"kl auf gr\n";
	for(k=0;k<rowcntr;k++){
	for(l=0;l<colcntr;l++){
		for(j=(int)k*(rowtest/rowcntr);j<ceil((k+1)*(rowtest/rowcntr));j++){
			for(i=(int)l*(coltest/colcntr);i<ceil((l+1)*(coltest/colcntr));i++){
				vegetationtype[j][i]=values[(rowcntr-1)-k][l];
				}
			}
	}
	}
	
	for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//replacement of dummy 66
			if((vegetationtype[j][i]==66)&&(i!=0)&&(vegetationtype[j][i-1]!=66)){
				vegetationtype[j][i]=vegetationtype[j][i-1];
			}else if((i==0)&&(j!=0)&&(vegetationtype[j][i]==66)&&(vegetationtype[j-1][i]!=66)){
				vegetationtype[j][i]=vegetationtype[j-1][i];//naja
			}
		}
	}
				
}else{//große Umweltdatenmatrix auf kleine Datenmatrix 
	//cout<<"gr auf kl\n";
	for(i=0;i<coltest;i++){
		for(j=0;j<rowtest;j++){
			vegetationtype[j][i]-=66;
			for(l=(int)i*((colcntr)/(coltest));l<(int)(i+1)*((colcntr)/(coltest));l++){
				for(k=(int)j*((rowcntr)/(rowtest));k<(int)(j+1)*((rowcntr)/(rowtest));k++){
					vegetationtype[j][i]+=values[(rowcntr-1)-k][l];
				}	
			}
			vegetationtype[j][i]/=(2*(colcntr*rowcntr)/(coltest*rowtest));
		}	
	}
	
		for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//replacement of dummy 66,
			if((vegetationtype[j][i]==66)&&(i==0)&&(j!=0)){
				vegetationtype[j][i]=vegetationtype[j-1][i];
			}else if((vegetationtype[j][i]==66)&&(i!=0)&&(j==0)){
				vegetationtype[j][i]=vegetationtype[j][i-1];//naja
			}else if((vegetationtype[j][i]==66)&&(j!=0)&&(i!=0)){vegetationtype[j][i]=vegetationtype[j][i-1];}
		}
		}
}

	//replace vegetationtypes with densities
	for(j=0;j<rowtest;j++){
	for(i=0;i<coltest;i++){
	if(vegetationtype[j][i]<0.1){
		vegetationtype[j][i]=0.0;
	}else if(vegetationtype[j][i]>1){
		vegetationtype[j][i]=0.9;//testing: 0.9 trees per grid cell in forest env.
	}else if(vegetationtype[j][i]==1){vegetationtype[j][i]=0.2;//testing: 0.4 trees per grid cell in tundra env.
	}//else{vegetationtype[j][i]=rand()/RAND_MAX;}
	}
	}


	//smoothing:
for(j=0;j<rowtest;j++){
	for(i=0;i<coltest;i++){
		if(vegetationtype[j][i]!=0){//do not smooth lakes
		if((i==0)&&(j!=0)&&(vegetationtype[j][i]!=vegetationtype[j-1][i])&&(vegetationtype[j-1][i]!=0)){
			vegetationtype[j][i]*=3.0;
			vegetationtype[j][i]+=vegetationtype[j-1][i];
			vegetationtype[j][i]*=0.25;
		}else if((i!=0)&&(j==0)&&(vegetationtype[j][i]!=vegetationtype[j][i-1])&&(vegetationtype[j][i-1]!=0)){
			vegetationtype[j][i]*=3;
			vegetationtype[j][i]+=vegetationtype[j][i-1];
			vegetationtype[j][i]*=0.25;
		}else if((j!=0)&&(i!=0)&&(vegetationtype[j][i]!=vegetationtype[j-1][i-1])&&(vegetationtype[j][i]!=0)&&(vegetationtype[j-1][i-1]!=0)){
			vegetationtype[j][i]*=2;
			vegetationtype[j][i]+=vegetationtype[j-1][i-1]+vegetationtype[j-1][i]+vegetationtype[j][i-1];
			vegetationtype[j][i]*=0.2;}
			}else{continue;}
		}
	}
	values.clear();
//	valueline.clear();
	//if(parameter[0].)
	cout<<"vegetypeini finished"<<endl;
}





/****************************************************************************************//**
 * \brief Start either with empty site and seed input or with 11-CH-17/I tree coordinates
 *
 * 
 *
 *******************************************************************************************/
void Treeverteilung(int treerows, int treecols, struct Parameter *parameter, int wortlaengemax, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list,  vector<vector<double>> &vegetationtype)
{
	
	///PROBLEM: SIEHE main.cpp/Vegetationdynamics()
	
	// Entweder wenn gewaehlt seed eintragen ...
	if ((parameter[0].starter==true))
		Seedin();
			
	// ... oder aus eine Dateien mit Treeverteilungen auf den Untersuchungsflaechen im Jahr 2011 einlesen
	else
		TreesIni();

}

