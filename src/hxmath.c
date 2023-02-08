#define EZMATH_WITH_UTILS
#include "hxmath.h"

#ifdef EZMATH_WITH_UTILS
    #include <stdio.h>

    void PrintMat4(MAT4* mat)
    {
        printf("[\n[%.4f,%.4f,%.4f,%.4f]\n[%.4f,%.4f,%.4f,%.4f]\n[%.4f,%.4f,%.4f,%.4f]\n[%.4f,%.4f,%.4f,%.4f]\n]",
            mat->elements[0], mat->elements[1], mat->elements[2], mat->elements[3],
            mat->elements[4], mat->elements[5], mat->elements[6], mat->elements[7],
            mat->elements[8], mat->elements[9], mat->elements[10], mat->elements[11],
            mat->elements[12], mat->elements[13], mat->elements[14], mat->elements[15]
        );
    }

    void PrintVec4(VEC4* vec)
    {
        printf("[%.4f,%.4f,%.4f,%.4f]", vec->x, vec->y, vec->z, vec->w);
    }

    void PrintVec3(VEC3* vec)
    {
        printf("[%.4f,%.4f,%.4f]", vec->x, vec->y, vec->z);
    }

    void PrintVec2(VEC2* vec)
    {
        printf("[%.4f,%.4f]", vec->x, vec->y);
    }
#endif // EZMATH_WITH_UTILS

#include <math.h>
float Sin(float x)
{
    return sin(x);
}

float Cos(float x)
{
    return cos(x);
}

float Tan(float x)
{
    return tan(x);
}

float Acos(float x)
{
    return acos(x);
}

float Sqrt(float x)
{
    return sqrt(x);
}

float Abs(float x)
{
    return fabsf(x);
}