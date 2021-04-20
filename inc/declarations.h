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
// constexpr unsigned int treerows = 14250; //30m_Ilirney_x637008.2-655008.2m_y7469996-7494716m
// constexpr unsigned int treecols = 11430;
// constexpr unsigned int treerows = 19710; //30m_Ilirney_x640008.2-649998.2m_y7475006-7494716m
// constexpr unsigned int treecols = 9990;
constexpr unsigned int treerows = 2520;  // 30m_SpasskayaPad_x529631-532151m_y6897789-6900309m
constexpr unsigned int treecols = 2520;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr const char* divisionsign = "=;";

class Parameter;
extern Parameter parameter[1];
class Speciestraits;
extern Speciestraits speciestrait[99];


