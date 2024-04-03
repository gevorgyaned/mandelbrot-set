#include "supplies.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <immintrin.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>

#define CHANGE_SPEED 100
#define ZOOM_SPEED 10

SDL_Window *main_window;
SDL_Renderer *render;

SDL_Color get_color(long iteration) {
    if (iteration < ITERATIONS)
        return palette[iteration % ARR_SIZE(palette)];

    return (SDL_Color){0, 0, 0};
}


int main(int argc, char *argv[]) 
{
    FILE *file = fopen("log.txt", "w+");
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &main_window, &render);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    const __m256i one = _mm256_set1_epi64x(1);
    const __m256d two = _mm256_set1_pd(2.0);
    const __m256d four = _mm256_set1_pd(4.0);
    int mouseX, mouseY;
    double zoomX, zoomY;
    
    for (;;) {
        double scaleX = (REAL_END - REAL_BEG) / (WINDOW_WIDTH);
        double scaleY = (IM_END - IM_BEG) / (WINDOW_HEIGHT);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:  
                    return 1;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            IM_END -= (0.1 * scaleY * CHANGE_SPEED);
                            IM_BEG -= (0.1 * scaleY * CHANGE_SPEED);
                            break;
                        case SDLK_DOWN:
                            IM_END += (0.1 * scaleY * CHANGE_SPEED);
                            IM_BEG += (0.1 * scaleY * CHANGE_SPEED);
                            break;
                        case SDLK_LEFT:
                            REAL_BEG -= (0.1 * scaleX * CHANGE_SPEED);
                            REAL_END -= (0.1 * scaleX * CHANGE_SPEED);
                            break;
                        case SDLK_RIGHT:
                            REAL_END += (0.1 * scaleX * CHANGE_SPEED);
                            REAL_BEG += (0.1 * scaleX * CHANGE_SPEED);
                            break;
                        case SDLK_RETURN:
                            scaleX /= 1.02;
                            scaleY /= 1.02;
                            REAL_END -= scaleX * ZOOM_SPEED;
                            IM_END -= scaleY * ZOOM_SPEED;
                            IM_BEG += scaleY * ZOOM_SPEED;
                            REAL_BEG += scaleX * ZOOM_SPEED;
                            fprintf(file, "REAL_END - %lf, REAL_BEG - %lf, IM_END - %lf, IM_BEG - %lf\n", REAL_END, REAL_BEG, IM_END, IM_BEG);
                            break;
                        case SDLK_SPACE:
                            ITERATIONS += 10; 
                            printf("%d\n", ITERATIONS);
                            break;
                    }
                break;
            }
        }

        __m256d add = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
        add = _mm256_mul_pd(add, _mm256_set1_pd(((double)(REAL_END - REAL_BEG)) / WINDOW_WIDTH));
        __m256i itr = _mm256_set1_epi64x(ITERATIONS);   

        __m256d cIm, cRe;
        clock_t start = clock();

        for (int i = 0; i < WINDOW_WIDTH; ++i) {
            double x = (REAL_BEG + (((double)i / WINDOW_WIDTH)) * (REAL_END - REAL_BEG));
            for (int j = 0; j < WINDOW_HEIGHT; j += 4) {
                double y = (IM_BEG + (((double)j / WINDOW_HEIGHT)) * (IM_END - IM_BEG));
                __m256i n, mask;
                __m256d zIm, zRe, zIm2, zRe2, float_mask, mod2;

                cRe = _mm256_set1_pd(x);
                cIm = _mm256_add_pd(add, _mm256_set1_pd(y));
                n = _mm256_set1_epi64x(0);
                mask = _mm256_set1_epi64x(-1);
                zIm = _mm256_set1_pd(0);
                zRe = _mm256_set1_pd(0);

                while (_mm256_movemask_epi8(mask)) {
                    zIm2 = _mm256_mul_pd(_mm256_mul_pd(two, zIm), zRe); // zim2 = 2 * zim * zre
                    zRe2 = _mm256_sub_pd(_mm256_mul_pd(zRe, zRe), _mm256_mul_pd(zIm, zIm)); // zre2 = zre * zre - zim * zim
                    zIm = _mm256_add_pd(zIm2, cIm); // zim = zim2 + y
                    zRe = _mm256_add_pd(zRe2, cRe); // zre = zre + x
                    mod2 = _mm256_add_pd(_mm256_mul_pd(zIm, zIm), _mm256_mul_pd(zRe, zRe));

                    float_mask = _mm256_cmp_pd(mod2, four, _CMP_LT_OQ);
                    mask = _mm256_cmpgt_epi64(itr, n);
                    mask = _mm256_and_si256(mask, _mm256_castpd_si256(float_mask));
                    n = _mm256_add_epi64(n, _mm256_and_si256(mask, one));
                }  

                for (int k = 0; k < 4; ++k) {
                    SDL_Color col;
                    if (n[k] < ITERATIONS)
                        col = palette[n[k] % ARR_SIZE(palette)];
                    else 
                        col = (SDL_Color){0, 0, 0};
                    
                    SDL_SetRenderDrawColor(render, col.r, col.g, col.b, 255);
                    SDL_RenderDrawPoint(render, i, j - k + 3);
                }      
            }
        }

        clock_t end = clock();
        printf("time passed - %lf\n", ((double)(end - start)) / CLOCKS_PER_SEC);

        SDL_RenderPresent(render);
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}
