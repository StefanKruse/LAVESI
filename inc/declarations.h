#pragma once

#include <list>
#include <vector>
#include "structures.h"

using namespace std;

// plot dimensions rows==y, cols==x
const int treerows = 14010;//dem_30m_Ilirney_653902x7489357m
const int treecols = 11020;
// const int treerows = 4020;//dem_30m_Ilirney_653902x7489357m
// const int treecols = 5010;

// parameter input
const int stringlengthmax = 255;
char divisionsign[] = "=;";
struct Parameter parameter[1];
