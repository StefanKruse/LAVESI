#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

void Dataoutput(int t,
                int jahr,
                Parameter* parameter,
                int yearposition,
                vector<VectorList<Tree>>& world_tree_list,
                vector<VectorList<Seed>>& world_seed_list,
                vector<vector<Weather>>& world_weather_list,
                vector<vector<Envirgrid>>& world_plot_list,
                vector<vector<Evaluation>>& world_evaluation_list) {
    double xminwindow = 0.0;
    double xmaxwindow = 0.0;
    double yminwindow = 0.0;
    double ymaxwindow = 0.0;

    FILE* filepointer;
    string dateiname;

    if (parameter[0].outputall == 0) {
        xminwindow = ((double)treecols / 2.0) - 10.0;
        xmaxwindow = ((double)treecols / 2.0) + 10.0;
        yminwindow = ((double)treerows / 2.0) - 10.0;
        ymaxwindow = ((double)treerows / 2.0) + 10.0;
    } else if (parameter[0].outputall == 1) {
        xminwindow = ((double)0.0);
        xmaxwindow = ((double)treecols);
        yminwindow = ((double)0.0);
        ymaxwindow = ((double)treerows);
    }

    // declarations of switches for output
    bool outputcurrencies = false;
    bool outputposition = false;
    bool outputindividuals = false;
    bool outputgriddedbiomass = false;
    bool outputtestarea = false;
    bool outputtransects = false;
    bool ausgabedensity = false;

    // preprocessing and output of data for each plot
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {  // world tree list loop
        VectorList<Tree>& tree_list = *posw;

        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;

        vector<vector<Envirgrid>>::iterator poskarten = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *poskarten;

        vector<vector<Weather>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather>& weather_list = *posiwelt;

        vector<vector<Evaluation>>::iterator posiweltausw = (world_evaluation_list.begin() + aktort);
        vector<Evaluation>& evaluation_list = *posiweltausw;
        vector<Evaluation>::iterator posausw = evaluation_list.begin();
        auto& pEvaluation = (*posausw);

        aktort++;

        // calculation of the current location in a world grid // currently not used
        // int aktortyworldcoo = (aktort - 1) / parameter[0].mapxlength;
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);


        // define output based on parameter setting
        if (parameter[0].dataoutput == true) {
            if (parameter[0].outputmode == 0) {  // "full"
                if (parameter[0].spinupphase == true) {
                    outputcurrencies = true;
                    outputposition = true;
                } else {
                    outputcurrencies = true;
                    outputposition = true;
                    outputindividuals = true;
                    ausgabedensity = true;
                }
            } else if (parameter[0].outputmode == 1) {  // "normal,gridded"
                outputcurrencies = true;

                if (parameter[0].ivort == 1)  // write full Envirgrid once on sim start
                    ausgabedensity = true;

                if (parameter[0].ivort % 20 == 0) {
                    outputgriddedbiomass = true;
                }
            } else if (parameter[0].outputmode == 11) {  // "normal,gridded,large area"
               
                if ((parameter[0].ivort % 100 == 0) || ((parameter[0].ivort >= 1500) && (parameter[0].ivort % 5 == 0)))
					outputcurrencies = true;
				
				if(parameter[0].ivort == 1)// write full Envirgrid once on sim start
					ausgabedensity = true;
					
				if(parameter[0].ivort == 2)// test
					outputgriddedbiomass = true;

                if ( ( (parameter[0].ivort == 1300) // 1800
						| (parameter[0].ivort == 1360) // 1860
						| (parameter[0].ivort == 1400) // 1900
						| (parameter[0].ivort == 1490) // 1990
						| (parameter[0].ivort >= 1500) // 2000
					  )
					  && (parameter[0].ivort % 5 == 0)
					) {
					outputgriddedbiomass = true;
                }
				
                if ((parameter[0].ivort % 20 == 0)
						| (parameter[0].ivort >= 1500)) {
					outputtestarea=true; // area of 200x200 m with full tree output 
                }
            } else if (parameter[0].outputmode == 13) {  // "normal,gridded,large area"
                outputcurrencies = true;
				
				if(parameter[0].ivort == 1)// write full Envirgrid once on sim start
					ausgabedensity = true;
					
				if(parameter[0].ivort % 50 == 0)// test
					outputgriddedbiomass = true;

            } else if (parameter[0].outputmode == 12) {  // "normal,gridded,large area Khamra"
                outputcurrencies = true;

				if(parameter[0].ivort == 1)// write full Envirgrid once on sim start
					ausgabedensity = true;
					
				outputgriddedbiomass = true; // each sim step!
				
            } else if (parameter[0].outputmode == 2) {  // "OMP"
                outputcurrencies = true;
            } else if (parameter[0].outputmode == 3) {  // "transect"
                outputcurrencies = true;

                if ((parameter[0].ivort % 100 == 0) || ((parameter[0].ivort >= 1500) && (parameter[0].ivort % 10 == 0))) {
                    outputtransects = true;
                }
            }
        }  // define output

        ostringstream s1, s2, s3, s4, s5, s6, s7, s8;

        if (outputcurrencies == true) {  // currencies output
        // update evaluation lists
        // declarations
        double basalarea = 0.0;
        int nheight0b40 = 0, nheight41b200 = 0, nheight201b10000 = 0;
        double breastdiameter = 0.0;
        int breastdiametercount = 0;
        int stemcount = 0;
        double meantreeheight = 0.0, meantreeage = 0.0;

#pragma omp parallel default(shared)
        {
            // declarations for each thread
            double local_basalarea = 0.0;
            int local_nheight0b40 = 0, local_nheight41b200 = 0, local_nheight201b10000 = 0;
            double local_breastdiameter = 0.0;
            int local_breastdiametercount = 0;
            int local_stemcount = 0;
            double local_meantreeheight = 0.0, local_meantreeage = 0.0;

#pragma omp for schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];

				if (tree.growing == true) {
					if (((double)tree.xcoo / 1000 >= xminwindow) && ((double)tree.xcoo / 1000 <= xmaxwindow) && ((double)tree.ycoo / 1000 >= yminwindow)
						&& ((double)tree.ycoo / 1000 <= ymaxwindow)) {  // loop over reduced plot
						// basal area as population size identifier
						if ((double)tree.height / 100 >= 130) {
							local_basalarea += (M_PI * pow((tree.dbreast / 2), 2));
							local_stemcount++;
						}

						// population sizes in height classes
						if ((double)tree.height / 100 <= 40) {
							local_nheight0b40++;
						} else if (((double)tree.height / 100 > 40) && ((double)tree.height / 100 <= 200)) {
							local_nheight41b200++;
						} else if ((double)tree.height / 100 > 200) {
							local_nheight201b10000++;
							local_meantreeheight += (double)tree.height / 100;
							local_meantreeage += (double)tree.age;
						}

						if (tree.dbreast > 0) {
							local_breastdiameter += tree.dbreast;
							local_breastdiametercount++;
						}
					}
				}
            }

#pragma omp critical
            {
                basalarea += local_basalarea;
                nheight0b40 += local_nheight0b40;
                nheight41b200 += local_nheight41b200;
                nheight201b10000 += local_nheight201b10000;
                breastdiameter += local_breastdiameter;
                breastdiametercount += local_breastdiametercount;
                stemcount += local_stemcount;
                meantreeheight += local_meantreeheight;
                meantreeage += local_meantreeage;
            }

        }  // pragma

        // push back values in evaluation list
        pEvaluation.basalarealist.push_back(basalarea);
        pEvaluation.nheight0b40liste.push_back(nheight0b40);
        pEvaluation.nheight41b200liste.push_back(nheight41b200);
        pEvaluation.nheight201b10000liste.push_back(nheight201b10000);

        if (breastdiametercount > 0) {
            pEvaluation.meanbreastdiameterliste.push_back((breastdiameter / ((double)breastdiametercount)));
        } else {
            pEvaluation.meanbreastdiameterliste.push_back(0.0);
        }

        pEvaluation.stemcountliste.push_back(stemcount);

        if (nheight201b10000 > 0) {
            pEvaluation.meantreeheightliste.push_back(meantreeheight / (double)nheight201b10000);
            pEvaluation.meantreeageliste.push_back(meantreeage / (double)nheight201b10000);
        } else {
            pEvaluation.meantreeheightliste.push_back(0.0);
            pEvaluation.meantreeageliste.push_back(0.0);
        }

        // mean value calculation
        int basalareaanzahl = pEvaluation.basalarealist.size();

        double meanbasalarea = 0;
        double meannheight0b40 = 0, meannheight41b200 = 0, meannheight201b10000 = 0;
        double meanbreastdiameter = 0;
        double meanstemcount = 0;
        double meanmeantreeheigth = 0;
        double meanmeantreeage = 0;
        if (basalareaanzahl == (int)parameter[0].stabilmovingwindow) {  // if exactly parameter[0].stabilmovingwindow values are set
            for (int position = basalareaanzahl - (int)parameter[0].stabilmovingwindow; position < basalareaanzahl; ++position) {
                meanbasalarea += pEvaluation.basalarealist[position] / parameter[0].stabilmovingwindow;
                meannheight0b40 += pEvaluation.nheight0b40liste[position] / parameter[0].stabilmovingwindow;
                meannheight41b200 += pEvaluation.nheight41b200liste[position] / parameter[0].stabilmovingwindow;
                meannheight201b10000 += pEvaluation.nheight201b10000liste[position] / parameter[0].stabilmovingwindow;
                meanbreastdiameter += pEvaluation.meanbreastdiameterliste[position] / parameter[0].stabilmovingwindow;
                meanstemcount += pEvaluation.stemcountliste[position] / parameter[0].stabilmovingwindow;
                meanmeantreeheigth += pEvaluation.meantreeheightliste[position] / parameter[0].stabilmovingwindow;
                meanmeantreeage += pEvaluation.meantreeageliste[position] / parameter[0].stabilmovingwindow;
            }
            pEvaluation.basalarearunmeanlist.push_back(meanbasalarea);
            pEvaluation.nheight0b40runmeanliste.push_back(meannheight0b40);
            pEvaluation.nheight41b200runmeanliste.push_back(meannheight41b200);
            pEvaluation.nheight201b10000runmeanliste.push_back(meannheight201b10000);
            pEvaluation.meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);
            pEvaluation.stemcountrunmeanliste.push_back(meanstemcount);
            pEvaluation.meantreeheightrunmeanliste.push_back(meanmeantreeheigth);
            pEvaluation.meantreeagerunmeanliste.push_back(meanmeantreeage);
        } else if (basalareaanzahl > (int)parameter[0].stabilmovingwindow) {  // if more than parameter[0].stabilmovingwindow values are set
            meanbasalarea = pEvaluation.basalarearunmeanlist[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                            - (pEvaluation.basalarealist[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                            + (pEvaluation.basalarealist[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.basalarearunmeanlist.push_back(meanbasalarea);

            meannheight0b40 = pEvaluation.nheight0b40runmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                              - (pEvaluation.nheight0b40liste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                              + (pEvaluation.nheight0b40liste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.nheight0b40runmeanliste.push_back(meannheight0b40);

            meannheight41b200 =
                pEvaluation.nheight41b200runmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation.nheight41b200liste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation.nheight41b200liste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.nheight41b200runmeanliste.push_back(meannheight41b200);

            meannheight201b10000 =
                pEvaluation.nheight201b10000runmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation.nheight201b10000liste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation.nheight201b10000liste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.nheight201b10000runmeanliste.push_back(meannheight201b10000);

            meanbreastdiameter =
                pEvaluation.meanbreastdiameterrunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation.meanbreastdiameterliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation.meanbreastdiameterliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);

            meanstemcount = (double)pEvaluation.stemcountrunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                            - (pEvaluation.stemcountliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                            + (pEvaluation.stemcountliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.stemcountrunmeanliste.push_back(meanstemcount);

            meanmeantreeheigth =
                pEvaluation.meantreeheightrunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation.meantreeheightliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation.meantreeheightliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.meantreeheightrunmeanliste.push_back(meanmeantreeheigth);

            meanmeantreeage = pEvaluation.meantreeagerunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                              - (pEvaluation.meantreeageliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                              + (pEvaluation.meantreeageliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation.meantreeagerunmeanliste.push_back(meanmeantreeage);
        }

        // processing basal area mean values
        int runmeanbasalareaanzahl = pEvaluation.basalarearunmeanlist.size();
        if (runmeanbasalareaanzahl > 1) {  // if more than 2 parameter[0].stabilmovingwindow yearly mean values

            // calculation of the slope
            double slopediff = pEvaluation.basalarearunmeanlist[runmeanbasalareaanzahl - 1] - pEvaluation.basalarearunmeanlist[runmeanbasalareaanzahl - 2];
            if (pEvaluation.postyearofturningpoint != true) {
                // if a maximal slope is not surpassed in 50 years, the turning point of growth is reached
                if (slopediff >= pEvaluation.maxincrementbasalarea) {
                    pEvaluation.maxincrementbasalarea = slopediff;
                    pEvaluation.countermaxincrementbasalarea = 50;
                } else {
                    --pEvaluation.countermaxincrementbasalarea;
                }
                if (pEvaluation.countermaxincrementbasalarea == 0) {
                    pEvaluation.yearofturningpoint = jahr;
                    pEvaluation.postyearofturningpoint = true;
                }
            }
            // after the turning point is reached, the saturation of the system is reached as soon as the slope becomes negative
            else {
                if ((slopediff < 0.0) && (pEvaluation.yearofequilibrium == -9999)) {
                    pEvaluation.yearofequilibrium = jahr;
                }
            }
        }
        // evaluation_list update




		// currencies output
            // assemble file name
            if (parameter[0].spinupphase == true) {
                s1 << parameter[0].repeati;
                s2 << parameter[0].weatherchoice;
                dateiname = "output/dataentwicklung_currencies_" + s1.str() + "_" + s2.str() + ".csv";
                s1.str("");
                s1.clear();
                s2.str("");
                s2.clear();
            } else {
                s1 << parameter[0].weatherchoice;
                dateiname = "output/datatrees_currencies_" + s1.str() + ".csv";
                s1.str("");
                s1.clear();
                s2.str("");
                s2.clear();
            }

            // trying to open the data file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new data file:
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");
                fprintf(filepointer, "Repeat;");
                // fprintf(filepointer, "YPLOTPOS;");
                // fprintf(filepointer, "XPLOTPOS;");
                fprintf(filepointer, "Progress;");
                fprintf(filepointer, "Spinup;");

                // add column names to the file
                if (parameter[0].spinupphase == true) {
                    fprintf(filepointer, "Random_year;");
                } else {
                    fprintf(filepointer, "Year;");
                }

                fprintf(filepointer, "Mean_basal_diameter;");
                fprintf(filepointer, "Mean_breast_diameter;");

                // tree stand
                fprintf(filepointer, "Sampling_area;");
                fprintf(filepointer, "N_H0to40;");
                fprintf(filepointer, "N_H40to200;");
                fprintf(filepointer, "N_H200to10000;");
                fprintf(filepointer, "Basalarea;");
                fprintf(filepointer, "Stemcount;");
                fprintf(filepointer, "Mean_tree_height;");
                fprintf(filepointer, "Mean_tree_age;");
                fprintf(filepointer, "Turningpoint_year;");
                fprintf(filepointer, "Equillibrium_year;");
                fprintf(filepointer, "Age_Lgmel_0;");
                fprintf(filepointer, "Age_Lsibi_0;");
                fprintf(filepointer, "Age_Lgmel_1;");
                fprintf(filepointer, "Age_Lsibi_1;");
                fprintf(filepointer, "Age_Lgmel_2;");
                fprintf(filepointer, "Age_Lsibi_2;");
                fprintf(filepointer, "Age_Lgmel_3;");
                fprintf(filepointer, "Age_Lsibi_3;");
                fprintf(filepointer, "Age_Lgmel_4;");
                fprintf(filepointer, "Age_Lsibi_4;");
                fprintf(filepointer, "Age_Lgmel_5;");
                fprintf(filepointer, "Age_Lsibi_5;");
                fprintf(filepointer, "Age_Lgmel_6to10;");
                fprintf(filepointer, "Age_Lsibi_6to10;");
                fprintf(filepointer, "Age_Lgmel_11to20;");
                fprintf(filepointer, "Age_Lsibi_11to20;");
                fprintf(filepointer, "Age_Lgmel_21to50;");
                fprintf(filepointer, "Age_Lsibi_21to50;");
                fprintf(filepointer, "Age_Lgmel_51to100;");
                fprintf(filepointer, "Age_Lsibi_51to100;");
                fprintf(filepointer, "Age_Lgmel_101to150;");
                fprintf(filepointer, "Age_Lsibi_101to150;");
                fprintf(filepointer, "Age_Lgmel_151to200;");
                fprintf(filepointer, "Age_Lsibi_151to200;");
                fprintf(filepointer, "Age_Lgmel_201to300;");
                fprintf(filepointer, "Age_Lsibi_201to300;");
                fprintf(filepointer, "Age_Lgmel_301to400;");
                fprintf(filepointer, "Age_Lsibi_301to400;");
                fprintf(filepointer, "Age_Lgmel_401to500;");
                fprintf(filepointer, "Age_Lsibi_401to500;");
                fprintf(filepointer, "Age_Lgmel_501to600;");
                fprintf(filepointer, "Age_Lsibi_501to600;");
                fprintf(filepointer, "Age_Lgmel_601to700;");
                fprintf(filepointer, "Age_Lsibi_601to700;");
                fprintf(filepointer, "Age_Lgmel_701plus;");
                fprintf(filepointer, "Age_Lsibi_701plus;");
                fprintf(filepointer, "Seed_produced_currently;");
                fprintf(filepointer, "Seed_produced_total;");
                fprintf(filepointer, "N_trees_Lgmel;");
                fprintf(filepointer, "N_trees_Lsibi;");
                fprintf(filepointer, "N_seed_onground;");
                fprintf(filepointer, "N_seed_ontrees;");
                fprintf(filepointer, "N_seeds_Lgmel;");
                fprintf(filepointer, "N_seeds_Lsibi;");
                fprintf(filepointer, "Y_position_max;");
                // weather
                fprintf(filepointer, "Weather_type;");
                // fprintf(filepointer, "Start_trees;");
                fprintf(filepointer, "Temperature_mean_year;");
                fprintf(filepointer, "Temperature_mean_january;");
                fprintf(filepointer, "Temperature_mean_july;");
                fprintf(filepointer, "Vegetation_period_length;");
                fprintf(filepointer, "AAT;");
                fprintf(filepointer, "DDT;");
                fprintf(filepointer, "Precipitation_sum_year;");
                fprintf(filepointer, "Weather_factor_Lgmel;");
                fprintf(filepointer, "Weather_factor_Lsibi;");
                fprintf(filepointer, "Thawing_depth_influence;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: output file is missing!\n");
                    exit(1);
                }
            }

            // fill file with output
            fseek(filepointer, 0, SEEK_END);

            fprintf(filepointer, "%d;", parameter[0].repeati);
            // fprintf(filepointer, "%d;", aktortyworldcoo);
            // fprintf(filepointer, "%d;", aktortxworldcoo);
            fprintf(filepointer, "%d;", parameter[0].ivort);
            fprintf(filepointer, "%d;", t);
            fprintf(filepointer, "%d;", jahr);

            // declarations
            int ageg0 = 0, ageg1 = 0, ageg2 = 0, ageg3 = 0, ageg4 = 0, ageg5 = 0, ageg6b10 = 0, ageg11b20 = 0, ageg21b50 = 0, ageg51b100 = 0, ageg101b150 = 0,
                ageg151b200 = 0, ageg201b300 = 0, ageg301b400 = 0, ageg401b500 = 0, ageg501b600 = 0, ageg601b700 = 0, ageg701plus = 0;
            int ages0 = 0, ages1 = 0, ages2 = 0, ages3 = 0, ages4 = 0, ages5 = 0, ages6b10 = 0, ages11b20 = 0, ages21b50 = 0, ages51b100 = 0, ages101b150 = 0,
                ages151b200 = 0, ages201b300 = 0, ages301b400 = 0, ages401b500 = 0, ages501b600 = 0, ages601b700 = 0, ages701plus = 0;
            int gesamtseedAKT = 0, gesamtseedSUM = 0;
            int spectree1 = 0, spectree2 = 0;
            double yposmax = 0.0;
            double meanbas = 0.0;
            int h = 0;

#pragma omp parallel default(shared)
        {
            // declarations
            int localageg0 = 0, localageg1 = 0, localageg2 = 0, localageg3 = 0, localageg4 = 0, localageg5 = 0, localageg6b10 = 0, localageg11b20 = 0, localageg21b50 = 0, localageg51b100 = 0, localageg101b150 = 0,
                localageg151b200 = 0, localageg201b300 = 0, localageg301b400 = 0, localageg401b500 = 0, localageg501b600 = 0, localageg601b700 = 0, localageg701plus = 0;
            int localages0 = 0, localages1 = 0, localages2 = 0, localages3 = 0, localages4 = 0, localages5 = 0, localages6b10 = 0, localages11b20 = 0, localages21b50 = 0, localages51b100 = 0, localages101b150 = 0,
                localages151b200 = 0, localages201b300 = 0, localages301b400 = 0, localages401b500 = 0, localages501b600 = 0, localages601b700 = 0, localages701plus = 0;
            int localgesamtseedAKT = 0, localgesamtseedSUM = 0;
            int localspectree1 = 0, localspectree2 = 0;
            double localyposmax = 0.0;
            double localmeanbas = 0.0;
            int localh = 0;

#pragma omp for schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];

				if (tree.growing == true) {
					if (((double)tree.xcoo / 1000 >= xminwindow) && ((double)tree.xcoo / 1000 <= xmaxwindow) && ((double)tree.ycoo / 1000 >= yminwindow)
						&& ((double)tree.ycoo / 1000 <= ymaxwindow)) {
						if (tree.species == 1) {
							// bin trees by age
							if (tree.age == 0) {
								localageg0++;
							} else if (tree.age == 1) {
								localageg1++;
							} else if (tree.age == 2) {
								localageg2++;
							} else if (tree.age == 3) {
								localageg3++;
							} else if (tree.age == 4) {
								localageg4++;
							} else if (tree.age == 5) {
								localageg5++;
							} else if ((tree.age > 5) && (tree.age <= 10)) {
								localageg6b10++;
							} else if ((tree.age > 10) && (tree.age <= 20)) {
								localageg11b20++;
							} else if ((tree.age > 20) && (tree.age <= 50)) {
								localageg21b50++;
							} else if ((tree.age > 50) && (tree.age <= 100)) {
								localageg51b100++;
							} else if ((tree.age > 100) && (tree.age <= 150)) {
								localageg101b150++;
							} else if ((tree.age > 150) && (tree.age <= 200)) {
								localageg151b200++;
							} else if ((tree.age > 200) && (tree.age <= 300)) {
								localageg201b300++;
							} else if ((tree.age > 300) && (tree.age <= 400)) {
								localageg301b400++;
							} else if ((tree.age > 400) && (tree.age <= 500)) {
								localageg401b500++;
							} else if ((tree.age > 500) && (tree.age <= 600)) {
								localageg501b600++;
							} else if ((tree.age > 600) && (tree.age <= 700)) {
								localageg601b700++;
							} else if (tree.age > 700) {
								localageg701plus++;
							}
						} else if (tree.species == 2) {
							if (tree.age == 0) {
								localages0++;
							} else if (tree.age == 1) {
								localages1++;
							} else if (tree.age == 2) {
								localages2++;
							} else if (tree.age == 3) {
								localages3++;
							} else if (tree.age == 4) {
								localages4++;
							} else if (tree.age == 5) {
								localages5++;
							} else if ((tree.age > 5) && (tree.age <= 10)) {
								localages6b10++;
							} else if ((tree.age > 10) && (tree.age <= 20)) {
								localages11b20++;
							} else if ((tree.age > 20) && (tree.age <= 50)) {
								localages21b50++;
							} else if ((tree.age > 50) && (tree.age <= 100)) {
								localages51b100++;
							} else if ((tree.age > 100) && (tree.age <= 150)) {
								localages101b150++;
							} else if ((tree.age > 150) && (tree.age <= 200)) {
								localages151b200++;
							} else if ((tree.age > 200) && (tree.age <= 300)) {
								localages201b300++;
							} else if ((tree.age > 300) && (tree.age <= 400)) {
								localages301b400++;
							} else if ((tree.age > 400) && (tree.age <= 500)) {
								localages401b500++;
							} else if ((tree.age > 500) && (tree.age <= 600)) {
								localages501b600++;
							} else if ((tree.age > 600) && (tree.age <= 700)) {
								localages601b700++;
							} else if (tree.age > 700) {
								localages701plus++;
							}
						}

						if (tree.dbasal > 0) {
							localmeanbas = localmeanbas + tree.dbasal;
							localh++;
						}

						// counting seeds
						localgesamtseedAKT += tree.seednewly_produced;
						// gesamtseedSUM += tree.seedproduced;

						// counting species
						if (tree.species == 1) {
							localspectree1++;
						} else if (tree.species == 2) {
							localspectree2++;
						}

						if ((double)tree.ycoo / 1000 > localyposmax) {
							localyposmax = (double)tree.ycoo / 1000;
						}
					}
				}
            }
#pragma omp critical
            {
				ageg0 += localageg0; 
				ageg1 += localageg1; 
				ageg2 += localageg2; 
				ageg3 += localageg3; 
				ageg4 += localageg4; 
				ageg5 += localageg5; 
				ageg6b10 += localageg6b10; 
				ageg11b20 += localageg11b20; 
				ageg21b50 += localageg21b50; 
				ageg51b100 += localageg51b100; 
				ageg101b150 += localageg101b150;
                ageg151b200 += localageg151b200; 
				ageg201b300 += localageg201b300; 
				ageg301b400 += localageg301b400; 
				ageg401b500 += localageg401b500; 
				ageg501b600 += localageg501b600; 
				ageg601b700 += localageg601b700; 
				ageg701plus += localageg701plus;
				ages0 += localages0 ; 
				ages1 += localages1; 
				ages2 += localages2; 
				ages3 += localages3; 
				ages4 += localages4; 
				ages5 += localages5; 
				ages6b10 += localages6b10; 
				ages11b20 += localages11b20; 
				ages21b50 += localages21b50; 
				ages51b100 += localages51b100; 
				ages101b150 += localages101b150;
                ages151b200 += localages151b200; 
				ages201b300 += localages201b300; 
				ages301b400 += localages301b400; 
				ages401b500 += localages401b500; 
				ages501b600 += localages501b600; 
				ages601b700 += localages601b700; 
				ages701plus += localages701plus;
				gesamtseedAKT += localgesamtseedAKT; 
				gesamtseedSUM += localgesamtseedSUM;
				spectree1 += localspectree1; 
				spectree2 += localspectree2;
				meanbas += localmeanbas;
				h += localh;
			
				if (localyposmax > yposmax) {
					yposmax = localyposmax;
				}
            }
		}

            // for counting seeds:
            int seedconezahl = 0, seedbodenzahl = 0;
            // for counting species:
            int specseed1 = 0, specseed2 = 0;
#pragma omp parallel default(shared)
        {
            int localseedconezahl = 0;
			int localseedbodenzahl = 0;
            int localspecseed1 = 0;			
			int localspecseed2 = 0;
			
#pragma omp for schedule(guided)
            for (unsigned int i = 0; i < seed_list.size(); ++i) {
                const auto& seed = seed_list[i];
				if(!seed.dead) {
					if (((double)seed.xcoo / 1000 >= xminwindow) && ((double)seed.xcoo / 1000 <= xmaxwindow) && ((double)seed.ycoo / 1000 >= yminwindow)
						&& ((double)seed.ycoo / 1000 <= ymaxwindow)) {
						if (seed.incone) {
							localseedconezahl++;
						} else {
							localseedbodenzahl++;
						}
						// counting species:
						if (seed.species == 1) {
							localspecseed1++;
						} else if (seed.species == 2) {
							localspecseed2++;
						}
					}
				}
            }
#pragma omp critical
            {
				seedconezahl += localseedconezahl;
				seedbodenzahl +=  localseedbodenzahl;
				specseed1 +=  localspecseed1;			
				specseed2 +=  localspecseed2;			
			}
		}// pragma

            if (h > 0) {
                fprintf(filepointer, "%4.4f;", meanbas / ((double)h));
            } else {
                fprintf(filepointer, "%4.4f;", 0.0);
            }
            fprintf(filepointer, "%4.4f;", pEvaluation.meanbreastdiameterliste[pEvaluation.meanbreastdiameterliste.size() - 1]);
            fprintf(filepointer, "%4.2f;", (xmaxwindow - xminwindow) * (ymaxwindow - yminwindow));
            fprintf(filepointer, "%d;", pEvaluation.nheight0b40liste[pEvaluation.nheight0b40liste.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation.nheight41b200liste[pEvaluation.nheight41b200liste.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation.nheight201b10000liste[pEvaluation.nheight201b10000liste.size() - 1]);
            fprintf(filepointer, "%6.4f;", pEvaluation.basalarealist[pEvaluation.basalarealist.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation.stemcountliste[pEvaluation.stemcountliste.size() - 1]);
            fprintf(filepointer, "%6.4f;", pEvaluation.meantreeheightliste[pEvaluation.meantreeheightliste.size() - 1]);
            fprintf(filepointer, "%6.4f;", pEvaluation.meantreeageliste[pEvaluation.meantreeageliste.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation.yearofturningpoint);
            fprintf(filepointer, "%d;", pEvaluation.yearofequilibrium);
            fprintf(filepointer, "%d;", ageg0);
            fprintf(filepointer, "%d;", ages0);
            fprintf(filepointer, "%d;", ageg1);
            fprintf(filepointer, "%d;", ages1);
            fprintf(filepointer, "%d;", ageg2);
            fprintf(filepointer, "%d;", ages2);
            fprintf(filepointer, "%d;", ageg3);
            fprintf(filepointer, "%d;", ages3);
            fprintf(filepointer, "%d;", ageg4);
            fprintf(filepointer, "%d;", ages4);
            fprintf(filepointer, "%d;", ageg5);
            fprintf(filepointer, "%d;", ages5);
            fprintf(filepointer, "%d;", ageg6b10);
            fprintf(filepointer, "%d;", ages6b10);
            fprintf(filepointer, "%d;", ageg11b20);
            fprintf(filepointer, "%d;", ages11b20);
            fprintf(filepointer, "%d;", ageg21b50);
            fprintf(filepointer, "%d;", ages21b50);
            fprintf(filepointer, "%d;", ageg51b100);
            fprintf(filepointer, "%d;", ages51b100);
            fprintf(filepointer, "%d;", ageg101b150);
            fprintf(filepointer, "%d;", ages101b150);
            fprintf(filepointer, "%d;", ageg151b200);
            fprintf(filepointer, "%d;", ages151b200);
            fprintf(filepointer, "%d;", ageg201b300);
            fprintf(filepointer, "%d;", ages201b300);
            fprintf(filepointer, "%d;", ageg301b400);
            fprintf(filepointer, "%d;", ages301b400);
            fprintf(filepointer, "%d;", ageg401b500);
            fprintf(filepointer, "%d;", ages401b500);
            fprintf(filepointer, "%d;", ageg501b600);
            fprintf(filepointer, "%d;", ages501b600);
            fprintf(filepointer, "%d;", ageg601b700);
            fprintf(filepointer, "%d;", ages601b700);
            fprintf(filepointer, "%d;", ageg701plus);
            fprintf(filepointer, "%d;", ages701plus);
            fprintf(filepointer, "%d;", gesamtseedAKT);
            fprintf(filepointer, "%d;", gesamtseedSUM);
            fprintf(filepointer, "%d;", spectree1);
            fprintf(filepointer, "%d;", spectree2);
            fprintf(filepointer, "%d;", seedbodenzahl);
            fprintf(filepointer, "%d;", seedconezahl);
            fprintf(filepointer, "%d;", specseed1);
            fprintf(filepointer, "%d;", specseed2);
            fprintf(filepointer, "%4.2f;", yposmax);
            // weather
            fprintf(filepointer, "%d;", parameter[0].weatherchoice);
            // fprintf(filepointer, "%d;", parameter[0].starttrees);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].tempyearmean);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].temp1monthmean);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].temp7monthmean);
            fprintf(filepointer, "%d;", weather_list[yearposition].vegetationperiodlength);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].activeairtemp);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].degreday_sqrt * weather_list[yearposition].degreday_sqrt);
            fprintf(filepointer, "%4.2f;", weather_list[yearposition].precipitationsum);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].weatherfactorg);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].weatherfactors);
            fprintf(filepointer, "%d;", parameter[0].thawing_depth);

            fprintf(filepointer, "\n");

            fclose(filepointer);

            if (parameter[0].yearlyvis == true) {
                cout << endl << "\tBasalarea = " << pEvaluation.basalarealist[pEvaluation.basalarealist.size() - 1] << endl;
                cout << "\tN0-40 = " << nheight0b40 << "\tN40-200 = " << nheight41b200 << "\tN200+ = " << nheight201b10000 << endl;
                cout << "\tNseeds:\tproduced = " << gesamtseedAKT << "\tground = " << seedbodenzahl << "\tcones = " << seedconezahl << endl;
            }
        }  // trees currencies

        if (outputposition == true) {  // tree position output
            if (parameter[0].ivort == 1 || (parameter[0].spinupphase == true && (parameter[0].ivort % 10) == 0) || (parameter[0].spinupphase == false)) {
                // assemble file name
                s1 << parameter[0].repeati;
                s2 << parameter[0].weatherchoice;
                s3 << parameter[0].ivort;
                s4 << aktort;
                dateiname = "output/datatrees_positions_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + ".csv";
                s1.str("");
                s1.clear();
                s2.str("");
                s2.clear();
                s3.str("");
                s3.clear();
                s4.str("");
                s4.clear();

                if (parameter[0].boundaryconditions == 3) {
                    // trying to open the file for reading
                    filepointer = fopen(dateiname.c_str(), "r+");
                    // if fopen fails, open a new file + header output
                    if (filepointer == NULL) {
                        filepointer = fopen(dateiname.c_str(), "w+");

                        // column names
                        // fprintf(filepointer, "Name;");
                        // fprintf(filepointer, "NameM;");
                        fprintf(filepointer, "Diameter_basal;");
                        fprintf(filepointer, "Diameter_breast;");
                        fprintf(filepointer, "Height;");
                        fprintf(filepointer, "Age;");
                        fprintf(filepointer, "X;");
                        fprintf(filepointer, "Y;");
                        fprintf(filepointer, "Density_value;");
                        // fprintf(filepointer, "Generation;");
                        fprintf(filepointer, "Seeds_produced_currently;");
                        // fprintf(filepointer, "Seeds_produced_total;");
                        fprintf(filepointer, "\n");

                        if (filepointer == NULL) {
                            fprintf(stderr, "Error: File could not be opened!\n");
                            exit(1);
                        }
                    }

                    fseek(filepointer, 0, SEEK_END);

                    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                        auto& tree = tree_list[tree_i];

						if (tree.growing == true) {
							// fprintf(filepointer, "%d;", tree.name);
							// fprintf(filepointer, "%d;", tree.namem);
							fprintf(filepointer, "%4.4f;", tree.dbasal);
							fprintf(filepointer, "%4.4f;", tree.dbreast);
							fprintf(filepointer, "%4.4f;", (double)tree.height / 100);
							fprintf(filepointer, "%d;", tree.age);
							fprintf(filepointer, "%4.4f;", (double)tree.xcoo / 1000);
							fprintf(filepointer, "%4.4f;", (double)tree.ycoo / 1000);
							fprintf(filepointer, "%4.5f;", tree.densitywert);
							// fprintf(filepointer, "%d;", tree.generation);
							fprintf(filepointer, "%d;", tree.seednewly_produced);
							// fprintf(filepointer, "%d;", tree.seedproduced);
							fprintf(filepointer, "\n");
						}
					}

                    fclose(filepointer);
                } else {
                    // trying to open the file for reading
                    filepointer = fopen(dateiname.c_str(), "r+");
                    // if fopen fails, open a new file + header output
                    if (filepointer == NULL) {
                        filepointer = fopen(dateiname.c_str(), "w+");

                        fprintf(filepointer, "Repeat;");
                        // fprintf(filepointer, "YPLOTPOS;");
                        // fprintf(filepointer, "XPLOTPOS;");
                        fprintf(filepointer, "Year;");
                        fprintf(filepointer, "Random_year;");
                        fprintf(filepointer, "Weather_type;");
                        fprintf(filepointer, "Diameter_basal;");
                        fprintf(filepointer, "Diameter_breast;");
                        fprintf(filepointer, "Height;");
                        fprintf(filepointer, "Age;");
                        fprintf(filepointer, "X;");
                        fprintf(filepointer, "Y;");
                        fprintf(filepointer, "Density_value;");
                        // fprintf(filepointer, "Generation;");
                        fprintf(filepointer, "Cone_height;");
                        fprintf(filepointer, "Seeds_produced_currently;");
                        // fprintf(filepointer, "Seeds_produced_total;");
                        fprintf(filepointer, "Thawing_depth_influence;");
                        fprintf(filepointer, "\n");

                        if (filepointer == NULL) {
                            fprintf(stderr, "Error: output file is missing!\n");
                            exit(1);
                        }
                    }

                    fseek(filepointer, 0, SEEK_END);

                    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                        auto& tree = tree_list[tree_i];

						if (tree.growing == true) {
							fprintf(filepointer, "%d;", parameter[0].repeati);
							// fprintf(filepointer, "%d;", tree.yworldcoo);
							// fprintf(filepointer, "%d;", tree.xworldcoo);
							fprintf(filepointer, "%d;", parameter[0].ivort);
							fprintf(filepointer, "%d;", jahr);
							fprintf(filepointer, "%d;", parameter[0].weatherchoice);
							fprintf(filepointer, "%4.4f;", tree.dbasal);
							fprintf(filepointer, "%4.4f;", tree.dbreast);
							fprintf(filepointer, "%4.4f;", (double)tree.height / 100);
							fprintf(filepointer, "%d;", tree.age);
							fprintf(filepointer, "%4.4f;", (double)tree.xcoo / 1000);
							fprintf(filepointer, "%4.4f;", (double)tree.ycoo / 1000);
							fprintf(filepointer, "%4.5f;", tree.densitywert);
							// fprintf(filepointer, "%d;", tree.generation);
							fprintf(filepointer, "%4.4f;", (double)tree.coneheight / 100);
							fprintf(filepointer, "%d;", tree.seednewly_produced);
							// fprintf(filepointer, "%d;", tree.seedproduced);
							fprintf(filepointer, "%lf;", tree.thawing_depthinfluence);
							fprintf(filepointer, "\n");
						}
                    }

                    fclose(filepointer);
                }
            }
        }  // tree position output

        if (outputindividuals == true) {  // individual tree output

            // assemble file name
            s1 << parameter[0].repeati;
            s2 << parameter[0].weatherchoice;
            s3 << parameter[0].ivort;
            s4 << aktort;
            dateiname = "output/datatrees_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + ".csv";
            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();
            s3.str("");
            s3.clear();
            s4.str("");
            s4.clear();

            // trying to open the file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                // parameters
                // fprintf(filepointer, "Repeat;");
                // fprintf(filepointer, "YPLOTPOS;");
                // fprintf(filepointer, "XPLOTPOS;");
                // fprintf(filepointer, "Weather_type;");
                // time variables
                // fprintf(filepointer, "Progress;");
                // fprintf(filepointer, "Year;");
                // tree variables
                fprintf(filepointer, "X;");
                fprintf(filepointer, "Y;");
                // fprintf(filepointer, "Name;");
                // fprintf(filepointer, "NameM;");
                // fprintf(filepointer, "NameP;");
                // fprintf(filepointer, "Line;");
                // fprintf(filepointer, "Generation;");
                // fprintf(filepointer, "Species;");
                fprintf(filepointer, "Height;");
                fprintf(filepointer, "Dbasal;");
                fprintf(filepointer, "Dbreast;");
                fprintf(filepointer, "Age;");
                // fprintf(filepointer, "Cone;");
                // fprintf(filepointer, "Cone_height;");
                // fprintf(filepointer, "Seeds_produced_currently;");
                // fprintf(filepointer, "Seeds_produced_total;");
                // fprintf(filepointer, "Buffer;");
                // fprintf(filepointer, "Density_value;");
                // fprintf(filepointer, "Distance;");
                // fprintf(filepointer, "Thawing_depth_influence;");
                fprintf(filepointer, "Elevation;");
                fprintf(filepointer, "Envirgrowthimpact;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: File could not be opened!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data output for each tree
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					// parameters
					// fprintf(filepointer, "%d;", parameter[0].repeati);
					// fprintf(filepointer, "%d;", tree.yworldcoo);
					// fprintf(filepointer, "%d;", tree.xworldcoo);
					// fprintf(filepointer, "%d;", parameter[0].weatherchoice);
					// time variables
					// fprintf(filepointer, "%d;", t);
					// fprintf(filepointer, "%d;", jahr);
					// tree variables
					fprintf(filepointer, "%4.4f;", (double)tree.xcoo / 1000);
					fprintf(filepointer, "%4.4f;", (double)tree.ycoo / 1000);
					// fprintf(filepointer, "%d;", tree.name);
					// fprintf(filepointer, "%d;", tree.namem);
					// fprintf(filepointer, "%d;", tree.namep);
					// fprintf(filepointer, "%d;", tree.line);
					// fprintf(filepointer, "%d;", tree.generation);
					// fprintf(filepointer, "%d;", tree.species);
					fprintf(filepointer, "%4.4f;", (double)tree.height / 100);
					fprintf(filepointer, "%4.4f;", tree.dbasal);
					fprintf(filepointer, "%4.4f;", tree.dbreast);
					fprintf(filepointer, "%d;", tree.age);
					// fprintf(filepointer, "%d;", tree.cone);
					// fprintf(filepointer, "%4.4f;", tree.coneheight);
					// fprintf(filepointer, "%d;", tree.seednewly_produced);
					// fprintf(filepointer, "%d;", tree.seedproduced);
					// fprintf(filepointer, "%d;", tree.buffer);
					// fprintf(filepointer, "%4.5f;", tree.densitywert);
					// fprintf(filepointer, "%4.5f;", tree.dispersaldistance);
					// fprintf(filepointer, "%lf;", tree.thawing_depthinfluence);
					fprintf(filepointer, "%4.4f;", (double)tree.elevation / 10);
					fprintf(filepointer, "%4.4f;", (double)tree.envirimpact / 10000);
					fprintf(filepointer, "\n");
				}
			}

            fclose(filepointer);
        }  // individual tree output
        
		if (outputtestarea == true) {  // individual tree output only in a specified area
		
            // assemble file name
            s1 << parameter[0].repeati;
            s2 << parameter[0].weatherchoice;
            s3 << parameter[0].ivort;
            s4 << aktort;
            dateiname = "output/datatreesonselectedtransect_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + ".csv";
            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();
            s3.str("");
            s3.clear();
            s4.str("");
            s4.clear();

            // trying to open the file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                // parameters
                // fprintf(filepointer, "Repeat;");
                // fprintf(filepointer, "YPLOTPOS;");
                // fprintf(filepointer, "XPLOTPOS;");
                // fprintf(filepointer, "Weather_type;");
                // time variables
                // fprintf(filepointer, "Progress;");
                // fprintf(filepointer, "Year;");
                // tree variables
                fprintf(filepointer, "X;");
                fprintf(filepointer, "Y;");
                // fprintf(filepointer, "Name;");
                // fprintf(filepointer, "NameM;");
                // fprintf(filepointer, "NameP;");
                // fprintf(filepointer, "Line;");
                // fprintf(filepointer, "Generation;");
                // fprintf(filepointer, "Species;");
                fprintf(filepointer, "Height;");
                fprintf(filepointer, "Dbasal;");
                fprintf(filepointer, "Dbreast;");
                fprintf(filepointer, "Age;");
                // fprintf(filepointer, "Cone;");
                // fprintf(filepointer, "Cone_height;");
                // fprintf(filepointer, "Seeds_produced_currently;");
                // fprintf(filepointer, "Seeds_produced_total;");
                // fprintf(filepointer, "Buffer;");
                // fprintf(filepointer, "Density_value;");
                // fprintf(filepointer, "Distance;");
                // fprintf(filepointer, "Thawing_depth_influence;");
                fprintf(filepointer, "Elevation;");
                fprintf(filepointer, "Envirgrowthimpact;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: File could not be opened!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data output for each tree
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
			
				if (tree.growing == true) {
					if (   ((double)tree.xcoo / 1000 >= (643515-637008.2)) // currently for Site EN18022 30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m.csv"; //x=18000, y=24720
							// ... x 643515
							// ... y 7479959
							// as 100-m wide transect starting here in tundra and going south into forest for 400 m
						&& ((double)tree.xcoo / 1000 <= (643515+100-637008.2))
						&& ((double)tree.ycoo / 1000 >= (7479959-400-7469996))
						&& ((double)tree.ycoo / 1000 <= (7479959-7469996))) {
						// parameters
						// fprintf(filepointer, "%d;", parameter[0].repeati);
						// fprintf(filepointer, "%d;", tree.yworldcoo);
						// fprintf(filepointer, "%d;", tree.xworldcoo);
						// fprintf(filepointer, "%d;", parameter[0].weatherchoice);
						// time variables
						// fprintf(filepointer, "%d;", t);
						// fprintf(filepointer, "%d;", jahr);
						// tree variables
						fprintf(filepointer, "%4.4f;", (double)tree.xcoo / 1000);
						fprintf(filepointer, "%4.4f;", (double)tree.ycoo / 1000);
						// fprintf(filepointer, "%d;", tree.name);
						// fprintf(filepointer, "%d;", tree.namem);
						// fprintf(filepointer, "%d;", tree.namep);
						// fprintf(filepointer, "%d;", tree.line);
						// fprintf(filepointer, "%d;", tree.generation);
						// fprintf(filepointer, "%d;", tree.species);
						fprintf(filepointer, "%4.4f;", (double)tree.height / 100);
						fprintf(filepointer, "%4.4f;", tree.dbasal);
						fprintf(filepointer, "%4.4f;", tree.dbreast);
						fprintf(filepointer, "%d;", tree.age);
						// fprintf(filepointer, "%d;", tree.cone);
						// fprintf(filepointer, "%4.4f;", tree.coneheight);
						// fprintf(filepointer, "%d;", tree.seednewly_produced);
						// fprintf(filepointer, "%d;", tree.seedproduced);
						// fprintf(filepointer, "%d;", tree.buffer);
						// fprintf(filepointer, "%4.5f;", tree.densitywert);
						// fprintf(filepointer, "%4.5f;", tree.dispersaldistance);
						// fprintf(filepointer, "%lf;", tree.thawing_depthinfluence);
						// fprintf(filepointer, "%4.4f;", (double)tree.elevation / 10);
						// fprintf(filepointer, "%4.4f;", (double)tree.envirimpact / 10000);

						fprintf(filepointer, "\n");
					}
				}
			}

            fclose(filepointer);
        }  // individual tree output

        if (outputgriddedbiomass == true) {  // gridded tree biomass output
            // assemble file name
            s1 << parameter[0].repeati;
            s2 << parameter[0].weatherchoice;
            s3 << parameter[0].ivort;
            s4 << aktort;
            dateiname = "output/databiomassgrid_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + ".csv";
            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();
            s3.str("");
            s3.clear();
            s4.str("");
            s4.clear();

            // aggregate data on 30 m grid
            int deminputdimension_y = treerows / parameter[0].demresolution;  // matrix + 1 to avoid border effects
            int deminputdimension_x = treecols / parameter[0].demresolution;  // matrix + 1 to avoid border effects
            vector<double> AGBneedleliving;
            AGBneedleliving.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<double> AGBwoodliving;
            AGBwoodliving.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<unsigned int> Stemcount;
            Stemcount.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<double> Basalarea;
            Basalarea.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<unsigned int> Indicount_10;
            vector<unsigned int> Indicount_40;
            vector<unsigned int> Indicount_100;
            vector<unsigned int> Indicount_200;
            vector<unsigned int> Indicount_300;
            vector<unsigned int> Indicount_400;
            vector<unsigned int> Indicount_500;
            vector<unsigned int> Indicount_750;
            vector<unsigned int> Indicount_1000;
            vector<unsigned int> Indicount_1250;
            vector<unsigned int> Indicount_1500;
            vector<unsigned int> Indicount_2000;
            vector<unsigned int> Indicount_larger2000;
            Indicount_10.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_40.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_100.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_200.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_300.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_400.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_500.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_750.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_1000.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_1250.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_1500.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_2000.resize(deminputdimension_y * deminputdimension_x, 0);
            Indicount_larger2000.resize(deminputdimension_y * deminputdimension_x, 0);

#pragma omp parallel for default(shared) schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					// calculate grid position from x/y coordinates
					unsigned int grid_i = floor((double)tree.ycoo / 1000 / 30) * deminputdimension_x + floor((double)tree.xcoo / 1000 / 30);

					// calculate biomass values for each tree
					AGBneedleliving[grid_i] += 703.6207 / (1 + exp(-1.0 * ((double)tree.height / 100 - 579.4998) / 208.687));
					AGBwoodliving[grid_i] += 78713.62675 / (1 + exp(-1.0 * ((double)tree.height / 100 - 793.64156) / 73.91302));

					// aggregate stand level variables
					if (((double)tree.height / 100) > 130) {
						Stemcount[grid_i]++;
						Basalarea[grid_i] += (M_PI * pow((tree.dbreast / 100 / 2), 2));  // in sq.m so here conversion from cm to m
					}
					// aggregate further stand structure variables
					if (((double)tree.height / 100) < 10) {
						Indicount_10[grid_i]++;
					} else if ((((double)tree.height / 100) >= 10) & (((double)tree.height / 100) < 40)) {
						Indicount_40[grid_i]++;
					} else if ((((double)tree.height / 100) >= 40) & (((double)tree.height / 100) < 100)) {
						Indicount_100[grid_i]++;
					} else if ((((double)tree.height / 100) >= 100) & (((double)tree.height / 100) < 200)) {
						Indicount_200[grid_i]++;
					} else if ((((double)tree.height / 100) >= 200) & (((double)tree.height / 100) < 300)) {
						Indicount_300[grid_i]++;
					} else if ((((double)tree.height / 100) >= 300) & (((double)tree.height / 100) < 400)) {
						Indicount_400[grid_i]++;
					} else if ((((double)tree.height / 100) >= 400) & (((double)tree.height / 100) < 500)) {
						Indicount_500[grid_i]++;
					} else if ((((double)tree.height / 100) >= 500) & (((double)tree.height / 100) < 750)) {
						Indicount_750[grid_i]++;
					} else if ((((double)tree.height / 100) >= 750) & (((double)tree.height / 100) < 1000)) {
						Indicount_1000[grid_i]++;
					} else if ((((double)tree.height / 100) >= 1000) & (((double)tree.height / 100) < 1250)) {
						Indicount_1250[grid_i]++;
					} else if ((((double)tree.height / 100) >= 1250) & (((double)tree.height / 100) < 1500)) {
						Indicount_1500[grid_i]++;
					} else if ((((double)tree.height / 100) >= 1500) & (((double)tree.height / 100) < 2000)) {
						Indicount_2000[grid_i]++;
					} else if (((double)tree.height / 100) >= 2000) {
						Indicount_larger2000[grid_i]++;
					}
				}
			}

            // trying to open the file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                fprintf(filepointer, "x;");
                fprintf(filepointer, "y;");
                fprintf(filepointer, "AGBneedleliving;");
                fprintf(filepointer, "AGBwoodliving;");
                fprintf(filepointer, "Stemcount;");
                fprintf(filepointer, "Basalarea;");
                fprintf(filepointer, "Individuals_0_10cm;");
                fprintf(filepointer, "Individuals_10_40cm;");
                fprintf(filepointer, "Individuals_40_100cm;");
                fprintf(filepointer, "Individuals_100_200cm;");
                fprintf(filepointer, "Individuals_200_300cm;");
                fprintf(filepointer, "Individuals_300_400cm;");
                fprintf(filepointer, "Individuals_400_500cm;");
                fprintf(filepointer, "Individuals_500_750cm;");
                fprintf(filepointer, "Individuals_750_1000cm;");
                fprintf(filepointer, "Individuals_1000_1250cm;");
                fprintf(filepointer, "Individuals_1250_1500cm;");
                fprintf(filepointer, "Individuals_1500_2000cm;");
                fprintf(filepointer, "Individuals_larger2000cm;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: File could not be opened!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data output for each tree
            for (unsigned int grid_i = 0; grid_i < AGBneedleliving.size(); ++grid_i) {
                // if(AGBneedleliving[grid_i]>0) {
                unsigned int y = grid_i / deminputdimension_x;
                unsigned int x = (double)grid_i - y * deminputdimension_x;

                fprintf(filepointer, "%d;", x);
                fprintf(filepointer, "%d;", y);
                fprintf(filepointer, "%4.8f;", AGBneedleliving[grid_i] / 1000 / (30 * 30));    // in kg/sq.m.
                fprintf(filepointer, "%4.8f;", AGBwoodliving[grid_i] / 1000 / (30 * 30));      // in kg/sq.m.
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Stemcount[grid_i]);  // in stems/ha
                fprintf(filepointer, "%4.8f;", ((100 * 100) / (30 * 30)) * Basalarea[grid_i]);
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_10[grid_i]);          // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_40[grid_i]);          // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_100[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_200[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_300[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_400[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_500[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_750[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_1000[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_1250[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_1500[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_2000[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (30 * 30))) * Indicount_larger2000[grid_i]);  // in individuals/ha
                fprintf(filepointer, "\n");
                // }
            }

            fclose(filepointer);
        }  // gridded tree biomass output

        if (outputtransects == true) {  // transect output
            // assemble file name
            s1 << parameter[0].repeati;
            s2 << parameter[0].weatherchoice;
            s3 << parameter[0].ivort;

            dateiname = "output/datatransects_" + s1.str() + "_" + s2.str() + "_" + s3.str() + ".csv";

            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();
            s3.str("");
            s3.clear();

            // trying to open the file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                fprintf(filepointer, "Y;");
                fprintf(filepointer, "Species1_stems;");
                fprintf(filepointer, "Species2_stems;");
                fprintf(filepointer, "Species1_seedlings;");
                fprintf(filepointer, "Species2_seedlings;");

                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: File could not be opened!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // aggregate output for y-transects on m-precision
            vector<int> Species1_stems;
            vector<int> Species2_stems;
            vector<int> Species1_seedlings;
            vector<int> Species2_seedlings;
            // fill vectors with zeros
            Species1_stems.resize(treerows, 0);
            Species2_stems.resize(treerows, 0);
            Species1_seedlings.resize(treerows, 0);
            Species2_seedlings.resize(treerows, 0);

#pragma omp parallel for default(shared) schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					unsigned int yposi = tree.ycoo / 1000;

					// aggregate variables
					if (((double)tree.height / 100) > 130) {
						if (tree.species == 1)
							Species1_stems[yposi]++;
						else
							Species2_stems[yposi]++;
					} else {
						if (tree.species == 1)
							Species1_seedlings[yposi]++;
						else
							Species2_seedlings[yposi]++;
					}
				}
            }

            // add data to file
            for (std::size_t yposi = 0; yposi < treerows; ++yposi) {
                fprintf(filepointer, "%ld;", yposi);
                fprintf(filepointer, "%d;", Species1_stems[yposi]);
                fprintf(filepointer, "%d;", Species2_stems[yposi]);
                fprintf(filepointer, "%d;", Species1_seedlings[yposi]);
                fprintf(filepointer, "%d;", Species2_seedlings[yposi]);
                fprintf(filepointer, "\n");
            }

            fclose(filepointer);
        }  // transect output

        if (ausgabedensity == true) {  // tree density map output
            // assemble file name:
            s1 << parameter[0].ivort;
            s2 << parameter[0].weatherchoice;
            dateiname = "output/datatrees_Treedensity" + s1.str() + "_" + s2.str() + ".csv";
            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();

            // trying to open the file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                fprintf(filepointer, "Repeat;");
                // fprintf(filepointer, "YPLOTPOS;");
                // fprintf(filepointer, "XPLOTPOS;");
                fprintf(filepointer, "X;");
                fprintf(filepointer, "Y;");
                fprintf(filepointer, "Density_value;");
                fprintf(filepointer, "N_trees;");
                // fprintf(filepointer, "Litter_layer_height;");
                // fprintf(filepointer, "Litter_layer_height_mean;");
                fprintf(filepointer, "Max_thawing_depth;");
                fprintf(filepointer, "Elevation;");
                fprintf(filepointer, "Envirgrowthimpact;");
                // fprintf(filepointer, "Weather_type;");
                // fprintf(filepointer, "Thawing_depth;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: output file is missing!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data evaluation and output
            for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int)treerows * (unsigned long long int)parameter[0].sizemagnif * (unsigned long long int)treecols * (unsigned long long int)parameter[0].sizemagnif);
                 kartenpos = kartenpos + parameter[0].sizemagnif * parameter[0].demresolution) {
                auto& pEnvirgrid = plot_list[kartenpos];
                double ycooi = floor((double)kartenpos / ((double)treecols * parameter[0].sizemagnif));
                double xcooi = (double)kartenpos - ((ycooi-1) * ((double)treecols * parameter[0].sizemagnif));
                if ((parameter[0].demlandscape
                     && ((((xcooi / parameter[0].sizemagnif / parameter[0].demresolution) - floor(xcooi / parameter[0].sizemagnif / parameter[0].demresolution))
                          == 0)
                         && (((ycooi / parameter[0].sizemagnif / parameter[0].demresolution)
                              - floor(ycooi / parameter[0].sizemagnif / parameter[0].demresolution))
                             == 0)))
                    || ((pEnvirgrid.Treenumber > 0)
                        && ((xcooi >= xminwindow * parameter[0].sizemagnif) && (xcooi <= xmaxwindow * parameter[0].sizemagnif)
                            && (ycooi >= yminwindow * parameter[0].sizemagnif)
                            && (ycooi <= ymaxwindow * parameter[0].sizemagnif)))) {  // output only if tree density values >0
                    fprintf(filepointer, "%d;", parameter[0].repeati);
                    // fprintf(filepointer, "%d;", pEnvirgrid.yworldcoo);
                    // fprintf(filepointer, "%d;", pEnvirgrid.xworldcoo);
                    fprintf(filepointer, "%4.4f;", xcooi);
                    fprintf(filepointer, "%4.4f;", ycooi);
                    fprintf(filepointer, "%4.5f;", (double)pEnvirgrid.Treedensityvalue / 10000);
                    fprintf(filepointer, "%d;", pEnvirgrid.Treenumber);
                    // fprintf(filepointer, "%u;", pEnvirgrid.litterheight);
                    // fprintf(filepointer, "%u;", pEnvirgrid.litterheightmean);
                    fprintf(filepointer, "%u;", pEnvirgrid.maxthawing_depth);
                    fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.elevation / 10);
                    fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.envirgrowthimpact / 10000);
                    // fprintf(filepointer, "%d;", parameter[0].weatherchoice);
                    // fprintf(filepointer, "%d;", parameter[0].thawing_depth);
                    fprintf(filepointer, "\n");
                }
            }

            fclose(filepointer);
        }  // tree density map output
    }      // world list
}
