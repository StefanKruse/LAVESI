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
 

void BefrWahrsch(double x, double y, vector<std::list<Tree*> >::iterator world_positon_b, int yr, vector<int> &pName, vector<int> &cpSNPs1, vector<int> &cpSNPs2)
{
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
		
                for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ){
                        pTree_copy= *posb;
			
                        //only if the pollinating tree has cones:
			if(pTree_copy->cone!=0){
				
 			dx=(pTree_copy->xcoo)-x; 
			dy=(pTree_copy->ycoo)-y; 
 			dr=sqrt(dx*dx+dy*dy);
			
 			if((dr!=0)){
                        //set to 0=west:
 			  phi=atan2(dx,dy);
 			}else{phi=richtung+0.5*M_PI;}//makes self-pollination less probable: p~1/I0(kappa)
 			
   			p=exp(kappa*cos(phi-richtung))/(2*I0kappa)*(exp(-2*pe*pow(dr,1-0.5*m)/(sqrt(M_PI)*C*(1-0.5*m)))) ;
			//f(dr) aus Microbiology of the atmosphere, p(phi) ist von-Mises-Distribution

                        if(rand()>p*RAND_MAX){
			++posb;
			//	pName.push_back(pTree_copy->name);
			//	cpSNPs1.push_back(pTree_copy->cpSNP[0]);
			//	cpSNPs2.push_back(pTree_copy->cpSNP[1]);
			//	++posb; 
			}else{
				pName.push_back(pTree_copy->name);
				//cpSNPs1.push_back(pTree_copy->cpSNP[0]);
				//cpSNPs2.push_back(pTree_copy->cpSNP[1]);
				++posb; 
				
                                
				//data output for pollen flight analysis:
				/*if(((year==1989)||(year==1998)||(year==1987)||(year==1997)) &&(dr>0)&&(rand()<0.0001*RAND_MAX)){
				//print data in the most probable cases:
				FILE *fdir;
				outputstring<<year;
				output="windgen_"+outputstring.str()+"_.txt";
				fdir=fopen(output.c_str(),"a+");
				fprintf(fdir,"%f \t %f\n",dr,phi);
				fclose(fdir);
				outputstring.str("");
				outputstring.clear();
				}*/
			}
			}else{++posb;}
		}
		cntr=0;cntr2=0;
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
	{       // gaussian combined with fat tailed:
		double gaussfatratio=2.0;
		double gaussweite=D, gaussmaxh=1, gaussposcenter=0;	//gaussweite variieren??
																//oder unten 4500m?
		double fatalpha=0.5;
		entf_help= 
		( 0.5*( gaussfatratio*(sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter)+(1/gaussfatratio)*parameter[0].entfernungsteiler * (pow(ratiorn_help, (-1*(1+fatalpha)) )) ) );
	}
	else 
	{
		printf("LaVeSi wurde beendet\n\n");
		printf("... Grund: choice der Ausbreitung (= %d) ausserhalb der verfuegbaren Modi!\n", parameter[0].dispersalmode);
		exit(1);
	}

	return(entf_help);
}



void seeddisp(double rn, int yr, double& dx, double& dy){
    int findyr1=0,findyr2=0;
    int cntr=0,ripm=0;
    vector<double> wdir,wspd;   
    double entfernung = 0;
    double maxentfernung = 0;
    float richtung=0.0;
    float geschwindigkeit=0.0;
    
    if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5){
        if(parameter[0].windsource==1){
        findyr1=1947;findyr2=2012;
        }else if(parameter[0].windsource==2){
        findyr1=1979;findyr2=2012;
        }else if(parameter[0].windsource==3){
        findyr1=1959;findyr2=2002;
        }
                        
        if(yr<findyr1 or yr>findyr2){yr=(findyr1+floor(rand()/RAND_MAX*(findyr2-findyr1)));}
                        
        for(int i=0;i<(signed)globalyears.size();i++){
            if(globalyears[i]==yr){
                for(int pos=0;pos<(signed)winddir[i].size();pos++){
                    wdir.push_back(winddir[i][pos]);
                    wspd.push_back(windspd[i][pos]);
                } } }
            
        cntr=2*wdir.size();
    }else{cntr=0;}

    if(parameter[0].windsource==4){
        richtung=M_PI/180 *270;
        cntr=1;//speed up
    }else if(parameter[0].windsource==5){
        richtung=M_PI*0.5;
        cntr=1;//speed up
    }else if(cntr!=0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)){
    
        //choose a month between may and september:
        ripm=(int)(0.5*wdir.size() + wdir.size()/6 *(1-2*rand()/(RAND_MAX+1.0)));
        richtung = M_PI/180 *(wdir.at(ripm));
        geschwindigkeit=(wspd.at(ripm));
        cntr=1;//speed up
    }else if(cntr==0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)){
        richtung=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
        geschwindigkeit=0.36;
    }
                        
    if (pseed->species==1){
        maxentfernung = (geschwindigkeit*0.75*pseed->elternheight*0.01/(parameter[0].SeedDescentg));
    }else if (pseed->species==2){
        maxentfernung = (parameter[0].SeedTravelBreezes*0.75*pseed->elternheight*0.01/(parameter[0].SeedDescents));
    }                                        

    if(parameter[0].dispersalmode==5){
        entfernung= getEntfernung(maxentfernung,rn);                                       
    }else if(parameter[0].dispersalmode!=5){
        entfernung=getEntfernung(maxentfernung,rn);
    }
    
    dy=sin(richtung)*entfernung;
    dx=cos(richtung)*entfernung;
    wdir.clear();
    wspd.clear();      
}