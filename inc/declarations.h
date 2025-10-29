#pragma once

// plot dimensions rows==y, cols==x
// constexpr unsigned int treerows = 990;	//Satagay 990x990m "...x246083.210065-247073.210065m_y7003813.795713-7004803.795713m.csv"; x=990, y=990
// constexpr unsigned int treecols = 990;
constexpr unsigned int treerows = 1980;	//Lake 449 1980x1980m "...x580266.985221-582246.985221m_y6845968.841241-6847948.841241m.csv"; x=1980, y=1980
constexpr unsigned int treecols = 1980;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr const char* divisionsign = "=;";

class Parameter;
extern Parameter parameter[1];
class Speciestraits;
extern Speciestraits speciestrait[99];


