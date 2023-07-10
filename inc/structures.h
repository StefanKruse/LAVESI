#pragma once

#include <vector>

using namespace std;

struct Tree {                       // sizeof variable
    double dbasal;                  // 8->4	---> in cm, 0 to few meters -> unsigned int 4294967295 /10000 precision
    double dbreast;                 // 8->4	---> in cm, 0 to meters -> unsigned int 4294967295 /10000 precision
    double densitywert;             // 8->2
    double thawing_depthinfluence;  // 8->2
    unsigned int xcoo;  // 4 	---> unsigned int 4294967295 /1000 precision 
    unsigned int ycoo;  // 4
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
    bool cone;                              // 1	---> was int
    bool longdispersed;                     // 1
    bool growing;                           // 1
	unsigned short firedamage;				// 2
    unsigned short int crownstart;          // 2	in cm; max 65 m -> unsigned short int /10 still has 0.1 cm precision
    unsigned short int relcrowndamage;      // 2	relative; 0-1 -> precision /1000 lead to 0.001 precision
};

struct Seed {  // sizeof variable
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
	int firemort;			   // for fire mortality
};

struct Envirgrid {        // sizeof variable
    short int elevation;  // 2	---> for memory consumptiom optimization use: signed short int (max 32767), therefore precision only 10 cm max +/-3.2767 km
                          // elevation range (8 -> 2 bytes)
    unsigned short int Treedensityvalue;  // 2	---> values 0-1, but depending on setting could be also 5-10 -> max 65535 factor 10000 allows values between 0
                                          // and 65.535 with precision of 1/10000 which is sufficient
    unsigned short int Treenumber;        // 2	---> only for ouput; should in all cases below 65535
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
	double fire;
	unsigned short int firecells;	// for writing output
	double fireintensitymax;	// for writing output
	double fireintensitymean;	// for writing output

    unsigned short int soilhumidity;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 100, precision of 0.02, so 1/100
    unsigned short int twi;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 100, precision of 0.02, so 1/100


    // constructor
    Envirgrid(short int elevation = 0,
              unsigned short int Treedensityvalue = 0,
              unsigned short int Treenumber = 0,

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
			  double fireintensitymean = 0)	
			  
        : elevation(elevation),
          Treedensityvalue(Treedensityvalue),
          Treenumber(Treenumber),
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
		  fireintensitymean(fireintensitymean) {}
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
    unsigned short int species_max;
	int firemode;
	double fireintensitymode;
	int firegapoutput;
	double firegapoutput_threshold;
	int firegapoutput_years;
	int counter_fire_happened;

    // multi-core processing with omp
    int omp_num_threads;

    // simulated plots
    int mapxlength;
    int mapylength;
    bool weathercalcgradient;
    bool lineartransect;
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
    unsigned int seedintro_maxy;
    unsigned int seedintro_maxx;
    bool seedtravelbetween;
    int starttrees;
    int hinterland_maxlength;

    // climate processing
    int weatherchoice;
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
	double seedfiremort; // seed mortality fire-related
    int gmelseedmaxage;

    // ancestry
    int nameakt;
    int lineakt;
};

struct Speciestraits {
	int	number;
	string	species;
	double	coneage ;
	double	seedflightrate;
	double	seedtravelbreeze;
	double	seeddescent;
	double	distanceratio;
	double	seedprodfactor;
	double	germinationrate;
	double	germinationweatherinfluence;
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
	double  yearlycalcofanstiegweathermort;
	double  yearlycalcofanstiegweathermortmin;
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
};

struct Weather {
    int xworldcoo;
    int yworldcoo;
    int jahr;
    double temp1monthmean;
    double temp1monthmeanmin;
    double temp1monthmeaniso;
    double temp1monthmeanisomin;
    double temp2monthmean;
    double temp3monthmean;
    double temp4monthmean;
    double temp5monthmean;
    double temp6monthmean;
    double temp7monthmean;
    double temp7monthmeanmin;
    double temp7monthmeaniso;
    double temp7monthmeanisomin;
    double temp8monthmean;
    double temp9monthmean;
    double temp10monthmean;
    double temp11monthmean;
    double temp12monthmean;
    double tempyearmean;
    double tempyearmeanmin;
    double prec1monthmean;
    double prec2monthmean;
    double prec3monthmean;
    double prec4monthmean;
    double prec5monthmean;
    double prec6monthmean;
    double prec7monthmean;
    double prec8monthmean;
    double prec9monthmean;
    double prec10monthmean;
    double prec11monthmean;
    double prec12monthmean;
    double prec1monthmeanmin;
    double prec2monthmeanmin;
    double prec3monthmeanmin;
    double prec4monthmeanmin;
    double prec5monthmeanmin;
    double prec6monthmeanmin;
    double prec7monthmeanmin;
    double prec8monthmeanmin;
    double prec9monthmeanmin;
    double prec10monthmeanmin;
    double prec11monthmeanmin;
    double prec12monthmeanmin;
    int vegetationperiodlength;
    int vegetationperiodlengthmin;
    int vegetationperiodlengthiso;
    int vegetationperiodlengthisomin;
    double activeairtemp;
    double activeairtempmin;
    double degreday;
    double degredaymin;
    double precipitationsum;
    double precipitationsummin;
    vector<double> weatherfactor;
    vector<double> weatherfactormin;
    // double weatherfactorg;
    // double weatherfactorming;
    // double weatherfactors;
    // double weatherfactormins;
    double droughtmort;
    double droughtmortmin;
    vector<double> janisothermrestriktion;
    vector<double> janisothermrestriktionmin;
    // double janisothermrestriktions;
    // double janisothermrestriktionsmin;
    // double janisothermrestriktiong;
    // double janisothermrestriktiongmin;
    double julisothermrestriktion;
    double julisothermrestriktionmin;
    double nddrestriktion;
    double nddrestriktionmin;
    double janmorttemp;
	double fireindex1;
	double fireindex2;
	double fireindex3;
	double fireindex4;
	double fireindex5;
	double fireindex6;
	double fireindex7;
	double fireindex8;
	double fireindex9;
	double fireindex10;
	double fireindex11;
	double fireindex12;
	double FPR;
	double tempmeanjja;
	double precipitationsumjja;
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
