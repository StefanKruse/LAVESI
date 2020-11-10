#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

void Dataoutput(int t,
                int jahr,
                struct Parameter* parameter,
                int yearposition,
                vector<list<Tree*>>& world_tree_list,
                vector<VectorList<Seed>>& world_seed_list,
                vector<vector<Weather*>>& world_weather_list,
                vector<vector<Envirgrid*>>& world_plot_list,
                vector<vector<Evaluation*>>& world_evaluation_list) {
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
    bool ausgabedensity = false;

    // preprocessing and output of data for each plot
    int aktort = 0;
    for (vector<list<Tree*>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {  // world tree list loop
        list<Tree*>& tree_list = *posw;

        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;

        vector<vector<Envirgrid*>>::iterator poskarten = (world_plot_list.begin() + aktort);
        vector<Envirgrid*>& plot_list = *poskarten;

        vector<vector<Weather*>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather*>& weather_list = *posiwelt;

        vector<vector<Evaluation*>>::iterator posiweltausw = (world_evaluation_list.begin() + aktort);
        vector<Evaluation*>& evaluation_list = *posiweltausw;
        vector<Evaluation*>::iterator posausw = evaluation_list.begin();
        auto pEvaluation = (*posausw);

        aktort++;

        // calculation of the current location in a world grid
        // int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        // -- -- -- update evaluation lists -- -- -- --	-- -- -- -- -- //

        // declarations
        double basalarea = 0.0;
        int nheight0b40 = 0, nheight41b200 = 0, nheight201b10000 = 0;
        double breastdiameter = 0.0;
        int breastdiametercount = 0;
        int stemcount = 0;
        double meantreeheight = 0.0, meantreeage = 0.0;

        for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
            auto pTree = (*pos);

            if (((double)pTree->xcoo/1000 >= xminwindow) && ((double)pTree->xcoo/1000 <= xmaxwindow) && ((double)pTree->ycoo/1000 >= yminwindow)
                && ((double)pTree->ycoo/1000 <= ymaxwindow)) {  // loop over reduced plot
                // basal area as population size identifier
                if (pTree->height >= 130) {
                    basalarea += (M_PI * pow((pTree->dbreast / 2), 2));
                    stemcount++;
                }

                // population sizes in height classes
                if (pTree->height <= 40) {
                    nheight0b40++;
                } else if ((pTree->height > 40) && (pTree->height <= 200)) {
                    nheight41b200++;
                } else if (pTree->height > 200) {
                    nheight201b10000++;
                    meantreeheight += (double) pTree->height;
                    meantreeage += (double) pTree->age;
                }

                if (pTree->dbreast > 0) {
                    breastdiameter = breastdiameter + pTree->dbreast;
                    breastdiametercount++;
                }
            }

            ++pos;
        }

        // push back values in evaluation list
        pEvaluation->basalarealist.push_back(basalarea);
        pEvaluation->nheight0b40liste.push_back(nheight0b40);
        pEvaluation->nheight41b200liste.push_back(nheight41b200);
        pEvaluation->nheight201b10000liste.push_back(nheight201b10000);

        if (breastdiametercount > 0) {
            pEvaluation->meanbreastdiameterliste.push_back((breastdiameter / ((double)breastdiametercount)));
        } else {
            pEvaluation->meanbreastdiameterliste.push_back(0.0);
        }

        pEvaluation->stemcountliste.push_back(stemcount);

        if (nheight201b10000 > 0) {
            pEvaluation->meantreeheightliste.push_back(meantreeheight / (double)nheight201b10000);
            pEvaluation->meantreeageliste.push_back(meantreeage / (double)nheight201b10000);
        } else {
            pEvaluation->meantreeheightliste.push_back(0.0);
            pEvaluation->meantreeageliste.push_back(0.0);
        }

        // mean value calculation
        int basalareaanzahl = pEvaluation->basalarealist.size();

        double meanbasalarea = 0;
        double meannheight0b40 = 0, meannheight41b200 = 0, meannheight201b10000 = 0;
        double meanbreastdiameter = 0;
        double meanstemcount = 0;
        double meanmeantreeheigth = 0;
        double meanmeantreeage = 0;
        if (basalareaanzahl == (int)parameter[0].stabilmovingwindow) {  // if exactly parameter[0].stabilmovingwindow values are set
            for (int position = basalareaanzahl - (int)parameter[0].stabilmovingwindow; position < basalareaanzahl; ++position) {
                meanbasalarea += pEvaluation->basalarealist[position] / parameter[0].stabilmovingwindow;
                meannheight0b40 += pEvaluation->nheight0b40liste[position] / parameter[0].stabilmovingwindow;
                meannheight41b200 += pEvaluation->nheight41b200liste[position] / parameter[0].stabilmovingwindow;
                meannheight201b10000 += pEvaluation->nheight201b10000liste[position] / parameter[0].stabilmovingwindow;
                meanbreastdiameter += pEvaluation->meanbreastdiameterliste[position] / parameter[0].stabilmovingwindow;
                meanstemcount += pEvaluation->stemcountliste[position] / parameter[0].stabilmovingwindow;
                meanmeantreeheigth += pEvaluation->meantreeheightliste[position] / parameter[0].stabilmovingwindow;
                meanmeantreeage += pEvaluation->meantreeageliste[position] / parameter[0].stabilmovingwindow;
            }
            pEvaluation->basalarearunmeanlist.push_back(meanbasalarea);
            pEvaluation->nheight0b40runmeanliste.push_back(meannheight0b40);
            pEvaluation->nheight41b200runmeanliste.push_back(meannheight41b200);
            pEvaluation->nheight201b10000runmeanliste.push_back(meannheight201b10000);
            pEvaluation->meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);
            pEvaluation->stemcountrunmeanliste.push_back((int)floor(meanstemcount));
            pEvaluation->meantreeheightrunmeanliste.push_back(meanmeantreeheigth);
            pEvaluation->meantreeagerunmeanliste.push_back(meanmeantreeage);
        } else if (basalareaanzahl > (int)parameter[0].stabilmovingwindow) {  // if more than parameter[0].stabilmovingwindow values are set
            meanbasalarea = pEvaluation->basalarearunmeanlist[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                            - (pEvaluation->basalarealist[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                            + (pEvaluation->basalarealist[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->basalarearunmeanlist.push_back(meanbasalarea);

            meannheight0b40 = pEvaluation->nheight0b40runmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                              - (pEvaluation->nheight0b40liste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                              + (pEvaluation->nheight0b40liste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->nheight0b40runmeanliste.push_back(meannheight0b40);

            meannheight41b200 =
                pEvaluation->nheight41b200runmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation->nheight41b200liste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation->nheight41b200liste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->nheight41b200runmeanliste.push_back(meannheight41b200);

            meannheight201b10000 =
                pEvaluation->nheight201b10000runmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation->nheight201b10000liste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation->nheight201b10000liste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->nheight201b10000runmeanliste.push_back(meannheight201b10000);

            meanbreastdiameter =
                pEvaluation->meanbreastdiameterrunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation->meanbreastdiameterliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation->meanbreastdiameterliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);

            meanstemcount = (double)pEvaluation->stemcountrunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                            - (pEvaluation->stemcountliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                            + (pEvaluation->stemcountliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->stemcountrunmeanliste.push_back((int)floor(meanstemcount));

            meanmeantreeheigth =
                pEvaluation->meantreeheightrunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                - (pEvaluation->meantreeheightliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                + (pEvaluation->meantreeheightliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->meantreeheightrunmeanliste.push_back(meanmeantreeheigth);

            meanmeantreeage = pEvaluation->meantreeagerunmeanliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)]
                              - (pEvaluation->meantreeageliste[basalareaanzahl - ((int)parameter[0].stabilmovingwindow + 1)] / parameter[0].stabilmovingwindow)
                              + (pEvaluation->meantreeageliste[basalareaanzahl - 1] / parameter[0].stabilmovingwindow);
            pEvaluation->meantreeagerunmeanliste.push_back(meanmeantreeage);
        }

        // processing basal area mean values
        int runmeanbasalareaanzahl = pEvaluation->basalarearunmeanlist.size();
        if (runmeanbasalareaanzahl > 1) {  // if more than 2 parameter[0].stabilmovingwindow yearly mean values

            // calculation of the slope
            double slopediff = pEvaluation->basalarearunmeanlist[runmeanbasalareaanzahl - 1] - pEvaluation->basalarearunmeanlist[runmeanbasalareaanzahl - 2];
            if (pEvaluation->postyearofturningpoint != true) {
                // if a maximal slope is not surpassed in 50 years, the turning point of growth is reached
                if (slopediff >= pEvaluation->maxincrementbasalarea) {
                    pEvaluation->maxincrementbasalarea = slopediff;
                    pEvaluation->countermaxincrementbasalarea = 50;
                } else {
                    --pEvaluation->countermaxincrementbasalarea;
                }
                if (pEvaluation->countermaxincrementbasalarea == 0) {
                    pEvaluation->yearofturningpoint = jahr;
                    pEvaluation->postyearofturningpoint = true;
                }
            }
            // after the turning point is reached, the saturation of the system is reached as soon as the slope becomes negative
            else {
                if ((slopediff < 0.0) && (pEvaluation->yearofequilibrium == -9999)) {
                    pEvaluation->yearofequilibrium = jahr;
                }
            }
        }

        // -- --			 evaluation_list update				 -- -- //
        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        if (parameter[0].dataoutput == true) {
            if (parameter[0].outputmode == 0)  // "full"
            {
                if (parameter[0].spinupphase == true) {
                    outputcurrencies = true;
                    outputposition = true;
                } else {
                    outputcurrencies = true;
                    outputposition = true;
                    outputindividuals = true;
                    ausgabedensity = true;
                }
            } else if (parameter[0].outputmode == 1)  // "normal"
            {
                outputcurrencies = true;
				
				if(parameter[0].ivort == 1)// write full Envirgrid once on sim start
					ausgabedensity = true;

                if ((parameter[0].ivort % 25 == 0))
                // || parameter[0].ivort>())
                {
                    outputindividuals = true;
                }

            } else if (parameter[0].outputmode == 2)  // "OMP"
            {
                outputcurrencies = true;
            }
        }
        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

        ostringstream s1, s2, s3, s4, s5, s6, s7, s8;

        if (outputcurrencies == true) {  // currencies output
                                         // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
                                         // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
                                         // -- -- -- -- -- -- -- trees currencies -- -- -- -- -- -- -- //

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
                    fprintf(filepointer, "Randomyear;");
                } else {
                    fprintf(filepointer, "Year;");
                }

                fprintf(filepointer, "meanbas;");
                fprintf(filepointer, "meanbr;");

                // tree stand
                fprintf(filepointer, "Ausschnittsgroesze;");
                fprintf(filepointer, "NderH0bis40;");
                fprintf(filepointer, "NderH40bis200;");
                fprintf(filepointer, "NderH200bis10000;");
                fprintf(filepointer, "Basalarea;");
                fprintf(filepointer, "Stemcount;");
                fprintf(filepointer, "MeanTreeHeight;");
                fprintf(filepointer, "MeanTreeAge;");
                fprintf(filepointer, "Wendejahr;");
                fprintf(filepointer, "Saettigungsjahr;");
                fprintf(filepointer, "ageg0;");
                fprintf(filepointer, "ages0;");
                fprintf(filepointer, "ageg1;");
                fprintf(filepointer, "ages1;");
                fprintf(filepointer, "ageg2;");
                fprintf(filepointer, "ages2;");
                fprintf(filepointer, "ageg3;");
                fprintf(filepointer, "ages3;");
                fprintf(filepointer, "ageg4;");
                fprintf(filepointer, "ages4;");
                fprintf(filepointer, "ageg5;");
                fprintf(filepointer, "ages5;");
                fprintf(filepointer, "ageg6bis10;");
                fprintf(filepointer, "ages6bis10;");
                fprintf(filepointer, "ageg11bis20;");
                fprintf(filepointer, "ages11bis20;");
                fprintf(filepointer, "ageg21bis50;");
                fprintf(filepointer, "ages21bis50;");
                fprintf(filepointer, "ageg51bis100;");
                fprintf(filepointer, "ages51bis100;");
                fprintf(filepointer, "ageg101bis150;");
                fprintf(filepointer, "ages101bis150;");
                fprintf(filepointer, "ageg151bis200;");
                fprintf(filepointer, "ages151bis200;");
                fprintf(filepointer, "ageg201bis300;");
                fprintf(filepointer, "ages201bis300;");
                fprintf(filepointer, "ageg301bis400;");
                fprintf(filepointer, "ages301bis400;");
                fprintf(filepointer, "ageg401bis500;");
                fprintf(filepointer, "ages401bis500;");
                fprintf(filepointer, "ageg501bis600;");
                fprintf(filepointer, "ages501bis600;");
                fprintf(filepointer, "ageg601bis700;");
                fprintf(filepointer, "ages601bis700;");
                fprintf(filepointer, "ageg701plus;");
                fprintf(filepointer, "ages701plus;");
                fprintf(filepointer, "GesamtseedprodAKT;");
                fprintf(filepointer, "GesamtseedprodSUM;");
                fprintf(filepointer, "Trees_gmel;");
                fprintf(filepointer, "Trees_sib;");
                fprintf(filepointer, "GesamtseedBoden;");
                fprintf(filepointer, "Gesamtseedtrees;");
                fprintf(filepointer, "Seeds_gmel;");
                fprintf(filepointer, "Seeds_sib;");
                fprintf(filepointer, "Y_Pos_Max;");
                // weather
                fprintf(filepointer, "weathertyp;");
                fprintf(filepointer, "Starttrees;");
                fprintf(filepointer, "Jahrestemp;");
                fprintf(filepointer, "Jantemp;");
                fprintf(filepointer, "Julitemp;");
                fprintf(filepointer, "Veglaenge;");
                fprintf(filepointer, "AAT;");
                fprintf(filepointer, "DDT;");
                fprintf(filepointer, "Prec;");
                fprintf(filepointer, "weatherfaktor_gmel;");
                fprintf(filepointer, "weatherfaktor_sib;");
                fprintf(filepointer, "thawing_depth_infl;");
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

            for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
                auto pTree = (*pos);

                if (((double)pTree->xcoo/1000 >= xminwindow) && ((double)pTree->xcoo/1000 <= xmaxwindow) && ((double)pTree->ycoo/1000 >= yminwindow) && ((double)pTree->ycoo/1000 <= ymaxwindow)) {
                    if (pTree->species == 1) {
                        // bin trees by age
                        if (pTree->age == 0) {
                            ageg0++;
                        } else if (pTree->age == 1) {
                            ageg1++;
                        } else if (pTree->age == 2) {
                            ageg2++;
                        } else if (pTree->age == 3) {
                            ageg3++;
                        } else if (pTree->age == 4) {
                            ageg4++;
                        } else if (pTree->age == 5) {
                            ageg5++;
                        } else if ((pTree->age > 5) && (pTree->age <= 10)) {
                            ageg6b10++;
                        } else if ((pTree->age > 10) && (pTree->age <= 20)) {
                            ageg11b20++;
                        } else if ((pTree->age > 20) && (pTree->age <= 50)) {
                            ageg21b50++;
                        } else if ((pTree->age > 50) && (pTree->age <= 100)) {
                            ageg51b100++;
                        } else if ((pTree->age > 100) && (pTree->age <= 150)) {
                            ageg101b150++;
                        } else if ((pTree->age > 150) && (pTree->age <= 200)) {
                            ageg151b200++;
                        } else if ((pTree->age > 200) && (pTree->age <= 300)) {
                            ageg201b300++;
                        } else if ((pTree->age > 300) && (pTree->age <= 400)) {
                            ageg301b400++;
                        } else if ((pTree->age > 400) && (pTree->age <= 500)) {
                            ageg401b500++;
                        } else if ((pTree->age > 500) && (pTree->age <= 600)) {
                            ageg501b600++;
                        } else if ((pTree->age > 600) && (pTree->age <= 700)) {
                            ageg601b700++;
                        } else if (pTree->age > 700) {
                            ageg701plus++;
                        }
                    } else if (pTree->species == 2) {
                        if (pTree->age == 0) {
                            ages0++;
                        } else if (pTree->age == 1) {
                            ages1++;
                        } else if (pTree->age == 2) {
                            ages2++;
                        } else if (pTree->age == 3) {
                            ages3++;
                        } else if (pTree->age == 4) {
                            ages4++;
                        } else if (pTree->age == 5) {
                            ages5++;
                        } else if ((pTree->age > 5) && (pTree->age <= 10)) {
                            ages6b10++;
                        } else if ((pTree->age > 10) && (pTree->age <= 20)) {
                            ages11b20++;
                        } else if ((pTree->age > 20) && (pTree->age <= 50)) {
                            ages21b50++;
                        } else if ((pTree->age > 50) && (pTree->age <= 100)) {
                            ages51b100++;
                        } else if ((pTree->age > 100) && (pTree->age <= 150)) {
                            ages101b150++;
                        } else if ((pTree->age > 150) && (pTree->age <= 200)) {
                            ages151b200++;
                        } else if ((pTree->age > 200) && (pTree->age <= 300)) {
                            ages201b300++;
                        } else if ((pTree->age > 300) && (pTree->age <= 400)) {
                            ages301b400++;
                        } else if ((pTree->age > 400) && (pTree->age <= 500)) {
                            ages401b500++;
                        } else if ((pTree->age > 500) && (pTree->age <= 600)) {
                            ages501b600++;
                        } else if ((pTree->age > 600) && (pTree->age <= 700)) {
                            ages601b700++;
                        } else if (pTree->age > 700) {
                            ages701plus++;
                        }
                    }

                    if (pTree->dbasal > 0) {
                        meanbas = meanbas + pTree->dbasal;
                        h++;
                    }

                    // counting seeds
                    gesamtseedAKT += pTree->seednewly_produced;
                    // gesamtseedSUM += pTree->seedproduced;

                    // counting species
                    if (pTree->species == 1) {
                        spectree1++;
                    } else if (pTree->species == 2) {
                        spectree2++;
                    }

                    if ((double)pTree->ycoo/1000 > yposmax) {
                        yposmax = (double)pTree->ycoo/1000;
                    }
                }

                ++pos;
            }

            // for counting seeds:
            int seedconezahl = 0, seedbodenzahl = 0;
            // for counting species:
            int specseed1 = 0, specseed2 = 0;
            for (unsigned int i = 0; i < seed_list.size(); ++i) {
                const auto& seed = seed_list[i];
                if (((double)seed.xcoo/1000 >= xminwindow) && ((double)seed.xcoo/1000 <= xmaxwindow) && ((double)seed.ycoo/1000 >= yminwindow) && ((double)seed.ycoo/1000 <= ymaxwindow)) {
                    if (seed.incone) {
                        seedconezahl++;
                    } else {
                        seedbodenzahl++;
                    }
                    // counting species:
                    if (seed.species == 1) {
                        specseed1 = specseed1 + 1;
                    } else if (seed.species == 2) {
                        specseed2 = specseed2 + 1;
                    }
                }
            }

            if (h > 0) {
                fprintf(filepointer, "%4.4f;", meanbas / ((double)h));
            } else {
                fprintf(filepointer, "%4.4f;", 0.0);
            }
            fprintf(filepointer, "%4.4f;", pEvaluation->meanbreastdiameterliste[pEvaluation->meanbreastdiameterliste.size() - 1]);
            fprintf(filepointer, "%4.2f;", (xmaxwindow - xminwindow) * (ymaxwindow - yminwindow));
            fprintf(filepointer, "%d;", pEvaluation->nheight0b40liste[pEvaluation->nheight0b40liste.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation->nheight41b200liste[pEvaluation->nheight41b200liste.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation->nheight201b10000liste[pEvaluation->nheight201b10000liste.size() - 1]);
            fprintf(filepointer, "%6.4f;", pEvaluation->basalarealist[pEvaluation->basalarealist.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation->stemcountliste[pEvaluation->stemcountliste.size() - 1]);
            fprintf(filepointer, "%6.4f;", pEvaluation->meantreeheightliste[pEvaluation->meantreeheightliste.size() - 1]);
            fprintf(filepointer, "%6.4f;", pEvaluation->meantreeageliste[pEvaluation->meantreeageliste.size() - 1]);
            fprintf(filepointer, "%d;", pEvaluation->yearofturningpoint);
            fprintf(filepointer, "%d;", pEvaluation->yearofequilibrium);
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
            fprintf(filepointer, "%d;", parameter[0].starttrees);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->tempyearmean);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->temp1monthmean);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->temp7monthmean);
            fprintf(filepointer, "%d;", weather_list[yearposition]->vegetationperiodlength);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->activeairtemp);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->degreday);
            fprintf(filepointer, "%4.2f;", weather_list[yearposition]->precipitationsum);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->weatherfactorg);
            fprintf(filepointer, "%4.4f;", weather_list[yearposition]->weatherfactors);
            fprintf(filepointer, "%d;", parameter[0].thawing_depth);

            fprintf(filepointer, "\n");

            fclose(filepointer);

            if (parameter[0].yearlyvis == true) {
                cout << endl << "\tBasalarea = " << pEvaluation->basalarealist[pEvaluation->basalarealist.size() - 1] << endl;
                cout << "\tN0-40 = " << nheight0b40 << "\tN40-200 = " << nheight41b200 << "\tN200+ = " << nheight201b10000 << endl;
                cout << "\tNseeds:\tproduced = " << gesamtseedAKT << "\tground = " << seedbodenzahl << "\tcones = " << seedconezahl << endl;
            }

            // -- -- -- -- -- -- -- trees Currencies -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        }

        if (outputposition == true) {  // tree position output
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- - trees Position -- -- -- -- -- -- -- -- //

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
                        fprintf(filepointer, "DBasal;");
                        fprintf(filepointer, "DBrust;");
                        fprintf(filepointer, "height;");
                        fprintf(filepointer, "age;");
                        fprintf(filepointer, "X;");
                        fprintf(filepointer, "Y;");
                        fprintf(filepointer, "densitywert;");
                        // fprintf(filepointer, "Generation;");
                        fprintf(filepointer, "seedprodAKT;");
                        // fprintf(filepointer, "seedprodSUM;");
                        fprintf(filepointer, "\n");

                        if (filepointer == NULL) {
                            fprintf(stderr, "Error: File could not be opened!\n");
                            exit(1);
                        }
                    }

                    fseek(filepointer, 0, SEEK_END);

                    for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
                        auto pTree = (*pos);

                        // fprintf(filepointer, "%d;", pTree->name);
                        // fprintf(filepointer, "%d;", pTree->namem);
                        fprintf(filepointer, "%4.4f;", pTree->dbasal);
                        fprintf(filepointer, "%4.4f;", pTree->dbreast);
                        fprintf(filepointer, "%4.4f;", (double) pTree->height);
                        fprintf(filepointer, "%d;", pTree->age);
                        fprintf(filepointer, "%4.4f;", (double)pTree->xcoo/1000);
                        fprintf(filepointer, "%4.4f;", (double)pTree->ycoo/1000);
                        fprintf(filepointer, "%4.5f;", pTree->densitywert);
                        // fprintf(filepointer, "%d;", pTree->generation);
                        fprintf(filepointer, "%d;", pTree->seednewly_produced);
                        // fprintf(filepointer, "%d;", pTree->seedproduced);
                        fprintf(filepointer, "\n");

                        ++pos;
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
                        fprintf(filepointer, "Jahr;");
                        fprintf(filepointer, "zufallsJahr;");
                        fprintf(filepointer, "weatherchoice;");
                        fprintf(filepointer, "DBasal;");
                        fprintf(filepointer, "DBrust;");
                        fprintf(filepointer, "height;");
                        fprintf(filepointer, "age;");
                        fprintf(filepointer, "X;");
                        fprintf(filepointer, "Y;");
                        fprintf(filepointer, "densitywert;");
                        // fprintf(filepointer, "Generation;");
                        fprintf(filepointer, "coneheight;");
                        fprintf(filepointer, "seedprodAKT;");
                        // fprintf(filepointer, "seedprodSUM;");
                        fprintf(filepointer, "thawing_depth_infl;");
                        fprintf(filepointer, "\n");

                        if (filepointer == NULL) {
                            fprintf(stderr, "Error: output file is missing!\n");
                            exit(1);
                        }
                    }

                    fseek(filepointer, 0, SEEK_END);

                    for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
                        auto pTree = (*pos);

                        fprintf(filepointer, "%d;", parameter[0].repeati);
                        // fprintf(filepointer, "%d;", pTree->yworldcoo);
                        // fprintf(filepointer, "%d;", pTree->xworldcoo);
                        fprintf(filepointer, "%d;", parameter[0].ivort);
                        fprintf(filepointer, "%d;", jahr);
                        fprintf(filepointer, "%d;", parameter[0].weatherchoice);
                        fprintf(filepointer, "%4.4f;", pTree->dbasal);
                        fprintf(filepointer, "%4.4f;", pTree->dbreast);
                        fprintf(filepointer, "%4.4f;", (double) pTree->height);
                        fprintf(filepointer, "%d;", pTree->age);
                        fprintf(filepointer, "%4.4f;", (double)pTree->xcoo/1000);
                        fprintf(filepointer, "%4.4f;", (double)pTree->ycoo/1000);
                        fprintf(filepointer, "%4.5f;", pTree->densitywert);
                        // fprintf(filepointer, "%d;", pTree->generation);
                        fprintf(filepointer, "%4.4f;", (double) pTree->coneheight);
                        fprintf(filepointer, "%d;", pTree->seednewly_produced);
                        // fprintf(filepointer, "%d;", pTree->seedproduced);
                        fprintf(filepointer, "%lf;", pTree->thawing_depthinfluence);
                        fprintf(filepointer, "\n");

                        ++pos;
                    }

                    fclose(filepointer);
                }
            }

            // -- -- -- -- -- -- - trees Position -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        }  // tree position output

        if (outputindividuals == true) {  // individual tree output
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- --   individual    trees -- -- -- -- -- -- //

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
                // fprintf(filepointer, "weatherchoice;");
                // time variables
                // fprintf(filepointer, "Zeitschritt;");
                // fprintf(filepointer, "Jahr;");
                // tree variables
                fprintf(filepointer, "X;");
                fprintf(filepointer, "Y;");
                // fprintf(filepointer, "Name;");
                // fprintf(filepointer, "NameM;");
                // fprintf(filepointer, "NameP;");
                // fprintf(filepointer, "line;");
                // fprintf(filepointer, "Generation;");
                // fprintf(filepointer, "Art;");
                fprintf(filepointer, "height;");
                fprintf(filepointer, "Dbasal;");
                fprintf(filepointer, "Dbreast;");
                fprintf(filepointer, "age;");
                // fprintf(filepointer, "cone;");
                // fprintf(filepointer, "coneheight;");
                // fprintf(filepointer, "seedprodAKT;");
                // fprintf(filepointer, "seedprodSUM;");
                // fprintf(filepointer, "Speicher;");
                // fprintf(filepointer, "densitywert;");
                // fprintf(filepointer, "Entfernung;");
                // fprintf(filepointer, "thawing_depth_infl;");
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
            for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
                auto pTree = (*pos);
                // parameters
                // fprintf(filepointer, "%d;", parameter[0].repeati);
                // fprintf(filepointer, "%d;", pTree->yworldcoo);
                // fprintf(filepointer, "%d;", pTree->xworldcoo);
                // fprintf(filepointer, "%d;", parameter[0].weatherchoice);
                // time variables
                // fprintf(filepointer, "%d;", t);
                // fprintf(filepointer, "%d;", jahr);
                // tree variables
                fprintf(filepointer, "%4.4f;", (double)pTree->xcoo/1000);
                fprintf(filepointer, "%4.4f;", (double)pTree->ycoo/1000);
                // fprintf(filepointer, "%d;", pTree->name);
                // fprintf(filepointer, "%d;", pTree->namem);
                // fprintf(filepointer, "%d;", pTree->namep);
                // fprintf(filepointer, "%d;", pTree->line);
                // fprintf(filepointer, "%d;", pTree->generation);
                // fprintf(filepointer, "%d;", pTree->species);
                fprintf(filepointer, "%4.4f;", (double) pTree->height);
                fprintf(filepointer, "%4.4f;", pTree->dbasal);
                fprintf(filepointer, "%4.4f;", pTree->dbreast);
                fprintf(filepointer, "%d;", pTree->age);
                // fprintf(filepointer, "%d;", pTree->cone);
                // fprintf(filepointer, "%4.4f;", pTree->coneheight);
                // fprintf(filepointer, "%d;", pTree->seednewly_produced);
                // fprintf(filepointer, "%d;", pTree->seedproduced);
                // fprintf(filepointer, "%d;", pTree->buffer);
                // fprintf(filepointer, "%4.5f;", pTree->densitywert);
                // fprintf(filepointer, "%4.5f;", pTree->dispersaldistance);
                // fprintf(filepointer, "%lf;", pTree->thawing_depthinfluence);
                fprintf(filepointer, "%4.4f;", (double) pTree->elevation/10);
                fprintf(filepointer, "%4.4f;", (double) pTree->envirimpact/10000);
                fprintf(filepointer, "\n");

                ++pos;
            }

            fclose(filepointer);

            // -- -- -- -- --       individual  trees    -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        }  // individual tree output

        if (ausgabedensity == true) {  // tree density map output
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- --tree density -- -- -- -- -- -- -- -- //

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
                fprintf(filepointer, "Treedensityvalue;");
                fprintf(filepointer, "Treenumber;");
                // fprintf(filepointer, "Auflagenstaerke;");
                // fprintf(filepointer, "AuflagenstaerkeMittel;");
                fprintf(filepointer, "Maxthawing_depth;");
                fprintf(filepointer, "Elevation;");
                fprintf(filepointer, "Envirgrowthimpact;");
                // fprintf(filepointer, "weatherchoice;");
                // fprintf(filepointer, "thawing_depth;");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: output file is missing!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data evaluation and output
            for (unsigned long long int  kartenpos = 0; kartenpos < ((unsigned long long int) treerows * (unsigned long long int) parameter[0].sizemagnif * (unsigned long long int) treecols * (unsigned long long int) parameter[0].sizemagnif); kartenpos++) {
                auto pEnvirgrid = plot_list[kartenpos];
				double ycooi = floor((double)kartenpos / (treecols * parameter[0].sizemagnif));
				double xcooi = (double)kartenpos - (ycooi * (treecols * parameter[0].sizemagnif));
                // if (parameter[0].demlandscape | 
                if ( (parameter[0].demlandscape & ( (((xcooi/parameter[0].sizemagnif/30)-floor(xcooi/parameter[0].sizemagnif/30))==0) & (((ycooi/parameter[0].sizemagnif/30)-floor(ycooi/parameter[0].sizemagnif/30))==0) )) | 
					( (pEnvirgrid->Treenumber > 0)
						&& ((xcooi >= xminwindow * parameter[0].sizemagnif) && (xcooi <= xmaxwindow * parameter[0].sizemagnif)
                        && (ycooi >= yminwindow * parameter[0].sizemagnif)
                        && (ycooi <= ymaxwindow * parameter[0].sizemagnif)))
						) {  // output only if tree density values >0

						
                    fprintf(filepointer, "%d;", parameter[0].repeati);
                    // fprintf(filepointer, "%d;", pEnvirgrid->yworldcoo);
                    // fprintf(filepointer, "%d;", pEnvirgrid->xworldcoo);
                    fprintf(filepointer, "%4.4f;", xcooi);
                    fprintf(filepointer, "%4.4f;", ycooi);
                    fprintf(filepointer, "%4.5f;", (double) pEnvirgrid->Treedensityvalue/10000);
                    fprintf(filepointer, "%d;", pEnvirgrid->Treenumber);
                    // fprintf(filepointer, "%u;", pEnvirgrid->litterheight);
                    // fprintf(filepointer, "%u;", pEnvirgrid->litterheightmean);
                    fprintf(filepointer, "%u;", pEnvirgrid->maxthawing_depth);
                    fprintf(filepointer, "%4.4f;", (double) pEnvirgrid->elevation/10);
                    fprintf(filepointer, "%4.4f;", (double) pEnvirgrid->envirgrowthimpact/10000);
                    // fprintf(filepointer, "%d;", parameter[0].weatherchoice);
                    // fprintf(filepointer, "%d;", parameter[0].thawing_depth);
                    fprintf(filepointer, "\n");
                }
            }

            fclose(filepointer);

            // -- -- -- -- -- -- -- --tree density -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
            // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
        }
    }
}
