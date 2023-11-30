#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

double mixrand(double mu1,double std1,double mu2,double std2, double Lbound, double Rbound)
{
	double R1,R2,z1=-1,z2=-1;
	RandomNumber<double> uniform(0, 1);
do{
	R1=uniform.draw() ;
	R2=uniform.draw() ;
	z1=(sqrt(-2.0*log(R1))*cos(2.0*M_PI*R2))*std1+mu1;
	z2=(sqrt(-2.0*log(R1))*sin(2.0*M_PI*R2))*std2+mu2;
	R1=((uniform.draw() > 0.5)? z1:z2);
}while(R1<=(Lbound) || R1>=(Rbound));
//cout<<z1<<" "<<z2<<endl;
return R1;
}


double normrand(double mu,double std,double Lbound, double Rbound)
{
	double R1,R2,z1=-1;
	RandomNumber<double> uniform(0, 1);
do{
	R1=uniform.draw() ;
	R2=uniform.draw() ;
	z1=(sqrt(-2.0*log(R1))*cos(2.0*M_PI*R2))*std+mu;
}while(z1<=(Lbound) || z1>=(Rbound));
return z1;	
}

double averand(double mu1, double mu2, double weight,double std,double Lbound, double Rbound)
{
	double R1,R2,z1=-1,mu;
	RandomNumber<double> uniform(0, 1);
do{
	mu=(mu1*(1-weight))+(mu2*weight);
	R1=uniform.draw() ;
	R2=uniform.draw() ;
	z1=(sqrt(-2.0*log(R1))*cos(2.0*M_PI*R2))*std+mu;
}while(z1<=(Lbound) || z1>=(Rbound));
return z1;	
}

void mixvector(const std::array<unsigned int,24> &vectre, const std::array<unsigned int,24> &vecpol, std::array<unsigned int,24>& vecseed)
// vector <unsigned int> mixvector(vector<unsigned int> vectre,vector<unsigned int> vecpol)
{
	// vector<unsigned int> newvector(24, 0);
	//RandomNumber<int> uniformnum(1, 2);  ///this function is for double not int. int function ToDo
	RandomNumber<double> uniform(0, 1);
for(unsigned int neutralcounter=0 ;neutralcounter < 24 ;neutralcounter++){
	double randecide=uniform.draw();
	if (neutralcounter %2 == 0 ){
	
		if (randecide<0.5){
				vecseed[neutralcounter]=vectre[neutralcounter];
		}
		else {
			vecseed[neutralcounter]=vectre[neutralcounter+1];
		}
	}
	else{
	
		if (randecide<0.5){
				vecseed[neutralcounter]=vecpol[neutralcounter];
		}
		else {
			vecseed[neutralcounter]=vecpol[neutralcounter-1];
		}
	}
}

// return newvector;
}


// TODO temporary here
extern vector<double> wdir, wspd;
void Pollinationprobability(double x, double y,struct Parameter *parameter,
 //vector<std::list<Tree*> >::iterator world_positon_b,
 vector<vector<Pollengrid> >::iterator world_positon_p,
 double direction,double velocity,unsigned int ripm,unsigned int cntr,double p,double kappa,double phi,double dr,double dx,double dy,double I0kappa,double pe,double C,double m,
 vector<int> &pName, vector<double>  &thdpthinfl, vector<double>  &droghtinfl, vector<double>  &selvinginfl, vector<unsigned int>& neutralinfl, vector<int>  &fathname) {

	RandomNumber<double> uniform(0, 1);
	
	vector<Pollengrid>& pollen_list = *world_positon_p;
  
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
	droghtinfl.clear();
	droghtinfl.shrink_to_fit();
	selvinginfl.clear();
	selvinginfl.shrink_to_fit();
	neutralinfl.clear();
	neutralinfl.shrink_to_fit();
	fathname.clear();
	fathname.shrink_to_fit();
  		
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
	   direction=2*M_PI*uniform.draw(); 
	   velocity=2.777;
	}

	if(cntr!=0 && (parameter[0].windsource==1))
	{
		ripm=(int)(0.5*wdir.size() + wdir.size()/6 *(1-2*uniform.draw()-0.000001));
	
		direction=	M_PI * ( wdir.at(ripm) / 180 );
		velocity=wspd.at(ripm);
	}
	else if((cntr==0 && (parameter[0].windsource==1) || parameter[0].windsource==0))
	{
	   direction=0.0+((double)(2*M_PI)*(uniform.draw()-0.000001));
	   velocity=2.777;//10 km/h
	}
	
	pe=parameter[0].pollenfall/velocity;
	
	
	// Simpson integration + elec424.wikia.com/wiki/Modified_Bessel_Functions:
	I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa));
	
	
	for (unsigned int pos = 0; pos < pollen_list.size(); ++pos) {
//		Old version: use pTree_copy and the above for loop, in order to check every tree for genetic lineage.	
		//auto& pTree_copy = tree_list[pos];
		auto& pPollengrid = pollen_list[pos];
		int tresize=pPollengrid.Treenames.size();
		// only if the pollinating tree has cones (see mortality.cpp)!
		if((tresize)!=0)
		{
	
			dx=(pPollengrid.xcoo)-(x/1000); 
			dy=(pPollengrid.ycoo)-(y/1000); 
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
			
			if(uniform.draw()>p)
			{
				++pos;
			}
			else
			{
				
				pName.push_back(pPollengrid.Number);
				
				//HAVE ONE PARTICULAR TRAIT VALUE FOR EVERY GRID CELL!
				
				//INHERITING SEED WEIGHT
				thdpthinfl.push_back(pPollengrid.seedweight);
				droghtinfl.push_back(pPollengrid.droughtresist);
				// numberinfl.push_back(pPollengrid.seednumber);
				
				selvinginfl.push_back(pPollengrid.selving);
				// printf("Pnetralsize: %ld ",pPollengrid.neutralmarkers.size());
				
				
				
				double randpreneutral=uniform.draw();
				int neutralran= floor (randpreneutral *((pPollengrid.neutralmarkers.size()/24)-0.00001));
				for (unsigned int i=0+(24*neutralran); i < 24+(24*neutralran); i++ ){
					neutralinfl.push_back( pPollengrid.neutralmarkers[i]);
					// printf(" pPollengrid.neut: %d ", pPollengrid.neutralmarkers[i] );
				}		
				// neutralinfl.push_back(pPollengrid.neutralmarkers);
				fathname.push_back(pPollengrid.name);
				//thdpthinflvar.push_back(pPollengrid->seedweightvar);
				
				
				++pos; 
			}
		}
		else
		{
			++pos;
		}
	}
}

double getEntfernung(double D, double ratiorn_help) {
    double entf_help;

    if (parameter[0].dispersalmode == 0) {  // random in certain distance
        entf_help = ((ratiorn_help - 1) * 100.0) / (0.05 - 1);
    } else if (parameter[0].dispersalmode == 1) {  // neg. exponential
        entf_help = parameter[0].distanceratio * ((log(ratiorn_help) / (-0.2)) / 0.16);
    } else if (parameter[0].dispersalmode == 2) {  // fat tailed/power law
        double fatalpha = 0.5;
        entf_help = parameter[0].distanceratio * pow(ratiorn_help, (-1 * (1 + fatalpha)));
    } else if (parameter[0].dispersalmode == 3) {  // gaussian
        double gaussweite = D, gaussmaxh = 1, gaussposcenter = 0;
        entf_help = parameter[0].distanceratio * sqrt(2 * pow(gaussweite, 2) * (-1 * log(ratiorn_help / gaussmaxh))) + gaussposcenter;
    } else if (parameter[0].dispersalmode == 4 || parameter[0].dispersalmode == 5) {  // gaussian combined with fat tailed:
        double gaussfatratio = 2.0;
        double gaussweite = D, gaussmaxh = 1, gaussposcenter = 0;
        double fatalpha = 0.5;
        entf_help = (0.5
                     * (gaussfatratio * (sqrt(2 * pow(gaussweite, 2) * (-1 * log(ratiorn_help / gaussmaxh))) + gaussposcenter)
                        + (1 / gaussfatratio) * parameter[0].distanceratio * (pow(ratiorn_help, (-1 * (1 + fatalpha))))));

    } else {
        printf("LAVESI was stopped\n\n");
        printf("... choice of dispersal mode (= %d) not available!\n", parameter[0].dispersalmode);
        exit(1);
    }

    return (entf_help);
}

void Seedwinddispersal(double rn, double& dx, double& dy, double& windspeed, double& winddirection, double parhei, int seedspec, double randomnumberwind) {
    int ripm = 0;
    double dispersaldistance = 0;
    double maxdispersaldistance = 0;
    double direction = 0.0;
    double velocity = 0.0;

    if (parameter[0].windsource == 1) {  // EraInterim
        // choose a month between May and September:
        ripm = (int)(0.5 * wdir.size() + wdir.size() / 6 * (1 - 2 * randomnumberwind));
        direction = M_PI * (wdir.at(ripm) / 180);
        velocity = (wspd.at(ripm));
    } else {  // random
        direction = 0.0 + ((double)(2 * M_PI) * randomnumberwind);
        velocity = 2.7777;
    }

    maxdispersaldistance = (velocity * 0.75 * parhei * 0.01 / (speciestrait[seedspec].seeddescent));

    dispersaldistance = getEntfernung(maxdispersaldistance, rn);

    // set return variables
    dy = cos(direction) * dispersaldistance;
    dx = sin(direction) * dispersaldistance;
    windspeed = velocity;
    winddirection = direction;
}
