#pragma once

#include <vector>

using namespace std;

struct Tree {
    int xworldcoo;
    int yworldcoo;
    double xcoo;
    double ycoo;
    int name;
    int namem;
    int namep;
    int line;
    int generation;
    int yr_of_establishment;
    int yr_of_dying;
    double dbasal;
    double dbasalmax;
    double dbasalrel;
    double dbreast;
    double dbreastrel;
    double height;
    int age;
    int cone;
    double coneheight;
    int seednewly_produced;
    int seedproduced;
    int buffer;
    double densitywert;
    double thawing_depthinfluence;
    bool longdispersed;
    double dispersaldistance;
    bool growing;
    int species;
};

struct Seed {
    int xworldcoo;
    int yworldcoo;
    double xcoo;
    double ycoo;
    int namem;
    int namep;
    int line;
    int generation;
    bool incone;
    double weight;
    int age;
    bool longdispersed;
    double dispersaldistance;
    int species;
    double releaseheight;
    double maxgrowth;
    double pollenfall;
    double descent;
    double thawing_depthinfluence;
    bool dead = false;
};

struct Envirgrid {
    int xworldcoo;
    int yworldcoo;
    double xcoo;
    double ycoo;
    double Treedensityvalue;
    int Treenumber;
    unsigned short maxthawing_depth;
    unsigned short litterheight;
    unsigned short litterheight0;
    unsigned short litterheight1;
    unsigned short litterheight2;
    unsigned short litterheight3;
    unsigned short litterheight4;
    unsigned short litterheight5;
    unsigned short litterheight6;
    unsigned short litterheight7;
    unsigned short litterheight8;
    unsigned short litterheight9;
    // TODO std::array<unsigned short, 10> litterheight;
    unsigned short litterheightmean;
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
	bool prmz_autoimprove;
	
    // submodules
    bool thawing_depth;
    bool vegetation;
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
    bool seedtravelbetween;
    int starttrees;
    int hinterland_maxlength;

    // climate processing
    int weatherchoice;
    int precweather;
    double precthreshold;
    double tempjandiffort;
    double tempjuldiffort;
    double tempdiffort;
    double precdiffort;
    double tempjandiffortmin;
    double tempjuldiffortmin;
    double tempdiffortmin;
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
    double degreday;
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
