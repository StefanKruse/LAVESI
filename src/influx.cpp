/*
	function has to be called once a year (-> implementation in main.cpp?)
	if a year is visited N times-> mean value? how do i know the year has been visited before?
	
	Bastian: appr. 0.5 pollen/(cm^2*yr)
	
*/

/*
#include "libraries.h"
#include "strukturen.h"
#include "deklarationen.h"
*/



void influx(int treerows, int treecols,  struct Parameter *parameter, int year, vector<list<Tree*> > &world_tree_list , vector<vector<double> > &vegetationtype){
	int i, j;
	int aktort=0;
	//vector<double> influx;
	double influxrate=0.0;
	//list<Tree*>& tree_list = *world_positon_b;
 
  double  richtung=0.0,A=0.0;
  double  geschwindigkeit=0.0;//,lambda=0.67;
  int    findyr1=2,findyr2=-100;
  int pickyear=0;
  string filename;
  std::ostringstream ss;
  string item;
  unsigned int    ripm=0,cntr=0,cntr2=0,treecntr=0;
  //int zufyear=0;
  vector<double> wdir,wspd;
  vector<int> Vname;
  double  p[10*treecols-1],kappa=0.0,phi=0.0,dr=0.0,dx=0.0,dy=0.0;
  double  I0kappa=0.0, x[10*treecols-1], y[10*treecols-1];//0.16666*(exp(kappa) + 4.0*exp(-0.0*kappa)+exp(-1.0*kappa))/;
  //nach Simpsonintegration und elec424.wikia.com/wiki/Modified_Bessel_Functions
  
//EINHEITEN BEACHTEN, hier ist häufig cm
  double pe=0.01;//vg/u, beides in m/s
  double  C=0.6*0.56234;
  double  m=1.25;//Microbiology of the Atmosphere, Gregory et al
  
  
  		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;
		//world_positon_b_copy-dekl.??
		aktort++;
  
  
  for(j=0;j<10*treecols;j++){y[j]=j*0.1;p[j]=0.0;}
  
	char datname[20];
	FILE *fp;
	
	sprintf(datname,"pollDist_%d.txt",year);
	
	fp=fopen(datname,"w+");
				
	wdir.clear();wspd.clear();
	
	 if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5){
		
			if(parameter[0].windsource==1){
		    findyr1=1947;findyr2=2012;
		    }else if(parameter[0].windsource==2){
		    findyr1=1979;findyr2=2012;
		    }else if(parameter[0].windsource==3){
		    findyr1=1959;findyr2=2002;
			}
			
		if((year<findyr1) or (year>findyr2)){pickyear=(findyr1+(int)(rand()/RAND_MAX*(findyr2-findyr1)));}else{pickyear=year;}
		
		for(unsigned int i=0;i<(unsigned int)globalyears.size();i++){
		if(globalyears[i]==pickyear){
		for(unsigned int pos=0;pos<(unsigned int)winddir[i].size();pos++)
		{		 
		 wdir.push_back(M_PI*winddir[i][pos]/180);
		 wspd.push_back(windspd[i][pos]);
		}}}
		cntr=2*wdir.size();
		
		}else if(parameter[0].windsource==4){
		 richtung=M_PI/180 *270;
// 		  kappa=pow(M_PI/(30*180),2);
		}else if(parameter[0].windsource==5){
		  richtung=M_PI/180 *90;
// 		   kappa=pow(M_PI/(30*180),2);
		}else if(parameter[0].windsource==0){
		richtung=M_PI*(rand()/RAND_MAX); 
		}
		
		
		if(cntr!=0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)){
		 ripm=((unsigned int)floor(0.25*cntr - 2* 0.0833333*cntr));//etwa Anfang Mai
		 //Mittelwertberechnung
		 for(unsigned int i=ripm;i<floor(ripm+0.5*0.58*cntr);i++){//bis etwa Ende August
// 		   if(wspd[i]>parameter[0].pollenthreshold){
		  geschwindigkeit+=wspd.at(i);
		  richtung+=wdir.at(i);
		  cntr2++;		  
// 		 }//pollenthreshold-> not tested
		 }
		 
		 richtung/=cntr2;//mean value!
		 geschwindigkeit/=cntr2;
		 
		 //cout<<"ripm and cntr2:  "<<ripm<<"\t"<<cntr2<<endl;
			
		 for(unsigned int i=ripm;i<(unsigned int)(ripm+cntr2);i++){
// 		  if(wspd[i]>parameter[0].pollenthreshold){
		   kappa+=((M_PI/180*wdir.at(i)-richtung)*(M_PI/180*wdir.at(i)-richtung));//
// 		   lambda+=(wspd[i]-geschwindigkeit)*(wspd[i]-geschwindigkeit);
// 		  }//pollenthreshold-> not tested
		 }
		 		 
		 //pe=(parameter[0].pollenfall+parameter[0].pollenauftr)/geschwindigkeit; //??
		 pe=0.126/geschwindigkeit;
		 
		 if(kappa==0.0){kappa=pow((parameter[0].pollenrichtungsvarianz*180/M_PI),2);cntr2=1;}
		 
		 kappa=(double)cntr2/kappa;
		 
 		 }else if((cntr==0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3))|| parameter[0].windsource==0){
			 cout<<"no data is used now\n";
		   richtung=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
		   geschwindigkeit=0.2777;
		}
		
			wdir.clear();
			wspd.clear();
		 
		 I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa));
	
	
	
	treecntr=0;
	for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); ){
			//for(j=0;j<10*treecols;j++) {p[j]=0;}
			
			pTree= *posb;
		
		if(pTree->cone!=0){
		treecntr++;
	//cout<<"iteriere über gesamte fläche\n";
	
	for(j=0;j<10*treecols;j++){
		
	//for(i=0;i<10*treerows;i++){
		i=0;
		x[0]=0.5*treerows;

		
		//if(((parameter[0].defTreevert==1)&&(vegetationtype[i][j]==0.0)) or ((parameter[0].defTreevert==0)&&(i==floor(0.5*treerows))&&(j==floor(0.5*treecols))) ){
											//if no vegetationtype matrix is given (no lake)->  calc. pollen influx rates in center of plot
											//i= rather than i==-> defining i and j rather than comparing-> speed up
				//pollen productivity :=		distance weighted slope of the function: pollen percentages = slope*(plant abundance)+background pollen loading
				//										=0.16 +- 0.05 for larix pollen in lakes of the khatanga region... 
				//A+=pow(10,4);//seefläche in cm
			//treecntr=0;
	//cout<<"iteriere über alle trees\n";
 	
			//treecntr++;
			
			//cout<<pTree->name<<endl;
			
				//cout<<"cone==true\n";
				
 			dx=(pTree->xcoo) -x[i]; 	//-0.5/+0.5
			dy=(pTree->ycoo) -y[j];	//-0.5/+0.5
 			dr=sqrt(pow(dx,2)+pow(dy,2));
			
 			if((dx!=0)&&(dy!=0)){
 			  phi=atan(dx/dy);//
 			}else if((dx==0)&&(dy!=0)){
 			  phi=0.0;
 			}else if((dx!=0)&&(dy==0)){
 			  phi=M_PI*0.5;
 			}else{phi=richtung;}
 			
			//integrated spacial derivative of number of pollen in the air column above vegetationtype[i,j]==0
			//= the decline of total pollen number from the borders of vegtype==0.
   			p[j]+=exp(kappa*cos(phi-richtung))/(2*I0kappa)*(exp(-2*pe*pow(dr,1-0.5*m)/(sqrt(M_PI)*C*(1-0.5*m)))) ;
			
			//cout<<p<<endl;
			//p[j]/=treecntr;	
			
			//cout<<dx<<"\t"<<dy<<"\t"<<richtung<<"\t"<<phi<<"\t"<<kappa<<"\t"<<pe<<endl;
			//cout<<p<<endl;
			
 			//}else{		//cout<<"cone==false\n";
			//	++posb;p+=0;}
			//f(dr) aus Microbiology of the atmosphere, p(phi) ist von-Mises-Distribution
		
		//}
	}
			
			++posb;
		
		//}	
		//if(parameter[0].defTreevert==0){break;} 
	}else{++posb;}
	//if(parameter[0].defTreevert==0){break;} 
	//now: ABSOLUTE pollen distribution function in x direction!
}

			//Ziel: Größenordnung 0.5 pollen/(yr*cm^2)
			//->parameter der Pollenanzahl in parameter.txt, parametereinlesen und strukturen.h deklarieren.
			//09.05.17: change to this to a pollen distribution function in y-direction
			
			//influxrate=(p/A);
			for(j=0;j<10*treecols;j++) fprintf(fp,"%lf ; %lf \n",y[j],p[j]);

	fclose(fp);
} 