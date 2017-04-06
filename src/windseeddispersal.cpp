double getEntfernung(double D, double ratiorn_help)	
{

	double entf_help;
	if (parameter[0].dispersalmode==0)
	{ // zufaellig in bestimmter Entfernung
		entf_help= ((ratiorn_help-1)*100.0)/(0.05-1);
	}
	else if (parameter[0].dispersalmode==1)
	{ // neg. exponential
		entf_help= parameter[0].entfernungsteiler * ((log(ratiorn_help)/(-0.2))/0.16);
	}
	else if (parameter[0].dispersalmode==2)
	{ // fat tailed/power law
		double fatalpha=0.5;
		entf_help= parameter[0].entfernungsteiler *  pow(ratiorn_help, (-1*(1+fatalpha)) );
	}
	else if (parameter[0].dispersalmode==3)
	{ // gaussian
		double gaussweite=D, gaussmaxh=1, gaussposcenter=0;
		entf_help= parameter[0].entfernungsteiler *  sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter;
	}
	else if (parameter[0].dispersalmode==4 || parameter[0].dispersalmode==5)
	{ // gaussian combined with fat tailed
		double gaussfatratio=2.0;
		double gaussweite=D, gaussmaxh=1, gaussposcenter=0;	//gaussweite variieren??
																//oder unten 4500m?
		double fatalpha=0.5;
		entf_help= 
		( 0.5*( gaussfatratio*(sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter)+(1/gaussfatratio)*parameter[0].entfernungsteiler * (pow(ratiorn_help, (-1*(1+fatalpha)) )) ) );
		
		//
	}
	else 
	{
		printf("LaVeSi wurde beendet\n\n");
		printf("... Grund: choice der Ausbreitung (= %d) ausserhalb der verfuegbaren Modi!\n", parameter[0].dispersalmode);
		exit(1);
	}

	return(entf_help);


}