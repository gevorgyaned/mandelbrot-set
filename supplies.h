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

static double REAL_BEG   =   -1;
static double REAL_END   =    1;
static double IM_BEG     =   -1;
static double IM_END     =    1;
static int WINDOW_WIDTH  =  400;
static int WINDOW_HEIGHT =  400;
static int THREADS_COUNT =    4;
static int ITERATIONS    =  100;

#endif /* SUPPLIES_H */
