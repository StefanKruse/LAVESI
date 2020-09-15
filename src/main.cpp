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

// parameterization
// vector<double> vldt_weights;
vector<double> vldt_stemcount;
vector<double> prmz_weights;
vector<double> prmz_mortyouth;
vector<double> prmz_mgrowth;
vector<double> prmz_mweather;
vector<double> prmz_mdrought;
vector<double> prmz_mdensity;
vector<double> prmz_heightweathermorteinflussexp;
vector<double> prmz_germinatioweatherinfluence;
		double ndistrn(float mean, float sdev)
		{
		// Normalverteilte Zahl ziehen und 
			//The polar form of the Box-Muller transformation is both faster and more robust numerically. The algorithmic description of it is:
			float x1, x2, w, y1;//, y2;
			do
			{
				x1 = 2.0 * (0.0 +(float) (1.0*rand()/(RAND_MAX + 1.0) ) ) - 1.0;
				x2 = 2.0 * (0.0 +(float) (1.0*rand()/(RAND_MAX + 1.0) ) ) - 1.0;
				w = x1 * x1 + x2 * x2;
			} while ( w >= 1.0 );

			w = sqrtf( (-2.0 * logf( w ) ) / w );
			y1 = x1 * w;
			// y2 = x2 * w;
			float rn;
			rn= mean + y1 * sdev; // mean=m, sdev=s
			//printf("rn=%4.4f ", rn);
			return rn;
		}

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
    Environmentupdate(&parameter[0], yearposition, world_plot_list, world_tree_list, world_weather_list);

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
    Seeddispersal(yr, &parameter[0], world_seed_list);

    Seedproduction(&parameter[0], world_tree_list);

    if (parameter[0].seedintro == true && parameter[0].yearswithseedintro > 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
        parameter[0].yearswithseedintro--;
    } else if (parameter[0].seedintropermanent == true && parameter[0].yearswithseedintro <= 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
    }

    Hinterlandseedintro(&parameter[0], yearposition, world_seed_list, world_weather_list);

    Treeestablishment(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);

    Dataoutput(t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);

    Mortality(&parameter[0], yr, yearposition, world_tree_list, world_seed_list, world_weather_list);
    wspd.clear();
    wdir.clear();
    wspd.shrink_to_fit();
    wdir.shrink_to_fit();

    Ageing(&parameter[0], world_tree_list, world_seed_list);
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
            // lastyear=2013;
            lastyear = world_weather_list[0][0]->jahr + 100;
            // }

            // choose a random year for weather determination
            double x = rand() / (RAND_MAX + 1.0);
            int jahr = (firstyear + startlag) + (int)((double)((lastyear - startlag) - firstyear) * x);

            // calculate current year position in list, according to first year in the Weather-List and the random year
            int yearposition = (world_weather_list[0][0]->jahr - jahr) * -1;

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
	// clear lists to prevent duplications for multiple runs
	world_tree_list.clear();
	world_seed_list.clear();
	world_weather_list.clear();
	world_plot_list.clear();
	world_evaluation_list.clear();
	
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

void initialiseMaps() {
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
        int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
        int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        for (int kartenpos = 0; kartenpos < (treerows * parameter[0].sizemagnif * treecols * parameter[0].sizemagnif); kartenpos++) {
            auto pEnvirgrid = new Envirgrid();

            pEnvirgrid->yworldcoo = aktortyworldcoo;
            pEnvirgrid->xworldcoo = aktortxworldcoo;

            pEnvirgrid->ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
            pEnvirgrid->xcoo = (double)kartenpos - (pEnvirgrid->ycoo * (treecols * parameter[0].sizemagnif));

            pEnvirgrid->Treedensityvalue = 0;
            pEnvirgrid->Treenumber = 0;
            pEnvirgrid->maxthawing_depth = 1000;

            pEnvirgrid->litterheight = 200;   // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight0 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight1 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight2 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight3 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight4 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight5 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight6 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight7 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight8 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheight9 = 200;  // in 0.1mm -> max 6.5535 m
            pEnvirgrid->litterheightmean = 200;

            plot_list.push_back(pEnvirgrid);
        }

        // create an evaluation element for each site
        auto pEvaluation = new Evaluation();
        pEvaluation->yworldcoo = aktortyworldcoo;
        pEvaluation->xworldcoo = aktortxworldcoo;
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
	
    for (int nruns = 0; nruns < parameter[0].runs; nruns++) {
        parameter[0].starter = false;

        parameter[0].repeati++;
        parameter[0].simduration = simdurationini;

        nruns++;
        printf("\n\tProgress: %d of %d\n", nruns, parameter[0].runs);

        parameter[0].nameakt = 0;
        parameter[0].lineakt = 0;
        parameter[0].yearswithseedintro = yearswithseedintropuffer;

		// START Parameterization
		if(nruns==1)
		{
			// Gewichte speichern => gelten fuer alle Parameter!
			prmz_weights.push_back(1.0);// nur bei Lauf==1
			prmz_weights.push_back(1.0);// nur bei Lauf==1 
		}

		// START set comparisonvalues
			if (parameter[0].weatherchoice==0)
			{
				// 17 levels/sites
				// vldt_weights.push_back();
				vldt_stemcount.push_back(976.15032);
				vldt_stemcount.push_back(2164.50723); 
				vldt_stemcount.push_back(580.03135); 
				vldt_stemcount.push_back(0.0); 
				vldt_stemcount.push_back(3050.00000); 
				vldt_stemcount.push_back(848.82636); 
				vldt_stemcount.push_back(1867.41800); 
				vldt_stemcount.push_back(735.64951); 
				vldt_stemcount.push_back(6812.50000);
				vldt_stemcount.push_back(1200.96154);
				vldt_stemcount.push_back(509.29582);
				vldt_stemcount.push_back(1037.50000);
				vldt_stemcount.push_back(1518.40278);
				vldt_stemcount.push_back(1437.50000);
				vldt_stemcount.push_back(1043.75000);
				vldt_stemcount.push_back(787.50000);
				vldt_stemcount.push_back(16.22251);
						   // # CRUplotlvl Count stemcount_mean stemcount_median stemcount_max
						// # 1           1    13      961.74614        976.15032    2574.77330
						// # 2           2     7     5959.65131       2164.50723   33400.00000
						// # 3           3    21      615.73713        580.03135    1457.15192
									// 4                				  0.0
						// # 4           5     2     3050.00000       3050.00000    4600.00000
						// # 5           6     2      848.82636        848.82636     947.85611
						// # 6           7     3     2999.18648       1867.41800    7115.99434
						// # 7           8     1      735.64951        735.64951     735.64951
						// # 8           9     2     6812.50000       6812.50000    7531.25000
						// # 9          10     2     1200.96154       1200.96154    1575.00000
						// # 10         11     9      780.18000        509.29582    1875.00000
						// # 11         12     2     1037.50000       1037.50000    1375.00000
						// # 12         13     2     1518.40278       1518.40278    1700.00000
						// # 13         14     2     1437.50000       1437.50000    1550.00000
						// # 14         15     6      942.84812       1043.75000    1649.30556
						// # 15         16     2      787.50000        787.50000     975.00000
						// # 16         17     2       16.22251         16.22251      18.47177

			}
		// END set comparisonvalues

// mortyouth=0.1;
// mgrowth=0.1;
// mweather=0.5;
// mdrought=0.1;
// mdensity=0.1;
// heightweathermorteinflussexp=0.75;
// germinatioweatherinfluence=1.0;
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_mortyouth=0.0;
				double parametermax_mortyouth=0.2;

				prmz_mortyouth.push_back(parametermin_mortyouth);
				prmz_mortyouth.push_back(parametermax_mortyouth);
			}
			double sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			double wmean=0.0;
			for (unsigned int position=0;position<prmz_mortyouth.size();++position)
			{
				wmean+=prmz_mortyouth[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			double wsd=0.0;
			for (unsigned int position=0;position<prmz_mortyouth.size();++position)
			{
				wsd+= pow(prmz_mortyouth[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].mortyouth=ndistrn(wmean,wsd);
cout << parameter[0].mortyouth << " ";

					if(parameter[0].mortyouth>=0 && parameter[0].mortyouth<=1)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: mortyouth value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].mortyouth);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_mortyouth.push_back(parameter[0].mortyouth);
			}
		// END for each parameter
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_mgrowth=0.0;
				double parametermax_mgrowth=0.2;

				prmz_mgrowth.push_back(parametermin_mgrowth);
				prmz_mgrowth.push_back(parametermax_mgrowth);
			}
			sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			wmean=0.0;
			for (unsigned int position=0;position<prmz_mgrowth.size();++position)
			{
				wmean+=prmz_mgrowth[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			wsd=0.0;
			for (unsigned int position=0;position<prmz_mgrowth.size();++position)
			{
				wsd+= pow(prmz_mgrowth[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].mgrowth=ndistrn(wmean,wsd);
cout << parameter[0].mgrowth << " ";

					if(parameter[0].mgrowth>=0 && parameter[0].mgrowth<=1)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: mgrowth value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].mgrowth);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_mgrowth.push_back(parameter[0].mgrowth);
			}
		// END for each parameter
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_mweather=0.0;
				double parametermax_mweather=1.0;

				prmz_mweather.push_back(parametermin_mweather);
				prmz_mweather.push_back(parametermax_mweather);
			}
			sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			wmean=0.0;
			for (unsigned int position=0;position<prmz_mweather.size();++position)
			{
				wmean+=prmz_mweather[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			wsd=0.0;
			for (unsigned int position=0;position<prmz_mweather.size();++position)
			{
				wsd+= pow(prmz_mweather[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].mweather=ndistrn(wmean,wsd);
cout << parameter[0].mweather << " ";

					if(parameter[0].mweather>=0 && parameter[0].mweather<=1)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: mweather value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].mweather);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_mweather.push_back(parameter[0].mweather);
			}
		// END for each parameter
		
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_mdrought=0.0;
				double parametermax_mdrought=0.2;

				prmz_mdrought.push_back(parametermin_mdrought);
				prmz_mdrought.push_back(parametermax_mdrought);
			}
			sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			wmean=0.0;
			for (unsigned int position=0;position<prmz_mdrought.size();++position)
			{
				wmean+=prmz_mdrought[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			wsd=0.0;
			for (unsigned int position=0;position<prmz_mdrought.size();++position)
			{
				wsd+= pow(prmz_mdrought[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].mdrought=ndistrn(wmean,wsd);
cout << parameter[0].mdrought << " ";

					if(parameter[0].mdrought>=0 && parameter[0].mdrought<=1)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: mdrought value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].mdrought);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_mdrought.push_back(parameter[0].mdrought);
			}
		// END for each parameter
		
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_mdensity=0.0;
				double parametermax_mdensity=0.2;

				prmz_mdensity.push_back(parametermin_mdensity);
				prmz_mdensity.push_back(parametermax_mdensity);
			}
			sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			wmean=0.0;
			for (unsigned int position=0;position<prmz_mdensity.size();++position)
			{
				wmean+=prmz_mdensity[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			wsd=0.0;
			for (unsigned int position=0;position<prmz_mdensity.size();++position)
			{
				wsd+= pow(prmz_mdensity[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].mdensity=ndistrn(wmean,wsd);
cout << parameter[0].mdensity << " ";

					if(parameter[0].mdensity>=0 && parameter[0].mdensity<=1)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: mdensity value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].mdensity);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_mdensity.push_back(parameter[0].mdensity);
			}
		// END for each parameter
		
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_heightweathermorteinflussexp=0.0;
				double parametermax_heightweathermorteinflussexp=1.5;

				prmz_heightweathermorteinflussexp.push_back(parametermin_heightweathermorteinflussexp);
				prmz_heightweathermorteinflussexp.push_back(parametermax_heightweathermorteinflussexp);
			}
			sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			wmean=0.0;
			for (unsigned int position=0;position<prmz_heightweathermorteinflussexp.size();++position)
			{
				wmean+=prmz_heightweathermorteinflussexp[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			wsd=0.0;
			for (unsigned int position=0;position<prmz_heightweathermorteinflussexp.size();++position)
			{
				wsd+= pow(prmz_heightweathermorteinflussexp[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].heightweathermorteinflussexp=ndistrn(wmean,wsd);
cout << parameter[0].heightweathermorteinflussexp << " ";

					if(parameter[0].heightweathermorteinflussexp>=0.0 && parameter[0].heightweathermorteinflussexp<=2.0)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: heightweathermorteinflussexp value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].heightweathermorteinflussexp);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_heightweathermorteinflussexp.push_back(parameter[0].heightweathermorteinflussexp);
			}
		// END for each parameter
		// START for each parameter set values
			// ... set min/max
			if(nruns==1)
			{
				double parametermin_germinatioweatherinfluence=0.9;
				double parametermax_germinatioweatherinfluence=1.1;

				prmz_germinatioweatherinfluence.push_back(parametermin_germinatioweatherinfluence);
				prmz_germinatioweatherinfluence.push_back(parametermax_germinatioweatherinfluence);
			}
			sumweights=0;
			for (unsigned int position=0;position<prmz_weights.size();++position)
			{
				sumweights+=prmz_weights[position];
			}
cout << "sum weights=" << sumweights << endl;

			// calculate weightet mean and sd
			wmean=0.0;
			for (unsigned int position=0;position<prmz_germinatioweatherinfluence.size();++position)
			{
				wmean+=prmz_germinatioweatherinfluence[position]*prmz_weights[position];
			}
			wmean=wmean/sumweights;

			wsd=0.0;
			for (unsigned int position=0;position<prmz_germinatioweatherinfluence.size();++position)
			{
				wsd+= pow(prmz_germinatioweatherinfluence[position]-wmean, 2) *  (prmz_weights[position]/sumweights);
			}
			wsd=pow(wsd, 0.5);

			// estimate new parameter value
			for(int posi=0; posi<1; ++posi)
			{
				bool inboundary=false;
				int anzahldos=0;
				do
				{
					++anzahldos;

					parameter[0].germinatioweatherinfluence=ndistrn(wmean,wsd);
cout << parameter[0].germinatioweatherinfluence << " ";

					if(parameter[0].germinatioweatherinfluence>=0.0 && parameter[0].germinatioweatherinfluence<=1.0)
					{
						inboundary=true;
					}
					if(anzahldos>1000)
					{
						// print warning to file
						FILE *fpoint;
						string fname;

						fname="datatrees_pameterization_error_log.csv";
						 
						fpoint = fopen (fname.c_str(), "r+");

						if (fpoint == NULL)
						{
							fpoint = fopen (fname.c_str(), "w");

							fprintf(fpoint, "Repeat;");
							fprintf(fpoint, "Prompt;");
							fprintf(fpoint, "\n");

							if (fpoint == NULL)
							{
								fprintf(stderr, "ERROR: file for log parameter sampling warnings not found!\n");
								exit(1);
							}
						}

						// Die neuen Informationen werden ans Ende der Datei geschrieben
						fseek(fpoint,0,SEEK_END);

						// Datenaufbereiten und in die Datei schreiben
						fprintf(fpoint, "%d;", parameter[0].repeati);
						fprintf(fpoint, "WARNING: germinatioweatherinfluence value after 1000 samples not able to estimated in boundaries, loop aborted and set to %4.5f;", parameter[0].germinatioweatherinfluence);
						fprintf(fpoint, "\n");

						fclose (fpoint);

						inboundary=true;
					}
				} while(inboundary==false);

				prmz_germinatioweatherinfluence.push_back(parameter[0].germinatioweatherinfluence);
			}
		// END for each parameter
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		// START print estimated parameters to file
			FILE *ftarget;
			string fname;

			fname="datatrees_parameterization_estimates.csv";
			 
			ftarget = fopen (fname.c_str(), "r+");
			if (ftarget == NULL)
			{
				ftarget = fopen (fname.c_str(), "w");
				fprintf(ftarget, "Repeat;");
				 
				fprintf(ftarget, "mortyouth;"); 
				fprintf(ftarget, "mgrowth;"); 
				fprintf(ftarget, "mweather;"); 
				fprintf(ftarget, "mdrought;"); 
				fprintf(ftarget, "mdensity;"); 
				fprintf(ftarget, "heightweathermorteinflussexp;"); 
				fprintf(ftarget, "germinatioweatherinfluence;"); 

				fprintf(ftarget, "\n");

				if (ftarget == NULL)
				{
					fprintf(stderr, "ERROR: file for parameter output not found!\n");
					exit(1);
				}
			}

			fseek(ftarget,0,SEEK_END);
			fprintf(ftarget, "%d;", parameter[0].repeati);
			fprintf(ftarget, "%4.5f;", parameter[0].mortyouth); 
			fprintf(ftarget, "%4.5f;", parameter[0].mgrowth); 
			fprintf(ftarget, "%4.5f;", parameter[0].mweather); 
			fprintf(ftarget, "%4.5f;", parameter[0].mdrought); 
			fprintf(ftarget, "%4.5f;", parameter[0].mdensity); 
			fprintf(ftarget, "%4.5f;", parameter[0].heightweathermorteinflussexp); 
			fprintf(ftarget, "%4.5f;", parameter[0].germinatioweatherinfluence); 
			fprintf(ftarget, "\n");

			fclose (ftarget);
		// END print estimated parameters to file




























			
		// END Parameterization
		
        runSimulation();
		
		// START Parameterization
			// gewicht startet bei 0 und wird dann mit dem Gewicht aufsummiert von allen Sites und am Ende der Mittelwert berechnet
			// double deviation=0.0;
			double residuals=0.0;

			// Faktor für Umrechnung von pro 20*20 m^2 Fläche zu 1 ha Flächen
			double perhascalingfactor=1.0/((treerows*treecols)/(100.0*100.0));
			
			int aktort_parameterization=0;
			for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
			{
				// list<Tree*>& tree_list = *posw;

				vector<vector<Evaluation*> >::iterator posiweltausw = (world_evaluation_list.begin()+aktort_parameterization);
				vector<Evaluation*>& evaluation_list = *posiweltausw;
				vector<Evaluation*>::iterator posausw = evaluation_list.begin();
				auto pEvaluation=(*posausw);
				
				aktort_parameterization++;
				
				// Berechnung des aktuellen Ortes
				int aktortyweltcoo=(int) floor( (double) (aktort_parameterization-1)/parameter[0].mapxlength );
				// int aktortxweltcoo=(aktort_parameterization-1) - (aktortyweltcoo * parameter[0].mapxlength);

cout << " yweltout = " << aktortyweltcoo << endl;						
				if(parameter[0].weatherchoice==0)
				{
					// +1 to avoid 0 in division
					// deviation+=fabs( ( (1.0+pEvaluation->stemcountrunmeanliste.back()*perhascalingfactor) - vldt_stemcount[aktortyweltcoo]) / (1.0+vldt_stemcount[aktortyweltcoo]) );
// cout << "   " << deviation << "   " << pEvaluation->stemcountrunmeanliste.back() << "   " << ( pEvaluation->stemcountrunmeanliste.back()*perhascalingfactor) << "  " << vldt_stemcount[aktortyweltcoo] << "  " << endl;
					residuals+=fabs( (pEvaluation->stemcountrunmeanliste.back()*perhascalingfactor)-vldt_stemcount[aktortyweltcoo] );
cout << " residuals=" << residuals << endl;
				}
			}
			
			// Gewichte-Wert Mittelwert berechnen und in List ans Ende anfuegen
				// ... wenn keine Individuen gewachsen sind ist der Wert bei 24.0, dann wird der Wert auf 0.001 gesetzt
				double abweichungsmapylength=17; // number of plots

				// limitierung des Maximalwertes auf 100 (Gewicht = 1/ deviation => wert=0.01 => Gewicht=100)
				// if(deviation<0.01)
				// {
					// deviation=0.01;
				// }
				
				if(parameter[0].prmz_autoimprove==1)
				{
					prmz_weights.push_back( (1.0/fabs( residuals/abweichungsmapylength )) );
				} else
				{
					prmz_weights.push_back( 1.0 );
				}

			//Ausgabe der Gewichte in eine Datei
				// Dateinamen zusammensetzen
				fname="databaeume_Gewichte_Parameterization.csv";
				 
				// Datei versuchen zum Lesen und Schreiben zu oeffnen
				ftarget = fopen (fname.c_str(), "r+");
				// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
				if (ftarget == NULL)
				{
				  ftarget = fopen (fname.c_str(), "w");

					fprintf(ftarget, "Repeat;");
					fprintf(ftarget, "Parametersetweight;");
					fprintf(ftarget, "\n");

					if (ftarget == NULL)
					{
						fprintf(stderr, "Fehler: databaeume_Gewichte_Parameterization.csv-Datei konnte nicht geoeffnet werden!\n");
						exit(1);
					}
				}

				// Die neuen Informationen werden ans Ende der Datei geschrieben
				fseek(ftarget,0,SEEK_END);

				// Datenaufbereiten und in die Datei schreiben
				fprintf(ftarget, "%d;", parameter[0].repeati);
				fprintf(ftarget, "%4.5f;", prmz_weights.back());
				fprintf(ftarget, "\n");

				fclose (ftarget);
			// Ende der Ausgabefunktion

cout << "prmz_weights.back() => letzter Wert=" << prmz_weights.back() << endl << endl;
			
			if(parameter[0].prmz_autoimprove==1 && nruns==11) // auf 11 damit noch 10 Werte zum Schätzen verbleiben => BEGINNT bei 0
			{
				// lösche die ersten beiden startwerte diese führen zu einer großen Streuung
				// ... dazu kann auch einfach das Gewicht auf 0.0 gesetzt werden
				cout << "prmz_weights[0] =" << prmz_weights[0] << "prmz_weights[1] =" << prmz_weights[1] << endl;
				prmz_weights[0]=0.0;
				prmz_weights[1]=0.0;
				cout << "prmz_weights[0] =" << prmz_weights[0] << "prmz_weights[1] =" << prmz_weights[1] << endl << endl;

			} 
			if(parameter[0].prmz_autoimprove==1 && nruns>=21) //  auf 21 damit 20 Werte zum Schätzen genutzt werden können
			{
cout << "prmz_weights veraendern:    von= ";
				// kleinsten Gewichtswert auf 0.0 setzen, da immer je Lauf ein neuer hinzukommt sollte es stabil laufen
				signed int minimumgewichtposi=0;
				double minimumgewicht=0.0;
				int minimumgewichtiter=0;
				for(signed int parameteri=0; parameteri<prmz_weights.size(); ++parameteri)
				{
cout << prmz_weights[parameteri] << ", ";
					
					if(prmz_weights[parameteri]>0.0)
					{
						// vergleiche Wert
						// ... wenn es der erste Wert ist, dann speichere diesen in die Puffervariable
						// ... ansonsten vergleiche den aktuellen mit dem gespeicherten Wert
						// ... falls der neue Wert kleiner als der gespeicherte ist, kopiere diesen
						if(minimumgewichtiter==0)
						{
							minimumgewicht=prmz_weights[parameteri];
							minimumgewichtposi=parameteri;
						} else
						{
							if(prmz_weights[parameteri]<minimumgewicht)
							{
								minimumgewicht=prmz_weights[parameteri];
								minimumgewichtposi=parameteri;
							}
						}
						
						++minimumgewichtiter;
					}
				}
cout << endl << "   => kleinster Wert=" << minimumgewicht << " auf Position=" << minimumgewichtposi << endl;
				prmz_weights[minimumgewichtposi]=0.0;
				
				cout << "prmz_weights veraendern:    nach= ";
				// kleinsten Gewichtswert auf 0.0 setzen, da immer je Lauf ein neuer hinzukommt sollte es stabil laufen
				for(signed int parameteri=0; parameteri<prmz_weights.size(); ++parameteri)
				{
					cout << prmz_weights[parameteri] << ", ";
				}
			}

		// END Parameterization
		
    }

    return 0;
}
