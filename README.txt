/* ------------------------------------ Date: 22.01.2018 */

LAVESI-WIND 1.0

This source code can be compiled to build the <i>Larix</i> vegetation simulator LAVESI-WIND, updated by wind-dependent seed dispersal and parallelisation computationally intense parts of the code. This program simulates the vegetation dynamics of the larch tree species <i>Larix gmelinii</i> (RUPR.) RUPR. which are driven by monthly temperature and precipitation data series, as well as wind data from the vegetation period (May-Aug).

Version history:
 - The version of this code is LAVESI-WIND 1.0 and is SUBMITTED for review in Geoscientific Model Development.
 - The former version LAVESI v1.01 and its building process and application is described in the publication in Ecological Modelling: Stefan Kruse, Mareike Wieczorek, Florian Jeltsch and Ulrike Herzschuh (2016) Treeline dynamics in Siberia under changing climates as inferred from an individual-based model for Larix. Ecological Modelling, 338, 101–121. http://dx.doi.org/10.1016/j.ecolmodel.2016.08.003 Additional data and results are available at https://doi.pangaea.de/10.1594/PANGAEA.863584 Code available at https://doi.org/10.5281/zenodo.1155486

Authors:
- Stefan Kruse - stefan.kruse@awi.de
- Alexander Gerdes - alexander.gerdes@awi.de
- Nadja J. Kath - nkath@uni-potsdam.de

/* ------------------------------------ */

Containing files:
1. Source code and header files: "....cpp" in/src/ and "....h" in /inc/ , and the makefile "Makefile"
2. Parameter file: "parameter.txt"
3. Forcing data in /input/
3.1. temperature and precipitation series
		"..._1919-2011.txt" contain the climate data, which is the monthly mean temperature and precipitation sum supplied for six locations. It is extracted from the CRU TS 3.22 gridded data of grid cells which cover visited field sites (Harris et al., 2014 [1]). See parameter.txt for more information.
3.2. wind data in /input/winddaten/
		"winddaten..._EraInterim.dat" contain pairs of wind speed in m/s and wind direction in degrees [°]. The winds at 10 m above the surface, of the years 1979-2012 and in 6-hourly-resolution, were extracted only for the vegetation period (May-Aug) from the ERA-Interim reanalysis data set (Balsamo et al., 2015 [2]).

References:
	[1] Harris, I., Jones, P.D., Osborn, T.J., Lister, D.H., 2014. Updated high-resolution grids of monthly climatic observations - the CRU TS3.10 Dataset. Int. J. Climatol. 34, 623–642. https://doi.org/10.1002/joc.3711
	[2] Balsamo, G., Albergel, C., Beljaars, A., Boussetta, S., Brun, E., Cloke, H., Dee, D., Dutra, E., Munõz-Sabater, J., Pappenberger, F., De Rosnay, P., Stockdale, T., Vitart, F., 2015. ERA-Interim/Land: A global land surface reanalysis data set. Hydrol. Earth Syst. Sci. 19, 389–407. https://doi.org/10.5194/hess-19-389-2015
			
/* ------------------------------------ */

Start a simulation:
1. compile the code with the help of the makefile: "make parallel"
2. update the settings of the simulation run in the parameter file "parameter.txt"
3. create the output folder "mkdir output"
3. execute the program: "./LAVESI"

Prerequisites:
- linux OS
- g++ compiler (install e.g. by 'sudo apt install g++')
- valgrind (www.valgrind.org, and follow installation instructions there)

Further hints: 
1. start in background not bound to your console session by "nohup ./LAVESI_WIND &"
2. change the area size of simulation in /inc/declarations.h at "...treerows" and "...cols", values given in meters


/* ------------------------------------ */