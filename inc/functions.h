#pragma once

#include <vector>
#include "VectorList.h"
#include "structures.h"

using namespace std;

double Parameterinput(const char* uebergabestring, int stringlengthmax, const char* divisionsign, Parameter* parameter);
void Weatherinput(Parameter* parameter, int stringlengthmax, vector<vector<Weather>>& world_weather_list);
void Dataoutput(int t,
                int jahr,
                Parameter* parameter,
                int yearposition,
                vector<VectorList<Tree>>& world_tree_list,
                vector<VectorList<Seed>>& world_seed_list,
                vector<vector<Weather>>& world_weather_list,
                vector<vector<Envirgrid>>& world_plot_list,
                vector<vector<Evaluation>>& world_evaluation_list);
void Treedistribution(Parameter* parameter, int stringlengthmax);
void Hinterlandseedintro(Parameter* parameter, int yearposition, vector<VectorList<Seed>>& world_seed_list, vector<vector<Weather>>& world_weather_list);
void Environmentupdate(//Parameter* parameter,
                       int yearposition,
                       vector<vector<Envirgrid>>& world_plot_list,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<vector<Weather>>& world_weather_list);
void Growth(Parameter* parameter, int yearposition, vector<VectorList<Tree>>& world_tree_list, vector<vector<Weather>>& world_weather_list);
void Seeddispersal(//int Jahr, 
				   Parameter* parameter, 
				   vector<VectorList<Seed>>& world_seed_list, 
				   vector<vector<Envirgrid>>& world_plot_list);
void Seedproduction(Parameter* parameter, vector<VectorList<Tree>>& world_tree_list);
void Treeestablishment(Parameter* parameter,
                       int yearposition,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<VectorList<Seed>>& world_seed_list,
                       vector<vector<Weather>>& world_weather_list,
                       vector<vector<Envirgrid>>& world_plot_list);
void Fire(Parameter* parameter,
                       int yearposition,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<VectorList<Seed>>& world_seed_list,
                       vector<vector<Weather>>& world_weather_list,
                       vector<vector<Envirgrid>>& world_plot_list);
void Mortality(Parameter* parameter,
               // int Jahr,
               int yearposition,
               vector<VectorList<Tree>>& world_tree_list,
               vector<VectorList<Seed>>& world_seed_list,
               vector<vector<Weather>>& world_weather_list);
void Ageing(Parameter* parameter, vector<VectorList<Tree>>& world_tree_list, vector<VectorList<Seed>>& world_seed_list);
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
                            double randomnumberwindfather);
void Seedwinddispersal(double rn, double& dx, double& dy, double& windspeed, double& winddirection, double parhei, int seedspec, double randomnumberwind);
void Parameterinput(void);
void Getspeciestraits(void);
void Getdisturbanceclimresponse(void);
void Getdisturbanceimpact(vector<Weather>& weather_list);
void Savealllists(void);
void Clearalllists(void);
void Restorealllists(void);
void Clearalllists_copy(void);
