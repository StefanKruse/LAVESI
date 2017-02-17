// basic I/O functions
extern double Parametereingabe(char *uebergabestring, int wortlaengemax, char *trennzeichen, struct Parameter *parameter);
extern void Wettereinlesen(int baeumerows, struct Parameter *parameter,  int wortlaengemax, vector<vector<Wetter*> > &WeltWetterListe);
extern void Anzeige(int baeumerows, int baeumecols, vector<Baum*> &BaumListe, vector<Samen*> &SamenListe);
extern void Datenausgabe(int baeumerows, int baeumecols, int t, int jahr, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe, vector<vector<Auswertung*> > &WeltAuswertungsListe);

// functions for each time step
extern void Baumverteilung(int baeumerows, int baeumecols, struct Parameter *parameter, int wortlaengemax, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe);
extern void Kartenupdate(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<vector<Karten*> > &WeltKartenListe, vector<list<Baum*> > &WeltBaumListe, vector<vector<Wetter*> > &WeltWetterListe)
extern void Wachstum(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe);
extern void Samenausbreitung(int baeumerows, int baeumecols, struct Parameter *parameter, vector<list<Samen*> > &WeltSamenListe);
extern void Samenproduktion(int baeumerows, int baeumecols, struct Parameter *parameter, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe);
extern void Etablierung(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe);
extern void Mortalitaet(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe);
extern void Altern(int baeumerows, int baeumecols, struct Parameter *parameter, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe);
