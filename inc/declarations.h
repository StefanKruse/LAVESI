#pragma once

#include <list>
#include <vector>
#include "structures.h"

using namespace std;

// plot dimensions rows==y, cols==x
// const int treerows = 13260;
// const int treecols = 31440;
	// TODO: parameter file implememtation of area based on filles to read in: x 13260 x y 31440 m for full Lake Ilirney site
// const int treerows = 12249;
// const int treecols = 11615;
// const int treerows = 4020;
// const int treerows = 8010;
// const int treecols = 5010;
// const int treerows = 10020;//dem_30m_Ilirney_652912x7487367m
// const int treecols = 10020;
const int treerows = 14010;//dem_30m_Ilirney_653902x7489357m
const int treecols = 11020;

// parameter input
const int stringlengthmax = 255;
char divisionsign[] = "=;";
struct Parameter parameter[1];
