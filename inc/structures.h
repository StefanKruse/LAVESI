//! Arrays fuer die verschiedenen Bestandteile auf der simulierten Flaeche (trees und seed)
//! sowie fuer die Parameter (Modellvariablen/-parameter und weather)
	
	struct Tree 
	{
		
		
		 double maxgrowth;
		 
		 double pollenfall;
		 
		 double descent;

		int		xworldcoo;						//!<
		int		yworldcoo;						//!<
		double	xcoo;							//!<
		double	ycoo;							//!<
		int		name;							//!<
		int		namem;							//!<
		int		namep;							//!<
		int		line;							//!<
		int		generation;						//!<
		int		yr_of_establishment;			//!< Etabliert im Jahr X
		int		yr_of_dying;					//!< gestorben im Jahr X
		double	dbasal;							//!< basaler Durchmesser
		//vector<double> Dbasalliste;
		double	dbasalrel;						//!< is \in (0,1), 
		double	dbrust;							//!< Brustdurchmesser
		//vector<double> Dbrustliste;
		double	dbrustrel;						//!<
		double	height;							//!< Treehöhe
		int		age;							//!< Treeage
		int		cone;							//!< cone am Tree ja/nein(coneanzahl?)
		double	coneheight;						//!< 
		int		seednewly_produced;				//!<
		int		seedproduced;					//!<
		int		speicher;						//!< Energiespeicher aus guten Jahren
	
		double	densitywert;						//!<
		double thawing_depthinfluence;	//!< 100 meint normaler, also weder positiver noch negativer Einfluss
		bool	longdispersed;					//!<
		double	entfernung;						
		bool    growing;						//!< eingesetzte Bäume sind statisch und wachsen nicht
		int 	species;							//!< 1 = Larix gmelinii, 2 = Larix sibirica,
		
		
	};


	struct seed 
	{
		int		xworldcoo;						//!<
		int		yworldcoo;						//!<
		double	xcoo;							//!<
		double	ycoo;							//!<
		int		namem;							//!<
		int		namep;							//!<
		int		line;							//!<
		int		generation;						//!<
		bool	imcone;						//!<
		double	gewicht;						//!< seed weight
		int		age;							//!< seed age
		bool	longdispersed;					//!<
		double 	entfernung;						// distance travelled by a seed
		int		species;						//!< Art: 1= L.gmelinii, 2=L.sibirica
		double 	elternheight;					// Height of the parent tree
		
		 double maxgrowth;
		 
		 double pollenfall;
		 
		 double descent;
		 
		double thawing_depthinfluence;
	
	};
	


	struct Karten 
	{
		int		xworldcoo;						//!<
		int		yworldcoo;						//!<
		double	xcoo;							//!<
		double	ycoo;							//!<
		double	Treedensitywert;					//!<
		int		Treeanzahl;						//!<
		unsigned short maxthawing_depth; 			//!< in mm demnach max 65.535 m
		unsigned short auflagenstaerke;			//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke0;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke1;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke2;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke3;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke4;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke5;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke6;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke7;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke8;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerke9;		//!< in 0.1mm demnach max 6.5535 m
		unsigned short auflagenstaerkemittel;	//!< in 0.1mm demnach max 6.5535 m
	};


	struct Parameter
	{
		// ----------Logische Operatoren fuer Anzeigen und einzelne Funktionsaufrufe-------- //
		
		// festlegen in parameter.txt, aufgerufen in 
		bool	parameterlesenanzeige;			//!< 0 = einegelesene Parameter in der Konsole nicht anzeigen; 1 = anzeigen
		bool	jahranzeige;					//!< 0 = nichts anzeigen, 1 = Schreibe Anzahl Sites pro Ort, Jahr, Zeitschritt, Simulationsdauer in Konsole
		bool	qualijahranzeige;				//!< 0 = nichts anzeigen, 1 = Schreibe  jahr, tempjahrmittel, niederschlagssumme, activeairtemp, degreday, vegetationslaenge
		bool	seedausbranzeige;				//!< 0  nichts anzeigen, 1 = Schreibe Rausflieger(N/O/S/W)= rausgeflogenN, rausgeflogenO, rausgeflogenS, rausgeflogenW
		bool	mortanzeige;					//!< Anzeige der berechneten Mortalitaeten fuer jedes Individuum;  0=nicht, 1=anzeigen
		bool	Data_output;					//!< 
		
		// festlegen in main() und aufgerufen in Main(), Treeverteilungwiederholung
		bool	starter;						//!< Intern: Mit leerer Flaeche beginnen;  true= leere Fläche, false= Treeverteilung 11-CH-17/I
		
		// festlegen und aufgerufen in Main()
		bool	seedintro;						//!< Intern: seed einbringen;  true= seed einbringen
		
		int		starttrees;					//!< soll mit treesn auf den Flaechen gestartet werden
		
		// festlegen in main() und aufgerufen in Main(), Kartenupdate, Feuer, Data_output
		bool	einschwingen;					//!< Ausgabe waehrend der Einschwingphase;  true=Einschwingphasae, false= keine, beendete Einschwingphase


		// festlegen in parameter.txt, aufgerufen in  Kartenupdate
		bool	thawing_depth;					//!< soll die thawing_depth in jedem Zeitschritt berechnet werden?  0=nein, 1=ja
		bool	vegetation;						//!< soll Vegetation auf den Flaechen wachsen und sich auf die thawing_depth, Etablierung und das Wachstum auswirken?   0=nein, 1=ja

		// festlegen in parameter.txt, aufgerufen in  Feuer
		int		feuer;							//!< soll das Modul Feuer ausgefuehrt werden? 0=nein, 1=ganze Flaeche	
		// mulit-core processing with omp
		int		omp_num_threads;

		// festlegen in parameter.txt, aufgerufen in  weathereingabe
		int		precweather;						//!< soll der Niederschlag einen Einfluss auf das Wachstum haben? 0=nein, 1=linearer Einfluss bei Abweichung von Grenzwert und 2="normaler" Einfluss mittels Parabel
		
		// festlegen in parameter.txt oder main, aufgerufen in  weathereingabe
		double	precgrenzwert;					//!< Grenzwert ab welchem der Niederschlag einen negativen Einfluss auf das Wachstum haben soll


		// ----------Allgemeine Modellvariablen und -parameter-------- //
		// Fuer die Data_output
		
		// festlegen in main() und aufgerufen in Data_output
		bool ordnerda;							//!<  Ordner fuer die Zuwachsleistungsausgabe im Heidelberg-Format
		
		// festlegen in parameter.txt, aufgerufen in aufgerufen in  Data_output
		int ausgabemodus;						//!< ausgabemodus==0 "normal", ==1 "SensitivityAnalysis -> 2 Phasenausgabe log und Capacity" 
		

			//NEUNEUNEUNEUNEUNEUNEU:
			int parameteri;


		// festlegen in parameter.txt und aufgerufen in  und Main
		int		simdauer;						//!< MODIFIZIERBAR (1-6099) Bsp: 10 => Startjahr 2090 Endjahr 2100 ingesamt von -3998 bis 2100 definiert fuer das weather 9 =>6099 Jahre
		
		// festlegen und aufgerufen in Main
		int		startjahr;						//!<  =2101-parameter[0].simdauer
		
		// festlegen in parameter.txt und aufgerufen in  und Main
		int		runs; 							//!< Anzahl an Simulationsläufen
		unsigned int ivortmax;					//!< Anzahl an Jahren zum Einschwingen sowie maximale Simulationsdauer fuer das Erreichen des quasiGG falls stabilperiod==1
		bool stabilperiod;						//!< Vor den Simulationslaeufen kann eine Stabilisierungsphase vorgeschoben werden (int 0,1)
		double stabilmovingwindow;				//!< Fensterweite zur Berechnung der Mittelwerte fuer die Evaluierung des Zustands des Systems
		double stabilpercentchangethreshold;	//!< Schwellenwert der bei Unterschreitung anzeigt ob ein System Stabil ist
		int resetyear;							//!< Jahr in dem alle Container gespeichert werden für späteren Resume
		int specpres;							//!< choice, ob beide Arten (0), L.gmel (1) oder L.sib (2) im Modell vorkommen sollen
		
		// festlegen und aufgerufen in Main
		unsigned int ivort;						//!< Intern: Variable fuer das aktuelle Jahr
		
		// festlegen in Main aufgerufen in Main Data_output
		int		wiederholung;					//!< 	
		
		// festlegen in parameter.txt und aufgerufen in  und Main
		int		weatherchoice;						//!< 1==konstante Stationswerte der Mittelwerte der Jahre 1947-1999, 9==eigens zusammengesetztes weather
		
		// festlegen und aufgerufen in weathereingabe
		double	tempdiffort; 					//!< =-0.3508 * (Nposakt-Nposcenter)
		double	precdiffort; 					//!< =-5.3699 * (Nposakt-Nposcenter) 
		double	tempdiffortmin; 				//!< Differenzwerte berechnen mit Temp=-0.3508 pro Grad
		double	precdiffortmin;					//!< Differenzwerte berechnen mit Prec=-5.3699 pro Grad

		// festlegen in parameter.txt und aufgerufen in  und weathereingabe
		double	evapod;							//!< "Evaporation rates of 1.5 mm/day mean growing season" (Dolmann, 2004)
		



		// ---------- Weltparameter -------- //
		
		// festlegen in parameter.txt und aufgerufen in  weathereingabe, Main, Treeverteilung, seedproduktion, Data_output 
		int		mapxlength;						//!< Wiederholungen je Ort

		// festlegen in parameter.txt und aufgerufen in  weathereingabe, Main, Treeverteilung, seedausbreitung, Data_output
		int		mapylength;						//!< Anzahl Transektorte

		// festlegen in parameter.txt und aufgerufen in  weathereingabe, Wachstum, Etablierung  
		bool	weathercalcgradient;			//!< Sollen die Temperatur- und Niederschlagswerte anhand der latitudinalen Position des Plots korrigiert werden - Dazu werden abgeleitete Klimagradienten herangezogen
		bool	lineartransekt;					//!< Sollen Minimalwerte im Norden aus abgeleiteten Klimagradienten berechnet werden?
		
		// festlegen in parameter.txt und aufgerufen in  weathereingabe, seedausbreitung
		double	Nposmax;						//!< Maximale Position in Grad Nord 
		double	Nposmin;						//!< Minimale Position in Grad Nord
		
		// festlegen in Main aufgerufen in Main, Data_output, Feuer, Kartenupdate, Etablierung
		int		sizemagnif;						//!< Aufloesung des Treedensityngitters (z. B. 5 => 20x20cm² gitter, 1 => 100x100cm² gitter


		// festlegen in parameter.txt oder Main  und aufgerufen in  weathereingabe, Etablierung, Mortalität
		// Basaldurchmesser-Faktor und -Constante in e^(const + dbh*factor + factor quadratisch * dbh*dbh) for both species
		double gdbasalfacqgmel;	
		double gdbasalfacgmel;			
		double gdbasalconstgmel;
		double gdbasalfacqsib;			
		double gdbasalfacsib;			
		double gdbasalconstsib;			
	
		// festlegen in parameter.txt oder Main aufgerufen in   Wachstum
		// Brustdurchmesser-Faktor und -Constante in e^(const + dbh*factor) for both species
		double gdbrustfacqgmel;	
		double gdbrustfacgmel;			
		double gdbrustconstgmel;
		double gdbrustfacqsib;			
		double gdbrustfacsib;			
		double gdbrustconstsib;			

		// festlegen in parameter.txt und aufgerufen in  Wachstum
		double	basaleinflussaltneu;			//!< (0,+oo) f=dbasalneu*0.3*dbasalalt 
		int		relwachstumeinfluss;			//!< Art des Einflusses des aktuellen Wachstums auf andere Prozesse: 0="keiner", 1="normal"
		
		// festlegen in parameter.txt und aufgerufen in  Treeverteilung
		bool	realseedconnect;				//!< logisch (1/0): Wenn 1, dann werden nur in der suedlichsten Site seed (N=Grundetab) ausgebracht
		
		// festlegen in parameter.txt und aufgerufen in  Main
		int		jahremitseedeintrag; 			//!< Wieviele Jahre von Beginn an sollen seed auf der Flaeche ausgebracht werden?
		
		bool	seedintropermanent;
		
		// festlegen in parameter.txt und aufgerufen in  Treeverteilung
		int		seeddispmode;					//!< Art des seedeinbringens der Starterseed 1==vom Sueden einbringen; 2==zufaellig ueberall einbringen
		
		// festlegen in parameter.txt oder Main aufgerufen in  Treeverteilung
		int		etabbg;							//!< soviele seed werden jährlich von außen ins Feld von Sueden aus eingebracht
		int 	etabbgpermanent;
		
		// festlegen in parameter.txt und aufgerufen in  seedausbreitung
		bool	seedtravelbetween;				//!< Sind die Einzelflaechen verknuepft und es kann LDSD passieren?

		

		// Allometrische Beziehungen: heightn ueber Basaldurchmesserfunktion
		// festlegen in parameter.txt und aufgerufen in  Wachstum, Etablierung, Mortalität
		int		allometriefunktionstyp;			//!< Art wie die height aus den Durchmessern bestimmt werde soll: 1=linear (H=m*D^exp+0 bzw. 130), 2=nichtlinear (H=m*DBAS+0 sowie H^0.5=m*DBRu^0.5+130)

		// festlegen in parameter.txt und aufgerufen in  Wachstum, Etablierung, Mortalität
		double	dbasalheightallosteig;			//!<   Steigung in allometrischer Wachstumsfunktion wenn allometriefunktionstyp == 1 
		double	dbasalheightalloexp;				//!<   Exponent in allometrischer Wachstumsfunktion wenn allometriefunktionstyp == 1
		
		// festlegen in parameter.txt und aufgerufen in  Wachstum, Mortalität
		double	dbrustheightallosteig; 			//!< 

		// festlegen in parameter.txt und aufgerufen in  Wachstum
		double	dbrustheightalloexp;				//!< 

		// festlegen in parameter.txt und aufgerufen in  Wachstum, Etablierung, Mortalität
		double	dbasalheightsteignonlin;			//!< s.o.  Steigung in allometrischer Wachstumsfunktion wenn allometriefunktionstyp == 2

		// festlegen in parameter.txt und aufgerufen in  Wachstum, Etablierung
		double	dbrustheightsteignonlin;			//!< s.o.  Exponent in allometrischer Wachstumsfunktion wenn allometriefunktionstyp == 2
		

		// festlegen in parameter.txt oder Main aufgerufen in  weathereingabe
		//double	aatnddratio;					//!< =2; geschaetzt
		
		// festlegen in parameter.txt oder Main aufgerufen in  Kartenupdate
		int		densityart; 						//!< 1: keine density (densitywert der trees=0), 2: ZOI-kind of type und 3: Zufaellige Felder aus ZOI-type
		double	incfac;							//!< Groesze der Flaeche die ein Tree beeinflusst (Max R=2.4 m mit einer DB=80cm entsp. ca. 16m² = 10)
		int calcinfarea;						//!< Wie wird die Flaechengroesze berechnet, die ein Tree beeinflusst (1 linear, 2 logistisch)
		double densitywertmanipulatorexp;
		double densitysmallweighing;			//!< Wird der densitywert fuer Individuen die nur eine densitykachel beeinflussen anhand des Einflusses gewichtet? (0 keine Wichtung oder 1 Wichtung)
		double densitytreetile;					//!< Wie wird der densitywert berechnet, der von einem Tree in die jeweiligen Zellen geschrieben wird (0 von 1 bis 0 skaliert, 1 der Basaldurchmesser wird genutzt)
		int densitytiletree;					//!< Wie werden die densitywerte die auf einen Tree innerhalb seiner ZOI verechnet (1 Summe, 2 Multiplikation)
		double desitymaxreduction;				//!< Welcher Maximalwert fuer den densityeinfluss wird angenommen (zwischen 0 und 1)
		int dichtheightrel;
		
		// festlegen in parameter.txt und aufgerufen in  Kartenupdate
		double	densitywertdbasaleinfluss; 		//!< Einfluss des Basaldurchmessers auf den densitywert => grosze trees haben staerkere Konkurrenz untereinander als mit kleinen treesn; Bis zu einem Brustdurchmesser von 100 cm wenn der Faktor 0.01 eingestellt ist steigt der Einfluss linear zum Durchmesser
		
		// festlegen in parameter.txt und aufgerufen in  Mortalität
		double	densitywertmaximumbeiheight; 		///! Einfluss der height mit einem Maximum bei diesem Wert per Kugelfunktion => WERT=150 == Min=0cm Max=300cm mit Maximum bei 150cm von 1.0 

		// festlegen WO? und aufgerufen in Main
		int		individuenzahl; 				//!<

		// festlegen in parameter.txt oder Main aufgerufen in  Mortalität
		double	mortbg;  						//!< Hintergrundmortalität, geschaetzt aus Daten abgeleitet => keine Steigung ab etwa age 150 bis 350 => Vmtl. vom weather?!
		int		maximalage;  					//!< PE

		// festlegen in parameter.txt oder Main aufgerufen in  Ageing
		int		coneage; 					//!< =15;  age bei den ersten cone; nach PE  
		
		// festlegen in parameter.txt oder Main aufgerufen in  seedproduktion
		double	seedprodfaktor; 				//!< =10;  nach PE ca. 20
		
		// festlegen in parameter.txt oder Main aufgerufen in  seedausbreitung
		double	seedflugrate;
		
		// festlegen in parameter.txt und aufgerufen in  seedausbreitung  Treeverteilung
		int		dispersalmode; 					//!<  0=zufaellig, 1=exp, 2=fat tailed, 3=gaussian und 4=gaussian+fat tailed combined

		// festlegen in parameter.txt und aufgerufen in seedausbreitung
		double SeedTravelBreezeg;						// gibt nach Matlack 1987 geschaetzte Entfernung wieder, die die seed von L.gmelinii bei einer Windgeschwindigkeit von 10km/h = breeze fliegen
		double SeedTravelBreezes;						// gibt nach Matlack 1987 geschaetzte Entfernung wieder, die die seed von L.sibirca bei einer Windgeschwindigkeit von 10km/h = breeze fliegen
		double SeedDescentg;							// gibt nach Matlack 1987 die Sinkflugrate von L.gmelinii wieder, anhand anderer seed desselben Papers geschaetzt
		double SeedDescents;							// gibt nach Matlack 1987 die Sinkflugrate von L.sibirica wieder, anhand anderer seed desselben Papers geschaetzt
		
		// festlegen in parameter.txt oder Main aufgerufen in  seedausbreitung, Treeverteilung
		double	distanceratio; 				//!< =0.16; geschaetzt von 0.4 geaendert weil nun 1 Einheit = 1 m und nicht 0.4 m

		// festlegen in parameter.txt oder Main aufgerufen in  Etablierung
		double	keimungsrate; 					//!< =0.05; geschaetzt
		double	keimungweathereinfluss; 			//!< (0,+oo) kein einfluss oder 1 dann +(WERT*(maxbasawachstum/basaldurchmesserwachstumsrate))
		
		// festlegen in parameter.txt oder Main aufgerufen in  Mortalität
		double	mjung; 							//!< Mortalitaetsfaktor_Jugend=0.95;
		double	jugendmorteinflussexp;	 		//!<Einfluss der Jugendmortalitaet; Werte von <1 lassen die Mortalitaet ueber einen laengeren Zeitraum wirken und vice versa
		// festlegen in parameter.txt und aufgerufen in  Mortalität
		double	mgrowth;						//!<Mortalitaetsfaktor_Growth
		double	relwachstummorteinflussexp;  	//!<Einfluss des relativen Wachstums auf die Mortalitaet. Staerke der Abweichung von 1 => exp>1 = staerker, <1 = schwaecher
		double	mweather; 						//!< Mortalitaetsfaktor_weather=0.5;
		double	heightweathermorteinflussexp;		//!< / Bei 0=1 bei 0.1=0.5 bei 10m oder 0.5 0.03 bei 10m height 
		
		// festlegen in parameter.txt und aufgerufen in  Mortalität, Kartenupdate, Data_output
		double	mdensity; 						//!<  Mortalitaetsfaktor_density=0.1; (0,+oo)
		
		
		// festlegen in parameter.txt und aufgerufen in  Mortalität
		double	malt;		 					//!  Mortalitaetsfaktor_age=10.0;
		double	mtrockenheit;		 			//!< Mortalitaetsfaktor_Trockenheit=0.0; (0,+oo) Zuletzt war ein Wert von 10 eingestellt
		double	seedTreemort;		 			//!<	 
		double	seedbodenmort;		 			//!<	
		
		// Fuer Verwandtschaftsanalysen
		// festlegen in Main und aufgerufen in Etablierung, Treeverteilung
		int		nameakt;		 				//!<	
		
		// festlegen in Main und aufgerufen in  Treeverteilung
		int		lineakt;
		
		// festlegen in Main und aufgerufen in Feuer, Data_output
		int		feuerausgabe;		 			//!<	Fuer die Ausgabe ob es ein Feuer in dem Jahr gab
		
		// neu dazugekommen
		int gmel_seedtod;						// legt fest, wann die sterben
		double jantodestempgmel;				// legt Temperatur für Januar-Restriktion fest


		double weatherzahlgmela;					// legt Kurvenverlauf des Julindex für gmel fest
		double weatherzahlgmelb;					// legt Kurvenverlauf des Julindex für gmel fest
		double weatherzahlgmelc;					// legt Kurvenverlauf des Julindex für gmel fest
		
		int windsource;
		int periodRB;

		int outputall;

		//double pollenthreshold;
		
		double pollenrichtungsvarianz;
		
		int pollenvert;//switch genealogy!
		int pollenDistdensplot;
		
		
		double SNPMutationrate;
		
		double microsatelliteMutationrate;
		
		double pollenfall;
		
		double Gregorym,GregoryC;
		
		bool computationtime;
		
		double rel_trait_variance;
		
	};


	struct weather 
	{
		int		xworldcoo;		 				//!<
		int		yworldcoo;		 				//!<
		int		jahr; 		 					//!<

		double	temp1monatmittel; 		 		//!<
		double	temp1monatmitteliso; 		 	//!<
		double	temp1monatmittelmin; 		 	//!<
		double	temp1monatmittelisomin; 		//!<
		
		double	temp2monatmittel; 		 		//!<
		double	temp3monatmittel; 		 		//!<
		double	temp4monatmittel; 		 		//!<
		double	temp5monatmittel; 		 		//!<
		double	temp6monatmittel; 		 		//!<

		double	temp7monatmittel; 		 		//!<
		double	temp7monatmitteliso; 		 	//!<
		double	temp7monatmittelmin; 		 	//!<
		double	temp7monatmittelisomin; 		//!<
		
		double	temp8monatmittel; 		 		//!<
		double	temp9monatmittel; 		 		//!<
		double	temp10monatmittel; 		 		//!<
		double	temp11monatmittel; 		 		//!<
		double	temp12monatmittel; 		 		//!<
		
		double	tempjahrmittel; 		 		//!<
		double	tempjahrmittelmin; 		 		//!<
		
		double	prec1monatmittel; 		 		//!<
		double	prec2monatmittel; 		 		//!<
		double	prec3monatmittel; 		 		//!<
		double	prec4monatmittel; 		 		//!<
		double	prec5monatmittel; 		 		//!<
		double	prec6monatmittel; 		 		//!<
		double	prec7monatmittel; 		 		//!<
		double	prec8monatmittel; 		 		//!<
		double	prec9monatmittel; 		 		//!<
		double	prec10monatmittel; 		 		//!<
		double	prec11monatmittel; 		 		//!<
		double	prec12monatmittel; 

		int		vegetationslaenge; 		 		//!<
		int		vegetationslaengeiso; 		 	//!<
		int		vegetationslaengemin; 		 	//!<
		int		vegetationslaengeisomin;		//!< 
		
		double	activeairtemp; 		 			//!<
		double	activeairtempmin;		 		//!<
		
		double	degreday;		 				//!<
		
		double	niederschlagssumme; 		 	//!<
		double	niederschlagssummemin;		 	//!<
		
		double	weatherfactorg; 		 			//!<
		double	weatherfactorming;				//!<
		double	weatherfactors; 		 			//!<
		double	weatherfactormins;				//!<		
		
		double	trockenheitsmort;				//!<
		
		//double??
		double janisothermrestriktions;
		double julisothermrestriktion;
		double nddrestriktion;
		double janmorttemp;
		double janisothermrestriktiong;
		
		
	};


	struct Evaluation
	{
		int		xworldcoo;						//!<
		int		yworldcoo;						//!<
		vector<double> BAliste;					//!<
		vector<double> BArunmeanliste;			//!<
		double	maxsteigungbasalarea;			//!<
		int		countermaxsteigungbasalarea;	//!<
		int		wendejahr;						//!<
		int		saettigungsjahr;				//!<
		bool	nachwendejahr;					//!<	
		
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
