#include <random>
#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

void Ageing(struct Parameter* parameter, vector<VectorList<Tree>>& world_tree_list, vector<VectorList<Seed>>& world_seed_list) {
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;
// pragma omp initializing
omp_set_dynamic(1); //enable dynamic teams
omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

#pragma omp parallel
{
#pragma omp for
        for (unsigned int i = 0; i < seed_list.size(); ++i) {
            auto& seed = seed_list[i];
            if (seed.dead) {
                continue;
            }
            seed.age++;

            // seeds older than gmelseedmaxage years (L.gmelinii) and 10 years (L.sibirica) die
            if (((seed.species == 1) && (seed.age > parameter[0].gmelseedmaxage)) || ((seed.species == 2) && (seed.age > 10))) {
                seed.dead = true;
                seed_list.remove(i);
            }
        }
} // pragma
        seed_list.consolidate();

    int mat_age_length = 183;  // length of array maturationheight
    // height values in percent (0-99) computed externally
    unsigned short int maturationheight[] = {200,  201,  202,  203,  204,  205,  206,  207,  208,  209,  210,  211,  212,  213,   214,  215,  216,  217,  218,  219,  220,
                                 221,  222,  223,  224,  225,  226,  227,  228,  229,  230,  231,  232,  233,  234,   235,  236,  237,  238,  239,  240,  241,
                                 242,  243,  244,  245,  246,  248,  249,  250,  251,  252,  253,  254,  256,  257,   258,  259,  260,  262,  263,  264,  266,
                                 267,  269,  270,  271,  273,  274,  276,  277,  279,  281,  282,  284,  286,  287,   289,  291,  293,  295,  297,  299,  300,
                                 303,  305,  307,  309,  311,  313,  316,  318,  320,  323,  325,  328,  330,  333,   336,  339,  341,  344,  347,  350,  354,
                                 357,  360,  364,  367,  371,  374,  378,  382,  386,  390,  395,  399,  404,  408,   413,  418,  423,  428,  434,  439,  445,
                                 451,  458,  464,  471,  478,  485,  493,  500,  509,  517,  526,  535,  545,  555,   566,  577,  588,  600,  613,  627,  641,
                                 656,  672,  689,  707,  725,  746,  767,  790,  815,  841,  870,  900,  934,  970,   1010, 1053, 1100, 1153, 1212, 1277, 1350,
                                 1434, 1529, 1639, 1767, 1919, 2100, 2323, 2600, 2958, 3434, 4100, 5100, 6767, 10100, 20100};

    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;
			std::random_device random_dev;
// pragma omp initializing
omp_set_dynamic(1); //enable dynamic teams
omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

#pragma omp parallel
{
			std::mt19937 rng(random_dev());
			std::uniform_real_distribution<double> uniform(0, 1);

#pragma omp for
			for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
				auto& tree = tree_list[tree_i];

				tree.age++;

				if (tree.cone == false) {
					if (tree.coneheight == 65535) {
						// trees reaching the maturation age are assigned a minimum height value for them to bear cones
						if (tree.age > parameter[0].coneage) {
							// calculate random position in the array of maturation heights defined earlier
							// ... in this there are values between 0 and 182 (corresp. to (0,1) )
							int fraction = 0 + (int)floor(((mat_age_length - 1) * uniform(rng)));

							// possibility for a tree <2m to maturate
							if (fraction == 0) {
								tree.coneheight = (unsigned short int) 100 + ((double)100 * uniform(rng));
							} else {
								tree.coneheight = maturationheight[fraction];
							}
						}
					}
					// tree already has a height of maturation assigned to it
					// ... if a tree is taller than this maturation height, he starts to produce seeds
					else if (tree.coneheight != 65535) {
						if (tree.height >= tree.coneheight) {
							tree.cone = true;
						}
					}
				} else if (tree.cone == true) {
					tree.seednewly_produced = 0;
				}
			}
} // pragma
		} // tree list
	} // world list
}

