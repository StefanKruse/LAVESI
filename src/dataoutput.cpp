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
                vector<vector<Evaluation>>& world_evaluation_list,
				vector<vector<Cryogrid>>& world_cryo_list) {
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
    bool outputcryogrid = false;

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
		
		vector<vector<Cryogrid> >::iterator worldposcryogrid = (world_cryo_list.begin()+aktort);
		vector<Cryogrid>& cryo_list = *worldposcryogrid;

        aktort++;

        // calculation of the current location in a world grid // currently not used
         int aktortyworldcoo = (aktort - 1) / parameter[0].mapxlength;
         int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);


        // define output based on parameter setting
        if (parameter[0].dataoutput == true) {
            if (parameter[0].outputmode == 0) {  // "full"
                if (parameter[0].ivort == 1)  // write full Envirgrid once on sim start
                    ausgabedensity = true;

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
            } else if (parameter[0].outputmode == 99) {  // "test"
                if ( (parameter[0].ivort % 50 == 0) || (parameter[0].ivort >= 2005) ) {
                // if ( (parameter[0].ivort % 20 == 0) | (parameter[0].ivort % 21 == 0) | (parameter[0].ivort % 24 == 0) | (parameter[0].ivort % 25 == 0) ) {
                // if ( (parameter[0].ivort % 5 == 0) ) {
                // if ( (parameter[0].ivort == 19) | (parameter[0].ivort == 20) | (parameter[0].ivort == 21) ) {
					outputcurrencies = true;
                    ausgabedensity = true;
                    outputgriddedbiomass = true;
                    outputindividuals = true;
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
             } else if (parameter[0].outputmode == 14) {  // "normal,gridded,large area" for Cryogrid coupling
                // outputcurrencies = true;
				
				// if(parameter[0].ivort == 1)// write full Envirgrid once on sim start
				if( (parameter[0].ivort == 1) || (parameter[0].ivort >= 2014) ) {// write full Envirgrid once on sim start
					ausgabedensity = true;
                    outputindividuals = true;
				}
				if( (parameter[0].ivort % 50 == 0) || (parameter[0].ivort >= 2005) )// test
					outputcryogrid = true;

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
            for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];

				if (tree.growing == true) {
					if (((double)tree.xcoo / 1000 >= xminwindow) && ((double)tree.xcoo / 1000 <= xmaxwindow) && ((double)tree.ycoo / 1000 >= yminwindow)
						&& ((double)tree.ycoo / 1000 <= ymaxwindow)) {  // loop over reduced plot
						// basal area as population size identifier
						if ((double)tree.height / 10 >= 130) {
							local_basalarea += (M_PI * pow((tree.dbreast / 2), 2));
							local_stemcount++;
						}

						// population sizes in height classes
						if ((double)tree.height / 10 <= 40) {
							local_nheight0b40++;
						} else if (((double)tree.height / 10 > 40) && ((double)tree.height / 10 <= 200)) {
							local_nheight41b200++;
						} else if ((double)tree.height / 10 > 200) {
							local_nheight201b10000++;
							local_meantreeheight += (double)tree.height / 10;
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
                 fprintf(filepointer, "YPLOTPOS;");
                 fprintf(filepointer, "XPLOTPOS;");
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
				fprintf(filepointer, "Seedweight_0;");
				fprintf(filepointer, "Drought_0;");
				fprintf(filepointer, "Selfing_0;");
				fprintf(filepointer, "Inbreedingdepression_0;");
				fprintf(filepointer, "origin_0;");
                fprintf(filepointer, "Age_Lsibi_0;");
                fprintf(filepointer, "Age_Lgmel_1;");
				fprintf(filepointer, "Seedweight_1;");
				fprintf(filepointer, "Drought_1;");
				fprintf(filepointer, "Selfing_1;");
				fprintf(filepointer, "Inbreedingdepression_1;");
				fprintf(filepointer, "origin_1;");
                fprintf(filepointer, "Age_Lsibi_1;");
                fprintf(filepointer, "Age_Lgmel_2;");
				fprintf(filepointer, "Seedweight_2;");
				fprintf(filepointer, "Drought_2;");
				fprintf(filepointer, "Selfing_2;");
				fprintf(filepointer, "Inbreedingdepression_2;");
				fprintf(filepointer, "origin_2;");
                fprintf(filepointer, "Age_Lsibi_2;");
                fprintf(filepointer, "Age_Lgmel_3;");
				fprintf(filepointer, "Seedweight_3;");
				fprintf(filepointer, "Drought_3;");
				fprintf(filepointer, "Selfing_3;");
				fprintf(filepointer, "Inbreedingdepression_3;");
				fprintf(filepointer, "origin_3;");
                fprintf(filepointer, "Age_Lsibi_3;");
                fprintf(filepointer, "Age_Lgmel_4;");
				fprintf(filepointer, "Seedweight_4;");
				fprintf(filepointer, "Drought_4;");
				fprintf(filepointer, "Selfing_4;");
				fprintf(filepointer, "Inbreedingdepression_4;");
				fprintf(filepointer, "origin_4;");
                fprintf(filepointer, "Age_Lsibi_4;");
                fprintf(filepointer, "Age_Lgmel_5;");
				fprintf(filepointer, "Seedweight_5;");
				fprintf(filepointer, "Drought_5;");
				fprintf(filepointer, "Selfing_5;");
				fprintf(filepointer, "Inbreedingdepression_5;");
				fprintf(filepointer, "origin_5;");
                fprintf(filepointer, "Age_Lsibi_5;");
                fprintf(filepointer, "Age_Lgmel_6to10;");
				fprintf(filepointer, "Seedweight_6to10;");
				fprintf(filepointer, "Drought_6to10;");
				fprintf(filepointer, "Selfing_6to10;");
				fprintf(filepointer, "Inbreedingdepression_6to10;");
				fprintf(filepointer, "origin_6to10;");
                fprintf(filepointer, "Age_Lsibi_6to10;");
                fprintf(filepointer, "Age_Lgmel_11to20;");
				fprintf(filepointer, "Seedweight_11to20;");
				fprintf(filepointer, "Drought_11to20;");
				fprintf(filepointer, "Selfing_11to20;");
				fprintf(filepointer, "Inbreedingdepression_11to20;");
				fprintf(filepointer, "origin_11to20;");
                fprintf(filepointer, "Age_Lsibi_11to20;");
                fprintf(filepointer, "Age_Lgmel_21to50;");
				fprintf(filepointer, "Seedweight_21to50;");
				fprintf(filepointer, "Drought_21to50;");
				fprintf(filepointer, "Selfing_21to50;");
				fprintf(filepointer, "Inbreedingdepression_21to50;");
				fprintf(filepointer, "origin_21to50;");
                fprintf(filepointer, "Age_Lsibi_21to50;");
                fprintf(filepointer, "Age_Lgmel_51to100;");
				fprintf(filepointer, "Seedweight_51to100;");
				fprintf(filepointer, "Drought_51to100;");
				fprintf(filepointer, "Selfing_51to100;");
				fprintf(filepointer, "Inbreedingdepression_51to100;");
				fprintf(filepointer, "origin_51to100;");
                fprintf(filepointer, "Age_Lsibi_51to100;");
                fprintf(filepointer, "Age_Lgmel_101to150;");
				fprintf(filepointer, "Seedweight_101to150;");
				fprintf(filepointer, "Drought_101to150;");
				fprintf(filepointer, "Selfing_101to150;");
				fprintf(filepointer, "Inbreedingdepression_101to150;");
				fprintf(filepointer, "origin_101to150;");
                fprintf(filepointer, "Age_Lsibi_101to150;");
                fprintf(filepointer, "Age_Lgmel_151to200;");
				fprintf(filepointer, "Seedweight_151to200;");
				fprintf(filepointer, "Drought_151to200;");
				fprintf(filepointer, "Selfing_151to200;");
				fprintf(filepointer, "Inbreedingdepression_151to200;");
				fprintf(filepointer, "origin_151to200;");
                fprintf(filepointer, "Age_Lsibi_151to200;");
                fprintf(filepointer, "Age_Lgmel_201to300;");
				fprintf(filepointer, "Seedweight_201to300;");
				fprintf(filepointer, "Drought_201to300;");
				fprintf(filepointer, "Selfing_201to300;");
				fprintf(filepointer, "Inbreedingdepression_201to300;");
				fprintf(filepointer, "origin_201to300;");
                fprintf(filepointer, "Age_Lsibi_201to300;");
                fprintf(filepointer, "Age_Lgmel_301to400;");
				fprintf(filepointer, "Seedweight_301to400;");
				fprintf(filepointer, "Drought_301to400;");
				fprintf(filepointer, "Selfing_301to400;");
				fprintf(filepointer, "Inbreedingdepression_301to400;");
				fprintf(filepointer, "origin_301to400;");
                fprintf(filepointer, "Age_Lsibi_301to400;");
                fprintf(filepointer, "Age_Lgmel_401to500;");
				fprintf(filepointer, "Seedweight_401to500;");
				fprintf(filepointer, "Drought_401to500;");
				fprintf(filepointer, "Selfing_401to500;");
				fprintf(filepointer, "Inbreedingdepression_401to500;");
				fprintf(filepointer, "origin_401to500;");
                fprintf(filepointer, "Age_Lsibi_401to500;");
                fprintf(filepointer, "Age_Lgmel_501to600;");
				fprintf(filepointer, "Seedweight_501to600;");
				fprintf(filepointer, "Drought_501to600;");
				fprintf(filepointer, "Selfing_501to600;");
				fprintf(filepointer, "Inbreedingdepression_501to600;");
				fprintf(filepointer, "origin_501to600;");
                fprintf(filepointer, "Age_Lsibi_501to600;");
                fprintf(filepointer, "Age_Lgmel_601to700;");
				fprintf(filepointer, "Seedweight_601to700;");
				fprintf(filepointer, "Drought_601to700;");
				fprintf(filepointer, "Selfing_601to700;");
				fprintf(filepointer, "Inbreedingdepression_601to700;");
				fprintf(filepointer, "origin_601to700;");
                fprintf(filepointer, "Age_Lsibi_601to700;");
                fprintf(filepointer, "Age_Lgmel_701plus;");
				fprintf(filepointer, "Seedweight_701plus;");
				fprintf(filepointer, "Drought_701plus;");
				fprintf(filepointer, "Selfing_701plus;");
				fprintf(filepointer, "Inbreedingdepression_701plus;");
				fprintf(filepointer, "origin_701plus;");
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
             fprintf(filepointer, "%d;", aktortyworldcoo);
             fprintf(filepointer, "%d;", aktortxworldcoo);
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
			double seedweightage0=0, seedweightage1=0, seedweightage2=0, seedweightage3=0, seedweightage4=0, seedweightage5=0, seedweightage6b10=0, seedweightage11b20=0, seedweightage21b50=0, seedweightage51b100=0, seedweightage101b150=0, seedweightage151b200=0, seedweightage201b300=0, seedweightage301b400=0, seedweightage401b500=0, seedweightage501b600=0, seedweightage601b700=0, seedweightage701plus=0;
			double droughtage0=0, droughtage1=0, droughtage2=0, droughtage3=0, droughtage4=0, droughtage5=0, droughtage6b10=0, droughtage11b20=0, droughtage21b50=0, droughtage51b100=0, droughtage101b150=0, droughtage151b200=0, droughtage201b300=0, droughtage301b400=0, droughtage401b500=0, droughtage501b600=0, droughtage601b700=0, droughtage701plus=0;
			double selfingage0=0, selfingage1=0, selfingage2=0, selfingage3=0, selfingage4=0, selfingage5=0, selfingage6b10=0, selfingage11b20=0, selfingage21b50=0, selfingage51b100=0, selfingage101b150=0, selfingage151b200=0, selfingage201b300=0, selfingage301b400=0, selfingage401b500=0, selfingage501b600=0, selfingage601b700=0, selfingage701plus=0;
            double inbreedage0=0, inbreedage1=0, inbreedage2=0, inbreedage3=0, inbreedage4=0, inbreedage5=0, inbreedage6b10=0, inbreedage11b20=0, inbreedage21b50=0, inbreedage51b100=0, inbreedage101b150=0, inbreedage151b200=0, inbreedage201b300=0, inbreedage301b400=0, inbreedage401b500=0, inbreedage501b600=0, inbreedage601b700=0, inbreedage701plus=0;
			double originage0=0, originage1=0, originage2=0, originage3=0, originage4=0, originage5=0, originage6b10=0, originage11b20=0, originage21b50=0, originage51b100=0, originage101b150=0, originage151b200=0, originage201b300=0, originage301b400=0, originage401b500=0, originage501b600=0, originage601b700=0, originage701plus=0;
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
			double localseedweightage0=0, localseedweightage1=0, localseedweightage2=0, localseedweightage3=0, localseedweightage4=0, localseedweightage5=0, localseedweightage6b10=0, localseedweightage11b20=0, localseedweightage21b50=0, localseedweightage51b100=0, localseedweightage101b150=0, localseedweightage151b200=0, localseedweightage201b300=0, localseedweightage301b400=0, localseedweightage401b500=0, localseedweightage501b600=0, localseedweightage601b700=0, localseedweightage701plus=0;
			double localdroughtage0=0, localdroughtage1=0, localdroughtage2=0, localdroughtage3=0, localdroughtage4=0, localdroughtage5=0, localdroughtage6b10=0, localdroughtage11b20=0, localdroughtage21b50=0, localdroughtage51b100=0, localdroughtage101b150=0, localdroughtage151b200=0, localdroughtage201b300=0, localdroughtage301b400=0, localdroughtage401b500=0, localdroughtage501b600=0, localdroughtage601b700=0, localdroughtage701plus=0;
			double localselfingage0=0, localselfingage1=0, localselfingage2=0, localselfingage3=0, localselfingage4=0, localselfingage5=0, localselfingage6b10=0, localselfingage11b20=0, localselfingage21b50=0, localselfingage51b100=0, localselfingage101b150=0, localselfingage151b200=0, localselfingage201b300=0, localselfingage301b400=0, localselfingage401b500=0, localselfingage501b600=0, localselfingage601b700=0, localselfingage701plus=0;
            double localinbreedage0=0, localinbreedage1=0, localinbreedage2=0, localinbreedage3=0, localinbreedage4=0, localinbreedage5=0, localinbreedage6b10=0, localinbreedage11b20=0, localinbreedage21b50=0, localinbreedage51b100=0, localinbreedage101b150=0, localinbreedage151b200=0, localinbreedage201b300=0, localinbreedage301b400=0, localinbreedage401b500=0, localinbreedage501b600=0, localinbreedage601b700=0, localinbreedage701plus=0;
			double localoriginage0=0, localoriginage1=0, localoriginage2=0, localoriginage3=0, localoriginage4=0, localoriginage5=0, localoriginage6b10=0, localoriginage11b20=0, localoriginage21b50=0, localoriginage51b100=0, localoriginage101b150=0, localoriginage151b200=0, localoriginage201b300=0, localoriginage301b400=0, localoriginage401b500=0, localoriginage501b600=0, localoriginage601b700=0, localoriginage701plus=0;
			double localyposmax = 0.0;
            double localmeanbas = 0.0;
            int localh = 0;

#pragma omp for schedule(guided)
            for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];

				if (tree.growing == true) {
					if (((double)tree.xcoo / 1000 >= xminwindow) && ((double)tree.xcoo / 1000 <= xmaxwindow) && ((double)tree.ycoo / 1000 >= yminwindow)
						&& ((double)tree.ycoo / 1000 <= ymaxwindow)) {
						if (tree.species == 1) {
							// bin trees by age
							if (tree.age == 0) {
								localageg0++;
								localseedweightage0=localseedweightage0+tree.seedweight;
								localdroughtage0=localdroughtage0+tree.droughtresist;
								localselfingage0=localselfingage0+tree.selving;
								localinbreedage0=localinbreedage0+tree.inbreedingdepression;
								localoriginage0=localoriginage0+tree.origin;
							} else if (tree.age == 1) {
								localageg1++;
								localseedweightage1=localseedweightage1+tree.seedweight;
								localdroughtage1=localdroughtage1+tree.droughtresist;
								localselfingage1=localselfingage1+tree.selving;
								localinbreedage1=localinbreedage1+tree.inbreedingdepression;
								localoriginage1=localoriginage1+tree.origin;
							} else if (tree.age == 2) {
								localageg2++;
								localseedweightage2=localseedweightage2+tree.seedweight;
								localdroughtage2=localdroughtage2+tree.droughtresist;
								localselfingage2=localselfingage2+tree.selving;
								localinbreedage2=localinbreedage2+tree.inbreedingdepression;
								localoriginage2=localoriginage2+tree.origin;
							} else if (tree.age == 3) {
								localageg3++;
								localseedweightage3=localseedweightage3+tree.seedweight;
								localdroughtage3=localdroughtage3+tree.droughtresist;
								localselfingage3=localselfingage3+tree.selving;
								localinbreedage3=localinbreedage3+tree.inbreedingdepression;
								localoriginage3=localoriginage3+tree.origin;
							} else if (tree.age == 4) {
								localageg4++;
								localseedweightage4=localseedweightage4+tree.seedweight;
								localdroughtage4=localdroughtage4+tree.droughtresist;
								localselfingage4=localselfingage4+tree.selving;
								localinbreedage4=localinbreedage4+tree.inbreedingdepression;
								localoriginage4=localoriginage4+tree.origin;
							} else if (tree.age == 5) {
								localageg5++;
								localseedweightage5=localseedweightage5+tree.seedweight;
								localdroughtage5=localdroughtage5+tree.droughtresist;
								localselfingage5=localselfingage5+tree.selving;
								localinbreedage5=localinbreedage5+tree.inbreedingdepression;
								localoriginage5=localoriginage5+tree.origin;
							} else if ((tree.age > 5) && (tree.age <= 10)) {
								localageg6b10++;
								localseedweightage6b10=localseedweightage6b10+tree.seedweight;
								localdroughtage6b10=localdroughtage6b10+tree.droughtresist;
								localselfingage6b10=localselfingage6b10+tree.selving;
								localinbreedage6b10=localinbreedage6b10+tree.inbreedingdepression;
								localoriginage6b10=localoriginage6b10+tree.origin;
							} else if ((tree.age > 10) && (tree.age <= 20)) {
								localageg11b20++;
								localseedweightage11b20=localseedweightage11b20+tree.seedweight;
								localdroughtage11b20=localdroughtage11b20+tree.droughtresist;
								localselfingage11b20=localselfingage11b20+tree.selving;
								localinbreedage11b20=localinbreedage11b20+tree.inbreedingdepression;
								localoriginage11b20=localoriginage11b20+tree.origin;
							} else if ((tree.age > 20) && (tree.age <= 50)) {
								localageg21b50++;
								localseedweightage21b50=localseedweightage21b50+tree.seedweight;
								localdroughtage21b50=localdroughtage21b50+tree.droughtresist;
								localselfingage21b50=localselfingage21b50+tree.selving;
								localinbreedage21b50=localinbreedage21b50+tree.inbreedingdepression;
								localoriginage21b50=localoriginage21b50+tree.origin;
							} else if ((tree.age > 50) && (tree.age <= 100)) {
								localageg51b100++;
								localseedweightage51b100=localseedweightage51b100+tree.seedweight;
								localdroughtage51b100=localdroughtage51b100+tree.droughtresist;
								localselfingage51b100=localselfingage51b100+tree.selving;
								localinbreedage51b100=localinbreedage51b100+tree.inbreedingdepression;
								localoriginage51b100=localoriginage51b100+tree.origin;
							} else if ((tree.age > 100) && (tree.age <= 150)) {
								localageg101b150++;
								localseedweightage101b150=localseedweightage101b150+tree.seedweight;
								localdroughtage101b150=localdroughtage101b150+tree.droughtresist;
								localselfingage101b150=localselfingage101b150+tree.selving;
								localinbreedage101b150=localinbreedage101b150+tree.inbreedingdepression;
								localoriginage101b150=localoriginage101b150+tree.origin;
							} else if ((tree.age > 150) && (tree.age <= 200)) {
								localageg151b200++;
								localseedweightage151b200=localseedweightage151b200+tree.seedweight;
								localdroughtage151b200=localdroughtage151b200+tree.droughtresist;
								localselfingage151b200=localselfingage151b200+tree.selving;
								localinbreedage151b200=localinbreedage151b200+tree.inbreedingdepression;
								localoriginage151b200=localoriginage151b200+tree.origin;
							} else if ((tree.age > 200) && (tree.age <= 300)) {
								localageg201b300++;
								localseedweightage201b300=localseedweightage201b300+tree.seedweight;
								localdroughtage201b300=localdroughtage201b300+tree.droughtresist;
								localselfingage201b300=localselfingage201b300+tree.selving;
								localinbreedage201b300=localinbreedage201b300+tree.inbreedingdepression;
								localoriginage201b300=localoriginage201b300+tree.origin;
							} else if ((tree.age > 300) && (tree.age <= 400)) {
								localageg301b400++;
								localseedweightage301b400=localseedweightage301b400+tree.seedweight;
								localdroughtage301b400=localdroughtage301b400+tree.droughtresist;
								localselfingage301b400=localselfingage301b400+tree.selving;
								localinbreedage301b400=localinbreedage301b400+tree.inbreedingdepression;
								localoriginage301b400=localoriginage301b400+tree.origin;
							} else if ((tree.age > 400) && (tree.age <= 500)) {
								localageg401b500++;
								localseedweightage401b500=localseedweightage401b500+tree.seedweight;
								localdroughtage401b500=localdroughtage401b500+tree.droughtresist;
								localselfingage401b500=localselfingage401b500+tree.selving;
								localinbreedage401b500=localinbreedage401b500+tree.inbreedingdepression;
								localoriginage401b500=localoriginage401b500+tree.origin;
							} else if ((tree.age > 500) && (tree.age <= 600)) {
								localageg501b600++;
								localseedweightage501b600=localseedweightage501b600+tree.seedweight;
								localdroughtage501b600=localdroughtage501b600+tree.droughtresist;
								localselfingage501b600=localselfingage501b600+tree.selving;
								localinbreedage501b600=localinbreedage501b600+tree.inbreedingdepression;
								localoriginage501b600=localoriginage501b600+tree.origin;
							} else if ((tree.age > 600) && (tree.age <= 700)) {
								localageg601b700++;
								localseedweightage601b700=localseedweightage601b700+tree.seedweight;
								localdroughtage601b700=localdroughtage601b700+tree.droughtresist;
								localselfingage601b700=localselfingage601b700+tree.selving;
								localinbreedage601b700=localinbreedage601b700+tree.inbreedingdepression;
								localoriginage601b700=localoriginage601b700+tree.origin;
							} else if (tree.age > 700) {
								localageg701plus++;
								localseedweightage701plus=localseedweightage701plus+tree.seedweight;
								localdroughtage701plus=localdroughtage701plus+tree.droughtresist;
								localselfingage701plus=localselfingage701plus+tree.selving;
								localinbreedage701plus=localinbreedage701plus+tree.inbreedingdepression;
								localoriginage701plus=localoriginage701plus+tree.origin;
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
				seedweightage0 += localseedweightage0; 
				droughtage0 += localdroughtage0; 
				selfingage0 += localselfingage0; 
				inbreedage0 += localinbreedage0; 
				originage0 += localoriginage0; 
				ageg1 += localageg1;
				seedweightage1 += localseedweightage1; 
				droughtage1 += localdroughtage1; 
				selfingage1 += localselfingage1; 
				inbreedage1 += localinbreedage1; 
				originage1 += localoriginage1; 
				ageg2 += localageg2; 
				seedweightage2 += localseedweightage2; 
				droughtage2 += localdroughtage2; 
				selfingage2 += localselfingage2; 
				inbreedage2 += localinbreedage2; 
				originage2 += localoriginage2; 
				ageg3 += localageg3; 
				seedweightage3 += localseedweightage3; 
				droughtage3 += localdroughtage3; 
				selfingage3 += localselfingage3; 
				inbreedage3 += localinbreedage3; 
				originage3 += localoriginage3; 
				ageg4 += localageg4; 
				seedweightage4 += localseedweightage4; 
				droughtage4 += localdroughtage4; 
				selfingage4 += localselfingage4; 
				inbreedage4 += localinbreedage4; 
				originage4 += localoriginage4; 
				ageg5 += localageg5; 
				seedweightage5 += localseedweightage5; 
				droughtage5 += localdroughtage5; 
				selfingage5 += localselfingage5; 
				inbreedage5 += localinbreedage5; 
				originage5 += localoriginage5; 
				ageg6b10 += localageg6b10; 
				seedweightage6b10 += localseedweightage6b10; 
				droughtage6b10 += localdroughtage6b10; 
				selfingage6b10 += localselfingage6b10; 
				inbreedage6b10 += localinbreedage6b10; 
				originage6b10 += localoriginage6b10; 
				ageg11b20 += localageg11b20; 
				seedweightage11b20 += localseedweightage11b20; 
				droughtage11b20 += localdroughtage11b20; 
				selfingage11b20 += localselfingage11b20; 
				inbreedage11b20 += localinbreedage11b20; 
				originage11b20 += localoriginage11b20; 
				ageg21b50 += localageg21b50; 
				seedweightage21b50 += localseedweightage21b50; 
				droughtage21b50 += localdroughtage21b50; 
				selfingage21b50 += localselfingage21b50; 
				inbreedage21b50 += localinbreedage21b50; 
				originage21b50 += localoriginage21b50; 
				ageg51b100 += localageg51b100; 
				seedweightage51b100 += localseedweightage51b100; 
				droughtage51b100 += localdroughtage51b100; 
				selfingage51b100 += localselfingage51b100; 
				inbreedage51b100 += localinbreedage51b100; 
				originage51b100 += localoriginage51b100; 
				ageg101b150 += localageg101b150;
				seedweightage101b150 += localseedweightage101b150; 
				droughtage101b150 += localdroughtage101b150; 
				selfingage101b150 += localselfingage101b150; 
				inbreedage101b150 += localinbreedage101b150; 
				originage101b150 += localoriginage101b150; 
                ageg151b200 += localageg151b200;
				seedweightage151b200 += localseedweightage151b200; 
				droughtage151b200 += localdroughtage151b200; 
				selfingage151b200 += localselfingage151b200; 
				inbreedage151b200 += localinbreedage151b200; 
				originage151b200 += localoriginage151b200; 				
				ageg201b300 += localageg201b300; 
				seedweightage201b300 += localseedweightage201b300; 
				droughtage201b300 += localdroughtage201b300; 
				selfingage201b300 += localselfingage201b300; 
				inbreedage201b300 += localinbreedage201b300; 
				originage201b300 += localoriginage201b300; 
				ageg301b400 += localageg301b400;
				seedweightage301b400 += localseedweightage301b400; 
				droughtage301b400 += localdroughtage301b400; 
				selfingage301b400 += localselfingage301b400; 
				inbreedage301b400 += localinbreedage301b400; 
				originage301b400 += localoriginage301b400; 				
				ageg401b500 += localageg401b500;
				seedweightage401b500 += localseedweightage401b500; 
				droughtage401b500 += localdroughtage401b500; 
				selfingage401b500 += localselfingage401b500; 
				inbreedage401b500 += localinbreedage401b500; 
				originage401b500 += localoriginage401b500; 				
				ageg501b600 += localageg501b600;
				seedweightage501b600 += localseedweightage501b600; 
				droughtage501b600 += localdroughtage501b600; 
				selfingage501b600 += localselfingage501b600; 
				inbreedage501b600 += localinbreedage501b600; 
				originage501b600 += localoriginage501b600; 				
				ageg601b700 += localageg601b700;
				seedweightage601b700 += localseedweightage601b700; 
				droughtage601b700 += localdroughtage601b700; 
				selfingage601b700 += localselfingage601b700; 
				inbreedage601b700 += localinbreedage601b700; 
				originage601b700 += localoriginage601b700; 				
				ageg701plus += localageg701plus;
				seedweightage701plus += localseedweightage701plus; 
				droughtage701plus += localdroughtage701plus; 
				selfingage701plus += localselfingage701plus; 
				inbreedage701plus += localinbreedage701plus; 
				originage701plus += localoriginage701plus; 
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
            for (unsigned long long int i = 0; i < (unsigned long long int) seed_list.size(); ++i) {
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
			fprintf(filepointer, "%4.4f;", seedweightage0 / ((double)ageg0));
			fprintf(filepointer, "%4.4f;", droughtage0 / ((double)ageg0));
			fprintf(filepointer, "%4.4f;", selfingage0 / ((double)ageg0));
			fprintf(filepointer, "%4.4f;", inbreedage0 / ((double)ageg0));
			fprintf(filepointer, "%4.4f;", originage0 / ((double)ageg0));
            fprintf(filepointer, "%d;", ages0);
            fprintf(filepointer, "%d;", ageg1);
			fprintf(filepointer, "%4.4f;", seedweightage1 / ((double)ageg1));
			fprintf(filepointer, "%4.4f;", droughtage1 / ((double)ageg1));
			fprintf(filepointer, "%4.4f;", selfingage1 / ((double)ageg1));
			fprintf(filepointer, "%4.4f;", inbreedage1 / ((double)ageg1));
			fprintf(filepointer, "%4.4f;", originage1 / ((double)ageg1));
            fprintf(filepointer, "%d;", ages1);
            fprintf(filepointer, "%d;", ageg2);
			fprintf(filepointer, "%4.4f;", seedweightage2 / ((double)ageg2));
			fprintf(filepointer, "%4.4f;", droughtage2 / ((double)ageg2));
			fprintf(filepointer, "%4.4f;", selfingage2 / ((double)ageg2));
			fprintf(filepointer, "%4.4f;", inbreedage2 / ((double)ageg2));
			fprintf(filepointer, "%4.4f;", originage2 / ((double)ageg2));
            fprintf(filepointer, "%d;", ages2);
            fprintf(filepointer, "%d;", ageg3);
			fprintf(filepointer, "%4.4f;", seedweightage3 / ((double)ageg3));
			fprintf(filepointer, "%4.4f;", droughtage3 / ((double)ageg3));
			fprintf(filepointer, "%4.4f;", selfingage3 / ((double)ageg3));
			fprintf(filepointer, "%4.4f;", inbreedage3 / ((double)ageg3));
			fprintf(filepointer, "%4.4f;", originage3 / ((double)ageg3));
            fprintf(filepointer, "%d;", ages3);
            fprintf(filepointer, "%d;", ageg4);
			fprintf(filepointer, "%4.4f;", seedweightage4 / ((double)ageg4));
			fprintf(filepointer, "%4.4f;", droughtage4 / ((double)ageg4));
			fprintf(filepointer, "%4.4f;", selfingage4 / ((double)ageg4));
			fprintf(filepointer, "%4.4f;", inbreedage4 / ((double)ageg4));
			fprintf(filepointer, "%4.4f;", originage4 / ((double)ageg4));
            fprintf(filepointer, "%d;", ages4);
            fprintf(filepointer, "%d;", ageg5);
			fprintf(filepointer, "%4.4f;", seedweightage5 / ((double)ageg5));
			fprintf(filepointer, "%4.4f;", droughtage5 / ((double)ageg5));
			fprintf(filepointer, "%4.4f;", selfingage5 / ((double)ageg5));
			fprintf(filepointer, "%4.4f;", inbreedage5 / ((double)ageg5));
			fprintf(filepointer, "%4.4f;", originage5 / ((double)ageg5));
            fprintf(filepointer, "%d;", ages5);
            fprintf(filepointer, "%d;", ageg6b10);
			fprintf(filepointer, "%4.4f;", seedweightage6b10 / ((double)ageg6b10));
			fprintf(filepointer, "%4.4f;", droughtage6b10 / ((double)ageg6b10));
			fprintf(filepointer, "%4.4f;", selfingage6b10 / ((double)ageg6b10));
			fprintf(filepointer, "%4.4f;", inbreedage6b10 / ((double)ageg6b10));
			fprintf(filepointer, "%4.4f;", originage6b10 / ((double)ageg6b10));
            fprintf(filepointer, "%d;", ages6b10);
            fprintf(filepointer, "%d;", ageg11b20);
			fprintf(filepointer, "%4.4f;", seedweightage11b20 / ((double)ageg11b20));
			fprintf(filepointer, "%4.4f;", droughtage11b20 / ((double)ageg11b20));
			fprintf(filepointer, "%4.4f;", selfingage11b20 / ((double)ageg11b20));
			fprintf(filepointer, "%4.4f;", inbreedage11b20 / ((double)ageg11b20));
			fprintf(filepointer, "%4.4f;", originage11b20 / ((double)ageg11b20));
            fprintf(filepointer, "%d;", ages11b20);
            fprintf(filepointer, "%d;", ageg21b50);
			fprintf(filepointer, "%4.4f;", seedweightage21b50 / ((double)ageg21b50));
			fprintf(filepointer, "%4.4f;", droughtage21b50 / ((double)ageg21b50));
			fprintf(filepointer, "%4.4f;", selfingage21b50 / ((double)ageg21b50));
			fprintf(filepointer, "%4.4f;", inbreedage21b50 / ((double)ageg21b50));
			fprintf(filepointer, "%4.4f;", originage21b50 / ((double)ageg21b50));
            fprintf(filepointer, "%d;", ages21b50);
            fprintf(filepointer, "%d;", ageg51b100);
			fprintf(filepointer, "%4.4f;", seedweightage51b100 / ((double)ageg51b100));
			fprintf(filepointer, "%4.4f;", droughtage51b100 / ((double)ageg51b100));
			fprintf(filepointer, "%4.4f;", selfingage51b100 / ((double)ageg51b100));
			fprintf(filepointer, "%4.4f;", inbreedage51b100 / ((double)ageg51b100));
			fprintf(filepointer, "%4.4f;", originage51b100 / ((double)ageg51b100));
            fprintf(filepointer, "%d;", ages51b100);
            fprintf(filepointer, "%d;", ageg101b150);
			fprintf(filepointer, "%4.4f;", seedweightage101b150 / ((double)ageg101b150));
			fprintf(filepointer, "%4.4f;", droughtage101b150 / ((double)ageg101b150));
			fprintf(filepointer, "%4.4f;", selfingage101b150 / ((double)ageg101b150));
			fprintf(filepointer, "%4.4f;", inbreedage101b150 / ((double)ageg101b150));
			fprintf(filepointer, "%4.4f;", originage101b150 / ((double)ageg101b150));
            fprintf(filepointer, "%d;", ages101b150);
            fprintf(filepointer, "%d;", ageg151b200);
			fprintf(filepointer, "%4.4f;", seedweightage151b200 / ((double)ageg151b200));
			fprintf(filepointer, "%4.4f;", droughtage151b200 / ((double)ageg151b200));
			fprintf(filepointer, "%4.4f;", selfingage151b200 / ((double)ageg151b200));
			fprintf(filepointer, "%4.4f;", inbreedage151b200 / ((double)ageg151b200));
			fprintf(filepointer, "%4.4f;", originage151b200 / ((double)ageg151b200));
            fprintf(filepointer, "%d;", ages151b200);
            fprintf(filepointer, "%d;", ageg201b300);
			fprintf(filepointer, "%4.4f;", seedweightage201b300 / ((double)ageg201b300));
			fprintf(filepointer, "%4.4f;", droughtage201b300 / ((double)ageg201b300));
			fprintf(filepointer, "%4.4f;", selfingage201b300 / ((double)ageg201b300));
			fprintf(filepointer, "%4.4f;", inbreedage201b300 / ((double)ageg201b300));
			fprintf(filepointer, "%4.4f;", originage201b300 / ((double)ageg201b300));
            fprintf(filepointer, "%d;", ages201b300);
            fprintf(filepointer, "%d;", ageg301b400);
			fprintf(filepointer, "%4.4f;", seedweightage301b400 / ((double)ageg301b400));
			fprintf(filepointer, "%4.4f;", droughtage301b400 / ((double)ageg301b400));
			fprintf(filepointer, "%4.4f;", selfingage301b400 / ((double)ageg301b400));
			fprintf(filepointer, "%4.4f;", inbreedage301b400 / ((double)ageg301b400));
			fprintf(filepointer, "%4.4f;", originage301b400 / ((double)ageg301b400));
            fprintf(filepointer, "%d;", ages301b400);
            fprintf(filepointer, "%d;", ageg401b500);
			fprintf(filepointer, "%4.4f;", seedweightage401b500 / ((double)ageg401b500));
			fprintf(filepointer, "%4.4f;", droughtage401b500 / ((double)ageg401b500));
			fprintf(filepointer, "%4.4f;", selfingage401b500 / ((double)ageg401b500));
			fprintf(filepointer, "%4.4f;", inbreedage401b500 / ((double)ageg401b500));
			fprintf(filepointer, "%4.4f;", originage401b500 / ((double)ageg401b500));
            fprintf(filepointer, "%d;", ages401b500);
            fprintf(filepointer, "%d;", ageg501b600);
			fprintf(filepointer, "%4.4f;", seedweightage501b600 / ((double)ageg501b600));
			fprintf(filepointer, "%4.4f;", droughtage501b600 / ((double)ageg501b600));
			fprintf(filepointer, "%4.4f;", selfingage501b600 / ((double)ageg501b600));
			fprintf(filepointer, "%4.4f;", inbreedage501b600 / ((double)ageg501b600));
			fprintf(filepointer, "%4.4f;", originage501b600 / ((double)ageg501b600));
            fprintf(filepointer, "%d;", ages501b600);
            fprintf(filepointer, "%d;", ageg601b700);
			fprintf(filepointer, "%4.4f;", seedweightage601b700 / ((double)ageg601b700));
			fprintf(filepointer, "%4.4f;", droughtage601b700 / ((double)ageg601b700));
			fprintf(filepointer, "%4.4f;", selfingage601b700 / ((double)ageg601b700));
			fprintf(filepointer, "%4.4f;", inbreedage601b700 / ((double)ageg601b700));
			fprintf(filepointer, "%4.4f;", originage601b700 / ((double)ageg601b700));
            fprintf(filepointer, "%d;", ages601b700);
            fprintf(filepointer, "%d;", ageg701plus);
			fprintf(filepointer, "%4.4f;", seedweightage701plus / ((double)ageg701plus));
			fprintf(filepointer, "%4.4f;", droughtage701plus / ((double)ageg701plus));
			fprintf(filepointer, "%4.4f;", selfingage701plus / ((double)ageg701plus));
			fprintf(filepointer, "%4.4f;", inbreedage701plus / ((double)ageg701plus));
			fprintf(filepointer, "%4.4f;", originage701plus / ((double)ageg701plus));
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
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].degreday);
            fprintf(filepointer, "%4.2f;", weather_list[yearposition].precipitationsum);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].weatherfactor[1]);//TODO: add here for all species output
            fprintf(filepointer, "%4.4f;", weather_list[yearposition].weatherfactor[2]);
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
            if (parameter[0].ivort == 1 || (parameter[0].spinupphase == true && (parameter[0].ivort % 100) == 0) || (parameter[0].spinupphase == false)) {
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

                    for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                        auto& tree = tree_list[tree_i];

						if (tree.growing == true) {
							// fprintf(filepointer, "%d;", tree.name);
							// fprintf(filepointer, "%d;", tree.namem);
							fprintf(filepointer, "%4.4f;", tree.dbasal);
							fprintf(filepointer, "%4.4f;", tree.dbreast);
							fprintf(filepointer, "%4.4f;", (double)tree.height / 10);
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
                        fprintf(filepointer, "YPLOTPOS;");
                        fprintf(filepointer, "XPLOTPOS;");
                        fprintf(filepointer, "Year;");
                        fprintf(filepointer, "Random_year;");
                        fprintf(filepointer, "Weather_type;");
                        fprintf(filepointer, "Diameter_basal;");
                        fprintf(filepointer, "Diameter_breast;");
                        fprintf(filepointer, "Height;");
                        fprintf(filepointer, "Age;");
						// fprintf(filepointer, "seednumber;");
						
						fprintf(filepointer, "cloningactive;");
						fprintf(filepointer, "clonetimer;");
						fprintf(filepointer, "cloned;");
						fprintf(filepointer, "seedweight;");
						fprintf(filepointer, "drought;");
						fprintf(filepointer, "selving;");
						fprintf(filepointer, "inbreedingdepression;");
						fprintf(filepointer, "1inbreedingdepression;");
						fprintf(filepointer, "2inbreedingdepression;");
						fprintf(filepointer, "3inbreedingdepression;");
						fprintf(filepointer, "4inbreedingdepression;");
						fprintf(filepointer, "5inbreedingdepression;");
						fprintf(filepointer, "6inbreedingdepression;");
						fprintf(filepointer, "7inbreedingdepression;");
						fprintf(filepointer, "8inbreedingdepression;");
						fprintf(filepointer, "9inbreedingdepression;");
						fprintf(filepointer, "10inbreedingdepression;");
						fprintf(filepointer, "11inbreedingdepression;");
						fprintf(filepointer, "12inbreedingdepression;");
						fprintf(filepointer, "13inbreedingdepression;");
						fprintf(filepointer, "14inbreedingdepression;");
						fprintf(filepointer, "15inbreedingdepression;");
						fprintf(filepointer, "16inbreedingdepression;");
						fprintf(filepointer, "17inbreedingdepression;");
						fprintf(filepointer, "18inbreedingdepression;");
						fprintf(filepointer, "19inbreedingdepression;");
						fprintf(filepointer, "20inbreedingdepression;");
						fprintf(filepointer, "21inbreedingdepression;");
						fprintf(filepointer, "22inbreedingdepression;");
						fprintf(filepointer, "23inbreedingdepression;");
						fprintf(filepointer, "24inbreedingdepression;");
                        fprintf(filepointer, "X;");
                        fprintf(filepointer, "Y;");
                        fprintf(filepointer, "Density_value;");
                         fprintf(filepointer, "Name;");
						fprintf(filepointer, "NameM;");
						fprintf(filepointer, "NameP;");
						// fprintf(filepointer, "Line;");
						fprintf(filepointer, "Generation;");
						fprintf(filepointer, "origin;");
						fprintf(filepointer, "Species;");
						fprintf(filepointer, "Cone;");
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

                    for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                        auto& tree = tree_list[tree_i];

						if (tree.growing == true) {
							fprintf(filepointer, "%d;", parameter[0].repeati);
							fprintf(filepointer, "%d;", tree.yworldcoo);
							fprintf(filepointer, "%d;", tree.xworldcoo);
							fprintf(filepointer, "%d;", parameter[0].ivort);
							fprintf(filepointer, "%d;", jahr);
							fprintf(filepointer, "%d;", parameter[0].weatherchoice);
							fprintf(filepointer, "%4.4f;", tree.dbasal);
							fprintf(filepointer, "%4.4f;", tree.dbreast);
							fprintf(filepointer, "%4.4f;", (double)tree.height / 10);
							fprintf(filepointer, "%d;", tree.age);
							// fprintf(filepointer, "%4.4f;", tree.seednumber);
							
							fprintf(filepointer, "%d;", tree.cloningactive);
							fprintf(filepointer, "%d;", tree.clonetimer);
							fprintf(filepointer, "%d;", tree.cloned);
							fprintf(filepointer, "%4.4f;", tree.seedweight);
							fprintf(filepointer, "%4.4f;", tree.droughtresist);
							fprintf(filepointer, "%4.4f;", tree.selving);
							fprintf(filepointer, "%4.4f;", tree.inbreedingdepression);
							fprintf(filepointer, "%d;", tree.neutralmarkers[0]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[1]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[2]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[3]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[4]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[5]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[6]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[7]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[8]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[9]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[10]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[11]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[12]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[13]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[14]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[15]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[16]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[17]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[18]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[19]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[20]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[21]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[22]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[23]);
							fprintf(filepointer, "%4.4f;", (double)tree.xcoo / 1000);
							fprintf(filepointer, "%4.4f;", (double)tree.ycoo / 1000);
							fprintf(filepointer, "%4.5f;", tree.densitywert);
							 fprintf(filepointer, "%d;", tree.name);
							fprintf(filepointer, "%d;", tree.namem);
							fprintf(filepointer, "%d;", tree.namep);
							// fprintf(filepointer, "%d;", tree.line);
							fprintf(filepointer, "%d;", tree.generation);
							fprintf(filepointer, "%f;", tree.origin);
							fprintf(filepointer, "%d;", tree.species);
							fprintf(filepointer, "%d;", tree.cone);
							fprintf(filepointer, "%4.4f;", (double)tree.coneheight);
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
                fprintf(filepointer, "YPLOTPOS;");
                fprintf(filepointer, "XPLOTPOS;");
                // fprintf(filepointer, "Weather_type;");
                // time variables
                // fprintf(filepointer, "Progress;");
                // fprintf(filepointer, "Year;");
                // tree variables
                fprintf(filepointer, "X;");
                fprintf(filepointer, "Y;");
                fprintf(filepointer, "Name;");
                 fprintf(filepointer, "NameM;");
                 fprintf(filepointer, "NameP;");
                // fprintf(filepointer, "Line;");
                 fprintf(filepointer, "Generation;");
				 fprintf(filepointer, "origin;");
                fprintf(filepointer, "Species;");
                fprintf(filepointer, "Height;");
                fprintf(filepointer, "Dbasal;");
                fprintf(filepointer, "Dbreast;");
                fprintf(filepointer, "Age;");
				// fprintf(filepointer, "seednumber;");
				
						
						fprintf(filepointer, "cloningactive;");
						fprintf(filepointer, "clonetimer;");
						fprintf(filepointer, "cloned;");
						fprintf(filepointer, "seedweight;");
						fprintf(filepointer, "drought;");
						fprintf(filepointer, "selving;");
						fprintf(filepointer, "inbreedingdepression;");
						fprintf(filepointer, "1inbreedingdepression;");
						fprintf(filepointer, "2inbreedingdepression;");
						fprintf(filepointer, "3inbreedingdepression;");
						fprintf(filepointer, "4inbreedingdepression;");
						fprintf(filepointer, "5inbreedingdepression;");
						fprintf(filepointer, "6inbreedingdepression;");
						fprintf(filepointer, "7inbreedingdepression;");
						fprintf(filepointer, "8inbreedingdepression;");
						fprintf(filepointer, "9inbreedingdepression;");
						fprintf(filepointer, "10inbreedingdepression;");
						fprintf(filepointer, "11inbreedingdepression;");
						fprintf(filepointer, "12inbreedingdepression;");
						fprintf(filepointer, "13inbreedingdepression;");
						fprintf(filepointer, "14inbreedingdepression;");
						fprintf(filepointer, "15inbreedingdepression;");
						fprintf(filepointer, "16inbreedingdepression;");						
						fprintf(filepointer, "17inbreedingdepression;");
						fprintf(filepointer, "18inbreedingdepression;");
						fprintf(filepointer, "19inbreedingdepression;");
						fprintf(filepointer, "20inbreedingdepression;");
						fprintf(filepointer, "21inbreedingdepression;");
						fprintf(filepointer, "22inbreedingdepression;");
						fprintf(filepointer, "23inbreedingdepression;");
						fprintf(filepointer, "24inbreedingdepression;");
                 fprintf(filepointer, "Cone;");
                 fprintf(filepointer, "Cone_height;");
                // fprintf(filepointer, "Seeds_produced_currently;");
                // fprintf(filepointer, "Seeds_produced_total;");
                // fprintf(filepointer, "Buffer;");
                // fprintf(filepointer, "Density_value;");
                // fprintf(filepointer, "Distance;");
                fprintf(filepointer, "Thawing_depth_influence;");
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
            for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					// parameters
					// fprintf(filepointer, "%d;", parameter[0].repeati);
					fprintf(filepointer, "%d;", tree.yworldcoo);
					fprintf(filepointer, "%d;", tree.xworldcoo);
					// fprintf(filepointer, "%d;", parameter[0].weatherchoice);
					// time variables
					// fprintf(filepointer, "%d;", t);
					// fprintf(filepointer, "%d;", jahr);
					// tree variables
					fprintf(filepointer, "%4.4f;", (double)tree.xcoo / 1000);
					fprintf(filepointer, "%4.4f;", (double)tree.ycoo / 1000);
					fprintf(filepointer, "%d;", tree.name);
					 fprintf(filepointer, "%d;", tree.namem);
					 fprintf(filepointer, "%d;", tree.namep);
					// fprintf(filepointer, "%d;", tree.line);
					 fprintf(filepointer, "%d;", tree.generation);
					 fprintf(filepointer, "%f;", tree.origin);
					fprintf(filepointer, "%d;", tree.species);
					fprintf(filepointer, "%4.4f;", (double)tree.height / 10);
					fprintf(filepointer, "%4.4f;", tree.dbasal);
					fprintf(filepointer, "%4.4f;", tree.dbreast);
					fprintf(filepointer, "%d;", tree.age);
					// fprintf(filepointer, "%4.4f;", tree.seednumber);
							
							fprintf(filepointer, "%d;", tree.cloningactive);
							fprintf(filepointer, "%d;", tree.clonetimer);
							fprintf(filepointer, "%d;", tree.cloned);
							fprintf(filepointer, "%4.4f;", tree.seedweight);
							fprintf(filepointer, "%4.4f;", tree.droughtresist);
							fprintf(filepointer, "%4.4f;", tree.selving);
							fprintf(filepointer, "%4.4f;", tree.inbreedingdepression);
							fprintf(filepointer, "%d;", tree.neutralmarkers[0]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[1]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[2]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[3]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[4]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[5]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[6]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[7]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[8]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[9]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[10]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[11]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[12]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[13]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[14]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[15]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[16]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[17]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[18]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[19]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[20]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[21]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[22]);
							fprintf(filepointer, "%d;", tree.neutralmarkers[23]);
					fprintf(filepointer, "%d;", tree.cone);
					fprintf(filepointer, "%4.4f;",(double) tree.coneheight);
					// fprintf(filepointer, "%d;", tree.seednewly_produced);
					// fprintf(filepointer, "%d;", tree.seedproduced);
					// fprintf(filepointer, "%d;", tree.buffer);
					// fprintf(filepointer, "%4.5f;", tree.densitywert);
					// fprintf(filepointer, "%4.5f;", tree.dispersaldistance);
					fprintf(filepointer, "%lf;", tree.thawing_depthinfluence);
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
                fprintf(filepointer, "YPLOTPOS;");
                fprintf(filepointer, "XPLOTPOS;");
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
            for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
			
				if ( (tree.growing == true) && (tree.age > 0) ) {// only >0 years to prevent overly large files in very warm scenarios
					/*
					if (   ((double)tree.xcoo / 1000 >= (643515-637008.2)) // currently for Site EN18022 30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m.csv"; //x=18000, y=24720
							// ... x 643515
							// ... y 7479959
							// as 100-m wide transect starting here in tundra and going south into forest for 400 m
						&& ((double)tree.xcoo / 1000 <= (643515+100-637008.2))
						&& ((double)tree.ycoo / 1000 >= (7479959-400-7469996))
						&& ((double)tree.ycoo / 1000 <= (7479959-7469996))) {
					*/
					if (   ((double)tree.xcoo / 1000 >= (643515-640008.2)) // currently for Site EN18022 
					// => 30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m
					// => 30m_Ilirney_x640008.2-649998.2m_y7475006-7494716m smaller area
							// ... x 643515
							// ... y 7479959
							// as 100-m wide transect starting here in tundra and going south into forest for 400 m and north 400 m
						&& ((double)tree.xcoo / 1000 <= (643515+20-640008.2))
						&& ((double)tree.ycoo / 1000 >= (7479959-1400-7475006))
						&& ((double)tree.ycoo / 1000 <= ((7479959+600)-7475006))) {
						// parameters
						// fprintf(filepointer, "%d;", parameter[0].repeati);
						fprintf(filepointer, "%d;", tree.yworldcoo);
						fprintf(filepointer, "%d;", tree.xworldcoo);
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
						fprintf(filepointer, "%4.4f;", (double)tree.height / 10);
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
            int deminputdimension_y = treerows / 25;  // matrix + 1 to avoid border effects
            int deminputdimension_x = treecols / 25;  // matrix + 1 to avoid border effects
            vector<double> AGBneedleliving;
            AGBneedleliving.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<double> AGBwoodliving;
            AGBwoodliving.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<unsigned int> Stemcount;
            Stemcount.resize(deminputdimension_y * deminputdimension_x, 0);
            vector<double> Basalarea;
            Basalarea.resize(deminputdimension_y * deminputdimension_x, 0);
			vector<double> Seedweight;
            Seedweight.resize(deminputdimension_y * deminputdimension_x, 0);
			vector<double> Drought;
            Drought.resize(deminputdimension_y * deminputdimension_x, 0);
			vector<double> Selfing;
            Selfing.resize(deminputdimension_y * deminputdimension_x, 0);
			vector<double> Inbreeding;
            Inbreeding.resize(deminputdimension_y * deminputdimension_x, 0);
			vector<double> Origin;
            Origin.resize(deminputdimension_y * deminputdimension_x, 0);
			vector<unsigned int> Indicount_Total;
			Indicount_Total.resize(deminputdimension_y * deminputdimension_x, 0);
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
            for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					// calculate grid position from x/y coordinates
					unsigned int grid_i = floor((double)tree.ycoo / 1000 / 25) * deminputdimension_x + floor((double)tree.xcoo / 1000 / 25);

					// living branches at the tree 
					double livingtreefraction = (tree.height - tree.crownstart) / tree.height;
					if(livingtreefraction < 0.0)
						livingtreefraction = 0.0;
					else if(livingtreefraction > 1.0)
						livingtreefraction = 1.0;
					// calculate biomass values for each tree
					AGBneedleliving[grid_i] += livingtreefraction * speciestrait[tree.species].biomassleafbase / (1 + exp(-1.0 * ((double)tree.height / 10 - speciestrait[tree.species].biomassleaffaca) / speciestrait[tree.species].biomassleaffacb));
					AGBwoodliving[grid_i] += speciestrait[tree.species].biomasswoodbase / (1 + exp(-1.0 * ((double)tree.height / 10 - speciestrait[tree.species].biomasswoodfaca) / speciestrait[tree.species].biomasswoodfacb));

					// aggregate stand level variables
					if (((double)tree.height / 10) > 130) {
						Stemcount[grid_i]++;
						Basalarea[grid_i] += (M_PI * pow((tree.dbreast / 100 / 2), 2));  // in sq.m so here conversion from cm to m
					}
					Inbreeding[grid_i] += tree.inbreedingdepression;
					Origin[grid_i] += tree.origin;
					Seedweight[grid_i] += tree.seedweight;
					Drought[grid_i] += tree.droughtresist;
					Selfing[grid_i] += tree.selving;
					Indicount_Total[grid_i]++;
					// aggregate further stand structure variables
					if (((double)tree.height / 10) < 10) {
						Indicount_10[grid_i]++;
					} else if ((((double)tree.height / 10) >= 10) & (((double)tree.height / 10) < 40)) {
						Indicount_40[grid_i]++;
					} else if ((((double)tree.height / 10) >= 40) & (((double)tree.height / 10) < 100)) {
						Indicount_100[grid_i]++;
					} else if ((((double)tree.height / 10) >= 100) & (((double)tree.height / 10) < 200)) {
						Indicount_200[grid_i]++;
					} else if ((((double)tree.height / 10) >= 200) & (((double)tree.height / 10) < 300)) {
						Indicount_300[grid_i]++;
					} else if ((((double)tree.height / 10) >= 300) & (((double)tree.height / 10) < 400)) {
						Indicount_400[grid_i]++;
					} else if ((((double)tree.height / 10) >= 400) & (((double)tree.height / 10) < 500)) {
						Indicount_500[grid_i]++;
					} else if ((((double)tree.height / 10) >= 500) & (((double)tree.height / 10) < 750)) {
						Indicount_750[grid_i]++;
					} else if ((((double)tree.height / 10) >= 750) & (((double)tree.height / 10) < 1000)) {
						Indicount_1000[grid_i]++;
					} else if ((((double)tree.height / 10) >= 1000) & (((double)tree.height / 10) < 1250)) {
						Indicount_1250[grid_i]++;
					} else if ((((double)tree.height / 10) >= 1250) & (((double)tree.height / 10) < 1500)) {
						Indicount_1500[grid_i]++;
					} else if ((((double)tree.height / 10) >= 1500) & (((double)tree.height / 10) < 2000)) {
						Indicount_2000[grid_i]++;
					} else if (((double)tree.height / 10) >= 2000) {
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
				fprintf(filepointer, "Inbreedingdepression;");
				fprintf(filepointer, "Origin;");
				fprintf(filepointer, "Seedweight;");
				fprintf(filepointer, "Drought;");
				fprintf(filepointer, "Selfing;");
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
                fprintf(filepointer, "%4.8f;", AGBneedleliving[grid_i] / 1000 / (25 * 25));    // in kg/sq.m.
                fprintf(filepointer, "%4.8f;", AGBwoodliving[grid_i] / 1000 / (25 * 25));      // in kg/sq.m.
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Stemcount[grid_i]);  // in stems/ha
                fprintf(filepointer, "%4.8f;", ((100 * 100) / (25 * 25)) * Basalarea[grid_i]);
				fprintf(filepointer, "%4.8f;", (Inbreeding[grid_i]/Indicount_Total[grid_i]));
				fprintf(filepointer, "%4.8f;", (Origin[grid_i]/Indicount_Total[grid_i]));
				fprintf(filepointer, "%4.8f;", (Seedweight[grid_i]/Indicount_Total[grid_i]));
				fprintf(filepointer, "%4.8f;", (Drought[grid_i]/Indicount_Total[grid_i]));
				fprintf(filepointer, "%4.8f;", (Selfing[grid_i]/Indicount_Total[grid_i]));
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_10[grid_i]);          // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_40[grid_i]);          // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_100[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_200[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_300[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_400[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_500[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_750[grid_i]);         // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_1000[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_1250[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_1500[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_2000[grid_i]);        // in individuals/ha
                fprintf(filepointer, "%d;", (((100 * 100) / (25 * 25))) * Indicount_larger2000[grid_i]);  // in individuals/ha
                fprintf(filepointer, "\n");
                // }
            }

            fclose(filepointer);
        }  // gridded tree biomass output

        if (outputcryogrid == true) {  // gridded tree biomass output
            // assemble file name
            s1 << parameter[0].repeati;
            s2 << parameter[0].weatherchoice;
            s3 << parameter[0].ivort;
            s4 << aktort;
            s5 << parameter[0].cryogrid_scenario;
			if(parameter[0].cryogrid_scenario > 0)
				dateiname = "./output_" + s5.str() + "/datacryogrid_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + ".csv";
			else
				dateiname = "./output/datacryogrid_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + ".csv";
            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();
            s3.str("");
            s3.clear();
            s4.str("");
            s4.clear();
            s5.str("");
            s5.clear();

            // trying to open the file for writing
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                fprintf(filepointer, "x;");
                fprintf(filepointer, "y;");
                fprintf(filepointer, "leafarea;");
                fprintf(filepointer, "stemarea;");
                fprintf(filepointer, "maxtreeheight;");
                fprintf(filepointer, "meantreeheight;");
                fprintf(filepointer, "treecount;");
                fprintf(filepointer, "maxthawing_depth;");
                fprintf(filepointer, "litterheight0;");
                fprintf(filepointer, "soilhumidity;");
                fprintf(filepointer, "envirgridcount");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: File could not be opened!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data output for each grid cell
			double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;
			for (int kartenpos=0; kartenpos < (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)); kartenpos++) {
				auto& pCryogrid = cryo_list[kartenpos];
				
				fprintf(filepointer, "%d;", (int)pCryogrid.xcoo);
				fprintf(filepointer, "%d;", (int)pCryogrid.ycoo);
				fprintf(filepointer, "%4.4f;", pCryogrid.leafarea);
				fprintf(filepointer, "%4.4f;", pCryogrid.stemarea);
				fprintf(filepointer, "%4.2f;", pCryogrid.maxtreeheight);
				fprintf(filepointer, "%4.2f;", pCryogrid.meantreeheight);
				fprintf(filepointer, "%d;", pCryogrid.treecount);
				fprintf(filepointer, "%4.2f;", pCryogrid.maxthawing_depth);
				fprintf(filepointer, "%4.2f;", pCryogrid.litterheight0);
				fprintf(filepointer, "%4.2f;", pCryogrid.soilhumidity);
				fprintf(filepointer, "%d", pCryogrid.envirgridcount);				
				fprintf(filepointer, "\n");
			}
			
            fclose(filepointer);
        }  // gridded output

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
            for (unsigned long long int tree_i = 0; tree_i < (unsigned long long int) tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					unsigned int yposi = tree.ycoo / 1000;

					// aggregate variables
					if (((double)tree.height / 10) > 130) {
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
            s5 << parameter[0].cryogrid_scenario;
			if(parameter[0].cryogrid_scenario > 0)
				dateiname = "./output_" + s5.str() + "/datatrees_Treedensity" + s1.str() + "_" + s2.str() + ".csv";
			else
				dateiname = "./output/datatrees_Treedensity" + s1.str() + "_" + s2.str() + ".csv";
            s1.str("");
            s1.clear();
            s2.str("");
            s2.clear();
            s5.str("");
            s5.clear();

            // trying to open the file for reading
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                fprintf(filepointer, "Repeat;");
                 fprintf(filepointer, "YPLOTPOS;");
                fprintf(filepointer, "XPLOTPOS;");
                fprintf(filepointer, "X;");
                fprintf(filepointer, "Y;");
                fprintf(filepointer, "Density_value;");
                fprintf(filepointer, "N_trees;");
                fprintf(filepointer, "Litter_layer_height;");
                fprintf(filepointer, "Litter_layer_height_mean;");
                fprintf(filepointer, "Max_thawing_depth;");
                fprintf(filepointer, "Elevation;");
                fprintf(filepointer, "TWI;");
                fprintf(filepointer, "Envirgrowthimpact;");
                // fprintf(filepointer, "Weather_type;");
                fprintf(filepointer, "Soilhumidity;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: output file is missing!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data evaluation and output
            for (unsigned long long int kartenpos = 0; kartenpos < ((unsigned long long int)treerows * (unsigned long long int)parameter[0].sizemagnif * (unsigned long long int)treecols * (unsigned long long int)parameter[0].sizemagnif); kartenpos = kartenpos + parameter[0].sizemagnif * parameter[0].demresolution) {
                auto& pEnvirgrid = plot_list[kartenpos];
                double ycooi = floor((double)kartenpos / ((double)treecols * parameter[0].sizemagnif));
                double xcooi = (double)kartenpos - (ycooi * ((double)treecols * parameter[0].sizemagnif));
                if (((parameter[0].demlandscape == true)
                     && ((((xcooi / parameter[0].sizemagnif / parameter[0].demresolution) - floor(xcooi / parameter[0].sizemagnif / parameter[0].demresolution))
                          == 0)
                         && (((ycooi / parameter[0].sizemagnif / parameter[0].demresolution)
                              - floor(ycooi / parameter[0].sizemagnif / parameter[0].demresolution))
                             == 0)))
                    || ((parameter[0].demlandscape == false) && (pEnvirgrid.Treenumber > 0) // output only if tree density values >0
                        && ((xcooi >= xminwindow * parameter[0].sizemagnif) && (xcooi <= xmaxwindow * parameter[0].sizemagnif)
                            && (ycooi >= yminwindow * parameter[0].sizemagnif)
                            && (ycooi <= ymaxwindow * parameter[0].sizemagnif)))
						) {  
                    fprintf(filepointer, "%d;", parameter[0].repeati);
                     fprintf(filepointer, "%d;", pEnvirgrid.yworldcoo);
                     fprintf(filepointer, "%d;", pEnvirgrid.xworldcoo);
                    fprintf(filepointer, "%4.4f;", xcooi / parameter[0].sizemagnif);
                    fprintf(filepointer, "%4.4f;", ycooi / parameter[0].sizemagnif);
                    fprintf(filepointer, "%4.5f;", (double)pEnvirgrid.Treedensityvalue / 10000);
                    fprintf(filepointer, "%d;", pEnvirgrid.Treenumber);
                    fprintf(filepointer, "%4.5f;", (double)pEnvirgrid.litterheight0 / 100); // in cm
                    fprintf(filepointer, "%4.5f;", (double)pEnvirgrid.litterheightmean / 100); // in cm
                    fprintf(filepointer, "%4.5f;", (double)pEnvirgrid.maxthawing_depth / 10);
                    fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.elevation / 10);
                    fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.twi / 100);
                    // fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.slope/ 10);
                    fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.envirgrowthimpact / 10000);
                    // fprintf(filepointer, "%d;", parameter[0].weatherchoice);
                    fprintf(filepointer, "%4.4f;", (double)pEnvirgrid.soilhumidity / 100);
                    fprintf(filepointer, "\n");
                }
            }

            fclose(filepointer);
        }  // tree density map output
    }      // world list
}

