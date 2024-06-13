all:
	g++ --std=c++23 -O3 -fopenmp -fopenmp-simd -march=native ../src/main.cpp -o ../bin/math_table 

clean:
	rm -f *.o 
	rm -f ../bin/*
