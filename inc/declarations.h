#pragma once

#include <vector>

#include "structures.h"

// plot dimensions rows==y, cols==x
constexpr unsigned int treerows = 4020;  // 30m_Ilirney_647902x7481367m
constexpr unsigned int treecols = 5010;
// constexpr unsigned int treerows = 14010;  // 30m_Ilirney_653902x7489357m
// constexpr unsigned int treecols = 11020;
// constexpr unsigned int treerows = 18210;  // 30m_Ilirney_x635418-652338m_y7472396-7490606m
// constexpr unsigned int treecols = 16920;
// constexpr unsigned int treerows = 14220;  // 30m_Ilirney_x641658-649518m_y7476056-7490276m
// constexpr unsigned int treecols = 7860;
// constexpr unsigned int treerows = 14310;  // 30m_Ilirney_x641989-649489m_y7476026-7490336m
// constexpr unsigned int treecols = 7500;

// parameter input
constexpr unsigned int stringlengthmax = 255;
constexpr char* divisionsign = "=;";

extern Parameter parameter[1];
