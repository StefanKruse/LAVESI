using namespace std;
	
	struct Tree 
	{
		int		xworldcoo;
		int		yworldcoo;
		double	xcoo;	
		double	ycoo;	
		int		name;	
		int		namem;	
		int		namep;	
		int		line;	
		int		generation;
		int		yr_of_establishment;
		int		yr_of_dying;
		double	dbasal;	
		double	dbasalrel;	
		double	dbrust;	
		double	dbrustrel;
		double	height;
		int		age;
		int		cone;
		double	coneheight;
		int		seednewly_produced;
		int		seedproduced;
		int		buffer;
		double	densitywert;
		double 	thawing_depthinfluence;
		bool	longdispersed;					
		double	dispersaldistance;						
		bool    growing;
		int 	species;
	};

	struct Seed 
	{
		int		xworldcoo;
		int		yworldcoo;
		double	xcoo;
		double	ycoo;
		int		namem;
		int		namep;
		int		line;
		int		generation;
		bool	incone;
		double	weight;
		int		age;
		bool	longdispersed;
		double 	dispersaldistance;
		int		species;
		double 	releaseheight;
		double 	maxgrowth;
		double 	pollenfall;
		double 	descent; 
		double 	thawing_depthinfluence;
	};
	
	struct Envirgrid 
	{
		int		xworldcoo;
		int		yworldcoo;
		double	xcoo;
		double	ycoo;
		double	Treedensityvalue;
		int		Treenumber;
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
		unsigned short litterheightmean;
	};

	struct Parameter
	{
		// output
		bool	parameterinputvis;
		bool	yearlyvis;
		bool	qualiyearlyvis;
		bool	seedwinddispersalvis;
		bool	mortvis;
		bool	dataoutput;
		int 	outputmode;
		int 	parameteri;
		int 	outputall;
		bool 	computationtimevis;

		// time steps and repeats
		int		simduration;	
		int		startjahr;
		int		runs;
		int		repeati;
		unsigned int ivortmax;
		unsigned int ivort;
		bool 	stabilperiod;
		double 	stabilmovingwindow;	
		double 	stabilpercentchangethreshold;
		int 	resetyear;
		bool	spinupphase;

		// submodules
		bool	thawing_depth;
		bool	vegetation;	
		int		specpres;

		// multi-core processing with omp
		int		omp_num_threads;
		
		// simulated plots
		int		mapxlength;
		int		mapylength;	
		bool	weathercalcgradient;
		bool	lineartransect;
		double	nposmax;
		double	nposmin;
		
		// stabilization period
		bool	starter;
		bool	seedintro;	
		
		// initialization 
		bool	realseedconnect;
		int		yearswithseedintro;
		bool	seedintropermanent;
		int		seedwinddispersalmode;
		int		seedintronumber;
		int 	seedintronumberpermanent;
		bool	seedtravelbetween;
		int		starttrees;	
		
		// climate processing
		int		weatherchoice;
		int		precweather;
		double	precthreshold;
		double	tempdiffort;
		double	precdiffort;
		double	tempdiffortmin;
		double	precdiffortmin;
		double	evapod;
		double 	janthresholdtempgmel;
		double 	weathervariablegmela;
		double 	weathervariablegmelb;
		double 	weathervariablegmelc;
		
		// environmental grid
		int		sizemagnif;

		// tree growth
		double 	gdbasalfacqgmel;	
		double 	gdbasalfacgmel;			
		double 	gdbasalconstgmel;
		double 	gdbasalfacqsib;			
		double 	gdbasalfacsib;			
		double 	gdbasalconstsib;			
		double 	gdbrustfacqgmel;	
		double 	gdbrustfacgmel;			
		double 	gdbrustconstgmel;
		double 	gdbrustfacqsib;			
		double 	gdbrustfacsib;			
		double 	gdbrustconstsib;			
		double	basalinfluenceoldyoung;
		int		relgrowthinfluence;
		int		allometryfunctiontype;
		double	dbasalheightalloslope;
		double	dbasalheightalloexp;
		double	dbrustheightalloslope;
		double	dbrustheightalloexp;
		double	dbasalheightslopenonlin;
		double	dbrustheightslopenonlin;
		
		// ... density evaluation
		int		densitymode;
		double	incfac;
		int 	calcinfarea;
		double 	densityvaluemanipulatorexp;
		double 	densitysmallweighing;
		double 	densitytreetile;
		int 	densitytiletree;
		double 	desitymaxreduction;
		int 	dichtheightrel;
		double	densityvaluedbasalinfluence;
		double	densityvaluemaximumatheight; 
		int		individuenzahl;
		
		// ... seed production, dispersal and pollination
		int		coneage;
		double	seedprodfactor; 
		double	seedflightrate;
		int		dispersalmode; 
		double 	seedtravelbreezeg;
		double 	seedtravelbreezes;
		double 	seeddescentg;
		double 	seeddescents;
		double	distanceratio; 
		int 	windsource;
		int 	boundaryconditions;
		double 	pollendirectionvariance;
		int 	pollination;
		int 	pollenDistdensplot;
		double 	pollenfall;
		double 	pollengregorym,pollengregoryc;
		
		// ... germination		
		double	germinationrate; 
		double	germinatioweatherinfluence;
		
		// ... mortality
		double	mortbg;
		int		maximumage;
		double	mortyouth; 	
		double	mortyouthinfluenceexp;
		double	mgrowth;
		double	relgrowthmortinfluenceexp;
		double	mweather;
		double	heightweathermorteinflussexp;
		double	mdensity;
		double	mortage;
		double	mdrought;
		double	seedconemort;
		double	seedfloormort;
		int 	gmelseedmaxage;

		// ... ancestry
		int		nameakt;
		int		lineakt;
		double SNPMutationrate;
		double microsatelliteMutationrate;
		double rel_trait_variance;
	};

	struct Weather 
	{
		int		xworldcoo;		 			
		int		yworldcoo;		 			
		int		jahr; 		 				
		double	temp1monatmittel; 		 	
		double	temp1monatmitteliso; 		 
		double	temp1monatmittelmin; 		 
		double	temp1monatmittelisomin; 	
		double	temp2monatmittel; 		 	
		double	temp3monatmittel; 		 	
		double	temp4monatmittel; 		 	
		double	temp5monatmittel; 		 	
		double	temp6monatmittel; 		 	
		double	temp7monatmittel; 		 	
		double	temp7monatmitteliso; 		 
		double	temp7monatmittelmin; 		 
		double	temp7monatmittelisomin; 	
		double	temp8monatmittel; 		 	
		double	temp9monatmittel; 		 	
		double	temp10monatmittel; 		 	
		double	temp11monatmittel; 		 	
		double	temp12monatmittel; 		 	
		double	tempjahrmittel; 		 	
		double	tempjahrmittelmin; 		 	
		double	prec1monatmittel; 		 	
		double	prec2monatmittel; 		 	
		double	prec3monatmittel; 		 	
		double	prec4monatmittel; 		 	
		double	prec5monatmittel; 		 	
		double	prec6monatmittel; 		 	
		double	prec7monatmittel; 		 	
		double	prec8monatmittel; 		 	
		double	prec9monatmittel; 		 	
		double	prec10monatmittel; 		 	
		double	prec11monatmittel; 		 	
		double	prec12monatmittel;
		int		vegetationslaenge; 		 	
		int		vegetationslaengeiso; 		 
		int		vegetationslaengemin; 		 
		int		vegetationslaengeisomin;	 
		double	activeairtemp; 		 		
		double	activeairtempmin;		 	
		double	degreday;		 			
		double	niederschlagssumme; 		 
		double	niederschlagssummemin;		 
		double	weatherfactorg; 		 		
		double	weatherfactorming;			
		double	weatherfactors; 		 		
		double	weatherfactormins;					
		double	trockenheitsmort;			
		double 	janisothermrestriktions;
		double 	julisothermrestriktion;
		double 	nddrestriktion;
		double 	janmorttemp;
		double 	janisothermrestriktiong;
	};

	struct Evaluation
	{
		int		xworldcoo;					
		int		yworldcoo;					
		vector<double> BAliste;				
		vector<double> BArunmeanliste;		
		double	maxincrementbasalarea;		
		int		countermaxincrementbasalarea;
		int		wendejahr;					
		int		saettigungsjahr;			
		bool	nachwendejahr;					
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
