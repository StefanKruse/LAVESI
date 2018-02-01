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
	// density grid update
	double start_time_kartenup = omp_get_wtime();
			Kartenupdate(&parameter[0], yearposition, world_plot_list, world_tree_list, world_weather_list);
	double end_time_kartenup = omp_get_wtime();
	
			// growth
	double start_time_wachstum = omp_get_wtime();
			Wachstum( &parameter[0], yearposition, world_tree_list, world_weather_list);
	double end_time_wachstum = omp_get_wtime();

			// seed dispersal
	double start_time_seedausbreitung = omp_get_wtime();
	
				int findyr1=0,findyr2=0,yr=0;
				if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5)
				{
					if(parameter[0].windsource==1)
					{
					findyr1=1979;findyr2=2012;
					}
				}
									
				if(jahr<findyr1 or jahr>findyr2)
				{
					yr=(findyr1+(int)(rand()/(RAND_MAX+1.0)*(findyr2-findyr1)));
				}
				else
				{
					yr=jahr;
				}
								
				for(int i=0;i<(signed)globalyears.size();i++)
				{
					if(globalyears[i]==yr)
					{
						for(int pos=0;pos<(signed)winddir.at(i).size();pos++)
						{
							wdir.push_back(winddir.at(i).at(pos));
							wspd.push_back(windspd.at(i).at(pos));
						} 
					}
				}
					
		
			seedausbreitung( yr, &parameter[0], world_seed_list);
			
	double end_time_seedausbreitung = omp_get_wtime();
	
			// seed production
	double start_time_seedproduktion = omp_get_wtime();
			seedproduktion( &parameter[0], world_tree_list);
	double end_time_seedproduktion = omp_get_wtime();
	
	
			// Tree distribution	
	double start_time_Treedistribution = omp_get_wtime();	
			if (parameter[0].seedintro==true && parameter[0].jahremitseedeintrag>0)
			{// begin seed introduction
				
				parameter[0].starter=true;
				
				Treedistribution(&parameter[0], stringlengthmax);
				
				parameter[0].jahremitseedeintrag--;
			}
			else if ( parameter[0].seedintropermanent==true && parameter[0].jahremitseedeintrag<=0) 
			{
				parameter[0].starter=true;
				Treedistribution(&parameter[0],  stringlengthmax);
			}
			
	double end_time_Treedistribution = omp_get_wtime();
	
	
			// establishment
	double start_time_etablierung = omp_get_wtime();
			Etablierung(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);
	double end_time_etablierung = omp_get_wtime();
	
	
			// fire
	double start_time_feuer = omp_get_wtime();
			Fire( &parameter[0], yearposition, world_plot_list, world_weather_list);
	double end_time_feuer = omp_get_wtime();
		
			// Dataoutput
	double start_time_Dataoutput = omp_get_wtime();
			Dataoutput(t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);
	double end_time_Dataoutput = omp_get_wtime();
						
			// MORTALITÄT,
	double start_time_mortalitaet = omp_get_wtime();
			Mortalitaet( &parameter[0],yr, yearposition, world_tree_list, world_seed_list, world_weather_list);
				wspd.clear();
				wdir.clear();
				wspd.shrink_to_fit();
				wdir.shrink_to_fit();
	double end_time_mortalitaet = omp_get_wtime();
	
			
			// Ageing
	double start_time_Ageing = omp_get_wtime();
			Ageing(&parameter[0], world_tree_list, world_seed_list);
	double end_time_Ageing = omp_get_wtime();

	// print the computation time to the console and into a file
	if(parameter[0].computationtimevis==1)
	{
		
		if(((parameter[0].ivort%50)==0) | (parameter[0].ivort==1))
			printf("\n   - plotupdategrowth    seeddisp  seedprod  treedistr treeestab fire      output    mortality ageing    TOTAL     ");
		printf("\n %d  - %10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f", 
				parameter[0].ivort,
				end_time_kartenup - start_time_kartenup ,
				end_time_wachstum - start_time_wachstum ,
				end_time_seedausbreitung - start_time_seedausbreitung,
				end_time_seedproduktion - start_time_seedproduktion,
				end_time_Treedistribution - start_time_Treedistribution,
				end_time_etablierung - start_time_etablierung,
				end_time_feuer - start_time_feuer,
				end_time_Dataoutput - start_time_Dataoutput ,
				end_time_mortalitaet - start_time_mortalitaet,
				end_time_Ageing - start_time_Ageing ,
				((end_time_Ageing - start_time_Ageing)+
					(end_time_mortalitaet - start_time_mortalitaet)+
					(end_time_Dataoutput - start_time_Dataoutput)+
					(end_time_feuer - start_time_feuer)+
					(end_time_etablierung - start_time_etablierung)+
					(end_time_Treedistribution - start_time_Treedistribution)+
					(end_time_seedproduktion - start_time_seedproduktion)+
					(end_time_seedausbreitung - start_time_seedausbreitung)+
					(end_time_wachstum - start_time_wachstum)+
					(end_time_kartenup - start_time_kartenup)
					)
				 );

				 
		
		
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin());
		list<Tree*>& tree_list = *world_positon_b;

		openmort:
		FILE *fp3;
		fp3 =fopen("t_N_mort.txt","a+");
		if(fp3==0)
			{goto openmort;}
			fprintf(fp3,"%lu;%d;%f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f\n",
			tree_list.size(),
			parameter[0].ivort, 
			end_time_mortalitaet - start_time_mortalitaet,
			end_time_kartenup - start_time_kartenup ,// from here newly introduced to record all
				end_time_wachstum - start_time_wachstum ,
				end_time_seedausbreitung - start_time_seedausbreitung,
				end_time_seedproduktion - start_time_seedproduktion,
				end_time_Treedistribution - start_time_Treedistribution,
				end_time_etablierung - start_time_etablierung,
				end_time_feuer - start_time_feuer,
				end_time_Dataoutput - start_time_Dataoutput ,
				end_time_mortalitaet - start_time_mortalitaet,
				end_time_Ageing - start_time_Ageing ,
				(end_time_Ageing - start_time_Ageing)+
					(end_time_mortalitaet - start_time_mortalitaet)+
					(end_time_Dataoutput - start_time_Dataoutput)+
					(end_time_feuer - start_time_feuer)+
					(end_time_etablierung - start_time_etablierung)+
					(end_time_Treedistribution - start_time_Treedistribution)+
					(end_time_seedproduktion - start_time_seedproduktion)+
					(end_time_seedausbreitung - start_time_seedausbreitung)+
					(end_time_wachstum - start_time_wachstum)+
					(end_time_kartenup - start_time_kartenup)
			);
		fclose(fp3);
	}//END: computation time output
	
 }








/****************************************************************************************//**
 * \brief Spinupphase - get stable state before starting the real simulation
 *
 * initialise "Einschwingung" to reach a stable state before starting the normal routine
 * and call vegetationDynamics
 *
 *
 *******************************************************************************************/
void Spinupphase()
{
		
		int t = -1;
		
		// spin-up-phase
		if (parameter[0].ivortmax>0 && parameter[0].stabilperiod==false)
		{ 
			parameter[0].einschwingen=true;
	
			printf("\nSpin up phase:");
			do
			{
				parameter[0].ivort++;

				int firstyear =0;
				int lastyear=0;
				int startlag=5;
				
				if (parameter[0].weatherchoice==10 || parameter[0].weatherchoice==11 || parameter[0].weatherchoice==12 || parameter[0].weatherchoice==13 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999)
				{
					firstyear=1901;
					lastyear=2011;
				}
				else if (parameter[0].weatherchoice==1 || parameter[0].weatherchoice==9)
				{
					firstyear=-3998;
					lastyear=5688;
				}
				else if (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==309 || parameter[0].weatherchoice==1111 || parameter[0].weatherchoice==120 || parameter[0].weatherchoice==121 || parameter[0].weatherchoice==122 || parameter[0].weatherchoice==123)
				{
					firstyear=1921;
					lastyear=2021;		
				}
				else if (parameter[0].weatherchoice==2221)
				{
					firstyear=2011;
					lastyear=2071;
				}
				else if (parameter[0].weatherchoice==21 || parameter[0].weatherchoice==22 || parameter[0].weatherchoice==23 || parameter[0].weatherchoice==24)
				{
					firstyear=1934;
					lastyear=2013;
				}
				
				// choose a random year for weather determination
				double x = rand()/(RAND_MAX + 1.0);
				int jahr= (firstyear+startlag) + (int) ( (double) ((lastyear-startlag)-firstyear)*x);		
			
				// calculate current year position in list, according to first year in the Weather-List and the random year
				int yearposition = (world_weather_list[0][0]->jahr - jahr) * -1; 
				
				// go through all functions for vegetation dynamics
				vegetationDynamics(yearposition, jahr,t);
	
			} while (parameter[0].ivort<parameter[0].ivortmax);

		}
		else if (parameter[0].ivortmax>0 && parameter[0].stabilperiod==true)
		{
			printf("\nStabilization period:\n");
			
			// Choose random years from the weather files until...
			// ... either a percentage of variance of certain observable is underestimated at certain sites
			// ... or, if this is not possible, after 1000 years
			double stabilerrorthreshold=0.1;
			bool stabilized=false;
			do
			{
				parameter[0].ivort++;
				
				int firstyear=0, lastyear=0;
				int startlag=5; // How many years should left out in the beginning of the series
				if (parameter[0].weatherchoice==10 || parameter[0].weatherchoice==11 || parameter[0].weatherchoice==12 || parameter[0].weatherchoice==13 || parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999)
				{
					firstyear=1901;
					lastyear=2011;
				}
				else if (parameter[0].weatherchoice==1 || parameter[0].weatherchoice==9)
				{
					firstyear=-3998;
					lastyear=5688;
				}
				else if (parameter[0].weatherchoice==111 || parameter[0].weatherchoice==309 || parameter[0].weatherchoice==1111)
				{
					firstyear=944;	
					lastyear=1044;
				}
				else if (parameter[0].weatherchoice==2221)
				{
					firstyear=2011;	
					lastyear=2071;
				}
				else if ( parameter[0].weatherchoice==22 )
				{
					firstyear=1934;
					lastyear=2013;
				}
				
				// take a random year for weather determination
				double x = rand()/(RAND_MAX + 1.0);
				int jahr= (firstyear+startlag) + (int) ( (double) ((lastyear-startlag)-firstyear)*x);		
			
				int yearposition = (world_weather_list[0][0]->jahr - jahr) * -1; // calculate actual year position in list, according to first year in the Weather-List and the random year
				cout<<world_weather_list[0][0]->jahr<<endl;
	
				// progress console output
				printf("==> N:%d/%d\t=>\tzYear=%d\tyearPos=%d\n", parameter[0].ivort, parameter[0].ivortmax, jahr, yearposition);	

				// go through all functions for vegetation dynamics
				vegetationDynamics(yearposition, jahr,t);
				
				// Calculation of the deviation from reference values
				double meanpercentchange=0;
				double stabilerror=1;
				int stabilizationtype=1;
				if (stabilizationtype==1)
				{// is an equilibrium reached?
					
					int aktort=0;
					for (vector<vector<Evaluation*> >::iterator posausw = world_evaluation_list.begin(); posausw != world_evaluation_list.end(); ++posausw)
					{ // world evaluation list begin
						vector<Evaluation*>& evaluation_list = *posausw;
						vector<Evaluation*>::iterator posauswakt = evaluation_list.begin();
						pEvaluation=(*posauswakt);
		
						aktort++;
							
						// observing the basal area	for testing reasons
						if (pEvaluation->BArunmeanliste.size()>1)
						{
							cout << "BA" << endl << pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1] << endl;

							meanpercentchange+=fabs((pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-2]-pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1])
								/
								(pEvaluation->BArunmeanliste[pEvaluation->BArunmeanliste.size()-1]));
						}
						cout << "Cumulative %Change = " << meanpercentchange << endl;
						// + N_0-40
						if (pEvaluation->nheight0b40runmeanliste.size()>1)
						{
							cout << "N_0-40" << endl << pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-1] << endl;
							
							meanpercentchange+=fabs(
								(pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-2]-pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-1])
								/
								(pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size()-1]));
						}
						cout << "Cumulative %Change = " << meanpercentchange << endl;
						// + N_40-200
						if (pEvaluation->nheight41b200runmeanliste.size()>1)
						{
							cout << "N_40-200" << endl << pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1] << endl;
							
							meanpercentchange+=fabs(
								(pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-2]-pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1])
								/
								(pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size()-1]));

						}
						cout << "Cumulative %Change = " << meanpercentchange << endl;
						//StefanC: + N_200+
						if (pEvaluation->nheight201b10000runmeanliste.size()>1)
						{
							cout << "N_200-10000" << endl << pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1] << endl;
							
							
							meanpercentchange+=fabs(
								(pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-2]-pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1])
								/
								(pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size()-1]));
						}						
						cout << "Cumulative %Change = " << meanpercentchange << endl;
						
					}	 // world evaluation list iteration end
					cout << "\t==> STAB%CHANGE = " << meanpercentchange << "\t" << "Stabilization runs = " << parameter[0].ivort << endl << endl;
				
				}
				
				
				
				// Assign the condition on when the year iteration should be stopped at random year position == stabilization period
				if ((parameter[0].ivort>parameter[0].stabilmovingwindow && meanpercentchange<parameter[0].stabilpercentchangethreshold) || stabilerror<=stabilerrorthreshold || parameter[0].ivort>parameter[0].ivortmax)
				{	
					stabilized=true;
				}
			} while (stabilized!=true);
		}

		parameter[0].einschwingen=false;
}





/****************************************************************************************//**
 * \brief Yearsteps - run through the simulation for all years (simdauer)
 *
 * 
 * initialise yearly steps and call vegetationDynamics
 *
 *
 *******************************************************************************************/
void Yearsteps()
{

		printf("\n\nstarting yearly time steps...\n");
		
		
	
		for (int t=0;t<parameter[0].simdauer;t++)
		{ // year step loop 
			
			parameter[0].ivort++;


			// Variable to record a fire in the current year and print it (console output)

			parameter[0].feuerausgabe=0;	

                        //  Calculate current year and print a summary of the year if this is wanted
                        
                        //***german:
						// Aktuelles Jahr berechnen und falls gewuenscht eine Uebersicht ueber das Jahr ausgeben
			int jahr=parameter[0].startjahr+t;
			
			yearposition = ((world_weather_list[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year 
            

			if (parameter[0].yearlyvis ==true) 
			{
				printf("\nSites per location\tyear\ttimestep\tSimulation length\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
				//printf("\nSites pro Ort\tJahr\tZeitschritt\tSimulationsdauer\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
			}
			
			
			///go through all functions for vegetation dynamics
			vegetationDynamics(yearposition, jahr,t);

			
			
			
			//if the year  towards which the whole simulation should be resetted is reached, save all data
			/// Wenn das Jahr, auf das später die ganze Simulation zurückgesetzt werden soll, erreicht ist, dann speichere alle Daten!
			if (jahr==parameter[0].resetyear)
			{
				SaveAllLists();
				cout << "In timestep = " << jahr << " everything was saved!" << endl << endl;
				//cout << "In Zeitschritt = " << jahr << " alles gespeichert!" << endl << endl;
			}

			
		} // year step loop end

		if (parameter[0].resetyear>0)
		{

			bool SCHalterweilVergleichGUT=true;
			
			if(SCHalterweilVergleichGUT==true)
			{

		
				// Variation of parameters depends on experimental setting
				// ... firstly save the value read in from the parameter file
				
				//***german: Parametervariation je nach Expsetting
				// ... erst Speicherung des mit der Parameterdatei eingelesenen Wertes
				double incfac_buffer=parameter[0].incfac;
				double densitywertmanipulatorexp_buffer=parameter[0].densitywertmanipulatorexp;
				int etabbgpermanent_buffer=parameter[0].etabbgpermanent;

				double tempdiffort_buffer=parameter[0].tempdiffort;
				
				for(double tempdifforti=-0.5; tempdifforti<0.9; tempdifforti=tempdifforti+0.5)
				{
					parameter[0].tempdiffort=tempdifforti;
					// read in weather data with new tempdiffort parameter
					// (emptying the weather list before)
					
					//***german: mit neuen tempdiffort muss das weather eingelesen werden
					// ... dafür muss aber vorher die world_weather_list leer gemacht werden.
					//aus finishSimulations()
					for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
					{	// world weather list loop begin
							vector<weather*>& weather_list = *posw;

							// weather_listn loeschen
							for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
							{// weather_list Beginn
								pweather=weather_list[iweather];
								delete pweather;
							}// weather_list Ende
							weather_list.clear();
					} // world weather list loop end

					//from runSimulations()
					weathereinlesen( &parameter[0],  stringlengthmax, world_weather_list);

				
					// Repeat for different parameter settings
					
					//***german:
					// für verschiedene Parameter-Settings wiederholen am 18.11.2014 eingefügt
					for(int parameteri=0;parameteri<4;parameteri++)
					{
						//parameter variation depending on experimental setting
						// ... and change towards desired value
						
						//***german:
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

						
						cout << " starting simulation runs " << endl;
						
						// Reset of all lists and simulation  run:
						
						//***german:
						// Reset der Listen und Simulationslauf
							
								ClearAllLists();
								cout << "           Lists deleted!!" << endl;
								RestoreAllLists();
								cout << "           Lists restored!!" << endl;
								
						printf("\n\n begin the simulation run time steps...\n");
						cout << "     Length of a simulation=" << ((parameter[0].simdauer-(2011-parameter[0].resetyear))+1) << endl;
						

						
						for (int t=((parameter[0].simdauer-(2011-parameter[0].resetyear))+1);t<parameter[0].simdauer;t++)
						{ // year step loop begin
							
							parameter[0].ivort++;
							
							// Variable to record a fire in the current year and print it (console output)

							parameter[0].feuerausgabe=0;	

							//  Calculate current year and print a summary of the year if this is wanted
							// Aktuelles Jahr berechnen und falls gewuenscht eine Uebersicht ueber das Jahr ausgeben
							int jahr=parameter[0].startjahr+t;
							
							yearposition = ((world_weather_list[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year

							if (parameter[0].yearlyvis ==true) 
								printf("\nSites pro Ort\tJahr\tZeitschritt\tSimulationsdauer\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simdauer);
							
							else 
							{
								printf("t=%d", jahr);

								if ((jahr%100)==0) 
									printf("\n"); 
							}

							
							///go through all functions for vegetation dynamics
							vegetationDynamics(yearposition, jahr,t);

							
							/// Wenn das Jahr auf das später die ganze Simulation zurückgesetzt werden soll erreicht ist, dann speichere alle Daten!
							if (jahr==parameter[0].resetyear)
							{
								SaveAllLists();
								cout << "In Zeitschritt = " << jahr << " alles gespeichert!" << endl << endl;
							}

							
						} // year step loop end
						
						// Parameter variation depending on experimental setting,
						// ... afterwards restore initial values
						
						//***german:
						// Parametervariation je nach Expsetting
						// ... dann nach wiederholter Simulation zurücksetzen auf Standardwert
							parameter[0].incfac=incfac_buffer;
							parameter[0].densitywertmanipulatorexp=densitywertmanipulatorexp_buffer;
							parameter[0].etabbgpermanent=etabbgpermanent_buffer;
							
					}//parameter settings END
				
					parameter[0].tempdiffort=tempdiffort_buffer;
				}//weather settings END
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
		{// world list loop begin
		
			for (int j=0;j< parameter[0].mapxlength;j++)		
			{
				list<Tree*> tree_list;							  // Creating new tree_list  
				world_tree_list.push_back(tree_list);			  // include new tree_list in corresponding world list

				list<seed*> seed_list;							  // Creating new seed_list 
				world_seed_list.push_back(seed_list);			  // include new seed_list in corresponding world list

				vector<weather*> weather_list;				 	  // Creating new weather_list 
				world_weather_list.push_back(weather_list);		  // include new weather_list in corresponding world list

				vector<Karten*> plot_list;						  // Creating new plot_list 
				world_plot_list.push_back(plot_list);			  // include new plot_list in corresponding world list

				vector<Evaluation*> evaluation_list;			  // Creating new evaluation_list 
				world_evaluation_list.push_back(evaluation_list); // include new evaluation_list in corresponding world list
				
				
				if (parameter[0].resetyear>0)
				{
					// Create lists for resetting to a certain year
					list<Tree*> tree_list;								// Creating new tree_list  
					world_tree_list_copy.push_back(tree_list);			// include new tree_list in corresponding world list

					list<seed*> seed_list;								// Creating new seed_list 
					world_seed_list_copy.push_back(seed_list);			// include new seed_list in corresponding world list

					vector<Karten*> plot_list;							// Creating new plot_list 
					world_plot_list_copy.push_back(plot_list);			// include new plot_list in corresponding world list

					vector<Evaluation*> evaluation_list;			  	// Creating new evaluation_list 
					world_evaluation_list_copy.push_back(evaluation_list); // include new evaluation_list in corresponding world list
				}
			}
		} // world list loop end
}





/****************************************************************************************//**
 * \brief initialise Karten Element and Evaluation
 *
 *
 *
 *
 *
 *******************************************************************************************/
 
 
 //***german, didn't know how to translate:
void initialiseMaps()
{
		int aktort=0;
		for (vector<vector<Karten*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++)
		{ /// World list loop BEGIN
			vector<Karten*>& plot_list = *posw;
		
			vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list.begin()+aktort);
			vector<Evaluation*>& evaluation_list = *posiwelt;
	
			aktort++;
				
			
			//Calculation of a different position in coordinates:
			//		xwelt= repeating the same position
			//		ywelt= different position along the transect
			//necessary for the global lists
				
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength ); // floor = nächstniedriger Wert (2.3 => 2; -2.3 => -3)
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
		

			// Append new plot list:
			for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++) 
			{ 
				pKarten= new Karten();	// create new plot list element

				pKarten->yworldcoo=aktortyworldcoo;
				pKarten->xworldcoo=aktortxworldcoo;

				pKarten->ycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
				pKarten->xcoo=(double) kartenpos - (pKarten->ycoo * (treecols*parameter[0].sizemagnif));

				pKarten->Treedensitywert=0;
				pKarten->Treeanzahl=0;
				pKarten->maxthawing_depth=1000;
				
				pKarten->auflagenstaerke=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke0=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke1=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke2=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke3=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke4=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke5=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke6=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke7=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke8=200;	// in 0.1mm -> max 6.5535 m
				pKarten->auflagenstaerke9=200;	// in 0.1mm -> max 6.5535 m 
				pKarten->auflagenstaerkemittel=200;
				

				plot_list.push_back(pKarten);	
				//push back new plot list element
			}
			
			
			// Create an evaluation element for each site
			pEvaluation=new Evaluation();
			pEvaluation->yworldcoo=aktortyworldcoo;
			pEvaluation->xworldcoo=aktortxworldcoo;
			pEvaluation->BAliste.clear();
			pEvaluation->BAliste.shrink_to_fit();
			pEvaluation->BArunmeanliste.clear();
			pEvaluation->BArunmeanliste.shrink_to_fit();
			//Start growth stage calculation
			pEvaluation->maxsteigungbasalarea=0.0;
			pEvaluation->countermaxsteigungbasalarea=0;
			pEvaluation->wendejahr=-9999;
			pEvaluation->saettigungsjahr=-9999;
			pEvaluation->nachwendejahr=false;
			//End growth stage calculation
			//Start general values
			pEvaluation->nheight0b40liste.clear();
			pEvaluation->nheight0b40liste.shrink_to_fit();
			pEvaluation->nheight0b40runmeanliste.clear();
			pEvaluation->nheight0b40runmeanliste.shrink_to_fit();
			pEvaluation->nheight41b200liste.clear();
			pEvaluation->nheight41b200liste.shrink_to_fit();
			pEvaluation->nheight41b200runmeanliste.clear();
			pEvaluation->nheight41b200runmeanliste.shrink_to_fit();
			pEvaluation->nheight201b10000liste.clear();
			pEvaluation->nheight201b10000liste.shrink_to_fit();
			pEvaluation->nheight201b10000runmeanliste.clear();
			pEvaluation->nheight201b10000runmeanliste.shrink_to_fit();
			pEvaluation->meanbreastdiameterliste.clear();
			pEvaluation->meanbreastdiameterliste.shrink_to_fit();
			pEvaluation->meanbreastdiameterrunmeanliste.clear();
			pEvaluation->meanbreastdiameterrunmeanliste.shrink_to_fit();
			pEvaluation->stemcountliste.clear();
			pEvaluation->stemcountliste.shrink_to_fit();
			pEvaluation->stemcountrunmeanliste.clear();
			pEvaluation->stemcountrunmeanliste.shrink_to_fit();
			pEvaluation->meantreeheightliste.clear();
			pEvaluation->meantreeheightliste.shrink_to_fit();
			pEvaluation->meantreeheightrunmeanliste.clear();
			pEvaluation->meantreeheightrunmeanliste.shrink_to_fit();
			pEvaluation->meantreeageliste.clear();
			pEvaluation->meantreeageliste.shrink_to_fit();
			pEvaluation->meantreeagerunmeanliste.clear();
			pEvaluation->meantreeagerunmeanliste.shrink_to_fit();
			//Ende general values
			evaluation_list.push_back(pEvaluation);
			
			cout << "evaluation_list.size()=" << evaluation_list.size() << endl;
			
		} /// World list loop END
	cout<<"World list loop END\n";
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
		// Create world lists
		createLists();

		// reading in weather
		weathereinlesen( &parameter[0],  stringlengthmax, world_weather_list);

		// Plot and evaluation list preparation for each location on the transect 
		initialiseMaps();

		// tree input similar to CH17 or seed input
		Treedistribution(&parameter[0], stringlengthmax);
		
		// Variable for the whole progress, including the stabilization phase
		parameter[0].ivort=0;		

		// get to a stable state before starting the real simulation
		Spinupphase();
		
		Yearsteps();

}






/****************************************************************************************//**
 * \brief finish the simulation
 *
 * do something I don't understand \n
 * clear all lists
 *
 *******************************************************************************************/
void finishSimulation()
{
		
		int aktort=0;
		
			
		for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
		{	// World weather list loop begin
			
			
			// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Referenz
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
			// Calculation of the current location in coordinates
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
			
			int number_of_seeds_in_cone=0;
			for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); pos++) 
			{
					if ((*pos)->imcone==true)
					{
						number_of_seeds_in_cone++;
					}
			}
			
			if(parameter[0].yearlyvis ==true)
			{
				printf("seed number Z= %d\n", number_of_seeds_in_cone);
			}
				
		
		
		
				
			// weather_list deletion
			for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
			{// weather_list begin
				 pweather=weather_list.at(iweather);
				 delete pweather;
			}// weather_list end
			weather_list.clear();
			weather_list.shrink_to_fit();
			

		} // World weather list loop end
		
		// Delete all lists after each repeat:
		
		// Calling the function to delete all lists
		ClearAllLists();
		
		
		
		
		cout<<"\nclearing world lists\n";
		world_tree_list.clear();
		world_tree_list.shrink_to_fit();
		world_seed_list.clear();
		world_seed_list.shrink_to_fit();
		world_weather_list.clear();
		world_weather_list.shrink_to_fit();
		world_plot_list.clear();
		world_plot_list.shrink_to_fit();
		world_evaluation_list.clear();
		world_evaluation_list.shrink_to_fit();
		
		
		
		cout<<"clearing wind lists\n";
		
		for(int i=0; i<globalyears.size();i++){
		winddir.at(i).clear();
		winddir.at(i).shrink_to_fit();
	
		windspd.at(i).clear();
		windspd.at(i).shrink_to_fit();
		}
		
		winddir.clear();
		winddir.shrink_to_fit();
	
		windspd.clear();
		windspd.shrink_to_fit();
	
		globalyears.clear();
		globalyears.shrink_to_fit();
	
		
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
	//---------Initialisation ---------
	
		//time registration for the whole simulation run
		clock_t start_time_main = clock();
						

		// random number initialization
		srand((unsigned) time(NULL));

		
		// console output of the version and general information
		printf("\n---->\tLAVESI\n");
		printf("\n You have started  LAVESI-WIND, " 
				"An individually-based and spatial explicit simulation model"
				" for the vegetation dynamics of LARIX (L.)"
				" - driven by temperature, precipitation and wind data."
				"\n\n Version:\t 2.0 (Multiprocessing)"
				"\n Date:\t\t 22.01.2018" 
				"\n authors:"
				"\n\t Stefan Kruse\tstefan.kruse@awi.de"
				"\n\t Alexander Gerdes\talexander.gerdes@awi.de"
				"\n\t Nadja Kath\tnadja.kath@awi.de"
				"\n\t of prior versions:"
				"\n\t Mareike Wieczorek\tmareike.wieczorek@awi.de"
				"\n");
		printf("\n<----\n");

		Parametereinlesen();

		/// Set parameter and variables to starting values
		/// calculation of the starting year of the simulation
		if((parameter[0].weatherchoice== 21) or (parameter[0].weatherchoice== 22) or (parameter[0].weatherchoice== 23) or (parameter[0].weatherchoice== 24))
		{	
			parameter[0].startjahr=2014-parameter[0].simdauer; 
		}
		else
		{
			parameter[0].startjahr=2011-parameter[0].simdauer;
		}
		
		int jahremitseedeintragpuffer=parameter[0].jahremitseedeintrag;	 
		
		parameter[0].repeati=0;

		int zaehler=0;					

		// buffer simulation length
		int simdauerini=parameter[0].simdauer;
		
		for (int nruns=0; nruns<parameter[0].runs; nruns++)
		{
			// Fast R supported SA_levels
			#include "SA_parametervariation.cpp"
		
			parameter[0].starter=false;

			// Begin multiple scenarios
			parameter[0].repeati++;
			parameter[0].simdauer=simdauerini;

			zaehler++;
			printf("\n\tProgress: %d of %d\n", zaehler, parameter[0].runs); 
			
			// Reset variables:
				parameter[0].nameakt=0;
				parameter[0].lineakt=0;	
				parameter[0].jahremitseedeintrag=jahremitseedeintragpuffer;	
				parameter[0].individuenzahl=1;
				
			runSimulation();
				
			finishSimulation();
		}
	
	clock_t end_time_main = clock();
	cout << endl << "... ending simulation.\nTotal time: " << (((double) (end_time_main - start_time_main))/ CLOCKS_PER_SEC) << endl;
	
	// export total simulation time for the current simulation area
	if(parameter[0].computationtimevis==1)
	{
		FILE *fptA;
		fptA=fopen("t_A.txt","a+");
			fprintf(fptA,"%d \t %f \n",(treerows*treecols), (((double) (end_time_main - start_time_main))/ CLOCKS_PER_SEC));
		fclose(fptA);
	}
	return 0;
}
