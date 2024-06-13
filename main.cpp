#include <iostream>
#include <iomanip>
#include <fstream>
#include "trig_table.hpp"
#include "log_table.hpp"

void calulate_trig_tables(){
    TrigTable entt;
    std::ofstream fout_sin("../outputs/log_sin_table.tex");
    std::ofstream fout_tan("../outputs/log_tan_table.tex");
    
    init_entities(entt, 60 * 90);
    compute_angles(entt);
    compute_sines(entt);
    fout_sin << entt;

    compute_tangents(entt);
    fout_tan << entt;
}

void calculate_log_table(){
    CommonLogTable entt;
    std::ofstream fout("../outputs/log_table.tex");
    compute(entt);
    
    fout << entt;
}

int main(int agrc, char** argv){
    calulate_trig_tables();
    calculate_log_table();
    
    return 0;
}