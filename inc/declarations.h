#pragma once

// plot dimensions rows==y, cols==x
constexpr unsigned int treerows = 5530;	
constexpr unsigned int treecols = 5720;
//constexpr unsigned int treerows = 100;	
//constexpr unsigned int treecols = 100;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr const char* divisionsign = "=;";

class Parameter;
extern Parameter parameter[1];
class Speciestraits;
extern Speciestraits speciestrait[99];


