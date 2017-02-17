//! Arrays fuer die verschiedenen Bestandteile auf der simulierten Flaeche (Baeume und Samen)
//! sowie fuer die Parameter (Modellvariablen/-parameter und Wetter)
/****************************************************************************************//**
 * \brief Arrays for the handled objects (seeds/trees/density grid/parameter/weather/output)
 *
 *
 *
 *
 *******************************************************************************************/

struct Baum 
{
	int		xweltcoo;						
	int		yweltcoo;						
	double	xcoo;							
	double	ycoo;							
	int		name;							
	int		namem;							
	int		namep;							
	int		linie;							
	int		generation;						
	int		etablierungsjahr;	
	int		sterbejahr;					
	double	dbasal;					
	double	dbasalrel;						 
	double	dbrust;							
	vector<double> Dbrustliste;				
	double	dbrustrel;						
	double	hoehe;							
	int		alter;							
	int		zapfen;							
	double	zapfenhoehe;					 
	int		samenneuproduziert;				
	int		samenproduziert;				
	double	dichtewert;						
	bool	longdispersed;					
	bool    growing;						

};

struct Samen 
{
	int		xweltcoo;						
	int		yweltcoo;						
	double	xcoo;							
	double	ycoo;							
	int		namem;							
	int		namep;							
	int		linie;							
	int		generation;						
	bool	imzapfen;						
	double	gewicht;		
	int		alter;			
	bool	longdispersed;					
};

struct Karten 
{
	int		xweltcoo;						
	int		yweltcoo;						
	double	xcoo;							
	double	ycoo;							
	double	baumdichtewert;					
	int		baumanzahl;						

};

struct Parameter
{
	// general parameters
	// ... output
	bool	parameterlesenanzeige;	
	bool	jahranzeige;			
	bool	qualijahranzeige;			
	bool	samenausbranzeige;		
	bool	mortanzeige;
	bool	datenausgabe;				
	int		ausgabemodus;
	
	// ... time steps and repeats
	int		simdauer;		
	int		startjahr;	
	int		runs; 		
	unsigned int ivortmax;	
	
	// simulated plots
	int		mapxlength;
	int		mapylength;
	bool	weathercalcgradient;
	bool	lineartransekt;
	double	Nposmax;
	double	Nposmin;
	
	// ... stabilization period
	bool	starter;			
	bool	seedintro;	
	bool	einschwingen;			
	int 	startlag; 			
	int		stabilfirstweatheryears;	
	bool 	stabilperiod;			
	double 	stabilmovingwindow;			
	double 	stabilpercentchangethreshold;
	int 	resetyear;			
	unsigned int ivort;	
	int		wiederholung;	
	
	// initialization 
	bool	realseedconnect;	
	int		jahremitsameneintrag; 	
	bool	seedintropermanent;
	int		seeddispmode;		
	int		etabbg;	
	int 	etabbgpermanent;
	bool	seedtravelbetween;	
	
	// ... climate processing
	int		wetterwahl;		
	int		precwetter;				
	double	precgrenzwert;		
	double	precindexstrength;	
	double	precindexexponent;	
	double	tempdiffort; 
	double	precdiffort; 
	double	tempdiffortmin; 
	double	precdiffortmin;	
	double	evapod;			
	
	// ... density grid
	int		sizemagnif;	

	// specific paramters for the submodules
	// ... tree growth
	double	gdbasal;
	double	gdbasalsapl;	
	double	gdbrust;
	double	basaleinflussaltneu;
	int		relwachstumeinfluss;
	int		allometriefunktionstyp;	
	double	dbasalhoeheallosteig;	
	double	dbasalhoehealloexp;	
	double	dbrusthoeheallosteig; 	
	double	dbrusthoehealloexp;	
	double	dbasalhoehesteignonlin;		
	double	dbrusthoehesteignonlin;		
	
	// ... climate influences
	double	aatnddratio;				
	
	// ... density evaluation
	int		dichteart; 					
	double	incfac;					
	int 	calcinfarea;				
	double 	dichtewertmanipulatorexp;
	double 	densitysmallweighing;		
	double 	densitytreetile;				
	int 	densitytiletree;					
	double 	desitymaxreduction;
	int 	dichtheightrel;
	double	dichtewertdbasaleinfluss; 
	double	dichtewertmaximumbeihoehe; 	
	int		individuenzahl; 				

	// ... seed production and dispersal
	int		zapfenalter; 				
	double	samenprodfaktor; 		
	double	samenflugrate;
	int		dispersalmode; 	
	double	entfernungsteiler; 		

	// ... germination
	double	keimungsrate; 	
	double	keimungwettereinfluss; 
	
	// ... mortality
	double	mortbg; 
	int		hoechstalter;
	double	mjung; 							
	double	jugendmorteinflussexp;	 		
	double	mgrowth;						
	double	relwachstummorteinflussexp;  
	double	mwetter; 				
	double	hoehewettermorteinflussexp;		
	double	mdichte; 						
	double	malt;
	double	mtrockenheit;
	double	samenbaummort;		 				 
	double	samenbodenmort;		 				
	
	// ... ancestry
	int		nameakt;		 					
	int		linieakt;
};

struct Wetter 
{
	int		xweltcoo;		 				
	int		yweltcoo;		 				
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
	
	double	maxbasalwachstumjahr; 		 	
	double	maxbasalwachstumjahrmin;		
	
	double	maxbasalsaplwachstumjahr;		 
	double	maxbasalsaplwachstumjahrmin;	
	
	double	maxbrustwachstumjahr; 			
	double	maxbrustwachstumjahrmin;		

	double	trockenheitsmort;				
};

struct Auswertung
{
	int		xweltcoo;
	int		yweltcoo;	
	vector<double> BAliste;	
	vector<double> BArunmeanliste;
	double	maxsteigungbasalarea;
	int		countermaxsteigungbasalarea;
	int		wendejahr;
	int		saettigungsjahr;
	bool	nachwendejahr;
	
	vector<int> nhoehe0b40liste;
	vector<double> nhoehe0b40runmeanliste;
	vector<int> nhoehe41b200liste;
	vector<double> nhoehe41b200runmeanliste;
	vector<int> nhoehe201b10000liste;
	vector<double> nhoehe201b10000runmeanliste;
	
	vector<double> meanbreastdiameterliste;
	vector<double> meanbreastdiameterrunmeanliste;
	
	vector<int> stemcountliste;
	vector<int> stemcountrunmeanliste;
	
	vector<double> meantreeheightliste;
	vector<double> meantreeheightrunmeanliste;
	vector<double> meantreeageliste;
	vector<double> meantreeagerunmeanliste;
	
};	

	
