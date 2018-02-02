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
	// Environmentupdate
	double start_time_kartenup = omp_get_wtime();
	Environmentupdate(&parameter[0], yearposition, world_plot_list, world_tree_list, world_weather_list);
	double end_time_kartenup = omp_get_wtime();

	// Growth
	double start_time_wachstum = omp_get_wtime();
	Growth( &parameter[0], yearposition, world_tree_list, world_weather_list);
	double end_time_wachstum = omp_get_wtime();

	// Seeddispersal
	double start_time_Seeddispersal = omp_get_wtime();

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
			

	Seeddispersal( yr, &parameter[0], world_seed_list);
	double end_time_Seeddispersal = omp_get_wtime();

	// Seedproduction
	double start_time_Seedproduction = omp_get_wtime();
	Seedproduction( &parameter[0], world_tree_list);
	double end_time_Seedproduction = omp_get_wtime();


	// Treedistribution	
	double start_time_Treedistribution = omp_get_wtime();	
	if (parameter[0].seedintro==true && parameter[0].yearswithseedintro>0)
	{
		
		parameter[0].starter=true;
		
		Treedistribution(&parameter[0], stringlengthmax);
		
		parameter[0].yearswithseedintro--;
	}
	else if ( parameter[0].seedintropermanent==true && parameter[0].yearswithseedintro<=0) 
	{
		parameter[0].starter=true;
		Treedistribution(&parameter[0],  stringlengthmax);
	}
	double end_time_Treedistribution = omp_get_wtime();


	// Treeestablishment
	double start_time_etablierung = omp_get_wtime();
	Treeestablishment(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);
	double end_time_etablierung = omp_get_wtime();

	// Dataoutput
	double start_time_Dataoutput = omp_get_wtime();
	Dataoutput(t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);
	double end_time_Dataoutput = omp_get_wtime();
				
	// Mortality
	double start_time_mortalitaet = omp_get_wtime();
	Mortality( &parameter[0],yr, yearposition, world_tree_list, world_seed_list, world_weather_list);
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
		{
			printf("\n   - plotupdategrowth    Seedwinddispersal  seedprod  treedistr treeestab  output    mortality ageing    TOTAL     ");
		}
		
		printf("\n %d  - %10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f", 
			parameter[0].ivort,
			end_time_kartenup - start_time_kartenup ,
			end_time_wachstum - start_time_wachstum ,
			end_time_Seeddispersal - start_time_Seeddispersal,
			end_time_Seedproduction - start_time_Seedproduction,
			end_time_Treedistribution - start_time_Treedistribution,
			end_time_etablierung - start_time_etablierung,
			end_time_Dataoutput - start_time_Dataoutput ,
			end_time_mortalitaet - start_time_mortalitaet,
			end_time_Ageing - start_time_Ageing ,
			((end_time_Ageing - start_time_Ageing)+
				(end_time_mortalitaet - start_time_mortalitaet)+
				(end_time_Dataoutput - start_time_Dataoutput)+
				(end_time_etablierung - start_time_etablierung)+
				(end_time_Treedistribution - start_time_Treedistribution)+
				(end_time_Seedproduction - start_time_Seedproduction)+
				(end_time_Seeddispersal - start_time_Seeddispersal)+
				(end_time_wachstum - start_time_wachstum)+
				(end_time_kartenup - start_time_kartenup)
				)
			 );

		// output to file
		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin());
		list<Tree*>& tree_list = *world_positon_b;

		openmort:
		FILE *fp3;
		fp3 =fopen("t_N_mort.txt","a+");
		if(fp3==0)
		{
			goto openmort;
		}
		
		fprintf(fp3,"%lu;%d;%f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f;%10.20f\n",
			tree_list.size(),
			parameter[0].ivort, 
			end_time_mortalitaet - start_time_mortalitaet,
			end_time_kartenup - start_time_kartenup,
			end_time_wachstum - start_time_wachstum,
			end_time_Seeddispersal - start_time_Seeddispersal,
			end_time_Seedproduction - start_time_Seedproduction,
			end_time_Treedistribution - start_time_Treedistribution,
			end_time_etablierung - start_time_etablierung,
			end_time_Dataoutput - start_time_Dataoutput ,
			end_time_mortalitaet - start_time_mortalitaet,
			end_time_Ageing - start_time_Ageing ,
			(end_time_Ageing - start_time_Ageing)+
				(end_time_mortalitaet - start_time_mortalitaet)+
				(end_time_Dataoutput - start_time_Dataoutput)+
				(end_time_etablierung - start_time_etablierung)+
				(end_time_Treedistribution - start_time_Treedistribution)+
				(end_time_Seedproduction - start_time_Seedproduction)+
				(end_time_Seeddispersal - start_time_Seeddispersal)+
				(end_time_wachstum - start_time_wachstum)+
				(end_time_kartenup - start_time_kartenup)
			);
			
		fclose(fp3);
	}// computation time output
	
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
 * \brief Yearsteps - run through the simulation for all years (simduration)
 *
 * 
 * initialise yearly steps and call vegetationDynamics
 *
 *
 *******************************************************************************************/
void Yearsteps()
{
		printf("\n\nstarting yearly time steps...\n");
	
		for (int t=0;t<parameter[0].simduration;t++)
		{ 
			parameter[0].ivort++;

			// calculate current year and print a summary of the year
			int jahr=parameter[0].startjahr+t;
			yearposition = ((world_weather_list[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year 

			if (parameter[0].yearlyvis ==true) 
			{
				printf("\nSites per location\tyear\ttimestep\tSimulation length\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simduration);
			}
			
			
			// go through all functions for vegetation dynamics
			vegetationDynamics(yearposition, jahr,t);

			// if the year towards which the whole simulation should be resetted is reached, save all data
			if (jahr==parameter[0].resetyear)
			{
				Savealllists();
				cout << "In year = " << jahr << " everything was saved!" << endl << endl;
			}

			
		}// year step
		
		// variation of parameters depends on experimental setting beginning at resetyear
		if (parameter[0].resetyear>0)
		{
			// save the value read in from the parameter file
			double incfac_buffer=parameter[0].incfac;
			double densityvaluemanipulatorexp_buffer=parameter[0].densityvaluemanipulatorexp;
			int seedintronumberpermanent_buffer=parameter[0].seedintronumberpermanent;

			double tempdiffort_buffer=parameter[0].tempdiffort;
			
			for(double tempdifforti=-0.5; tempdifforti<0.9; tempdifforti=tempdifforti+0.5)
			{
				parameter[0].tempdiffort=tempdifforti;
				// read in weather data with new tempdiffort parameter
				for (vector<vector<Weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
				{
					vector<Weather*>& weather_list = *posw;

					// empty list
					for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
					{
						pWeather=weather_list[iweather];
						delete pWeather;
					}
					weather_list.clear();
				}
				Weatherinput( &parameter[0],  stringlengthmax, world_weather_list);

				// repeat simulation runs beginning at resetyear for different parameter settings
				for(int parameteri=0;parameteri<4;parameteri++)
				{
					// manual parameter variation
					if(parameteri==1)
					{
						parameter[0].incfac=5;
					}
					else if(parameteri==2)
					{
						parameter[0].densityvaluemanipulatorexp=2;
					}
					else if(parameteri==3)
					{
						parameter[0].seedintronumberpermanent=1000;
					}

					cout << " starting simulation runs " << endl;
					
					// reset of the simulation run to resetyear
					Clearalllists();
					cout << "           Lists deleted!!" << endl;
					Restorealllists();
					cout << "           Lists restored!!" << endl;
							
					printf("\n\n begin the simulation run time steps...\n");
					cout << "     Length of a simulation=" << ((parameter[0].simduration-(2011-parameter[0].resetyear))+1) << endl;
					
					for (int t=((parameter[0].simduration-(2011-parameter[0].resetyear))+1);t<parameter[0].simduration;t++)
					{
						
						parameter[0].ivort++;
						
						// calculate current year and print a summary of the year if this is wanted
						int jahr=parameter[0].startjahr+t;
						
						yearposition = ((world_weather_list[0][0]->jahr-parameter[0].startjahr) * -1)+t; // calculate actual year position in the weather-list, according to first year in the Weather-List and the startjahr

						if (parameter[0].yearlyvis==true)
						{
							printf("\nSites pro Ort\tJahr\tZeitschritt\tSimulationsdauer\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr, t, parameter[0].simduration);
						}						
						else 
						{
							printf("t=%d", jahr);

							if ((jahr%100)==0)
							{
								printf("\n");
							}
						}
						
						// go through all functions for vegetation dynamics
						vegetationDynamics(yearposition, jahr,t);
						
						// save all data at resetyear 
						if (jahr==parameter[0].resetyear)
						{
							Savealllists();
							cout << "At year= " << jahr << " all saved!" << endl << endl;
						}
					} // year steps
					
					// restore initial values
					parameter[0].incfac=incfac_buffer;
					parameter[0].densityvaluemanipulatorexp=densityvaluemanipulatorexp_buffer;
					parameter[0].seedintronumberpermanent=seedintronumberpermanent_buffer;
						
				}// parameter settings
			
				parameter[0].tempdiffort=tempdiffort_buffer;
			}// weather settings
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

				list<Seed*> seed_list;							  // Creating new seed_list 
				world_seed_list.push_back(seed_list);			  // include new seed_list in corresponding world list

				vector<Weather*> weather_list;				 	  // Creating new weather_list 
				world_weather_list.push_back(weather_list);		  // include new weather_list in corresponding world list

				vector<Envirgrid*> plot_list;						  // Creating new plot_list 
				world_plot_list.push_back(plot_list);			  // include new plot_list in corresponding world list

				vector<Evaluation*> evaluation_list;			  // Creating new evaluation_list 
				world_evaluation_list.push_back(evaluation_list); // include new evaluation_list in corresponding world list
				
				
				if (parameter[0].resetyear>0)
				{
					// Create lists for resetting to a certain year
					list<Tree*> tree_list;								// Creating new tree_list  
					world_tree_list_copy.push_back(tree_list);			// include new tree_list in corresponding world list

					list<Seed*> seed_list;								// Creating new seed_list 
					world_seed_list_copy.push_back(seed_list);			// include new seed_list in corresponding world list

					vector<Envirgrid*> plot_list;							// Creating new plot_list 
					world_plot_list_copy.push_back(plot_list);			// include new plot_list in corresponding world list

					vector<Evaluation*> evaluation_list;			  	// Creating new evaluation_list 
					world_evaluation_list_copy.push_back(evaluation_list); // include new evaluation_list in corresponding world list
				}
			}
		} // world list loop end
}





/****************************************************************************************//**
 * \brief initialise Envirgrid Element and Evaluation
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
		for (vector<vector<Envirgrid*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++)
		{ /// World list loop BEGIN
			vector<Envirgrid*>& plot_list = *posw;
		
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
				pEnvirgrid= new Envirgrid();	// create new plot list element

				pEnvirgrid->yworldcoo=aktortyworldcoo;
				pEnvirgrid->xworldcoo=aktortxworldcoo;

				pEnvirgrid->ycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
				pEnvirgrid->xcoo=(double) kartenpos - (pEnvirgrid->ycoo * (treecols*parameter[0].sizemagnif));

				pEnvirgrid->Treedensityvalue=0;
				pEnvirgrid->Treenumber=0;
				pEnvirgrid->maxthawing_depth=1000;
				
				pEnvirgrid->litterheight=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight0=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight1=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight2=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight3=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight4=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight5=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight6=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight7=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight8=200;	// in 0.1mm -> max 6.5535 m
				pEnvirgrid->litterheight9=200;	// in 0.1mm -> max 6.5535 m 
				pEnvirgrid->litterheightmean=200;
				

				plot_list.push_back(pEnvirgrid);	
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
			pEvaluation->maxincrementbasalarea=0.0;
			pEvaluation->countermaxincrementbasalarea=0;
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
		Weatherinput( &parameter[0],  stringlengthmax, world_weather_list);

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
		
			
		for (vector<vector<Weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw)
		{	// World weather list loop begin
			
			
			// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Referenz
			// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
			// realisiert ist weiterverfahren werden
			// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
			vector<Weather*>& weather_list = *posw;

			// Um auf ein bestimmtes Element in der Welt zuzugreifen muss ein Iterator bis
			// zum entsprechenden Element justiert werden und dann eine tree_list als Referenz
			// erzeugt werden
			vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
			list<Tree*>& tree_list = *world_positon_b;

			vector<list<Seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
			list<Seed*>& seed_list = *world_positon_s;

			//vector<vector<Envirgrid*> >::iterator posiweltk = (world_plot_list.begin()+aktort);
			//vector<Envirgrid*>& plot_list = *posiweltk;

			//vector<vector<Evaluation*> >::iterator posiwelt = (world_evaluation_list.begin()+aktort);
			//vector<Evaluation*>& evaluation_list = *posiwelt;
			
			
			aktort++;
			// Calculation of the current location in coordinates
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
			
			int number_of_seeds_in_cone=0;
			for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); pos++) 
			{
					if ((*pos)->incone==true)
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
				 pWeather=weather_list.at(iweather);
				 delete pWeather;
			}// weather_list end
			weather_list.clear();
			weather_list.shrink_to_fit();
			

		} // World weather list loop end
		
		// Delete all lists after each repeat:
		
		// Calling the function to delete all lists
		Clearalllists();
		
		
		
		
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
			Clearalllists_copy();
			
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
				" for the vegetation dynamics of LARIX (Mill.)"
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

		Parameterinput();

		/// Set parameter and variables to starting values
		/// calculation of the starting year of the simulation
		if((parameter[0].weatherchoice== 21) or (parameter[0].weatherchoice== 22) or (parameter[0].weatherchoice== 23) or (parameter[0].weatherchoice== 24))
		{	
			parameter[0].startjahr=2014-parameter[0].simduration; 
		}
		else
		{
			parameter[0].startjahr=2011-parameter[0].simduration;
		}
		
		int yearswithseedintropuffer=parameter[0].yearswithseedintro;	 
		
		parameter[0].repeati=0;

		int zaehler=0;					

		// buffer simulation length
		int simdurationini=parameter[0].simduration;
		
		for (int nruns=0; nruns<parameter[0].runs; nruns++)
		{
			parameter[0].starter=false;

			// Begin multiple scenarios
			parameter[0].repeati++;
			parameter[0].simduration=simdurationini;

			zaehler++;
			printf("\n\tProgress: %d of %d\n", zaehler, parameter[0].runs); 
			
			// Reset variables:
				parameter[0].nameakt=0;
				parameter[0].lineakt=0;	
				parameter[0].yearswithseedintro=yearswithseedintropuffer;	
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
