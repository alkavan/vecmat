// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Adds vectors a and b component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
void vec2_add_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
}

/**
 * @brief Subtracts vector b from vector a component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
void vec2_sub_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
}

/**
 * @brief Multiplies vector v by scalar s component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Input scalar.
 */
void vec2_mul_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
}

/**
 * @brief Divides vector v by scalar s component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Input scalar.
 */
void vec2_div_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    if (s == 0.0f) {
        *res = *v;
        return;
    }
    res->x = v->x / s;
    res->y = v->y / s;
}

/**
 * @brief Multiplies vectors a and b component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
void vec2_mul_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
}

/**
 * @brief Negates the components of vector v, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_neg_ptr(vector2 *res, const vector2 *v)
{
    res->x = -v->x;
    res->y = -v->y;
}

/**
 * @brief Computes the absolute values of the components of vector v, storing
 * the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_abs_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_FABS(v->x);
    res->y = VECMAT_FABS(v->y);
}

/**
 * @brief Normalizes vector v to unit length, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_normalize_ptr(vector2 *res, const vector2 *v)
{
    const vm_float_t len = vec2_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t inv_len = 1.0f / len;
    res->x = v->x * inv_len;
    res->y = v->y * inv_len;
}

/**
 * @brief Computes the component-wise minimum of vectors a and b, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
void vec2_min_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = VECMAT_FMIN(a->x, b->x);
    res->y = VECMAT_FMIN(a->y, b->y);
}

/**
 * @brief Computes the component-wise maximum of vectors a and b, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
void vec2_max_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = VECMAT_FMAX(a->x, b->x);
    res->y = VECMAT_FMAX(a->y, b->y);
}

/**
 * @brief Sets each component of res to the sign of the corresponding component
 * in v (+1, -1).
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_sign_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_COPYSIGN(1.0f, v->x);
    res->y = VECMAT_COPYSIGN(1.0f, v->y);
}

/**
 * @brief Applies the floor function to each component of vector v, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_floor_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_FLOOR(v->x);
    res->y = VECMAT_FLOOR(v->y);
}

/**
 * @brief Applies the ceil function to each component of vector v, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_ceil_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_CEIL(v->x);
    res->y = VECMAT_CEIL(v->y);
}

/**
 * @brief Applies the round function to each component of vector v, storing the
 * result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_round_ptr(vector2 *res, const vector2 *v)
{
    res->x = VECMAT_ROUND(v->x);
    res->y = VECMAT_ROUND(v->y);
}

/**
 * @brief Computes the perpendicular vector to v (90 degrees counterclockwise
 * rotation), storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_perpendicular_ptr(vector2 *res, const vector2 *v)
{
    res->x = -v->y;
    res->y = v->x;
}

/**
 * @brief Computes the 2D cross-product of a and b, storing the scalar value in
 * res->x and 0 in res->y.
 *
 * @param res Output vector.
 * @param a Input vector a.
 * @param b Input vector b.
 */
void vec2_cross_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{

    res->x = a->x * b->y - a->y * b->x;
    res->y = 0.f;
}

/**
 * @brief Scales a vector by a scalar component-wise, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Input scalar.
 */
void vec2_scale_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
}

/**
 * @brief Reflects vector v across the normal, storing the result in res.
 *
 * @param res Output vector.
 * @param v Input incident vector.
 * @param normal Input surface normal.
 */
void vec2_reflect_ptr(vector2 *res, const vector2 *v, const vector2 *normal)
{
    const vm_float_t d = 2.0f * vec2_dot(*v, *normal);
    res->x = v->x - normal->x * d;
    res->y = v->y - normal->y * d;
}

/**
 * @brief Projects vector `a` onto vector `b` (scalar projection scaled by `b`).
 *
 * @param res The output projected vector.
 * @param a The vector to project.
 * @param b The direction vector (non-zero length recommended).
 */
void vec2_project_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    const vm_float_t denom = vec2_dot(*b, *b);
    if (denom == 0.0f) {
        res->x = 0.0f;
        res->y = 0.0f;
        return;
    }
    const vm_float_t scale = vec2_dot(*a, *b) / denom;
    res->x = b->x * scale;
    res->y = b->y * scale;
}

/**
 * @brief Computes a tangent vector perpendicular to the input (90 degrees clockwise).
 *
 * Equivalent to `(v.y, -v.x)`.
 *
 * @param res The output tangent vector.
 * @param v The input vector.
 */
void vec2_tangent_ptr(vector2 *res, const vector2 *v)
{
    res->x = v->y;
    res->y = -v->x;
}

/**
 * @brief Rotates the input vector counterclockwise by the given angle (radians).
 *
 * Uses standard 2D rotation matrix.
 *
 * @param result The output rotated vector.
 * @param v The input vector.
 * @param angle The rotation angle in radians.
 */
void vec2_rotate_ptr(vector2 *result, const vector2 *v, const vm_float_t angle)
{
    const vm_float_t cs = VECMAT_COS(angle);
    const vm_float_t sn = VECMAT_SIN(angle);
    const vm_float_t x = v->x;
    const vm_float_t y = v->y;
    result->x = x * cs - y * sn;
    result->y = x * sn + y * cs;
}

/**
 * @brief Slides the input vector tangent to the normal (removes normal component).
 *
 * Formula: v - dot(v, normal) * normal (assumes unit normal).
 *
 * @param result The output slid vector.
 * @param v The input vector.
 * @param normal The unit normal vector.
 */
void vec2_slide_ptr(vector2 *result, const vector2 *v, const vector2 *normal)
{
    const vm_float_t d = vec2_dot(*v, *normal);
    result->x = v->x - normal->x * d;
    result->y = v->y - normal->y * d;
}

/**
 * @brief Clamps vector v component-wise between min and max.
 *
 * @param res Output vector.
 * @param v Input vector to clamp.
 * @param min Minimum bounds vector.
 * @param max Maximum bounds vector.
 */
void vec2_clamp_ptr(vector2 *res, const vector2 *v, const vector2 *min, const vector2 *max)
{
    res->x = VECMAT_FMIN(VECMAT_FMAX(v->x, min->x), max->x);
    res->y = VECMAT_FMIN(VECMAT_FMAX(v->y, min->y), max->y);
}

/**
 * @brief Linearly interpolates from a to b by t.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 * @param t Interpolation factor.
 */
void vec2_lerp_ptr(vector2 *res, const vector2 *a, const vector2 *b, const vm_float_t t)
{
    res->x = a->x + t * (b->x - a->x);
    res->y = a->y + t * (b->y - a->y);
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec2_div_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    res->x = (b->x == 0.0f) ? 0.0f : a->x / b->x;
    res->y = (b->y == 0.0f) ? 0.0f : a->y / b->y;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec2_add_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec2_sub_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
}

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
void vec2_clamp_scalar_ptr(vector2 *res, const vector2 *v, const vm_float_t min, const vm_float_t max)
{
    res->x = VECMAT_FMIN(VECMAT_FMAX(v->x, min), max);
    res->y = VECMAT_FMIN(VECMAT_FMAX(v->y, min), max);
}

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_saturate_ptr(vector2 *res, const vector2 *v)
{
    vec2_clamp_scalar_ptr(res, v, 0.0f, 1.0f);
}

/**
 * @brief Returns the fractional part of each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec2_fract_ptr(vector2 *res, const vector2 *v)
{
    res->x = v->x - VECMAT_FLOOR(v->x);
    res->y = v->y - VECMAT_FLOOR(v->y);
}

/**
 * @brief Computes the refraction of incident across normal with ratio eta.
 *
 * @param res Output vector.
 * @param incident Incident vector.
 * @param normal Surface normal.
 * @param eta Ratio of indices of refraction.
 */
void vec2_refract_ptr(vector2 *res, const vector2 *incident, const vector2 *normal, const vm_float_t eta)
{
    const vm_float_t dot = vec2_dot(*incident, *normal);
    const vm_float_t k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f) {
        res->x = 0.0f;
        res->y = 0.0f;
        return;
    }
    const vm_float_t factor = eta * dot + VECMAT_SQRT(k);
    res->x = eta * incident->x - factor * normal->x;
    res->y = eta * incident->y - factor * normal->y;
}

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec2_reject_ptr(vector2 *res, const vector2 *a, const vector2 *b)
{
    vector2 projected;
    vec2_project_ptr(&projected, a, b);
    res->x = a->x - projected.x;
    res->y = a->y - projected.y;
}

/**
 * @brief Rotates v around pivot by angle radians.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param pivot Rotation pivot.
 * @param angle Angle in radians.
 */
void vec2_rotate_around_ptr(vector2 *res, const vector2 *v, const vector2 *pivot, const vm_float_t angle)
{
    vector2 offset;
    offset.x = v->x - pivot->x;
    offset.y = v->y - pivot->y;
    vec2_rotate_ptr(res, &offset, angle);
    res->x += pivot->x;
    res->y += pivot->y;
}

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * @param res Output vector.
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 */
void vec2_move_toward_ptr(vector2 *res, const vector2 *current, const vector2 *target, const vm_float_t max_delta)
{
    const vm_float_t dx = target->x - current->x;
    const vm_float_t dy = target->y - current->y;
    const vm_float_t dist = VECMAT_SQRT(dx * dx + dy * dy);
    if (dist <= max_delta || dist == 0.0f) {
        *res = *target;
        return;
    }
    const vm_float_t scale = max_delta / dist;
    res->x = current->x + dx * scale;
    res->y = current->y + dy * scale;
}

/**
 * @brief Clamps the vector length to max_len.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param max_len Maximum length.
 */
void vec2_limit_length_ptr(vector2 *res, const vector2 *v, const vm_float_t max_len)
{
    const vm_float_t len = vec2_length(*v);
    if (len <= max_len || len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t scale = max_len / len;
    res->x = v->x * scale;
    res->y = v->y * scale;
}

/**
 * @brief Converts a vector2 to a vector3 with the given z.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
void vec2_to_vec3_ptr(vector3 *res, const vector2 *v, const vm_float_t z)
{
    res->x = v->x;
    res->y = v->y;
    res->z = z;
}
