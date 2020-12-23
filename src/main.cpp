#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

int yearposition;

// lists and pointers
vector<VectorList<Tree>> world_tree_list;
vector<VectorList<Seed>> world_seed_list;
vector<vector<Weather*>> world_weather_list;
vector<vector<Envirgrid*>> world_plot_list;
vector<vector<Evaluation*>> world_evaluation_list;

// lists for resetyear copies
vector<VectorList<Tree>> world_tree_list_copy;
vector<VectorList<Seed>> world_seed_list_copy;
vector<vector<Envirgrid*>> world_plot_list_copy;
vector<vector<Evaluation*>> world_evaluation_list_copy;

// wind data
vector<int> globalyears;
vector<vector<double>> windspd;
vector<vector<double>> winddir;
int cntr;
vector<double> wdir, wspd;

void vegetationDynamics(int yearposition, int jahr, int t) {

cout << " started vegetation dynamics / ivort=" << parameter[0].ivort << " / ";

auto time_start = chrono::high_resolution_clock::now();
    Environmentupdate(&parameter[0], yearposition, world_plot_list, world_tree_list, world_weather_list);
auto time_end = chrono::high_resolution_clock::now();
chrono::duration<double> elapsed;
elapsed = time_end - time_start;
cout << "envirupdate(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Growth(&parameter[0], yearposition, world_tree_list, world_weather_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Growth(" << elapsed.count() << ")+";

    int findyr1 = 0, findyr2 = 0, yr = 0;
    if (parameter[0].windsource != 0 && parameter[0].windsource != 4 && parameter[0].windsource != 5) {
        if (parameter[0].windsource == 1) {
            findyr1 = 1979;
            findyr2 = 2012;// TODO: adjust to available data
        }
    }

    if (jahr < findyr1 or jahr > findyr2) {
        yr = (findyr1 + (int)(rand() / (RAND_MAX + 1.0) * (findyr2 - findyr1)));
    } else {
        yr = jahr;
    }

    for (int i = 0; i < (signed)globalyears.size(); i++) {
        if (globalyears[i] == yr) {
            for (int pos = 0; pos < (signed)winddir.at(i).size(); pos++) {
                wdir.push_back(winddir.at(i).at(pos));
                wspd.push_back(windspd.at(i).at(pos));
            }
        }
    }
time_start = chrono::high_resolution_clock::now();
    Seeddispersal(yr, &parameter[0], world_seed_list, world_plot_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Seeddispersal(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Seedproduction(&parameter[0], world_tree_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Seedproduction(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    if (parameter[0].seedintro == true && parameter[0].yearswithseedintro > 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
        parameter[0].yearswithseedintro--;
    } else if (parameter[0].seedintropermanent == true && parameter[0].yearswithseedintro <= 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
    }
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Treedistribution(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Hinterlandseedintro(&parameter[0], yearposition, world_seed_list, world_weather_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Hinterlandseedintro(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Treeestablishment(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Treeestablishment(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Dataoutput(t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Dataoutput(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Mortality(&parameter[0], yr, yearposition, world_tree_list, world_seed_list, world_weather_list);
    wspd.clear();
    wdir.clear();
    wspd.shrink_to_fit();
    wdir.shrink_to_fit();
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Mortality(" << elapsed.count() << ")+";

time_start = chrono::high_resolution_clock::now();
    Ageing(&parameter[0], world_tree_list, world_seed_list);
time_end = chrono::high_resolution_clock::now();
elapsed = time_end - time_start;
cout << "Ageing(" << elapsed.count() << ")+";
	
cout << " /// done " << endl;
}

void Spinupphase() {
    int t = -1;

    if (parameter[0].ivortmax > 0 && parameter[0].stabilperiod == false) {
        parameter[0].spinupphase = true;

        printf("\nSpin up phase:");
        do {
            parameter[0].ivort++;

            int firstyear = 0;
            int lastyear = 0;
            int startlag = 5;
			
            firstyear = world_weather_list[0][0]->jahr;
			cout << "firstyear: " << firstyear << endl;
            lastyear = world_weather_list[0][0]->jahr + 100;
			cout << "lastyear: " << lastyear << endl;

            // choose a random year for weather determination
            double x = rand() / (RAND_MAX + 1.0);
            int jahr = (firstyear + startlag) + (int)((double)((lastyear - startlag) - firstyear) * x);

            // calculate current year position in list, according to first year in the Weather-List and the random year
            int yearposition = (world_weather_list[0][0]->jahr - jahr) * -1;
			cout << "yearposition: " << yearposition << endl;

            // go through all functions for vegetation dynamics
            vegetationDynamics(yearposition, jahr, t);

        } while (parameter[0].ivort < parameter[0].ivortmax);

    } else if (parameter[0].ivortmax > 0 && parameter[0].stabilperiod == true) {
        printf("\nStabilization period:\n");

        // choose random years from the weather files until...
        // ... either a percentage of variance of certain observable is reached
        // ... or, if this is not possible, after 1000 years
        double stabilerrorthreshold = 0.1;
        bool stabilized = false;
        do {
            parameter[0].ivort++;

            int firstyear = 0, lastyear = 0;
            int startlag = 5;

            firstyear = world_weather_list[0][0]->jahr;
            lastyear = world_weather_list[0][0]->jahr + 100;

            // take a random year for weather determination
            double x = rand() / (RAND_MAX + 1.0);
            int jahr = (firstyear + startlag) + (int)((double)((lastyear - startlag) - firstyear) * x);

            int yearposition = (world_weather_list[0][0]->jahr - jahr)
                               * -1;  // calculate actual year position in list, according to first year in the Weather-List and the random year
            cout << world_weather_list[0][0]->jahr << endl;

            // progress console output
            printf("==> N:%d/%d\t=>\tzYear=%d\tyearPos=%d\n", parameter[0].ivort, parameter[0].ivortmax, jahr, yearposition);

            // go through all functions for vegetation dynamics
            vegetationDynamics(yearposition, jahr, t);

            // calculation of the deviation from reference values
            double meanpercentchange = 0;
            double stabilerror = 1;
            int stabilizationtype = 1;
            if (stabilizationtype == 1) {  // is an equilibrium reached?

                int aktort = 0;
                for (vector<vector<Evaluation*>>::iterator posausw = world_evaluation_list.begin(); posausw != world_evaluation_list.end();
                     ++posausw) {  // world evaluation list begin
                    vector<Evaluation*>& evaluation_list = *posausw;
                    vector<Evaluation*>::iterator posauswakt = evaluation_list.begin();
                    auto pEvaluation = (*posauswakt);

                    aktort++;

                    if (pEvaluation->basalarearunmeanlist.size() > 1) {
                        cout << "BA" << endl << pEvaluation->basalarearunmeanlist[pEvaluation->basalarearunmeanlist.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation->basalarearunmeanlist[pEvaluation->basalarearunmeanlist.size() - 2]
                                                   - pEvaluation->basalarearunmeanlist[pEvaluation->basalarearunmeanlist.size() - 1])
                                                  / (pEvaluation->basalarearunmeanlist[pEvaluation->basalarearunmeanlist.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                    // + N_0-40
                    if (pEvaluation->nheight0b40runmeanliste.size() > 1) {
                        cout << "N_0-40" << endl << pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size() - 2]
                                                   - pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size() - 1])
                                                  / (pEvaluation->nheight0b40runmeanliste[pEvaluation->nheight0b40runmeanliste.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                    // + N_40-200
                    if (pEvaluation->nheight41b200runmeanliste.size() > 1) {
                        cout << "N_40-200" << endl << pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size() - 2]
                                                   - pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size() - 1])
                                                  / (pEvaluation->nheight41b200runmeanliste[pEvaluation->nheight41b200runmeanliste.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                    // + N_200+
                    if (pEvaluation->nheight201b10000runmeanliste.size() > 1) {
                        cout << "N_200-10000" << endl
                             << pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size() - 2]
                                                   - pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size() - 1])
                                                  / (pEvaluation->nheight201b10000runmeanliste[pEvaluation->nheight201b10000runmeanliste.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                }
                cout << "\t==> STAB%CHANGE = " << meanpercentchange << "\t"
                     << "Stabilization runs = " << parameter[0].ivort << endl
                     << endl;
            }

            // assign the condition on when the year iteration should be stopped at random year position == stabilisation period
            if ((parameter[0].ivort > parameter[0].stabilmovingwindow && meanpercentchange < parameter[0].stabilpercentchangethreshold)
                || stabilerror <= stabilerrorthreshold || parameter[0].ivort > parameter[0].ivortmax) {
                stabilized = true;
            }
        } while (stabilized != true);
    }

    parameter[0].spinupphase = false;
}

void Yearsteps() {
    printf("\n\nstarting yearly time steps...\n");

    for (int t = 0; t < parameter[0].simduration; t++) {
        parameter[0].ivort++;

        // calculate current year and print a summary of the year
        int jahr = parameter[0].startjahr + t;
        yearposition = ((world_weather_list[0][0]->jahr - parameter[0].startjahr) * -1)
                       + t;  // calculate actual year position in the weather-list, according to first year in the Weather-List and the Start-Year

        if (parameter[0].yearlyvis == true) {
            printf("\nSites per location\tyear\ttimestep\tSimulation length\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(), parameter[0].mapylength, jahr,
                   t, parameter[0].simduration);
        }

        // go through all functions for vegetation dynamics
        vegetationDynamics(yearposition, jahr, t);

        // if the year towards which the whole simulation should be resetted is reached, save all data
        if (jahr == parameter[0].resetyear) {
            Savealllists();
            cout << "In year = " << jahr << " everything was saved!" << endl << endl;
        }
		
		// abort simulation when year set
		if( parameter[0].ivort > (parameter[0].ivortmax
								  +parameter[0].stopatyear 
								  -(parameter[0].lastyearweatherdata-parameter[0].simduration)
									)
			) {
			cout << "Simulation aborted as requested in parameter.txt->stopatyear!" << endl;
			exit(1);
		}
	
    }// year step

    // variation of parameters depends on experimental setting beginning at resetyear
    if (parameter[0].resetyear > 0) {
        // save the value read in from the parameter file
        double incfac_buffer = parameter[0].incfac;
        double densityvaluemanipulatorexp_buffer = parameter[0].densityvaluemanipulatorexp;
        int seedintronumberpermanent_buffer = parameter[0].seedintronumberpermanent;

        double tempdiffort_buffer = parameter[0].tempdiffort;

        for (double tempdifforti = -0.5; tempdifforti < 0.9; tempdifforti = tempdifforti + 0.5) {
            parameter[0].tempdiffort = tempdifforti;
            // read in weather data with new tempdiffort parameter
            for (vector<vector<Weather*>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
                vector<Weather*>& weather_list = *posw;

                for (unsigned int iweather = 0; iweather < weather_list.size(); ++iweather) {
                    auto pWeather = weather_list[iweather];
                    delete pWeather;
                }
                weather_list.clear();
            }
            Weatherinput(&parameter[0], stringlengthmax, world_weather_list);

            // repeat simulation runs beginning at resetyear for different parameter settings
            for (int parameteri = 0; parameteri < 4; parameteri++) {
                // parameter variation
                if (parameteri == 1) {
                    parameter[0].incfac = 5;
                } else if (parameteri == 2) {
                    parameter[0].densityvaluemanipulatorexp = 2;
                } else if (parameteri == 3) {
                    parameter[0].seedintronumberpermanent = 1000;
                }

                cout << " starting simulation runs " << endl;

                // reset of the simulation run to resetyear
                Clearalllists();
                cout << "           Lists deleted!!" << endl;
                Restorealllists();
                cout << "           Lists restored!!" << endl;

                printf("\n\n begin the simulation run time steps...\n");
                cout << "     Length of a simulation=" << ((parameter[0].simduration - (2011 - parameter[0].resetyear)) + 1) << endl;

                for (int t = ((parameter[0].simduration - (2011 - parameter[0].resetyear)) + 1); t < parameter[0].simduration; t++) {
                    parameter[0].ivort++;

                    int jahr = parameter[0].startjahr + t;

                    yearposition = ((world_weather_list[0][0]->jahr - parameter[0].startjahr) * -1)
                                   + t;  // calculate actual year position in the weather-list, according to first year in the Weather-List and the startjahr

                    if (parameter[0].yearlyvis == true) {
                        printf("\nSites per location\tYear\tProgress\tSimulation duration\n%zu/%d\t\t%d\t%d\t\t%d\n", world_tree_list.size(),
                               parameter[0].mapylength, jahr, t, parameter[0].simduration);
                    } else {
                        printf("t=%d", jahr);

                        if ((jahr % 100) == 0) {
                            printf("\n");
                        }
                    }

                    // go through all functions for vegetation dynamics
                    vegetationDynamics(yearposition, jahr, t);

                    // save all data at resetyear
                    if (jahr == parameter[0].resetyear) {
                        Savealllists();
                        cout << "At year= " << jahr << " all saved!" << endl << endl;
                    }
                }

                // restore initial values
                parameter[0].incfac = incfac_buffer;
                parameter[0].densityvaluemanipulatorexp = densityvaluemanipulatorexp_buffer;
                parameter[0].seedintronumberpermanent = seedintronumberpermanent_buffer;
            }

            parameter[0].tempdiffort = tempdiffort_buffer;
        }
    }
}

void createLists() {
    for (int i = 0; i < parameter[0].mapylength; i++) {
        for (int j = 0; j < parameter[0].mapxlength; j++) {
            world_tree_list.emplace_back(parameter[0].omp_num_threads);  // include new tree_list in corresponding world list

            world_seed_list.emplace_back(parameter[0].omp_num_threads);  // include new seed_list in corresponding world list

            vector<Weather*> weather_list;               // Creating new weather_list
            world_weather_list.push_back(weather_list);  // include new weather_list in corresponding world list
            vector<Envirgrid*> plot_list;          // Creating new plot_list
            world_plot_list.push_back(plot_list);  // include new plot_list in corresponding world list
            vector<Evaluation*> evaluation_list;               // Creating new evaluation_list
            world_evaluation_list.push_back(evaluation_list);  // include new evaluation_list in corresponding world list

            if (parameter[0].resetyear > 0) {
                // Create lists for resetting to a certain year
                world_tree_list_copy.emplace_back(parameter[0].omp_num_threads);  // include new seed_list in corresponding world list
				
                world_seed_list_copy.emplace_back(parameter[0].omp_num_threads);  // include new seed_list in corresponding world list

                vector<Envirgrid*> plot_list;               // Creating new plot_list
                world_plot_list_copy.push_back(plot_list);  // include new plot_list in corresponding world list
                vector<Evaluation*> evaluation_list;                    // Creating new evaluation_list
                world_evaluation_list_copy.push_back(evaluation_list);  // include new evaluation_list in corresponding world list
            }
        }
    }
}

void fillElevations(){
	// 0. only once at initializing & if(parameter[0].mapylength==1)
	// 1. load elevation data on coars resolution (e.g. 30 m)
	// 2. go over each point and interpolate (weighted mean) within a buffer radius of 15 m (minimum 1 point falls in)
	// TODO: add fileinput to be set by parameter.txt
	
	if(parameter[0].mapylength==1) {
		// ... read dem data
		FILE* f;
		char demfilename[250];
		char deminputbuf[] = "input/dem_30m_Ilirney_647902x7481367m.csv"; //x=5010 y=4020 
		// char deminputbuf[] = "input/dem_30m_Ilirney_653902x7489357m.csv"; //x=11010 y=14010 
		// char deminputbuf[] = "input/dem_30m_Ilirney_x635418-652338m_y7472396-7490606m.csv"; //x=16920 y=18210 
		// char deminputbuf[] = "input/dem_30m_Ilirney_x641658-649518m_y7476056-7490276m.csv"; //x=7860 y=14220 
		// char deminputbuf[] = "input/dem_30m_Ilirney_x641989-649489m_y7476026-7490336m.csv"; //x=7500 y=14310 
		strcpy(demfilename, deminputbuf);
		f = fopen(demfilename, "r");
		if (f == NULL) {
			printf("DEM file not available!\n");
			exit(1);
		}

		int deminputdimension_y = treerows/parameter[0].demresolution; // matrix + 1 to avoid border effects
		int deminputdimension_x = treecols/parameter[0].demresolution; // matrix + 1 to avoid border effects
		char puffer[6000];
		vector<double> elevationinput;
		elevationinput.resize(deminputdimension_y*deminputdimension_x,0);
		int counter=-1;
		// read in line by line, and fill dem input vector (dimension e.g. 3x3 km each data point is pixel of 30 m resolution, so a 100x100 matrix with 10000 entries)
		while (fgets(puffer, 6000, f) != NULL) {
			counter++;
			elevationinput[counter] = strtod(strtok(puffer, " "), NULL);
			for(int i=1; i<deminputdimension_x; ++i){
				counter++;// rows
				elevationinput[counter] = strtod(strtok(NULL, " "), NULL);
			}
		}
		fclose(f);

		// ... read slope data
		char slopefilename[250];
		char slopeinputbuf[] = "input/slope_30m_Ilirney_647902x7481367m.csv"; //x=5010 y=4020 
		// char slopeinputbuf[] = "input/slope_30m_Ilirney_653902x7489357m.csv";//x=11010 y=14010 
		// char slopeinputbuf[] = "input/slope_30m_Ilirney_x635418-652338m_y7472396-7490606m.csv";//x=16920 y=18210 
		// char slopeinputbuf[] = "input/slope_30m_Ilirney_x641658-649518m_y7476056-7490276m.csv";//x=7860 y=14220  
		// char slopeinputbuf[] = "input/slope_30m_Ilirney_x641989-649489m_y7476026-7490336m.csv";//x=7500 y=14310  
		strcpy(slopefilename, slopeinputbuf);
		f = fopen(slopefilename, "r");
		if (f == NULL) {
			printf("Slope file not available!\n");
			exit(1);
		}

		vector<double> slopeinput;
		slopeinput.resize(deminputdimension_y*deminputdimension_x,0);
		counter=-1;
		while (fgets(puffer, 6000, f) != NULL) {
			counter++;
			slopeinput[counter] = strtod(strtok(puffer, " "), NULL);
			for(int i=1; i<deminputdimension_x; ++i){
				counter++;
				slopeinput[counter] = strtod(strtok(NULL, " "), NULL);
			}
		}
		fclose(f);

		// ... read slope data
		char twifilename[250];
		char twiinputbuf[] = "input/twi_30m_Ilirney_647902x7481367m.csv"; //x=5010 y=4020  
		// char twiinputbuf[] = "input/twi_30m_Ilirney_653902x7489357m.csv";//x=11010 y=14010 
		// char twiinputbuf[] = "input/twi_30m_Ilirney_x635418-652338m_y7472396-7490606m.csv";//x=16920 y=18210 
		// char twiinputbuf[] = "input/twi_30m_Ilirney_x641658-649518m_y7476056-7490276m.csv";//x=7860 y=14220 
		// char twiinputbuf[] = "input/twi_30m_Ilirney_x641989-649489m_y7476026-7490336m.csv";//x=7500 y=14310 
		strcpy(twifilename, twiinputbuf);
		f = fopen(twifilename, "r");
		if (f == NULL) {
			printf("TWI file not available!\n");
			exit(1);
		}

		vector<double> twiinput;
		twiinput.resize(deminputdimension_y*deminputdimension_x,0);
		counter=-1;
		while (fgets(puffer, 6000, f) != NULL) {
			counter++;
			twiinput[counter] = strtod(strtok(puffer, " "), NULL);
			for(int i=1; i<deminputdimension_x; ++i){
				counter++;
				twiinput[counter] = strtod(strtok(NULL, " "), NULL);
			}
		}
		fclose(f);

		// interpolate to envirgrid		
		for (vector<vector<Envirgrid*>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++) {
			vector<Envirgrid*>& plot_list = *posw;
// pragma omp initializing
omp_set_dynamic(1); //enable dynamic teams
omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

#pragma omp parallel
{
#pragma omp for
				for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif); kartenpos++) {
					// determine position and distances to gridpoints in low resolution grid
					double ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
					double xcoo = (double)kartenpos - ycoo * (treecols * parameter[0].sizemagnif);
					double ycoodem = ycoo/parameter[0].sizemagnif/parameter[0].demresolution;
					double xcoodem = xcoo/parameter[0].sizemagnif/parameter[0].demresolution;
					double ycoodemmod = ycoodem-floor(ycoodem);
					ycoodem = floor(ycoodem);
					double xcoodemmod = xcoodem-floor(xcoodem);
					xcoodem = floor(xcoodem);

					// elevation is filled with elevationoffset from parameters needs to sense elevation from input of 4 corners of grid cell
					if( (ycoodem<(deminputdimension_y-1)) & (xcoodem<(deminputdimension_x-1)) )// only if in range leaving out border
					{
						double eleinter = (
							// upper left
							elevationinput[ycoodem * deminputdimension_x + xcoodem] * (1-ycoodemmod)
							+ elevationinput[ycoodem * deminputdimension_x + xcoodem] * (1-xcoodemmod)
							// lower left
							+ elevationinput[(ycoodem+1) * deminputdimension_x + xcoodem] * (ycoodemmod)
							+ elevationinput[(ycoodem+1) * deminputdimension_x + xcoodem] * (1-xcoodemmod)
							// upper right
							+ elevationinput[ycoodem * deminputdimension_x + (xcoodem+1)] * (1-ycoodemmod)
							+ elevationinput[ycoodem * deminputdimension_x + (xcoodem+1)] * (xcoodemmod)
							// lower right
							+ elevationinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)] * (ycoodemmod)
							+ elevationinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)] * (xcoodemmod)
							)/4;
						double slopeinter = (
							// upper left
							slopeinput[ycoodem * deminputdimension_x + xcoodem] * (1-ycoodemmod)
							+ slopeinput[ycoodem * deminputdimension_x + xcoodem] * (1-xcoodemmod)
							// lower left
							+ slopeinput[(ycoodem+1) * deminputdimension_x + xcoodem] * (ycoodemmod)
							+ slopeinput[(ycoodem+1) * deminputdimension_x + xcoodem] * (1-xcoodemmod)
							// upper right
							+ slopeinput[ycoodem * deminputdimension_x + (xcoodem+1)] * (1-ycoodemmod)
							+ slopeinput[ycoodem * deminputdimension_x + (xcoodem+1)] * (xcoodemmod)
							// lower right
							+ slopeinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)] * (ycoodemmod)
							+ slopeinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)] * (xcoodemmod)
							)/4;
						double twiinter = (
							// upper left
							twiinput[ycoodem * deminputdimension_x + xcoodem] * (1-ycoodemmod)
							+ twiinput[ycoodem * deminputdimension_x + xcoodem] * (1-xcoodemmod)
							// lower left
							+ twiinput[(ycoodem+1) * deminputdimension_x + xcoodem] * (ycoodemmod)
							+ twiinput[(ycoodem+1) * deminputdimension_x + xcoodem] * (1-xcoodemmod)
							// upper right
							+ twiinput[ycoodem * deminputdimension_x + (xcoodem+1)] * (1-ycoodemmod)
							+ twiinput[ycoodem * deminputdimension_x + (xcoodem+1)] * (xcoodemmod)
							// lower right
							+ twiinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)] * (ycoodemmod)
							+ twiinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)] * (xcoodemmod)
							)/4;

						int countwatercells = 0;
						if( (elevationinput[ycoodem * deminputdimension_x + xcoodem]==9999) 
								| (slopeinput[ycoodem * deminputdimension_x + xcoodem]==9999) 
								| (twiinput[ycoodem * deminputdimension_x + xcoodem]==9999) 
								)
							countwatercells++;
						if( (elevationinput[(ycoodem+1) * deminputdimension_x + xcoodem]==9999)
								| (slopeinput[(ycoodem+1) * deminputdimension_x + xcoodem]==9999) 
								| (twiinput[(ycoodem+1) * deminputdimension_x + xcoodem]==9999) 
								)
							countwatercells++;
						if( (elevationinput[ycoodem * deminputdimension_x + (xcoodem+1)]==9999)
								| (slopeinput[ycoodem * deminputdimension_x + (xcoodem+1)]==9999) 
								| (twiinput[ycoodem * deminputdimension_x + (xcoodem+1)]==9999) 
								)
							countwatercells++;
						if( (elevationinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)]==9999)
								| (slopeinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)]==9999) 
								| (twiinput[(ycoodem +1)* deminputdimension_x + (xcoodem+1)]==9999) 
								)
							countwatercells++;
						// in case of water (or rock which would need to be implemented) are in the vicinity of the current envir grid cell the value will be set to 32767
						if(countwatercells==0) {
							plot_list[kartenpos]->elevation = plot_list[kartenpos]->elevation + (short int) floor(10*eleinter);
							// plot_list[kartenpos]->slope = slopeinter;
							
							// calculate environment-growth-impact (value between 0 and 1)
								// f(TWI)		= slope * TWI + intercept
								// f(slope) 	= k * exp(-1/2 * (xp - mu)^2/sigma^2)
							double envirgrowthimpact =
												parameter[0].slopetwiratio * (-0.045999* twiinter + 0.994066 )
												+ (1-parameter[0].slopetwiratio) * (0.85654 * exp((-0.5) * ((slopeinter - 8.78692)*(slopeinter - 8.78692))/(6.90743*6.90743)));
												
							
							// plausibility check
							if(envirgrowthimpact>1.0)
								envirgrowthimpact=1.0;
							if(envirgrowthimpact<0.0)
								envirgrowthimpact=0.0;

							// adjust by factor
							double envirgrowthimpactfactor=1.0;
							plot_list[kartenpos]->envirgrowthimpact = (unsigned short int) floor(10000* envirgrowthimpactfactor * envirgrowthimpact);
						} else {
							plot_list[kartenpos]->elevation = 32767;
							plot_list[kartenpos]->envirgrowthimpact = 0;
						}
					} else {
						plot_list[kartenpos]->elevation = 32767;
						plot_list[kartenpos]->envirgrowthimpact = 0;
					}
				}
}//pragma
		}
	}
cout << " ... end dem and slope input" << endl;
}

void initialiseMaps() {
cout << " -> started initialise Maps " << endl;
    int aktort = 0;
    for (vector<vector<Envirgrid*>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++) {
        vector<Envirgrid*>& plot_list = *posw;
        vector<vector<Evaluation*>>::iterator posiwelt = (world_evaluation_list.begin() + aktort);
        vector<Evaluation*>& evaluation_list = *posiwelt;
        aktort++;
        // calculation of a different position in coordinates:
        //		xworld= repeating the same position
        //		yworld= different position along the transect
        // necessary for the global lists
        // int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

		plot_list.reserve(((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif)); 

		short int initialelevation = 0;
		if(parameter[0].mapylength==1)
			initialelevation = (short int) floor(10*parameter[0].elevationoffset);

		auto time_start = chrono::high_resolution_clock::now();
		for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif); kartenpos++) {
            auto pEnvirgrid = new Envirgrid({initialelevation,0,0,1000,0});
            plot_list.emplace_back(pEnvirgrid);
        }
		
		auto time_end = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed;
		elapsed = time_end - time_start;
		cout << "Elapsed time: " << elapsed.count() << " s\n";

        // create an evaluation element for each site
        auto pEvaluation = new Evaluation();
        // pEvaluation->yworldcoo = aktortyworldcoo;
        // pEvaluation->xworldcoo = aktortxworldcoo;
        pEvaluation->basalarealist.clear();
        pEvaluation->basalarealist.shrink_to_fit();
        pEvaluation->basalarearunmeanlist.clear();
        pEvaluation->basalarearunmeanlist.shrink_to_fit();
        // growth stage calculation
        pEvaluation->maxincrementbasalarea = 0.0;
        pEvaluation->countermaxincrementbasalarea = 0;
        pEvaluation->yearofturningpoint = -9999;
        pEvaluation->yearofequilibrium = -9999;
        pEvaluation->postyearofturningpoint = false;
        // general values
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

        evaluation_list.push_back(pEvaluation);
    }
cout << " ... ... ended initialise Maps " << endl;
}

void runSimulation() {
    createLists();

    Weatherinput(&parameter[0], stringlengthmax, world_weather_list);

    // plot and evaluation list preparation for each location on the transect
    initialiseMaps();

	// compute dem for each envir grid tile from read in data
	if(parameter[0].demlandscape)
		fillElevations();

    // tree input from files and/or seed input
    Treedistribution(&parameter[0], stringlengthmax);

    parameter[0].ivort = 0;

    Spinupphase();
    Yearsteps();
}

///////////////////////////////////////////////////////////////////////////////////////////////
/////////////						  /////////////////////////////////////////////////////////
/////////////		   MAIN		      /////////////////////////////////////////////////////////
/////////////			 			  /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    // random number initialization ; TODO delete when all rand() are replaced
    srand((unsigned)time(NULL));

    // console output of the version and general information
    printf("\n---->\tLAVESI\n");
    printf(
        "\n You have started  LAVESI, "
        "An individual-based and spatially explicit simulation model for vegetation dynamics of summergreen larches (Larix Mill.) in a 3-dimensional landscape - driven by temperature, precipitation and wind data."
        "\n\n Version:\t 1.2 (LAVESI-WIND-3DENVIR)"
        "\n Date:\t\t 22.12.2020"
        "\n authors:"
        "\n\t Stefan Kruse\tstefan.kruse@awi.de"
        "\n\t of prior versions:"
        "\n\t Alexander Gerdes, Nadja Kath, Mareike Wieczorek"
        "\n");
    printf("\n<----\n");

	// pragma omp initializing
	omp_set_dynamic(1); //enable dynamic teams
	omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

	// read in all simulation parameters from parameters.txt
    Parameterinput();

    // calculation of the starting year of the simulation
    parameter[0].startjahr = parameter[0].lastyearweatherdata - parameter[0].simduration;

	// initialize run parameters for each repeated run
    int yearswithseedintropuffer = parameter[0].yearswithseedintro;
    parameter[0].repeati = 0;
    int simdurationini = parameter[0].simduration;

	// run repeated simulations
    for (int nruns = 0; nruns < parameter[0].runs; nruns++) {
        parameter[0].starter = false;

        parameter[0].repeati++;
        parameter[0].simduration = simdurationini;

        nruns++;
        printf("\n\tProgress: %d of %d\n", nruns, parameter[0].runs);

        parameter[0].nameakt = 0;
        // parameter[0].lineakt = 0;
        parameter[0].yearswithseedintro = yearswithseedintropuffer;

        runSimulation();
    }

    return 0;
}

