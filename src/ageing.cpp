#include <random>
#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void Ageing(//Parameter* parameter, 
			vector<VectorList<Tree>>& world_tree_list, 
			vector<VectorList<Seed>>& world_seed_list) {
    for (vector<VectorList<Seed>>::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw) {
        VectorList<Seed>& seed_list = *posw;

#pragma omp parallel for default(shared) schedule(guided)
        for (unsigned int i = 0; i < seed_list.size(); ++i) {
            auto& seed = seed_list[i];
            if (!seed.dead) {
                seed.age++;
                // seeds older than seedmaxage years die
                if (seed.age > speciestrait[seed.species].seedmaxage) {
                    seed.dead = true;
                    seed_list.remove(i);
                }
            }
        }
        seed_list.consolidate();
		} // world seed list

        for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
            VectorList<Tree>& tree_list = *posw;

            RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
            for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
                auto& tree = tree_list[tree_i];
				
				if (tree.growing == true) {
					tree.age++;

					if (tree.cone == false) {
						if (tree.age > speciestrait[tree.species].coneage) {
							if(uniform.draw() < 0.1) {
								tree.cone = true;
								tree.seednewly_produced = 0;
							}
						}
					} else if (tree.cone == true) {
						tree.seednewly_produced = 0;
					}
				}
        }  // tree list
    }  // world list
}
