#pragma once 

#include <iostream>
#include <memory>
#include <cmath>

struct CommonLogTable {
    size_t num_entities{0};
    std::unique_ptr<size_t[]> index{nullptr};
    std::unique_ptr<double[]> pre_exp{nullptr};
    std::unique_ptr<double[]> values{nullptr};
    std::unique_ptr<size_t[]> mantissa{nullptr}; 
};

void init(CommonLogTable & entt, size_t N){
    entt.index = std::make_unique<size_t[]>(N);
    entt.pre_exp = std::make_unique<double[]>(N);
    entt.values = std::make_unique<double[]>(N); 
    entt.mantissa = std::make_unique<size_t[]>(N);
    entt.num_entities = N;
}

void compute(CommonLogTable & entt){
    init(entt, 9000);
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.index[idx] = idx + 1000;
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.pre_exp[idx] = static_cast<double>(entt.index[idx]);
        entt.pre_exp[idx] /= static_cast<double>(1000);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.values[idx] = std::log(entt.pre_exp[idx]);
        entt.values[idx] /= std::log(10.0);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.mantissa[idx] = static_cast<size_t>(1.0e5 * entt.values[idx] + 0.5);
    }
}

static std::ostream & operator << (std::ostream & out, CommonLogTable & entt){
    for(size_t idx = 0; idx < static_cast<size_t>(entt.num_entities/10); ++idx){
        out << std::setw(3) << std::setfill('0') << static_cast<size_t>(entt.index[idx * 10]/10);
        
        for(size_t jdx = 0; jdx < 10; ++jdx){
            out << " & " << std::setw(5) << std::setfill('0') << entt.mantissa[idx * 10 + jdx];
        }
        
        out << " \\\\\n";
    }
    
    return out;
}