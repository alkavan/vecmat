// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <math.h>
#include <string.h>
#include <vecmat.h>

#if defined(__clang__)
#pragma STDC FP_CONTRACT OFF
#endif

/*
 * Splitmix64 (Vigna): state += 0x9E3779B97F4A7C15, then the two multiplies.
 * Uniforms take the top 53 bits, set the low bit, and scale by 2^-53.
 * That product is exact and lies in (0, 1). Box–Muller uses a power series
 * for the logarithm and a Taylor sine/cosine, then the IEEE square root.
 * frexp, ldexp, and sqrt are the libm calls. They are exact or correctly
 * rounded, so the stored bits are fixed by IEEE-754.
 */

static double from_bits(const uint64_t bits)
{
    double out;
    memcpy(&out, &bits, sizeof out);
    return out;
}

/*
 * SplitMix64 (Vigna). The increment is floor(2^64 / φ), so the counter
 * visits every state. The two odd multipliers are Stafford Mix13; with
 * the xorshifts they avalanche the Weyl counter into the output word.
 */
static uint64_t splitmix64_next(vm_rng *rng)
{
    uint64_t z = (rng->state += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

static double uniform_open(vm_rng *rng)
{
    const uint64_t mant = (splitmix64_next(rng) >> 11) | 1ULL;
    return ldexp((double)mant, -53);
}

static double ln_positive(const double x)
{
    const double ln2 = from_bits(0x3fe62e42fefa39efULL);
    const double inv_sqrt2 = from_bits(0x3fe6a09e667f3bcdULL);
    int exponent = 0;
    double m = frexp(x, &exponent);
    if (m < inv_sqrt2) {
        m *= 2.0;
        exponent -= 1;
    }
    const double f = m - 1.0;
    const double s = f / (2.0 + f);
    const double z = s * s;

    /* log(1+f) = 2 s * sum_{k=0}^{12} z^k / (2k+1). */
    double r = 1.0 / 25.0;
    for (int k = 11; k >= 0; --k) {
        const double zr = r * z;
        const double inv = 1.0 / (2.0 * (double)k + 1.0);
        r = zr + inv;
    }

    const double ln_m = (2.0 * s) * r;
    const double scaled = (double)exponent * ln2;
    return ln_m + scaled;
}

static double sin_small(const double x)
{
    const double z = x * x;
    double term = x;
    double sum = term;
    for (int k = 1; k <= 12; ++k) {
        const double den = (double)(2 * k) * (double)(2 * k + 1);
        term = -term * z / den;
        sum += term;
    }
    return sum;
}

static double cos_small(const double x)
{
    const double z = x * x;
    double term = 1.0;
    double sum = term;
    for (int k = 1; k <= 12; ++k) {
        const double den = (double)(2 * k - 1) * (double)(2 * k);
        term = -term * z / den;
        sum += term;
    }
    return sum;
}

/*
 * sin(2πu) and cos(2πu) for u in (0, 1). Fold into a quadrant, evaluate the
 * Taylor series on [0, π/2], then restore the signs.
 */
static void sincos_unit(double u, double *sn, double *cs)
{
    const double half_pi = from_bits(0x3ff921fb54442d18ULL);
    int q = 0;
    if (u >= 0.5) {
        u -= 0.5;
        q += 2;
    }

    if (u >= 0.25) {
        u -= 0.25;
        q += 1;
    }

    const double two_pi = half_pi * 4.0;
    double angle = two_pi * u;
    if (angle > half_pi) {
        angle = half_pi;
    }

    const double quarter = half_pi * 0.5;
    double s;
    double c;
    if (angle > quarter) {
        const double t = half_pi - angle;
        s = cos_small(t);
        c = sin_small(t);
    } else {
        s = sin_small(angle);
        c = cos_small(angle);
    }

    if (q == 0) {
        *sn = s;
        *cs = c;
    } else if (q == 1) {
        *sn = c;
        *cs = -s;
    } else if (q == 2) {
        *sn = -s;
        *cs = -c;
    } else {
        *sn = -c;
        *cs = s;
    }
}

/*
 * One Box–Muller pair. Two open uniforms: radius from −2 ln u1, angle from
 * u2. z0 and z1 are independent N(0, 1) in IEEE-754 double.
 */
static void box_muller(vm_rng *rng, double *z0, double *z1)
{
    const double u1 = uniform_open(rng);
    const double u2 = uniform_open(rng);
    const double ln_u = ln_positive(u1);
    const double radius = sqrt(-2.0 * ln_u);
    double sine;
    double cosine;
    sincos_unit(u2, &sine, &cosine);
    *z0 = radius * cosine;
    *z1 = radius * sine;
}

/*
 * Box–Muller pair cache. `rng` is the splitmix64 source. After a pair is
 * drawn, `spare` holds the unused partner and `have` is true.
 */
typedef struct {
    vm_rng *rng;
    double spare;
    bool have;
} normal_stream;

/* One N(0, 1) from `stream`, generating a Box–Muller pair when none is waiting. */
static double standard_normal(normal_stream *stream)
{
    if (stream->have) {
        stream->have = false;
        return stream->spare;
    }
    double z0;
    double z1;
    box_muller(stream->rng, &z0, &z1);
    stream->spare = z1;
    stream->have = true;
    return z0;
}

static vm_float_t scaled(const double z, const double scale)
{
    const double y = z * scale;
    return (vm_float_t)y;
}

static void store(const vm_mat *A, const int r, const int c, const vm_float_t v)
{
    A->data[(size_t)r + (size_t)c * (size_t)A->rows] = v;
}

static bool square_ready(const vm_mat *A)
{
    return A && A->data && A->rows >= 1 && A->cols == A->rows;
}

void vm_rng_seed(vm_rng *rng, const uint64_t seed)
{
    if (!rng) {
        return;
    }
    rng->state = seed;
    (void)splitmix64_next(rng);
}

bool vm_goe(const vm_mat *A, vm_rng *rng)
{
    if (!rng || !square_ready(A)) {
        return false;
    }

    const double sqrt2 = from_bits(0x3ff6a09e667f3bcdULL);
    normal_stream stream = {.rng = rng, .spare = 0.0, .have = false};

    const int n = A->rows;
    for (int i = 0; i < n; ++i) {
        store(A, i, i, scaled(standard_normal(&stream), sqrt2));
        for (int j = i + 1; j < n; ++j) {
            const vm_float_t off = scaled(standard_normal(&stream), 1.0);
            store(A, i, j, off);
            store(A, j, i, off);
        }
    }

    return true;
}

bool vm_gue(const vm_mat *Ar, const vm_mat *Ai, vm_rng *rng)
{
    if (!rng || !square_ready(Ar) || !square_ready(Ai) || Ar->rows != Ai->rows) {
        return false;
    }

    const double inv_sqrt2 = from_bits(0x3fe6a09e667f3bcdULL);
    normal_stream stream = {.rng = rng, .spare = 0.0, .have = false};

    const int n = Ar->rows;
    for (int i = 0; i < n; ++i) {
        store(Ar, i, i, scaled(standard_normal(&stream), 1.0));
        store(Ai, i, i, VM_F(0.0));
        for (int j = i + 1; j < n; ++j) {
            const vm_float_t re = scaled(standard_normal(&stream), inv_sqrt2);
            const vm_float_t im = scaled(standard_normal(&stream), inv_sqrt2);
            store(Ar, i, j, re);
            store(Ar, j, i, re);
            store(Ai, i, j, im);
            store(Ai, j, i, -im);
        }
    }

    return true;
}
