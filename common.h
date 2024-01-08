#ifndef COMMON_H_
#define COMMON_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <SDL.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60
#define DELTA_TIME_SEC (1.0f / SCREEN_FPS)
#define DELTA_TIME_MS ((Uint32) floorf(DELTA_TIME_SEC * 1000.0f))
#define MARKER_SIZE 15.0f
#define BACKGROUND_COLOR 0x353535FF
#define RED_COLOR 0xDA2C38FF
#define GREEN_COLOR 0x87C38FFF
#define BLUE_COLOR 0x748CABFF

#define HEX_COLOR(hex)                      \
    ((hex) >> (3 * 8)) & 0xFF,              \
    ((hex) >> (2 * 8)) & 0xFF,              \
    ((hex) >> (1 * 8)) & 0xFF,              \
    ((hex) >> (0 * 8)) & 0xFF

int check_sdl_code(int code)
{
    if (code < 0) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    return code;
}

void *check_sdl_ptr(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}

typedef struct {
    float x;
    float y;
} Vec2;

Vec2 vec2(float x, float y)
{
    return (Vec2){x, y};
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

Vec2 vec2_scale(Vec2 a, float s)
{
    return vec2(a.x * s, a.y * s);
}

Vec2 vec2_add(Vec2 a, Vec2 b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

float vec2_length(Vec2 a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

float lerpf(float a, float b, float p)
{
    return a + (b - a) * p;
}

Vec2 lerpv2(Vec2 a, Vec2 b, float p)
{
    return vec2_add(a, vec2_scale(vec2_sub(b, a), p));
}

void init_pascal_triangle(uint32_t **C, size_t maxn)
{
    for (size_t n = 0; n <= maxn; ++n) {
        C[n] = (uint32_t *)malloc((n/2 + 1 + 1) * sizeof(uint32_t));
        C[n][0] = 1;
        for (size_t k = 1; k < n / 2 + 1; ++k) {
            C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
        }
        if (n != 0 && n % 2 == 0) {
            C[n][n / 2] = 2 * C[n - 1][n / 2 - 1];
        }
    }
}

void free_pascal_triangle(uint32_t **C, size_t maxn)
{
  for (size_t n = 0; n <= maxn; ++n) {
    free(C[n]);
  }
  free(C);
}

uint32_t lookup_pascal_triangle(uint32_t **C, size_t row, size_t col)
{
  return col <= row / 2 ? C[row][col] : C[row][row - col];
}

int binomial_coeff(int n, int r) 
{
    int res = 1;
    for (int i = 0; i < r; i++) {
        res = res * (n - i);
        res = res / (i + 1);
    }
    return res;
}

char *cstr_slurp_file(const char *file_path)
{
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    long m = ftell(f);
    if (m < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    char *buffer = malloc(m + 1);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for file: %s\n",
                strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    size_t n = fread(buffer, 1, m, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    buffer[n] = '\0';

    fclose(f);

    return buffer;
}

#endif  // COMMON_H_
