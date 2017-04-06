

//void BefrWahrsch(double x, double y, vector<std::list<Tree*> >::iterator world_positon_b, int jahr, int &pName, int &cpSNPs1, int &cpSNPs2)
void BefrWahrsch(double x, double y, vector<std::list<Tree*> >::iterator world_positon_b, int jahr, vector<int> &pName, vector<int> &cpSNPs1, vector<int> &cpSNPs2)
{
 int year=jahr-1;
 list<Tree*>& tree_list = *world_positon_b;

  double  richtung=0.0,mwrichtung=0.0;
  double  geschwindigkeit=0.0;//,lambda=0.67;
  int    findyr1=2,findyr2=-100;
  string filename;
  std::ostringstream ss;
  std::ostringstream outputstring;
  string output;
  string item;
  unsigned int    ripm=0,cntr=0,cntr2=0;
  //int zufyear=0;
  vector<double> wdir,wspd;
  
  vector<int> Vname;
  vector<int> SNP1,SNP2;
  
  double  p=0.0,kappa=0.0,phi=0.0,dr=0.0,dx=0.0,dy=0.0;
  double  I0kappa=0.0;
  // Simpson integration + elec424.wikia.com/wiki/Modified_Bessel_Functions
  
  double pe=0.01;//vg/u, beides in m/s
  double  C=parameter[0].GregoryC;
  double  m=parameter[0].Gregorym;//src: Microbiology of the Atmosphere, Gregory et al
  
  //just to be sure:
  Vname.clear();
  wdir.clear();
  wspd.clear();
  
  if(parameter[0].windsource!=0 && parameter[0].windsource!=4 && parameter[0].windsource!=5){
		//cout<<"older wind data is used now, winddir[0].size()="<<winddir[0].size()<<endl;
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
		for(unsigned int pos=0;pos<(unsigned int)winddir[i].size()-1;pos++)
				{		 
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
		 
		 //cout<<"cntr2= "<<cntr2<<endl;
		 //cout<<"new random integer: "<<(int)(0.5*(cntr +cntr2*(1-2*rand()/(RAND_MAX+1.0))))<<endl;
		 
		 richtung=wdir.at((int)(0.5*(cntr +cntr2*(1-2*rand()/(RAND_MAX+1.0)))));
		 mwrichtung/=cntr2;//mean value!
		 geschwindigkeit/=cntr2;
		 
		//cout<<"ripm and cntr2:  "<<ripm<<"\t"<<cntr2<<endl;
			
		 //for(unsigned int i=ripm;i<(unsigned int)(ripm+cntr2);i++){
// 		  if(wspd[i]>parameter[0].pollenthreshold){
		   //kappa+=((M_PI/180*wdir.at(i)-mwrichtung)*(M_PI/180*wdir.at(i)-mwrichtung));
// 		   lambda+=(wspd[i]-geschwindigkeit)*(wspd[i]-geschwindigkeit);
// 		  }//pollenthreshold-> not tested
		 //}
		 		 
		 //pe=(parameter[0].pollenfall+parameter[0].pollenauftr)/geschwindigkeit; //??
		 pe=parameter[0].pollenfall/geschwindigkeit;
		 
		 if(kappa==0.0){kappa=pow((parameter[0].pollenrichtungsvarianz *M_PI/180),2);cntr2=1;}
		 
		 //inverse kappa afterwards 
		
		 kappa=(double)cntr2/kappa;
		 
 		 }else if((cntr==0 && (parameter[0].windsource==1 || parameter[0].windsource==2 || parameter[0].windsource==3)) or parameter[0].windsource==0){
			 //cout<<"no data is used now\n";
		   richtung=0.0+((double)(2*M_PI)*rand()/(RAND_MAX+1.0));
		   geschwindigkeit=0.2777;//10 km/h
			}
		
			wdir.clear();
			wspd.clear();
		 
		 I0kappa=0.16666*(exp(kappa) +4.0+exp(-1.0*kappa));
// I0kappa=0.16666*(exp(kappa) + 4.0*exp(-0.0*kappa)+exp(-1.0*kappa));
//  		 cout<<I0kappa<<endl;
//Ws vs W*? also Fall- vs Diffusionsgeschwindigkeit für die Berechnung des maximalen abstands (D=entfvorfaktor*u*H/W)
	
	//p+=exp((-lambda*dr+kappa*cos(phi-richtung)))*sqrt(lambda)/(sqrt(2*(M_PI*dr)**3)*I(0,kappa)); 
	//I=modified bessel function.. kappa=variance of direction?
	//Das entspricht (summiert) jetzt g aus tufto, engen,hindar. Es wird ja ohnehin dr eingelesen
		
 	for (list<Tree*>::iterator posb = tree_list.begin(); posb != tree_list.end(); )
 		{
			pTree_copy= *posb;
			
			if(pTree_copy->cone!=0){//if the pollinating tree has cones
				
 			dx=(pTree_copy->xcoo)-x; 
			dy=(pTree_copy->ycoo)-y; 
 			dr=sqrt(dx*dx+dy*dy);
			
			
			
 			if((dr!=0)){
 			  phi=atan2(dx,dy);//normalized to 0=west
 			}else{phi=richtung+0.5*M_PI;}//makes self-pollination less probable ~1/I0(kappa)
			
			cout<<"dx= "<<dx<<"\n dy ="<<dy<<"\n phi="<<phi<<
			"\n phi-richtung/pi= "<<(phi-richtung)/M_PI<<
			"\n p_phi="<<exp(kappa*cos(phi-richtung))/(2*I0kappa)<<endl;
 			
   			p=exp(kappa*cos((phi-richtung)))/(2*I0kappa)*(exp(-2*pe*pow(dr,1-0.5*m)/(sqrt(M_PI)*C*(1-0.5*m)))) ;
			//f(dr) aus Microbiology of the atmosphere, p(phi) ist von-Mises-Distribution

     		
 			//cout<<"r="<<dr<<endl;
  			//cout<<"p="<<p<<endl;//->hohe Wahrscheinlichkeitswerte auch für wenig korrelierte Winkel bedeutet Zufallsanteil ist zu hoch!!!!!
			
 			if(rand()>p*RAND_MAX){
  			////more likely, p is usually very small
			//Vname.push_back(0);
			++posb;
 			
				//cout<<"p>0.3\n";
			//	pName.push_back(pTree_copy->name);
			//	cpSNPs1.push_back(pTree_copy->cpSNP[0]);
			//	cpSNPs2.push_back(pTree_copy->cpSNP[1]);
			//	++posb; 
			}else{
				//cout<<"phi-wdir="<<phi-richtung<<endl;
				pName.push_back(pTree_copy->name);
				//cpSNPs1.push_back(pTree_copy->cpSNP[0]);
				//cpSNPs2.push_back(pTree_copy->cpSNP[1]);
				++posb; 
				
				
				if(((year==1989)||(year==1998)||(year==1987)||(year==1997)) &&(dr>0)&&(rand()<0.0001*RAND_MAX)){
				//print data in the most probable cases:
				FILE *fdir;
				outputstring<<year;
				output="windgen_"+outputstring.str()+"_.txt";
				fdir=fopen(output.c_str(),"a+");
				fprintf(fdir,"%f \t %f\n",dr,phi);
				fclose(fdir);
				outputstring.str("");
				outputstring.clear();
				}
			}
			}else{++posb;}
		}
		cntr=0;cntr2=0;
}