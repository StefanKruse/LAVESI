#include "LAVESI.h"

using namespace std;

/****************************************************************************************/
/**
 * \brief calculate  seed production
 *
 * depends on seed production factor, tree size, actual growth
 *******************************************************************************************/

void Seedproduction(struct Parameter* parameter, vector<list<Tree*>>& world_tree_list) {
    for (vector<list<Tree*>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        list<Tree*>& tree_list = *posw;

        for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();) {
            auto pTree = (*pos);

            if (pTree->cone == true) {
                int newseedsproduced = (int)floor(parameter[0].seedprodfactor               // seed production in dependence of a factor
                                                  * (double) pTree->height / 100                     // ... the tree's  height in m...
                                                  * (double) pTree->dbasalrel/1000                        // ... the tree's current growth in cm...
                                                  * (1.0 - (1.0 / ((double) pTree->height / 50))));  // ... height.
                if (newseedsproduced > 0) {
                    pTree->seednewly_produced = newseedsproduced;
                } else {
                    pTree->seednewly_produced = 0;
                }
                ++pos;
            } else {
                ++pos;
            }
        }
    }
}
