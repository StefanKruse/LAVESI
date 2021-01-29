#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

// TODO temporary here
extern vector<VectorList<Tree>> world_tree_list;
extern vector<VectorList<Seed>> world_seed_list;
extern vector<vector<Weather>> world_weather_list;
extern vector<vector<Envirgrid>> world_plot_list;
extern vector<vector<Evaluation>> world_evaluation_list;
extern vector<VectorList<Tree>> world_tree_list_copy;
extern vector<VectorList<Seed>> world_seed_list_copy;
extern vector<vector<Envirgrid>> world_plot_list_copy;
extern vector<vector<Evaluation>> world_evaluation_list_copy;

void Savealllists() {
    // copy Trees
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;
        // opening the original tree list and the referring reset list ..._copy
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list_copy.begin() + aktort);
        VectorList<Tree>& tree_list_copy = *world_positon_b;
        aktort++;

        cout << endl;
        cout << "tree_list.size()=" << tree_list.size() << endl;
        cout << "tree_list_copy.size() before copying =" << tree_list_copy.size() << endl;

        int countertree = 1;
        for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
            auto& tree = tree_list[tree_i];
			
			if (tree.growing == true) {
				Tree newtree;

				// copy
				// newtree.yworldcoo = tree.yworldcoo;
				// newtree.xworldcoo = tree.xworldcoo;
				newtree.xcoo = tree.xcoo;
				newtree.ycoo = tree.ycoo;
				// newtree.name = tree.name;
				// newtree.namem = tree.namem;
				// newtree.namep = tree.namep;
				// newtree.line = tree.line;
				// newtree.generation = tree.generation;
				newtree.species = tree.species;
				// newtree.yr_of_establishment = tree.yr_of_establishment;
				// newtree.yr_of_dying = tree.yr_of_dying;
				newtree.dbasal = tree.dbasal;
				newtree.dbasalrel = tree.dbasalrel;
				newtree.dbreast = tree.dbreast;
				newtree.dbreastrel = tree.dbreastrel;
				newtree.height = tree.height;
				newtree.age = tree.age;
				newtree.cone = tree.cone;
				newtree.coneheight = tree.coneheight;
				newtree.seednewly_produced = tree.seednewly_produced;
				// newtree.seedproduced = tree.seedproduced;
				// newtree.buffer = tree.buffer;
				newtree.densitywert = tree.densitywert;
				newtree.thawing_depthinfluence = tree.thawing_depthinfluence;
				newtree.longdispersed = tree.longdispersed;
				newtree.growing = tree.growing;

				// add
				tree_list_copy.add(newtree);

				if (countertree == 1) {
					cout << tree.height << " <-tree ... Treeheight ... newtree. " << newtree.height << endl;
				}

				++countertree;
			}
        }
        tree_list_copy.consolidate();

        cout << "tree_list_copy.size() after copying=" << tree_list_copy.size() << endl;
        cout << endl;
        cout << endl;
    }

    // copy Seeds
    aktort = 0;
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;
        // opening the original seed list and the referring reset list ..._copy
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list_copy.begin() + aktort);
        VectorList<Seed>& seed_list_copy = *world_positon_s;
        aktort++;

        cout << endl;
        cout << "seed_list.size()=" << seed_list.size() << endl;
        cout << "seed_list_copy.size() before copying=" << seed_list_copy.size() << endl;

        seed_list_copy = seed_list;

        cout << "seed_list_copy.size() after copying=" << seed_list_copy.size() << endl;
        cout << endl;
        cout << endl;
    }

    // copy Envirgrid
    aktort = 0;
    for (vector<vector<Envirgrid>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw) {
        vector<Envirgrid>& plot_list = *posw;
        // opening the original plot list and the referring reset list ..._copy
        vector<vector<Envirgrid>>::iterator posiweltk = (world_plot_list_copy.begin() + aktort);
        vector<Envirgrid>& plot_list_copy = *posiweltk;
        aktort++;

        cout << endl;
        cout << "plot_list.size()=" << plot_list.size() << endl;
        cout << "plot_list_copy.size() vor Kopieren=" << plot_list_copy.size() << endl;

        int countertotalmaps = 1;  // for displaying the first plot
        for (vector<Envirgrid>::iterator pos = plot_list.begin(); pos != plot_list.end();) {
            auto& pEnvirgrid = (*pos);

            Envirgrid pEnvirgrid_copy;

            // pEnvirgrid_copy.xworldcoo = pEnvirgrid.xworldcoo;
            // pEnvirgrid_copy.yworldcoo = pEnvirgrid.yworldcoo;
            // pEnvirgrid_copy.xcoo = pEnvirgrid.xcoo;
            // pEnvirgrid_copy.ycoo = pEnvirgrid.ycoo;
            pEnvirgrid_copy.Treedensityvalue = pEnvirgrid.Treedensityvalue;
            pEnvirgrid_copy.Treenumber = pEnvirgrid.Treenumber;
            pEnvirgrid_copy.maxthawing_depth = pEnvirgrid.maxthawing_depth;
            // pEnvirgrid_copy.litterheight = pEnvirgrid.litterheight;
            // pEnvirgrid_copy.litterheight0 = pEnvirgrid.litterheight0;
            // pEnvirgrid_copy.litterheight1 = pEnvirgrid.litterheight1;
            // pEnvirgrid_copy.litterheight2 = pEnvirgrid.litterheight2;
            // pEnvirgrid_copy.litterheight3 = pEnvirgrid.litterheight3;
            // pEnvirgrid_copy.litterheight4 = pEnvirgrid.litterheight4;
            // pEnvirgrid_copy.litterheight5 = pEnvirgrid.litterheight5;
            // pEnvirgrid_copy.litterheight6 = pEnvirgrid.litterheight6;
            // pEnvirgrid_copy.litterheight7 = pEnvirgrid.litterheight7;
            // pEnvirgrid_copy.litterheight8 = pEnvirgrid.litterheight8;
            // pEnvirgrid_copy.litterheight9 = pEnvirgrid.litterheight9;
            // pEnvirgrid_copy.litterheightmean = pEnvirgrid.litterheightmean;
            plot_list_copy.emplace_back(std::move(pEnvirgrid_copy));

            if (countertotalmaps == 1) {
                cout << pEnvirgrid.Treedensityvalue << " <-pEnvirgrid ... EnvirgridTreedensity ... pEnvirgrid_copy. " << pEnvirgrid_copy.Treedensityvalue
                     << endl;
            }

            ++pos;
            ++countertotalmaps;
        }

        cout << "plot_list_copy.size() after copying=" << plot_list_copy.size() << endl;
        cout << endl;
        cout << endl;
    }

    // copy Evaluation
    aktort = 0;
    for (vector<vector<Evaluation>>::iterator posw = world_evaluation_list.begin(); posw != world_evaluation_list.end(); ++posw) {
        vector<Evaluation>& EvaluationListe = *posw;
        // opening the original evaluation list and the referring reset list ..._copy
        vector<vector<Evaluation>>::iterator posiwelta = (world_evaluation_list_copy.begin() + aktort);
        vector<Evaluation>& EvaluationListe_copy = *posiwelta;
        aktort++;

        cout << endl;
        cout << "EvaluationListe.size()=" << EvaluationListe.size() << endl;
        cout << "EvaluationListe_copy.size() before copying=" << EvaluationListe_copy.size() << endl;

        int counterauswert = 1;  // for displaying the first evaluation list element
        for (vector<Evaluation>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end();) {
            auto& pEvaluation = (*pos);
            Evaluation pEvaluation_copy;

            // pEvaluation_copy->xworldcoo = pEvaluation->xworldcoo;
            // pEvaluation_copy->yworldcoo = pEvaluation->yworldcoo;
            for (vector<double>::iterator posval = pEvaluation.basalarealist.begin(); posval < pEvaluation.basalarealist.end();) {
                pEvaluation_copy.basalarealist.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.basalarearunmeanlist.begin(); posval < pEvaluation.basalarearunmeanlist.end();) {
                pEvaluation_copy.basalarearunmeanlist.push_back((*posval));
                ++posval;
            }
            pEvaluation_copy.maxincrementbasalarea = pEvaluation.maxincrementbasalarea;
            pEvaluation_copy.countermaxincrementbasalarea = pEvaluation.countermaxincrementbasalarea;
            pEvaluation_copy.yearofturningpoint = pEvaluation.yearofturningpoint;
            pEvaluation_copy.yearofequilibrium = pEvaluation.yearofequilibrium;
            pEvaluation_copy.postyearofturningpoint = pEvaluation.postyearofturningpoint;
            for (vector<int>::iterator posval = pEvaluation.nheight0b40liste.begin(); posval < pEvaluation.nheight0b40liste.end();) {
                pEvaluation_copy.nheight0b40liste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.nheight0b40runmeanliste.begin(); posval < pEvaluation.nheight0b40runmeanliste.end();) {
                pEvaluation_copy.nheight0b40runmeanliste.push_back((*posval));
                ++posval;
            }
            for (vector<int>::iterator posval = pEvaluation.nheight41b200liste.begin(); posval < pEvaluation.nheight41b200liste.end();) {
                pEvaluation_copy.nheight41b200liste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.nheight41b200runmeanliste.begin(); posval < pEvaluation.nheight41b200runmeanliste.end();) {
                pEvaluation_copy.nheight41b200runmeanliste.push_back((*posval));
                ++posval;
            }
            for (vector<int>::iterator posval = pEvaluation.nheight201b10000liste.begin(); posval < pEvaluation.nheight201b10000liste.end();) {
                pEvaluation_copy.nheight201b10000liste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.nheight201b10000runmeanliste.begin(); posval < pEvaluation.nheight201b10000runmeanliste.end();) {
                pEvaluation_copy.nheight201b10000runmeanliste.push_back((*posval));
                ++posval;
            }

            for (vector<double>::iterator posval = pEvaluation.meanbreastdiameterliste.begin(); posval < pEvaluation.meanbreastdiameterliste.end();) {
                pEvaluation_copy.meanbreastdiameterliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meanbreastdiameterrunmeanliste.begin();
                 posval < pEvaluation.meanbreastdiameterrunmeanliste.end();) {
                pEvaluation_copy.meanbreastdiameterrunmeanliste.push_back((*posval));
                ++posval;
            }

            for (vector<int>::iterator posval = pEvaluation.stemcountliste.begin(); posval < pEvaluation.stemcountliste.end();) {
                pEvaluation_copy.stemcountliste.push_back((*posval));
                ++posval;
            }
            for (vector<int>::iterator posval = pEvaluation.stemcountrunmeanliste.begin(); posval < pEvaluation.stemcountrunmeanliste.end();) {
                pEvaluation_copy.stemcountrunmeanliste.push_back((*posval));
                ++posval;
            }

            for (vector<double>::iterator posval = pEvaluation.meantreeheightliste.begin(); posval < pEvaluation.meantreeheightliste.end();) {
                pEvaluation_copy.meantreeheightliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meantreeheightrunmeanliste.begin(); posval < pEvaluation.meantreeheightrunmeanliste.end();) {
                pEvaluation_copy.meantreeheightrunmeanliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meantreeageliste.begin(); posval < pEvaluation.meantreeageliste.end();) {
                pEvaluation_copy.meantreeageliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meantreeagerunmeanliste.begin(); posval < pEvaluation.meantreeagerunmeanliste.end();) {
                pEvaluation_copy.meantreeagerunmeanliste.push_back((*posval));
                ++posval;
            }

            EvaluationListe_copy.emplace_back(std::move(pEvaluation_copy));

            if (counterauswert == 1) {
                cout << pEvaluation.maxincrementbasalarea << " <-pEvaluation ... Evaluationmaxincrementbasalarea ... pEvaluation_copy. "
                     << pEvaluation_copy.maxincrementbasalarea << endl;
            }

            ++pos;
            ++counterauswert;
        }

        cout << "EvaluationListe_copy.size() after copying=" << EvaluationListe_copy.size() << endl;
        cout << endl;
        cout << endl;
    }
}

void Clearalllists(void) {
    int aktort = 0;
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
        vector<vector<Envirgrid>>::iterator posiweltk = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *posiweltk;
        vector<vector<Evaluation>>::iterator posiwelt = (world_evaluation_list.begin() + aktort);
        vector<Evaluation>& evaluation_list = *posiwelt;
        aktort++;

        // remove elements
        tree_list.clear();
        seed_list.clear();
        plot_list.clear();
        plot_list.shrink_to_fit();
        evaluation_list.clear();
        evaluation_list.shrink_to_fit();
    }
}

void Restorealllists(void) {
    // copying Trees
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list_copy = *world_positon_b;
        aktort++;

        cout << endl;
        cout << "tree_list.size() before copying =" << tree_list.size() << endl;
        cout << "tree_list_copy.size() before copying =" << tree_list_copy.size() << endl;

        int countertree = 1;  // for displaying the first tree
        for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
            auto& tree = tree_list[tree_i];

			if (tree.growing == true) {
				Tree newtree;

				// copy
				// newtree.yworldcoo = tree.yworldcoo;
				// newtree.xworldcoo = tree.xworldcoo;
				newtree.xcoo = tree.xcoo;
				newtree.ycoo = tree.ycoo;
				// newtree.name = tree.name;
				// newtree.namem = tree.namem;
				// newtree.namep = tree.namep;
				// newtree.line = tree.line;
				// newtree.generation = tree.generation;
				newtree.species = tree.species;
				// newtree.yr_of_establishment = tree.yr_of_establishment;
				// newtree.yr_of_dying = tree.yr_of_dying;
				newtree.dbasal = tree.dbasal;
				newtree.dbasalrel = tree.dbasalrel;
				newtree.dbreast = tree.dbreast;
				newtree.dbreastrel = tree.dbreastrel;
				newtree.height = tree.height;
				newtree.age = tree.age;
				newtree.cone = tree.cone;
				newtree.coneheight = tree.coneheight;
				newtree.seednewly_produced = tree.seednewly_produced;
				// newtree.seedproduced = tree.seedproduced;
				// newtree.buffer = tree.buffer;
				newtree.densitywert = tree.densitywert;
				newtree.thawing_depthinfluence = tree.thawing_depthinfluence;
				newtree.longdispersed = tree.longdispersed;
				newtree.growing = tree.growing;

				// add
				tree_list_copy.add(newtree);

				if (countertree == 1) {
					cout << tree.height << " <-tree ... Treeheight ... newtree. " << newtree.height << endl;
				}

				++countertree;
			}
        }
        tree_list_copy.consolidate();

        cout << "tree_list.size() after copying=" << tree_list.size() << endl;
        cout << "tree_list_copy.size() after copying=" << tree_list_copy.size() << endl;
        cout << endl;
        cout << endl;
    }

    // copying Seeds
    aktort = 0;
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list_copy.begin(); posw != world_seed_list_copy.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;
        // opening the original seed list and the referring reset list ..._copy
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list_copy = *world_positon_s;
        aktort++;

        cout << endl;
        cout << "seed_list.size() before copying =" << seed_list.size() << endl;
        cout << "seed_list_copy.size() before copying =" << seed_list_copy.size() << endl;

        seed_list_copy = seed_list;

        cout << "seed_list.size() after copying=" << seed_list.size() << endl;
        cout << "seed_list_copy.size() after copying=" << seed_list_copy.size() << endl;
        cout << endl;
        cout << endl;
    }

    // copying Envirgrid
    aktort = 0;
    for (vector<vector<Envirgrid>>::iterator posw = world_plot_list_copy.begin(); posw != world_plot_list_copy.end(); ++posw) {
        vector<Envirgrid>& plot_list = *posw;
        // opening the original plot list and the referring reset list ..._copy
        vector<vector<Envirgrid>>::iterator posiweltk = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list_copy = *posiweltk;
        aktort++;

        cout << endl;
        cout << "plot_list.size() before copying =" << plot_list.size() << endl;
        cout << "plot_list_copy.size() before copying =" << plot_list_copy.size() << endl;

        int countertotalmaps = 1;  // for displaying the first plot list element
        for (vector<Envirgrid>::iterator pos = plot_list.begin(); pos != plot_list.end();) {
            auto& pEnvirgrid = (*pos);

            Envirgrid pEnvirgrid_copy;

            // pEnvirgrid_copy.xworldcoo = pEnvirgrid.xworldcoo;
            // pEnvirgrid_copy.yworldcoo = pEnvirgrid.yworldcoo;
            // pEnvirgrid_copy.xcoo = pEnvirgrid.xcoo;
            // pEnvirgrid_copy.ycoo = pEnvirgrid.ycoo;
            pEnvirgrid_copy.Treedensityvalue = pEnvirgrid.Treedensityvalue;
            pEnvirgrid_copy.Treenumber = pEnvirgrid.Treenumber;
            pEnvirgrid_copy.maxthawing_depth = pEnvirgrid.maxthawing_depth;
            // pEnvirgrid_copy.litterheight = pEnvirgrid.litterheight;
            // pEnvirgrid_copy.litterheight0 = pEnvirgrid.litterheight0;
            // pEnvirgrid_copy.litterheight1 = pEnvirgrid.litterheight1;
            // pEnvirgrid_copy.litterheight2 = pEnvirgrid.litterheight2;
            // pEnvirgrid_copy.litterheight3 = pEnvirgrid.litterheight3;
            // pEnvirgrid_copy.litterheight4 = pEnvirgrid.litterheight4;
            // pEnvirgrid_copy.litterheight5 = pEnvirgrid.litterheight5;
            // pEnvirgrid_copy.litterheight6 = pEnvirgrid.litterheight6;
            // pEnvirgrid_copy.litterheight7 = pEnvirgrid.litterheight7;
            // pEnvirgrid_copy.litterheight8 = pEnvirgrid.litterheight8;
            // pEnvirgrid_copy.litterheight9 = pEnvirgrid.litterheight9;
            // pEnvirgrid_copy.litterheightmean = pEnvirgrid.litterheightmean;
            plot_list_copy.emplace_back(std::move(pEnvirgrid_copy));

            if (countertotalmaps == 1) {
                cout << pEnvirgrid.Treedensityvalue << " <-pEnvirgrid ... EnvirgridTreedensity ... pEnvirgrid_copy. " << pEnvirgrid_copy.Treedensityvalue
                     << endl;
            }

            ++pos;
            ++countertotalmaps;
        }

        cout << "plot_list.size() after copying=" << plot_list.size() << endl;
        cout << "plot_list_copy.size() after copying=" << plot_list_copy.size() << endl;
        cout << endl;
        cout << endl;
    }

    // copying Evaluation
    aktort = 0;
    for (vector<vector<Evaluation>>::iterator posw = world_evaluation_list_copy.begin(); posw != world_evaluation_list_copy.end(); ++posw) {
        vector<Evaluation>& EvaluationListe = *posw;
        // opening the standard list to push back elements to it from the reset list "..._copy"
        vector<vector<Evaluation>>::iterator posiwelta = (world_evaluation_list.begin() + aktort);
        vector<Evaluation>& EvaluationListe_copy = *posiwelta;
        aktort++;

        cout << endl;
        cout << "EvaluationListe.size() before copying=" << EvaluationListe.size() << endl;
        cout << "EvaluationListe_copy.size() before copying=" << EvaluationListe_copy.size() << endl;

        int counterauswert = 1;
        for (vector<Evaluation>::iterator pos = EvaluationListe.begin(); pos != EvaluationListe.end();) {
            auto& pEvaluation = (*pos);

            Evaluation pEvaluation_copy;

            // pEvaluation_copy.xworldcoo = pEvaluation.xworldcoo;
            // pEvaluation_copy.yworldcoo = pEvaluation.yworldcoo;
            for (vector<double>::iterator posval = pEvaluation.basalarealist.begin(); posval < pEvaluation.basalarealist.end();) {
                pEvaluation_copy.basalarealist.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.basalarearunmeanlist.begin(); posval < pEvaluation.basalarearunmeanlist.end();) {
                pEvaluation_copy.basalarearunmeanlist.push_back((*posval));
                ++posval;
            }
            pEvaluation_copy.maxincrementbasalarea = pEvaluation.maxincrementbasalarea;
            pEvaluation_copy.countermaxincrementbasalarea = pEvaluation.countermaxincrementbasalarea;
            pEvaluation_copy.yearofturningpoint = pEvaluation.yearofturningpoint;
            pEvaluation_copy.yearofequilibrium = pEvaluation.yearofequilibrium;
            pEvaluation_copy.postyearofturningpoint = pEvaluation.postyearofturningpoint;
            for (vector<int>::iterator posval = pEvaluation.nheight0b40liste.begin(); posval < pEvaluation.nheight0b40liste.end();) {
                pEvaluation_copy.nheight0b40liste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.nheight0b40runmeanliste.begin(); posval < pEvaluation.nheight0b40runmeanliste.end();) {
                pEvaluation_copy.nheight0b40runmeanliste.push_back((*posval));
                ++posval;
            }
            for (vector<int>::iterator posval = pEvaluation.nheight41b200liste.begin(); posval < pEvaluation.nheight41b200liste.end();) {
                pEvaluation_copy.nheight41b200liste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.nheight41b200runmeanliste.begin(); posval < pEvaluation.nheight41b200runmeanliste.end();) {
                pEvaluation_copy.nheight41b200runmeanliste.push_back((*posval));
                ++posval;
            }
            for (vector<int>::iterator posval = pEvaluation.nheight201b10000liste.begin(); posval < pEvaluation.nheight201b10000liste.end();) {
                pEvaluation_copy.nheight201b10000liste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.nheight201b10000runmeanliste.begin(); posval < pEvaluation.nheight201b10000runmeanliste.end();) {
                pEvaluation_copy.nheight201b10000runmeanliste.push_back((*posval));
                ++posval;
            }

            for (vector<double>::iterator posval = pEvaluation.meanbreastdiameterliste.begin(); posval < pEvaluation.meanbreastdiameterliste.end();) {
                pEvaluation_copy.meanbreastdiameterliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meanbreastdiameterrunmeanliste.begin();
                 posval < pEvaluation.meanbreastdiameterrunmeanliste.end();) {
                pEvaluation_copy.meanbreastdiameterrunmeanliste.push_back((*posval));
                ++posval;
            }

            for (vector<int>::iterator posval = pEvaluation.stemcountliste.begin(); posval < pEvaluation.stemcountliste.end();) {
                pEvaluation_copy.stemcountliste.push_back((*posval));
                ++posval;
            }
            for (vector<int>::iterator posval = pEvaluation.stemcountrunmeanliste.begin(); posval < pEvaluation.stemcountrunmeanliste.end();) {
                pEvaluation_copy.stemcountrunmeanliste.push_back((*posval));
                ++posval;
            }

            for (vector<double>::iterator posval = pEvaluation.meantreeheightliste.begin(); posval < pEvaluation.meantreeheightliste.end();) {
                pEvaluation_copy.meantreeheightliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meantreeheightrunmeanliste.begin(); posval < pEvaluation.meantreeheightrunmeanliste.end();) {
                pEvaluation_copy.meantreeheightrunmeanliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meantreeageliste.begin(); posval < pEvaluation.meantreeageliste.end();) {
                pEvaluation_copy.meantreeageliste.push_back((*posval));
                ++posval;
            }
            for (vector<double>::iterator posval = pEvaluation.meantreeagerunmeanliste.begin(); posval < pEvaluation.meantreeagerunmeanliste.end();) {
                pEvaluation_copy.meantreeagerunmeanliste.push_back((*posval));
                ++posval;
            }

            EvaluationListe_copy.emplace_back(std::move(pEvaluation_copy));

            if (counterauswert == 1) {
                cout << pEvaluation.maxincrementbasalarea << " <-pEvaluation ... Evaluationmaxincrementbasalarea ... pEvaluation_copy. "
                     << pEvaluation_copy.maxincrementbasalarea << endl;
            }

            ++pos;
            ++counterauswert;
        }

        cout << "EvaluationListe.size() after copying=" << EvaluationListe.size() << endl;
        cout << "EvaluationListe_copy.size() after copying=" << EvaluationListe_copy.size() << endl;
        cout << endl;
        cout << endl;
    }
}

void Clearalllists_copy(void) {
    int aktort = 0;
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list_copy.begin(); posw != world_tree_list_copy.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list_copy.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
        vector<vector<Envirgrid>>::iterator posiweltk = (world_plot_list_copy.begin() + aktort);
        vector<Envirgrid>& plot_list = *posiweltk;
        vector<vector<Evaluation>>::iterator posiwelt = (world_evaluation_list_copy.begin() + aktort);
        vector<Evaluation>& evaluation_list = *posiwelt;
        aktort++;

        // remove elements
        tree_list.clear();
        seed_list.clear();
        plot_list.clear();
        evaluation_list.clear();
    }
}
