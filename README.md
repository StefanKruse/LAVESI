# LAVESI-Mountain Treelines 1.0
This source code can be compiled to build the <i>Larix</i> vegetation simulator LAVESI-WIND, with wind-dependent seed dispersal and pollination and parallelisation of computational intensive parts of the code. This program simulates the vegetation dynamics of northern boreal forest taxa in Siberia or North America which are driven by monthly temperature and precipitation data series, as well as wind data from the vegetation period (May-Aug). It has dynamical fire disturbances and pest outbreak simulation. Additionally, this version includes more processes necessary at moutain treelines especially involving snow.

### Version history:
- The version of this code is based on LAVESI-WIND v1.0 and LAVESI-PEST v1.0
- The former version LAVESI v1.01 and its building process and application is described in the publication in Ecological Modelling: Stefan Kruse, Mareike Wieczorek, Florian Jeltsch and Ulrike Herzschuh (2016) Treeline dynamics in Siberia under changing climates as inferred from an individual-based model for Larix. Ecological Modelling, 338, 101–121. http://dx.doi.org/10.1016/j.ecolmodel.2016.08.003 Additional data and results are available at https://doi.pangaea.de/10.1594/PANGAEA.863584 Code available at https://doi.org/10.5281/zenodo.1155486

## CHANGELOG (only major changes in functionalities):
- computing snowfall and accumulation on a daily basis
- different impacts of snow on survival, growth, and by direct damages are considered
 
### Authors:
- Stefan Kruse - stefan.kruse@awi.de
- Sarah Haupt - sarah.haupt@awi.de
- Katharina Schildt - katharina.schildt@awi.de
- Lisa Trimborn - lisa.trimborn@awi.de
- Luca Farkas - luca.farkas@awi.de
- Josias Gloy - josias.gloy@awi.de

## Containing files:
1. Source code and header files: "....cpp" in/src/ and "....h" in /inc/ , and the makefile "Makefile"
2. Parameter file: "parameter.txt" and species parameter file "specieslist.csv"
			
## Start a simulation:
1. compile the code with the help of the makefile: "make parallel"
2. update the settings of the simulation run in the parameter file "parameter.txt"
3. create a output folder if not existing, e.g. by "mkdir output"
3. execute the program with "./LAVESI_WIND"

## Prerequisites:
- linux OS
- g++ compiler (install e.g. by 'sudo apt install g++')
- valgrind (www.valgrind.org, and follow installation instructions there)

## Further hints: 
1. start in background not bound to your console session by "nohup ./LAVESI_WIND &"
2. change the area size of simulation in /inc/declarations.h at "...treerows" and "...cols", values given in meters

## Usage
This version is presented and used in Haupt, S., Gloy, J., Farkas, L., Schildt, K., Trimborn, L., and Kruse, S.: The significant role of snow in shaping alpine treeline responses in modelled boreal forests, EGUsphere [preprint], https://doi.org/10.5194/egusphere-2024-4036, 2025. 

