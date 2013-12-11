/*
Copyright (C) 2013 Ben Dyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _3DMATH_H_
#define _3DMATH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define X 0
#define Y 1
#define Z 2
#define W 3

#ifndef absval
#define absval(x) ((x) < 0 ? -x : x)
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef M_PI
#define M_PI ((real_t)3.14159265358979323846)
#define M_PI_2 (M_PI * 0.5)
#define M_PI_4 (M_PI * 0.25)
#endif

#ifdef __TI_COMPILER_VERSION__

static inline double sqrt_inv(double a) {
    double x, half_a;

    if (a < 0) {
        x = _lltod(0x7FEFFFFFFFFFFFFF);
    } else {
        half_a = a * 0.5;
        x = _rsqrdp(a);
        x = x * (1.5 - half_a*x*x);
        x = x * (1.5 - half_a*x*x);
        x = x * (1.5 - half_a*x*x);
    }

    return x;
}

static inline double fsqrt(double a) {
    double  y, X0, X1, X2, X4;
    int     upper;

    upper = _clr(_hi(a), 31, 31);
    y = _itod(upper, _lo(a));

    X0 = _rsqrdp(y);
    X1 = X0 * (1.5 - (y*X0*X0*0.5));
    X2 = X1 * (1.5 - (y*X1*X1*0.5));
    X4 = y * X2 * (1.5 - (y*X2*X2*0.5));

    if (a <= 0.0) {
        X4 = 0.0;
    }
    if (a > 1.7976931348623157E+308) {
        X4 = 1.7976931348623157E+308;
    }

    return X4;
}

static inline double divide(double a, double b) {
    double  x;
    if (a == 0.0) {
        x = 0.0;
    } else {
        x = _rcpdp(b);
        x = x * (2.0 - b*x);
        x = x * (2.0 - b*x);
        x = x * (2.0 - b*x);
        x = x * a;
    }

    return x;
}

static inline double recip(double b) {
    double  x;
    x = _rcpdp(b);
    x = x * (2.0 - b*x);
    x = x * (2.0 - b*x);
    x = x * (2.0 - b*x);

    return x;
}

#else

#define sqrt_inv(x) (1.0f / (float)sqrt((x)))
#define divide(a, b) ((a) / (b))
#define recip(a) (1.0f / (a))
#define fsqrt(a) (float)sqrt((a))
#define _nassert(x)

#endif


static inline void vector_add_f(unsigned int dim, float *restrict result,
const float *restrict v1, const float *restrict v2) {
    assert(dim && result && v1 && v2 && result != v1 && result != v2 &&
           v1 != v2);
    _nassert((size_t)result % 4 == 0);
    _nassert((size_t)v1 % 4 == 0);
    _nassert((size_t)v2 % 4 == 0);

    while (dim--) {
        result[dim] = v1[dim] + v2[dim];
    }
}

static inline void vector_add_inplace_f(unsigned int dim,
float *restrict result, const float *restrict v1) {
    assert(dim && result && v1 && result != v1);
    _nassert((size_t)result % 4 == 0);
    _nassert((size_t)v1 % 4 == 0);

    while (dim--) {
        result[dim] += v1[dim];
    }
}

static inline void vector_subtract_f(unsigned int dim, float *restrict result,
const float *restrict v1, const float *restrict v2) {
    assert(dim && result && v1 && v2 && result != v1 && result != v2 &&
           v1 != v2);
    _nassert((size_t)result % 4 == 0);
    _nassert((size_t)v1 % 4 == 0);
    _nassert((size_t)v2 % 4 == 0);

    while (dim--) {
        result[dim] = v1[dim] - v2[dim];
    }
}

static inline void vector_subtract_inplace_f(unsigned int dim,
float *restrict result, const float *restrict v1) {
    assert(dim && result && v1 && result != v1);
    _nassert((size_t)result % 4 == 0);
    _nassert((size_t)v1 % 4 == 0);

    while (dim--) {
        result[dim] -= v1[dim];
    }
}

static inline void vector_scale_f(unsigned int dim, float *restrict result,
const float *restrict v1, float scale) {
    assert(dim && result && v1 && result != v1);
    _nassert((size_t)result % 4 == 0);
    _nassert((size_t)v1 % 4 == 0);

    while (dim--) {
        result[dim] = v1[dim] * scale;
    }
}

static inline void vector_scale_inplace_f(unsigned int dim,
float *restrict result, float scale) {
    assert(dim && result);
    _nassert((size_t)result % 4 == 0);

    while (dim--) {
        result[dim] *= scale;
    }
}

static inline void vector3_cross_f(float *restrict result,
const float *restrict v1, const float *restrict v2) {
    assert(result && v1 && v2 && result != v1 && result != v2 && v1 != v2);
    _nassert((size_t)result % 4 == 0);
    _nassert((size_t)v1 % 4 == 0);
    _nassert((size_t)v2 % 4 == 0);

    float r0, r1, r2, r3, r4, r5;

    r0 = v1[Y]*v2[Z];
    r1 = v1[Z]*v2[Y];
    r2 = v1[Z]*v2[X];
    r3 = v1[X]*v2[Z];
    r4 = v1[X]*v2[Y];
    r5 = v1[Y]*v2[X];

    result[X] = r0 - r1;
    result[Y] = r2 - r3;
    result[Z] = r4 - r5;
}

static void matrix_multiply_f(float *restrict C, const float B[],
const float A[], const size_t AR, const size_t AC, const size_t BR,
const size_t BC, const float mul) {
    /*
    Calculate C = AB, where A has AN rows and AM columns, and B has BN rows
    and BM columns. C must be AN x BM.
    */
    assert(A && B && C && AR == BC);
    _nassert((size_t)C % 4 == 0);
    _nassert((size_t)B % 4 == 0);
    _nassert((size_t)A % 4 == 0);

    size_t i, j, k;
    #pragma MUST_ITERATE(1,24)
    for (i = 0; i < AC; i++) {
        #pragma MUST_ITERATE(1,24)
        for (j = 0; j < BR; j++) {
            float t = 0.0;
            #pragma MUST_ITERATE(1,24)
            for (k = 0; k < AR; k++) {
                t += A[i * AR + k] * B[k * BR + j];
            }

            C[i * BR + j] = t * mul;
        }
    }
}

static void matrix_cholesky_decomp_scale_f(unsigned int dim, float L[],
const float A[], const float mul) {
    assert(L && A && dim);
    _nassert((size_t)L % 8 == 0);
    _nassert((size_t)A % 8 == 0);

    /*
    9x9:
    900 mult
    72 div
    9 sqrt
    */

    unsigned int i, j, kn, in, jn;
    for (i = 0, in = 0; i < dim; i++, in += dim) {
        L[i + 0] = (i == 0) ? fsqrt(A[i + in]*mul) : recip(L[0]) * (A[i]*mul);

        for (j = 1, jn = dim; j <= i; j++, jn += dim) {
            float s = 0;
            #pragma MUST_ITERATE(1,9)
            for (kn = 0; kn < j*dim; kn += dim) {
                s += L[i + kn] * L[j + kn];
            }

            L[i + jn] = (i == j) ? fsqrt(A[i + in]*mul - s) :
                recip(L[j + jn]) * (A[i + jn]*mul - s);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif