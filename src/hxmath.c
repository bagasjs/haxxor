#define EZMATH_WITH_UTILS
#include "hxmath.h"

#include <stdlib.h>
#include <memory.h>

#ifdef EZMATH_WITH_UTILS
    #include <stdio.h>

    void PrintMat4(Mat4* mat)
    {
        printf("[\n[%.4f,%.4f,%.4f,%.4f]\n[%.4f,%.4f,%.4f,%.4f]\n[%.4f,%.4f,%.4f,%.4f]\n[%.4f,%.4f,%.4f,%.4f]\n]",
            mat->elements[0], mat->elements[1], mat->elements[2], mat->elements[3],
            mat->elements[4], mat->elements[5], mat->elements[6], mat->elements[7],
            mat->elements[8], mat->elements[9], mat->elements[10], mat->elements[11],
            mat->elements[12], mat->elements[13], mat->elements[14], mat->elements[15]
        );
    }

    void PrintVec4(Vec4* vec)
    {
        printf("[%.4f,%.4f,%.4f,%.4f]", vec->x, vec->y, vec->z, vec->w);
    }

    void PrintVec3(Vec3* vec)
    {
        printf("[%.4f,%.4f,%.4f]", vec->x, vec->y, vec->z);
    }

    void PrintVec2(Vec2* vec)
    {
        printf("[%.4f,%.4f]", vec->x, vec->y);
    }
#endif // EZMATH_WITH_UTILS

void Memset(void* dst, int val, uint32_t size)
{
    memset(dst, val, (size_t)size);
}