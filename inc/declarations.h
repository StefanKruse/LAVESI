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
constexpr unsigned int treerows = 990;	//Khamra 1000x1000m "...x272300.874-273290.874m_y6659779.814-6660769.814m.csv"; x=990, y=990
constexpr unsigned int treecols = 990;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr const char* divisionsign = "=;";

class Parameter;
extern Parameter parameter[1];
