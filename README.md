# LAVESI-PESTFIRE 1.0
This source code can be compiled to build the boreal forest dynamic vegetation simulator LAVESI-WIND. It features wind-dependent seed dispersal and pollination. and parallelisation of computational intensive parts of the code. This program simulates the vegetation dynamics of boreal forest taxa in Siberia and North America which are driven by monthly temperature and precipitation data series, as well as wind data from the vegetation period (May-Aug). Additionally, this version includes dynamical fire disturbances now available pan-boreal and circum-arctic and includes a pest outbreak simulation.

### Version history:
- The version of this code is based on LAVESI-WIND v1.0 
- The former version LAVESI v1.01 and its building process and application is described in the publication in Ecological Modelling: Stefan Kruse, Mareike Wieczorek, Florian Jeltsch and Ulrike Herzschuh (2016) Treeline dynamics in Siberia under changing climates as inferred from an individual-based model for Larix. Ecological Modelling, 338, 101–121. http://dx.doi.org/10.1016/j.ecolmodel.2016.08.003 Additional data and results are available at https://doi.pangaea.de/10.1594/PANGAEA.863584 Code available at https://doi.org/10.5281/zenodo.1155486

## CHANGELOG (only major changes in functionalities):
- includes fire parameters parameterized for global application
- includes reading pest disturbance impact and weather dependency table
- outbreak control mortality and defoliation
 
### Authors:
- Stefan Kruse - stefan.kruse@awi.de
- Luca Farkas - luca.farkas@awi.de
- Lisa Trimborn - lisa.trimborn@awi.de
- Fenja Sevke - fenja.sevke@awi.de

## Containing files:
1. Source code and header files: "....cpp" in/src/ and "....h" in /inc/ , and the makefile "Makefile"
2. Parameter file: "parameter.txt" and species parameter file "specieslist.csv"
3. Input parameter for pest outbreaks: "pest_outbreak_prediction.csv" and fire parameters for 3 different radii (50, 100, 300 km) e.g. "fireparameters_300km.csv"
			
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
3. include environment and forcing files (available by the authors) and update according path variables in code
