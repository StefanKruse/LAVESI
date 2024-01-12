#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

Parameter parameter[1];
Speciestraits speciestrait[99];
Pesttraits pesttrait[99];

int yearposition;

// lists and pointers
vector<VectorList<Tree>> world_tree_list;
vector<VectorList<Seed>> world_seed_list;
vector<vector<Weather>> world_weather_list;
vector<vector<Envirgrid>> world_plot_list;
vector<vector<Evaluation>> world_evaluation_list;

// lists for resetyear copies
vector<VectorList<Tree>> world_tree_list_copy;
vector<VectorList<Seed>> world_seed_list_copy;
vector<vector<Envirgrid>> world_plot_list_copy;
vector<vector<Evaluation>> world_evaluation_list_copy;

// wind data
vector<int> globalyears;
vector<vector<double>> windspd;
vector<vector<double>> winddir;
int cntr;
vector<double> wdir, wspd;

void vegetationDynamics(int yearposition, int jahr, int t) {
    RandomNumber<double> uniform(0, 1);

#ifdef OUTPUT_COMP_DURATION
    cout << " started vegetation dynamics / ivort=" << parameter[0].ivort << " / ";
#endif

#ifdef OUTPUT_COMP_DURATION
    auto time_start = chrono::high_resolution_clock::now();
#endif
    Environmentupdate(//&parameter[0], 
					  yearposition, 
					  world_plot_list, 
					  world_tree_list, 
					  world_weather_list);
#ifdef OUTPUT_COMP_DURATION
    auto time_end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed;
    elapsed = time_end - time_start;
    cout << "envirupdate(" << elapsed.count() << ")+";
#endif
	if(parameter[0].ivort > parameter[0].ivortmax)
		Fire(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);

#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Growth(&parameter[0], yearposition, world_tree_list, world_weather_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Growth(" << elapsed.count() << ")+";
#endif

    int findyr1 = 0, findyr2 = 0, yr = 0;
    if (parameter[0].windsource != 0 && parameter[0].windsource != 4 && parameter[0].windsource != 5) {
        if (parameter[0].windsource == 1) {
            findyr1 = 1979;
            findyr2 = 2012;  // TODO: adjust to available data
        } else if (parameter[0].windsource == 999) {
			findyr1 = 1;
			findyr2 = 25070;
	} else if (parameter[0].windsource == 998) {
			findyr1 = 1;
			findyr2 = 25070;

		}
    }

    if (jahr < findyr1 or jahr > findyr2) {
        yr = (findyr1 + (int)(uniform.draw() * (findyr2 - findyr1)));
    } else {
        yr = jahr;
    }

    for (int i = 0; i < (signed)globalyears.size(); i++) {
        if (globalyears[i] == yr) {
            const auto& winddir_p = winddir[i];
            std::copy(std::begin(winddir_p), std::end(winddir_p), std::back_inserter(wdir));
            const auto& windspd_p = windspd[i];
            std::copy(std::begin(windspd_p), std::end(windspd_p), std::back_inserter(wspd));
        }
    }
#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Seeddispersal(//yr, 
				  &parameter[0], 
				  world_seed_list, 
				  world_plot_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Seeddispersal(" << elapsed.count() << ")+";
#endif

#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Seedproduction(&parameter[0], world_tree_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Seedproduction(" << elapsed.count() << ")+";
#endif

#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    if (parameter[0].seedintro == true && parameter[0].yearswithseedintro > 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
        parameter[0].yearswithseedintro--;
    } else if (parameter[0].seedintropermanent == true && parameter[0].yearswithseedintro <= 0) {
        parameter[0].starter = true;
        Treedistribution(&parameter[0], stringlengthmax);
    }
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Treedistribution(" << elapsed.count() << ")+";
#endif

#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Hinterlandseedintro(&parameter[0], yearposition, world_seed_list, world_weather_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Hinterlandseedintro(" << elapsed.count() << ")+";
#endif

#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Treeestablishment(&parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Treeestablishment(" << elapsed.count() << ")+";
#endif

/*	if(parameter[0].ivort % 20 == 0){// test fire impact
        for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
            VectorList<Tree>& tree_list = *posw;

#pragma omp parallel for default(shared) private(uniform) schedule(guided)
			for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
				auto& tree = tree_list[tree_i];

				if (tree.growing == true){
					if(tree.xcoo/1000 > 2500){ // half of the plot for testing
						tree.crownstart = 500*10; // flames reach 500 cm high
						tree.relcrowndamage = ((tree.crownstart / 10) / (tree.height / 10))*1000; // update relative crown damage for mortality
// cout << tree.xcoo << " <- " << tree.crownstart/10 << " ... " << tree.height/10 << " ... " << tree.relcrowndamage/1000 << endl;
					}
				}
			}
		}
	}
*/
#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Dataoutput(t, jahr, &parameter[0], yearposition, world_tree_list, world_seed_list, world_weather_list, world_plot_list, world_evaluation_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Dataoutput(" << elapsed.count() << ")+";
#endif
	
	
#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Mortality(&parameter[0], 
			  // yr, 
			  yearposition, 
			  world_tree_list, 
			  world_seed_list, 
			  world_weather_list);
    wspd.clear();
    wdir.clear();
    wspd.shrink_to_fit();
    wdir.shrink_to_fit();
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Mortality(" << elapsed.count() << ")+";
#endif

#ifdef OUTPUT_COMP_DURATION
    time_start = chrono::high_resolution_clock::now();
#endif
    Ageing(&parameter[0], world_tree_list, world_seed_list);
#ifdef OUTPUT_COMP_DURATION
    time_end = chrono::high_resolution_clock::now();
    elapsed = time_end - time_start;
    cout << "Ageing(" << elapsed.count() << ")+";
#endif

#ifdef OUTPUT_COMP_DURATION
    cout << " /// done " << endl;
#endif
}

void Spinupphase() {
    RandomNumber<double> uniform(0, 1);

    int t = -1;

    if (parameter[0].ivortmax > 0 && parameter[0].stabilperiod == false) {
        parameter[0].spinupphase = true;

        printf("\nSpin up phase:");
        do {
            parameter[0].ivort++;

            int firstyear = 0;
            int lastyear = 0;
            int startlag = 5;

            firstyear = world_weather_list[0][0].jahr;
            cout << "firstyear: " << firstyear << endl;
            lastyear = world_weather_list[0][0].jahr + 100;
            cout << "lastyear: " << lastyear << endl;

            // choose a random year for weather determination
            double x = uniform.draw();
            int jahr = (firstyear + startlag) + (int)((double)((lastyear - startlag) - firstyear) * x);

            // calculate current year position in list, according to first year in the Weather-List and the random year
            int yearposition = (world_weather_list[0][0].jahr - jahr) * -1;
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

            firstyear = world_weather_list[0][0].jahr;
            lastyear = world_weather_list[0][0].jahr + 100;

            // take a random year for weather determination
            double x = uniform.draw();
            int jahr = (firstyear + startlag) + (int)((double)((lastyear - startlag) - firstyear) * x);

            int yearposition = (world_weather_list[0][0].jahr - jahr)
                               * -1;  // calculate actual year position in list, according to first year in the Weather-List and the random year
            cout << world_weather_list[0][0].jahr << endl;

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
                for (vector<vector<Evaluation>>::iterator posausw = world_evaluation_list.begin(); posausw != world_evaluation_list.end();
                     ++posausw) {  // world evaluation list begin
                    vector<Evaluation>& evaluation_list = *posausw;
                    vector<Evaluation>::iterator posauswakt = evaluation_list.begin();
                    auto& pEvaluation = (*posauswakt);

                    aktort++;

                    if (pEvaluation.basalarearunmeanlist.size() > 1) {
                        cout << "BA" << endl << pEvaluation.basalarearunmeanlist[pEvaluation.basalarearunmeanlist.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation.basalarearunmeanlist[pEvaluation.basalarearunmeanlist.size() - 2]
                                                   - pEvaluation.basalarearunmeanlist[pEvaluation.basalarearunmeanlist.size() - 1])
                                                  / (pEvaluation.basalarearunmeanlist[pEvaluation.basalarearunmeanlist.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                    // + N_0-40
                    if (pEvaluation.nheight0b40runmeanliste.size() > 1) {
                        cout << "N_0-40" << endl << pEvaluation.nheight0b40runmeanliste[pEvaluation.nheight0b40runmeanliste.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation.nheight0b40runmeanliste[pEvaluation.nheight0b40runmeanliste.size() - 2]
                                                   - pEvaluation.nheight0b40runmeanliste[pEvaluation.nheight0b40runmeanliste.size() - 1])
                                                  / (pEvaluation.nheight0b40runmeanliste[pEvaluation.nheight0b40runmeanliste.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                    // + N_40-200
                    if (pEvaluation.nheight41b200runmeanliste.size() > 1) {
                        cout << "N_40-200" << endl << pEvaluation.nheight41b200runmeanliste[pEvaluation.nheight41b200runmeanliste.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation.nheight41b200runmeanliste[pEvaluation.nheight41b200runmeanliste.size() - 2]
                                                   - pEvaluation.nheight41b200runmeanliste[pEvaluation.nheight41b200runmeanliste.size() - 1])
                                                  / (pEvaluation.nheight41b200runmeanliste[pEvaluation.nheight41b200runmeanliste.size() - 1]));
                    }
                    cout << "Cumulative %Change = " << meanpercentchange << endl;
                    // + N_200+
                    if (pEvaluation.nheight201b10000runmeanliste.size() > 1) {
                        cout << "N_200-10000" << endl << pEvaluation.nheight201b10000runmeanliste[pEvaluation.nheight201b10000runmeanliste.size() - 1] << endl;

                        meanpercentchange += fabs((pEvaluation.nheight201b10000runmeanliste[pEvaluation.nheight201b10000runmeanliste.size() - 2]
                                                   - pEvaluation.nheight201b10000runmeanliste[pEvaluation.nheight201b10000runmeanliste.size() - 1])
                                                  / (pEvaluation.nheight201b10000runmeanliste[pEvaluation.nheight201b10000runmeanliste.size() - 1]));
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
        yearposition = ((world_weather_list[0][0].jahr - parameter[0].startjahr) * -1)
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
        if (parameter[0].ivort > (parameter[0].ivortmax + parameter[0].stopatyear - (parameter[0].lastyearweatherdata - parameter[0].simduration))) {
            cout << "Simulation aborted as requested in parameter.txt->stopatyear!" << endl;
            exit(1);
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
            for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
                vector<Weather>& weather_list = *posw;
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

                    yearposition = ((world_weather_list[0][0].jahr - parameter[0].startjahr) * -1)
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

            // TODO use resize for the following
            world_weather_list.emplace_back();     // include new weather_list in corresponding world list
            world_plot_list.emplace_back();        // include new plot_list in corresponding world list
            world_evaluation_list.emplace_back();  // include new evaluation_list in corresponding world list

            if (parameter[0].resetyear > 0) {
                // Create lists for resetting to a certain year
                world_tree_list_copy.emplace_back(parameter[0].omp_num_threads);  // include new seed_list in corresponding world list

                world_seed_list_copy.emplace_back(parameter[0].omp_num_threads);  // include new seed_list in corresponding world list

                world_plot_list_copy.emplace_back();        // include new plot_list in corresponding world list
                world_evaluation_list_copy.emplace_back();  // include new evaluation_list in corresponding world list
            }
        }
    }
}

void fillElevations() {
    // 0. only once at initializing & if(parameter[0].mapylength==1)
    // 1. load elevation data on coars resolution (e.g. 30 m)
    // 2. go over each point and interpolate (weighted mean) within a buffer radius of 15 m (minimum 1 point falls in)
    // TODO: add fileinput to be set by parameter.txt

    if (parameter[0].mapylength == 1) {

	long int plotcodeNum;
	plotcodeNum = parameter[0].weatherchoice - 1000000000;
	
	std::stringstream plotcode;
	plotcode << std::setw(3) << std::setfill('0') << plotcodeNum;

	// ... read dem data
        FILE* f;
        char demfilename[250];
		
	if (parameter[0].weatherchoice > 1000000000 && parameter[0].weatherchoice < 2000000000){
		if (treerows == 50) {
			string deminputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_50x50/" + plotcode.str() + "_dem.csv";
			strcpy(demfilename, deminputbuf.c_str());
		} else if (treerows == 990) {
			string deminputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_990x990/" + plotcode.str() + "_dem.csv";
			strcpy(demfilename, deminputbuf.c_str());
		} else if (treerows == 100) {
			string deminputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_100x100/" + plotcode.str() + "_dem.csv";
			strcpy(demfilename, deminputbuf.c_str());
		}
	} else if (parameter[0].weatherchoice > 2000000000 && parameter[0].weatherchoice < 3000000000){
		if (treerows == 50) {
			string deminputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_50x50/" + plotcode.str() + "_dem.csv";
			strcpy(demfilename, deminputbuf.c_str());
		} else if (treerows == 990) {
			string deminputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_990x990/" + plotcode.str() + "_dem.csv";
			strcpy(demfilename, deminputbuf.c_str());
		} else if (treerows == 100) {
			string deminputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_100x100/" + plotcode.str() + "_dem.csv";
			strcpy(demfilename, deminputbuf.c_str());
		}
	} else if (parameter[0].weatherchoice == 50001) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Close_to_Fairbanks_EN23-611_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50002) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fielding_Lake_EN23-675_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50003) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fort_Mc_Pherson_East_EN22-021_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50004) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fort_Mc_Pherson_West_EN22-032_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50005) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Illerney_16KP01-V1_V20_Chukotka_2018_001-027_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50006) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Nutenvut_16KP03-V20_V39_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50007) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Ulu_EN21-201_219_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50008) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Mucho_Lake_EN22-065_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	} else if (parameter[0].weatherchoice == 50009) {
		char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Road_to_Central_EN23-651_dem.csv"; //x=990, y=990
		strcpy(demfilename, deminputbuf);
	}



		
        f = fopen(demfilename, "r");
        if (f == NULL) {
            printf("DEM file not available!\n");
            exit(1);
        }

        int deminputdimension_y = treerows / parameter[0].demresolution;  // matrix + 1 to avoid border effects
        int deminputdimension_x = treecols / parameter[0].demresolution;  // matrix + 1 to avoid border effects
        char puffer[6000];
        vector<double> elevationinput;
        elevationinput.resize(deminputdimension_y * deminputdimension_x, 0);
        int counter = -1;
        // read in line by line, and fill dem input vector (dimension e.g. 3x3 km each data point is pixel of 30 m resolution, so a 100x100 matrix with 10000
        // entries)
        while (fgets(puffer, 6000, f) != NULL) {
			char * allelements = strtok(puffer, " "); // separate into single tokens
			while(allelements != NULL) {
                counter++;  // rows
                elevationinput[counter] = strtod(allelements, NULL);
				allelements = strtok(NULL, " "); // set to next
			}
        }
        fclose(f);

    // ... read slope data
        char slopefilename[250];
		
	if (parameter[0].weatherchoice > 1000000000 && parameter[0].weatherchoice < 2000000000){
		if (treerows == 50) {
			string slopeinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_50x50/" + plotcode.str() + "_slope.csv";
			strcpy(slopefilename, slopeinputbuf.c_str());
		} else if (treerows == 990) {
			string slopeinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_990x990/" + plotcode.str() + "_slope.csv";
			strcpy(slopefilename, slopeinputbuf.c_str());
		} else if (treerows == 100) {
			string slopeinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_100x100/" + plotcode.str() + "_slope.csv";
			strcpy(slopefilename, slopeinputbuf.c_str());
		}
	} else if (parameter[0].weatherchoice > 2000000000 && parameter[0].weatherchoice < 3000000000){
		if (treerows == 50) {
			string slopeinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_50x50/" + plotcode.str() + "_slope.csv";
			strcpy(slopefilename, slopeinputbuf.c_str());
		} else if (treerows == 990) {
			string slopeinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_990x990/" + plotcode.str() + "_slope.csv";
			strcpy(slopefilename, slopeinputbuf.c_str());
		} else if (treerows == 100) {
			string slopeinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_100x100/" + plotcode.str() + "_slope.csv";
			strcpy(slopefilename, slopeinputbuf.c_str());
		}
	} else if (parameter[0].weatherchoice == 50001) {
	  char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Close_to_Fairbanks_EN23-611_slope.csv"; //x=990, y=990
	  strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50002) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fielding_Lake_EN23-675_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50003) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fort_Mc_Pherson_East_EN22-021_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50004) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fort_Mc_Pherson_West_EN22-032_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50005) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Illerney_16KP01-V1_V20_Chukotka_2018_001-027_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50006) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Nutenvut_16KP03-V20_V39_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50007) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Ulu_EN21-201_219_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50008) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Mucho_Lake_EN22-065_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	} else if (parameter[0].weatherchoice == 50009) {
		char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Road_to_Central_EN23-651_slope.csv"; //x=990, y=990
		strcpy(slopefilename, slopeinputbuf);
	}



        f = fopen(slopefilename, "r");
        if (f == NULL) {
            printf("Slope file not available!\n");
            exit(1);
        }

        vector<double> slopeinput;
        slopeinput.resize(deminputdimension_y * deminputdimension_x, 0);
        counter = -1;
        while (fgets(puffer, 6000, f) != NULL) {
			char * allelements = strtok(puffer, " "); // separate into single tokens
			while(allelements != NULL) {
                counter++;  // rows
                slopeinput[counter] = strtod(allelements, NULL);
				allelements = strtok(NULL, " "); // set to next
			}
        }
        fclose(f);

        // ... read twi data
        char twifilename[250];
		
	if (parameter[0].weatherchoice > 1000000000 && parameter[0].weatherchoice < 2000000000){
		if (treerows == 50) {
			string twiinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_50x50/" + plotcode.str() + "_twi.csv";
			strcpy(twifilename, twiinputbuf.c_str());
		} else if (treerows == 990) {
			string twiinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_990x990/" + plotcode.str() + "_twi.csv";
			strcpy(twifilename, twiinputbuf.c_str());
		} else if (treerows == 100) {
			string twiinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_100x100/" + plotcode.str() + "_twi.csv";
			strcpy(twifilename, twiinputbuf.c_str());
		}
	} else if (parameter[0].weatherchoice > 2000000000 && parameter[0].weatherchoice < 3000000000){
		if (treerows == 50) {
			string twiinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_50x50/" + plotcode.str() + "_twi.csv";
			strcpy(twifilename, twiinputbuf.c_str());
		} else if (treerows == 990) {
			string twiinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_990x990/" + plotcode.str() + "_twi.csv";
			strcpy(twifilename, twiinputbuf.c_str());
		} else if (treerows == 100) {
			string twiinputbuf = 
			"/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_100x100/" + plotcode.str() + "_twi.csv";
			strcpy(twifilename, twiinputbuf.c_str());
		}
	} else if (parameter[0].weatherchoice == 50001) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Close_to_Fairbanks_EN23-611_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50002) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fielding_Lake_EN23-675_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50003) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fort_Mc_Pherson_East_EN22-021_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50004) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Fort_Mc_Pherson_West_EN22-032_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50005) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Illerney_16KP01-V1_V20_Chukotka_2018_001-027_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50006) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Nutenvut_16KP03-V20_V39_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50007) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Lake_Ulu_EN21-201_219_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50008) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Mucho_Lake_EN22-065_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	} else if (parameter[0].weatherchoice == 50009) {
		char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data_mountains/Road_to_Central_EN23-651_twi.csv"; //x=990, y=990
		strcpy(twifilename, twiinputbuf);
	}

	

        f = fopen(twifilename, "r");
        if (f == NULL) {
            printf("TWI file not available!\n");
            exit(1);
        }

        vector<double> twiinput;
        twiinput.resize(deminputdimension_y * deminputdimension_x, 0);
        counter = -1;
        while (fgets(puffer, 6000, f) != NULL) {
			char * allelements = strtok(puffer, " "); // separate into single tokens
			while(allelements != NULL) {
                counter++;  // rows
                twiinput[counter] = strtod(allelements, NULL);
				allelements = strtok(NULL, " "); // set to next
			}
        }
        fclose(f);

        // interpolate to envirgrid
        for (vector<vector<Envirgrid>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++) {
            vector<Envirgrid>& plot_list = *posw;
#pragma omp parallel for default(shared) schedule(guided)
            for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int)treerows * (unsigned long long int)parameter[0].sizemagnif * (unsigned long long int)treecols * (unsigned long long int)parameter[0].sizemagnif);
                 kartenpos++) {
                // determine position and distances to gridpoints in low resolution grid
                double ycoo = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
                double xcoo = (double)kartenpos - ycoo * (treecols * parameter[0].sizemagnif);
                double ycoodem = ycoo / parameter[0].sizemagnif / parameter[0].demresolution;
                double xcoodem = xcoo / parameter[0].sizemagnif / parameter[0].demresolution;
                double ycoodemmod = ycoodem - floor(ycoodem);
                ycoodem = floor(ycoodem);
                double xcoodemmod = xcoodem - floor(xcoodem);
                xcoodem = floor(xcoodem);

                // elevation is filled with elevationoffset from parameters needs to sense elevation from input of 4 corners of grid cell
                if ((ycoodem < (deminputdimension_y - 1)) && (xcoodem < (deminputdimension_x - 1)))  // only if in range leaving out border
                {
                    double eleinter = (
                                          // upper left
                                          elevationinput[ycoodem * deminputdimension_x + xcoodem] * (1 - ycoodemmod)
                                          + elevationinput[ycoodem * deminputdimension_x + xcoodem] * (1 - xcoodemmod)
                                          // lower left
                                          + elevationinput[(ycoodem + 1) * deminputdimension_x + xcoodem] * (ycoodemmod)
                                          + elevationinput[(ycoodem + 1) * deminputdimension_x + xcoodem] * (1 - xcoodemmod)
                                          // upper right
                                          + elevationinput[ycoodem * deminputdimension_x + (xcoodem + 1)] * (1 - ycoodemmod)
                                          + elevationinput[ycoodem * deminputdimension_x + (xcoodem + 1)] * (xcoodemmod)
                                          // lower right
                                          + elevationinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] * (ycoodemmod)
                                          + elevationinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] * (xcoodemmod))
                                      / 4;
                    double slopeinter = (
                                            // upper left
                                            slopeinput[ycoodem * deminputdimension_x + xcoodem] * (1 - ycoodemmod)
                                            + slopeinput[ycoodem * deminputdimension_x + xcoodem] * (1 - xcoodemmod)
                                            // lower left
                                            + slopeinput[(ycoodem + 1) * deminputdimension_x + xcoodem] * (ycoodemmod)
                                            + slopeinput[(ycoodem + 1) * deminputdimension_x + xcoodem] * (1 - xcoodemmod)
                                            // upper right
                                            + slopeinput[ycoodem * deminputdimension_x + (xcoodem + 1)] * (1 - ycoodemmod)
                                            + slopeinput[ycoodem * deminputdimension_x + (xcoodem + 1)] * (xcoodemmod)
                                            // lower right
                                            + slopeinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] * (ycoodemmod)
                                            + slopeinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] * (xcoodemmod))
                                        / 4;
                    double twiinter = (
                                          // upper left
                                          twiinput[ycoodem * deminputdimension_x + xcoodem] * (1 - ycoodemmod)
                                          + twiinput[ycoodem * deminputdimension_x + xcoodem] * (1 - xcoodemmod)
                                          // lower left
                                          + twiinput[(ycoodem + 1) * deminputdimension_x + xcoodem] * (ycoodemmod)
                                          + twiinput[(ycoodem + 1) * deminputdimension_x + xcoodem] * (1 - xcoodemmod)
                                          // upper right
                                          + twiinput[ycoodem * deminputdimension_x + (xcoodem + 1)] * (1 - ycoodemmod)
                                          + twiinput[ycoodem * deminputdimension_x + (xcoodem + 1)] * (xcoodemmod)
                                          // lower right
                                          + twiinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] * (ycoodemmod)
                                          + twiinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] * (xcoodemmod))
                                      / 4;

                    int countwatercells = 0;
                    if ((elevationinput[ycoodem * deminputdimension_x + xcoodem] == 9999) || (slopeinput[ycoodem * deminputdimension_x + xcoodem] == 9999)
                        || (twiinput[ycoodem * deminputdimension_x + xcoodem] == 9999))
                        countwatercells++;
                    if ((elevationinput[(ycoodem + 1) * deminputdimension_x + xcoodem] == 9999)
                        || (slopeinput[(ycoodem + 1) * deminputdimension_x + xcoodem] == 9999)
                        || (twiinput[(ycoodem + 1) * deminputdimension_x + xcoodem] == 9999))
                        countwatercells++;
                    if ((elevationinput[ycoodem * deminputdimension_x + (xcoodem + 1)] == 9999)
                        || (slopeinput[ycoodem * deminputdimension_x + (xcoodem + 1)] == 9999)
                        || (twiinput[ycoodem * deminputdimension_x + (xcoodem + 1)] == 9999))
                        countwatercells++;
                    if ((elevationinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] == 9999)
                        || (slopeinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] == 9999)
                        || (twiinput[(ycoodem + 1) * deminputdimension_x + (xcoodem + 1)] == 9999))
                        countwatercells++;
                    // in case of water (or rock which would need to be implemented) are in the vicinity of the current envir grid cell the value will be set to
                    // 32767
                    if (countwatercells == 0) {
                        plot_list[kartenpos].elevation += 10 * eleinter;
                        // plot_list[kartenpos]->slope = slopeinter;
                        plot_list[kartenpos].twi += twiinter*100;

                        // calculate environment-growth-impact (value between 0 and 1)
                        // f(TWI)		= slope * TWI + intercept
                        // f(slope) 	= k * exp(-1/2 * (xp - mu)^2/sigma^2)
                        double envirgrowthimpact = parameter[0].slopetwiratio * (-0.045999 * twiinter + 0.994066)
                                                   + (1 - parameter[0].slopetwiratio)
                                                         * (0.85654 * exp((-0.5) * ((slopeinter - 8.78692) * (slopeinter - 8.78692)) / (6.90743 * 6.90743)));
						
						envirgrowthimpact = envirgrowthimpact + 0.3; //before: + 0.4
						
						// envirgrowthimpact = sqrt(envirgrowthimpact); //added to tune low envirgrowthimpact values at Khamra

						// relaxing the impact 
						// envirgrowthimpact = pow( envirgrowthimpact, 0.5*0.5); // double square root
						// envirgrowthimpact = pow( envirgrowthimpact, 0.5); // single square root

                        // plausibility check
                        if (envirgrowthimpact > 1.0)
                            envirgrowthimpact = 1.0;
                        if (envirgrowthimpact < 0.0)
                            envirgrowthimpact = 0.0;

                        // adjust by factor
                        double envirgrowthimpactfactor = 1.0;
                        plot_list[kartenpos].envirgrowthimpact = 10000 * envirgrowthimpactfactor * envirgrowthimpact;
						
						
                        plot_list[kartenpos].envirfireimpact = 1-(twiinter/25) * 10000;// scale: 10000 meaning value ==1 ... 10000 == very dry places
                    } else {
                        plot_list[kartenpos].elevation = 32767;
                        plot_list[kartenpos].envirgrowthimpact = 0;
                        plot_list[kartenpos].twi = 25*100;
                    }
                } else {
                    plot_list[kartenpos].elevation = 32767;
                    plot_list[kartenpos].envirgrowthimpact = 0;
                    plot_list[kartenpos].twi = 25*100;
                }
            }
        }
    }
    cout << " ... end dem and slope input" << endl;
}

void initialiseMaps() {
    cout << " -> started initialise Maps " << endl;
    int aktort = 0;
    for (vector<vector<Envirgrid>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); posw++) {
        vector<Envirgrid>& plot_list = *posw;
        vector<vector<Evaluation>>::iterator posiwelt = (world_evaluation_list.begin() + aktort);
        vector<Evaluation>& evaluation_list = *posiwelt;
        aktort++;
        // calculation of a different position in coordinates:
        //		xworld= repeating the same position
        //		yworld= different position along the transect
        // necessary for the global lists
        // int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        short int initialelevation = 0;
        if (parameter[0].mapylength == 1)
            initialelevation = 10 * parameter[0].elevationoffset;

        auto time_start = chrono::high_resolution_clock::now();
        plot_list.resize(((unsigned long long int)treerows * (unsigned long long int)parameter[0].sizemagnif * (unsigned long long int)treecols
                          * (unsigned long long int)parameter[0].sizemagnif),
						  
						 // Here, initial grid values are prepared. For the values check structures.h L100 following!  
                         // {initialelevation, 0, 0, 1000, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1, 0, 0}); // ###Fire version from before merging### Last digit resembles initial fire counter, put to "false" for bool variant
                         {initialelevation, 0, 0, 100*10, 10*100, 10*100, 10*100, 10*100, 10*100, 10*100, 10*100, 10*100, 10*100, 10*100, 10*100, 1, 30*100, (unsigned short int)6.25*100, 0, 0, 0, 0, 0}); //new version after merging; added last three 0s for fire
						 
        auto time_end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed;
        elapsed = time_end - time_start;
        cout << "Elapsed time: " << elapsed.count() << " s\n";

        // create an evaluation element for each site
        Evaluation pEvaluation;
        // pEvaluation.yworldcoo = aktortyworldcoo;
        // pEvaluation.xworldcoo = aktortxworldcoo;
        pEvaluation.basalarealist.clear();
        pEvaluation.basalarealist.shrink_to_fit();
        pEvaluation.basalarearunmeanlist.clear();
        pEvaluation.basalarearunmeanlist.shrink_to_fit();
        // growth stage calculation
        pEvaluation.maxincrementbasalarea = 0.0;
        pEvaluation.countermaxincrementbasalarea = 0;
        pEvaluation.yearofturningpoint = -9999;
        pEvaluation.yearofequilibrium = -9999;
        pEvaluation.postyearofturningpoint = false;
        // general values
        pEvaluation.nheight0b40liste.clear();
        pEvaluation.nheight0b40liste.shrink_to_fit();
        pEvaluation.nheight0b40runmeanliste.clear();
        pEvaluation.nheight0b40runmeanliste.shrink_to_fit();
        pEvaluation.nheight41b200liste.clear();
        pEvaluation.nheight41b200liste.shrink_to_fit();
        pEvaluation.nheight41b200runmeanliste.clear();
        pEvaluation.nheight41b200runmeanliste.shrink_to_fit();
        pEvaluation.nheight201b10000liste.clear();
        pEvaluation.nheight201b10000liste.shrink_to_fit();
        pEvaluation.nheight201b10000runmeanliste.clear();
        pEvaluation.nheight201b10000runmeanliste.shrink_to_fit();
        pEvaluation.meanbreastdiameterliste.clear();
        pEvaluation.meanbreastdiameterliste.shrink_to_fit();
        pEvaluation.meanbreastdiameterrunmeanliste.clear();
        pEvaluation.meanbreastdiameterrunmeanliste.shrink_to_fit();
        pEvaluation.stemcountliste.clear();
        pEvaluation.stemcountliste.shrink_to_fit();
        pEvaluation.stemcountrunmeanliste.clear();
        pEvaluation.stemcountrunmeanliste.shrink_to_fit();
        pEvaluation.meantreeheightliste.clear();
        pEvaluation.meantreeheightliste.shrink_to_fit();
        pEvaluation.meantreeheightrunmeanliste.clear();
        pEvaluation.meantreeheightrunmeanliste.shrink_to_fit();
        pEvaluation.meantreeageliste.clear();
        pEvaluation.meantreeageliste.shrink_to_fit();
        pEvaluation.meantreeagerunmeanliste.clear();
        pEvaluation.meantreeagerunmeanliste.shrink_to_fit();

        evaluation_list.emplace_back(std::move(pEvaluation));
    }
    cout << " ... ... ended initialise Maps " << endl;
}

void runSimulation() {
    createLists();

    Weatherinput(&parameter[0], stringlengthmax, world_weather_list);

    // plot and evaluation list preparation for each location on the transect
    initialiseMaps();

    // compute dem for each envir grid tile from read in data
    if (parameter[0].demlandscape)
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
    // console output of the version and general information
    printf("\n---->\tLAVESI\n");
    printf(
        "\n You have started  LAVESI-FIRE, "
        "An individual-based and spatially explicit simulation model for vegetation dynamics of boreal forests and wildfires in a 3-dimensional landscape "
        "- driven by temperature, precipitation and wind data."
        "\n\n Version:\t 2.01 (LAVESI-WIND-3DENVIR-MULTIPLESPECIES-FIRE)"
        "\n Date:\t\t 14.04.2021"
        "\n authors:"
        "\n\t Stefan Kruse\tstefan.kruse@awi.de"
        "\n\t Josias Gloy"
        "\n\t of prior versions:"
        "\n\t Alexander Gerdes, Nadja Kath, Mareike Wieczorek"
        "\n");
    printf("\n<----\n");

    // read in all simulation parameters from parameters.txt
    Parameterinput();
	
	Getspeciestraits();
	// print the present species in this simulation and find maximum possible species
	cout << "Species present in simulation:" << endl;
	for (int species_counter = 1; species_counter < 99; species_counter++) {
		if(speciestrait[species_counter].number == 0) {
			parameter[0].species_max = species_counter;
			break;
		}

		cout << speciestrait[species_counter].number << " -> " << speciestrait[species_counter].species << " | min ALD: " << speciestrait[species_counter].minactivelayer << " | min biomasswoodfacb: " << speciestrait[species_counter].biomasswoodfacb << endl;
	}

    // pragma omp initializing
    omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers

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