// compile the program with the "MAKE FILE" by typing in your linux-command line: "make all"

// header files
#include "libraries.h"
#include "strukturen.h"
#include "deklarationen.h"

// programme code files
#include "altern.cpp"
#include "baumverteilung.cpp"
#include "datenausgabe.cpp"
#include "etablierung.cpp"
#include "kartenupdate.cpp"
#include "mortalitaet.cpp"
#include "parametereingabe.cpp"
#include "samenausbreitung.cpp"
#include "samenproduktion.cpp"
#include "wachstum.cpp"
#include "wettereingabe.cpp"
#include "reset.cpp"

// declaration of global variables
int jahrPosition;


// declaration of functions
/****************************************************************************************//**
 * \brief Go through all functions for vegetation dynamics
 *
 * This function calls all other functions neccessary to determine growth, mortality, ageing etc
 * 
 *
 *
 *******************************************************************************************/
void vegetationDynamics(int jahrPosition, int jahr, int t)
{
			// update density map
			Kartenupdate(baeumerows, baeumecols, &parameter[0], jahrPosition, WeltKartenListe, WeltBaumListe, WeltWetterListe);

			// growth of tree indivduals
			Wachstum(baeumerows, baeumecols, &parameter[0], jahrPosition, WeltBaumListe, WeltWetterListe, WeltKartenListe);

			// seed dispersal
			Samenausbreitung(baeumerows, baeumecols, &parameter[0], WeltSamenListe);

			// seed production
			Samenproduktion(baeumerows, baeumecols, &parameter[0], WeltBaumListe, WeltSamenListe, WeltWetterListe,jahrPosition);

			// initial seed introduction
			if (parameter[0].seedintro==true && parameter[0].jahremitsameneintrag>0)
			{
				parameter[0].starter=true;
				
				Baumverteilung(baeumerows, baeumecols, &parameter[0], wortlaengemax, jahrPosition, WeltBaumListe, WeltSamenListe);
				
				parameter[0].jahremitsameneintrag--;
			}
			else if (parameter[0].seedintropermanent==true && parameter[0].jahremitsameneintrag<=0) 
			{
				parameter[0].starter=true;
				Baumverteilung(baeumerows, baeumecols, &parameter[0],  wortlaengemax, jahrPosition, WeltBaumListe, WeltSamenListe);
			}

			// tree germination
			Etablierung(baeumerows, baeumecols, &parameter[0], jahrPosition, WeltBaumListe, WeltSamenListe, WeltWetterListe, WeltKartenListe);

			// output
			Datenausgabe(baeumerows, baeumecols, t, jahr, &parameter[0], jahrPosition, WeltBaumListe, WeltSamenListe, WeltWetterListe, WeltKartenListe, WeltAuswertungsListe);
			
			// mortality
			Mortalitaet(baeumerows, baeumecols, &parameter[0], jahrPosition, WeltBaumListe, WeltSamenListe, WeltWetterListe, WeltKartenListe);

			// ageing
			Altern(baeumerows, baeumecols, &parameter[0], WeltBaumListe, WeltSamenListe);

 }








/****************************************************************************************//**
 * \brief Stabilization period - get stable state before starting the real simulation
 *
 *
 *
 *
 *******************************************************************************************/
void Einschwingphase()
{
		int t = -1;	//dummy variable for output

		// determine the time period to be used for the stabilization phase
		int firstyear, lastyear;
		if (parameter[0].wetterwahl==1001)
		{
			firstyear=1919;
			lastyear=firstyear+parameter[0].stabilfirstweatheryears;
		}
				
		// run the spin up
		// ... either with a specified length
		if (parameter[0].ivortmax>0 && parameter[0].stabilperiod==false)
		{ 
			parameter[0].einschwingen=true;
	
			printf("\nSpin up starts...");
			do
			{
				parameter[0].ivort++;
				
				printf("\n\tSimulation progress: %d of %d\n", parameter[0].ivort, parameter[0].ivortmax); 

				// sample a random year
				double x = rand()/(RAND_MAX + 1.0);
				int jahr= (firstyear+parameter[0].startlag) + (int) ( (double) ((lastyear-parameter[0].startlag)-firstyear)*x);		
			
				// calculate actual year position in list, according to first year in the Weather-List and the random year
				int jahrPosition = (WeltWetterListe[0][0]->jahr - jahr) * -1; 
				
				//go through all functions for vegetation dynamics
				vegetationDynamics(jahrPosition, jahr,t);
	
			} while (parameter[0].ivort<parameter[0].ivortmax);
		}
		// ... or with a threshold of dynamics to reach
		else if (parameter[0].ivortmax>0 && parameter[0].stabilperiod==true)
		{
			printf("\nStabilization period starts ...");
			// simulate dynamics with randomized weather until ...
			// ... the inter-year variation falls below a specified error rate (stabilpercentchangethreshold)
			// ... or if exceeding the specified maximum simulation years
			bool stabilized=false;
			bool stabilizedplots=false;
			do
			{
				parameter[0].ivort++;
				
				// sample a random year
				double x = rand()/(RAND_MAX + 1.0);
				int jahr= (firstyear+parameter[0].startlag) + (int) ( (double) ((lastyear-parameter[0].startlag)-firstyear)*x);		
			
				// calculate actual year position in list, according to first year in the Weather-List and the random year
				int jahrPosition = (WeltWetterListe[0][0]->jahr - jahr) * -1; 
				
				// go through all functions for vegetation dynamics
				vegetationDynamics(jahrPosition, jahr,t);

				// start not before two times the length of the initial seed introduction
				if(parameter[0].ivort> (parameter[0].jahremitsameneintrag*2))
				{
					stabilizedplots=true;

					// calculation of the deviance of the running mean state values and determination of a quasi-stable-state
					double meanpercentchange=0;
					int stabilizationtype=1;
					
					if (stabilizationtype==1)
					{
						int aktort=0;
						
						for (vector<vector<Auswertung*> >::iterator posausw = WeltAuswertungsListe.begin(); posausw != WeltAuswertungsListe.end(); ++posausw)
						{ 
							vector<Auswertung*>& AuswertungsListe = *posausw;
							vector<Auswertung*>::iterator posauswakt = AuswertungsListe.begin();
							pAuswertung=(*posauswakt);
			
							aktort++;
							
							// initialize for each location
							meanpercentchange=0;
					
							// based on Basal Area
							if (pAuswertung->BArunmeanliste.size()>1)
							{
								if(pAuswertung->BArunmeanliste[pAuswertung->BArunmeanliste.size()-1]!=0)
								{
									meanpercentchange+=fabs(
										(pAuswertung->BArunmeanliste[pAuswertung->BArunmeanliste.size()-2]-pAuswertung->BArunmeanliste[pAuswertung->BArunmeanliste.size()-1])
										/
										(pAuswertung->BArunmeanliste[pAuswertung->BArunmeanliste.size()-1])
										);
								}
								else
								{
									meanpercentchange+=0;
								}
							}
							// + N_0-40
							if (pAuswertung->nhoehe0b40runmeanliste.size()>1)
							{
								if(pAuswertung->nhoehe0b40runmeanliste[pAuswertung->nhoehe0b40runmeanliste.size()-1]!=0)
								{
									meanpercentchange+=fabs(
										(pAuswertung->nhoehe0b40runmeanliste[pAuswertung->nhoehe0b40runmeanliste.size()-2]-pAuswertung->nhoehe0b40runmeanliste[pAuswertung->nhoehe0b40runmeanliste.size()-1])
										/
										(pAuswertung->nhoehe0b40runmeanliste[pAuswertung->nhoehe0b40runmeanliste.size()-1])
										);
								}
								else
								{
									meanpercentchange+=0;
								}
							}
							// + N_40-200
							if (pAuswertung->nhoehe41b200runmeanliste.size()>1)
							{
								if(pAuswertung->nhoehe41b200runmeanliste[pAuswertung->nhoehe41b200runmeanliste.size()-1]!=0)
								{
									meanpercentchange+=fabs(
										(pAuswertung->nhoehe41b200runmeanliste[pAuswertung->nhoehe41b200runmeanliste.size()-2]-pAuswertung->nhoehe41b200runmeanliste[pAuswertung->nhoehe41b200runmeanliste.size()-1])
										/
										(pAuswertung->nhoehe41b200runmeanliste[pAuswertung->nhoehe41b200runmeanliste.size()-1])
										);
								}
								else
								{
									meanpercentchange+=0;
								}
							}
							// + N_200+
							if (pAuswertung->nhoehe201b10000runmeanliste.size()>1)
							{
								if(pAuswertung->nhoehe201b10000runmeanliste[pAuswertung->nhoehe201b10000runmeanliste.size()-1]!=0)
								{
									meanpercentchange+=fabs(
										(pAuswertung->nhoehe201b10000runmeanliste[pAuswertung->nhoehe201b10000runmeanliste.size()-2]-pAuswertung->nhoehe201b10000runmeanliste[pAuswertung->nhoehe201b10000runmeanliste.size()-1])
										/
										(pAuswertung->nhoehe201b10000runmeanliste[pAuswertung->nhoehe201b10000runmeanliste.size()-1])
										);
								}
								else
								{
									meanpercentchange+=0;
								}
							}					
							
							if (meanpercentchange>parameter[0].stabilpercentchangethreshold)
							{
								stabilizedplots=false;
							}
						}

					}
				
					if ((parameter[0].ivort>(parameter[0].jahremitsameneintrag*2) && stabilizedplots==true) || parameter[0].ivort>parameter[0].ivortmax)
					{
						stabilized=true;
					}
				}	
			} while (stabilized!=true);
		}
		
		parameter[0].einschwingen=false;
}





/****************************************************************************************//**
 * \brief Yearly time steps - run through the simulation for all years (simdauer)
 *
 * 
 *
 *
 *******************************************************************************************/
void Jahresschritte()
{
		printf("\nSimulation phase starts...");
		
		for (int t=0;t<parameter[0].simdauer-parameter[0].startlag;t++)
		{
			parameter[0].ivort++;
			
			printf("\n\tSimulation progress: %d of %d\n", t, parameter[0].simdauer-parameter[0].startlag); 

			// calculate current year
			int jahr=parameter[0].startjahr+t+parameter[0].startlag;

			jahrPosition = ((WeltWetterListe[0][0]->jahr-parameter[0].startjahr) * -1)+t+parameter[0].startlag; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year 

			if (parameter[0].jahranzeige ==true)
			{
				printf("\nLocations\tYear\tTime step\tSimulation length\n%zu/%d\t\t%d\t%d\t\t%d\n", WeltBaumListe.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
			}
			else 
			{
				if ((jahr%100)==0) 
				{
					printf("t=%d", jahr); 
				}
			}
			
			// go through all functions for vegetation dynamics
			vegetationDynamics(jahrPosition, jahr,t);
			
			// save all data if later one wants to go back
			if (jahr==parameter[0].resetyear)
			{
				SaveAllLists();
				cout << "Saved all lists at year = " << jahr << endl << endl;
			}
		}

		// save at resetyear the complete data and resume from this with different manipulated temperature series
		if (parameter[0].resetyear>0)
		{
			// buffer for variables
			double tempdiffort_buffer=parameter[0].tempdiffort;
			
			// -0.5, 0 and +0.5 °C manipulated temperature series computation
			for(double tempdifforti=-0.5; tempdifforti<0.9; tempdifforti=tempdifforti+0.5)
			{
				parameter[0].tempdiffort=tempdifforti;
				
				// empty the WeltwetterListe
				for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); ++posw)
				{
					vector<Wetter*>& WetterListe = *posw;

					for (unsigned int iWetter=0; iWetter<WetterListe.size(); ++iWetter)	
					{
						pWetter=WetterListe[iWetter];
						delete pWetter;
					}
					WetterListe.clear();
				}
				// read and process the climate data
				Wettereinlesen(baeumerows, &parameter[0],  wortlaengemax, WeltWetterListe);
		
				cout << " START of simulation runs" << endl;
				
				// reset all lists
					ClearAllLists();
					cout << "           Lists deleted!!" << endl;
					RestoreAllLists();
					cout << "           Lists load!" << endl;
						
				printf("\n\nSimulation run starts ...\n");
				cout << "     Length of simulation=" << ((parameter[0].simdauer-(2011-parameter[0].resetyear))+1) << endl;
				
				for (int t=((parameter[0].simdauer-(2011-parameter[0].resetyear))+1);t<parameter[0].simdauer;t++)
				{
					
					parameter[0].ivort++;

					int jahr=parameter[0].startjahr+t;
					
					jahrPosition = ((WeltWetterListe[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year

					if (parameter[0].jahranzeige ==true) 
					{
						printf("\nSites at each Location\tYear\tTime Step\tDuration of Simulation\n%zu/%d\t\t%d\t%d\t\t%d\n", WeltBaumListe.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
					}
					else 
					{
						if ((jahr%100)==0) 
						{
							printf("t=%d", jahr); 
						}
					}

					// go through all functions for vegetation dynamics
					vegetationDynamics(jahrPosition, jahr,t);

					// save all data for later resume from the specified year
					if (jahr==parameter[0].resetyear)
					{
						SaveAllLists();
						cout << "At year = " << jahr << " all saved!" << endl << endl;
					}

					
				}

				parameter[0].tempdiffort=tempdiffort_buffer;
			}
		}
		
}




/****************************************************************************************//**
 * \brief Create all lists needed for the model
 *
 * 
 * (Welt)BaumListe \n
 * (Welt)SamenListe \n
 * (Welt)WetterListe \n
 * (Welt)KartenListe \n
 * (Welt)AuswertungsListe \n
 *******************************************************************************************/
void createLists()
{	
		for (int i=0;i< parameter[0].mapylength;i++)	
		{
			for (int j=0;j< parameter[0].mapxlength;j++)		
			{
				list<Baum*> BaumListe;
				WeltBaumListe.push_back(BaumListe);

				list<Samen*> SamenListe;
				WeltSamenListe.push_back(SamenListe);

				vector<Wetter*> WetterListe;
				WeltWetterListe.push_back(WetterListe);

				vector<Karten*> KartenListe;
				WeltKartenListe.push_back(KartenListe);

				vector<Auswertung*> AuswertungsListe;
				WeltAuswertungsListe.push_back(AuswertungsListe);
				
				
				if (parameter[0].resetyear>0)
				{
					list<Baum*> BaumListe;
					WeltBaumListe_copy.push_back(BaumListe);

					list<Samen*> SamenListe;
					WeltSamenListe_copy.push_back(SamenListe);

					vector<Karten*> KartenListe;
					WeltKartenListe_copy.push_back(KartenListe);

					vector<Auswertung*> AuswertungsListe;
					WeltAuswertungsListe_copy.push_back(AuswertungsListe);
				}
			}
		}
}







/****************************************************************************************//**
 * \brief Initialize Karten-, Element- and AuswertungsListe
 *
 *
 *
 *
 *
 *******************************************************************************************/
void initialiseMaps()
{
		int aktort=0;
		
		for (vector<vector<Karten*> >::iterator posw = WeltKartenListe.begin(); posw != WeltKartenListe.end(); posw++)
		{
			vector<Karten*>& KartenListe = *posw;
		
			vector<vector<Auswertung*> >::iterator posiwelt = (WeltAuswertungsListe.begin()+aktort);
			vector<Auswertung*>& AuswertungsListe = *posiwelt;
	
			aktort++;

			int aktortyweltcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxweltcoo=(aktort-1) - (aktortyweltcoo * parameter[0].mapxlength);
		
			// initialize the density grid
			for (int kartenpos=0; kartenpos< (baeumerows*parameter[0].sizemagnif*baeumecols*parameter[0].sizemagnif); kartenpos++) 
			{ 
				pKarten= new Karten();

				pKarten->yweltcoo=aktortyweltcoo;
				pKarten->xweltcoo=aktortxweltcoo;

				pKarten->ycoo=floor( (double) kartenpos/(baeumecols*parameter[0].sizemagnif) );
				pKarten->xcoo=(double) kartenpos - (pKarten->ycoo * (baeumecols*parameter[0].sizemagnif));

				pKarten->baumdichtewert=0;
				pKarten->baumanzahl=0;

				KartenListe.push_back(pKarten);	
			} 

			// initialize the list used for output creation and run time assessment of the population state
			pAuswertung=new Auswertung();
			pAuswertung->yweltcoo=aktortyweltcoo;
			pAuswertung->xweltcoo=aktortxweltcoo;
			pAuswertung->BAliste.clear();
			pAuswertung->BArunmeanliste.clear();
			pAuswertung->maxsteigungbasalarea=0.0;
			pAuswertung->countermaxsteigungbasalarea=0;
			pAuswertung->wendejahr=-9999;
			pAuswertung->saettigungsjahr=-9999;
			pAuswertung->nachwendejahr=false;
			pAuswertung->nhoehe0b40liste.clear();
			pAuswertung->nhoehe0b40runmeanliste.clear();
			pAuswertung->nhoehe41b200liste.clear();
			pAuswertung->nhoehe41b200runmeanliste.clear();
			pAuswertung->nhoehe201b10000liste.clear();
			pAuswertung->nhoehe201b10000runmeanliste.clear();
			pAuswertung->meanbreastdiameterliste.clear();
			pAuswertung->meanbreastdiameterrunmeanliste.clear();
			pAuswertung->stemcountliste.clear();
			pAuswertung->stemcountrunmeanliste.clear();
			pAuswertung->meantreeheightliste.clear();
			pAuswertung->meantreeheightrunmeanliste.clear();
			pAuswertung->meantreeageliste.clear();
			pAuswertung->meantreeagerunmeanliste.clear();
			AuswertungsListe.push_back(pAuswertung);
		}
}





/****************************************************************************************//**
 * \brief Start the simulation
 *
 * initialise some parameters \n
 * create lists \n
 * read and process weather \n
 * initilise Maps \n
 * introduce seeds \n
 * start Enschwingphase \n
 * start yearly timesteps \n
 *******************************************************************************************/
void runSimulation()
{
		// create WeltListen
		createLists();

		// read and process the weather data
		Wettereinlesen(baeumerows, &parameter[0],  wortlaengemax, WeltWetterListe);
			
		// initialize Karten- und AuswertungListe
		initialiseMaps();

		// introduce seeds
		Baumverteilung(baeumerows, baeumecols, &parameter[0], wortlaengemax, parameter[0].startjahr+3999, WeltBaumListe, WeltSamenListe);

		// get stable state before starting the real simulation // spin up
		Einschwingphase();
		
		// simulate for each specified year // simulation period
		Jahresschritte();
}






/****************************************************************************************//**
 * \brief Finish the simulation
 *
 * 
 * clear all lists
 *
 *
 *******************************************************************************************/
void finishSimulation(int jahrPosition)
{
		printf("\n\nFinal output\n");
		int aktort=0;

		for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); ++posw)
		{
			vector<Wetter*>& WetterListe = *posw;

			vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe.begin()+aktort);
			list<Baum*>& BaumListe = *posiweltb;

			vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe.begin()+aktort);
			list<Samen*>& SamenListe = *posiwelts;

			vector<vector<Karten*> >::iterator posiweltk = (WeltKartenListe.begin()+aktort);
			vector<Karten*>& KartenListe = *posiweltk;

			vector<vector<Auswertung*> >::iterator posiwelt = (WeltAuswertungsListe.begin()+aktort);
			vector<Auswertung*>& AuswertungsListe = *posiwelt;

			aktort++;

			int aktortyweltcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxweltcoo=(aktort-1) - (aktortyweltcoo * parameter[0].mapxlength);

			printf("Pos(Y,X)=(%d,%d) - JulyTmean(%d)=%4.2f - TreeNumber=%zu - SeedNumberGround=%zu - ", aktortyweltcoo, aktortxweltcoo, WetterListe[jahrPosition]->jahr, WetterListe[jahrPosition]->tempjahrmittel, BaumListe.size(), SamenListe.size());
			
			int samenimzapfenzahl=0;
			for(list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); pos++) 
			{
					if ((*pos)->imzapfen==true) 
					{
						samenimzapfenzahl++;
					}
			};
			printf("SeedNumberTrees= %d\n", samenimzapfenzahl);

			// delete Wetterlisten
			for (unsigned int iWetter=0; iWetter<WetterListe.size(); ++iWetter)	
			{
				pWetter=WetterListe[iWetter];
				delete pWetter;
			}
			WetterListe.clear();

		}
		
		// Function to delete evere element in Baum-, Samen-, Karten- und Auswertungslisten in WeltListen
		ClearAllLists();	
		// delete all Welt-lists
		WeltBaumListe.clear();
		WeltSamenListe.clear();
		WeltWetterListe.clear();
		WeltKartenListe.clear();
		WeltAuswertungsListe.clear();
		if (parameter[0].resetyear>0)
		{
			ClearAllLists_copy();
			
			WeltBaumListe_copy.clear();
			WeltSamenListe_copy.clear();
			WeltKartenListe_copy.clear();
			WeltAuswertungsListe_copy.clear();
		}
}






///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////						  /////////////////////////////////////////////////////////
/////////			MAIN			///////////////////////////////////////////////////////
///////////			 			  /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////




/****************************************************************************************//**
 * \brief Main routine to start model 
 *
 * read Parameters
 * start Simulation
 * finish Simulation
 *
 *******************************************************************************************/
int main()
{
	// seed random numbers
	srand((unsigned)time(0));
	
	printf("\n---->\tLaVeSi\n");
	printf("\n The Larix vegetation simulator LAVESI was started, " 
			"with this program vegetation dynamics of the species "
			"LARIX GMELINII (RUPR.) RUPR. can be simulated, "
			"driven by monthly weater data series."
			"\n\n Version:\t 1.01 - published in  Ecological Modelling"
			"\n Date:\t\t 13.02.2017" 
			"\n Authors:"
			"\n\t Stefan Kruse\tstefan.kruse@awi.de"
			"\n\t Mareike Wieczorek\tmareike.wieczorek@awi.de\n");
	printf("\n<----\n");

	// read in the parameter from parameter.txt
	Parametereinlesen();

	// intialize run time parameters and variables
	// ... calculate beginnig year for simulation runs
	parameter[0].startjahr=2011-parameter[0].simdauer;

	//...  reset years for seed introduction at each repeat
	int jahremitsameneintragpuffer=parameter[0].jahremitsameneintrag;

	// ... count simulation repeats
	parameter[0].wiederholung=0;

	// ... buffer for simulation time duration because it gets manipulated throughout a simulation run
	int simdauerini=parameter[0].simdauer;
	
	// start simulations for every simulation repeat
	for (int nruns=0; nruns<parameter[0].runs; nruns++)
	{
		parameter[0].starter=false;

		// multiple scenario header
		parameter[0].wiederholung++;
		parameter[0].simdauer=simdauerini;

		printf("\n\tSimulation progress: %d of %d\n", parameter[0].wiederholung, parameter[0].runs); 
		
		// initialise variables for each simulation instance
		parameter[0].nameakt=0;
		parameter[0].linieakt=0;	
		parameter[0].ivort=0;		
		parameter[0].jahremitsameneintrag=jahremitsameneintragpuffer;	
		parameter[0].individuenzahl=1;

		// start the simulation
		runSimulation();
		
		// finish the simulation
		finishSimulation(jahrPosition);
	}

	return 0;
}
