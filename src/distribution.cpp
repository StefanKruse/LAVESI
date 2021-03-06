using namespace std;

/***********************************************************************************
 * AUTHORS: Gerdes, Alexander; Kath, Nadja; Kruse, Stefan
 * 
 * NAME:
 * distribution.cpp
 * 
 * CONTENTS:
 * This .cpp file contains the three functions used by LaVeSi to realize wind driven
 * seed- and pollen dispersal:
 * a) Pollinationprobability:
 *    A function returning a vector of the most probable trees to pollinate a tree 
 *    at the position x,y
 * 
 * b) getEntfernung:
 *    A function calculating and returning the traveling distance for:
 * 
 * c) Seedwinddispersal:
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
 * POSSIBLE OUTPUT DATA:
 *  a) a vector of the names of the most probable trees to pollinate another tree,
 *     earlier versions: conceptual preparation for later use: cpSNPs-> genetic data
 *  b) the flight distance of a dispersed seed
 *  c) the absolute position of a dispersed seed in x,y-coordinates
 * 
 ************************************************************************************/
 
void Pollinationprobability(double x, double y,struct Parameter *parameter, vector<std::list<Tree*> >::iterator world_positon_b, double direction,double velocity,unsigned int ripm,unsigned int cntr,double p,double kappa,double phi,double dr,double dx,double dy,double I0kappa,double pe,double C,double m, vector<int> &pName, vector<double>  &thdpthinfl, int outputtreesiter)
{
  list<Tree*>& tree_list = *world_positon_b;
  direction=0.0;
  velocity=0.0;
  ripm=0;
  cntr=0;
  p=0.0;
  kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);
  phi=0.0;
  dr=0.0;
  dx=0.0;
  dy=0.0;
  I0kappa=0.0;
  pe=0.01;
  C=parameter[0].pollengregoryc;
  m=parameter[0].pollengregorym;
		
  pName.clear();
  pName.shrink_to_fit();
  thdpthinfl.clear();
  thdpthinfl.shrink_to_fit();
  		
	if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5)
	{
		cntr=1;
	}
	else if(parameter[0].windsource==4)
	{
		direction=	M_PI * ( 0 / 180 );
		velocity=2.777;
	}
	else if(parameter[0].windsource==5)
	{
		direction=	M_PI * ( 180 / 180 );
		velocity=2.777;
	}
	else if(parameter[0].windsource==0)
	{
	   direction=2*M_PI*(rand()/RAND_MAX); 
	   velocity=2.777;
	}

	if(cntr!=0 && (parameter[0].windsource==1))
	{
		ripm=(int)(0.5*wdir.size() + wdir.size()/6 *(1-2*rand()/(RAND_MAX+1.0)));
	
		direction=	M_PI * ( wdir.at(ripm) / 180 );
		velocity=wspd.at(ripm);
	}
	else if((cntr==0 && (parameter[0].windsource==1) || parameter[0].windsource==0))
	{
	   direction=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
	   velocity=2.777;//10 km/h
	}
	
	pe=parameter[0].pollenfall/velocity;
	 
	// Simpson integration + elec424.wikia.com/wiki/Modified_Bessel_Functions:
	I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa));
			 
	for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); )
	{
		pTree_copy= *posb;
		
		// only if the pollinating tree has cones:
		if(pTree_copy->cone!=0)
		{
			
			dx=(pTree_copy->xcoo)-x; 
			dy=(pTree_copy->ycoo)-y; 
			dr=sqrt(dx*dx+dy*dy);
			
			if((dr!=0))
			{
			  phi=atan2(dx,dy);
			}
			else
			{
				phi=direction+0.5*M_PI;// makes self-pollination less probable: p~1/I0(kappa)
			}
			
			p=exp(kappa*(cos(phi-direction)*-1))/(2*I0kappa)*(exp(-2*pe*pow(dr,1-0.5*m)/(sqrt(M_PI)*C*(1-0.5*m))));
			// f(dr) based on Microbiology of the atmosphere, p(phi) von Mises distribution
			
			if(rand()>p*RAND_MAX)
			{
				++posb;
			}
			else
			{
				pName.push_back(pTree_copy->name);
				thdpthinfl.push_back(100);
				
				++posb; 
			}
			
			// data output for pollen flight analysis
			if(parameter[0].pollination==1 && parameter[0].omp_num_threads==1 && outputtreesiter<=5 && parameter[0].ivort>=1046)
			{
				FILE *fdir;
				char filenamechar[25];
				sprintf(filenamechar, "IVORT%.4d_REP%.3d", parameter[0].ivort, parameter[0].repeati);
				string output="output/windgen_pollination_total_" + string(filenamechar) + ".txt";
				fdir=fopen(output.c_str(),"a+");
				fprintf(fdir,"%10.20f \t %10.20f \t %10.20f \t %d \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t \n", dr, phi, p, pTree_copy->name, pTree_copy->xcoo, pTree_copy->ycoo, x, y);
				fclose(fdir);
			}
		}
		else
		{
			++posb;
		}
	}
}


double getEntfernung(double D, double ratiorn_help)	
{
	double entf_help;
	
	if (parameter[0].dispersalmode==0)
	{ // random in certain distance
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
	{ // gaussian combined with fat tailed:
		double gaussfatratio=2.0;
		double gaussweite=D, gaussmaxh=1, gaussposcenter=0;	
		double fatalpha=0.5;
		entf_help=( 0.5*( gaussfatratio*(sqrt( 2*pow(gaussweite,2)*(-1*log(ratiorn_help/gaussmaxh)) )+gaussposcenter)+(1/gaussfatratio)*parameter[0].distanceratio * (pow(ratiorn_help, (-1*(1+fatalpha)) )) ) );

	}
	else 
	{
		printf("LaVeSi was stopped\n\n");
		printf("... choice of dispersal mode (= %d) not available!\n", parameter[0].dispersalmode);
		exit(1);
	}

	return(entf_help);
}



void Seedwinddispersal(double rn, double& dx, double& dy, double &windspeed, double &winddirection, double parhei, int seedspec)
{
    int cntr=1,ripm=0;
    double dispersaldistance = 0;
    double maxdispersaldistance = 0;
    double direction=0.0;
    double velocity=0.0;
	
    if(parameter[0].windsource==4)
	{
        direction=	M_PI * ( 0 / 180 );
        cntr=0;//speed up
		velocity=2.7777;
    }
	else if(parameter[0].windsource==5)
	{
        direction=	M_PI * ( 180 / 180 );
        cntr=0;//speed up
		velocity=2.7777;
    }
	else if(cntr!=0 && (parameter[0].windsource==1))
	{
        //choose a month between May and September:
        ripm=(int)(0.5*wdir.size() + wdir.size()/6 *(1-2*rand()/(RAND_MAX+1.0)));
        direction = M_PI * ( wdir.at(ripm) / 180 );
        velocity=(wspd.at(ripm));
        cntr=1;//speed up
    }
	else if((parameter[0].windsource==0)||(cntr==0 && (parameter[0].windsource==1 )))
	{
        direction=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
        velocity=2.7777;
    }
	
    if (seedspec==1)
	{
        maxdispersaldistance = (velocity*0.75*parhei*0.01/(parameter[0].seeddescentg));
    }
	else if (seedspec==2)
	{
        maxdispersaldistance = (parameter[0].seedtravelbreezes*0.75*parhei*0.01/(parameter[0].seeddescents));     
    }                                       

    if(parameter[0].dispersalmode==5)
	{
        dispersaldistance= getEntfernung(maxdispersaldistance,rn);                                       
    }
	else if(parameter[0].dispersalmode!=5)
	{
        dispersaldistance=getEntfernung(maxdispersaldistance,rn);
    }

    dy=cos(direction)*dispersaldistance;
    dx=sin(direction)*dispersaldistance;
	
	windspeed=velocity;
	winddirection=direction;    
}
