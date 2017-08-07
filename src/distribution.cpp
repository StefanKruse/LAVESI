/***********************************************************************************
 * AUTHORS: Gerdes, Alexander; Kath, Nadja; Kruse, Stefan
 * 
 * NAME:
 * distribution.cpp
 * 
 * USE:
 * integration into different functions of an individuum based model via:
 * #ifndef _distribution_H_
 * #define _distribution_H_
 * #include "distribution.cpp"
 * #endif
 * 
 * CONTENTS:
 * This .cpp file contains the three functions used by LaVeSi to realize wind driven
 * seed- and pollen dispersal:
 * a) BefrWahrsch:
 *    A function returning a vector of the most probable trees to pollinate a tree 
 *    at the position x,y
 * 
 * b) getEntfernung:
 *    A function calculating and returning the traveling distance for:
 * 
 * c) seeddisp:
 *    The function returning the new position of a seed after dispersal. 
 *    In this the wind direction and wind speed is chosen, using the wind direction
 *    for the seed flight direction and the wind velocity to determine the ballistic
 *    maximal flight distance (Matlack) to determine the standard deviation in b) 
 * 
 * INPUT DATA:
 *  a) absolute position of an individual tree/seed, the tree list's iterator,
 *     the year of seed production, 
 *     a globally defined std:vector<double> of wind speeds and wind directions and
 *     a set of parameters
 *  b) the ballistic maximum dispersal distance D, a random number ratiorn \in (0,1)
 *     and a set of parameters
 *  c) the random number \in (0,1) mentioned above, the year of seed dispersal,
 *     a globally defined std:vector<double> of wind speeds and wind directions and
 *     a set of parameters
 * 
 * OUTPUT DATA:
 *  a) a vector of the names of the most probable trees to pollinate another tree,
 *     conceptual preparation for later use: cpSNPs-> genetic data
 *  b) the flight distance of a dispersed seed
 *  c) the absolute position of a dispersed seed in x,y-coordinates
 * 
 ************************************************************************************/
 
//				(pTree->xcoo,pTree->ycoo,struct *parameter,world_positon_b,Jahr,Vname);
void BefrWahrsch(double x, double y,struct Parameter *parameter, vector<std::list<Tree*> >::iterator world_positon_b, int yr, 
				double richtung,double geschwindigkeit,unsigned int ripm,unsigned int cntr,double p,double kappa,double phi,double dr,double dx,double dy,double I0kappa,double pe,double C,double m,
			vector<int> &pName, vector<double>  &thdpthinfl,
											int outputtreesiter
		)//, vector<int> &cpSNPs1, vector<int> &cpSNPs2)
{
	/*
  int year=yr-1;
  list<Tree*>& tree_list = *world_positon_b;
  double  richtung=0.0,mwrichtung=0.0;
  double  geschwindigkeit=0.0;
  int    findyr1=2,findyr2=-100;
//   std::ostringstream outputstring;
  string output;
  string item;
  unsigned int    ripm=0,cntr=0,cntr2=0;
  vector<double> wdir,wspd;
//   vector<int> Vname;
  vector<int> SNP1,SNP2;
  double  p=0.0,kappa=0.0,phi=0.0,dr=0.0,dx=0.0,dy=0.0;
  double  I0kappa=0.0;
  double pe=0.01;
  double  C=parameter[0].GregoryC;
  double  m=parameter[0].Gregorym;
  
    pName.clear();
//     Vname.clear();
    wdir.clear();
    wspd.clear();
  
 */
 
   list<Tree*>& tree_list = *world_positon_b;
  richtung=0.0;
  geschwindigkeit=0.0;
  // char output[50];
  ripm=0;
  cntr=0;
  // vector<int> SNP1,SNP2;
  p=0.0;
  kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2);
  phi=0.0;
  dr=0.0;
  dx=0.0;
  dy=0.0;
  I0kappa=0.0;
  pe=0.01;
  C=parameter[0].GregoryC;
  m=parameter[0].Gregorym;
		/// #####################################
		/// #####################################
  pName.clear();
  thdpthinfl.clear();
  
  
  /*
  if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5){
                
                if(parameter[0].windsource==1){
		    findyr1=1947;findyr2=2012;
		    }else if(parameter[0].windsource==2){
		    findyr1=1979;findyr2=2012;
		    }else if(parameter[0].windsource==3){
		    findyr1=1959;findyr2=2002;
                }
			
		if(year<findyr1 or year>findyr2){year=(findyr1+floor(rand()/RAND_MAX*(findyr2-findyr1)));}
			
		for(unsigned int i=0;i<(unsigned int)globalyears.size();i++){
		if(globalyears[i]==year){
		for(unsigned int pos=0;pos<(unsigned int)winddir[i].size()-1;pos++){
                        wdir.push_back(M_PI*winddir[i][pos]/180);
                        wspd.push_back(windspd[i][pos]);
                        }
                    }
		}
		cntr=wdir.size();
		
                }else if(parameter[0].windsource==4){
		  richtung=0.0;
		  geschwindigkeit=0.2777;
 		  kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2);
		}else if(parameter[0].windsource==5){
		   richtung=M_PI*1.0;
		   geschwindigkeit=0.2777;
 		   kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2);
		}else if(parameter[0].windsource==0){
                   richtung=M_PI*(rand()/RAND_MAX); 
		}
		
		
		if(cntr!=0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)){
		  ripm=((unsigned int)floor(0.5*cntr - cntr*0.16666));//etwa Anfang Mai
		 //Mittelwertberechnung
                for(unsigned int i=ripm;i<floor(ripm+0.5*0.58*cntr);i++){//bis etwa Ende August
		  geschwindigkeit+=wspd.at(i);
		  mwrichtung+=wdir.at(i);
		  cntr2++;		  
                }
		 
		 richtung=wdir.at((int)(0.5*(cntr +cntr2*(1-2*rand()/(RAND_MAX+1.0)))));
		 mwrichtung/=cntr2;
                 //mean value of wind speed:
		 geschwindigkeit/=cntr2;
		//pe= pTree->pollenfall/geschwindigkeit;
		 pe=parameter[0].pollenfall/geschwindigkeit;
		 
		 //inverse kappa afterwards:
		 kappa=(double)cntr2/kappa;
		 
 		 }else if((cntr==0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)) or parameter[0].windsource==0){
		   richtung=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
		   geschwindigkeit=0.2777;//10 km/h
                 }
		
                wdir.clear();
				wspd.clear();
		 
                //Simpson integration + elec424.wikia.com/wiki/Modified_Bessel_Functions:
                 I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa));
		*/
		
		if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5)
		{
			cntr=1;//floor(windspd[i].size()*2.5*0.1666)-windspd[i].at(floor(windspd[i].size()*4.5*0.1666));
        }else if(parameter[0].windsource==4)
		{
			richtung=	M_PI * ( 0 / 180 );
			geschwindigkeit=2.777;
 		  //kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2);
		}else if(parameter[0].windsource==5)
		{
			richtung=	M_PI * ( 180 / 180 );
			geschwindigkeit=2.777;
 		   //kappa=pow(180/(parameter[0].pollenrichtungsvarianz*M_PI),2);
		}else if(parameter[0].windsource==0)
		{
           richtung=2*M_PI*(rand()/RAND_MAX); 
		   geschwindigkeit=2.777;
		}


		if(cntr!=0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3))
		{
			// ripm=(unsigned int)floor(winddir.size()*2.5*0.16666+rand()/(RAND_MAX+1.0)*winddir.size()*4.5*0.16666);//((unsigned int)floor(0.5*cntr - cntr*0.16666));//etwa Anfang Mai
			ripm=(int)(0.5*wdir.size() + wdir.size()/6 *(1-2*rand()/(RAND_MAX+1.0)));
		
			// richtung=180/M_PI * wdir.at(ripm);
			richtung=	M_PI * ( wdir.at(ripm) / 180 );
			geschwindigkeit=wspd.at(ripm);
		
		}else if((cntr==0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)) || parameter[0].windsource==0)
		{
		   richtung=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
		   geschwindigkeit=2.777;//10 km/h
        }
		
		pe=parameter[0].pollenfall/geschwindigkeit;
		 
        //Simpson integration + elec424.wikia.com/wiki/Modified_Bessel_Functions:
        I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa));
				 
				 
        for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); )
		{
			pTree_copy= *posb;
			
            //only if the pollinating tree has cones:
			if(pTree_copy->cone!=0)
			{
				
				dx=(pTree_copy->xcoo)-x; 
				dy=(pTree_copy->ycoo)-y; 
				dr=sqrt(dx*dx+dy*dy);
				
				if((dr!=0)){
							//set to 0=west:
				  phi=atan2(dx,dy);
				}else{phi=richtung+0.5*M_PI;}//makes self-pollination less probable: p~1/I0(kappa)
				
				p=exp(kappa*(cos(phi-richtung)*-1))/(2*I0kappa)*(exp(-2*pe*pow(dr,1-0.5*m)/(sqrt(M_PI)*C*(1-0.5*m))));
				//f(dr) aus Microbiology of the atmosphere, p(phi) ist von-Mises-Distribution

				/* RCODE
					# constants
					kappa=(180/(10*pi)^2)
					I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa))
					C=0.6
					m=1.25
					pollenfall=0.126
					pe=pollenfall/2.777


					# variables
					dx=-5
					dy=5
					phi=atan2(dx,dy)
					phi
					dr=sqrt(dx*dx+dy*dy)
					dr

					#N/E/S/W winds
					windri=c(0,pi/2,pi,3*pi/2)
					coss=cos(phi-windri)*-1
					round(coss,1)
						###round(cos(windri-phi),1)
					
					p=exp(kappa*coss)/(2*I0kappa)*(exp(-2*pe*(dr^(1-0.5*m))/(sqrt(pi)*C*(1-0.5*m))))
					p
					
				*/
				
				if(rand()>p*RAND_MAX){
				++posb;
				//	pName.push_back(pTree_copy->name);
				//	cpSNPs1.push_back(pTree_copy->cpSNP[0]);
				//	cpSNPs2.push_back(pTree_copy->cpSNP[1]);
				//	++posb; 
				}else{
					pName.push_back(pTree_copy->name);
					// thdpthinfl.push_back(pTree->thawing_depthinfluence);
					thdpthinfl.push_back(100);
					//cpSNPs1.push_back(pTree_copy->cpSNP[0]);
					//cpSNPs2.push_back(pTree_copy->cpSNP[1]);
					++posb; 
				}
				
				//data output for pollen flight analysis:
				// if( parameter[0].ivort==1057 && parameter[0].omp_num_threads==1) // ivort==1057 => 1990
				if(parameter[0].pollenvert==1 && parameter[0].omp_num_threads==1 && outputtreesiter<=5 && parameter[0].ivort==1057) // ivort==1057 => 1990
				{
					//print data
					FILE *fdir;
					char filenamechar[25];
					sprintf(filenamechar, "IVORT%.4d_REP%.3d", parameter[0].ivort, parameter[0].wiederholung);
					string output="output/windgen_pollination_total_" + string(filenamechar) + ".txt";
					fdir=fopen(output.c_str(),"a+");
					fprintf(fdir,"%10.20f \t %10.20f \t %10.20f \t %d \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t \n", dr, phi, p, pTree_copy->name, pTree_copy->xcoo, pTree_copy->ycoo, x, y);
					fclose(fdir);
					// outputstring.str("");
					// outputstring.clear();
				}
			}else{++posb;}
		}
		// cntr=0;cntr2=0;
}


double getEntfernung(double D, double ratiorn_help)	
{
	double entf_help;
	if (parameter[0].dispersalmode==0)
	{ // zufaellig in bestimmter Entfernung
		entf_help= ((ratiorn_help-1)*100.0)/(0.05-1);
	}
	else if (parameter[0].dispersalmode==1)
	{ // neg. exponential
		entf_help= parameter[0].distanceratio * ((log(ratiorn_help)/(-0.2))/0.16);
	}
	else if (parameter[0].dispersalmode==2)
	{ // fat tailed/power law
		double fatalpha=0.5;
		entf_help= parameter[0].distanceratio *  pow(ratiorn_help, (-1*(1+fatalpha)) );
	}
	else if (parameter[0].dispersalmode==3)
	{ // gaussian
		double gaussweite=D, gaussmaxh=1, gaussposcenter=0;
		entf_help= parameter[0].distanceratio *  sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter;
	}
	else if (parameter[0].dispersalmode==4 || parameter[0].dispersalmode==5)
	{       // gaussian combined with fat tailed:
		double gaussfatratio=2.0;
		double gaussweite=D, gaussmaxh=1, gaussposcenter=0;	//gaussweite variieren??
																//oder unten 4500m?
		double fatalpha=0.5;
		entf_help= 
		( 0.5*( gaussfatratio*(sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter)+(1/gaussfatratio)*parameter[0].distanceratio * (pow(ratiorn_help, (-1*(1+fatalpha)) )) ) );


//cout << " => " << entf_help;		

		/* for R
		
		
			pow=function(x,exp){x^exp}
			( 0.5*( 2*(sqrt( 2*pow(11.77,2)*(-1*log(c(0,1)/1)) )+0)+(1/2)*0.16 * (pow(c(0,1), (-1*(1+0.5)) )) ) )
			
		
		*/
	}
	else 
	{
		printf("LaVeSi wurde beendet\n\n");
		printf("... choice of dispersal mode (= %d) not available!\n", parameter[0].dispersalmode);
		exit(1);
	}

	return(entf_help);
}



void seeddisp(double rn, int yr, double& dx, double& dy, double &windspeed, double &winddirection, double parhei, int seedspec)
{
    int cntr=1,ripm=0;

	// local
    double entfernung = 0;
    double maxentfernung = 0;
    double richtung=0.0;
    double geschwindigkeit=0.0;
	
    if(parameter[0].windsource==4){
        richtung=	M_PI * ( 0 / 180 );
        cntr=0;//speed up
		geschwindigkeit=2.7777;
    }else if(parameter[0].windsource==5){
        richtung=	M_PI * ( 180 / 180 );
        cntr=0;//speed up
		geschwindigkeit=2.7777;
    }else if(cntr!=0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)){

        //choose a month between may and september:
        ripm=(int)(0.5*wdir.size() + wdir.size()/6 *(1-2*rand()/(RAND_MAX+1.0)));
        richtung = M_PI * ( wdir.at(ripm) / 180 );
        geschwindigkeit=(wspd.at(ripm));
        cntr=1;//speed up
    }else if((parameter[0].windsource==0)||(cntr==0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3))){
        richtung=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
        geschwindigkeit=2.7777;
    }
	
    if (seedspec==1){

        maxentfernung = (geschwindigkeit*0.75*parhei*0.01/(parameter[0].SeedDescentg));
//        maxentfernung = (geschwindigkeit*0.75*pseed->elternheight*0.01/(parameter[0].SeedDescentg));
		//maxentfernung = (geschwindigkeit*0.75*pseed->elternheight*0.01/pSeed->descent)
		// cout << " - " << maxentfernung;
    }else if (seedspec==2){
        maxentfernung = (parameter[0].SeedTravelBreezes*0.75*parhei*0.01/(parameter[0].SeedDescents));     
//    maxentfernung = (parameter[0].SeedTravelBreezes*0.75*pseed->elternheight*0.01/(parameter[0].SeedDescents));
		//maxentfernung = (geschwindigkeit*0.75*pseed->elternheight*0.01/pSeed->descent)
    }                                        

    if(parameter[0].dispersalmode==5){


//cout << " (( ph/pseed-elternh=" << parhei << "/" << pseed->elternheight << " + mentf=" << maxentfernung << " ))";

        entfernung= getEntfernung(maxentfernung,rn);                                       
    }else if(parameter[0].dispersalmode!=5){
        entfernung=getEntfernung(maxentfernung,rn);
    }
    // cout << " - " << richtung;
	
    // dy=sin(richtung)*entfernung;
    // dx=cos(richtung)*entfernung;
    dy=cos(richtung)*entfernung;
    dx=sin(richtung)*entfernung;
	/* RCODE
	
		wind in north direction
			winddirrect=0
			entfernungstest=10
			###dy=sin(winddirrect)*entfernungstest
			dy=cos(winddirrect)*entfernungstest
			dy
			###dx=cos(winddirrect)*entfernungstest
			dx=sin(winddirrect)*entfernungstest
			dx
		wind in east direction
			winddirrect=pi/2
			entfernungstest=10
			dy=cos(winddirrect)*entfernungstest
			dy
			dx=sin(winddirrect)*entfernungstest
			dx
		wind in south direction
			winddirrect=pi
			entfernungstest=10
			dy=cos(winddirrect)*entfernungstest
			dy
			dx=sin(winddirrect)*entfernungstest
			dx
		wind in west direction
			winddirrect=3*pi/2
			entfernungstest=10
			dy=cos(winddirrect)*entfernungstest
			dy
			dx=sin(winddirrect)*entfernungstest
			dx
		
	
	*/
	// cout << " - " << dy << "/" << dx;
	
	windspeed=geschwindigkeit;
	winddirection=richtung;    
}
