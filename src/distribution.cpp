#include "LAVESI.h"
#include "VectorList.h"

using namespace std;

// TODO temporary here
extern vector<double> wdir, wspd;

void Pollinationprobability(double x,
                            double y,
                            Parameter* parameter,
                            vector<VectorList<Tree>>::iterator world_positon_b,
                            double direction,
                            double velocity,
                            unsigned int ripm,
                            // unsigned int cntr,
                            double p,
                            double kappa,
                            double phi,
                            double dr,
                            double dx,
                            double dy,
                            double I0kappa,
                            double pe,
                            double C,
                            double m,
                            vector<int>& pName,
                            vector<double>& thdpthinfl,
                            int outputtreesiter,
                            double randomnumberwind,
                            double randomnumberwindfather) {
    VectorList<Tree>& tree_list = *world_positon_b;
    direction = 0.0;
    velocity = 0.0;
    ripm = 0;
    // cntr = 0;
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
    pName.clear();
    pName.shrink_to_fit();
    thdpthinfl.clear();
    thdpthinfl.shrink_to_fit();

    if (parameter[0].windsource == 1) {  // EraInterim
        ripm = (int)(0.5 * wdir.size() + wdir.size() / 6 * (1 - 2 * randomnumberwind));
        direction = M_PI * (wdir.at(ripm) / 180);
        velocity = wspd.at(ripm);
    } else {  // random
        direction = 0.0 + ((double)(2 * M_PI) * randomnumberwind);
        velocity = 2.777;
    }
    pe = parameter[0].pollenfall / velocity;

    // Simpson integration + https://elec424.fandom.com/wiki/Modified_Bessel_Functions
    I0kappa = 0.16666 * (exp(kappa) + 4.0 + exp(-1.0 * kappa));

    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree_copy = tree_list[tree_i];
		
		if (tree_copy.growing == true) {
			if (tree_copy.cone == true) {  // only if the pollinating tree has cones:
				dx = ((double)tree_copy.xcoo / 1000) - x;
				dy = ((double)tree_copy.ycoo / 1000) - y;
				dr = sqrt(dx * dx + dy * dy);

				if ((dr != 0)) {
					phi = atan2(dx, dy);
				} else {
					phi = direction + 0.5 * M_PI;  // makes self-pollination less probable: p~1/I0(kappa)
				}

				p = exp(kappa * (cos(phi - direction) * -1)) / (2 * I0kappa) * (exp(-2 * pe * pow(dr, 1 - 0.5 * m) / (sqrt(M_PI) * C * (1 - 0.5 * m))));
				// f(dr) based on Microbiology of the atmosphere, p(phi) von Mises distribution

				if (randomnumberwindfather <= p) {
					// pName.push_back(tree_copy.name);
					thdpthinfl.push_back(100);
					// data output for pollen flight analysis
					if (parameter[0].pollination == 1 && parameter[0].omp_num_threads == 1 && outputtreesiter <= 5 && parameter[0].ivort >= 1046) {
						FILE* fdir;
						char filenamechar[25];
						sprintf(filenamechar, "IVORT%.4d_REP%.3d", parameter[0].ivort, parameter[0].repeati);
						string output = "output/windgen_pollination_total_" + string(filenamechar) + ".txt";
						fdir = fopen(output.c_str(), "a+");
						fprintf(fdir, "%10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t \n", dr, phi, p, (double)tree_copy.xcoo / 1000,
								(double)tree_copy.ycoo / 1000, x, y);
						fclose(fdir);
					}
				}
			}
		}
    }
}

double getEntfernung(double D, double ratiorn_help) {
    double entf_help;

    if (parameter[0].dispersalmode == 0) {  // random in certain distance
        entf_help = ((ratiorn_help - 1) * 100.0) / (0.05 - 1);
    } else if (parameter[0].dispersalmode == 1) {  // neg. exponential
        entf_help = parameter[0].distanceratio * ((log(ratiorn_help) / (-0.2)) / 0.16);
    } else if (parameter[0].dispersalmode == 2) {  // fat tailed/power law
        double fatalpha = 0.5;
        entf_help = parameter[0].distanceratio * pow(ratiorn_help, (-1 * (1 + fatalpha)));
    } else if (parameter[0].dispersalmode == 3) {  // gaussian
        double gaussweite = D, gaussmaxh = 1, gaussposcenter = 0;
        entf_help = parameter[0].distanceratio * sqrt(2 * pow(gaussweite, 2) * (-1 * log(ratiorn_help / gaussmaxh))) + gaussposcenter;
    } else if (parameter[0].dispersalmode == 4 || parameter[0].dispersalmode == 5) {  // gaussian combined with fat tailed:
        double gaussfatratio = 2.0;
        double gaussweite = D, gaussmaxh = 1, gaussposcenter = 0;
        double fatalpha = 0.5;
        entf_help = (0.5
                     * (gaussfatratio * (sqrt(2 * pow(gaussweite, 2) * (-1 * log(ratiorn_help / gaussmaxh))) + gaussposcenter)
                        + (1 / gaussfatratio) * parameter[0].distanceratio * (pow(ratiorn_help, (-1 * (1 + fatalpha))))));

    } else {
        printf("LAVESI was stopped\n\n");
        printf("... choice of dispersal mode (= %d) not available!\n", parameter[0].dispersalmode);
        exit(1);
    }

    return (entf_help);
}

void Seedwinddispersal(double rn, double& dx, double& dy, double& windspeed, double& winddirection, double parhei, int seedspec, double randomnumberwind) {
    int ripm = 0;
    double dispersaldistance = 0;
    double maxdispersaldistance = 0;
    double direction = 0.0;
    double velocity = 0.0;

    if (parameter[0].windsource == 1) {  // EraInterim
        // choose a month between May and September:
        ripm = (int)(0.5 * wdir.size() + wdir.size() / 6 * (1 - 2 * randomnumberwind));
        direction = M_PI * (wdir.at(ripm) / 180);
        velocity = (wspd.at(ripm));
    } else {  // random
        direction = 0.0 + ((double)(2 * M_PI) * randomnumberwind);
        velocity = 2.7777;
    }

    if (seedspec == 1) {
        maxdispersaldistance = (velocity * 0.75 * parhei * 0.01 / (parameter[0].seeddescentg));
    } else if (seedspec == 2) {
        maxdispersaldistance = (parameter[0].seedtravelbreezes * 0.75 * parhei * 0.01 / (parameter[0].seeddescents));
    }

    if (parameter[0].dispersalmode == 5) {
        dispersaldistance = getEntfernung(maxdispersaldistance, rn);
    } else {
        dispersaldistance = getEntfernung(maxdispersaldistance, rn);
    }

    // set return variables
    dy = cos(direction) * dispersaldistance;
    dx = sin(direction) * dispersaldistance;
    windspeed = velocity;
    winddirection = direction;
}
