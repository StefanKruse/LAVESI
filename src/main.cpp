#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

Parameter parameter[1];
Speciestraits speciestrait[99];

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

        // ... read dem data
        FILE* f;
        char demfilename[250];
		if (parameter[0].weatherchoice == 1000000001) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/001_dem.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000002) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/002_dem.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000003) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/003_dem.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000004) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/004_dem.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000005) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/005_dem.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000006) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/006_dem.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000007) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/007_dem.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000008) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/008_dem.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000009) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/009_dem.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000010) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/010_dem.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000011) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/011_dem.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000012) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/012_dem.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000013) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/013_dem.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000014) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/014_dem.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000015) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/015_dem.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000016) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/016_dem.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000017) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/017_dem.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000018) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/018_dem.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000019) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/019_dem.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000020) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/020_dem.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000021) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/021_dem.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000022) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/022_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000023) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/023_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000024) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/024_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000025) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/025_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000026) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/026_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000027) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/027_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000028) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/028_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000029) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/029_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000030) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/030_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000031) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/031_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000032) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/032_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000033) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/033_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000034) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/034_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000035) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/035_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000036) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/036_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000037) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/037_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000038) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/038_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000039) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/039_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000040) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/040_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000041) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/041_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000042) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/042_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000043) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/043_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000044) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/044_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000045) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/045_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000046) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/046_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000047) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/047_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000048) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/048_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000049) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/049_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000050) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/050_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000051) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/051_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000052) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/052_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000053) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/053_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000054) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/054_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000055) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/055_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000056) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/056_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000057) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/057_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000058) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/058_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000059) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/059_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000060) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/060_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000061) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/061_dem.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000062) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/062_dem.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000063) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/063_dem.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000064) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/064_dem.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000065) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/065_dem.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000066) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/066_dem.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000067) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/067_dem.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000068) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/068_dem.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000069) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/069_dem.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000070) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/070_dem.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000071) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/071_dem.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000072) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/072_dem.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000073) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/073_dem.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000074) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/074_dem.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000075) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/075_dem.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000076) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/076_dem.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000077) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/077_dem.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000078) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/078_dem.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000079) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/079_dem.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000080) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/080_dem.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000081) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/081_dem.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000082) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/082_dem.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000083) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/083_dem.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000084) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/084_dem.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000085) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/085_dem.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000086) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/086_dem.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000087) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/087_dem.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000088) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/088_dem.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000089) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/089_dem.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000090) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/090_dem.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000091) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/091_dem.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000092) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/092_dem.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000093) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/093_dem.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000094) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/094_dem.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000095) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/095_dem.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000096) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/096_dem.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000097) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/097_dem.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000098) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/098_dem.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000099) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/099_dem.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000100) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/100_dem.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000101) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/101_dem.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000102) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/102_dem.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000103) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/103_dem.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000104) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/104_dem.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000105) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/105_dem.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000106) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/106_dem.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000107) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/107_dem.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000108) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/108_dem.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000109) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/109_dem.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000110) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/110_dem.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000111) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/111_dem.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000112) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/112_dem.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000113) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/113_dem.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000114) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/114_dem.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000115) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/115_dem.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000116) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/116_dem.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000117) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/117_dem.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000118) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/118_dem.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000119) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/119_dem.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000120) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/120_dem.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000121) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/121_dem.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000122) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/122_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000123) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/123_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000124) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/124_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000125) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/125_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000126) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/126_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000127) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/127_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000128) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/128_dem.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000129) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/129_dem.csv"; //x=990, y=990 ### FOR B19-T1 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000130) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/130_dem.csv"; //x=990, y=990 ### FOR B19-T2 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000131) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/131_dem.csv"; //x=990, y=990 ### FOR EN18061 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000132) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/132_dem.csv"; //x=990, y=990 ### FOR EN18062 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000133) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/133_dem.csv"; //x=990, y=990 ### FOR EN18063 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000134) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/134_dem.csv"; //x=990, y=990 ### FOR EN18064 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000135) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/135_dem.csv"; //x=990, y=990 ### FOR EN18065 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000136) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/136_dem.csv"; //x=990, y=990 ### FOR EN18066 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000137) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/137_dem.csv"; //x=990, y=990 ### FOR EN18067 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000138) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/138_dem.csv"; //x=990, y=990 ### FOR EN18068 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000139) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/139_dem.csv"; //x=990, y=990 ### FOR EN18069 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000140) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/140_dem.csv"; //x=990, y=990 ### FOR EN18070_centre ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000141) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/141_dem.csv"; //x=990, y=990 ### FOR EN18070_edge ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000142) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/142_dem.csv"; //x=990, y=990 ### FOR EN18070_end ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000143) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/143_dem.csv"; //x=990, y=990 ### FOR EN18070_transition ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000144) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/144_dem.csv"; //x=990, y=990 ### FOR EN18071 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000145) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/145_dem.csv"; //x=990, y=990 ### FOR EN18072 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000146) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/146_dem.csv"; //x=990, y=990 ### FOR EN18073 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000147) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/147_dem.csv"; //x=990, y=990 ### FOR EN18074 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000148) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/148_dem.csv"; //x=990, y=990 ### FOR EN18075 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000149) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/149_dem.csv"; //x=990, y=990 ### FOR EN18076 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000150) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/150_dem.csv"; //x=990, y=990 ### FOR EN18077 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000151) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/151_dem.csv"; //x=990, y=990 ### FOR EN18078 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000152) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/152_dem.csv"; //x=990, y=990 ### FOR EN18079 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000153) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/153_dem.csv"; //x=990, y=990 ### FOR EN18080 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000154) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/154_dem.csv"; //x=990, y=990 ### FOR EN18081 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000155) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/155_dem.csv"; //x=990, y=990 ### FOR EN18082 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000156) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/156_dem.csv"; //x=990, y=990 ### FOR EN18083 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000157) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/157_dem.csv"; //x=990, y=990 ### FOR EN21201 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000158) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/158_dem.csv"; //x=990, y=990 ### FOR EN21202 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000159) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/159_dem.csv"; //x=990, y=990 ### FOR EN21203 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000160) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/160_dem.csv"; //x=990, y=990 ### FOR EN21204 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000161) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/161_dem.csv"; //x=990, y=990 ### FOR EN21205 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000162) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/162_dem.csv"; //x=990, y=990 ### FOR EN21206 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000163) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/163_dem.csv"; //x=990, y=990 ### FOR EN21207 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000164) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/164_dem.csv"; //x=990, y=990 ### FOR EN21208 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000165) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/165_dem.csv"; //x=990, y=990 ### FOR EN21209 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000166) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/166_dem.csv"; //x=990, y=990 ### FOR EN21210 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000167) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/167_dem.csv"; //x=990, y=990 ### FOR EN21211 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000168) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/168_dem.csv"; //x=990, y=990 ### FOR EN21212 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000169) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/169_dem.csv"; //x=990, y=990 ### FOR EN21213 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000170) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/170_dem.csv"; //x=990, y=990 ### FOR EN21214 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000171) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/171_dem.csv"; //x=990, y=990 ### FOR EN21215 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000172) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/172_dem.csv"; //x=990, y=990 ### FOR EN21216 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000173) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/173_dem.csv"; //x=990, y=990 ### FOR EN21217 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000174) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/174_dem.csv"; //x=990, y=990 ### FOR EN21218 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000175) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/175_dem.csv"; //x=990, y=990 ### FOR EN21219 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000176) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/176_dem.csv"; //x=990, y=990 ### FOR EN21220 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000177) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/177_dem.csv"; //x=990, y=990 ### FOR EN21221 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000178) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/178_dem.csv"; //x=990, y=990 ### FOR EN21222 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000179) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/179_dem.csv"; //x=990, y=990 ### FOR EN21223 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000180) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/180_dem.csv"; //x=990, y=990 ### FOR EN21224 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000181) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/181_dem.csv"; //x=990, y=990 ### FOR EN21225 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000182) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/182_dem.csv"; //x=990, y=990 ### FOR EN21226 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000183) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/183_dem.csv"; //x=990, y=990 ### FOR EN21227 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000184) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/184_dem.csv"; //x=990, y=990 ### FOR EN21228 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000185) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/185_dem.csv"; //x=990, y=990 ### FOR EN21229 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000186) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/186_dem.csv"; //x=990, y=990 ### FOR EN21230 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000187) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/187_dem.csv"; //x=990, y=990 ### FOR EN21231 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000188) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/188_dem.csv"; //x=990, y=990 ### FOR EN21232 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000189) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/189_dem.csv"; //x=990, y=990 ### FOR EN21233 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000190) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/190_dem.csv"; //x=990, y=990 ### FOR EN21234 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000191) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/191_dem.csv"; //x=990, y=990 ### FOR EN21235 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000192) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/192_dem.csv"; //x=990, y=990 ### FOR EN21236 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000193) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/193_dem.csv"; //x=990, y=990 ### FOR EN21237 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000194) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/194_dem.csv"; //x=990, y=990 ### FOR EN21238 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000195) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/195_dem.csv"; //x=990, y=990 ### FOR EN21239 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000196) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/196_dem.csv"; //x=990, y=990 ### FOR EN21240 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000197) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/197_dem.csv"; //x=990, y=990 ### FOR EN21241 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000198) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/198_dem.csv"; //x=990, y=990 ### FOR EN21242 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000199) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/199_dem.csv"; //x=990, y=990 ### FOR EN21243 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000200) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/200_dem.csv"; //x=990, y=990 ### FOR EN21244 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000201) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/201_dem.csv"; //x=990, y=990 ### FOR EN21245 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000202) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/202_dem.csv"; //x=990, y=990 ### FOR EN21246 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000203) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/203_dem.csv"; //x=990, y=990 ### FOR EN21247 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000204) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/204_dem.csv"; //x=990, y=990 ### FOR EN21248 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000205) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/205_dem.csv"; //x=990, y=990 ### FOR EN21249 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000206) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/206_dem.csv"; //x=990, y=990 ### FOR EN21250 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000207) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/207_dem.csv"; //x=990, y=990 ### FOR EN21251 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000208) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/208_dem.csv"; //x=990, y=990 ### FOR EN21252 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000209) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/209_dem.csv"; //x=990, y=990 ### FOR EN21253 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000210) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/210_dem.csv"; //x=990, y=990 ### FOR EN21254 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000211) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/211_dem.csv"; //x=990, y=990 ### FOR EN21255 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000212) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/212_dem.csv"; //x=990, y=990 ### FOR EN21256 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000213) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/213_dem.csv"; //x=990, y=990 ### FOR EN21257 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000214) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/214_dem.csv"; //x=990, y=990 ### FOR EN21258 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000215) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/215_dem.csv"; //x=990, y=990 ### FOR EN21259 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000216) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/216_dem.csv"; //x=990, y=990 ### FOR EN21260 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000217) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/217_dem.csv"; //x=990, y=990 ### FOR EN21261 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000218) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/218_dem.csv"; //x=990, y=990 ### FOR EN21262 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000219) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/219_dem.csv"; //x=990, y=990 ### FOR EN22-001 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000220) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/220_dem.csv"; //x=990, y=990 ### FOR EN22-002 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000221) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/221_dem.csv"; //x=990, y=990 ### FOR EN22-003 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000222) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/222_dem.csv"; //x=990, y=990 ### FOR EN22-004 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000223) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/223_dem.csv"; //x=990, y=990 ### FOR EN22-005 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000224) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/224_dem.csv"; //x=990, y=990 ### FOR EN22-006 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000225) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/225_dem.csv"; //x=990, y=990 ### FOR EN22-007 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000226) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/226_dem.csv"; //x=990, y=990 ### FOR EN22-008 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000227) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/227_dem.csv"; //x=990, y=990 ### FOR EN22-009 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000228) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/228_dem.csv"; //x=990, y=990 ### FOR EN22-010 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000229) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/229_dem.csv"; //x=990, y=990 ### FOR EN22-011 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000230) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/230_dem.csv"; //x=990, y=990 ### FOR EN22-012 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000231) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/231_dem.csv"; //x=990, y=990 ### FOR EN22-013 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000232) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/232_dem.csv"; //x=990, y=990 ### FOR EN22-014 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000233) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/233_dem.csv"; //x=990, y=990 ### FOR EN22-015 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000234) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/234_dem.csv"; //x=990, y=990 ### FOR EN22-016 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000235) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/235_dem.csv"; //x=990, y=990 ### FOR EN22-017 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000236) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/236_dem.csv"; //x=990, y=990 ### FOR EN22-018 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000237) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/237_dem.csv"; //x=990, y=990 ### FOR EN22-019 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000238) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/238_dem.csv"; //x=990, y=990 ### FOR EN22-020 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000239) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/239_dem.csv"; //x=990, y=990 ### FOR EN22-021 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000240) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/240_dem.csv"; //x=990, y=990 ### FOR EN22-022 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000241) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/241_dem.csv"; //x=990, y=990 ### FOR EN22-023 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000242) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/242_dem.csv"; //x=990, y=990 ### FOR EN22-024 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000243) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/243_dem.csv"; //x=990, y=990 ### FOR EN22-025 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000244) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/244_dem.csv"; //x=990, y=990 ### FOR EN22-026 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000245) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/245_dem.csv"; //x=990, y=990 ### FOR EN22-027 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000246) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/246_dem.csv"; //x=990, y=990 ### FOR EN22-028 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000247) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/247_dem.csv"; //x=990, y=990 ### FOR EN22-029 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000248) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/248_dem.csv"; //x=990, y=990 ### FOR EN22-030 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000249) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/249_dem.csv"; //x=990, y=990 ### FOR EN22-031 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000250) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/250_dem.csv"; //x=990, y=990 ### FOR EN22-032 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000251) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/251_dem.csv"; //x=990, y=990 ### FOR EN22-033 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000252) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/252_dem.csv"; //x=990, y=990 ### FOR EN22-034 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000253) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/253_dem.csv"; //x=990, y=990 ### FOR EN22-035 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000254) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/254_dem.csv"; //x=990, y=990 ### FOR EN22-036 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000255) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/255_dem.csv"; //x=990, y=990 ### FOR EN22-037 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000256) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/256_dem.csv"; //x=990, y=990 ### FOR EN22-038 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000257) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/257_dem.csv"; //x=990, y=990 ### FOR EN22-039 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000258) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/258_dem.csv"; //x=990, y=990 ### FOR EN22-040 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000259) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/259_dem.csv"; //x=990, y=990 ### FOR EN22-041 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000260) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/260_dem.csv"; //x=990, y=990 ### FOR EN22-042 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000261) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/261_dem.csv"; //x=990, y=990 ### FOR EN22-043 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000262) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/262_dem.csv"; //x=990, y=990 ### FOR EN22-044 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000263) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/263_dem.csv"; //x=990, y=990 ### FOR EN22-045 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000264) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/264_dem.csv"; //x=990, y=990 ### FOR EN22-046 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000265) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/265_dem.csv"; //x=990, y=990 ### FOR EN22-047 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000266) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/266_dem.csv"; //x=990, y=990 ### FOR EN22-048 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000267) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/267_dem.csv"; //x=990, y=990 ### FOR EN22-049 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000268) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/268_dem.csv"; //x=990, y=990 ### FOR EN22-050 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000269) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/269_dem.csv"; //x=990, y=990 ### FOR EN22-051 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000270) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/270_dem.csv"; //x=990, y=990 ### FOR EN22-052 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000271) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/271_dem.csv"; //x=990, y=990 ### FOR EN22-053 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000272) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/272_dem.csv"; //x=990, y=990 ### FOR EN22-054 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000273) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/273_dem.csv"; //x=990, y=990 ### FOR EN22-055 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000274) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/274_dem.csv"; //x=990, y=990 ### FOR EN22-056 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000275) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/275_dem.csv"; //x=990, y=990 ### FOR EN22-057 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000276) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/276_dem.csv"; //x=990, y=990 ### FOR EN22-058 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000277) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/277_dem.csv"; //x=990, y=990 ### FOR EN22-059 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000278) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/278_dem.csv"; //x=990, y=990 ### FOR EN22-060 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000279) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/279_dem.csv"; //x=990, y=990 ### FOR EN22-061 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000280) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/280_dem.csv"; //x=990, y=990 ### FOR EN22-062 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000281) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/281_dem.csv"; //x=990, y=990 ### FOR EN22-063 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000282) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/282_dem.csv"; //x=990, y=990 ### FOR EN22-064 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000283) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/283_dem.csv"; //x=990, y=990 ### FOR EN22-065 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000284) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/284_dem.csv"; //x=990, y=990 ### FOR EN22-066 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000285) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/285_dem.csv"; //x=990, y=990 ### FOR EN22-067 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000286) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/286_dem.csv"; //x=990, y=990 ### FOR EN22-068 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000287) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/287_dem.csv"; //x=990, y=990 ### FOR EN22-069 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000288) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/288_dem.csv"; //x=990, y=990 ### FOR EN22-070 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000289) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/289_dem.csv"; //x=990, y=990 ### FOR EN22-071 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000290) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/290_dem.csv"; //x=990, y=990 ### FOR EN22-072 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000291) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/291_dem.csv"; //x=990, y=990 ### FOR EN22-073 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000292) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/292_dem.csv"; //x=990, y=990 ### FOR EN22-074 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000293) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/293_dem.csv"; //x=990, y=990 ### FOR EN22-075 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000294) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/294_dem.csv"; //x=990, y=990 ### FOR TY04VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000295) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/295_dem.csv"; //x=990, y=990 ### FOR TY04VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000296) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/296_dem.csv"; //x=990, y=990 ### FOR TY07VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000297) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/297_dem.csv"; //x=990, y=990 ### FOR TY07VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000298) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/298_dem.csv"; //x=990, y=990 ### FOR TY09VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 1000000299) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/299_dem.csv"; //x=990, y=990 ### FOR TY09VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000001) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/001_dem.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000002) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/002_dem.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000003) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/003_dem.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000004) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/004_dem.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000005) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/005_dem.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000006) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/006_dem.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000007) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/007_dem.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000008) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/008_dem.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000009) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/009_dem.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000010) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/010_dem.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000011) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/011_dem.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000012) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/012_dem.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000013) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/013_dem.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000014) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/014_dem.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000015) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/015_dem.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000016) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/016_dem.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000017) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/017_dem.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000018) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/018_dem.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000019) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/019_dem.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000020) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/020_dem.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000021) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/021_dem.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000022) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/022_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000023) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/023_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000024) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/024_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000025) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/025_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000026) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/026_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000027) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/027_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000028) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/028_dem.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000029) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/029_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000030) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/030_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000031) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/031_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000032) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/032_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000033) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/033_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000034) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/034_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000035) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/035_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000036) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/036_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000037) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/037_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000038) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/038_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000039) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/039_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000040) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/040_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000041) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/041_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000042) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/042_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000043) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/043_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000044) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/044_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000045) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/045_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000046) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/046_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000047) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/047_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000048) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/048_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000049) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/049_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000050) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/050_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000051) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/051_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000052) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/052_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000053) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/053_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000054) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/054_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000055) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/055_dem.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000056) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/056_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000057) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/057_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000058) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/058_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000059) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/059_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000060) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/060_dem.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000061) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/061_dem.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000062) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/062_dem.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000063) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/063_dem.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000064) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/064_dem.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000065) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/065_dem.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000066) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/066_dem.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000067) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/067_dem.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000068) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/068_dem.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000069) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/069_dem.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000070) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/070_dem.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000071) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/071_dem.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000072) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/072_dem.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000073) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/073_dem.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000074) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/074_dem.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000075) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/075_dem.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000076) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/076_dem.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000077) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/077_dem.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000078) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/078_dem.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000079) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/079_dem.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000080) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/080_dem.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000081) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/081_dem.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000082) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/082_dem.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000083) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/083_dem.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000084) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/084_dem.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000085) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/085_dem.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000086) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/086_dem.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000087) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/087_dem.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000088) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/088_dem.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000089) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/089_dem.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000090) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/090_dem.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000091) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/091_dem.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000092) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/092_dem.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000093) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/093_dem.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000094) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/094_dem.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000095) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/095_dem.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000096) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/096_dem.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000097) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/097_dem.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000098) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/098_dem.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000099) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/099_dem.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000100) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/100_dem.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000101) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/101_dem.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000102) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/102_dem.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000103) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/103_dem.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000104) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/104_dem.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000105) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/105_dem.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000106) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/106_dem.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000107) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/107_dem.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000108) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/108_dem.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000109) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/109_dem.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000110) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/110_dem.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000111) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/111_dem.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000112) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/112_dem.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000113) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/113_dem.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000114) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/114_dem.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000115) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/115_dem.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000116) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/116_dem.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000117) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/117_dem.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000118) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/118_dem.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000119) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/119_dem.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000120) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/120_dem.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000121) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/121_dem.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000122) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/122_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000123) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/123_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000124) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/124_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000125) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/125_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000126) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/126_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000127) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/127_dem.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000128) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/128_dem.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000129) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/129_dem.csv"; //x=990, y=990 ### FOR B19-T1 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000130) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/130_dem.csv"; //x=990, y=990 ### FOR B19-T2 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000131) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/131_dem.csv"; //x=990, y=990 ### FOR EN18061 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000132) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/132_dem.csv"; //x=990, y=990 ### FOR EN18062 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000133) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/133_dem.csv"; //x=990, y=990 ### FOR EN18063 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000134) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/134_dem.csv"; //x=990, y=990 ### FOR EN18064 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000135) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/135_dem.csv"; //x=990, y=990 ### FOR EN18065 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000136) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/136_dem.csv"; //x=990, y=990 ### FOR EN18066 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000137) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/137_dem.csv"; //x=990, y=990 ### FOR EN18067 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000138) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/138_dem.csv"; //x=990, y=990 ### FOR EN18068 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000139) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/139_dem.csv"; //x=990, y=990 ### FOR EN18069 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000140) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/140_dem.csv"; //x=990, y=990 ### FOR EN18070_centre ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000141) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/141_dem.csv"; //x=990, y=990 ### FOR EN18070_edge ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000142) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/142_dem.csv"; //x=990, y=990 ### FOR EN18070_end ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000143) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/143_dem.csv"; //x=990, y=990 ### FOR EN18070_transition ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000144) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/144_dem.csv"; //x=990, y=990 ### FOR EN18071 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000145) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/145_dem.csv"; //x=990, y=990 ### FOR EN18072 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000146) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/146_dem.csv"; //x=990, y=990 ### FOR EN18073 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000147) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/147_dem.csv"; //x=990, y=990 ### FOR EN18074 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000148) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/148_dem.csv"; //x=990, y=990 ### FOR EN18075 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000149) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/149_dem.csv"; //x=990, y=990 ### FOR EN18076 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000150) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/150_dem.csv"; //x=990, y=990 ### FOR EN18077 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000151) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/151_dem.csv"; //x=990, y=990 ### FOR EN18078 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000152) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/152_dem.csv"; //x=990, y=990 ### FOR EN18079 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000153) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/153_dem.csv"; //x=990, y=990 ### FOR EN18080 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000154) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/154_dem.csv"; //x=990, y=990 ### FOR EN18081 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000155) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/155_dem.csv"; //x=990, y=990 ### FOR EN18082 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000156) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/156_dem.csv"; //x=990, y=990 ### FOR EN18083 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000157) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/157_dem.csv"; //x=990, y=990 ### FOR EN21201 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000158) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/158_dem.csv"; //x=990, y=990 ### FOR EN21202 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000159) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/159_dem.csv"; //x=990, y=990 ### FOR EN21203 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000160) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/160_dem.csv"; //x=990, y=990 ### FOR EN21204 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000161) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/161_dem.csv"; //x=990, y=990 ### FOR EN21205 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000162) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/162_dem.csv"; //x=990, y=990 ### FOR EN21206 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000163) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/163_dem.csv"; //x=990, y=990 ### FOR EN21207 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000164) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/164_dem.csv"; //x=990, y=990 ### FOR EN21208 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000165) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/165_dem.csv"; //x=990, y=990 ### FOR EN21209 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000166) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/166_dem.csv"; //x=990, y=990 ### FOR EN21210 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000167) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/167_dem.csv"; //x=990, y=990 ### FOR EN21211 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000168) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/168_dem.csv"; //x=990, y=990 ### FOR EN21212 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000169) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/169_dem.csv"; //x=990, y=990 ### FOR EN21213 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000170) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/170_dem.csv"; //x=990, y=990 ### FOR EN21214 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000171) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/171_dem.csv"; //x=990, y=990 ### FOR EN21215 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000172) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/172_dem.csv"; //x=990, y=990 ### FOR EN21216 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000173) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/173_dem.csv"; //x=990, y=990 ### FOR EN21217 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000174) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/174_dem.csv"; //x=990, y=990 ### FOR EN21218 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000175) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/175_dem.csv"; //x=990, y=990 ### FOR EN21219 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000176) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/176_dem.csv"; //x=990, y=990 ### FOR EN21220 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000177) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/177_dem.csv"; //x=990, y=990 ### FOR EN21221 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000178) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/178_dem.csv"; //x=990, y=990 ### FOR EN21222 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000179) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/179_dem.csv"; //x=990, y=990 ### FOR EN21223 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000180) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/180_dem.csv"; //x=990, y=990 ### FOR EN21224 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000181) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/181_dem.csv"; //x=990, y=990 ### FOR EN21225 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000182) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/182_dem.csv"; //x=990, y=990 ### FOR EN21226 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000183) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/183_dem.csv"; //x=990, y=990 ### FOR EN21227 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000184) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/184_dem.csv"; //x=990, y=990 ### FOR EN21228 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000185) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/185_dem.csv"; //x=990, y=990 ### FOR EN21229 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000186) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/186_dem.csv"; //x=990, y=990 ### FOR EN21230 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000187) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/187_dem.csv"; //x=990, y=990 ### FOR EN21231 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000188) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/188_dem.csv"; //x=990, y=990 ### FOR EN21232 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000189) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/189_dem.csv"; //x=990, y=990 ### FOR EN21233 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000190) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/190_dem.csv"; //x=990, y=990 ### FOR EN21234 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000191) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/191_dem.csv"; //x=990, y=990 ### FOR EN21235 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000192) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/192_dem.csv"; //x=990, y=990 ### FOR EN21236 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000193) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/193_dem.csv"; //x=990, y=990 ### FOR EN21237 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000194) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/194_dem.csv"; //x=990, y=990 ### FOR EN21238 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000195) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/195_dem.csv"; //x=990, y=990 ### FOR EN21239 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000196) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/196_dem.csv"; //x=990, y=990 ### FOR EN21240 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000197) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/197_dem.csv"; //x=990, y=990 ### FOR EN21241 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000198) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/198_dem.csv"; //x=990, y=990 ### FOR EN21242 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000199) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/199_dem.csv"; //x=990, y=990 ### FOR EN21243 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000200) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/200_dem.csv"; //x=990, y=990 ### FOR EN21244 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000201) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/201_dem.csv"; //x=990, y=990 ### FOR EN21245 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000202) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/202_dem.csv"; //x=990, y=990 ### FOR EN21246 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000203) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/203_dem.csv"; //x=990, y=990 ### FOR EN21247 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000204) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/204_dem.csv"; //x=990, y=990 ### FOR EN21248 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000205) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/205_dem.csv"; //x=990, y=990 ### FOR EN21249 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000206) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/206_dem.csv"; //x=990, y=990 ### FOR EN21250 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000207) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/207_dem.csv"; //x=990, y=990 ### FOR EN21251 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000208) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/208_dem.csv"; //x=990, y=990 ### FOR EN21252 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000209) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/209_dem.csv"; //x=990, y=990 ### FOR EN21253 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000210) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/210_dem.csv"; //x=990, y=990 ### FOR EN21254 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000211) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/211_dem.csv"; //x=990, y=990 ### FOR EN21255 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000212) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/212_dem.csv"; //x=990, y=990 ### FOR EN21256 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000213) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/213_dem.csv"; //x=990, y=990 ### FOR EN21257 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000214) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/214_dem.csv"; //x=990, y=990 ### FOR EN21258 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000215) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/215_dem.csv"; //x=990, y=990 ### FOR EN21259 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000216) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/216_dem.csv"; //x=990, y=990 ### FOR EN21260 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000217) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/217_dem.csv"; //x=990, y=990 ### FOR EN21261 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000218) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/218_dem.csv"; //x=990, y=990 ### FOR EN21262 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000219) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/219_dem.csv"; //x=990, y=990 ### FOR EN22-001 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000220) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/220_dem.csv"; //x=990, y=990 ### FOR EN22-002 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000221) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/221_dem.csv"; //x=990, y=990 ### FOR EN22-003 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000222) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/222_dem.csv"; //x=990, y=990 ### FOR EN22-004 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000223) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/223_dem.csv"; //x=990, y=990 ### FOR EN22-005 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000224) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/224_dem.csv"; //x=990, y=990 ### FOR EN22-006 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000225) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/225_dem.csv"; //x=990, y=990 ### FOR EN22-007 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000226) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/226_dem.csv"; //x=990, y=990 ### FOR EN22-008 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000227) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/227_dem.csv"; //x=990, y=990 ### FOR EN22-009 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000228) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/228_dem.csv"; //x=990, y=990 ### FOR EN22-010 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000229) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/229_dem.csv"; //x=990, y=990 ### FOR EN22-011 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000230) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/230_dem.csv"; //x=990, y=990 ### FOR EN22-012 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000231) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/231_dem.csv"; //x=990, y=990 ### FOR EN22-013 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000232) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/232_dem.csv"; //x=990, y=990 ### FOR EN22-014 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000233) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/233_dem.csv"; //x=990, y=990 ### FOR EN22-015 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000234) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/234_dem.csv"; //x=990, y=990 ### FOR EN22-016 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000235) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/235_dem.csv"; //x=990, y=990 ### FOR EN22-017 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000236) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/236_dem.csv"; //x=990, y=990 ### FOR EN22-018 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000237) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/237_dem.csv"; //x=990, y=990 ### FOR EN22-019 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000238) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/238_dem.csv"; //x=990, y=990 ### FOR EN22-020 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000239) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/239_dem.csv"; //x=990, y=990 ### FOR EN22-021 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000240) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/240_dem.csv"; //x=990, y=990 ### FOR EN22-022 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000241) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/241_dem.csv"; //x=990, y=990 ### FOR EN22-023 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000242) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/242_dem.csv"; //x=990, y=990 ### FOR EN22-024 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000243) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/243_dem.csv"; //x=990, y=990 ### FOR EN22-025 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000244) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/244_dem.csv"; //x=990, y=990 ### FOR EN22-026 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000245) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/245_dem.csv"; //x=990, y=990 ### FOR EN22-027 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000246) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/246_dem.csv"; //x=990, y=990 ### FOR EN22-028 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000247) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/247_dem.csv"; //x=990, y=990 ### FOR EN22-029 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000248) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/248_dem.csv"; //x=990, y=990 ### FOR EN22-030 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000249) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/249_dem.csv"; //x=990, y=990 ### FOR EN22-031 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000250) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/250_dem.csv"; //x=990, y=990 ### FOR EN22-032 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000251) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/251_dem.csv"; //x=990, y=990 ### FOR EN22-033 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000252) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/252_dem.csv"; //x=990, y=990 ### FOR EN22-034 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000253) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/253_dem.csv"; //x=990, y=990 ### FOR EN22-035 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000254) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/254_dem.csv"; //x=990, y=990 ### FOR EN22-036 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000255) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/255_dem.csv"; //x=990, y=990 ### FOR EN22-037 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000256) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/256_dem.csv"; //x=990, y=990 ### FOR EN22-038 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000257) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/257_dem.csv"; //x=990, y=990 ### FOR EN22-039 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000258) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/258_dem.csv"; //x=990, y=990 ### FOR EN22-040 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000259) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/259_dem.csv"; //x=990, y=990 ### FOR EN22-041 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000260) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/260_dem.csv"; //x=990, y=990 ### FOR EN22-042 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000261) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/261_dem.csv"; //x=990, y=990 ### FOR EN22-043 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000262) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/262_dem.csv"; //x=990, y=990 ### FOR EN22-044 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000263) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/263_dem.csv"; //x=990, y=990 ### FOR EN22-045 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000264) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/264_dem.csv"; //x=990, y=990 ### FOR EN22-046 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000265) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/265_dem.csv"; //x=990, y=990 ### FOR EN22-047 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000266) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/266_dem.csv"; //x=990, y=990 ### FOR EN22-048 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000267) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/267_dem.csv"; //x=990, y=990 ### FOR EN22-049 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000268) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/268_dem.csv"; //x=990, y=990 ### FOR EN22-050 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000269) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/269_dem.csv"; //x=990, y=990 ### FOR EN22-051 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000270) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/270_dem.csv"; //x=990, y=990 ### FOR EN22-052 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000271) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/271_dem.csv"; //x=990, y=990 ### FOR EN22-053 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000272) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/272_dem.csv"; //x=990, y=990 ### FOR EN22-054 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000273) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/273_dem.csv"; //x=990, y=990 ### FOR EN22-055 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000274) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/274_dem.csv"; //x=990, y=990 ### FOR EN22-056 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000275) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/275_dem.csv"; //x=990, y=990 ### FOR EN22-057 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000276) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/276_dem.csv"; //x=990, y=990 ### FOR EN22-058 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000277) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/277_dem.csv"; //x=990, y=990 ### FOR EN22-059 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000278) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/278_dem.csv"; //x=990, y=990 ### FOR EN22-060 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000279) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/279_dem.csv"; //x=990, y=990 ### FOR EN22-061 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000280) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/280_dem.csv"; //x=990, y=990 ### FOR EN22-062 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000281) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/281_dem.csv"; //x=990, y=990 ### FOR EN22-063 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000282) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/282_dem.csv"; //x=990, y=990 ### FOR EN22-064 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000283) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/283_dem.csv"; //x=990, y=990 ### FOR EN22-065 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000284) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/284_dem.csv"; //x=990, y=990 ### FOR EN22-066 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000285) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/285_dem.csv"; //x=990, y=990 ### FOR EN22-067 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000286) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/286_dem.csv"; //x=990, y=990 ### FOR EN22-068 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000287) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/287_dem.csv"; //x=990, y=990 ### FOR EN22-069 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000288) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/288_dem.csv"; //x=990, y=990 ### FOR EN22-070 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000289) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/289_dem.csv"; //x=990, y=990 ### FOR EN22-071 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000290) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/290_dem.csv"; //x=990, y=990 ### FOR EN22-072 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000291) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/291_dem.csv"; //x=990, y=990 ### FOR EN22-073 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000292) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/292_dem.csv"; //x=990, y=990 ### FOR EN22-074 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000293) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/293_dem.csv"; //x=990, y=990 ### FOR EN22-075 ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000294) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/294_dem.csv"; //x=990, y=990 ### FOR TY04VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000295) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/295_dem.csv"; //x=990, y=990 ### FOR TY04VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000296) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/296_dem.csv"; //x=990, y=990 ### FOR TY07VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000297) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/297_dem.csv"; //x=990, y=990 ### FOR TY07VII ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000298) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/298_dem.csv"; //x=990, y=990 ### FOR TY09VI ###
			strcpy(demfilename, deminputbuf);
		} else if (parameter[0].weatherchoice == 2000000299) {
			char deminputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/299_dem.csv"; //x=990, y=990 ### FOR TY09VII ###
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
		if (parameter[0].weatherchoice == 1000000001) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/001_slope.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000002) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/002_slope.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000003) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/003_slope.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000004) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/004_slope.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000005) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/005_slope.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000006) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/006_slope.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000007) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/007_slope.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000008) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/008_slope.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000009) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/009_slope.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000010) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/010_slope.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000011) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/011_slope.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000012) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/012_slope.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000013) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/013_slope.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000014) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/014_slope.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000015) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/015_slope.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000016) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/016_slope.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000017) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/017_slope.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000018) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/018_slope.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000019) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/019_slope.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000020) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/020_slope.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000021) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/021_slope.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000022) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/022_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000023) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/023_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000024) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/024_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000025) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/025_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000026) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/026_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000027) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/027_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000028) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/028_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000029) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/029_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000030) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/030_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000031) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/031_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000032) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/032_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000033) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/033_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000034) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/034_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000035) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/035_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000036) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/036_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000037) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/037_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000038) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/038_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000039) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/039_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000040) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/040_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000041) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/041_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000042) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/042_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000043) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/043_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000044) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/044_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000045) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/045_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000046) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/046_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000047) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/047_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000048) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/048_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000049) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/049_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000050) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/050_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000051) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/051_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000052) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/052_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000053) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/053_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000054) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/054_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000055) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/055_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000056) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/056_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000057) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/057_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000058) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/058_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000059) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/059_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000060) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/060_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000061) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/061_slope.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000062) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/062_slope.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000063) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/063_slope.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000064) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/064_slope.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000065) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/065_slope.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000066) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/066_slope.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000067) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/067_slope.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000068) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/068_slope.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000069) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/069_slope.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000070) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/070_slope.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000071) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/071_slope.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000072) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/072_slope.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000073) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/073_slope.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000074) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/074_slope.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000075) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/075_slope.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000076) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/076_slope.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000077) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/077_slope.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000078) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/078_slope.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000079) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/079_slope.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000080) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/080_slope.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000081) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/081_slope.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000082) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/082_slope.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000083) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/083_slope.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000084) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/084_slope.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000085) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/085_slope.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000086) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/086_slope.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000087) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/087_slope.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000088) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/088_slope.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000089) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/089_slope.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000090) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/090_slope.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000091) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/091_slope.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000092) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/092_slope.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000093) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/093_slope.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000094) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/094_slope.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000095) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/095_slope.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000096) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/096_slope.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000097) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/097_slope.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000098) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/098_slope.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000099) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/099_slope.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000100) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/100_slope.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000101) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/101_slope.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000102) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/102_slope.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000103) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/103_slope.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000104) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/104_slope.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000105) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/105_slope.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000106) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/106_slope.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000107) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/107_slope.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000108) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/108_slope.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000109) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/109_slope.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000110) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/110_slope.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000111) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/111_slope.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000112) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/112_slope.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000113) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/113_slope.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000114) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/114_slope.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000115) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/115_slope.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000116) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/116_slope.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000117) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/117_slope.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000118) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/118_slope.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000119) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/119_slope.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000120) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/120_slope.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000121) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/121_slope.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000122) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/122_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000123) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/123_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000124) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/124_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000125) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/125_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000126) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/126_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000127) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/127_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000128) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/128_slope.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000129) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/129_slope.csv"; //x=990, y=990 ### FOR B19-T1 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000130) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/130_slope.csv"; //x=990, y=990 ### FOR B19-T2 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000131) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/131_slope.csv"; //x=990, y=990 ### FOR EN18061 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000132) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/132_slope.csv"; //x=990, y=990 ### FOR EN18062 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000133) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/133_slope.csv"; //x=990, y=990 ### FOR EN18063 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000134) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/134_slope.csv"; //x=990, y=990 ### FOR EN18064 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000135) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/135_slope.csv"; //x=990, y=990 ### FOR EN18065 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000136) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/136_slope.csv"; //x=990, y=990 ### FOR EN18066 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000137) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/137_slope.csv"; //x=990, y=990 ### FOR EN18067 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000138) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/138_slope.csv"; //x=990, y=990 ### FOR EN18068 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000139) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/139_slope.csv"; //x=990, y=990 ### FOR EN18069 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000140) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/140_slope.csv"; //x=990, y=990 ### FOR EN18070_centre ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000141) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/141_slope.csv"; //x=990, y=990 ### FOR EN18070_edge ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000142) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/142_slope.csv"; //x=990, y=990 ### FOR EN18070_end ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000143) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/143_slope.csv"; //x=990, y=990 ### FOR EN18070_transition ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000144) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/144_slope.csv"; //x=990, y=990 ### FOR EN18071 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000145) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/145_slope.csv"; //x=990, y=990 ### FOR EN18072 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000146) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/146_slope.csv"; //x=990, y=990 ### FOR EN18073 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000147) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/147_slope.csv"; //x=990, y=990 ### FOR EN18074 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000148) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/148_slope.csv"; //x=990, y=990 ### FOR EN18075 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000149) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/149_slope.csv"; //x=990, y=990 ### FOR EN18076 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000150) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/150_slope.csv"; //x=990, y=990 ### FOR EN18077 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000151) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/151_slope.csv"; //x=990, y=990 ### FOR EN18078 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000152) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/152_slope.csv"; //x=990, y=990 ### FOR EN18079 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000153) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/153_slope.csv"; //x=990, y=990 ### FOR EN18080 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000154) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/154_slope.csv"; //x=990, y=990 ### FOR EN18081 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000155) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/155_slope.csv"; //x=990, y=990 ### FOR EN18082 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000156) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/156_slope.csv"; //x=990, y=990 ### FOR EN18083 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000157) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/157_slope.csv"; //x=990, y=990 ### FOR EN21201 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000158) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/158_slope.csv"; //x=990, y=990 ### FOR EN21202 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000159) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/159_slope.csv"; //x=990, y=990 ### FOR EN21203 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000160) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/160_slope.csv"; //x=990, y=990 ### FOR EN21204 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000161) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/161_slope.csv"; //x=990, y=990 ### FOR EN21205 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000162) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/162_slope.csv"; //x=990, y=990 ### FOR EN21206 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000163) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/163_slope.csv"; //x=990, y=990 ### FOR EN21207 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000164) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/164_slope.csv"; //x=990, y=990 ### FOR EN21208 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000165) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/165_slope.csv"; //x=990, y=990 ### FOR EN21209 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000166) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/166_slope.csv"; //x=990, y=990 ### FOR EN21210 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000167) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/167_slope.csv"; //x=990, y=990 ### FOR EN21211 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000168) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/168_slope.csv"; //x=990, y=990 ### FOR EN21212 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000169) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/169_slope.csv"; //x=990, y=990 ### FOR EN21213 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000170) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/170_slope.csv"; //x=990, y=990 ### FOR EN21214 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000171) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/171_slope.csv"; //x=990, y=990 ### FOR EN21215 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000172) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/172_slope.csv"; //x=990, y=990 ### FOR EN21216 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000173) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/173_slope.csv"; //x=990, y=990 ### FOR EN21217 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000174) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/174_slope.csv"; //x=990, y=990 ### FOR EN21218 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000175) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/175_slope.csv"; //x=990, y=990 ### FOR EN21219 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000176) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/176_slope.csv"; //x=990, y=990 ### FOR EN21220 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000177) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/177_slope.csv"; //x=990, y=990 ### FOR EN21221 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000178) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/178_slope.csv"; //x=990, y=990 ### FOR EN21222 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000179) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/179_slope.csv"; //x=990, y=990 ### FOR EN21223 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000180) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/180_slope.csv"; //x=990, y=990 ### FOR EN21224 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000181) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/181_slope.csv"; //x=990, y=990 ### FOR EN21225 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000182) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/182_slope.csv"; //x=990, y=990 ### FOR EN21226 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000183) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/183_slope.csv"; //x=990, y=990 ### FOR EN21227 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000184) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/184_slope.csv"; //x=990, y=990 ### FOR EN21228 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000185) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/185_slope.csv"; //x=990, y=990 ### FOR EN21229 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000186) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/186_slope.csv"; //x=990, y=990 ### FOR EN21230 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000187) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/187_slope.csv"; //x=990, y=990 ### FOR EN21231 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000188) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/188_slope.csv"; //x=990, y=990 ### FOR EN21232 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000189) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/189_slope.csv"; //x=990, y=990 ### FOR EN21233 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000190) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/190_slope.csv"; //x=990, y=990 ### FOR EN21234 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000191) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/191_slope.csv"; //x=990, y=990 ### FOR EN21235 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000192) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/192_slope.csv"; //x=990, y=990 ### FOR EN21236 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000193) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/193_slope.csv"; //x=990, y=990 ### FOR EN21237 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000194) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/194_slope.csv"; //x=990, y=990 ### FOR EN21238 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000195) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/195_slope.csv"; //x=990, y=990 ### FOR EN21239 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000196) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/196_slope.csv"; //x=990, y=990 ### FOR EN21240 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000197) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/197_slope.csv"; //x=990, y=990 ### FOR EN21241 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000198) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/198_slope.csv"; //x=990, y=990 ### FOR EN21242 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000199) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/199_slope.csv"; //x=990, y=990 ### FOR EN21243 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000200) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/200_slope.csv"; //x=990, y=990 ### FOR EN21244 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000201) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/201_slope.csv"; //x=990, y=990 ### FOR EN21245 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000202) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/202_slope.csv"; //x=990, y=990 ### FOR EN21246 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000203) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/203_slope.csv"; //x=990, y=990 ### FOR EN21247 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000204) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/204_slope.csv"; //x=990, y=990 ### FOR EN21248 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000205) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/205_slope.csv"; //x=990, y=990 ### FOR EN21249 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000206) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/206_slope.csv"; //x=990, y=990 ### FOR EN21250 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000207) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/207_slope.csv"; //x=990, y=990 ### FOR EN21251 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000208) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/208_slope.csv"; //x=990, y=990 ### FOR EN21252 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000209) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/209_slope.csv"; //x=990, y=990 ### FOR EN21253 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000210) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/210_slope.csv"; //x=990, y=990 ### FOR EN21254 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000211) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/211_slope.csv"; //x=990, y=990 ### FOR EN21255 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000212) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/212_slope.csv"; //x=990, y=990 ### FOR EN21256 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000213) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/213_slope.csv"; //x=990, y=990 ### FOR EN21257 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000214) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/214_slope.csv"; //x=990, y=990 ### FOR EN21258 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000215) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/215_slope.csv"; //x=990, y=990 ### FOR EN21259 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000216) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/216_slope.csv"; //x=990, y=990 ### FOR EN21260 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000217) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/217_slope.csv"; //x=990, y=990 ### FOR EN21261 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000218) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/218_slope.csv"; //x=990, y=990 ### FOR EN21262 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000219) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/219_slope.csv"; //x=990, y=990 ### FOR EN22-001 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000220) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/220_slope.csv"; //x=990, y=990 ### FOR EN22-002 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000221) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/221_slope.csv"; //x=990, y=990 ### FOR EN22-003 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000222) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/222_slope.csv"; //x=990, y=990 ### FOR EN22-004 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000223) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/223_slope.csv"; //x=990, y=990 ### FOR EN22-005 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000224) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/224_slope.csv"; //x=990, y=990 ### FOR EN22-006 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000225) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/225_slope.csv"; //x=990, y=990 ### FOR EN22-007 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000226) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/226_slope.csv"; //x=990, y=990 ### FOR EN22-008 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000227) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/227_slope.csv"; //x=990, y=990 ### FOR EN22-009 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000228) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/228_slope.csv"; //x=990, y=990 ### FOR EN22-010 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000229) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/229_slope.csv"; //x=990, y=990 ### FOR EN22-011 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000230) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/230_slope.csv"; //x=990, y=990 ### FOR EN22-012 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000231) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/231_slope.csv"; //x=990, y=990 ### FOR EN22-013 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000232) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/232_slope.csv"; //x=990, y=990 ### FOR EN22-014 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000233) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/233_slope.csv"; //x=990, y=990 ### FOR EN22-015 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000234) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/234_slope.csv"; //x=990, y=990 ### FOR EN22-016 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000235) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/235_slope.csv"; //x=990, y=990 ### FOR EN22-017 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000236) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/236_slope.csv"; //x=990, y=990 ### FOR EN22-018 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000237) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/237_slope.csv"; //x=990, y=990 ### FOR EN22-019 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000238) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/238_slope.csv"; //x=990, y=990 ### FOR EN22-020 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000239) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/239_slope.csv"; //x=990, y=990 ### FOR EN22-021 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000240) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/240_slope.csv"; //x=990, y=990 ### FOR EN22-022 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000241) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/241_slope.csv"; //x=990, y=990 ### FOR EN22-023 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000242) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/242_slope.csv"; //x=990, y=990 ### FOR EN22-024 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000243) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/243_slope.csv"; //x=990, y=990 ### FOR EN22-025 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000244) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/244_slope.csv"; //x=990, y=990 ### FOR EN22-026 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000245) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/245_slope.csv"; //x=990, y=990 ### FOR EN22-027 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000246) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/246_slope.csv"; //x=990, y=990 ### FOR EN22-028 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000247) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/247_slope.csv"; //x=990, y=990 ### FOR EN22-029 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000248) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/248_slope.csv"; //x=990, y=990 ### FOR EN22-030 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000249) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/249_slope.csv"; //x=990, y=990 ### FOR EN22-031 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000250) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/250_slope.csv"; //x=990, y=990 ### FOR EN22-032 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000251) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/251_slope.csv"; //x=990, y=990 ### FOR EN22-033 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000252) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/252_slope.csv"; //x=990, y=990 ### FOR EN22-034 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000253) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/253_slope.csv"; //x=990, y=990 ### FOR EN22-035 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000254) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/254_slope.csv"; //x=990, y=990 ### FOR EN22-036 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000255) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/255_slope.csv"; //x=990, y=990 ### FOR EN22-037 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000256) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/256_slope.csv"; //x=990, y=990 ### FOR EN22-038 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000257) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/257_slope.csv"; //x=990, y=990 ### FOR EN22-039 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000258) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/258_slope.csv"; //x=990, y=990 ### FOR EN22-040 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000259) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/259_slope.csv"; //x=990, y=990 ### FOR EN22-041 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000260) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/260_slope.csv"; //x=990, y=990 ### FOR EN22-042 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000261) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/261_slope.csv"; //x=990, y=990 ### FOR EN22-043 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000262) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/262_slope.csv"; //x=990, y=990 ### FOR EN22-044 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000263) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/263_slope.csv"; //x=990, y=990 ### FOR EN22-045 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000264) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/264_slope.csv"; //x=990, y=990 ### FOR EN22-046 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000265) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/265_slope.csv"; //x=990, y=990 ### FOR EN22-047 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000266) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/266_slope.csv"; //x=990, y=990 ### FOR EN22-048 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000267) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/267_slope.csv"; //x=990, y=990 ### FOR EN22-049 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000268) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/268_slope.csv"; //x=990, y=990 ### FOR EN22-050 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000269) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/269_slope.csv"; //x=990, y=990 ### FOR EN22-051 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000270) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/270_slope.csv"; //x=990, y=990 ### FOR EN22-052 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000271) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/271_slope.csv"; //x=990, y=990 ### FOR EN22-053 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000272) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/272_slope.csv"; //x=990, y=990 ### FOR EN22-054 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000273) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/273_slope.csv"; //x=990, y=990 ### FOR EN22-055 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000274) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/274_slope.csv"; //x=990, y=990 ### FOR EN22-056 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000275) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/275_slope.csv"; //x=990, y=990 ### FOR EN22-057 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000276) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/276_slope.csv"; //x=990, y=990 ### FOR EN22-058 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000277) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/277_slope.csv"; //x=990, y=990 ### FOR EN22-059 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000278) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/278_slope.csv"; //x=990, y=990 ### FOR EN22-060 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000279) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/279_slope.csv"; //x=990, y=990 ### FOR EN22-061 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000280) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/280_slope.csv"; //x=990, y=990 ### FOR EN22-062 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000281) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/281_slope.csv"; //x=990, y=990 ### FOR EN22-063 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000282) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/282_slope.csv"; //x=990, y=990 ### FOR EN22-064 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000283) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/283_slope.csv"; //x=990, y=990 ### FOR EN22-065 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000284) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/284_slope.csv"; //x=990, y=990 ### FOR EN22-066 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000285) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/285_slope.csv"; //x=990, y=990 ### FOR EN22-067 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000286) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/286_slope.csv"; //x=990, y=990 ### FOR EN22-068 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000287) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/287_slope.csv"; //x=990, y=990 ### FOR EN22-069 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000288) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/288_slope.csv"; //x=990, y=990 ### FOR EN22-070 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000289) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/289_slope.csv"; //x=990, y=990 ### FOR EN22-071 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000290) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/290_slope.csv"; //x=990, y=990 ### FOR EN22-072 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000291) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/291_slope.csv"; //x=990, y=990 ### FOR EN22-073 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000292) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/292_slope.csv"; //x=990, y=990 ### FOR EN22-074 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000293) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/293_slope.csv"; //x=990, y=990 ### FOR EN22-075 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000294) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/294_slope.csv"; //x=990, y=990 ### FOR TY04VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000295) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/295_slope.csv"; //x=990, y=990 ### FOR TY04VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000296) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/296_slope.csv"; //x=990, y=990 ### FOR TY07VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000297) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/297_slope.csv"; //x=990, y=990 ### FOR TY07VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000298) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/298_slope.csv"; //x=990, y=990 ### FOR TY09VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 1000000299) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/299_slope.csv"; //x=990, y=990 ### FOR TY09VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000001) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/001_slope.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000002) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/002_slope.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000003) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/003_slope.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000004) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/004_slope.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000005) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/005_slope.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000006) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/006_slope.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000007) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/007_slope.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000008) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/008_slope.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000009) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/009_slope.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000010) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/010_slope.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000011) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/011_slope.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000012) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/012_slope.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000013) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/013_slope.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000014) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/014_slope.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000015) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/015_slope.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000016) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/016_slope.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000017) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/017_slope.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000018) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/018_slope.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000019) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/019_slope.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000020) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/020_slope.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000021) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/021_slope.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000022) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/022_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000023) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/023_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000024) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/024_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000025) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/025_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000026) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/026_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000027) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/027_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000028) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/028_slope.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000029) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/029_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000030) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/030_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000031) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/031_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000032) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/032_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000033) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/033_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000034) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/034_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000035) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/035_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000036) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/036_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000037) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/037_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000038) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/038_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000039) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/039_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000040) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/040_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000041) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/041_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000042) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/042_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000043) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/043_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000044) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/044_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000045) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/045_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000046) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/046_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000047) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/047_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000048) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/048_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000049) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/049_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000050) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/050_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000051) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/051_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000052) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/052_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000053) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/053_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000054) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/054_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000055) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/055_slope.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000056) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/056_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000057) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/057_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000058) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/058_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000059) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/059_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000060) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/060_slope.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000061) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/061_slope.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000062) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/062_slope.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000063) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/063_slope.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000064) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/064_slope.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000065) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/065_slope.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000066) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/066_slope.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000067) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/067_slope.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000068) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/068_slope.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000069) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/069_slope.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000070) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/070_slope.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000071) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/071_slope.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000072) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/072_slope.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000073) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/073_slope.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000074) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/074_slope.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000075) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/075_slope.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000076) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/076_slope.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000077) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/077_slope.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000078) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/078_slope.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000079) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/079_slope.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000080) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/080_slope.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000081) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/081_slope.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000082) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/082_slope.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000083) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/083_slope.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000084) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/084_slope.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000085) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/085_slope.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000086) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/086_slope.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000087) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/087_slope.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000088) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/088_slope.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000089) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/089_slope.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000090) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/090_slope.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000091) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/091_slope.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000092) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/092_slope.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000093) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/093_slope.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000094) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/094_slope.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000095) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/095_slope.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000096) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/096_slope.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000097) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/097_slope.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000098) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/098_slope.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000099) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/099_slope.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000100) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/100_slope.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000101) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/101_slope.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000102) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/102_slope.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000103) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/103_slope.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000104) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/104_slope.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000105) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/105_slope.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000106) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/106_slope.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000107) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/107_slope.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000108) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/108_slope.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000109) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/109_slope.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000110) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/110_slope.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000111) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/111_slope.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000112) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/112_slope.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000113) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/113_slope.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000114) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/114_slope.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000115) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/115_slope.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000116) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/116_slope.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000117) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/117_slope.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000118) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/118_slope.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000119) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/119_slope.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000120) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/120_slope.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000121) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/121_slope.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000122) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/122_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000123) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/123_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000124) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/124_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000125) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/125_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000126) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/126_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000127) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/127_slope.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000128) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/128_slope.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000129) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/129_slope.csv"; //x=990, y=990 ### FOR B19-T1 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000130) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/130_slope.csv"; //x=990, y=990 ### FOR B19-T2 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000131) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/131_slope.csv"; //x=990, y=990 ### FOR EN18061 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000132) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/132_slope.csv"; //x=990, y=990 ### FOR EN18062 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000133) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/133_slope.csv"; //x=990, y=990 ### FOR EN18063 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000134) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/134_slope.csv"; //x=990, y=990 ### FOR EN18064 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000135) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/135_slope.csv"; //x=990, y=990 ### FOR EN18065 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000136) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/136_slope.csv"; //x=990, y=990 ### FOR EN18066 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000137) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/137_slope.csv"; //x=990, y=990 ### FOR EN18067 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000138) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/138_slope.csv"; //x=990, y=990 ### FOR EN18068 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000139) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/139_slope.csv"; //x=990, y=990 ### FOR EN18069 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000140) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/140_slope.csv"; //x=990, y=990 ### FOR EN18070_centre ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000141) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/141_slope.csv"; //x=990, y=990 ### FOR EN18070_edge ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000142) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/142_slope.csv"; //x=990, y=990 ### FOR EN18070_end ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000143) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/143_slope.csv"; //x=990, y=990 ### FOR EN18070_transition ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000144) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/144_slope.csv"; //x=990, y=990 ### FOR EN18071 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000145) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/145_slope.csv"; //x=990, y=990 ### FOR EN18072 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000146) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/146_slope.csv"; //x=990, y=990 ### FOR EN18073 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000147) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/147_slope.csv"; //x=990, y=990 ### FOR EN18074 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000148) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/148_slope.csv"; //x=990, y=990 ### FOR EN18075 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000149) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/149_slope.csv"; //x=990, y=990 ### FOR EN18076 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000150) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/150_slope.csv"; //x=990, y=990 ### FOR EN18077 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000151) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/151_slope.csv"; //x=990, y=990 ### FOR EN18078 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000152) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/152_slope.csv"; //x=990, y=990 ### FOR EN18079 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000153) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/153_slope.csv"; //x=990, y=990 ### FOR EN18080 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000154) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/154_slope.csv"; //x=990, y=990 ### FOR EN18081 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000155) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/155_slope.csv"; //x=990, y=990 ### FOR EN18082 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000156) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/156_slope.csv"; //x=990, y=990 ### FOR EN18083 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000157) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/157_slope.csv"; //x=990, y=990 ### FOR EN21201 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000158) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/158_slope.csv"; //x=990, y=990 ### FOR EN21202 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000159) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/159_slope.csv"; //x=990, y=990 ### FOR EN21203 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000160) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/160_slope.csv"; //x=990, y=990 ### FOR EN21204 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000161) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/161_slope.csv"; //x=990, y=990 ### FOR EN21205 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000162) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/162_slope.csv"; //x=990, y=990 ### FOR EN21206 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000163) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/163_slope.csv"; //x=990, y=990 ### FOR EN21207 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000164) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/164_slope.csv"; //x=990, y=990 ### FOR EN21208 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000165) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/165_slope.csv"; //x=990, y=990 ### FOR EN21209 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000166) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/166_slope.csv"; //x=990, y=990 ### FOR EN21210 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000167) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/167_slope.csv"; //x=990, y=990 ### FOR EN21211 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000168) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/168_slope.csv"; //x=990, y=990 ### FOR EN21212 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000169) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/169_slope.csv"; //x=990, y=990 ### FOR EN21213 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000170) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/170_slope.csv"; //x=990, y=990 ### FOR EN21214 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000171) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/171_slope.csv"; //x=990, y=990 ### FOR EN21215 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000172) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/172_slope.csv"; //x=990, y=990 ### FOR EN21216 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000173) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/173_slope.csv"; //x=990, y=990 ### FOR EN21217 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000174) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/174_slope.csv"; //x=990, y=990 ### FOR EN21218 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000175) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/175_slope.csv"; //x=990, y=990 ### FOR EN21219 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000176) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/176_slope.csv"; //x=990, y=990 ### FOR EN21220 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000177) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/177_slope.csv"; //x=990, y=990 ### FOR EN21221 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000178) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/178_slope.csv"; //x=990, y=990 ### FOR EN21222 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000179) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/179_slope.csv"; //x=990, y=990 ### FOR EN21223 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000180) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/180_slope.csv"; //x=990, y=990 ### FOR EN21224 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000181) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/181_slope.csv"; //x=990, y=990 ### FOR EN21225 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000182) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/182_slope.csv"; //x=990, y=990 ### FOR EN21226 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000183) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/183_slope.csv"; //x=990, y=990 ### FOR EN21227 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000184) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/184_slope.csv"; //x=990, y=990 ### FOR EN21228 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000185) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/185_slope.csv"; //x=990, y=990 ### FOR EN21229 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000186) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/186_slope.csv"; //x=990, y=990 ### FOR EN21230 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000187) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/187_slope.csv"; //x=990, y=990 ### FOR EN21231 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000188) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/188_slope.csv"; //x=990, y=990 ### FOR EN21232 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000189) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/189_slope.csv"; //x=990, y=990 ### FOR EN21233 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000190) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/190_slope.csv"; //x=990, y=990 ### FOR EN21234 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000191) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/191_slope.csv"; //x=990, y=990 ### FOR EN21235 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000192) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/192_slope.csv"; //x=990, y=990 ### FOR EN21236 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000193) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/193_slope.csv"; //x=990, y=990 ### FOR EN21237 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000194) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/194_slope.csv"; //x=990, y=990 ### FOR EN21238 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000195) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/195_slope.csv"; //x=990, y=990 ### FOR EN21239 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000196) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/196_slope.csv"; //x=990, y=990 ### FOR EN21240 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000197) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/197_slope.csv"; //x=990, y=990 ### FOR EN21241 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000198) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/198_slope.csv"; //x=990, y=990 ### FOR EN21242 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000199) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/199_slope.csv"; //x=990, y=990 ### FOR EN21243 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000200) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/200_slope.csv"; //x=990, y=990 ### FOR EN21244 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000201) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/201_slope.csv"; //x=990, y=990 ### FOR EN21245 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000202) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/202_slope.csv"; //x=990, y=990 ### FOR EN21246 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000203) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/203_slope.csv"; //x=990, y=990 ### FOR EN21247 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000204) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/204_slope.csv"; //x=990, y=990 ### FOR EN21248 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000205) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/205_slope.csv"; //x=990, y=990 ### FOR EN21249 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000206) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/206_slope.csv"; //x=990, y=990 ### FOR EN21250 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000207) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/207_slope.csv"; //x=990, y=990 ### FOR EN21251 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000208) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/208_slope.csv"; //x=990, y=990 ### FOR EN21252 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000209) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/209_slope.csv"; //x=990, y=990 ### FOR EN21253 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000210) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/210_slope.csv"; //x=990, y=990 ### FOR EN21254 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000211) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/211_slope.csv"; //x=990, y=990 ### FOR EN21255 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000212) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/212_slope.csv"; //x=990, y=990 ### FOR EN21256 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000213) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/213_slope.csv"; //x=990, y=990 ### FOR EN21257 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000214) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/214_slope.csv"; //x=990, y=990 ### FOR EN21258 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000215) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/215_slope.csv"; //x=990, y=990 ### FOR EN21259 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000216) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/216_slope.csv"; //x=990, y=990 ### FOR EN21260 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000217) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/217_slope.csv"; //x=990, y=990 ### FOR EN21261 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000218) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/218_slope.csv"; //x=990, y=990 ### FOR EN21262 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000219) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/219_slope.csv"; //x=990, y=990 ### FOR EN22-001 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000220) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/220_slope.csv"; //x=990, y=990 ### FOR EN22-002 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000221) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/221_slope.csv"; //x=990, y=990 ### FOR EN22-003 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000222) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/222_slope.csv"; //x=990, y=990 ### FOR EN22-004 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000223) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/223_slope.csv"; //x=990, y=990 ### FOR EN22-005 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000224) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/224_slope.csv"; //x=990, y=990 ### FOR EN22-006 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000225) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/225_slope.csv"; //x=990, y=990 ### FOR EN22-007 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000226) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/226_slope.csv"; //x=990, y=990 ### FOR EN22-008 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000227) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/227_slope.csv"; //x=990, y=990 ### FOR EN22-009 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000228) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/228_slope.csv"; //x=990, y=990 ### FOR EN22-010 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000229) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/229_slope.csv"; //x=990, y=990 ### FOR EN22-011 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000230) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/230_slope.csv"; //x=990, y=990 ### FOR EN22-012 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000231) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/231_slope.csv"; //x=990, y=990 ### FOR EN22-013 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000232) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/232_slope.csv"; //x=990, y=990 ### FOR EN22-014 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000233) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/233_slope.csv"; //x=990, y=990 ### FOR EN22-015 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000234) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/234_slope.csv"; //x=990, y=990 ### FOR EN22-016 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000235) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/235_slope.csv"; //x=990, y=990 ### FOR EN22-017 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000236) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/236_slope.csv"; //x=990, y=990 ### FOR EN22-018 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000237) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/237_slope.csv"; //x=990, y=990 ### FOR EN22-019 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000238) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/238_slope.csv"; //x=990, y=990 ### FOR EN22-020 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000239) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/239_slope.csv"; //x=990, y=990 ### FOR EN22-021 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000240) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/240_slope.csv"; //x=990, y=990 ### FOR EN22-022 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000241) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/241_slope.csv"; //x=990, y=990 ### FOR EN22-023 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000242) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/242_slope.csv"; //x=990, y=990 ### FOR EN22-024 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000243) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/243_slope.csv"; //x=990, y=990 ### FOR EN22-025 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000244) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/244_slope.csv"; //x=990, y=990 ### FOR EN22-026 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000245) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/245_slope.csv"; //x=990, y=990 ### FOR EN22-027 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000246) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/246_slope.csv"; //x=990, y=990 ### FOR EN22-028 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000247) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/247_slope.csv"; //x=990, y=990 ### FOR EN22-029 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000248) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/248_slope.csv"; //x=990, y=990 ### FOR EN22-030 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000249) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/249_slope.csv"; //x=990, y=990 ### FOR EN22-031 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000250) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/250_slope.csv"; //x=990, y=990 ### FOR EN22-032 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000251) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/251_slope.csv"; //x=990, y=990 ### FOR EN22-033 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000252) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/252_slope.csv"; //x=990, y=990 ### FOR EN22-034 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000253) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/253_slope.csv"; //x=990, y=990 ### FOR EN22-035 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000254) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/254_slope.csv"; //x=990, y=990 ### FOR EN22-036 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000255) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/255_slope.csv"; //x=990, y=990 ### FOR EN22-037 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000256) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/256_slope.csv"; //x=990, y=990 ### FOR EN22-038 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000257) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/257_slope.csv"; //x=990, y=990 ### FOR EN22-039 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000258) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/258_slope.csv"; //x=990, y=990 ### FOR EN22-040 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000259) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/259_slope.csv"; //x=990, y=990 ### FOR EN22-041 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000260) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/260_slope.csv"; //x=990, y=990 ### FOR EN22-042 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000261) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/261_slope.csv"; //x=990, y=990 ### FOR EN22-043 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000262) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/262_slope.csv"; //x=990, y=990 ### FOR EN22-044 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000263) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/263_slope.csv"; //x=990, y=990 ### FOR EN22-045 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000264) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/264_slope.csv"; //x=990, y=990 ### FOR EN22-046 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000265) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/265_slope.csv"; //x=990, y=990 ### FOR EN22-047 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000266) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/266_slope.csv"; //x=990, y=990 ### FOR EN22-048 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000267) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/267_slope.csv"; //x=990, y=990 ### FOR EN22-049 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000268) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/268_slope.csv"; //x=990, y=990 ### FOR EN22-050 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000269) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/269_slope.csv"; //x=990, y=990 ### FOR EN22-051 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000270) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/270_slope.csv"; //x=990, y=990 ### FOR EN22-052 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000271) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/271_slope.csv"; //x=990, y=990 ### FOR EN22-053 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000272) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/272_slope.csv"; //x=990, y=990 ### FOR EN22-054 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000273) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/273_slope.csv"; //x=990, y=990 ### FOR EN22-055 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000274) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/274_slope.csv"; //x=990, y=990 ### FOR EN22-056 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000275) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/275_slope.csv"; //x=990, y=990 ### FOR EN22-057 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000276) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/276_slope.csv"; //x=990, y=990 ### FOR EN22-058 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000277) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/277_slope.csv"; //x=990, y=990 ### FOR EN22-059 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000278) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/278_slope.csv"; //x=990, y=990 ### FOR EN22-060 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000279) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/279_slope.csv"; //x=990, y=990 ### FOR EN22-061 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000280) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/280_slope.csv"; //x=990, y=990 ### FOR EN22-062 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000281) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/281_slope.csv"; //x=990, y=990 ### FOR EN22-063 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000282) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/282_slope.csv"; //x=990, y=990 ### FOR EN22-064 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000283) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/283_slope.csv"; //x=990, y=990 ### FOR EN22-065 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000284) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/284_slope.csv"; //x=990, y=990 ### FOR EN22-066 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000285) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/285_slope.csv"; //x=990, y=990 ### FOR EN22-067 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000286) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/286_slope.csv"; //x=990, y=990 ### FOR EN22-068 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000287) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/287_slope.csv"; //x=990, y=990 ### FOR EN22-069 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000288) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/288_slope.csv"; //x=990, y=990 ### FOR EN22-070 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000289) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/289_slope.csv"; //x=990, y=990 ### FOR EN22-071 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000290) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/290_slope.csv"; //x=990, y=990 ### FOR EN22-072 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000291) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/291_slope.csv"; //x=990, y=990 ### FOR EN22-073 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000292) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/292_slope.csv"; //x=990, y=990 ### FOR EN22-074 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000293) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/293_slope.csv"; //x=990, y=990 ### FOR EN22-075 ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000294) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/294_slope.csv"; //x=990, y=990 ### FOR TY04VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000295) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/295_slope.csv"; //x=990, y=990 ### FOR TY04VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000296) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/296_slope.csv"; //x=990, y=990 ### FOR TY07VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000297) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/297_slope.csv"; //x=990, y=990 ### FOR TY07VII ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000298) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/298_slope.csv"; //x=990, y=990 ### FOR TY09VI ###
			strcpy(slopefilename, slopeinputbuf);
		} else if (parameter[0].weatherchoice == 2000000299) {
			char slopeinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/299_slope.csv"; //x=990, y=990 ### FOR TY09VII ###
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
		if (parameter[0].weatherchoice == 1000000001) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/001_twi.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000002) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/002_twi.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000003) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/003_twi.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000004) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/004_twi.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000005) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/005_twi.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000006) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/006_twi.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000007) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/007_twi.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000008) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/008_twi.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000009) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/009_twi.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000010) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/010_twi.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000011) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/011_twi.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000012) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/012_twi.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000013) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/013_twi.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000014) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/014_twi.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000015) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/015_twi.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000016) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/016_twi.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000017) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/017_twi.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000018) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/018_twi.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000019) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/019_twi.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000020) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/020_twi.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000021) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/021_twi.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000022) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/022_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000023) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/023_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000024) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/024_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000025) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/025_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000026) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/026_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000027) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/027_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000028) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/028_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000029) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/029_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000030) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/030_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000031) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/031_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000032) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/032_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000033) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/033_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000034) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/034_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000035) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/035_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000036) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/036_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000037) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/037_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000038) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/038_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000039) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/039_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000040) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/040_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000041) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/041_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000042) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/042_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000043) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/043_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000044) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/044_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000045) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/045_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000046) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/046_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000047) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/047_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000048) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/048_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000049) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/049_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000050) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/050_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000051) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/051_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000052) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/052_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000053) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/053_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000054) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/054_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000055) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/055_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000056) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/056_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000057) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/057_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000058) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/058_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000059) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/059_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000060) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/060_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000061) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/061_twi.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000062) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/062_twi.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000063) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/063_twi.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000064) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/064_twi.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000065) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/065_twi.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000066) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/066_twi.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000067) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/067_twi.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000068) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/068_twi.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000069) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/069_twi.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000070) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/070_twi.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000071) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/071_twi.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000072) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/072_twi.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000073) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/073_twi.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000074) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/074_twi.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000075) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/075_twi.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000076) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/076_twi.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000077) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/077_twi.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000078) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/078_twi.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000079) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/079_twi.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000080) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/080_twi.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000081) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/081_twi.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000082) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/082_twi.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000083) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/083_twi.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000084) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/084_twi.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000085) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/085_twi.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000086) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/086_twi.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000087) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/087_twi.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000088) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/088_twi.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000089) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/089_twi.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000090) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/090_twi.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000091) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/091_twi.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000092) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/092_twi.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000093) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/093_twi.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000094) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/094_twi.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000095) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/095_twi.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000096) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/096_twi.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000097) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/097_twi.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000098) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/098_twi.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000099) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/099_twi.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000100) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/100_twi.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000101) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/101_twi.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000102) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/102_twi.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000103) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/103_twi.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000104) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/104_twi.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000105) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/105_twi.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000106) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/106_twi.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000107) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/107_twi.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000108) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/108_twi.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000109) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/109_twi.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000110) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/110_twi.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000111) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/111_twi.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000112) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/112_twi.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000113) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/113_twi.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000114) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/114_twi.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000115) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/115_twi.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000116) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/116_twi.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000117) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/117_twi.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000118) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/118_twi.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000119) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/119_twi.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000120) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/120_twi.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000121) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/121_twi.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000122) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/122_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000123) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/123_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000124) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/124_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000125) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/125_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000126) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/126_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000127) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/127_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000128) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/128_twi.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000129) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/129_twi.csv"; //x=990, y=990 ### FOR B19-T1 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000130) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/130_twi.csv"; //x=990, y=990 ### FOR B19-T2 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000131) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/131_twi.csv"; //x=990, y=990 ### FOR EN18061 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000132) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/132_twi.csv"; //x=990, y=990 ### FOR EN18062 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000133) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/133_twi.csv"; //x=990, y=990 ### FOR EN18063 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000134) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/134_twi.csv"; //x=990, y=990 ### FOR EN18064 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000135) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/135_twi.csv"; //x=990, y=990 ### FOR EN18065 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000136) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/136_twi.csv"; //x=990, y=990 ### FOR EN18066 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000137) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/137_twi.csv"; //x=990, y=990 ### FOR EN18067 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000138) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/138_twi.csv"; //x=990, y=990 ### FOR EN18068 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000139) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/139_twi.csv"; //x=990, y=990 ### FOR EN18069 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000140) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/140_twi.csv"; //x=990, y=990 ### FOR EN18070_centre ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000141) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/141_twi.csv"; //x=990, y=990 ### FOR EN18070_edge ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000142) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/142_twi.csv"; //x=990, y=990 ### FOR EN18070_end ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000143) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/143_twi.csv"; //x=990, y=990 ### FOR EN18070_transition ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000144) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/144_twi.csv"; //x=990, y=990 ### FOR EN18071 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000145) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/145_twi.csv"; //x=990, y=990 ### FOR EN18072 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000146) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/146_twi.csv"; //x=990, y=990 ### FOR EN18073 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000147) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/147_twi.csv"; //x=990, y=990 ### FOR EN18074 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000148) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/148_twi.csv"; //x=990, y=990 ### FOR EN18075 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000149) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/149_twi.csv"; //x=990, y=990 ### FOR EN18076 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000150) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/150_twi.csv"; //x=990, y=990 ### FOR EN18077 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000151) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/151_twi.csv"; //x=990, y=990 ### FOR EN18078 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000152) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/152_twi.csv"; //x=990, y=990 ### FOR EN18079 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000153) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/153_twi.csv"; //x=990, y=990 ### FOR EN18080 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000154) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/154_twi.csv"; //x=990, y=990 ### FOR EN18081 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000155) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/155_twi.csv"; //x=990, y=990 ### FOR EN18082 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000156) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/156_twi.csv"; //x=990, y=990 ### FOR EN18083 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000157) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/157_twi.csv"; //x=990, y=990 ### FOR EN21201 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000158) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/158_twi.csv"; //x=990, y=990 ### FOR EN21202 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000159) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/159_twi.csv"; //x=990, y=990 ### FOR EN21203 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000160) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/160_twi.csv"; //x=990, y=990 ### FOR EN21204 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000161) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/161_twi.csv"; //x=990, y=990 ### FOR EN21205 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000162) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/162_twi.csv"; //x=990, y=990 ### FOR EN21206 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000163) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/163_twi.csv"; //x=990, y=990 ### FOR EN21207 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000164) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/164_twi.csv"; //x=990, y=990 ### FOR EN21208 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000165) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/165_twi.csv"; //x=990, y=990 ### FOR EN21209 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000166) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/166_twi.csv"; //x=990, y=990 ### FOR EN21210 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000167) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/167_twi.csv"; //x=990, y=990 ### FOR EN21211 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000168) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/168_twi.csv"; //x=990, y=990 ### FOR EN21212 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000169) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/169_twi.csv"; //x=990, y=990 ### FOR EN21213 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000170) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/170_twi.csv"; //x=990, y=990 ### FOR EN21214 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000171) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/171_twi.csv"; //x=990, y=990 ### FOR EN21215 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000172) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/172_twi.csv"; //x=990, y=990 ### FOR EN21216 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000173) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/173_twi.csv"; //x=990, y=990 ### FOR EN21217 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000174) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/174_twi.csv"; //x=990, y=990 ### FOR EN21218 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000175) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/175_twi.csv"; //x=990, y=990 ### FOR EN21219 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000176) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/176_twi.csv"; //x=990, y=990 ### FOR EN21220 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000177) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/177_twi.csv"; //x=990, y=990 ### FOR EN21221 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000178) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/178_twi.csv"; //x=990, y=990 ### FOR EN21222 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000179) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/179_twi.csv"; //x=990, y=990 ### FOR EN21223 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000180) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/180_twi.csv"; //x=990, y=990 ### FOR EN21224 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000181) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/181_twi.csv"; //x=990, y=990 ### FOR EN21225 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000182) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/182_twi.csv"; //x=990, y=990 ### FOR EN21226 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000183) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/183_twi.csv"; //x=990, y=990 ### FOR EN21227 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000184) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/184_twi.csv"; //x=990, y=990 ### FOR EN21228 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000185) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/185_twi.csv"; //x=990, y=990 ### FOR EN21229 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000186) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/186_twi.csv"; //x=990, y=990 ### FOR EN21230 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000187) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/187_twi.csv"; //x=990, y=990 ### FOR EN21231 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000188) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/188_twi.csv"; //x=990, y=990 ### FOR EN21232 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000189) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/189_twi.csv"; //x=990, y=990 ### FOR EN21233 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000190) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/190_twi.csv"; //x=990, y=990 ### FOR EN21234 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000191) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/191_twi.csv"; //x=990, y=990 ### FOR EN21235 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000192) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/192_twi.csv"; //x=990, y=990 ### FOR EN21236 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000193) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/193_twi.csv"; //x=990, y=990 ### FOR EN21237 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000194) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/194_twi.csv"; //x=990, y=990 ### FOR EN21238 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000195) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/195_twi.csv"; //x=990, y=990 ### FOR EN21239 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000196) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/196_twi.csv"; //x=990, y=990 ### FOR EN21240 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000197) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/197_twi.csv"; //x=990, y=990 ### FOR EN21241 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000198) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/198_twi.csv"; //x=990, y=990 ### FOR EN21242 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000199) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/199_twi.csv"; //x=990, y=990 ### FOR EN21243 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000200) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/200_twi.csv"; //x=990, y=990 ### FOR EN21244 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000201) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/201_twi.csv"; //x=990, y=990 ### FOR EN21245 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000202) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/202_twi.csv"; //x=990, y=990 ### FOR EN21246 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000203) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/203_twi.csv"; //x=990, y=990 ### FOR EN21247 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000204) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/204_twi.csv"; //x=990, y=990 ### FOR EN21248 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000205) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/205_twi.csv"; //x=990, y=990 ### FOR EN21249 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000206) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/206_twi.csv"; //x=990, y=990 ### FOR EN21250 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000207) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/207_twi.csv"; //x=990, y=990 ### FOR EN21251 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000208) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/208_twi.csv"; //x=990, y=990 ### FOR EN21252 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000209) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/209_twi.csv"; //x=990, y=990 ### FOR EN21253 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000210) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/210_twi.csv"; //x=990, y=990 ### FOR EN21254 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000211) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/211_twi.csv"; //x=990, y=990 ### FOR EN21255 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000212) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/212_twi.csv"; //x=990, y=990 ### FOR EN21256 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000213) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/213_twi.csv"; //x=990, y=990 ### FOR EN21257 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000214) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/214_twi.csv"; //x=990, y=990 ### FOR EN21258 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000215) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/215_twi.csv"; //x=990, y=990 ### FOR EN21259 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000216) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/216_twi.csv"; //x=990, y=990 ### FOR EN21260 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000217) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/217_twi.csv"; //x=990, y=990 ### FOR EN21261 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000218) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/218_twi.csv"; //x=990, y=990 ### FOR EN21262 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000219) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/219_twi.csv"; //x=990, y=990 ### FOR EN22-001 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000220) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/220_twi.csv"; //x=990, y=990 ### FOR EN22-002 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000221) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/221_twi.csv"; //x=990, y=990 ### FOR EN22-003 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000222) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/222_twi.csv"; //x=990, y=990 ### FOR EN22-004 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000223) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/223_twi.csv"; //x=990, y=990 ### FOR EN22-005 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000224) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/224_twi.csv"; //x=990, y=990 ### FOR EN22-006 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000225) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/225_twi.csv"; //x=990, y=990 ### FOR EN22-007 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000226) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/226_twi.csv"; //x=990, y=990 ### FOR EN22-008 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000227) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/227_twi.csv"; //x=990, y=990 ### FOR EN22-009 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000228) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/228_twi.csv"; //x=990, y=990 ### FOR EN22-010 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000229) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/229_twi.csv"; //x=990, y=990 ### FOR EN22-011 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000230) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/230_twi.csv"; //x=990, y=990 ### FOR EN22-012 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000231) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/231_twi.csv"; //x=990, y=990 ### FOR EN22-013 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000232) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/232_twi.csv"; //x=990, y=990 ### FOR EN22-014 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000233) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/233_twi.csv"; //x=990, y=990 ### FOR EN22-015 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000234) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/234_twi.csv"; //x=990, y=990 ### FOR EN22-016 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000235) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/235_twi.csv"; //x=990, y=990 ### FOR EN22-017 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000236) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/236_twi.csv"; //x=990, y=990 ### FOR EN22-018 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000237) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/237_twi.csv"; //x=990, y=990 ### FOR EN22-019 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000238) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/238_twi.csv"; //x=990, y=990 ### FOR EN22-020 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000239) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/239_twi.csv"; //x=990, y=990 ### FOR EN22-021 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000240) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/240_twi.csv"; //x=990, y=990 ### FOR EN22-022 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000241) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/241_twi.csv"; //x=990, y=990 ### FOR EN22-023 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000242) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/242_twi.csv"; //x=990, y=990 ### FOR EN22-024 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000243) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/243_twi.csv"; //x=990, y=990 ### FOR EN22-025 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000244) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/244_twi.csv"; //x=990, y=990 ### FOR EN22-026 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000245) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/245_twi.csv"; //x=990, y=990 ### FOR EN22-027 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000246) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/246_twi.csv"; //x=990, y=990 ### FOR EN22-028 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000247) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/247_twi.csv"; //x=990, y=990 ### FOR EN22-029 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000248) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/248_twi.csv"; //x=990, y=990 ### FOR EN22-030 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000249) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/249_twi.csv"; //x=990, y=990 ### FOR EN22-031 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000250) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/250_twi.csv"; //x=990, y=990 ### FOR EN22-032 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000251) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/251_twi.csv"; //x=990, y=990 ### FOR EN22-033 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000252) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/252_twi.csv"; //x=990, y=990 ### FOR EN22-034 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000253) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/253_twi.csv"; //x=990, y=990 ### FOR EN22-035 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000254) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/254_twi.csv"; //x=990, y=990 ### FOR EN22-036 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000255) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/255_twi.csv"; //x=990, y=990 ### FOR EN22-037 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000256) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/256_twi.csv"; //x=990, y=990 ### FOR EN22-038 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000257) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/257_twi.csv"; //x=990, y=990 ### FOR EN22-039 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000258) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/258_twi.csv"; //x=990, y=990 ### FOR EN22-040 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000259) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/259_twi.csv"; //x=990, y=990 ### FOR EN22-041 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000260) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/260_twi.csv"; //x=990, y=990 ### FOR EN22-042 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000261) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/261_twi.csv"; //x=990, y=990 ### FOR EN22-043 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000262) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/262_twi.csv"; //x=990, y=990 ### FOR EN22-044 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000263) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/263_twi.csv"; //x=990, y=990 ### FOR EN22-045 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000264) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/264_twi.csv"; //x=990, y=990 ### FOR EN22-046 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000265) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/265_twi.csv"; //x=990, y=990 ### FOR EN22-047 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000266) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/266_twi.csv"; //x=990, y=990 ### FOR EN22-048 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000267) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/267_twi.csv"; //x=990, y=990 ### FOR EN22-049 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000268) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/268_twi.csv"; //x=990, y=990 ### FOR EN22-050 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000269) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/269_twi.csv"; //x=990, y=990 ### FOR EN22-051 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000270) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/270_twi.csv"; //x=990, y=990 ### FOR EN22-052 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000271) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/271_twi.csv"; //x=990, y=990 ### FOR EN22-053 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000272) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/272_twi.csv"; //x=990, y=990 ### FOR EN22-054 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000273) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/273_twi.csv"; //x=990, y=990 ### FOR EN22-055 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000274) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/274_twi.csv"; //x=990, y=990 ### FOR EN22-056 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000275) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/275_twi.csv"; //x=990, y=990 ### FOR EN22-057 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000276) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/276_twi.csv"; //x=990, y=990 ### FOR EN22-058 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000277) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/277_twi.csv"; //x=990, y=990 ### FOR EN22-059 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000278) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/278_twi.csv"; //x=990, y=990 ### FOR EN22-060 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000279) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/279_twi.csv"; //x=990, y=990 ### FOR EN22-061 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000280) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/280_twi.csv"; //x=990, y=990 ### FOR EN22-062 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000281) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/281_twi.csv"; //x=990, y=990 ### FOR EN22-063 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000282) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/282_twi.csv"; //x=990, y=990 ### FOR EN22-064 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000283) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/283_twi.csv"; //x=990, y=990 ### FOR EN22-065 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000284) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/284_twi.csv"; //x=990, y=990 ### FOR EN22-066 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000285) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/285_twi.csv"; //x=990, y=990 ### FOR EN22-067 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000286) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/286_twi.csv"; //x=990, y=990 ### FOR EN22-068 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000287) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/287_twi.csv"; //x=990, y=990 ### FOR EN22-069 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000288) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/288_twi.csv"; //x=990, y=990 ### FOR EN22-070 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000289) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/289_twi.csv"; //x=990, y=990 ### FOR EN22-071 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000290) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/290_twi.csv"; //x=990, y=990 ### FOR EN22-072 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000291) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/291_twi.csv"; //x=990, y=990 ### FOR EN22-073 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000292) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/292_twi.csv"; //x=990, y=990 ### FOR EN22-074 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000293) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/293_twi.csv"; //x=990, y=990 ### FOR EN22-075 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000294) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/294_twi.csv"; //x=990, y=990 ### FOR TY04VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000295) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/295_twi.csv"; //x=990, y=990 ### FOR TY04VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000296) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/296_twi.csv"; //x=990, y=990 ### FOR TY07VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000297) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/297_twi.csv"; //x=990, y=990 ### FOR TY07VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000298) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/298_twi.csv"; //x=990, y=990 ### FOR TY09VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 1000000299) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/299_twi.csv"; //x=990, y=990 ### FOR TY09VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000001) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/001_twi.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000002) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/002_twi.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000003) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/003_twi.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000004) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/004_twi.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000005) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/005_twi.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000006) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/006_twi.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000007) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/007_twi.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000008) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/008_twi.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000009) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/009_twi.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000010) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/010_twi.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000011) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/011_twi.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000012) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/012_twi.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000013) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/013_twi.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000014) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/014_twi.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000015) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/015_twi.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000016) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/016_twi.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000017) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/017_twi.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000018) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/018_twi.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000019) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/019_twi.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000020) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/020_twi.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000021) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/021_twi.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000022) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/022_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000023) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/023_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000024) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/024_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000025) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/025_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000026) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/026_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000027) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/027_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000028) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/028_twi.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000029) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/029_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000030) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/030_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000031) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/031_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000032) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/032_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000033) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/033_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000034) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/034_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000035) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/035_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000036) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/036_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000037) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/037_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000038) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/038_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000039) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/039_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000040) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/040_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000041) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/041_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000042) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/042_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000043) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/043_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000044) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/044_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000045) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/045_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000046) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/046_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000047) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/047_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000048) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/048_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000049) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/049_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000050) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/050_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000051) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/051_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000052) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/052_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000053) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/053_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000054) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/054_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000055) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/055_twi.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000056) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/056_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000057) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/057_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000058) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/058_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000059) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/059_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000060) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/060_twi.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000061) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/061_twi.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000062) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/062_twi.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000063) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/063_twi.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000064) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/064_twi.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000065) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/065_twi.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000066) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/066_twi.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000067) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/067_twi.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000068) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/068_twi.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000069) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/069_twi.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000070) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/070_twi.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000071) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/071_twi.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000072) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/072_twi.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000073) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/073_twi.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000074) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/074_twi.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000075) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/075_twi.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000076) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/076_twi.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000077) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/077_twi.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000078) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/078_twi.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000079) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/079_twi.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000080) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/080_twi.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000081) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/081_twi.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000082) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/082_twi.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000083) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/083_twi.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000084) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/084_twi.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000085) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/085_twi.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000086) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/086_twi.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000087) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/087_twi.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000088) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/088_twi.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000089) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/089_twi.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000090) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/090_twi.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000091) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/091_twi.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000092) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/092_twi.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000093) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/093_twi.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000094) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/094_twi.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000095) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/095_twi.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000096) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/096_twi.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000097) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/097_twi.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000098) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/098_twi.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000099) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/099_twi.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000100) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/100_twi.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000101) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/101_twi.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000102) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/102_twi.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000103) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/103_twi.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000104) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/104_twi.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000105) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/105_twi.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000106) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/106_twi.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000107) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/107_twi.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000108) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/108_twi.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000109) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/109_twi.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000110) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/110_twi.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000111) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/111_twi.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000112) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/112_twi.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000113) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/113_twi.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000114) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/114_twi.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000115) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/115_twi.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000116) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/116_twi.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000117) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/117_twi.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000118) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/118_twi.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000119) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/119_twi.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000120) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/120_twi.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000121) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/121_twi.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000122) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/122_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000123) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/123_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000124) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/124_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000125) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/125_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000126) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/126_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000127) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/127_twi.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000128) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/128_twi.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000129) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/129_twi.csv"; //x=990, y=990 ### FOR B19-T1 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000130) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/130_twi.csv"; //x=990, y=990 ### FOR B19-T2 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000131) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/131_twi.csv"; //x=990, y=990 ### FOR EN18061 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000132) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/132_twi.csv"; //x=990, y=990 ### FOR EN18062 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000133) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/133_twi.csv"; //x=990, y=990 ### FOR EN18063 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000134) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/134_twi.csv"; //x=990, y=990 ### FOR EN18064 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000135) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/135_twi.csv"; //x=990, y=990 ### FOR EN18065 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000136) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/136_twi.csv"; //x=990, y=990 ### FOR EN18066 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000137) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/137_twi.csv"; //x=990, y=990 ### FOR EN18067 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000138) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/138_twi.csv"; //x=990, y=990 ### FOR EN18068 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000139) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/139_twi.csv"; //x=990, y=990 ### FOR EN18069 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000140) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/140_twi.csv"; //x=990, y=990 ### FOR EN18070_centre ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000141) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/141_twi.csv"; //x=990, y=990 ### FOR EN18070_edge ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000142) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/142_twi.csv"; //x=990, y=990 ### FOR EN18070_end ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000143) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/143_twi.csv"; //x=990, y=990 ### FOR EN18070_transition ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000144) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/144_twi.csv"; //x=990, y=990 ### FOR EN18071 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000145) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/145_twi.csv"; //x=990, y=990 ### FOR EN18072 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000146) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/146_twi.csv"; //x=990, y=990 ### FOR EN18073 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000147) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/147_twi.csv"; //x=990, y=990 ### FOR EN18074 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000148) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/148_twi.csv"; //x=990, y=990 ### FOR EN18075 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000149) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/149_twi.csv"; //x=990, y=990 ### FOR EN18076 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000150) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/150_twi.csv"; //x=990, y=990 ### FOR EN18077 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000151) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/151_twi.csv"; //x=990, y=990 ### FOR EN18078 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000152) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/152_twi.csv"; //x=990, y=990 ### FOR EN18079 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000153) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/153_twi.csv"; //x=990, y=990 ### FOR EN18080 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000154) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/154_twi.csv"; //x=990, y=990 ### FOR EN18081 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000155) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/155_twi.csv"; //x=990, y=990 ### FOR EN18082 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000156) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/156_twi.csv"; //x=990, y=990 ### FOR EN18083 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000157) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/157_twi.csv"; //x=990, y=990 ### FOR EN21201 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000158) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/158_twi.csv"; //x=990, y=990 ### FOR EN21202 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000159) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/159_twi.csv"; //x=990, y=990 ### FOR EN21203 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000160) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/160_twi.csv"; //x=990, y=990 ### FOR EN21204 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000161) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/161_twi.csv"; //x=990, y=990 ### FOR EN21205 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000162) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/162_twi.csv"; //x=990, y=990 ### FOR EN21206 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000163) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/163_twi.csv"; //x=990, y=990 ### FOR EN21207 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000164) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/164_twi.csv"; //x=990, y=990 ### FOR EN21208 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000165) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/165_twi.csv"; //x=990, y=990 ### FOR EN21209 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000166) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/166_twi.csv"; //x=990, y=990 ### FOR EN21210 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000167) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/167_twi.csv"; //x=990, y=990 ### FOR EN21211 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000168) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/168_twi.csv"; //x=990, y=990 ### FOR EN21212 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000169) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/169_twi.csv"; //x=990, y=990 ### FOR EN21213 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000170) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/170_twi.csv"; //x=990, y=990 ### FOR EN21214 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000171) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/171_twi.csv"; //x=990, y=990 ### FOR EN21215 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000172) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/172_twi.csv"; //x=990, y=990 ### FOR EN21216 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000173) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/173_twi.csv"; //x=990, y=990 ### FOR EN21217 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000174) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/174_twi.csv"; //x=990, y=990 ### FOR EN21218 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000175) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/175_twi.csv"; //x=990, y=990 ### FOR EN21219 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000176) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/176_twi.csv"; //x=990, y=990 ### FOR EN21220 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000177) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/177_twi.csv"; //x=990, y=990 ### FOR EN21221 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000178) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/178_twi.csv"; //x=990, y=990 ### FOR EN21222 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000179) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/179_twi.csv"; //x=990, y=990 ### FOR EN21223 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000180) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/180_twi.csv"; //x=990, y=990 ### FOR EN21224 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000181) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/181_twi.csv"; //x=990, y=990 ### FOR EN21225 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000182) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/182_twi.csv"; //x=990, y=990 ### FOR EN21226 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000183) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/183_twi.csv"; //x=990, y=990 ### FOR EN21227 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000184) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/184_twi.csv"; //x=990, y=990 ### FOR EN21228 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000185) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/185_twi.csv"; //x=990, y=990 ### FOR EN21229 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000186) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/186_twi.csv"; //x=990, y=990 ### FOR EN21230 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000187) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/187_twi.csv"; //x=990, y=990 ### FOR EN21231 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000188) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/188_twi.csv"; //x=990, y=990 ### FOR EN21232 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000189) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/189_twi.csv"; //x=990, y=990 ### FOR EN21233 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000190) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/190_twi.csv"; //x=990, y=990 ### FOR EN21234 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000191) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/191_twi.csv"; //x=990, y=990 ### FOR EN21235 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000192) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/192_twi.csv"; //x=990, y=990 ### FOR EN21236 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000193) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/193_twi.csv"; //x=990, y=990 ### FOR EN21237 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000194) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/194_twi.csv"; //x=990, y=990 ### FOR EN21238 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000195) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/195_twi.csv"; //x=990, y=990 ### FOR EN21239 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000196) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/196_twi.csv"; //x=990, y=990 ### FOR EN21240 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000197) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/197_twi.csv"; //x=990, y=990 ### FOR EN21241 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000198) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/198_twi.csv"; //x=990, y=990 ### FOR EN21242 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000199) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/199_twi.csv"; //x=990, y=990 ### FOR EN21243 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000200) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/200_twi.csv"; //x=990, y=990 ### FOR EN21244 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000201) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/201_twi.csv"; //x=990, y=990 ### FOR EN21245 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000202) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/202_twi.csv"; //x=990, y=990 ### FOR EN21246 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000203) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/203_twi.csv"; //x=990, y=990 ### FOR EN21247 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000204) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/204_twi.csv"; //x=990, y=990 ### FOR EN21248 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000205) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/205_twi.csv"; //x=990, y=990 ### FOR EN21249 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000206) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/206_twi.csv"; //x=990, y=990 ### FOR EN21250 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000207) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/207_twi.csv"; //x=990, y=990 ### FOR EN21251 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000208) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/208_twi.csv"; //x=990, y=990 ### FOR EN21252 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000209) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/209_twi.csv"; //x=990, y=990 ### FOR EN21253 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000210) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/210_twi.csv"; //x=990, y=990 ### FOR EN21254 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000211) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/211_twi.csv"; //x=990, y=990 ### FOR EN21255 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000212) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/212_twi.csv"; //x=990, y=990 ### FOR EN21256 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000213) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/213_twi.csv"; //x=990, y=990 ### FOR EN21257 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000214) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/214_twi.csv"; //x=990, y=990 ### FOR EN21258 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000215) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/215_twi.csv"; //x=990, y=990 ### FOR EN21259 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000216) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/216_twi.csv"; //x=990, y=990 ### FOR EN21260 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000217) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/217_twi.csv"; //x=990, y=990 ### FOR EN21261 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000218) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/218_twi.csv"; //x=990, y=990 ### FOR EN21262 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000219) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/219_twi.csv"; //x=990, y=990 ### FOR EN22-001 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000220) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/220_twi.csv"; //x=990, y=990 ### FOR EN22-002 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000221) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/221_twi.csv"; //x=990, y=990 ### FOR EN22-003 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000222) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/222_twi.csv"; //x=990, y=990 ### FOR EN22-004 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000223) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/223_twi.csv"; //x=990, y=990 ### FOR EN22-005 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000224) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/224_twi.csv"; //x=990, y=990 ### FOR EN22-006 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000225) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/225_twi.csv"; //x=990, y=990 ### FOR EN22-007 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000226) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/226_twi.csv"; //x=990, y=990 ### FOR EN22-008 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000227) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/227_twi.csv"; //x=990, y=990 ### FOR EN22-009 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000228) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/228_twi.csv"; //x=990, y=990 ### FOR EN22-010 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000229) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/229_twi.csv"; //x=990, y=990 ### FOR EN22-011 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000230) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/230_twi.csv"; //x=990, y=990 ### FOR EN22-012 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000231) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/231_twi.csv"; //x=990, y=990 ### FOR EN22-013 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000232) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/232_twi.csv"; //x=990, y=990 ### FOR EN22-014 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000233) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/233_twi.csv"; //x=990, y=990 ### FOR EN22-015 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000234) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/234_twi.csv"; //x=990, y=990 ### FOR EN22-016 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000235) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/235_twi.csv"; //x=990, y=990 ### FOR EN22-017 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000236) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/236_twi.csv"; //x=990, y=990 ### FOR EN22-018 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000237) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/237_twi.csv"; //x=990, y=990 ### FOR EN22-019 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000238) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/238_twi.csv"; //x=990, y=990 ### FOR EN22-020 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000239) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/239_twi.csv"; //x=990, y=990 ### FOR EN22-021 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000240) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/240_twi.csv"; //x=990, y=990 ### FOR EN22-022 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000241) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/241_twi.csv"; //x=990, y=990 ### FOR EN22-023 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000242) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/242_twi.csv"; //x=990, y=990 ### FOR EN22-024 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000243) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/243_twi.csv"; //x=990, y=990 ### FOR EN22-025 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000244) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/244_twi.csv"; //x=990, y=990 ### FOR EN22-026 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000245) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/245_twi.csv"; //x=990, y=990 ### FOR EN22-027 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000246) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/246_twi.csv"; //x=990, y=990 ### FOR EN22-028 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000247) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/247_twi.csv"; //x=990, y=990 ### FOR EN22-029 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000248) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/248_twi.csv"; //x=990, y=990 ### FOR EN22-030 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000249) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/249_twi.csv"; //x=990, y=990 ### FOR EN22-031 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000250) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/250_twi.csv"; //x=990, y=990 ### FOR EN22-032 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000251) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/251_twi.csv"; //x=990, y=990 ### FOR EN22-033 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000252) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/252_twi.csv"; //x=990, y=990 ### FOR EN22-034 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000253) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/253_twi.csv"; //x=990, y=990 ### FOR EN22-035 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000254) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/254_twi.csv"; //x=990, y=990 ### FOR EN22-036 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000255) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/255_twi.csv"; //x=990, y=990 ### FOR EN22-037 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000256) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/256_twi.csv"; //x=990, y=990 ### FOR EN22-038 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000257) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/257_twi.csv"; //x=990, y=990 ### FOR EN22-039 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000258) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/258_twi.csv"; //x=990, y=990 ### FOR EN22-040 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000259) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/259_twi.csv"; //x=990, y=990 ### FOR EN22-041 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000260) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/260_twi.csv"; //x=990, y=990 ### FOR EN22-042 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000261) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/261_twi.csv"; //x=990, y=990 ### FOR EN22-043 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000262) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/262_twi.csv"; //x=990, y=990 ### FOR EN22-044 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000263) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/263_twi.csv"; //x=990, y=990 ### FOR EN22-045 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000264) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/264_twi.csv"; //x=990, y=990 ### FOR EN22-046 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000265) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/265_twi.csv"; //x=990, y=990 ### FOR EN22-047 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000266) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/266_twi.csv"; //x=990, y=990 ### FOR EN22-048 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000267) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/267_twi.csv"; //x=990, y=990 ### FOR EN22-049 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000268) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/268_twi.csv"; //x=990, y=990 ### FOR EN22-050 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000269) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/269_twi.csv"; //x=990, y=990 ### FOR EN22-051 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000270) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/270_twi.csv"; //x=990, y=990 ### FOR EN22-052 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000271) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/271_twi.csv"; //x=990, y=990 ### FOR EN22-053 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000272) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/272_twi.csv"; //x=990, y=990 ### FOR EN22-054 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000273) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/273_twi.csv"; //x=990, y=990 ### FOR EN22-055 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000274) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/274_twi.csv"; //x=990, y=990 ### FOR EN22-056 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000275) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/275_twi.csv"; //x=990, y=990 ### FOR EN22-057 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000276) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/276_twi.csv"; //x=990, y=990 ### FOR EN22-058 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000277) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/277_twi.csv"; //x=990, y=990 ### FOR EN22-059 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000278) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/278_twi.csv"; //x=990, y=990 ### FOR EN22-060 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000279) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/279_twi.csv"; //x=990, y=990 ### FOR EN22-061 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000280) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/280_twi.csv"; //x=990, y=990 ### FOR EN22-062 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000281) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/281_twi.csv"; //x=990, y=990 ### FOR EN22-063 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000282) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/282_twi.csv"; //x=990, y=990 ### FOR EN22-064 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000283) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/283_twi.csv"; //x=990, y=990 ### FOR EN22-065 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000284) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/284_twi.csv"; //x=990, y=990 ### FOR EN22-066 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000285) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/285_twi.csv"; //x=990, y=990 ### FOR EN22-067 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000286) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/286_twi.csv"; //x=990, y=990 ### FOR EN22-068 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000287) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/287_twi.csv"; //x=990, y=990 ### FOR EN22-069 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000288) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/288_twi.csv"; //x=990, y=990 ### FOR EN22-070 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000289) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/289_twi.csv"; //x=990, y=990 ### FOR EN22-071 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000290) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/290_twi.csv"; //x=990, y=990 ### FOR EN22-072 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000291) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/291_twi.csv"; //x=990, y=990 ### FOR EN22-073 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000292) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/292_twi.csv"; //x=990, y=990 ### FOR EN22-074 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000293) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/293_twi.csv"; //x=990, y=990 ### FOR EN22-075 ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000294) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/294_twi.csv"; //x=990, y=990 ### FOR TY04VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000295) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/295_twi.csv"; //x=990, y=990 ### FOR TY04VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000296) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/296_twi.csv"; //x=990, y=990 ### FOR TY07VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000297) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/297_twi.csv"; //x=990, y=990 ### FOR TY07VII ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000298) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/298_twi.csv"; //x=990, y=990 ### FOR TY09VI ###
			strcpy(twifilename, twiinputbuf);
		} else if (parameter[0].weatherchoice == 2000000299) {
			char twiinputbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_environmental_data/299_twi.csv"; //x=990, y=990 ### FOR TY09VII ###
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
