#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

int yearposition;

// lists and pointers
vector<list<Tree*>> world_tree_list;
vector<VectorList<Seed>> world_seed_list;
vector<vector<Weather*>> world_weather_list;
vector<vector<Envirgrid*>> world_plot_list;
vector<vector<Evaluation*>> world_evaluation_list;

// lists for resetyear copies
vector<list<Tree*>> world_tree_list_copy;
vector<VectorList<Seed>> world_seed_list_copy;
vector<vector<Envirgrid*>> world_plot_list_copy;
vector<vector<Evaluation*>> world_evaluation_list_copy;

// wind data
vector<int> globalyears;
vector<vector<double>> windspd;
vector<vector<double>> winddir;
int cntr;
vector<double> wdir, wspd;

/****************************************************************************************/
/**
 * \brief go through all functions for vegetation dynamics
 *
 * This function calls all other functions necessary to determine
 *growth, mortality, ageing etc
 *
 *
 *******************************************************************************************/
void vegetationDynamics(int yearposition, int jahr, int t) {

cout << " started vegDyn / ";

cout << "envirupdate+";
    Environmentupdate(&parameter[0], yearposition, world_plot_list, world_tree_list, world_weather_list);

cout << "growth+";
    Growth(&parameter[0], yearposition, world_tree_list, world_weather_list);

    int findyr1 = 0, findyr2 = 0, yr = 0;
    if (parameter[0].windsource != 0 && parameter[0].windsource != 4 && parameter[0].windsource != 5) {
        if (parameter[0].windsource == 1) {
            findyr1 = 1979;
            findyr2 = 2012;
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
cout << "seeddispersal+";
    Seeddispersal(yr, &parameter[0], world_seed_list);

cout << "seedproduction+";
    Seedproduction(&parameter[0], world_tree_list);

    if (parameter[0].seedintro == true && parameter[0].yearswithseedintro > 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
        parameter[0].yearswithseedintro--;
    } else if (parameter[0].seedintropermanent == true && parameter[0].yearswithseedintro <= 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
    }

cout << "hinterlandseed+";
    Hinterlandseedintro(&parameter[0], yearposition, world_seed_list, world_weather_list);

cout << "treeestab+";
    Treeestablishment(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);

cout << "dataoutput+";
    Dataoutput(t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);

cout << "mortality+";
    Mortality(&parameter[0], yr, yearposition, world_tree_list, world_seed_list, world_weather_list);
    wspd.clear();
    wdir.clear();
    wspd.shrink_to_fit();
    wdir.shrink_to_fit();

cout << "ageing";
    Ageing(&parameter[0], world_tree_list, world_seed_list);
	
cout << " /// done " << endl;
}

/****************************************************************************************/
/**
 * \brief Spinupphase - get stable state before starting the real
 *simulation
 *
 * initialise "Einschwingung" to reach a stable state before starting
 *the normal routine and call vegetationDynamics
 *
 *
 *******************************************************************************************/
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

            // if (parameter[0].weatherchoice==21 || parameter[0].weatherchoice==22 || parameter[0].weatherchoice==23 || parameter[0].weatherchoice==24)
            // {
            // firstyear=1934;
            firstyear = world_weather_list[0][0]->jahr;
			cout << "firstyear: " << firstyear << endl;
            // lastyear=2013;
            lastyear = world_weather_list[0][0]->jahr + 100;
			cout << "lastyear: " << lastyear << endl;
            // }

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

            // if ( parameter[0].weatherchoice==21 || parameter[0].weatherchoice==22 || parameter[0].weatherchoice==23 || parameter[0].weatherchoice==24 )
            // {
            // firstyear=1934;
            firstyear = world_weather_list[0][0]->jahr;
            // lastyear=2013;
            lastyear = world_weather_list[0][0]->jahr + 100;
            // }

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

/****************************************************************************************/
/**
 * \brief Yearsteps - run through the simulation for all years
 *(simduration)
 *
 *
 * initialise yearly steps and call vegetationDynamics
 *
 *
 *******************************************************************************************/
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
    }  // year step

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

/****************************************************************************************/
/**
 * \brief create all lists needed for the model
 *
 *
 * (Welt)tree_list \n
 * (Welt)seed_list \n
 * (Welt)weather_list \n
 * (Welt)plot_list \n
 * (Welt)evaluation_list \n
 *******************************************************************************************/
void createLists() {
    for (int i = 0; i < parameter[0].mapylength; i++) {
        for (int j = 0; j < parameter[0].mapxlength; j++) {
            list<Tree*> tree_list;                 // Creating new tree_list
            world_tree_list.push_back(tree_list);  // include new tree_list in corresponding world list

            world_seed_list.emplace_back();  // include new seed_list in corresponding world list

            vector<Weather*> weather_list;               // Creating new weather_list
            world_weather_list.push_back(weather_list);  // include new weather_list in corresponding world list

            vector<Envirgrid*> plot_list;          // Creating new plot_list
            world_plot_list.push_back(plot_list);  // include new plot_list in corresponding world list

            vector<Evaluation*> evaluation_list;               // Creating new evaluation_list
            world_evaluation_list.push_back(evaluation_list);  // include new evaluation_list in corresponding world list

            if (parameter[0].resetyear > 0) {
                // Create lists for resetting to a certain year
                list<Tree*> tree_list;                      // Creating new tree_list
                world_tree_list_copy.push_back(tree_list);  // include new tree_list in corresponding world list

                world_seed_list_copy.emplace_back();  // include new seed_list in corresponding world list

                vector<Envirgrid*> plot_list;               // Creating new plot_list
                world_plot_list_copy.push_back(plot_list);  // include new plot_list in corresponding world list

                vector<Evaluation*> evaluation_list;                    // Creating new evaluation_list
                world_evaluation_list_copy.push_back(evaluation_list);  // include new evaluation_list in corresponding world list
            }
        }
    }
}

/****************************************************************************************/
/**
 * \brief initialise Envirgrid Element and Evaluation
 *
 *
 *
 *
 *
 *******************************************************************************************/

void fillElevations(){
	//0. only once at initializing & if(parameter[0].mapylength==1)
	//1. load elevation data on coars resolution (e.g. 30 m)
	//2. go over each point and interpolate (weighted mean) within a buffer radius of 15 m (minimum 1 point falls in)
	
	if(parameter[0].mapylength==1) {
cout << " ... start dem input" << endl;
cout << " ... treerows=y: " << treerows << "    & treecols=x:" << treecols << endl;

	// ... read dem data
    FILE* f;
    char demfilename[250];
	// char deminputbuf[] = "input/dem_30m_test.csv";
	// char deminputbuf[] = "input/dem_30m_Ilirney_3000m.csv"; //TODO: use parameter to set the input data file
	// char deminputbuf[] = "input/dem_30m_Ilirney_13260x31440m.csv"; //TODO: use parameter to set the input data file
	// char deminputbuf[] = "input/dem_30m_Ilirney_12249x11615m.csv"; //TODO: use parameter to set the input data file
	// char deminputbuf[] = "input/dem_30m_Ilirney_5000x4000m.csv"; //TODO: use parameter to set the input data file
	// char deminputbuf[] = "input/dem_30m_Ilirney_647902x7481367m.csv"; //x=5000 y=4000 north Ilirney
	// char deminputbuf[] = "input/dem_30m_Ilirney_647902x7485357m.csv"; //x=5010 y=8010 north Ilirney
	// char deminputbuf[] = "input/dem_30m_Ilirney_652912x7487367m.csv"; //x=10010 y=10010 north Ilirney
	char deminputbuf[] = "input/dem_30m_Ilirney_653902x7489357m.csv"; //x=11010 y=14010 
	// char deminputbuf[] = "input/dem_30m_Ilirney_647902x7481367m.csv"; //x=5010 y=4020 
    strcpy(demfilename, deminputbuf);
    f = fopen(demfilename, "r");
    if (f == NULL) {
        printf("DEM file not available!\n");
        exit(1);
    }

	// int deminputdimension = treecols/parameter[0].demresolution; // matrix + 1 to avoid border effects
	int deminputdimension_y = treerows/parameter[0].demresolution; // matrix + 1 to avoid border effects
	int deminputdimension_x = treecols/parameter[0].demresolution; // matrix + 1 to avoid border effects
    char puffer[3000];
	vector<double> elevationinput;
	// elevationinput.resize(deminputdimension*deminputdimension,0);
	elevationinput.resize(deminputdimension_y*deminputdimension_x,0);
	int counter=-1;
cout << " ... elevation input set up ..." << endl;
    // read in line by line, and fill dem input vector (dimension e.g. 3x3 km each data point is pixel of 30 m resolution, so a 100x100 matrix with 10000 entries)
    while (fgets(puffer, 3000, f) != NULL) {
		counter++;
		elevationinput[counter] = strtod(strtok(puffer, " "), NULL);
		// for(int i=1; i<deminputdimension; ++i){
		for(int i=1; i<deminputdimension_x; ++i){
			counter++;//0 to 99 are the rows
			elevationinput[counter] = strtod(strtok(NULL, " "), NULL);
		}
    }
    fclose(f);
cout << " ==> elevationinput length = " << elevationinput.size() << endl;

	// for (unsigned int i=0;i<elevationinput.size();i++) {
		// cout << ' ' << elevationinput[i];
		// // if((i+1)%deminputdimension==0)
		// if((i+1)%deminputdimension_y==0)
			// cout << endl;
	// }

	// ... read slope data
    char slopefilename[250];
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_3000m.csv"; //TODO: use parameter to set the input data file
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_13260x31440m.csv"; //manually change in declarations.h -> const int treerows = 3000; y to 31440 and x cols to 13260
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_12249x11615m.csv"; //manually change in declarations.h -> const int treerows = 3000; y to 31440 and x cols to 13260
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_647902x7481367m.csv";
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_647902x7485357m.csv";
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_652912x7487367m.csv";	//1010x1010
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_650902x7485357m.csv";// 8010x8010
	char slopeinputbuf[] = "input/slope_30m_Ilirney_653902x7489357m.csv";//x=11010 y=14010 
	// char slopeinputbuf[] = "input/slope_30m_Ilirney_647902x7481367m.csv"; //x=5010 y=4020 
    strcpy(slopefilename, slopeinputbuf);
    f = fopen(slopefilename, "r");
    if (f == NULL) {
        printf("Slope file not available!\n");
        exit(1);
    }

	vector<double> slopeinput;
	// slopeinput.resize(deminputdimension*deminputdimension,0);
	slopeinput.resize(deminputdimension_y*deminputdimension_x,0);
	counter=-1;
    // read in line by line, and fill dem input vector (dimension e.g. 3x3 km each data point is pixel of 30 m resolution, so a 100x100 matrix with 10000 entries)
    while (fgets(puffer, 3000, f) != NULL) {
		counter++;
		slopeinput[counter] = strtod(strtok(puffer, " "), NULL);
		// for(int i=1; i<deminputdimension; ++i){
		for(int i=1; i<deminputdimension_x; ++i){
			counter++;//0 to 99 are the rows
			slopeinput[counter] = strtod(strtok(NULL, " "), NULL);
		}
    }
    fclose(f);
cout << " ==> slopeinput length = " << slopeinput.size() << endl;

	// for (unsigned int i=0;i<slopeinput.size();i++) {
		// cout << ' ' << slopeinput[i];
		// // if((i+1)%deminputdimension==0)
		// if((i+1)%deminputdimension_y==0)
			// cout << endl;
	// }


	// ... read slope data
    char twifilename[250];
	char twiinputbuf[] = "input/twi_30m_Ilirney_653902x7489357m.csv";//x=11010 y=14010 
	// char twiinputbuf[] = "input/twi_30m_Ilirney_647902x7481367m.csv"; //x=5010 y=4020  
    strcpy(twifilename, twiinputbuf);
    f = fopen(twifilename, "r");
    if (f == NULL) {
        printf("TWI file not available!\n");
        exit(1);
    }

	vector<double> twiinput;
	twiinput.resize(deminputdimension_y*deminputdimension_x,0);
	counter=-1;
    // read in line by line, and fill dem input vector (dimension e.g. 3x3 km each data point is pixel of 30 m resolution, so a 100x100 matrix with 10000 entries)
    while (fgets(puffer, 3000, f) != NULL) {
		counter++;
		twiinput[counter] = strtod(strtok(puffer, " "), NULL);
		// for(int i=1; i<deminputdimension; ++i){
		for(int i=1; i<deminputdimension_x; ++i){
			counter++;//0 to 99 are the rows
			twiinput[counter] = strtod(strtok(NULL, " "), NULL);
		}
    }
    fclose(f);
cout << " ==> twiinput length = " << twiinput.size() << endl;



	// interpolate to envirgrid		
cout << "... interpolate to envirgrid started" << endl;
	for (vector<vector<Envirgrid*>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++) {
		vector<Envirgrid*>& plot_list = *posw;
// TODO: paralellize loop
// cout << " ... treerows=y: " << treerows << "    & treecols=x:" << treecols << "    & sizemagnif:" << parameter[0].sizemagnif << endl;
// cout << " std::numeric_limits<unsigned long long int>::max() =" << std::numeric_limits<unsigned long long int>::max() << endl;


omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers
#pragma omp parallel
		{
#pragma omp for
			for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif); kartenpos++) {
				// determine position and distances to gridpoints in low resolution grid
				// pEnvirgrid->ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
				// pEnvirgrid->xcoo = (double)kartenpos - (pEnvirgrid->ycoo * (treecols * parameter[0].sizemagnif));
				double ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
				double xcoo = (double)kartenpos - ycoo * (treecols * parameter[0].sizemagnif);
				// double ycoodem =deminputdimension*ycoo/parameter[0].sizemagnif/treecols;
				// double xcoodem = deminputdimension*xcoo/parameter[0].sizemagnif/treecols;
				double ycoodem = ycoo/parameter[0].sizemagnif/parameter[0].demresolution;
				double xcoodem = xcoo/parameter[0].sizemagnif/parameter[0].demresolution;
				double ycoodemmod = ycoodem-floor(ycoodem);
				ycoodem = floor(ycoodem);
				double xcoodemmod = xcoodem-floor(xcoodem);
				xcoodem = floor(xcoodem);
	// cout << " y/x = " << ycoo << "/" << xcoo << "   => ycoodem =" << ycoodem <<"   => xcoodem =" << xcoodem << "   => ycoodemmod =" << ycoodemmod <<"   => xcoodemmod =" << xcoodemmod << endl;
	// cout << "ELE=" << elevationinput[ycoodem * 10 + xcoodem] << endl;

	// cout << kartenpos << endl;
				// elevation is filled with elevationoffset from parameters needs to sense elevation from input of 4 corners of grid cell
				if( (ycoodem<(deminputdimension_y-1)) & (xcoodem<(deminputdimension_x-1)) )// only if in range leaving out border
				{
	// cout << elevationinput[ycoodem * deminputdimension_x + xcoodem] 
	// << " .. " << elevationinput[(ycoodem+1) * deminputdimension_x + xcoodem] 
	// << " .. " << elevationinput[ycoodem * deminputdimension_x + (xcoodem+1)] 
	// << " .. " << elevationinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)]<< endl;

	// cout << slopeinput[ycoodem * deminputdimension_x + xcoodem] 
	// << " .. " << slopeinput[(ycoodem+1) * deminputdimension_x + xcoodem] 
	// << " .. " << slopeinput[ycoodem * deminputdimension_x + (xcoodem+1)] 
	// << " .. " << slopeinput[(ycoodem+1) * deminputdimension_x + (xcoodem+1)]<< endl;

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
	// cout << " = => eleinter:" << eleinter << endl;
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
	// cout << " = => countwatercells:" << countwatercells << endl;
					// in case of water (or rock which would need to be implemented) are in the vicinity of the current envir grid cell the value will be set to 32767
					if(countwatercells==0) {
						plot_list[kartenpos]->elevation = plot_list[kartenpos]->elevation + (short int) floor(10*eleinter);
						// plot_list[kartenpos]->slope = slopeinter;
						
						// calculate environment-growth-impact (value between 0 and 1)
							// f(TWI)		= -0.045999* TWI + 0.994066
							// f(slope) 	= k * exp(-1/2 * (xp - mu)^2/sigma^2)
										// 	= 0.85654 * exp(-1/2 * (slope- 8.78692)^2/6.90743^2)
	// cout << " = => slopeinter:" << slopeinter << " => factor:" << (0.85654 * exp((-0.5) * ((slopeinter - 8.78692)*(slopeinter - 8.78692))/(6.90743*6.90743))) << endl;
	// cout << " = => twiinter:" << twiinter << " => factor:" << (-0.045999* twiinter + 0.994066 ) << endl;
						double envirgrowthimpact =
											0.75 * (-0.045999* twiinter + 0.994066 )
											+ (1.0-0.75) * (0.85654 * exp((-0.5) * ((slopeinter - 8.78692)*(slopeinter - 8.78692))/(6.90743*6.90743)));
						// plausibility check
						if(envirgrowthimpact>1.0)
							envirgrowthimpact=1.0;
						if(envirgrowthimpact<0.0)
							envirgrowthimpact=0.0;
						
						// adjust by factor
						double envirgrowthimpactfactor=1.0;
						
						plot_list[kartenpos]->envirgrowthimpact = (unsigned short int) floor(envirgrowthimpactfactor * envirgrowthimpact*10000);
	// cout << " => envirgrowthimit pact in plot=" << plot_list[kartenpos]->envirgrowthimpact << endl;
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
	
	
	// cp -r /bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI /bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_050twi_050slope
	// cd  /bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_050twi_050slope
	// make clean
	// make parallel -j
	// nohup ./LAVESI_WIND &
	
	// cp -r /bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI /bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_025twi_075slope
	// cd  /bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_025twi_075slope
	// make clean
	// make parallel -j
	// nohup ./LAVESI_WIND &
	
	// cp -r /bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI /bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_075twi_025slope
	// cd  /bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_075twi_025slope
	// make clean
	// make parallel -j
	// nohup ./LAVESI_WIND &
	
	
	// TODO: remove not used input vectors
	// elevationinput
	// slopeinput
	// twiinput
	

	// test output with r 
	// setwd("//dmawi/potsdam/data/bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI/output")
	// setwd("/bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI/output")
	setwd("/bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_025twi_075slope/output")
	setwd("/bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_050twi_050slope/output")
	setwd("/bioing/user/stkruse/ChukotkaBiomassSims2020/spatial_growthfactuning/growthfac_100_075twi_025slope/output")
	yrtoplot=1375
	elein=read.csv("datatrees_Treedensity1_2300851.csv", sep=";",dec=".")
	str(elein)	
	elein$Elevation[elein$Elevation==3276.7000]=NA
	library(lattice)
	elein$Xc=elein$X/5
	elein$Yc=elein$Y/5
	
	png("datatrees_Treedensity1_2300851_Elevation.png",width=2100, height=2100)
		p=with(elein, levelplot(Elevation~Xc+Yc, asp=1))
		print(p)
	dev.off()
	png("datatrees_Treedensity1_2300851_Envirgrowthimpact.png",width=2100, height=2100)
		p=with(elein, levelplot(Envirgrowthimpact~Xc+Yc, asp=1))
		print(p)
	dev.off()
	

	trein=read.csv(paste0("datatrees_1_2300851_",yrtoplot,"_1.csv"), sep=";",dec=".")

	p=with(elein, levelplot(Elevation~Xc+Yc, asp=1))
	p2=update(p, panel = function(...) {
	  panel.levelplot(...)
	  panel.xyplot(trein$X,trein$Y, pch = 17, col = "forestgreen", cex=0.5+trein$height/1000)
	})
	png(paste0("datatrees_1_2300851_",yrtoplot,"_1.png"),width=2100, height=2100)
		print(p2)
	dev.off()

	
	
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
        //		xwelt= repeating the same position
        //		ywelt= different position along the transect
        // necessary for the global lists
        // int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

// cout << " ... ... (unsigned long long int) (treerows * parameter[0].sizemagnif * treecols * parameter[0].sizemagnif)= " << (unsigned long long int) (treerows * parameter[0].sizemagnif * treecols * parameter[0].sizemagnif) << endl;
// cout << " ... ... (unsigned long long int) (treerows * parameter[0].sizemagnif * treecols * parameter[0].sizemagnif)= " << ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif) << endl;
// should be for 10010*10010 tiles= 2505002500

// parallelize with pragma as only single core needs ~ 4min for 14x11 km area
		// old replaced by parallelized version
		for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif); kartenpos++) {
            auto pEnvirgrid = new Envirgrid();

            // pEnvirgrid->yworldcoo = aktortyworldcoo;
            // pEnvirgrid->xworldcoo = aktortxworldcoo;

            // pEnvirgrid->ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
            // pEnvirgrid->xcoo = (double)kartenpos - (pEnvirgrid->ycoo * (treecols * parameter[0].sizemagnif));

            pEnvirgrid->Treedensityvalue = 0;
            pEnvirgrid->Treenumber = 0;
            pEnvirgrid->maxthawing_depth = 1000;

            // pEnvirgrid->litterheight = 200;   // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight0 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight1 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight2 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight3 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight4 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight5 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight6 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight7 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight8 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight9 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheightmean = 200;

			// elevation of each grid in m 
			// baseline to zero and set later after parameterization of elevationoffset
			// ... in case of mapylength>1 weather files will be preprocessed on read in
			if(parameter[0].mapylength>1)
				pEnvirgrid->elevation = 0.0; 
			else
				pEnvirgrid->elevation = (short int) floor(10*parameter[0].elevationoffset); // will be filled later with data on top of elevationoffset
			

            plot_list.push_back(pEnvirgrid);
        }

/*
// srtruct		
		// paralellize
		// 1. resize list container to final size
		
		// Envirgrid evi;
		// evi.elevation=0;
		// evi.Treedensityvalue=0; 
		// evi.Treenumber=0; 		
		// evi.maxthawing_depth=0;
		// evi.envirgrowthimpact=0;
	
		// cout << " plot_list resizing, size at start=" << plot_list.size();
		// plot_list.resize(((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif), evi);
		// plot_list.fill(0, ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif)-1, evi);
		// cout << " ====> after= " << plot_list.size() << endl;
		// access last element
		// cout << " last element elevation..." << plot_list[((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif)-1]->elevation << endl;
		// cout << " last element Treenumber..." << plot_list[((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif)-1]->Treenumber << endl;
		
		
cout << " max length should be:" << ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif) << endl;
omp_set_dynamic(0);                                 // disable dynamic teams
omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers
#pragma omp parallel
{
            // int thread_count = omp_get_num_threads();
            // int thread_num = omp_get_thread_num();
            // size_t chunk_size = plot_list.size() / thread_count;
            // auto begin = plot_list.begin();
            // std::advance(begin, thread_num * chunk_size);
            // auto end = begin;

            // if (thread_num == (thread_count - 1))  // last thread takes the remaining elements
            // {
                // end = plot_list.end();
            // } else {
                // std::advance(end, chunk_size);
            // }

// wait for all threads to initialize and then proceed
// #pragma omp barrier

		// for (auto it = begin; it != end; ++it) {
			// auto pEnvirgrid = (*it);
#pragma omp for
		for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif); kartenpos++) {
cout << " kapos:" << kartenpos << endl;
            // auto pEnvirgrid = new Envirgrid();

            // pEnvirgrid->yworldcoo = aktortyworldcoo;
            // pEnvirgrid->xworldcoo = aktortxworldcoo;

            // pEnvirgrid->ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
            // pEnvirgrid->xcoo = (double)kartenpos - (pEnvirgrid->ycoo * (treecols * parameter[0].sizemagnif));

            plot_list[kartenpos]->Treedensityvalue = 0;
            plot_list[kartenpos]->Treenumber = 0;
            plot_list[kartenpos]->maxthawing_depth = 1000;

            // pEnvirgrid->litterheight = 200;   // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight0 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight1 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight2 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight3 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight4 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight5 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight6 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight7 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight8 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheight9 = 200;  // in 0.1mm -> max 6.5535 m
            // pEnvirgrid->litterheightmean = 200;

			// elevation of each grid in m 
			// baseline to zero and set later after parameterization of elevationoffset
			// ... in case of mapylength>1 weather files will be preprocessed on read in
			if(parameter[0].mapylength>1)
				plot_list[kartenpos]->elevation = 0.0; 
			else
				plot_list[kartenpos]->elevation = (short int) floor(10*parameter[0].elevationoffset); // will be filled later with data on top of elevationoffset
			

            // plot_list.push_back(pEnvirgrid);
        }
}
*/ // par end
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

/****************************************************************************************/
/**
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
void runSimulation() {
    createLists();

    Weatherinput(&parameter[0], stringlengthmax, world_weather_list);

    // plot and evaluation list preparation for each location on the transect
    initialiseMaps();

	// compute dem for each envir grid tile from read in data
	if(parameter[0].demlandscape)
		fillElevations();

    // tree input similar to CH17 or seed input
    Treedistribution(&parameter[0], stringlengthmax);

    parameter[0].ivort = 0;

    // get to a stable state before starting the real simulation
    Spinupphase();

    Yearsteps();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////						  /////////////////////////////////////////////////////////
/////////////		   MAIN		      /////////////////////////////////////////////////////////
/////////////			 			  /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************/
/**
 * \brief main routine to start model
 *
 * Read Parameters
 * Start Simulation
 * Finish Simulation
 *
 *******************************************************************************************/
int main() {
    // random number initialization ; TODO delete when all rand() replaces
    srand((unsigned)time(NULL));

    // console output of the version and general information
    printf("\n---->\tLAVESI\n");
    printf(
        "\n You have started  LAVESI-WIND, "
        "An individually-based and spatial explicit simulation model"
        " for the vegetation dynamics of LARIX (Mill.)"
        " - driven by temperature, precipitation and wind data."
        "\n\n Version:\t 1.0 (Multiprocessing)"
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

    // calculation of the starting year of the simulation
    parameter[0].startjahr = parameter[0].lastyearweatherdata - parameter[0].simduration;


    int yearswithseedintropuffer = parameter[0].yearswithseedintro;
    parameter[0].repeati = 0;

    // buffer simulation length
    int simdurationini = parameter[0].simduration;


	// test structure sizes for optimization
	cout << " size of Tree structure: " << sizeof(Tree) << endl;
	cout << " size of Seed structure: " << sizeof(Seed) << endl;
	cout << " size of Envirgrid structure: " << sizeof(Envirgrid) << endl;
	cout << " ... ... " << endl;
	cout << "unsigned short int : " << sizeof(unsigned short int) << endl;
	cout << "short int : " << sizeof(short int) << endl;//-32768 to 32767
	cout << "unsigned int : " << sizeof(unsigned int) << endl;//0 to 4294967295
	cout << "int : " << sizeof(int) << endl;
	cout << "double : " << sizeof(double) << endl;
	cout << "bool : " << sizeof(bool) << endl;
	cout << "unsigned short : " << sizeof(unsigned short) << endl;


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
