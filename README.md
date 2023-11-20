# LAVESI-FIRE v1.0
This source code can be compiled to build the <i>Larix</i> vegetation simulator LAVESI-FIRE, with wind-dependent seed dispersal and pollination and parallelisation of computational intensive parts of the code. This program simulates the vegetation dynamics of multiple tree species (<i>Larix gmelinii</i> (RUPR.) RUPR., <i>L. cajanderi</i>, <i>L. sibirica</i>, <i>Picea obovata</i>, <i>Pinus sylvestris</i>, and <i>P. sibirica</i>) which are driven by monthly temperature and precipitation data series, as well as wind data from the vegetation period (May-Aug). This version also supports simulation of annual, climate-driven wildfires with impacts to trees, seeds and the litter layer.

### Version history:
The version of this code is based on LAVESI-WIND (v2.0) with landscape for which details can be found in the publication in Gescientific Model Development: Stefan Kruse, Simone Maria Stuenzi, Julia Boike, Moritz Langer, Josias Gloy, Ulrike Herzschuh (2022). Novel coupled permafrost–forest model (LAVESI–CryoGrid v1.0) revealing the interplay between permafrost, vegetation, and climate across eastern Siberia. Geoscientific Model Development, 15, 2395–2422. https://doi.org/10.5194/gmd-15-2395-2022
LAVESI-WIND v1.0 was described in detail in: Stefan Kruse, Alexander Gerdes, Nadja J. Kath, Ulrike Herzschuh (2018). Implementing spatially explicit wind-driven seed and pollen dispersal in the individual-based larch simulation model: LAVESI-WIND 1.0. Geoscientific Model Development, 11, 4451–4467. https://doi.org/10.5194/gmd-11-4451-2018
The former version LAVESI v1.01 and its building process and application is described in the publication in Ecological Modelling: Stefan Kruse, Mareike Wieczorek, Florian Jeltsch and Ulrike Herzschuh (2016) Treeline dynamics in Siberia under changing climates as inferred from an individual-based model for Larix. Ecological Modelling, 338, 101–121. http://dx.doi.org/10.1016/j.ecolmodel.2016.08.003 Additional data and results are available at https://doi.pangaea.de/10.1594/PANGAEA.863584 Code available at https://doi.org/10.5281/zenodo.1155486
 
### Authors:
- Stefan Kruse - stefan.kruse@awi.de
- Ramesh Glückler - ramesh.glueckler@awi.de
- Josias Gloy - josias.gloy@awi.de
- Of previous versions:
- Alexander Gerdes - alexander.gerdes@awi.de
- Nadja J. Kath - nkath@uni-potsdam.de

## Containing files:
1. Source code and header files: "....cpp" in/src/ and "....h" in /inc/ , and the makefile "Makefile"
2. Parameter file: "parameter.txt"
3. Climate forcing data as monthly temperature and precipitation, using MPI-ESM1.2 [1], MPI-ESM1.2-HR & -LR (SSP126 via CMIP6) [2], CRU TS v4.06 [3], MPI-ESM-CR [4], and TraCE-21ka [5]
4. Six-hourly wind data from ERA5 [6]
5. Landscape input using TanDEM-X 90 m [7] 

### References:
	[1] Dallmeyer, A., Kleinen, T., Claussen, M., Weitzel, N., Cao, X., Herzschuh, U., 2022. The deglacial forest conundrum. Nat Commun 13, 6035. https://doi.org/10.1038/s41467-022-33646-6
 	[2] O’Neill, B.C., Tebaldi, C., van Vuuren, D.P., Eyring, V., Friedlingstein, P., Hurtt, G., Knutti, R., Kriegler, E., Lamarque, J.-F., Lowe, J., Meehl, G.A., Moss, R., Riahi, K., Sanderson, B.M., 2016. The Scenario Model Intercomparison Project (ScenarioMIP) for CMIP6. Geoscientific Model Development 9, 3461–3482. https://doi.org/10.5194/gmd-9-3461-2016
  	[3] Harris, I., Osborn, T.J., Jones, P., Lister, D., 2020. Version 4 of the CRU TS monthly high-resolution gridded multivariate climate dataset. Sci Data 7, 109. https://doi.org/10.1038/s41597-020-0453-3
   	Kapsch, M.-L., Mikolajewicz, U., Ziemen, F., Schannwell, C., 2022. Ocean Response in Transient Simulations of the Last Deglaciation Dominated by Underlying Ice-Sheet Reconstruction and Method of Meltwater Distribution. Geophysical Research Letters 49, e2021GL096767. https://doi.org/10.1029/2021GL096767
	[5] He, F., 2011. Simulating Transient Climate Evolution of the Last Deglaciation with CCSM3. Ph.D thesis, Madison, WI: University of Wisconsin-Madison.
 	[6] Hersbach, H., Bell, B., Berrisford, P., Hirahara, S., Horányi, A., Muñoz-Sabater, J., Nicolas, J., Peubey, C., Radu, R., Schepers, D., Simmons, A., Soci, C., Abdalla, S., Abellan, X., Balsamo, G., Bechtold, P., Biavati, G., Bidlot, J., Bonavita, M., De Chiara, G., Dahlgren, P., Dee, D., Diamantakis, M., Dragani, R., Flemming, J., Forbes, R., Fuentes, M., Geer, A., Haimberger, L., Healy, S., Hogan, R.J., Hólm, E., Janisková, M., Keeley, S., Laloyaux, P., Lopez, P., Lupu, C., Radnoti, G., de Rosnay, P., Rozum, I., Vamborg, F., Villaume, S., Thépaut, J.-N., 2020. The ERA5 global reanalysis. Quarterly Journal of the Royal Meteorological Society 146, 1999–2049. https://doi.org/10.1002/qj.3803
	[7] Rizzoli, P., Martone, M., Gonzalez, C., Wecklich, C., Borla Tridon, D., Bräutigam, B., Bachmann, M., Schulze, D., Fritz, T., Huber, M., Wessel, B., Krieger, G., Zink, M., Moreira, A., 2017. Generation and performance assessment of the global TanDEM-X digital elevation model. ISPRS Journal of Photogrammetry and Remote Sensing 132, 119–139. https://doi.org/10.1016/j.isprsjprs.2017.08.008
			
## Start a simulation:
1. Update the settings of the simulation run in the parameter file "parameter.txt"
2. Compile the code with the help of the makefile: "make parallel"
3. Create a output folder if not existing, e.g. by "mkdir output"
4. Execute the program with "./LAVESI_FIRE"

## Prerequisites:
- linux OS
- g++ compiler (install e.g. by 'sudo apt install g++')
- valgrind (www.valgrind.org, and follow installation instructions there)

## Further hints: 
1. Start in background not bound to your console session by "nohup ./LAVESI_FIRE &"
2. Change the area size of simulation in /inc/declarations.h at "...treerows" and "...cols", values given in meters
