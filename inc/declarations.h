#pragma once

// plot dimensions rows==y, cols==x
// constexpr unsigned int treerows = 4020;  // 30m_Ilirney_647902x7481367m
// constexpr unsigned int treecols = 5010;
// constexpr unsigned int treerows = 14010;  // 30m_Ilirney_653902x7489357m
// constexpr unsigned int treecols = 11020;
// constexpr unsigned int treerows = 18210;  // 30m_Ilirney_x635418-652338m_y7472396-7490606m
// constexpr unsigned int treecols = 16920;
// constexpr unsigned int treerows = 14220;  // 30m_Ilirney_x641658-649518m_y7476056-7490276m
// constexpr unsigned int treecols = 7860;
// constexpr unsigned int treerows = 14310;  // 30m_Ilirney_x641989-649489m_y7476026-7490336m
// constexpr unsigned int treecols = 7500;
// constexpr unsigned int treerows = 24720; //30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m
// constexpr unsigned int treecols = 18000;
// constexpr unsigned int treerows = 10080; //30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m
// constexpr unsigned int treecols = 9990;
// constexpr unsigned int treerows = 9990; //Khamra 10x10km "...x271389.9-281379.9m_y6651969.54-6661959.54m.csv"; x=9990, y=9990
// constexpr unsigned int treecols = 9990;
// constexpr unsigned int treerows = 450;	//Khamra 500x500m "...x275889.94-276339.94m_y6656469.44-6656919.44m.csv"; x=450, y=4500
// constexpr unsigned int treecols = 450;
// constexpr unsigned int treerows = 990;	//Khamra 1000x1000m "...x272300.874-273290.874m_y6659779.814-6660769.814m.csv"; x=990, y=990
// constexpr unsigned int treecols = 990;
// constexpr unsigned int treerows = 1980;	//Khamra 2000x2000m "...x271300.874-273280.874m_y6658779.814-6660759.814m.csv"; x=1980, y=1980 ### PREVIOUS USE ###
// constexpr unsigned int treecols = 1980;
// constexpr unsigned int treerows = 9990;	//Satagay 9990x9990m "...x242511.6202-252501.6202m_y6999138.4447-7009128.4447m.csv"; x=9990, y=9990
// constexpr unsigned int treecols = 9990;
// constexpr unsigned int treerows = 1980;	//Satagay 2000x2000m "...x245684-247664m_y7000974-7002954m.csv"; x=1980, y=1980 -> Works
// constexpr unsigned int treecols = 1980;
// constexpr unsigned int treerows = 180;	//Satagay 200x200m "...x246883.185256-247063.185256m_y7004613.696481-7004793.696481m.csv"; x=180, y=180 -> Works
// constexpr unsigned int treecols = 180;
// constexpr unsigned int treerows = 19980;	//Satagay 20000x200000m (10km buffer) "...x237506.6203-257486.6203m_y6994133.4448-7014113.4448m.csv"; x=19980, y=19980 -> Works
// constexpr unsigned int treecols = 19980;
constexpr unsigned int treerows = 990;	//Satagay 990x990m (1km buffer) "...x246083.210065-247073.210065m_y7003813.795713-7004803.795713m.csv"; x=990, y=990 -> Works
constexpr unsigned int treecols = 990;
// constexpr unsigned int treerows = 14250; //30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m
// constexpr unsigned int treecols = 11430;
// constexpr unsigned int treerows = 19710; //30m_Ilirney_x640008.2-649998.2m_y7475006-7494716m
// constexpr unsigned int treecols = 9990;
// constexpr unsigned int treerows = 2520;  // 30m_SpasskayaPad_x529631-532151m_y6897789-6900309m
// constexpr unsigned int treecols = 2520;
// constexpr unsigned int treerows = 270;  // 30m_x531881-532151m_y6900039-6900309m
// constexpr unsigned int treecols = 270;
// constexpr unsigned int treerows = 510;  // 30m_SpasskayaPad_x531881-532391m_y6900039-6900549m
// constexpr unsigned int treecols = 510;
// constexpr unsigned int treerows = 2520;  // 30m_SpasskayaPad_x530881-533401m_y6899039-6901559m
// constexpr unsigned int treecols = 2520;
// constexpr unsigned int treerows = 5010;  // 30m_SpasskayaPad_x529631-534641m_y6897789-6902799m
// constexpr unsigned int treecols = 5010;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr const char* divisionsign = "=;";

class Parameter;
extern Parameter parameter[1];
class Speciestraits;
extern Speciestraits speciestrait[99];


