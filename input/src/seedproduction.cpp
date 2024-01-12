#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

void Seedproduction(Parameter* parameter, vector<VectorList<Tree>>& world_tree_list) {
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;

#pragma omp parallel for default(shared) schedule(guided)
        for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
            auto& tree = tree_list[tree_i];
			if (tree.growing == true) {
				if (tree.cone == true) {
					int newseedsproduced = speciestrait[tree.species].seedprodfactor                           // seed production in dependence of a factor
											* ((double)tree.height / 10 / 100)                     // ... the tree's  height in m...
											* ((double)tree.dbasalrel / 1000)                       // ... the tree's current growth in cm...
											* (1.0 - (1.0 / ((double)tree.height / 10 / 50)));  // ... height.
					if (newseedsproduced > 0) {
// if(newseedsproduced>1000)
	// cout << " ... seedprod>1000 = " << newseedsproduced << " <- " << tree.height / 10 << " ... " << tree.dbasalrel / 1000 << " ... " << speciestrait[tree.species].seedprodfactor << endl;
	// cout << " seedprod = " << newseedsproduced << " <- " << (double)tree.height / 10 << " ... " << (double)tree.dbasalrel / 1000 << " ... " << speciestrait[tree.species].seedprodfactor << endl;
						tree.seednewly_produced = newseedsproduced;
					} else {
						tree.seednewly_produced = 0;
					}
				}
			}
        }
    }
}