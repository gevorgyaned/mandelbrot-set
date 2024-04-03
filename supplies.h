#ifndef SUPPLIES_H
#define SUPPLIES_H

#include <SDL2/SDL.h>
#include <limits.h>
#include <immintrin.h>

SDL_Color palette[] = {
	{ 66,  30 , 15}, 
	{ 25,   7 , 26},
	{  9,   1 , 47},
	{  4,   4 , 73},
	{  0,   7 ,100},
	{ 12,  44 ,138},
	{ 24,  82 ,177},
	{ 57, 125 ,209},
	{134, 181 ,229},
	{211, 236 ,248},
	{241, 233 ,191},
	{248, 201 , 95},
	{255, 170 ,  0},
	{204, 128 ,  0},
	{153,  87 ,  0},
	{106,  52 ,  3}
};

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

static double REAL_BEG   =   -2;
static double REAL_END   =    1;
static double IM_BEG     =   -1;
static double IM_END     =    1;
static int WINDOW_WIDTH  =  600;
static int WINDOW_HEIGHT =  400;
static int THREADS_COUNT =    4;
static int ITERATIONS    =  100;

void print_m256d(__m256 reg) {
    double d[4];
    _mm256_store_pd(d, reg);
    printf("%lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
}

void print_m256i(__m256 reg, FILE *file) {
    long d[4];
    _mm256_storeu_si256((__m256i_u *)d, reg);
    fprintf(file, "%lu\n", d[0]);
    fprintf(file, "%lu\n", d[1]);
    fprintf(file, "%lu\n", d[2]);
    fprintf(file, "%lu\n", d[3]);
}

void print_m256_binary(__m256 reg)
{
    long d[4];
    _mm256_storeu_si256((__m256i_u *)d, reg);

    for (int i = 0; i < 4; ++i) {
        for (int j = LONG_BIT - 1; j >= 0; --j) {
            printf("%d", (d[i] >> j) & 1 ? 1 : 0);
        }
        putchar('\n');
    }
}

#endif /* SUPPLIES_H */
