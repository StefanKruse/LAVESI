/* ------------------------------------ Date: 17.02.2017 */

This source code can be compiled to build the Larix vegetation simulator LAVESI. This program simulates the vegetation dynamics of the larch tree species LARIX GMELINII (RUPR.) RUPR. which are driven by monthly weater data series.

The version of this code is 1.01. and the building process and application is described in the following publication in Ecological Modelling: 
Stefan Kruse, Mareike Wieczorek, Florian Jeltsch and Ulrike Herzschuh (2016) Treeline dynamics in Siberia under changing climates as inferred from an individual-based model for Larix. Ecological Modelling, 338, 101–121. http://dx.doi.org/10.1016/j.ecolmodel.2016.08.003
Additional data and results are available at https://doi.pangaea.de/10.1594/PANGAEA.863584

Authors:
- Stefan Kruse - stefan.kruse@awi.de
- Mareike Wieczorek - mareike.wieczorek@awi.de

/* ------------------------------------ */

Containing files:
1. Source code and header files: "....cpp" and "....h" and the makefile "Makefile"
2. Parameter file: "parameter.txt"
3. Climate data: "..._1919-2011.txt" contain the climate data, which is the monthly mean temperature and precipitation sum is supplied for six locations. It is extracted from the CRU TS 2.1 gridded data of grid cells which cover visited field sites (Mitchell and Jones, 2005) and prolonged until 2011 with climate station data of the station Khatanga [data accessed on 2nd May 2012 from Global Historical Climatology Network (GHCN)-Daily; Menne et al., 2012)]. See parameter.txt for more information.

References:
Menne M.J., Durre I., Vose R.S., Gleason B.E. & Houston T.G. (2012) An overview of the Global Historical Climatology Network-Daily Database. Journal of Atmospheric and Oceanic Technology, 29, 897–910.
Mitchell T.D. & Jones P.D. (2005) An improved method of constructing a database of monthly climate observations and associated high-resolution grids. International Journal of Climatology, 25, 693–712. 
			
/* ------------------------------------ */
