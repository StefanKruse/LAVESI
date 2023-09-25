#include "LAVESI.h"
#include "VectorList.h"
#include "RandomNumber.h"
using namespace std;

void Seedproduction(Parameter* parameter, vector<VectorList<Tree>>& world_tree_list) {
    for (vector<VectorList<Tree>>::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw) {
        VectorList<Tree>& tree_list = *posw;

	RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
        for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
            auto& tree = tree_list[tree_i];
			if (tree.growing == true) {
				if (tree.cone == true) {
					double ranc = uniform.draw();
					// printf("deciding if to clone or seed. random: %f4.4 ", ranc);
					// printf("clonedsetvariable: %d ", tree.cloned);
					if (ranc*100>=1 && tree.cloned!=true){
					// printf("seeds choosen");
				
						int newseedsproduced = (speciestrait[tree.species].seedprodfactor                           // seed production in dependence of a factor
												* ((double)tree.height / 10 / 100)                     // ... the tree's  height in m...
												* ((double)tree.dbasalrel / 1000)                       // ... the tree's current growth in cm...
												* (1.0 - (1.0 / ((double)tree.height / 10 / 50)))  // ... height.
												// *pow((double)tree.seednumber,10)
												)/ pow((double)tree.seedweight,parameter[0].seednumberfactor);
						if (newseedsproduced > 0) {
	// if(newseedsproduced>1000)
		// cout << " ... seedprod>1000 = " << newseedsproduced << " <- " << tree.height / 10 << " ... " << tree.dbasalrel / 1000 << " ... " << speciestrait[tree.species].seedprodfactor << endl;
		// cout << " seedprod = " << newseedsproduced << " <- " << (double)tree.height / 10 << " ... " << (double)tree.dbasalrel / 1000 << " ... " << speciestrait[tree.species].seedprodfactor << endl;
							tree.seednewly_produced = newseedsproduced;
						} else {
							tree.seednewly_produced = 0;
						}
					} else {
						// printf("cloning choosen");
						tree.cloningactive= true;
						tree.seednewly_produced = 0;
					}
					// cout << "tree.seednewly_produced = " << tree.seednewly_produced << endl;
				}
			}
        }
    }
}

