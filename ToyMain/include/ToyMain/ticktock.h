// #pragma once

// #include <stdio.h>
// #include <stdint.h>
// #include "pico/stdlib.h"

// #define TICK(x) uint64_t bench_##x = time_us_64();
// #define TOCK(x) printf("time: ")<< #x " time: " << time_us_64() - bench_##x << "s" << std::endl;

// #define TOCK(x) std::cout << #x " time: " << time_us_64() - bench_##x << "s" << std::endl;




//         // clock_t startTime = clock();

// clock_t clock()
// {
//     return (clock_t) time_us_64() / 10000.f;
// }

//         // clock_t endTime = clock();        
//         // float executionTime = (float)(endTime - startTime) / CLOCKS_PER_SEC;
//         // printf("%.8f sec\n", executionTime);






// uint64_t TICK = time_us_64();

// uint64_t TOCK = time_us_64();
// printf("%llu\n", (TOCK - TICK));