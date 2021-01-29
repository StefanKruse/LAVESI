#pragma once

#include <vector>

using namespace std;

struct Tree {                       // sizeof variable //TODO: further could be replaced
    double dbasal;                  // 8->4	---> in cm, 0 to few meters -> unsigned int 4294967295 /10000 precision; could be replaced
    double dbreast;                 // 8->4	---> in cm, 0 to meters -> unsigned int 4294967295 /10000 precision; could be replaced
    double densitywert;             // 8->2	---> need to check range of values for replacment
    double thawing_depthinfluence;  // 8->2	---> need to check range of values for replacment
    unsigned int xcoo;  // 4	---> was double ---> only positive, -> unsigned int 4294967295 /1000 precision => mm which is sufficient and allows still 4294
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
    unsigned short int height;              // 2	---> was double; max 100 m -> unsigned short int /100 max 65 m and still has 0.01 cm precision
    unsigned short int coneheight;          // 2	---> was double; max 100 m -> unsigned short int /100 max 65 m and still has 0.01 cm precision
    unsigned short int age;                 // 2 	---> was int
    unsigned short int seednewly_produced;  // 2 	---> was int
    signed short int species;               // 2 	---> was int
    unsigned short int envirimpact;         // 2
    short int elevation;                    // 2
    bool cone;                              // 1	---> was int; could be further replaced by single use of coneheight
    bool longdispersed;                     // 1
    bool growing;                           // 1
	unsigned short int firedamage;			// 2
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
        releaseheight;         // 2	---> was double -> unsigned short in then /100 in cm 0 to 10000 so 65535 as max would be sufficient for trees up to 65m
    unsigned short int age;    // 2	---> was int
    signed short int species;  // 2	---> was int
    bool incone;               // 1
    bool longdispersed;        // 1
    bool dead = false;         // 1
};

struct Envirgrid {        // sizeof variable //TODO: further could be replaced
    short int elevation;  // 2	---> for memory consumptiom optimization use: signed short int (max 32767), therefore precision only 10 cm max +/-3.2767 km
                          // elevation range (8 -> 2 bytes)
    unsigned short int Treedensityvalue;  // 2	---> values 0-1, but depending on setting could be also 5-10 -> max 65535 factor 10000 allows values between 0
                                          // and 65.535 with precision of 1/10000 which is sufficient
    unsigned short int Treenumber;        // 2	---> only for ouput; should in all cases below 65535
    unsigned short maxthawing_depth;      // 2
                                          // unsigned short litterheight;						// 2	-> not in use right now; TODO: replace
                                          // litterheight with a more efficient
    // way unsigned short litterheight0;					// 2	-> not in use right now unsigned short litterheight1;
    // // 2	-> not in use right now unsigned short litterheight2;					// 2	-> not in use right now unsigned short
    // litterheight3;					// 2	-> not in use right now unsigned short litterheight4;					// 2
    // -> not in use right now unsigned short litterheight5;					// 2	-> not in use right now unsigned short litterheight6;
    // // 2	-> not in use right now unsigned short litterheight7;					// 2	-> not in use right now unsigned short
    // litterheight8;					// 2	-> not in use right now unsigned short litterheight9;					// 2
    // -> not in use right now unsigned short litterheightmean;					// 2	-> not in use right now std::array<unsigned short, 10>
    // litterheight;		// 2*10	-> not in use right now
    unsigned short int envirgrowthimpact;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 1, precision of  *10000 possible, so 1/10000
    unsigned short int envirfireimpact;  // 2	---> use of unsigned short int (max=32767), as only between 0 and 1, precision of  *10000 possible, so 1/10000
                                           // units precision (8 -> 2 bytes)
	//bool fire; //###FIRE### bool variant
	unsigned short int fire;	//###FIRE### changed int variant
	
    // constructor
    Envirgrid(short int elevation = 0,
              unsigned short int Treedensityvalue = 0,
              unsigned short int Treenumber = 0,
              unsigned short maxthawing_depth = 1000,
              unsigned short int envirgrowthimpact = 1,
              unsigned short int envirfireimpact = 0,
			  //bool fire = false)	// ###FIRE### bool variant
			  unsigned short int fire = 0)	//###FIRE### int variant
        : elevation(elevation),
          Treedensityvalue(Treedensityvalue),
          Treenumber(Treenumber),
          maxthawing_depth(maxthawing_depth),
          envirgrowthimpact(envirgrowthimpact),
          envirfireimpact(envirfireimpact),
		  fire(fire) {}
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
    bool vegetation;
    bool demlandscape;
    int demresolution;
    int specpres;

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
    int gmelseedmaxage;

    // ancestry
    int nameakt;
    int lineakt;
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
    double degreday_sqrt;
    double degredaymin;
    double precipitationsum;
    double precipitationsummin;
    double weatherfactorg;
    double weatherfactorming;
    double weatherfactors;
    double weatherfactormins;
    double droughtmort;
    double droughtmortmin;
    double janisothermrestriktions;
    double janisothermrestriktionsmin;
    double janisothermrestriktiong;
    double janisothermrestriktiongmin;
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
