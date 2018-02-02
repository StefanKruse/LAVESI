using namespace std;

// Container for the handled objects (seeds/trees/density grid/parameter/weather/output)
	
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
		int		speicher;
		double	densitywert;
		double thawing_depthinfluence;
		bool	longdispersed;					
		double	entfernung;						
		bool    growing;
		int 	species;
	};

	struct seed 
	{
		int		xworldcoo;
		int		yworldcoo;
		double	xcoo;
		double	ycoo;
		int		namem;
		int		namep;
		int		line;
		int		generation;
		bool	imcone;
		double	gewicht;
		int		age;
		bool	longdispersed;
		double 	entfernung;
		int		species;
		double 	elternheight;
		double 	maxgrowth;
		double 	pollenfall;
		double 	descent; 
		double 	thawing_depthinfluence;
	};
	
	struct Karten 
	{
		int		xworldcoo;
		int		yworldcoo;
		double	xcoo;
		double	ycoo;
		double	Treedensitywert;
		int		Treeanzahl;
		unsigned short maxthawing_depth;
		unsigned short auflagenstaerke;	
		unsigned short auflagenstaerke0;
		unsigned short auflagenstaerke1;
		unsigned short auflagenstaerke2;
		unsigned short auflagenstaerke3;
		unsigned short auflagenstaerke4;
		unsigned short auflagenstaerke5;
		unsigned short auflagenstaerke6;
		unsigned short auflagenstaerke7;
		unsigned short auflagenstaerke8;
		unsigned short auflagenstaerke9;
		unsigned short auflagenstaerkemittel;
	};

	struct Parameter
	{
		// general parameters
		// ... output
		bool	parameterinputvis;
		bool	yearlyvis;
		bool	qualiyearlyvis;
		bool	seeddispvis;
		bool	mortvis;
		bool	dataoutput;
		int 	outputmode;
		int 	parameteri;
		int 	outputall;
		bool 	computationtimevis;

		// ... time steps and repeats
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
		bool	einschwingen;

		// submodules
		bool	thawing_depth;
		bool	vegetation;	
		int		specpres;

		// mulit-core processing with omp
		int		omp_num_threads;
		
		// simulated plots
		int		mapxlength;
		int		mapylength;	
		bool	weathercalcgradient;
		bool	lineartransect;
		double	nposmax;
		double	nposmin;
		
		// ... stabilization period
		bool	starter;
		bool	seedintro;	
		
		// initialization 
		bool	realseedconnect;
		int		yearswithseedintro;
		bool	seedintropermanent;
		int		seeddispmode;
		int		seedintronumber;
		int 	seedintronumberpermanent;
		bool	seedtravelbetween;
		int		starttrees;	
		
		// ... climate processing
		int		weatherchoice;
		int		precweather;
		double	precgrenzwert;
		double	tempdiffort;
		double	precdiffort;
		double	tempdiffortmin;
		double	precdiffortmin;
		double	evapod;
		double 	jantodestempgmel;
		double 	weatherzahlgmela;
		double 	weatherzahlgmelb;
		double 	weatherzahlgmelc;
		
		// ... density grid
		int		sizemagnif;

		// specific parameters for the submodules
		// ... tree growth
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
		double	basaleinflussaltneu;
		int		relwachstumeinfluss;
		int		allometriefunktionstyp;
		double	dbasalheightallosteig;
		double	dbasalheightalloexp;
		double	dbrustheightallosteig;
		double	dbrustheightalloexp;
		double	dbasalheightsteignonlin;
		double	dbrustheightsteignonlin;
		
		// ... density evaluation
		int		densityart;
		double	incfac;
		int 	calcinfarea;
		double 	densitywertmanipulatorexp;
		double 	densitysmallweighing;
		double 	densitytreetile;
		int 	densitytiletree;
		double 	desitymaxreduction;
		int 	dichtheightrel;
		double	densitywertdbasaleinfluss;
		double	densitywertmaximumbeiheight; 
		int		individuenzahl;
		
		// ... seed production, dispersal and pollination
		int		coneage;
		double	seedprodfaktor; 
		double	seedflugrate;
		int		dispersalmode; 
		double 	SeedTravelBreezeg;
		double 	SeedTravelBreezes;
		double 	SeedDescentg;
		double 	SeedDescents;
		double	distanceratio; 
		int 	windsource;
		int 	boundaryconditions;
		double 	pollendirectionvariance;
		int 	pollination;
		int 	pollenDistdensplot;
		double 	pollenfall;
		double 	pollengregorym,pollengregoryc;
		
		// ... germination		
		double	keimungsrate; 
		double	keimungweathereinfluss;
		
		// ... mortality
		double	mortbg;
		int		maximalage;
		double	mjung; 	
		double	jugendmorteinflussexp;
		double	mgrowth;
		double	relwachstummorteinflussexp;
		double	mweather;
		double	heightweathermorteinflussexp;
		double	mdensity;
		double	malt;
		double	mtrockenheit;
		double	seedTreemort;
		double	seedbodenmort;
		int 	gmel_seedtod;

		// ... ancestry
		int		nameakt;
		int		lineakt;
		double SNPMutationrate;
		double microsatelliteMutationrate;
		double rel_trait_variance;
	};

	struct weather 
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
		double	maxsteigungbasalarea;		
		int		countermaxsteigungbasalarea;
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
