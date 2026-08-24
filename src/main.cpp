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

void show_sequence(Solution &s){
    for (int i = 0; i < s.sequence.size()-1; i++){
        std::cout << s.sequence[i] << " ";
    }
    std::cout << std::endl;
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

bool best_improvement_swap(Solution &s, Data &data){
    double best_delta = 0;
    int best_i, best_j;
    for (int i = 1; i < s.sequence.size()-1; i++){
        int vi = s.sequence[i];
        int vi_next = s.sequence[i+1];
        int vi_prev = s.sequence[i-1];

        for (int j = i+1; j < s.sequence.size()-1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];
            int vj_prev = s.sequence[j-1];

            double delta = 0;

            if (j != i+1){
                delta = (
                    data.getDistance(vi_prev, vj)+
                    data.getDistance(vj, vi_next)+
                    data.getDistance(vj_prev, vi)+
                    data.getDistance(vi, vj_next)
                ) - (
                    data.getDistance(vi_prev, vi)+
                    data.getDistance(vi, vi_next)+
                    data.getDistance(vj_prev, vj)+
                    data.getDistance(vj, vj_next)
                );
            }
            else {
                delta = (
                    data.getDistance(vi_prev, vj)+
                    data.getDistance(vi, vj_next)
                ) - (
                    data.getDistance(vi_prev, vi)+
                    data.getDistance(vj, vj_next)
                );
            }
            /*
            std::cout << "\nprev_vi: " << vi_prev << "\nvi: " << vi << "\nvi_next: " << vi_next;
            std::cout << "\nprev_vj: " << vj_prev << "\nvj: " << vj << "\nvj_next: " << vj_next;
            std::cout << "\ndelta calculado: " << delta << std::endl;
            */

            if (delta < best_delta){
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if (best_delta < 0){
        std::swap(s.sequence[best_i], s.sequence[best_j]);
        s.cost += best_delta;
        /*
        show_sequence(s);
        show_cost(s, data);
        */
        return true;
    }
    return false;
}

bool best_improvement_2opt(Solution &s, Data &data){
    double best_delta = 0;
    int best_i, best_j;
    for (int i = 0; i < s.sequence.size()-1; i++){
        int vi = s.sequence[i];
        int vi_next = s.sequence[i+1];

        for (int j = i+2; j < s.sequence.size()-1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];

            double delta = 0;

            if (vj_next != vi){
                delta = (
                    data.getDistance(vi, vj)+
                    data.getDistance(vi_next, vj_next)
                ) - (
                    data.getDistance(vi, vi_next)+
                    data.getDistance(vj, vj_next)
                );
            }

            if (delta < best_delta){
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if (best_delta < 0){
        std::reverse(s.sequence.begin() + best_i+1, s.sequence.begin() + best_j+1);
        s.cost += best_delta;
        
        return true;
    }
    return false;
}

bool best_improvement_oropt(Solution &s, Data &data, int size_block){
    double best_delta = 0;
    int best_i, best_j;
    for (int i = 1; i < s.sequence.size()-size_block; i++){
        int vi = s.sequence[i];
        int vi_end = s.sequence[i+size_block-1];
        int vi_next = s.sequence[i+size_block];
        int vi_prev = s.sequence[i-1];

        for (int j = 0; j < s.sequence.size()-1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];

            double delta = 0;

            if (j < i-1 || j > i+size_block-1){
                delta = (
                    data.getDistance(vi, vj)+
                    data.getDistance(vi_end, vj_next)+
                    data.getDistance(vi_prev, vi_next)
                ) - (
                    data.getDistance(vj, vj_next)+
                    data.getDistance(vi_end, vi_next)+
                    data.getDistance(vi_prev, vi)
                );

                if (delta < best_delta){
                    best_delta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }
    }

    if (best_delta < 0){
        if (size_block == 1){
            int reinserted = s.sequence[best_i];
            s.sequence.erase(s.sequence.begin() + best_i);
            if (best_i < best_j){
                s.sequence.insert(s.sequence.begin() + best_j, reinserted);
            }
            else {
                s.sequence.insert(s.sequence.begin() + best_j+1, reinserted);
            }
        }
        else {
            std::vector<int> reinserted(s.sequence.begin()+best_i, s.sequence.begin()+best_i+size_block);
            s.sequence.erase(s.sequence.begin()+best_i, s.sequence.begin()+best_i+size_block);
            if (best_i < best_j){
                s.sequence.insert(s.sequence.begin() + best_j+1-size_block, reinserted.begin(), reinserted.end());
            }
            else {
                s.sequence.insert(s.sequence.begin() + best_j+1, reinserted.begin(), reinserted.end());
            }
        }
        s.cost += best_delta;
        
        return true;
    }
    return false;
}

/*
void Local_search(Solution &s, Data &data){
    std::vector<int> NL = {1,2,3,4,5};
    bool improved = false;

    while (!NL.empty()){
        int n = (rand() % NL.size()) + 1;
        switch (NL[n]){
            case 1:
                improved = best_improvement_swap(s, data);
                break;
            case 2:

        }
    }
}
*/

/*
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
    /*std::cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();*/

    Solution s = Construction(data);
    show_sequence(s);
    show_cost(s, data);
    
    best_improvement_swap(s, data);
    show_sequence(s);
    std::cout << s.cost << std::endl;
    show_cost(s, data);

    best_improvement_2opt(s, data);
    show_sequence(s);
    std::cout << s.cost << std::endl;
    show_cost(s, data);

    int size_block = 3;

    best_improvement_oropt(s, data, size_block);
    show_sequence(s);
    std::cout << s.cost << std::endl;
    show_cost(s, data);

/*
    for (int i = 0; i <= n; i++){
        std::cout << s.sequence[i] << " ";
    }

    std::cout << std::endl;
    show_cost(s, data);
    std::cout << s.cost << std::endl;
*/

    return 0;
}
