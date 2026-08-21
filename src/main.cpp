#include "Data.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

typedef struct Solution
{
    std::vector<int> sequence;
    double cost;
} Solution;

typedef struct Insertion_info
{
    int inserted_node;
    int removed_edge;
    double cost;
} Insertion_info;

std::vector<Insertion_info> Insertion_cost_calculator(Solution &s, std::vector<int> &CL, Data &data){
    std::vector<Insertion_info> insertion_cost = std::vector<Insertion_info>((s.sequence.size() - 1) * CL.size());
    int l = 0;
    for (int a = 0; a < s.sequence.size()-1; a++){
        int i = s.sequence[a];
        int j = s.sequence[a+1];
        for (auto k : CL){
            insertion_cost[l].cost = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            insertion_cost[l].inserted_node = k;
            insertion_cost[l].removed_edge = a;
            l++;
        }
    }
    return insertion_cost;
}

void random_nodes(Solution &s, Data &data){
    while(s.sequence.size() < 5){
        int num = rand() % data.getDimension();
        if (std::find(s.sequence.begin(), s.sequence.end(), num) == s.sequence.end()){
            s.sequence.insert(s.sequence.begin()+1, num);
        }
    }
    // da p melhorar
    for (int i = 0; i < s.sequence.size()-1; i++){
        s.cost += data.getDistance(s.sequence[i], s.sequence[i+1]);
    }
}

std::vector<int> nodes_left(std::vector<int> sequence, Data &data){ // dá p melhorar isso aq
    std::vector<int> CL;
    for (int i = 0; i < data.getDimension(); i++){
        if (std::find(sequence.begin(), sequence.end(), i) == sequence.end()){
            CL.push_back(i);
        }
    }
    return CL;
}

void insert_solution(Solution &s, Insertion_info &insertion){
    s.sequence.insert(s.sequence.begin()+insertion.removed_edge+1, insertion.inserted_node);
    s.cost += insertion.cost;
}

void show_cost(Solution &s, Data &data){
    double cost = 0;
    for (int i = 0; i < s.sequence.size()-1; i++){
        cost += data.getDistance(s.sequence[i], s.sequence[i+1]);
    }
    std::cout << cost << std::endl;
}

Solution Construction(Data &data){
    Solution s;
    s.sequence = {0, 0};
    random_nodes(s, data);
    
    std::vector<int> CL = nodes_left(s.sequence, data);

    while (!CL.empty()){
        std::vector<Insertion_info> insertion_cost = Insertion_cost_calculator(s, CL, data);
        std::sort(insertion_cost.begin(), insertion_cost.end(), [](Insertion_info &a, Insertion_info &b){return a.cost < b.cost;});
        double alpha = (double) rand() / RAND_MAX;
        int selected = rand() % ((int) ceil(alpha * insertion_cost.size()));
        insert_solution(s, insertion_cost[selected]);
        CL = nodes_left(s.sequence, data);
    }
    return s;
}

/*void Local_search(Solution *s);
Solution Perturbation(Solution s);

Solution ILS(int max_iter, int max_iter_ils, Data &data){
    Solution best_of_all;
    best_of_all.cost = INFINITY;

    for(int i = 0; i < max_iter; i++){
        Solution s = Construction(data);
        Solution best = s;

        int iter_ils = 0;

        while (iter_ils <= max_iter_ils)
        {
            Local_search(&s);
            if (s.cost < best.cost){
                best = s;
                iter_ils = 0;
            }
            s = Perturbation(best);
            iter_ils++;
        }
        if (best.cost < best_of_all.cost){
            best_of_all = best;
        }
    }
    return best_of_all;
}*/

int main(int argc, char** argv) {
    std::srand(std::time(NULL));

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    std::cout << "Dimension: " << n << endl;
    std::cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();

    Solution s = Construction(data);
    for (int i = 0; i <= n; i++){
        std::cout << s.sequence[i] << " ";
    }
    std::cout << std::endl;
    std::cout << s.cost << std::endl;

    return 0;
}
