// Kompilieren mit Hilfer einer Makefile mittels dem Befehl: make -f Makefile ...
// ... oder manuell mit: g++ main.cpp -o wachstum -c -Wall



// used headers
//#include "libraries.h"
//#include "structures.h"
//#include "declarations.h"

// Programmmodule




using namespace std;

int yearposition; //deletion



/****************************************************************************************//**
 * \brief go through all functions for vegetation dynamics
 *
 * This function calls all other functions necessary to determine growth, 
 * mortality, ageing etc
 *
 *
 *******************************************************************************************/
void vegetationDynamics(int yearposition, int jahr, int t)
{
	// densityKARTE updaten
	clock_t start_time_kartenup = clock();
			/*!::Kartenupdate(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list)*/
			Kartenupdate(treerows, treecols, &parameter[0], yearposition, world_plot_list, world_tree_list, world_weather_list);
	clock_t end_time_kartenup = clock();

	
			// growth
	clock_t start_time_wachstum = clock();
			/*!::Wachstum(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)*/
			Wachstum(treerows, treecols, &parameter[0], yearposition, world_tree_list, world_weather_list, world_plot_list);
	clock_t end_time_wachstum = clock();

			
			// seed dispersal
	clock_t start_time_seedausbreitung = clock();
			/*!::seedausbreitung(int treerows, int treecols, struct Parameter *parameter, vector<list<seed*> > &world_seed_list)*/
			seedausbreitung(treerows, treecols, jahr, yearposition, &parameter[0], world_seed_list);
	clock_t end_time_seedausbreitung = clock();
	
	
	
			// seed production
	clock_t start_time_seedproduktion = clock();
			/*!::seedproduktion(int treerows, int treecols, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list)*/
			seedproduktion(treerows, treecols, &parameter[0], world_tree_list, world_seed_list, world_weather_list,yearposition);
	clock_t end_time_seedproduktion = clock();
	
	
			// Tree distribution	
	clock_t start_time_Treeverteilung = clock();	
			if (parameter[0].defTreevert==0 && parameter[0].seedintro==true && parameter[0].jahremitseedeintrag>0)
			{// seedeinbringen Beginn
				
				parameter[0].starter=true;
				
				/*!::Treeverteilung(int treerows, int treecols, struct Parameter *parameter, int wortlaengemax, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list)*/
				Treeverteilung(treerows, treecols, &parameter[0], wortlaengemax, yearposition, world_tree_list, world_seed_list, vegetationtype);
				
				parameter[0].jahremitseedeintrag--;
			}
			else if (parameter[0].defTreevert==0 && parameter[0].seedintropermanent==true && parameter[0].jahremitseedeintrag<=0) 
			{
				parameter[0].starter=true;
				Treeverteilung(treerows, treecols, &parameter[0],  wortlaengemax, yearposition, world_tree_list, world_seed_list, vegetationtype);
			}else if(parameter[0].defTreevert==1 && parameter[0].seedintro==true){
				parameter[0].starter=true;//seed statt trees
				Treeverteilung(treerows, treecols, &parameter[0],  wortlaengemax, yearposition, world_tree_list, world_seed_list, vegetationtype);
			}
	clock_t end_time_Treeverteilung = clock();
	
	
			// establishment
	clock_t start_time_etablierung = clock();
			/*!::Etablierung(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)*/			
			Etablierung(treerows, treecols, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);
			//printf("\n jahr=%d\n ", jahr+3998);
	clock_t end_time_etablierung = clock();
	
	
			// fire
	clock_t start_time_feuer = clock();
			/*!::Fire(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<vector<weather*> > &world_weather_list)*/
			Fire(treerows, treecols, &parameter[0], yearposition, world_plot_list, world_weather_list);
	clock_t end_time_feuer = clock();
		
			// Data_output
	clock_t start_time_Data_output = clock();
			/*!::Data_output(int treerows, int treecols, int t, int jahr, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list, vector<vector<Evaluation*> > &world_evaluation_list)*/
 			Data_output(treerows, treecols, t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);
	clock_t end_time_Data_output = clock();
						
			// MORTALITÄT,
	clock_t start_time_mortalitaet = clock();
			/*!::Mortalitaet(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)*/
			Mortalitaet(treerows, treecols, &parameter[0],jahr, yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);
	clock_t end_time_mortalitaet = clock();
	
			
			// Ageing
	clock_t start_time_Ageing = clock();
			/*!::Ageing(int treerows, int treecols, struct Parameter *parameter, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list)*/
			Ageing(treerows, treecols, &parameter[0], world_tree_list, world_seed_list);
	clock_t end_time_Ageing = clock();

	// print the computation time to the console and into a file
	if(parameter[0].computationtime==1){
		
		cout << endl << "plot update time: " << (((double) (end_time_kartenup - start_time_kartenup))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "growth time: " << (((double) (end_time_wachstum - start_time_wachstum))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "seed dispersal time: " << (((double) (end_time_seedausbreitung - start_time_seedausbreitung))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "seed production time: " << (((double) (end_time_seedproduktion - start_time_seedproduktion))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "tree distribution time: " << (((double) (end_time_Treeverteilung - start_time_Treeverteilung))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "establishment time: " << (((double) (end_time_etablierung - start_time_etablierung))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "fire time: " << (((double) (end_time_feuer - start_time_feuer))/ CLOCKS_PER_SEC) << endl;	
		cout << endl << "data output time: " << (((double) (end_time_Data_output - start_time_Data_output))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "mortality time: " << (((double) (end_time_mortalitaet - start_time_mortalitaet))/ CLOCKS_PER_SEC) << endl;
		cout << endl << "ageing time: " << (((double) (end_time_Ageing - start_time_Ageing))/ CLOCKS_PER_SEC) << endl;
		
		//vector<vector<Karten*> >::iterator posw = world_plot_list.begin();
		//vector<Karten*>& plot_list = *posw;
		
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin());
		list<Tree*>& tree_list = *world_positon_b;

		open:
		FILE *fp2;
		fp2 =fopen("t_N.txt","a+");
		if(fp2==0){goto open;}
		fprintf(fp2,"%lu \t %f \n",tree_list.size(), 
		((double)((end_time_Ageing - start_time_Ageing)+
		(end_time_mortalitaet - start_time_mortalitaet)+
		(end_time_Data_output - start_time_Data_output)+
		(end_time_feuer - start_time_feuer)+
		(end_time_etablierung - start_time_etablierung)+
		(end_time_Treeverteilung - start_time_Treeverteilung)+
		(end_time_seedproduktion - start_time_seedproduktion)+
		(end_time_seedausbreitung - start_time_seedausbreitung)+
		(end_time_wachstum - start_time_wachstum)+
		(end_time_kartenup - start_time_kartenup)
		)/ CLOCKS_PER_SEC));
		fclose(fp2);

		//open:
		FILE *fp3;
		fp3 =fopen("t_N_mort.txt","a+");
		if(fp3==0){goto open;}
		fprintf(fp2,"%lu;%d;%f\n",tree_list.size(),parameter[0].ivort, 
		((double)(end_time_mortalitaet - start_time_mortalitaet))
		/ CLOCKS_PER_SEC);
		fclose(fp3);
	}
	
 }








/****************************************************************************************//**
 * \brief Einschwingphase - get stable state before starting the real simulation
 *
 * initialise "Einschwingung" to reach a stable state before starting the normal routine
 * and call vegetationDynamics
 *
 *
 *******************************************************************************************/
void Einschwingphase()
{
		
		int t = -1;					//dummy for Data_output()

		/// Einschwingphase
		if (parameter[0].ivortmax>0 && parameter[0].stabilperiod==false)
		{ 
			parameter[0].einschwingen=true;
	
			printf("\nEinschwingphase:");
			do
			{

				parameter[0].ivort++;
				
				int firstyear =0;
				int lastyear=0;
				int startlag=5; // How many years should be left out in the beginning of the series
								// ... vielleicht eher gleich dem Wert machen, der die Länge der berechneten langjährigen Mittelwerte der weatherdaten bestimmt!
								// ... vielleicht diese Daten auch über Parameterdatei einlesen bzw. eine eigene Parameterdatei für die weatherdaten erstellen
				// Achtung: wenn Zeitspanne geaendert wird, auch Simdauer in Parameter ansehen, ob diese nicht analog geaendert werden muss! Sonst segfault!
				// Ebenso muss beachtet werden, ob das Startjahr in main noch richtig berechnet wird.
				
				if (parameter[0].weatherchoice==10 || parameter[0].weatherchoice==11 || parameter[0].weatherchoice==12 || parameter[0].weatherchoice==13 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999)
				{
					firstyear=1901;		//
					lastyear=2011;		//Bereich bis 2002
				}
				else if (parameter[0].weatherchoice==1 || parameter[0].weatherchoice==9)
				{
					firstyear=-3998;	// -3998 ist das "erste" Jahr 
					lastyear=5688;		// Bereich bis 1700
				}
				else if (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==309 || parameter[0].weatherchoice==1111 || parameter[0].weatherchoice==120 || parameter[0].weatherchoice==121 || parameter[0].weatherchoice==122 || parameter[0].weatherchoice==123)
				{
					firstyear=1921;		// minimal 934 maximal 2011  => 10 Jahre Puffer wegen 'running mean' calculation
					lastyear=2021;		
				}
				else if (parameter[0].weatherchoice==2221)
				{
					firstyear=2011;		// beginnt bei 1911 bis 2071
					lastyear=2071;
				}
				else if (parameter[0].weatherchoice==21 || parameter[0].weatherchoice==22 || parameter[0].weatherchoice==23 || parameter[0].weatherchoice==24)
				{
					firstyear=1934;
					lastyear=2013; // simdauer auf 80
				}
				
				
				// Ziehen eines zufaelligen Jahres fuer das weather 
				double x = rand()/(RAND_MAX + 1.0);
				int jahr= (firstyear+startlag) + (int) ( (double) ((lastyear-startlag)-firstyear)*x);		
			
			
				int yearposition = (world_weather_list[0][0]->jahr - jahr) * -1; // calculate actual year position in list, according to first year in the Weather-List and the random year
				//cout<<world_weather_list[0][0]->jahr<<endl;
	
				// Fortschrittsanzeige
				//printf("\n%d/%d\t=>\tzJahr=%d\t", parameter[0].ivort, parameter[0].ivortmax, jahr);	

				
				///go through all functions for vegetation dynamics
				///vegetationDynamics(int yearposition, int jahr, int t);
				vegetationDynamics(yearposition, jahr,t);
	
			} while (parameter[0].ivort<parameter[0].ivortmax);

		}
		else if (parameter[0].ivortmax>0 && parameter[0].stabilperiod==true)
		{
			printf("\nStabilization period:\n");
			// So lange zufaellige Jahre aus den eingelesenen weatherjahren ziehen bis ...
			// ... entweder	ein gewisser Prozentwert an Abweichung von festgelegten Waehrungen von Calibrationsites unterschritten wird
			// ... oder falls dies nicht moeglich ist nach 1000 Jahren 
			double stabilerrorthreshold=0.1;// TODO in Parameterfile einlesen
			bool stabilized=false;
			do
			{
				parameter[0].ivort++;
				
				int firstyear=0, lastyear=0;
				int startlag=5; // How many years should left out in the beginning of the series
				if (parameter[0].weatherchoice==10 || parameter[0].weatherchoice==11 || parameter[0].weatherchoice==12 || parameter[0].weatherchoice==13 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999)
				{
					firstyear=1901;		//
					lastyear=2011;		// Bereich bis 2002
				}
				else if (parameter[0].weatherchoice==1 || parameter[0].weatherchoice==9)
				{
					firstyear=-3998;	// -3998 ist das "erste" Jahr
					lastyear=5688;		// Bereich bis 1700
				}
				else if (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==309 || parameter[0].weatherchoice==1111)
				{
					firstyear=944;		// minimal 934 maximal 2011  => 10 Jahre Puffer wegen 'running mean' calculation
					lastyear=1044;
				}
				else if (parameter[0].weatherchoice==2221)
				{
					firstyear=2011;		// s.o.
					lastyear=2071;
				}else if ( parameter[0].weatherchoice==22 )
				{
					firstyear=1934;
					lastyear=2013; // simdauer auf 80
				}
				
				
				// take a random year for weather 
				double x = rand()/(RAND_MAX + 1.0);
				int jahr= (firstyear+startlag) + (int) ( (double) ((lastyear-startlag)-firstyear)*x);		
			
				int yearposition = (world_weather_list[0][0]->jahr - jahr) * -1; // calculate actual year position in list, according to first year in the Weather-List and the random year
				cout<<world_weather_list[0][0]->jahr<<endl;
	
				// Fortschrittsanzeige
				printf("==> N:%d/%d\t=>\tzJahr=%d\tjahrPos=%d\n", parameter[0].ivort, parameter[0].ivortmax, jahr, yearposition);	

				
				///go through all functions for vegetation dynamics
				///vegetationDynamics(int yearposition, int jahr, int t);
				vegetationDynamics(yearposition, jahr,t);
				
				
				
				// Berechnung der Abweichung von Referenzwerten
				double meanpercentchange=0;
				double stabilerror=1;
				int stabilizationtype=1; // 1==quasiGG; 2==ErrorReferenz TODO-in Parameterdatei
				if (stabilizationtype==1)
				{// Ermittlung ob ein quasiGG erreicht wurde
					
					int aktort=0;
					for (vector<vector<Evaluation*> >::iterator posausw = world_evaluation_list.begin(); posausw != world_evaluation_list.end(); ++posausw)
					{ // Weltschleife Beginn
						vector<Evaluation*>& evaluation_list = *posausw;
						vector<Evaluation*>::iterator posauswakt = evaluation_list.begin();
						pEvaluation=(*posauswakt);
		
						aktort++;
										
						// Zum Test mit der Basalarea
						if (pEvaluation->BArunmeanliste.size()>1)
						{
							cout << "BA" << endl << pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1] << endl;

							meanpercentchange+=fabs((pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-2]-pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1])
								/
								(pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1]));
						}
						cout << "Kumulierte %Change = " << meanpercentchange << endl;
						// + N_0-40
						if (pEvaluation->nheight0b40runmeanliste.size()>1)
						{
							cout << "N_0-40" << endl << pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-1] << endl;
							
							meanpercentchange+=fabs(
								(pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-2]-pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-1])
								/
								(pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-1]));
						}
						cout << "Kumulierte %Change = " << meanpercentchange << endl;
						// + N_40-200
						if (pEvaluation->nheight41b200runmeanliste.size()>1)
						{
							cout << "N_40-200" << endl << pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1] << endl;
							
							//if (pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1]>0)
							//{
							meanpercentchange+=fabs(
								(pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-2]-pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1])
								/
								(pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1]));
							//}
							//else
							//	printf("WARNING: Basalarea runmean ==0 \n");
						}
						cout << "Kumulierte %Change = " << meanpercentchange << endl;
						//StefanC: + N_200+
						if (pEvaluation->nheight201b10000runmeanliste.size()>1)
						{
							cout << "N_200-10000" << endl << pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1] << endl;
							
							//if (pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1]>0)
							//{
							meanpercentchange+=fabs(
								(pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-2]-pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1])
								/
								(pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1]));
							//}
							//else
							//	printf("WARNING: Basalarea runmean ==0 \n");
						}						
						cout << "Kumulierte %Change = " << meanpercentchange << endl;
						
					}	 // Weltschleife Ende
					cout << "\t==> STAB%CHANGE = " << meanpercentchange << "\t" << "Stabilization runs = " << parameter[0].ivort << endl << endl;
				
				}
				else if (stabilizationtype==2)
				{// Ermittlung der Abweichung von Referenzwerten
					/*
					int aktort=0;
					for (vector<vector<Evaluation*> >::iterator posausw = world_evaluation_list.begin(); posausw != world_evaluation_list.end(); ++posausw)
					{ // Weltschleife Beginn
						vector<Evaluation*>& evaluation_list = *posausw;
						vector<Evaluation*>::iterator posauswakt = evaluation_list.begin();
						pEvaluation=(*posauswakt);
		
						aktort++;
						
						//cout << "\nYCOO=" << pEvaluation->yworldcoo << "\tN>200=" << pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1] << endl;
						
						//StefanC: Zum Test mit der Basalarea
						if (pEvaluation->BArunmeanliste.size()>1)
						{
							cout << endl << pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1] << endl;
							
							if (pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1]>0)
							{
							meanpercentchange+=fabs(
								(pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-2]-pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1])
								/
								(pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1])
								);
							}
							else
								printf("WARNING: Basalarea runmean ==0 \n");
						}
						cout << "Kumulierte %Change = " << meanpercentchange << endl;
						
					}	 // Weltschleife Ende
					cout << "\t==> STAB%CHANGE = " << meanpercentchange << "\t" << "Stabilization runs = " << parameter[0].ivort << endl << endl;
				
					cout << "\t==> STABERROR = " << stabilerror << "\t" << "Stabilization runs = " << parameter[0].ivort << endl << endl;
					*/
				}
				
				// Bedingung festlegen wann die Jahresschritte an Zufallsyearposition == Stabilisierungsphase beendet werden sollen
				if ((parameter[0].ivort>parameter[0].stabilmovingwindow && meanpercentchange<parameter[0].stabilpercentchangethreshold) || stabilerror<=stabilerrorthreshold || parameter[0].ivort>parameter[0].ivortmax)
				{	
					stabilized=true;
				}
				
			} while (stabilized!=true);
			
		}
		
		parameter[0].einschwingen=false;

}





/****************************************************************************************//**
 * \brief Jahresschritte - run through the simulation for all years (simdauer)
 *
 * 
 * initialise yearly steps and call vegetationDynamics
 *
 *
 *******************************************************************************************/
void Jahresschritte()
{

		printf("\n\nJahresschritte beginnen ...\n");
		
		for (int t=0;t<parameter[0].simdauer;t++)
		{ // Jahresschrittschleife Beginn
			
			parameter[0].ivort++;


			parameter[0].feuerausgabe=0;	// Variable um ein eventuelles Feuer in dem Jahr zu registrieren und auszugeben


			// Aktuelles Jahr berechnen und falls gewuenscht eine Uebersicht ueber das Jahr ausgeben
			int jahr=parameter[0].startjahr+t;
			
			//influx(treerows,treecols,&parameter[0] , jahr , world_tree_list , vegetationtype);

			
			yearposition = ((world_weather_list[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year 


			if (parameter[0].jahranzeige ==true) 
				printf("\nSites pro Ort\tJahr\tZeitschritt\tSimulationsdauer\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
			
			/*else 
			{
				printf("t=%d", jahr);

				if ((jahr%100)==0) 
					printf("\n"); 
			}
			*/
			
			///vegetationDynamics(int yearposition, int jahr, int t);
			///go through all functions for vegetation dynamics
			vegetationDynamics(yearposition, jahr,t);

			
			/// Wenn das Jahr, auf das später die ganze Simulation zurückgesetzt werden soll, erreicht ist, dann speichere alle Daten!
			if (jahr==parameter[0].resetyear)
			{
				SaveAllLists();
				cout << "In Zeitschritt = " << jahr << " alles gespeichert!" << endl << endl;
			}

			
		} // Jahresschrittschleife Ende

		if (parameter[0].resetyear>0)
		{

			bool SCHalterweilVergleichGUT=true;
			
			if(SCHalterweilVergleichGUT==true)
			{

				// Parametervariation je nach Expsetting
				// ... erst Speicherung des mit der Parameterdatei eingelesenen Wertes
				double incfac_buffer=parameter[0].incfac;
				double densitywertmanipulatorexp_buffer=parameter[0].densitywertmanipulatorexp;
				int etabbgpermanent_buffer=parameter[0].etabbgpermanent;

				// parameter[0].tempdiffort=0.0;
				double tempdiffort_buffer=parameter[0].tempdiffort;
				// parameter[0].precdiffort=0.0;
				// für verschiedene Temperaturvarianten wiederholen am 18.11.2014 eingefügt
				for(double tempdifforti=-0.5; tempdifforti<0.9; tempdifforti=tempdifforti+0.5)
				{
					parameter[0].tempdiffort=tempdifforti;
					// mit neuen tempdiffort muss das weather eingelesen werden
					// ... dafür muss aber vorher die world_weather_list leer gemacht werden.
					//aus finishSimulations()
					for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
					{	// Weltschleife Beginn
							vector<weather*>& weather_list = *posw;

							// weather_listn loeschen
							for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
							{// weather_list Beginn
								pweather=weather_list[iweather];
								delete pweather;
							}// weather_list Ende
							weather_list.clear();
					} // Weltschleife Ende

					//aus runSimulations()
					weathereinlesen(treerows, &parameter[0],  wortlaengemax, world_weather_list);

				
					// für verschiedene Parameter-Settings wiederholen am 18.11.2014 eingefügt
					for(int parameteri=0;parameteri<4;parameteri++)
					{
						// Parametervariation je nach Expsetting
						// ... und Veränderung auf den gewünschsten Wert,
						if(parameteri==0)
						{}
						else if(parameteri==1)
						{
							parameter[0].incfac=5; //Konk1
						}
						else if(parameteri==2)
						{
							parameter[0].densitywertmanipulatorexp=2; //Konk2
						}
						else if(parameteri==3)
						{
							parameter[0].etabbgpermanent=1000; //Seedinput
						}
						
						
						
						
						
						
						cout << " START der Simulationslaeufe " << endl;
						// Reset der Listen und Simulationslauf
							// Ausgabe kennzeichnen
								ClearAllLists();
								cout << "           Listen geloescht!!" << endl;
								RestoreAllLists();
								cout << "           Listen wiederhergestellt!!" << endl;
								
						printf("\n\nJahresschritte des Simulationslaufes beginnen ...\n");
						cout << "     Laenge der Simulation=" << ((parameter[0].simdauer-(2011-parameter[0].resetyear))+1) << endl;
						

						
						for (int t=((parameter[0].simdauer-(2011-parameter[0].resetyear))+1);t<parameter[0].simdauer;t++)
						{ // Jahresschrittschleife Beginn
							
							parameter[0].ivort++;


							parameter[0].feuerausgabe=0;	// Variable um ein eventuelles Feuer in dem Jahr zu registrieren und auszugeben

							


							// Aktuelles Jahr berechnen und falls gewuenscht eine Uebersicht ueber das Jahr ausgeben
							int jahr=parameter[0].startjahr+t;
							
							yearposition = ((world_weather_list[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year

							if (parameter[0].jahranzeige ==true) 
								printf("\nSites pro Ort\tJahr\tZeitschritt\tSimulationsdauer\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
							
							else 
							{
								printf("t=%d", jahr);

								if ((jahr%100)==0) 
									printf("\n"); 
							}

							
							///vegetationDynamics(int yearposition, int jahr, int t);
							///go through all functions for vegetation dynamics
							vegetationDynamics(yearposition, jahr,t);

							
							/// Wenn das Jahr auf das später die ganze Simulation zurückgesetzt werden soll erreicht ist, dann speichere alle Daten!
							if (jahr==parameter[0].resetyear)
							{
								SaveAllLists();
								cout << "In Zeitschritt = " << jahr << " alles gespeichert!" << endl << endl;
							}

							
						} // Jahresschrittschleife Ende
					
						// Parametervariation je nach Expsetting
						// ... dann nach wiederholter Simulation zurücksetzen auf Standardwert
							parameter[0].incfac=incfac_buffer;
							parameter[0].densitywertmanipulatorexp=densitywertmanipulatorexp_buffer;
							parameter[0].etabbgpermanent=etabbgpermanent_buffer;
							
					}//parametersettings ENDE
				
					parameter[0].tempdiffort=tempdiffort_buffer;
				}//weathersettings ENDE
			}

		}
		
}




/****************************************************************************************//**
 * \brief create all lists needed for the model
 *
 * 
 * (Welt)tree_list \n
 * (Welt)seed_list \n
 * (Welt)weather_list \n
 * (Welt)plot_list \n
 * (Welt)evaluation_list \n
 *******************************************************************************************/
void createLists()
{	
		for (int i=0;i< parameter[0].mapylength;i++)	
		{// WeltListen Beginn
		
			for (int j=0;j< parameter[0].mapxlength;j++)		
			{
				list<Tree*> tree_list;					// Neue tree_list erstellen 
				world_tree_list.push_back(tree_list);		// Neue tree_list in die WeltListe eintragen 

				list<seed*> seed_list;				// Neue seed_list erstellen
				world_seed_list.push_back(seed_list);	// Neue seed_list in die WeltListe eintragen

				vector<weather*> weather_list;			// Neue weather_list erstellen
				world_weather_list.push_back(weather_list);	// Neue weather_list in die WeltListe eintragen

				vector<Karten*> plot_list;			// Neue plot_list erstellen
				world_plot_list.push_back(plot_list);	// Neue plot_list in die WeltListe eintragen

				vector<Evaluation*> evaluation_list;			  //Neue evaluation_list erzeugen
				world_evaluation_list.push_back(evaluation_list); // Neue evaluation_list in die Weltliste eintragen
				
				
				if (parameter[0].resetyear>0)
				{
					// Listen für das Resetten zu einem bestimmten Jahr anlegen
					list<Tree*> tree_list;					// Neue tree_list erstellen 
					world_tree_list_copy.push_back(tree_list);		// Neue tree_list in die WeltListe eintragen 

					list<seed*> seed_list;				// Neue seed_list erstellen
					world_seed_list_copy.push_back(seed_list);	// Neue seed_list in die WeltListe eintragen

					vector<Karten*> plot_list;			// Neue plot_list erstellen
					world_plot_list_copy.push_back(plot_list);	// Neue plot_list in die WeltListe eintragen

					vector<Evaluation*> evaluation_list;			  //Neue evaluation_list erzeugen
					world_evaluation_list_copy.push_back(evaluation_list); // Neue evaluation_list in die Weltliste eintragen
				}
				//INFLUX RATES
			}
		} // WeltListen Ende

}





/****************************************************************************************//**
 * \brief initialise Karten Element and Evaluation
 *
 *
 *
 *
 *
 *******************************************************************************************/
void initialiseMaps()//NICHT HIER EINLESEN?
{
		int aktort=0;
		for (vector<vector<Karten*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++)
		{ /// Weltschleife Beginn
			vector<Karten*>& plot_list = *posw;
		
			vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list.begin()+aktort);
			vector<Evaluation*>& evaluation_list = *posiwelt;
	
			aktort++;
				
			// Berechnung des aktuellen Ortes in Koordinaten 
			//			xwelt=Wiederholung gleicher Ort
			//			ywelt=anderer Ort entlang eines Transektes
			//nötig für Weltlisten
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength ); // floor = nächstniedriger Wert (2.3 => 2; -2.3 => -3)
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
		

			// Neue Kartenelemente hinzufuegen
			for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++) 
			{ 
				pKarten= new Karten();	// Neues Kartenelement erzeugen

				pKarten->yworldcoo=aktortyworldcoo;
				pKarten->xworldcoo=aktortxworldcoo;

				pKarten->ycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
				pKarten->xcoo=(double) kartenpos - (pKarten->ycoo * (treecols*parameter[0].sizemagnif));

				pKarten->Treedensitywert=0;
				pKarten->Treeanzahl=0;
				pKarten->maxthawing_depth=1000;
				
				pKarten->auflagenstaerke=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke0=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke1=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke2=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke3=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke4=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke5=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke6=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke7=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke8=200;	// in 0.1mm demnach max 6.5535 m
				pKarten->auflagenstaerke9=200;	// in 0.1mm demnach max 6.5535 m 
				pKarten->auflagenstaerkemittel=200;
				

				plot_list.push_back(pKarten);	
				//Kartenelement in Liste einfuegen
			} 


			
			
			
			
			// Fuer die Evaluation fuer jeden Ort ein Evaluation-Element erschaffen
			pEvaluation=new Evaluation();
			pEvaluation->yworldcoo=aktortyworldcoo;
			pEvaluation->xworldcoo=aktortxworldcoo;
			pEvaluation->BAliste.clear();
			pEvaluation->BArunmeanliste.clear();
			//Start Wachstumsphasenermittlung
			pEvaluation->maxsteigungbasalarea=0.0;
			pEvaluation->countermaxsteigungbasalarea=0;
			pEvaluation->wendejahr=-9999;
			pEvaluation->saettigungsjahr=-9999;
			pEvaluation->nachwendejahr=false;
			//Ende Wachstumsphasenermittlung
			//Start Allgemeine Werte
			pEvaluation->nheight0b40liste.clear();
			pEvaluation->nheight0b40runmeanliste.clear();
			pEvaluation->nheight41b200liste.clear();
			pEvaluation->nheight41b200runmeanliste.clear();
			pEvaluation->nheight201b10000liste.clear();
			pEvaluation->nheight201b10000runmeanliste.clear();
			pEvaluation->meanbreastdiameterliste.clear();
			pEvaluation->meanbreastdiameterrunmeanliste.clear();
			pEvaluation->stemcountliste.clear();
			pEvaluation->stemcountrunmeanliste.clear();
			pEvaluation->meantreeheightliste.clear();
			pEvaluation->meantreeheightrunmeanliste.clear();
			pEvaluation->meantreeageliste.clear();
			pEvaluation->meantreeagerunmeanliste.clear();
			//Ende Allgemeine Werte
			evaluation_list.push_back(pEvaluation);
			
			cout << "evaluation_list.size()=" << evaluation_list.size() << endl;
			
		} /// Weltschleife Ende
	cout<<"Weltschleife Ende\n";
}





/****************************************************************************************//**
 * \brief start the simulation
 *
 * initialise some parameters \n
 * create lists \n
 * read weather \n
 * initilise Maps \n
 * distribute trees \n
 * start Enschwingphase \n
 * start yearly timesteps \n
 *******************************************************************************************/
void runSimulation()
{
		

		// WeltListen erzeugen 
		// Listen an jedem Koordinatenpunkt der Welt hinzufuegen
		///createLists()
		createLists();
		

		/// weather einlesen
		/*! ::weathereinlesen(int treerows, struct Parameter *parameter,  int wortlaengemax, vector<vector<weather*> > &world_weather_list)*/
		weathereinlesen(treerows, &parameter[0],  wortlaengemax, world_weather_list);
		
		
		/// Karten und Evaluation Listen für jede Welt vorbereiten 
		///initialiseMaps();
		initialiseMaps();
		//Ende multiple Szenarien Header



		/// trees (Tree) wie auf CH17I einbringen oder seed verteilen
		/*! ::Treeverteilung(int treerows, int treecols, struct Parameter *parameter, int wortlaengemax, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list)*/
		Treeverteilung(treerows, treecols, &parameter[0], wortlaengemax, parameter[0].startjahr+3999, world_tree_list, world_seed_list, vegetationtype);

		parameter[0].ivort=0;		// Variable fuer den aktuellen Fortschritt über den gesamten Zeitraum, einschließlich Stabilisierungsphase

// 		cout << parameter[0].ivort << endl;
// 		exit(1);
		//Einschwingphase - get stable state before starting the real simulation
		///Einschwingphase()
		Einschwingphase();
		
		
		///Jahresschritte()
		Jahresschritte();


}






/****************************************************************************************//**
 * \brief finish the simulation
 *
 * do something I don't understand \n
 * clear all lists
 *
 *
 *******************************************************************************************/
void finishSimulation(int yearposition)
{
		printf("\n\nEndausgabe\n");
		int aktort=0;

		for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
		{	// Weltschleife Beginn
			// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Refernenz
			// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
			// realisiert ist weiterverfahren werden
			// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
			vector<weather*>& weather_list = *posw;

			// Um auf ein bestimmtes Element in der Welt zuzugreifen muss ein Iterator bis
			// zum entsprechenden Element justiert werden und dann eine tree_list als Referenz
			// erzeugt werden
			vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
			list<Tree*>& tree_list = *world_positon_b;

			vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
			list<seed*>& seed_list = *world_positon_s;

			//vector<vector<Karten*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
			//vector<Karten*>& plot_list = *posiweltk;

			//vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list.begin()+aktort);
			//vector<Evaluation*>& evaluation_list = *posiwelt;

			aktort++;
			// Berechnung des aktuellen Ortes in Koordinaten
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
			
			
			
			printf("Ort(Y,X)=(%d,%d) - JahreTmean(%d)=%4.2f - Treeanzahl=%zu - seedanzahlG=%zu - ", aktortyworldcoo, aktortxworldcoo, weather_list[yearposition]->jahr, weather_list[yearposition]->tempjahrmittel, tree_list.size(), seed_list.size());
			
			int seedimconezahl=0;
			for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); pos++) 
			{
					if ((*pos)->imcone==true) 
						seedimconezahl++;
			};
			printf("seedanzahlZ= %d\n", seedimconezahl);

			// weather_listn loeschen
			for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
			{// weather_list Beginn
				pweather=weather_list[iweather];
				delete pweather;
			}// weather_list Ende
			weather_list.clear();

		} // Weltschleife Ende
		
		// Aufruf der Funktion zum Listenloeschen
		ClearAllLists();	// Neue Funktion für Tree, seed, Karten und evaluation_listn hinzugefügt am 10.11.2014 leert alle Listen in WeltListen

		// Nach jeder Wiederholung werden die WeltListen gelöscht
		world_tree_list.clear();
		world_seed_list.clear();
		world_weather_list.clear();
		world_plot_list.clear();
		world_evaluation_list.clear();
		if (parameter[0].resetyear>0)
		{
			ClearAllLists_copy();
			
			world_tree_list_copy.clear();
			world_seed_list_copy.clear();
			world_plot_list_copy.clear();
			world_evaluation_list_copy.clear();
		}
		
}







///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////						  /////////////////////////////////////////////////////////
/////////////		   MAIN		      /////////////////////////////////////////////////////////
/////////////			 			  /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



/****************************************************************************************//**
 * \brief main routine to start model 
 *
 * Read Parameters
 * Start Simulation
 * Finish Simulation
 *
 *******************************************************************************************/
int main()
{
	
	system("mkdir output");
	clock_t start_time_main = clock(); //StefanC: Zeitaufnahme fuer Gesamtlauf
						
			//---------Just some Initialisation stuff---------
			

			// Zufallszahlen "mischen"
			srand((unsigned) time(NULL));

			// Anzeigen der Ueberschrift und Programmversion
			printf("\n---->\tLAVESI\n");
			printf("\n You have started  LAVESI, " 
					"An individually-based and spatial explicit simulation model"
					"for the vegetation dynamics of LARIX GMELINII (RUPR.) KUZEN."
					"- driven by weather data."
					"\n\n Version:\t 2.07 (Multiprocessing)"
					"\n Date:\t\t 10.04.2017" 
					"\n authors:"
					"\n\t Stefan Kruse\tstefan.kruse@awi.de"
					"\n\t Alexander Gerdes\talexander.gerdes@awi.de"
					"\n\t Nadja Kath\tnadja.kath@awi.de"
					"\n\t Mareike Wieczorek\tmareike.wieczorek@awi.de"
					"\n");
			printf("\n<----\n");


			
			//---------Just some Initialisation stuff---------
			


			/// Parametereinlesen()
			Parametereinlesen();
			
			/*if(parameter[0].defTreevert==1){
				VegeTypeIni();
			
			for(int j=0;j<treecols;j++){
			for(int i=0;i<treerows;i++){
			cout<<vegetationtype[j][i]<<" ";
			}
			cout<<endl;
			}
			
			}*/
			
			// Sicherheitsabfragen zu bestimmten Parameterkombinationen
			// ... weather- und Treestartchoice
			if (parameter[0].weatherchoice!=parameter[0].starttrees && parameter[0].starttrees!=0)
				printf("\n\n\tWARNING:\tWeather series=%d & Initial trees=%d", parameter[0].weatherchoice, parameter[0].starttrees);
			else if (parameter[0].seedintro==true && parameter[0].starttrees!=0)
				printf("\n\n\tWARNING:\tSimulation with initial trees and seed input");
			else if (parameter[0].seedintro==false && parameter[0].starttrees==0)
				printf("\n\n\tWARNING:\tSimulation without initial trees and without seed input");
			// ... Laufparameter
			if ( parameter[0].mapylength!=4 && (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999) )
				printf("\n\n\tWARNING:\tLatitudinal length >1 AND all 4 Sites will be simulated ==> mapylenght was set to 4");
			if ( (parameter[0].mapylength!=11 || (parameter[0].mapylength*parameter[0].mapylength)!=121) && (parameter[0].weatherchoice==1111) )
				printf("\n\n\tWARNING:\tLatitudinal length differs CHECK");
			if ( parameter[0].weathercalcgradient==true && (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999 || parameter[0].weatherchoice==1111 ) )
				printf("\n\n\tWARNING:\tWeather gradient (weathercalcgradient) will be calculated along latitudinal position of plots AND all 4 Sites will be simulated (weatherchoice==999 || 111)! This makes no sense; Adjust parameter settings!");
			
// 			signed int abbruch;
			// Abfrage ob das Programm beendet oder fortgesetzt werden soll*/
// 			printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n");
// 			scanf("%d", &abbruch);

// 			if (abbruch!=1) 
// 			{
// 				printf("LaVeSi wurde beendet\n\n"); 
// 				exit(0);
// 			}
			
			
			
			
			
			/// Parameter und Variablen auf Startwerte setzen
			/// Startjahr des Simulationslaufs berechnen (2011-Simdauer)
			
			if((parameter[0].weatherchoice== 21) or (parameter[0].weatherchoice== 22) or (parameter[0].weatherchoice== 23) or (parameter[0].weatherchoice== 24))
			{	
				parameter[0].startjahr=2014-parameter[0].simdauer; // (lastyear+1)-simdauer
			}
			else
			{
				parameter[0].startjahr=2011-parameter[0].simdauer;
			}
			
			// Fuer die Data_output
			parameter[0].ordnerda=false;	// Ordner fuer die Zuwachsleistungsausgabe im Heidelberg-Format ist zu Beginn nicht vorhanden, diese logische Variable wird beim Erstellen auf true gesetzt
			
			
			int jahremitseedeintragpuffer=parameter[0].jahremitseedeintrag;	// Zuruecksetzen der Jahre mit seedeintrag auf den Startwert bei jeder Wiederholung


			//Mitzaehlen und Ausgeben der Wiederholung
			parameter[0].wiederholung=0;

			
			// Mitzaehlen der Laeufe
			int zaehler=0;					

			// Simdauer speichern, da in Ausgabe der Wert modifiziert wird
			int simdauerini=parameter[0].simdauer;

			//StefanC: Hier kommt jetzt ein Bereich zur Initialisierung ueber alle vorhandenen Sites (10-13 bzw. 910-913)
			//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	
			if (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999)
			{
				//StefanC: Setze die Anzahl der Y-Tiles auf 4 um darueber die Simulation an Y-Orten zu ermoeglichen
				parameter[0].mapylength=4;
			}
			//else if (parameter[0].weatherchoice==1111)
			//{
				//StefanC: Setze die Anzahl der Y-Tiles auf 11 um darueber die Simulation an Y-Orten auf einem NS-Transekt zu ermoeglichen
				//parameter[0].mapylength=11;
			//}
			//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	

			//cout << zaehler<<endl;
			for (int nruns=0; nruns<parameter[0].runs; nruns++)
			{

				// Fast R supported SA_levels
				#include "SA_parametervariation.cpp"
			
				parameter[0].starter=false;

				//Beginn Multiple szenarien header
				parameter[0].wiederholung++;
				parameter[0].simdauer=simdauerini;

				zaehler++;
				printf("\n\tFortschritt: %d von %d\n", zaehler, parameter[0].runs); 

				// Variablen auf Startwerte setzen
				// Variable für die Treenamen im Modelllauf
				parameter[0].nameakt=0;
				// Variable für die linen im Modelllauf
				parameter[0].lineakt=0;	

				// Zuruecksetzen der Jahre mit seedeintrag auf den Startwert
				parameter[0].jahremitseedeintrag=jahremitseedeintragpuffer;	

				parameter[0].individuenzahl=1;
				///runSimulation()
				runSimulation();
					
				///finishSimulation()
				finishSimulation(yearposition);
			}
		
	clock_t end_time_main = clock();
	cout << endl << "Total time: " << (((double) (end_time_main - start_time_main))/ CLOCKS_PER_SEC) << endl; //StefanC: Zeit fuer Gesamtlauf
	
	//print areal dependency of total simulation time:
	if(parameter[0].computationtime==1){
		FILE *fp;
		fp=fopen("t_A.txt","a+");
		fprintf(fp,"%d \t %f \n",(treerows*treecols), (((double) (end_time_main - start_time_main))/ CLOCKS_PER_SEC));
	}

	return 0;
	
}
