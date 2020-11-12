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

omp_set_dynamic(0);                                 // disable dynamic teams ; TODO remove here
omp_set_num_threads(parameter[0].omp_num_threads);  // set the number of helpers ; TODO remove here
#pragma omp parallel
{
// split list and assigne to threads
int thread_count = omp_get_num_threads();
int thread_num = omp_get_thread_num();
size_t chunk_size = tree_list.size() / thread_count;
auto begin = tree_list.begin();
std::advance(begin, thread_num * chunk_size);
auto end = begin;

if (thread_num == (thread_count - 1))  // last thread takes the remaining elements
{
	end = tree_list.end();
} else {
	std::advance(end, chunk_size);
}
			
#pragma omp barrier
        // for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end();++pos) {
            // auto pTree = (*pos);
		for (auto it = begin; it != end; ++it) {
			auto pTree = (*it);

            if (pTree->cone == true) {
                int newseedsproduced = (int)floor(parameter[0].seedprodfactor               // seed production in dependence of a factor
                                                  * (double)pTree->height/100 / 100                     // ... the tree's  height in m...
                                                  * (double)pTree->dbasalrel/1000                        // ... the tree's current growth in cm...
                                                  * (1.0 - (1.0 / ((double)pTree->height/100 / 50))));  // ... height.
                if (newseedsproduced > 0) {
                    pTree->seednewly_produced = newseedsproduced;
                } else {
                    pTree->seednewly_produced = 0;
                }
            }
        }
}  // pragma
    }
}

