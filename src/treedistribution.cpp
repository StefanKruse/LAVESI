#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

// TODO temporary here
extern vector<VectorList<Tree>> world_tree_list;
extern vector<VectorList<Seed>> world_seed_list;

void Seedin() {
    RandomNumber<double> uniform(0, 1);
    int aktort = 0;
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;
        aktort++;
        // int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        bool seedinput;

        // seedinput on all sites
        if (parameter[0].realseedconnect == false) {
            seedinput = true;
        } else if (parameter[0].realseedconnect == true && aktort == 1) {  // seedinput on southern site only
            seedinput = true;
        } else {  // no seedinput
            seedinput = false;
        }

        if (seedinput == true) {
            int seednobuffer;
            if (parameter[0].yearswithseedintro <= 0) {
                seednobuffer = parameter[0].seedintronumberpermanent;
            } else {
                seednobuffer = parameter[0].seedintronumber;
            }
// #pragma omp parallel for default(shared) private(uniform) schedule(guided)
            for (int n = 0; n < seednobuffer; n++) {
                // calculate post-dispersal position
                double jseed, iseed;
                bool seedeintragen = false;

                // set limits
                double maxx = (double)(treecols - 1);
                if (parameter[0].seedintro_maxx > 0)
                    maxx = (double)parameter[0].seedintro_maxx;

                double maxy = (double)(treerows - 1);
                if (parameter[0].seedintro_maxy > 0)
                    maxy = (double)parameter[0].seedintro_maxy;

                // seedwinddispersalmode==1 => randomly from the south border.
                if (parameter[0].seedwinddispersalmode == 1) {
                    jseed = maxx * uniform.draw();

                    double dispersaldistance;

                    do {
                        double fraction;

                        do {
                            fraction = uniform.draw();

                        } while (fraction <= 0.0);
                        dispersaldistance = (log(fraction) / (-0.2)) / parameter[0].distanceratio;

                    } while ((dispersaldistance >= (double)(treerows - 1)) || (dispersaldistance < 0.0));

                    seedeintragen = true;
                    iseed = dispersaldistance;
                }
                // seedwinddispersalmode==2 => randomly all over the plot
                else if (parameter[0].seedwinddispersalmode == 2) {
                    jseed = maxx * uniform.draw();
                    iseed = maxy * uniform.draw();

                    seedeintragen = true;
                } else {
                    printf("\n\nLaVeSi was stopped\n");
                    printf("=> Treedistribution.cpp\n");
                    printf("... reason: no choice of a seed dispersal mode");
                    exit(1);
                }

                double rn_seed = 0.0;
                int rn_species = 0;
                if (parameter[0].specpres == 0) {
                    rn_seed = uniform.draw();
                    if (rn_seed <= 0.5) {
                        rn_species = 1;
                    } else {
                        rn_species = 2;
                    }
                } else if (parameter[0].specpres == 1) {
                    rn_species = 1;
                } else if (parameter[0].specpres == 2) {
                    rn_species = 2;
                }

                if (seedeintragen) {
                    Seed seed;
                    // seed.yworldcoo = aktortyworldcoo;
                    // seed.xworldcoo = aktortxworldcoo;
                    seed.xcoo = 1000 * jseed;
                    seed.ycoo = 1000 * iseed;
                    // seed.namem = 0;
                    // seed.namep = 0;
                    // seed.line = ++parameter[0].lineakt;
                    // seed.generation = 0;
                    seed.incone = false;
                    // seed.weight = 1;
                    seed.age = 0;
                    seed.longdispersed = false;
                    seed.species = rn_species;
                    seed.releaseheight = 0;
                    seed.thawing_depthinfluence = 100;
                    seed.dead = false;

                    seed_list.add_directly(std::move(seed));
                }
            }
            seed_list.consolidate();
        }
    }
}

void TreesIni(int maximal_word_length) {
    FILE* f;
    f = NULL;

    // set up of initial trees from different files
    if (parameter[0].starttrees != 0) {
        int aktort = 0;
        for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); posw++) {
            VectorList<Tree>& tree_list = *posw;
            aktort++;
            // int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
            // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

            if (parameter[0].starttrees == 12) {
                f = fopen("input/CH17I_Treevert2011.csv", "r");
                printf("load: input/CH17I_Treevert2011.csv");
            } else if (parameter[0].starttrees == 120100) {
                f = fopen("input/CH17I_Treevert2011_100_100.csv", "r");
                printf("load: input/CH17I_Treevert2011_100_100.csv");
            } else if (parameter[0].starttrees == 120500) {
                f = fopen("input/CH17I_Treevert2011_500_500.csv", "r");
                printf("load: input/CH17I_Treevert2011_500_500.csv");
            } else if (parameter[0].starttrees == 121000) {
                f = fopen("input/CH17I_Treevert2011_1000_1000.csv", "r");
                printf("load: input/CH17I_Treevert2011_1000_1000.csv");
            } else if (parameter[0].starttrees == 120100050) {
                f = fopen("input/CH17I_Treevert2011_100_50000.csv", "r");
                printf("load: input/CH17I_Treevert2011_100_50000.csv");
            }

            if (f == NULL) {
                printf("Tree input file not available!\n");
                exit(1);
            }

            char puffer[255];
            int counter = 1;

            double ybuffer, ycoobuf, xbuffer, xcoobuf;
            int conebuf, agebuf;
            double heightbuf, dbasalbuf, dbreastbuf;

            // ignoring the header the contents are appended to the tree array line by line
            while (fgets(puffer, maximal_word_length, f) != NULL) {
                if (counter >= 2) {
                    strtok(puffer, ";");
                    sscanf(strtok(NULL, ";"), "%lg", &ybuffer);
                    ycoobuf = ybuffer;
                    sscanf(strtok(NULL, ";"), "%lg", &xbuffer);
                    xcoobuf = xbuffer;
                    heightbuf = strtod(strtok(NULL, ";"), NULL);
                    dbasalbuf = strtod(strtok(NULL, ";"), NULL);
                    dbreastbuf = strtod(strtok(NULL, ";"), NULL);
                    sscanf(strtok(NULL, ";"), "%d", &conebuf);
                    sscanf(strtok(NULL, ";"), "%d", &agebuf);

                    Tree tree;

                    // tree.yworldcoo = aktortyworldcoo;
                    // tree.xworldcoo = aktortxworldcoo;
                    tree.xcoo = 1000 * xcoobuf;
                    tree.ycoo = 1000 * ycoobuf;
                    // tree.name = ++parameter[0].nameakt;
                    // tree.namem = 0;
                    // tree.namep = 0;
                    // tree.line = ++parameter[0].lineakt;
                    // tree.generation = 0;
                    // tree.yr_of_establishment = 0;
                    tree.dbasal = dbasalbuf;
                    tree.dbasalrel = 1000;
                    tree.dbreast = dbreastbuf;
                    tree.dbreastrel = 1000;
                    tree.height = 10 * heightbuf;
                    tree.age = agebuf;
                    tree.cone = conebuf;
                    if (tree.cone == true) {
                        tree.coneheight = 65535;
                    }
                    tree.seednewly_produced = 0;
                    // tree.seedproduced = 0;
                    // tree.buffer = 1;
                    tree.densitywert = 0;
                    tree.thawing_depthinfluence = 100;
                    tree.growing = true;
                    if (parameter[0].specpres == 0 || parameter[0].specpres == 1) {
                        tree.species = 1;
                    }
                    if (parameter[0].specpres == 2) {
                        tree.species = 2;
                    }

                    tree_list.add_directly(std::move(tree));
                }
                counter++;
            }
            tree_list.consolidate();

            fclose(f);
        }
    }
}

void Hinterlandseedintro(Parameter* parameter, int yearposition, vector<VectorList<Seed>>& world_seed_list, vector<vector<Weather>>& world_weather_list) {
    RandomNumber<double> uniform(0, 1);
    // function parameters
    double logmodel_seeds_Po = 7.8997307;
    double logmodel_seeds_r = -0.6616466;
    double logmodel_seeds_K = 2700.0081918;

    double logmodel_heights_Po = 6.0897390;
    double logmodel_heights_r = -0.5932627;
    double logmodel_heights_K = 353.5688136;

    // set 'trees' directly as seeds with the relevant information
    // ... for each tree the mean number of seeds and height at the position at the hinterland
    // ... jultemp based on position
    int aktort = 0;
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;

        // grant weather access
        vector<vector<Weather>>::iterator world_positon_weather = (world_weather_list.begin() + aktort);
        vector<Weather>& weather_list = *world_positon_weather;
        aktort++;
        // int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
        // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

#ifdef DEBUG
        cout << " ... seed_list.size=" << seed_list.size() << endl;
#endif

        if (parameter[0].hinterland_maxlength > 0) {
#ifdef DEBUG
            // debugging output
            int introduced = 0;
            int exceedstoN = 0;
            int exceedstoE = 0;
            int exceedstoS = 0;
            int exceedstoW = 0;
#endif

            // hinterland_maxlength is cut into 20 m-long pieces, start at 10 with steps of by 20 to determine the centre for each seed introduction nuclei
// #pragma omp parallel for default(shared) private(uniform) schedule(guided)
			for (int yposhint = -10; yposhint > -1 * parameter[0].hinterland_maxlength; yposhint = yposhint - 20) {
                double jultempi =
                    weather_list[yearposition].temp7monthmean[0] + (-0.3508 * yposhint / (111120));  // conversion to degree latitude see...weatherinput.cpp

                double hinterheightsi = logmodel_heights_K / (1 + exp(logmodel_heights_Po + logmodel_heights_r * jultempi));
                int hinterseedsi = (parameter[0].seedflightrate * logmodel_seeds_K
                                    / (1 + exp(logmodel_seeds_Po + logmodel_seeds_r * jultempi))  // determine number of seeds produced at this nucleus
                                    )
                                   * (double)treecols / 20;  // scaling to the witdth of the simulated area
                for (int n = 0; n < hinterseedsi; n++) {
                    // calculate post-dispersal position with the parameters set for the simulation run using the wind-dependent seed dispersal function

                    // x y start random in field
                    double xseed, yseed;
                    bool introduceseed = true;

                    xseed = (treecols - 1) * uniform.draw();        // x coo start
                    yseed = (yposhint - 10) + 20 * uniform.draw();  // y coo start

                    // define species
                    double seedzufall = 0.0;
                    int specieszufall = 0;
                    if (parameter[0].specpres == 0) {
                        seedzufall = uniform.draw();
                        if (seedzufall <= 0.5) {
                            specieszufall = 1;
                        } else {
                            specieszufall = 2;
                        }
                    } else if (parameter[0].specpres == 1) {
                        specieszufall = 1;
                    } else if (parameter[0].specpres == 2) {
                        specieszufall = 2;
                    }

                    // estimation of new positions
                    double ratiorn = uniform.draw();
                    // double dispersaldistance = 0.0;
                    double velocity = 0.0;
                    double wdirection = 0.0;
                    double jquer = 0;
                    double iquer = 0;

                    double randomnumberwind = uniform.draw();
                    Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, hinterheightsi, specieszufall, randomnumberwind);

                    xseed = xseed + jquer;
                    yseed = yseed + iquer;

                    // manipulate position according to boundary conditions if lands in simulated plot add to seedlist ...code adapted from seeddispersal.cpp
                    if (yseed > (double)(treerows - 1)) {
                        if ((parameter[0].boundaryconditions == 1)) {
                            yseed = fmod(yseed, (double)(treerows - 1));
                        } else if ((parameter[0].boundaryconditions == 3)) {
#ifdef DEBUG
                            exceedstoN++;
#endif
                            introduceseed = false;
                        } else {
#ifdef DEBUG
                            exceedstoN++;
#endif
                            introduceseed = false;
                        }
                    } else if (yseed < 0.0) {
                        if ((parameter[0].boundaryconditions == 1)) {
                            yseed = (double)(treerows - 1) + fmod(yseed, (double)(treerows - 1));
                        } else if ((parameter[0].boundaryconditions == 3)) {
#ifdef DEBUG
                            exceedstoS++;
#endif
                            introduceseed = false;
                        } else {
#ifdef DEBUG
                            exceedstoS++;
#endif
                            introduceseed = false;
                        }
                    }

                    if (xseed < 0.0) {
                        if ((parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3)) {
                            xseed = fmod(xseed, (double)(treecols - 1)) + (double)(treecols - 1);
                        } else {
#ifdef DEBUG
                            exceedstoW++;
#endif
                            introduceseed = false;
                        }
                    } else if (xseed > (double)(treecols - 1)) {
                        if (parameter[0].boundaryconditions == 1 || parameter[0].boundaryconditions == 3) {
                            xseed = fmod(xseed, (double)(treecols - 1));
                        } else if ((parameter[0].boundaryconditions == 2) && (uniform.draw() < 0.5)) {  // Reducing seed introduction on the western border:
                            xseed = fmod(xseed, (double)(treecols - 1));
                        } else {
#ifdef DEBUG
                            exceedstoE++;
#endif
                            introduceseed = false;
                        }
                    }

                    // add new seed to seedlist
                    if (introduceseed == true) {
#ifdef DEBUG
                        introduced++;
#endif

                        Seed seed;
                        // seed.yworldcoo = aktortyworldcoo;
                        // seed.xworldcoo = aktortxworldcoo;
                        seed.xcoo = 1000 * xseed;
                        seed.ycoo = 1000 * yseed;
                        // seed.namem = 0;
                        // seed.namep = 0;
                        // seed.line = ++parameter[0].lineakt;
                        // seed.generation = 0;
                        seed.incone = false;
                        // seed.weight = 1;
                        seed.age = 0;
                        seed.longdispersed = false;
                        seed.species = specieszufall;
                        seed.releaseheight = 10 * hinterheightsi;
                        seed.thawing_depthinfluence = 100;
                        // seed.dispersaldistance = dispersaldistance;
                        seed.dead = false;

                        seed_list.add_directly(std::move(seed));
                    }
                }
            }
            seed_list.consolidate();

#ifdef DEBUG
            cout << "C: introduced seeds=" << introduced << " ----> N=" << exceedstoN << "  E=" << exceedstoE << "  S=" << exceedstoS << "  W=" << exceedstoW
                 << endl;
#endif
        }
    }
}

void Treedistribution(Parameter* parameter, int stringlengthmax) {
    if ((parameter[0].starter == true)) {  // either seed introduction...
        Seedin();
    } else {  // ... or use initial tree input data files
        TreesIni(stringlengthmax);
    }
}
