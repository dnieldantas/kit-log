#include "Data.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

typedef struct Solution
{
    std::vector<int> sequence = {0,0};
    double cost = 0.0;
} Solution;

typedef struct Insertion_info
{
    int inserted_node = 0;
    int removed_edge = 0;
    double cost = 0.0;
} Insertion_info;

