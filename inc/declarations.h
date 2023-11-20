#pragma once

// plot dimensions rows==y, cols==x
constexpr unsigned int treerows = 990;	//Satagay 990x990m "...x246083.210065-247073.210065m_y7003813.795713-7004803.795713m.csv"; x=990, y=990
constexpr unsigned int treecols = 990;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr const char* divisionsign = "=;";

class Parameter;
extern Parameter parameter[1];
class Speciestraits;
extern Speciestraits speciestrait[99];


