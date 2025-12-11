#pragma once

#include <vector>

using namespace std;

struct Tree {                       // sizeof variable //TODO: further could be replaced
    double dbasal;                  // 8->4	---> in cm, 0 to few meters -> unsigned int 4294967295 /10000 precision; could be replaced
    double dbreast;                 // 8->4	---> in cm, 0 to meters -> unsigned int 4294967295 /10000 precision; could be replaced
    double densitywert;             // 8->2	---> need to check range of values for replacment
	double heightsubordination;		// is 0 when tallest tree and increases towards 1 when smaller
    double thawing_depthinfluence;  // 8->2	---> need to check range of values for replacment
    unsigned int xcoo;  // 4	---> was double ---> only positive, -> unsigned int 4294967295 /1000 precision => mm which is sufficient and allows still 
                        // km long/wide simulations // TODO really only positive? -> see compiler warnings
    unsigned int ycoo;  // 4	---> was double // TODO really only positive? -> see compiler warnings
    // double dispersaldistance;			// 8	-> not in use right now
    // int xworldcoo;						// 4	-> not in use right now
    // int yworldcoo;						// 4	-> not in use right now
    // int name;							// 4	-> not in use right now
    // int namem;							// 4	-> not in use right now
    // int namep;							// 4	-> not in use right now
    // int line;							// 4	-> not in use right now
    // int generation;						// 4	-> not in use right now
    // int yr_of_establishment;				// 4	-> not in use anymore
    // int yr_of_dying;						// 4	-> not in use anymore
    // unsigned int seedproduced;			// 4	-> not in use right now; used only for visualisation
    // int buffer;							// 4	-> not in use right now
    unsigned short int dbasalmax;           // 2	---> one year max probably few centimeters -> unsigned short int /1000 max 65cm
    unsigned short int dbasalrel;           // 2	---> one year max probably few centimeters -> unsigned short int /1000
    unsigned short int dbreastrel;          // 2	---> one year max probably few centimeters -> unsigned short int /1000
    unsigned short int height;              // 2	in cm; max 65 m -> unsigned short int /10 still has 0.1 cm precision
    unsigned short int coneheight;          // 2	in cm; max 655 m -> 1 cm precision
    unsigned short int age;                 // 2 	---> was int
    unsigned short int seednewly_produced;  // 2 	---> was int
    signed short int species;               // 2 	---> was int
    unsigned short int envirimpact;         // 2
    unsigned short int twi;                 // 2
    short int elevation;                    // 2
    unsigned short int soilhumidity;        // 2
    bool cone;                              // 1	---> was int; could be further replaced by single use of coneheight
    bool longdispersed;                     // 1
    bool growing;                           // 1
	double firedamage;				// 2
    unsigned short int crownstart;          // 2	in cm; max 65 m -> unsigned short int /10 still has 0.1 cm precision
    unsigned short int relcrowndamage;      // 2	relative; 0-1 -> precision /1000 lead to 0.001 precision
    unsigned short int pestinfectancedamage;// 2	relative; 0-1 -> precision /1000 lead to 0.001 precision
    unsigned long int pestinfection; // numeric value encoding which pest species had an impact on this tree
	unsigned short int snowdepth;			// 2	in cm; max 65 m -> unsigned short int /10 still has 0.1 cm precision
};

struct Seed {  // sizeof variable //TODO: further could be replaced
    // double weight;					// 8	-> not in use right now
    // double dispersaldistance;		// 8	-> not in use right now
    // double maxgrowth;				// 8	-> not in use right now
    // double pollenfall;				// 8	-> not in use right now
    // double descent;					// 8	-> not in use right now
    double thawing_depthinfluence;  	// 8
    int xcoo;              				// 4	---> was double -> int with /1000 precision
    int ycoo;              				// 4	---> was double -> int with /1000 precision
    // int xworldcoo;					// 4	-> not in use right now
    // int yworldcoo;					// 4	-> not in use right now
    // int namem;						// 4	-> not in use right now
    // int namep;						// 4	-> not in use right now
    // int line;						// 4	-> not in use right now
    // int generation;					// 4	-> not in use right now
    unsigned short int
        releaseheight;         // 2		in cm; max 65 m -> unsigned short int /10 max 65 m and still has 0.1 cm precision
    unsigned short int age;    // 2	---> was int
    signed short int species;  // 2	---> was int
    bool incone;               // 1
    bool longdispersed;        // 1
    bool dead = false;         // 1
	int firemort;
};

struct Envirgrid {        // sizeof variable //TODO: further could be replaced
    short int elevation;  // 2	---> for memory consumptiom optimization use: signed short int (max 32767), therefore precision only 10 cm max +/-3.2767 km
                          // elevation range (8 -> 2 bytes)
    // unsigned long int Treedensityvalue;  // 2	---> values 0-1, but depending on setting could be also 5-10 -> max 65535 factor 10000 allows values between 0
    double Treedensityvalue;  // 2	---> values 0-1, but depending on setting could be also 5-10 -> max 65535 factor 10000 allows values between 0
                                          // and 65.535 with precision of 1/10000 which is sufficient
    unsigned short int Treenumber;        // 2	---> only for ouput; should in all cases below 65535
    unsigned int maxtreeheight;        // 2	---> only for ouput; should in all cases below 65535
    unsigned short maxthawing_depth;      // 2
	unsigned short litterheight0;			// 2	 
	unsigned short litterheight1;			// 2 
	unsigned short litterheight2;			// 2
	unsigned short litterheight3;			// 2
	unsigned short litterheight4;			// 2  
	unsigned short litterheight5;			// 2 
	unsigned short litterheight6; 			// 2
	unsigned short litterheight7;			// 2
	unsigned short litterheight8;			// 2
	unsigned short litterheight9;			// 2 
	unsigned short litterheightmean;		// 2	-> not in use right now std::array<unsigned short, 10> litterheight;						// 2*10	-> not in use right now
    unsigned short int envirgrowthimpact;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 1, precision of  *10000 possible, so 1/10000
    unsigned short int envirfireimpact;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 1, precision of  *10000 possible, so 1/10000
                                           // units precision (8 -> 2 bytes)
	double fire;	//###FIRE###
	unsigned short int firecells;	// for writing output
	double fireintensitymax;	// for writing output
	double fireintensitymean;	// for writing output

    unsigned short int soilhumidity;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 100, precision of 0.02, so 1/100
    unsigned short int twi;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 100, precision of 0.02, so 1/100

	double snowdepth;

    // constructor
    Envirgrid(short int elevation = 0,
              // unsigned long int Treedensityvalue = 0,
              double Treedensityvalue = 0,
              unsigned short int Treenumber = 0,
			  unsigned int maxtreeheight = 0,
              unsigned short maxthawing_depth = 100*10,
			  unsigned short litterheight0 = 1000,					// 2 
			  unsigned short litterheight1 = 1000,					// 2	
			  unsigned short litterheight2 = 1000,					// 2 
			  unsigned short litterheight3 = 1000,					// 2
			  unsigned short litterheight4 = 1000,					// 2 
			  unsigned short litterheight5 = 1000,					// 2
			  unsigned short litterheight6 = 1000, 					// 2
			  unsigned short litterheight7 = 1000,					// 2 
			  unsigned short litterheight8 = 1000,					// 2
			  unsigned short litterheight9 = 1000,					// 2 
			  unsigned short litterheightmean = 1000,				// 2
              unsigned short int envirgrowthimpact = 1,
              unsigned short int soilhumidity = 30*100,
              unsigned short int twi = 6.25*100,
			  
			  unsigned short int envirfireimpact = 0,
			  double fire = 0.0,
			  unsigned short int firecells = 0,
			  double fireintensitymax = 0,
			  double fireintensitymean = 0,
			  double snowdepth = 0)	
			  
        : elevation(elevation),
          Treedensityvalue(Treedensityvalue),
          Treenumber(Treenumber),
          maxtreeheight(maxtreeheight),
          maxthawing_depth(maxthawing_depth),
          litterheight0(litterheight0),
          litterheight1(litterheight1),
          litterheight2(litterheight2),
          litterheight3(litterheight3),
          litterheight4(litterheight4),
          litterheight5(litterheight5),
          litterheight6(litterheight6),
          litterheight7(litterheight7),
          litterheight8(litterheight8),
          litterheight9(litterheight9),
          litterheightmean(litterheightmean),
          envirgrowthimpact(envirgrowthimpact),
		  soilhumidity(soilhumidity),
		  twi(twi),
          envirfireimpact(envirfireimpact),
		  fire(fire),
		  firecells(firecells),
          fireintensitymax(fireintensitymax),
		  fireintensitymean(fireintensitymean),
		  snowdepth(snowdepth) {}
};

struct Parameter {
    // output
    bool parameterinputvis;
    bool yearlyvis;
    bool qualiyearlyvis;
    bool seedwinddispersalvis;
    bool mortvis;
    bool dataoutput;
    int outputmode;
    int parameteri;
    int outputall;

    // time steps and repeats
    int lastyearweatherdata;
    int simduration;
    int startatyear;
    int stopatyear;
    int startjahr;
    int runs;
    int repeati;
    unsigned int ivortmax;
    unsigned int ivort;
    bool stabilperiod;
    double stabilmovingwindow;
    double stabilpercentchangethreshold;
    int resetyear;
    bool spinupphase;

    // submodules
    bool thawing_depth;
    bool litterlayer;
    bool demlandscape;
    int demresolution;
    int specpres;
	unsigned short int roi;
    unsigned short int species_max;
	int firemode;
	int globalfireradius;
	double firelocdistance;
	double threshold_mild;
	double threshold_medium;
	double threshold_severe;
	double fireintensitymode;
	int firegapoutput;
	double firegapoutput_threshold;
	int firegapoutput_years;
	int counter_fire_happened;
	bool allow_pest_disturbances;
	double pest_disturbances_impactfactor;
	unsigned short int pest_species_max;

    // multi-core processing with omp
    int omp_num_threads;

    // simulated plots
    int mapxlength;
    int mapylength;
    bool weathercalcgradient;
    bool lineartransect;
    unsigned int n_weather_along_grid;
    double locationshift;
    double nposmax;
    double nposmin;

    double elevationoffset;
    double slopetwiratio;
    double temperaturelapse_jan;
    double temperaturelapse_jul;
    double precipitationlapse_year;

    // stabilization period
    bool starter;
    bool seedintro;

    // initialization
    bool realseedconnect;
    int yearswithseedintro;
    bool seedintropermanent;
    int seedwinddispersalmode;
    int seedintronumber;
    int seedintronumberpermanent;
    unsigned int seedintro_miny;
    unsigned int seedintro_maxy;
    unsigned int seedintro_minx;
    unsigned int seedintro_maxx;
    bool seedtravelbetween;
    int starttrees;
    int hinterland_maxlength;

    // climate processing
    long long int weatherchoice;
	double plotcentre_lat;
	double plotcentre_lon;
	double plotcentre_lat_start;
	double plotcentre_lon_start;
	double plotcentre_lat_end;
	double plotcentre_lon_end;
    int precweather;
    double precthreshold;
    double tempjandiffort;
    double tempjandiffortmin;
    double tempjuldiffort;
    double tempjuldiffortmin;
    double tempdiffort;
    double tempdiffortmin;
    double precdiffort;
    double precdiffortmin;
    double evapod;
    double janthresholdtempgmel;
    double weathervariablegmela;
    double weathervariablegmelb;
    double weathervariablegmelc;

    // environmental grid
    int sizemagnif;

    // tree growth
    double gdbasalfacqgmel;
    double gdbasalfacgmel;
    double gdbasalconstgmel;
    double gdbasalfacqsib;
    double gdbasalfacsib;
    double gdbasalconstsib;
    double gdbreastfacqgmel;
    double gdbreastfacgmel;
    double gdbreastconstgmel;
    double gdbreastfacqsib;
    double gdbreastfacsib;
    double gdbreastconstsib;
    double basalinfluenceoldyoung;
    int relgrowthinfluence;
    int allometryfunctiontype;
    double dbasalheightalloslope;
    double dbasalheightalloexp;
    double dbreastheightalloslope;
    double dbreastheightalloexp;
    double dbasalheightslopenonlin;
    double dbreastheightslopenonlin;
	double envirgrowthimpacttree_mod;

    // density evaluation
    int densitymode;
    double incfac;
    int calcinfarea;
    double densityvaluemanipulatorexp;
    double densitysmallweighing;
    double densitytreetile;
    int densitytiletree;
    double desitymaxreduction;
    int dichtheightrel;
    double densityvaluedbasalinfluence;
    double densityvaluemaximumatheight;

    // seed production, dispersal and pollination
    int coneage;
    double seedprodfactor;
    double seedflightrate;
    int dispersalmode;
    double seedtravelbreezeg;
    double seedtravelbreezes;
    double seeddescentg;
    double seeddescents;
    double distanceratio;
    int windsource;
    int boundaryconditions;
    double pollendirectionvariance;
    int pollination;
    double pollenfall;
    double pollengregorym, pollengregoryc;

    // germination
    double germinationrate;
    double germinatioweatherinfluence;

    // mortality
    double mortbg;
    int maximumage;
    double mortyouth;
    double mortyouthinfluenceexp;
    double mgrowth;
    double relgrowthmortinfluenceexp;
    double mweather;
    double heightweathermorteinflussexp;
    double mdensity;
    double mortage;
    double mdrought;
    double seedconemort;
    double seedfloormort;
    int gmelseedmaxage;

	// fire
	double seedfiremort; // seed mortality fire-related
	double litterlayerburn_mod;
	double fireimpactareasize_mod;
	
    // ancestry
    int nameakt;
    int lineakt;

    bool globalstoprepeat;

	double sapl_mort_factor; // calibration
	
	short int maxeleinput;
	short int mineleinput;
	short int maxele;
	short int minele;
	
	bool snowcomputation;
};

struct Speciestraits {
	int	number;
	string	species;
	int	roi;
	double	coneage ;
	double	seedflightrate;
	double	seedtravelbreeze;
	double	seeddescent;
	double	distanceratio;
	double	seedprodfactor;
	double	germinationrate;
	double	germinationweatherinfluence;
	double  mindiametergrowth;
	double  meangrowthq75p;						
	double	gdbasalfacq;
	double	gdbasalfac;
	double	gdbasalconst;
	double	gdbreastfacq;
	double	gdbreastfac;
	double	gdbreastconst;
	double	dbasalheightalloslope;
	double	dbasalheightalloexp;
	double	dbreastheightalloslope;
	double	dbreastheightalloexp;
	double	dbasalheightslopenonlin;
	double	dbreastheightslopenonlin;
	double	heightloga;
	double	heightlogb;
	double	heightlogc;
	double	mortbg;
	double	maximumage;
	double	mortage;
	double	mortyouth;
	double	mortyouthinfluenceexp;
	double	mgrowth;
	double	mdensity;
	double	densityvaluemaximumatheight;
	double	mweather;
	double	heightweathermorteinflussexp;
	double	mwindthrow;
	double	mdrought;
	double	seedconemort;
	double	seedfloormort;
	double	seedmaxage;
	double	janthresholdtemp;	
	double	janthresholdtempcalcvalue;
	double	weathervariablea;
	double	weathervariableb;
	double	weathervariablec;
	double	weathervariabled;
	std::array<double, 100> yearlycalcofanstiegweathermort;
	std::array<double, 100> yearlycalcofanstiegweathermortmin;
	double  minactivelayer;	
	double  minsoilwater;	
	double  maxsoilwater;	
	double  rootingdepth;
	double  relbarkthickness;
	double  resprouting;
	double  biomassleafbase;
	double  biomassleaffaca;
	double  biomassleaffacb;
	double  biomasswoodbase;
	double  biomasswoodfaca;
	double  biomasswoodfacb;
	int  lightdemand;
	double biomasswoodongree;
	double growthmod;
};

struct Speciescolonizationtimes { // for each aktort
	int	location;
	std::array<int,99> speciestimes;
	// int	sp01t;
	// int	sp02t;
	// int	sp03t;
	// int	sp04t;
	// int	sp05t;
	// int	sp06t;
	// int	sp07t;
	// int	sp08t;
	// int	sp09t;
	// int	sp10t;
	// int	sp11t;
	// int	sp12t;
	// int	sp13t;
	// int	sp14t;
	// int	sp15t;
	// int	sp16t;
	// int	sp17t;
	// int	sp18t;
	// int	sp19t;
	// int	sp20t;
	// int	sp21t;
	// int	sp22t;
};

struct Weather {
    std::array<int, 100> xworldcoo;
    std::array<int, 100> yworldcoo;
    int jahr;
    std::array<double, 100> temp1monthmean;
    std::array<double, 100> temp1monthmeanmin;
    std::array<double, 100> temp1monthmeaniso;
    std::array<double, 100> temp1monthmeanisomin;
    std::array<double, 100> temp2monthmean;
    std::array<double, 100> temp3monthmean;
    std::array<double, 100> temp4monthmean;
    std::array<double, 100> temp5monthmean;
    std::array<double, 100> temp6monthmean;
    std::array<double, 100> temp7monthmean;
    std::array<double, 100> temp7monthmeanmin;
    std::array<double, 100> temp7monthmeaniso;
    std::array<double, 100> temp7monthmeanisomin;
    std::array<double, 100> temp8monthmean;
    std::array<double, 100> temp9monthmean;
    std::array<double, 100> temp10monthmean;
    std::array<double, 100> temp11monthmean;
    std::array<double, 100> temp12monthmean;
    std::array<double, 100> tempyearmean;
    std::array<double, 100> tempyearmeanmin;
    std::array<double, 100> prec1monthmean;
    std::array<double, 100> prec2monthmean;
    std::array<double, 100> prec3monthmean;
    std::array<double, 100> prec4monthmean;
    std::array<double, 100> prec5monthmean;
    std::array<double, 100> prec6monthmean;
    std::array<double, 100> prec7monthmean;
    std::array<double, 100> prec8monthmean;
    std::array<double, 100> prec9monthmean;
    std::array<double, 100> prec10monthmean;
    std::array<double, 100> prec11monthmean;
    std::array<double, 100> prec12monthmean;
    std::array<double, 100> prec1monthmeanmin;
    std::array<double, 100> prec2monthmeanmin;
    std::array<double, 100> prec3monthmeanmin;
    std::array<double, 100> prec4monthmeanmin;
    std::array<double, 100> prec5monthmeanmin;
    std::array<double, 100> prec6monthmeanmin;
    std::array<double, 100> prec7monthmeanmin;
    std::array<double, 100> prec8monthmeanmin;
    std::array<double, 100> prec9monthmeanmin;
    std::array<double, 100> prec10monthmeanmin;
    std::array<double, 100> prec11monthmeanmin;
    std::array<double, 100> prec12monthmeanmin;
    std::array<int, 100> vegetationperiodlength;
    std::array<int, 100> vegetationperiodlengthmin;
    std::array<int, 100> vegetationperiodlengthiso;
    std::array<int, 100> vegetationperiodlengthisomin;
    std::array<double, 100> activeairtemp;
    std::array<double, 100> activeairtempmin;
    std::array<double, 100> degreday;
    std::array<double, 100> degredaymin;
    std::array<double, 100> precipitationsum;
    std::array<double, 100> precipitationsummin;
    std::array<std::array<double,22>, 100> weatherfactor;	// need to change to array with fixed length - check for species=N=22 and Pest=N=X or Days=N=365
    std::array<std::array<double,22>, 100> weatherfactormin;	
    // double weatherfactorg;	
    // double weatherfactorming;		
    // double weatherfactors;	
    // double weatherfactormins;	
    std::array<double, 100> droughtmort;
    std::array<double, 100> droughtmortmin;
    std::array<std::array<double,22>, 100> janisothermrestriktion;
    std::array<std::array<double,22>, 100> janisothermrestriktionmin;
    // double janisothermrestriktions;
    // double janisothermrestriktionsmin;
    // double janisothermrestriktiong;
    // double janisothermrestriktiongmin;
    std::array<double, 100> julisothermrestriktion;
    std::array<double, 100> julisothermrestriktionmin;
    std::array<double, 100> nddrestriktion;
    std::array<double, 100> nddrestriktionmin;
    std::array<double, 100> janmorttemp;
	std::array<double, 100> fireindex1;
	std::array<double, 100> fireindex2;
	std::array<double, 100> fireindex3;
	std::array<double, 100> fireindex4;
	std::array<double, 100> fireindex5;
	std::array<double, 100> fireindex6;
	std::array<double, 100> fireindex7;
	std::array<double, 100> fireindex8;
	std::array<double, 100> fireindex9;
	std::array<double, 100> fireindex10;
	std::array<double, 100> fireindex11;
	std::array<double, 100> fireindex12;
	std::array<double, 100> FPR;
	std::array<double, 100> tempmeanjja;	
	std::array<double, 100> precipitationsumjja;
	// vector<double> pestoutbreakprobability;					// ?? CHECK
	std::array<std::array<double,10>, 100> pestoutbreakprobability;					// ?? CHECK
	// vector<double> dailytemp;	
	std::array<std::array<double,365>, 100> dailytemp;
    // vector<double> dailytempmin;	
	std::array<std::array<double,365>, 100> dailytempmin;
	std::array<double, 100> snow_pool_lastday;
	std::array<double, 100> snow_pool_lastdaymin;	
	std::array<double, 100> snow_depth_winterspring_max;	
	std::array<double, 100> snow_depth_winterspring_maxmin;		
	std::array<double, 100> snow_depth_fallwinter_max;		
	std::array<double, 100> snow_depth_fallwinter_maxmin;
	std::array<double, 100> snow_max_winterdepth;
	std::array<double, 100> snow_max_winterdepthmin;	
	std::array<double, 100> snow_off_dayofyear;
	std::array<double, 100> snow_off_dayofyearmin;
	std::array<double, 100> snow_free_period;
	std::array<double, 100> snow_free_periodmin;
};

struct Pesttraits {
	unsigned short int pestspecies;
	string pestspeciesname;
	
	vector<double> weathervalsteps;
	
	vector<double> jan_temp_resp;
	vector<double> feb_temp_resp;
	vector<double> mar_temp_resp;
	vector<double> apr_temp_resp;
	vector<double> may_temp_resp;
	vector<double> jun_temp_resp;
	vector<double> jul_temp_resp;
	vector<double> aug_temp_resp;
	vector<double> sep_temp_resp;
	vector<double> oct_temp_resp;
	vector<double> nov_temp_resp;
	vector<double> dec_temp_resp;
	
	vector<double> jan_prec_resp;
	vector<double> feb_prec_resp;
	vector<double> mar_prec_resp;
	vector<double> apr_prec_resp;
	vector<double> may_prec_resp;
	vector<double> jun_prec_resp;
	vector<double> jul_prec_resp;
	vector<double> aug_prec_resp;
	vector<double> sep_prec_resp;
	vector<double> oct_prec_resp;
	vector<double> nov_prec_resp;
	vector<double> dec_prec_resp;
};

struct Evaluation {
    int xworldcoo;
    int yworldcoo;
    vector<double> basalarealist;
    vector<double> basalarearunmeanlist;
    double maxincrementbasalarea;
    int countermaxincrementbasalarea;
    int yearofturningpoint;
    int yearofequilibrium;
    bool postyearofturningpoint;
    vector<int> nheight0b40liste;
    vector<double> nheight0b40runmeanliste;
    vector<int> nheight41b200liste;
    vector<double> nheight41b200runmeanliste;
    vector<int> nheight201b10000liste;
    vector<double> nheight201b10000runmeanliste;
    vector<double> meanbreastdiameterliste;
    vector<double> meanbreastdiameterrunmeanliste;
    vector<int> stemcountliste;
    vector<int> stemcountrunmeanliste;
    vector<double> meantreeheightliste;
    vector<double> meantreeheightrunmeanliste;
    vector<double> meantreeageliste;
    vector<double> meantreeagerunmeanliste;
};

struct GlobalFireParameter {
	std::array<int, 100>	number;
	std::array<double, 100>	distance;
	std::array<double, 100>	latitude;
	std::array<double, 100>	longitude;
	std::array<double, 100>	parameter1;
	std::array<double, 100>	parameter2;
	std::array<double, 100>	parameter3;
	std::array<double, 100>	parameter4;
	std::array<double, 100>	parameter5;
	std::array<double, 100>	parameter6;
	std::array<double, 100>	parameter7;
	std::array<double, 100>	parameter8;
	std::array<double, 100>	parameter9;
	std::array<double, 100>	threshold_mild;
	std::array<double, 100>	threshold_medium;
	std::array<double, 100>	threshold_severe;
};

