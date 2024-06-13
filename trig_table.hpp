#pragma once 

#include <iostream>
#include <memory>
#include <cmath>

struct TrigTable {
    size_t num_entities{0};
    std::unique_ptr<double[]> angle_minute{nullptr};
    std::unique_ptr<double[]> angle_degree{nullptr};
    std::unique_ptr<double[]> angle_radian{nullptr};
    std::unique_ptr<double[]> values{nullptr};
    std::unique_ptr<double[]> L_values{nullptr};
    std::unique_ptr<double[]> fraction_part{nullptr};
    std::unique_ptr<int[]> integer_part{nullptr};  
    std::unique_ptr<unsigned long[]> mantissa{nullptr};
};

static void init_entities(TrigTable & entt, size_t N){
    entt.angle_minute = std::make_unique<double[]>(N);
    entt.angle_degree = std::make_unique<double[]>(N);
    entt.angle_radian = std::make_unique<double[]>(N);
    entt.values = std::make_unique<double[]>(N);
    entt.L_values = std::make_unique<double[]>(N);
    entt.fraction_part = std::make_unique<double[]>(N);
    entt.integer_part = std::make_unique<int[]>(N);
    entt.mantissa = std::make_unique<unsigned long[]>(N);
    entt.num_entities = N;
}

static void compute_angles(TrigTable & entt){
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.angle_minute[idx] = static_cast<double>(idx + 1);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.angle_degree[idx] = entt.angle_minute[idx];
        entt.angle_degree[idx] /= static_cast<double>(60);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.angle_radian[idx] = entt.angle_degree[idx];
        entt.angle_radian[idx] /= static_cast<double>(180);
        entt.angle_radian[idx] *= static_cast<double>(std::atan2(0.0, -1.0));
    }
}

static void compute_sines(TrigTable & entt){
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.values[idx] = std::sin(entt.angle_radian[idx]);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.L_values[idx] = std::log(entt.values[idx]);
        entt.L_values[idx] /= std::log(static_cast<double>(10));
        entt.L_values[idx] += static_cast<double>(10);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.integer_part[idx] = static_cast<int>(std::floor(entt.L_values[idx]));
    }

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.fraction_part[idx] = entt.L_values[idx];
    }  

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.fraction_part[idx] -= static_cast<double>(entt.integer_part[idx]);
    }

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.mantissa[idx] = static_cast<unsigned long>(1.0e8 * entt.fraction_part[idx] + 0.5);
    }

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.integer_part[idx] -= 10;
    }       
}

static void compute_tangents(TrigTable & entt){
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.values[idx] = std::tan(entt.angle_radian[idx]);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.L_values[idx] = std::log(entt.values[idx]);
        entt.L_values[idx] /= std::log(static_cast<double>(10));
        entt.L_values[idx] += static_cast<double>(10);
    }
    
    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.integer_part[idx] = static_cast<int>(std::floor(entt.L_values[idx]));
    }

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.fraction_part[idx] = entt.L_values[idx];
    }  

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.fraction_part[idx] -= static_cast<double>(entt.integer_part[idx]);
    }

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.mantissa[idx] = static_cast<unsigned long>(1.0e8 * entt.fraction_part[idx] + 0.5);
    }

    #pragma omp parallel for simd 
    for(size_t idx = 0; idx < entt.num_entities; ++idx){
        entt.integer_part[idx] -= 10;
    }        
}

static std::ostream & operator << (std::ostream & out, TrigTable & entt){
    for(size_t idx = 0; idx < static_cast<size_t>(entt.num_entities/10); ++idx){
        out << std::setw(4) << std::setfill('0') << entt.angle_minute[idx * 10] - 1 ;
        
        for(size_t jdx = 0; jdx < 10; ++jdx){
            if(10 * idx + jdx != 0){
                out << " & ";
                
                if(entt.integer_part[idx * 10 + jdx - 1] >= 0) {
                    out << std::setw(1) << entt.integer_part[idx * 10 + jdx - 1] << '.';  
                } else {
                    out << "$\\bar{" << std::setw(1) << - entt.integer_part[idx * 10 + jdx - 1] << "}$.";
                }

                out << std::setw(8) << std::setfill('0') << entt.mantissa[idx * 10 + jdx - 1] ;
            } else {
                out << " & $-\\infty$ " ;
            }
        }
        
        out << " \\\\\n";
    }
    
    return out;
}

