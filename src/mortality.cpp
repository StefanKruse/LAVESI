#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

/****************************************************************************************/
/**
 * \brief calculate tree mortality
 *
 * depends on abiotic factors (temperature and number of days with temperatures above 20 degrees)
 * and on biotic factors
 *
 *
 *******************************************************************************************/
void TreeMort(int yearposition_help, vector<Weather*>& weather_list, list<Tree*>& tree_list) {
    // calculation of the factors in a function that adds a mortality rate impact
    double anstiegweathermortg = 160;
    double anstiegweathermorts = 160;

    anstiegweathermortg = (60 * weather_list[yearposition_help]->janisothermrestriktiong + 60 * weather_list[yearposition_help]->julisothermrestriktion
                           + 60 * weather_list[yearposition_help]->nddrestriktion);
    anstiegweathermorts = (60 * weather_list[yearposition_help]->janisothermrestriktions + 60 * weather_list[yearposition_help]->julisothermrestriktion
                           + 60 * weather_list[yearposition_help]->nddrestriktion);

    // biotic influence:
    for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
        auto pTree = (*pos);

        if (pTree->growing == true) {
            // if maximal age is exceeded an additional factor occurs
            double agesmort = 0.0;
            if (pTree->age > parameter[0].maximumage) {
                agesmort = 1.0;
            }

            // height dependent influences
            double wachstumrel = 1.0;
            if (pTree->height < 130.0) {
                wachstumrel = pTree->dbasalrel;
            } else {
                wachstumrel = pTree->dbreastrel;
            }

            // extra competition effect
            double heightnkugeleinfluss = 1;
            if (pTree->height < (parameter[0].densityvaluemaximumatheight * 2)) {
                heightnkugeleinfluss =
                    heightnkugeleinfluss
                    + (sqrt(pow(parameter[0].densityvaluemaximumatheight, 2) - pow(pTree->height - parameter[0].densityvaluemaximumatheight, 2))
                       / parameter[0].densityvaluemaximumatheight);
            }

            // calculation of young tree's mortality
            double maxhg;
            double maxhs;
            double maxhpufg = weather_list[yearposition_help]->weatherfactorg
                              * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                    + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal);
            double maxhpufs =
                weather_list[yearposition_help]->weatherfactors
                * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal);

            if (maxhpufg <= 0.0) {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhg = parameter[0].dbasalheightalloslope
                            * pow(exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                      + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhg = parameter[0].dbasalheightslopenonlin
                            * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                  + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal);
                }
            } else {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhg = parameter[0].dbasalheightalloslope
                            * pow(weather_list[yearposition_help]->weatherfactorg
                                      * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                            + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhg = parameter[0].dbasalheightslopenonlin * weather_list[yearposition_help]->weatherfactorg
                            * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                  + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal);
                }
            }

            if (maxhpufs <= 0.0) {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhs = parameter[0].dbasalheightalloslope
                            * pow(exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                      + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhs = parameter[0].dbasalheightslopenonlin
                            * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                  + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal);
                }
            } else {
                if (parameter[0].allometryfunctiontype == 1) {
                    maxhs = parameter[0].dbasalheightalloslope
                            * pow(weather_list[yearposition_help]->weatherfactors
                                      * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                            + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal),
                                  parameter[0].dbasalheightalloexp);
                } else {
                    maxhs = parameter[0].dbasalheightslopenonlin * weather_list[yearposition_help]->weatherfactors
                            * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                  + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal);
                }
            }

            double sapl_mort_gmel = parameter[0].mortyouth * pow(exp((-1.0 * pTree->height) + maxhg), parameter[0].mortyouthinfluenceexp);
            double sapl_mort_sib = parameter[0].mortyouth * pow(exp((-1.0 * pTree->height) + maxhs), parameter[0].mortyouthinfluenceexp);
            double age_mort = parameter[0].mortage * agesmort * (10.0 * parameter[0].mortbg);
            double growth_mort = parameter[0].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
            double dens_mort = parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert;
            double weathermortaddg = 1.0
                                     - (1.0
                                        / (1.0
                                           + (((1.0 - 0.5) / 0.5)
                                              * exp(anstiegweathermortg * weather_list[yearposition_help]->weatherfactorg
                                                    * exp(parameter[0].gdbasalconstgmel + parameter[0].gdbasalfacgmel * pTree->dbasal
                                                          + parameter[0].gdbasalfacqgmel * pTree->dbasal * pTree->dbasal)))));
            double weathermortadds = 1.0
                                     - (1.0
                                        / (1.0
                                           + (((1.0 - 0.5) / 0.5)
                                              * exp(anstiegweathermorts * weather_list[yearposition_help]->weatherfactors
                                                    * exp(parameter[0].gdbasalconstsib + parameter[0].gdbasalfacsib * pTree->dbasal
                                                          + parameter[0].gdbasalfacqsib * pTree->dbasal * pTree->dbasal)))));
            double weather_mort_gmel = parameter[0].mweather * weathermortaddg * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);
            double weather_mort_sib = parameter[0].mweather * weathermortadds * pow((1.0 / pTree->height), parameter[0].heightweathermorteinflussexp);
            double dry_mort = parameter[0].mdrought * weather_list[yearposition_help]->droughtmort * pow((1.0 / pTree->height), 0.5);

            // calculating the mortality rate of the tree considering the factors of each mortality rate
            double Treemortg = 0.0 + parameter[0].mortbg + sapl_mort_gmel + age_mort + growth_mort + dens_mort + weather_mort_gmel + dry_mort;

            double Treemorts = 0.0 + parameter[0].mortbg + sapl_mort_sib + age_mort + growth_mort + dens_mort + weather_mort_sib + dry_mort;

            if (Treemortg > 1.0) {
                Treemortg = 1.0;
            } else if (Treemortg < 0.0) {
                Treemortg = 0.0;
            }

            if (Treemorts > 1.0) {
                Treemorts = 1.0;
            } else if (Treemorts < 0.0) {
                Treemorts = 0.0;
            }

            if (parameter[0].mortvis == true && pTree->height > 200.0 && pTree->species == 1) {
                printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +drought=%4.4f =>%4.2f",
                       pTree->height, pTree->dbasalrel, pTree->dbreastrel,
                       parameter[0].mortyouth * pow(exp((-1.0 * pTree->height) + maxhg), parameter[0].mortyouthinfluenceexp),
                       parameter[0].mortage * (agesmort * (10.0 * parameter[0].mortbg)), +(1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp)),
                       parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert,
                       parameter[0].mweather * weathermortaddg * pow((1.0 / pTree->height), 0.5),
                       parameter[0].mdrought * (weather_list[yearposition_help]->droughtmort * pow((1.0 / pTree->height), 0.5)), Treemortg);
            }

            if (parameter[0].mortvis == true && pTree->height > 200.0 && pTree->species == 2) {
                printf("\n H=%4.1f DBrel/DBRrel=%4.2f/%4.2f => JUNG=%4.3f +ALT=%4.2f +AKTGRO=%4.2f +density=%4.4f +weather=%4.3f +drought=%4.4f =>%4.2f",
                       pTree->height, pTree->dbasalrel, pTree->dbreastrel,
                       parameter[0].mortyouth * pow(exp((-1.0 * pTree->height) + maxhs), parameter[0].mortyouthinfluenceexp),
                       parameter[0].mortage * (agesmort * (10.0 * parameter[0].mortbg)), +(1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp)),
                       parameter[0].mdensity * heightnkugeleinfluss * pTree->densitywert,
                       parameter[0].mweather * weathermortadds * pow((1.0 / pTree->height), 0.5),
                       parameter[0].mdrought * (weather_list[yearposition_help]->droughtmort * pow((1.0 / pTree->height), 0.5)), Treemorts);
            }

            // Determine if a tree dies (deletion of said tree in the corresponding list)
            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));
            if (((pTree->species == 1) && (zufallsz < Treemortg)) || ((pTree->species == 2) && (zufallsz < Treemorts))) {
                delete pTree;
                pos = tree_list.erase(pos);
            } else {
                ++pos;
            }
        } else {
            ++pos;
        }
    }
}

/****************************************************************************************/
/**
 * \brief calculate tree and seed mortality
 *
 * first create surviving seeds then call TreeMort()
 *
 *
 *******************************************************************************************/
void Mortality(struct Parameter* parameter,
               int Jahr,
               int yearposition,
               vector<list<Tree*>>& world_tree_list,
               vector<VectorList<Seed>>& world_seed_list,
               vector<vector<Weather*>>& world_weather_list) {
    int aktort = 0;

    for (vector<vector<Weather*>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather*>& weather_list = *posw;

        vector<list<Tree*>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        list<Tree*>& tree_list = *world_positon_b;

        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;

        aktort++;

        // mortality of seeds
        for (unsigned int i = 0; i < seed_list.size(); ++i) {
            auto& seed = seed_list[i];
            if (seed.dead) {
                continue;
            }
            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));

            if (zufallsz < seed.incone ? parameter[0].seedconemort : parameter[0].seedfloormort) {
                seed.dead = true;
                seed_list.remove(i);
            }
        }

        int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
        int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

        // chose the implementation of multi-core-processing
        int mcorevariant = 3;
        // 1 == use advance to iterate through lists, but this makes the computation really slow
        // 2 == split list to X lists of the same length
        // 3 == trees are ordered by age which is highly correlated with seedprodAKT so that only elements are considered untiil the last tree producing seeds
        if (mcorevariant == 1) {  // OMP==1
            // loop around the loop for MULTI-CORE-PROCESSING
            // before run a program parallel set the number of helpers by changing the environment variable
            // ... ... export OMP_NUM_THREADS=4
            // ... or explicitly overwrite the environmental variable by setting the helper number directly
            // ... ... omp_set_num_threads(int); // which is set in the parameter.txt file variable "omp_num_threads"

            // loop with omp through each element of the list
            omp_set_dynamic(0);                                 // disable dynamic teams
            omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers

            double direction = 0.0;
            double velocity = 0.0;
            unsigned int ripm = 0, cntr = 0;
            double p = 0.0, kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2), phi = 0.0, dr = 0.0, dx = 0.0, dy = 0.0;
            double I0kappa = 0.0;
            double pe = 0.01;
            double C = parameter[0].pollengregoryc;
            double m = parameter[0].pollengregorym;
            vector<int> Vname;
            vector<double> Vthdpth;

#pragma omp parallel default(shared) private(direction, velocity, ripm, cntr, p, kappa, phi, dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth)
            {
                // declare a local seed list to be filled by each thread
                VectorList<Seed> newseed_list;

                direction = 0.0;
                velocity = 0.0;
                ripm = 0, cntr = 0;
                p = 0.0;
                kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2);
                I0kappa = 0.0;
                pe = 0.01;
                C = parameter[0].pollengregoryc;
                m = parameter[0].pollengregorym;
                phi = 0.0;
                dr = 0.0;
                dx = 0.0;
                dy = 0.0;

                int n_trees = 0;
#pragma omp for nowait schedule(guided)
                for (unsigned int pari = 0; pari < tree_list.size(); ++pari) {
                    ++n_trees;  // for later calculating mean of computation times // TODO necesary?

                    list<Tree*>::iterator posb = tree_list.begin();
                    // since the iterator must be an int for omp, the iterator has to be constructed for each tree instance and advanced to the correct position
                    advance(posb, pari);

                    auto pTree = (*posb);

                    if ((parameter[0].ivort == 1) & (pari == 0)) {
                        cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads()
                             << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl
                             << endl;
                    }

                    if (pTree->seednewly_produced > 0) {
                        int seedlebend = 0;
                        for (int sna = 0; sna < pTree->seednewly_produced; sna++) {
                            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));
                            if (zufallsz >= parameter[0].seedconemort) {
                                ++seedlebend;
                            }
                        }

                        if (seedlebend > 0) {
                            if ((parameter[0].pollination == 1 && Jahr > 1978 && Jahr < 2013 && parameter[0].spinupphase == false && parameter[0].ivort > 1045)
                                || (parameter[0].pollination == 9)) {
                                Pollinationprobability(pTree->xcoo, pTree->ycoo, &parameter[0], world_positon_b, direction, velocity, ripm, cntr, p, kappa, phi,
                                                       dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth, n_trees);
                            }

                            // get the characteristics for each surviving seed and push these back new to seed_list
                            for (int sl = 0; sl < seedlebend; sl++) {
                                Seed seed;

                                seed.yworldcoo = aktortyworldcoo;
                                seed.xworldcoo = aktortxworldcoo;
                                seed.xcoo = pTree->xcoo;
                                seed.ycoo = pTree->ycoo;
                                seed.namem = pTree->name;

                                // if chosen, determine the father by pollination out of available (matured) trees
                                if ((Vname.size() > 0) && (parameter[0].pollination == 1 || parameter[0].pollination == 9)) {
                                    int iran = (int)rand() / (RAND_MAX + 1.0) * Vname.size() - 1;
                                    seed.namep = Vname.at(iran);
                                    seed.thawing_depthinfluence = Vthdpth.at(iran);
                                } else {
                                    seed.namep = 0;
                                    seed.thawing_depthinfluence = 100;
                                }

                                seed.line = pTree->line;
                                seed.generation = pTree->generation + 1;
                                seed.incone = true;
                                seed.weight = 1;
                                seed.age = 0;
                                seed.species = pTree->species;
                                seed.releaseheight = pTree->height;
                                seed.dead = false;

                                seed_list.add(seed);
                            }
                        }
                    }
                }
            }  // parallel region

            Vname.clear();
            Vname.shrink_to_fit();
            Vthdpth.clear();
            Vthdpth.shrink_to_fit();
        }                                                       // OMP==1
        if (mcorevariant == 2) {                                // OMP==2
            omp_set_dynamic(0);                                 // disable dynamic teams
            omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers

            double direction = 0.0;
            double velocity = 0.0;
            unsigned int ripm = 0, cntr = 0;
            double p = 0.0, kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2), phi = 0.0, dr = 0.0, dx = 0.0, dy = 0.0;
            double I0kappa = 0.0;
            double pe = 0.01;
            double C = parameter[0].pollengregoryc;
            double m = parameter[0].pollengregorym;
            vector<int> Vname;
            vector<double> Vthdpth;

#pragma omp parallel default(shared) private(direction, velocity, ripm, cntr, p, kappa, phi, dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth)
            {
                if ((parameter[0].ivort == 1)) {
                    cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads()
                         << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl
                         << endl;
                }

                direction = 0.0;
                velocity = 0.0;
                ripm = 0, cntr = 0;
                p = 0.0;
                kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2);
                I0kappa = 0.0;
                pe = 0.01;
                C = parameter[0].pollengregoryc;
                m = parameter[0].pollengregorym;
                phi = 0.0;
                dr = 0.0;
                dx = 0.0;
                dy = 0.0;

                // slit the lists manually
                int thread_count = omp_get_num_threads();
                int thread_num = omp_get_thread_num();
                size_t chunk_size = tree_list.size() / thread_count;
                auto begin = tree_list.begin();
                std::advance(begin, thread_num * chunk_size);
                auto end = begin;
                if (thread_num == (thread_count - 1))  // the last thread iterates through the remaining elements
                {
                    end = tree_list.end();
                } else {
                    std::advance(end, chunk_size);
                }

                int n_trees = 0;
// wait for all threads to initialize and then proceed
#pragma omp barrier
                for (auto it = begin; it != end; ++it) {
                    ++n_trees; // TODO necessary?

                    auto pTree = (*it);

                    if (pTree->seednewly_produced > 0) {
                        int seedlebend = 0;
                        for (int sna = 0; sna < pTree->seednewly_produced; sna++) {
                            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));
                            if (zufallsz >= parameter[0].seedconemort) {
                                ++seedlebend;
                            }
                        }

                        if (seedlebend > 0) {
                            if ((parameter[0].pollination == 1 && Jahr > 1978 && Jahr < 2013 && parameter[0].spinupphase == false && parameter[0].ivort > 1045)
                                || (parameter[0].pollination == 9)) {
                                Pollinationprobability(pTree->xcoo, pTree->ycoo, &parameter[0], world_positon_b, direction, velocity, ripm, cntr, p, kappa, phi,
                                                       dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth, n_trees);
                            }

                            // get the characteristics for each surviving seed and push these back new to seed_list
                            for (int sl = 0; sl < seedlebend; sl++) {
                                Seed seed;

                                seed.yworldcoo = aktortyworldcoo;
                                seed.xworldcoo = aktortxworldcoo;
                                seed.xcoo = pTree->xcoo;
                                seed.ycoo = pTree->ycoo;
                                seed.namem = pTree->name;
                                seed.dead = false;

                                // if chosen, determine the father by pollination out of available (matured) trees
                                if ((Vname.size() > 0) && (parameter[0].pollination == 1 || parameter[0].pollination == 9)) {
                                    int iran = (int)rand() / (RAND_MAX + 1.0) * Vname.size() - 1;
                                    seed.namep = Vname.at(iran);
                                    seed.thawing_depthinfluence = 100;
                                } else {
                                    seed.namep = 0;
                                    seed.thawing_depthinfluence = 100;
                                }

                                seed.line = pTree->line;
                                seed.generation = pTree->generation + 1;
                                seed.incone = true;
                                seed.weight = 1;
                                seed.age = 0;
                                seed.species = pTree->species;
                                seed.releaseheight = pTree->height;

                                seed_list.add(seed);
                            }
                        }
                    }
                }  // main tree loop on each core
            }      // parallel region

            Vname.clear();
            Vname.shrink_to_fit();
            Vthdpth.clear();
            Vthdpth.shrink_to_fit();
        }                                                       // OMP==2
        if (mcorevariant == 3) {                                // OMP==3
            omp_set_dynamic(0);                                 // disable dynamic teams
            omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers

            double direction = 0.0;
            double velocity = 0.0;
            unsigned int ripm = 0, cntr = 0;
            double p = 0.0, kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2), phi = 0.0, dr = 0.0, dx = 0.0, dy = 0.0;
            double I0kappa = 0.0;
            double pe = 0.01;
            double C = parameter[0].pollengregoryc;
            double m = parameter[0].pollengregorym;
            vector<int> Vname;
            vector<double> Vthdpth;

            // set end of the iterations split up to last tree with produced seeds
            list<Tree*>::iterator lasttreewithseeds_iter = tree_list.begin();
            int lasttreewithseeds_pos = 0;
            int treeiter = 0;
            for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ++posb) {
                auto pTree = (*posb);

                treeiter = treeiter + 1;

                if (pTree->seednewly_produced > 0) {
                    lasttreewithseeds_pos = treeiter;
                }
            }
            advance(lasttreewithseeds_iter, lasttreewithseeds_pos);

#pragma omp parallel default(shared) private(direction, velocity, ripm, cntr, p, kappa, phi, dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth)
            {
                direction = 0.0;
                velocity = 0.0;
                ripm = 0, cntr = 0;
                p = 0.0;
                kappa = pow(180 / (parameter[0].pollendirectionvariance * M_PI), 2);
                phi = 0.0;
                dr = 0.0;
                dx = 0.0;
                dy = 0.0;
                I0kappa = 0.0;
                pe = 0.01;
                C = parameter[0].pollengregoryc;
                m = parameter[0].pollengregorym;

                int thread_count = omp_get_num_threads();
                int thread_num = omp_get_thread_num();
                size_t chunk_size = lasttreewithseeds_pos / thread_count;
                auto begin = tree_list.begin();
                std::advance(begin, thread_num * chunk_size);
                auto end = begin;

                if (thread_num == (thread_count - 1)) {
                    if ((parameter[0].ivort == 1)) {
                        cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads()
                             << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl
                             << endl;
                    }

                    end = lasttreewithseeds_iter;
                } else {
                    std::advance(end, chunk_size);
                }

                // declare a local seed list to be filled by each thread
                VectorList<Seed> newseed_list;

                int n_trees = 0;

// wait for all threads to initialize and then proceed
#pragma omp barrier
                for (auto it = begin; it != end; ++it) {
                    ++n_trees;  // for calculating mean of computation times // TODO still necessary

                    auto pTree = (*it);

                    if (pTree->seednewly_produced > 0) {
                        int seedlebend = 0;
                        for (int sna = 0; sna < pTree->seednewly_produced; sna++) {
                            double zufallsz = 0.0 + ((double)1.0 * rand() / (RAND_MAX + 1.0));
                            if (zufallsz >= parameter[0].seedconemort) {
                                ++seedlebend;
                            }
                        }

                        if (seedlebend > 0) {
                            if ((parameter[0].pollination == 1 && parameter[0].ivort > 1045) || (parameter[0].pollination == 9)) {
                                Pollinationprobability(pTree->xcoo, pTree->ycoo, &parameter[0], world_positon_b, direction, velocity, ripm, cntr, p, kappa, phi,
                                                       dr, dx, dy, I0kappa, pe, C, m, Vname, Vthdpth, n_trees);
                            }

                            for (int sl = 0; sl < seedlebend; sl++) {
                                Seed seed;

                                seed.yworldcoo = aktortyworldcoo;
                                seed.xworldcoo = aktortxworldcoo;
                                seed.xcoo = pTree->xcoo;
                                seed.ycoo = pTree->ycoo;
                                seed.namem = pTree->name;
                                seed.dead = false;

                                // if chosen, determine the father by pollination out of available (matured) trees
                                if ((Vname.size() > 0) && (parameter[0].pollination == 1 || parameter[0].pollination == 9)) {
                                    int iran = (int)rand() / (RAND_MAX + 1.0) * Vname.size() - 1;
                                    seed.namep = Vname.at(iran);
                                    seed.thawing_depthinfluence = 100;
                                } else {
                                    seed.namep = 0;
                                    seed.thawing_depthinfluence = 100;
                                }

                                seed.line = pTree->line;
                                seed.generation = pTree->generation + 1;
                                seed.incone = true;
                                seed.weight = 1;
                                seed.age = 0;
                                seed.species = pTree->species;
                                seed.releaseheight = pTree->height;

                                seed_list.add(seed);
                            }
                        }
                    }
                }  // main tree loop on each core
            }      // parallel region

            Vname.clear();
            Vname.shrink_to_fit();
            Vthdpth.clear();
            Vthdpth.shrink_to_fit();
        }  // OMP==3

        TreeMort(yearposition, weather_list, tree_list);
    }
}
