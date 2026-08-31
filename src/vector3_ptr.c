// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

/**
 * @brief Component-wise addition of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec3_add_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

/**
 * @brief Component-wise subtraction of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec3_sub_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
}

/**
 * @brief Component-wise multiplication of vector by scalar.
 *
 * @param res Result vector.
 * @param v Vector.
 * @param s Scalar.
 */
void vec3_mul_scalar_ptr(vector3 *res, const vector3 *v, const vm_float_t s)
{
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
}

/**
 * @brief Component-wise division of vector by scalar.
 *
 * @param res Result vector.
 * @param v Vector.
 * @param s Scalar.
 */
void vec3_div_scalar_ptr(vector3 *res, const vector3 *v, const vm_float_t s)
{
    if (s == 0.0f) {
        *res = *v;
        return;
    }
    res->x = v->x / s;
    res->y = v->y / s;
    res->z = v->z / s;
}

/**
 * @brief Component-wise multiplication of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec3_mul_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
}

/**
 * @brief Negation of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_neg_ptr(vector3 *res, const vector3 *v)
{
    res->x = -v->x;
    res->y = -v->y;
    res->z = -v->z;
}

/**
 * @brief Computes the absolute value per component of a `vector3`.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_abs_ptr(vector3 *res, const vector3 *v)
{
    res->x = VECMAT_FABS(v->x);
    res->y = VECMAT_FABS(v->y);
    res->z = VECMAT_FABS(v->z);
}

/**
 * @brief Normalize a vector to unit length.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_normalize_ptr(vector3 *res, const vector3 *v)
{
    const vm_float_t len = vec3_length(*v);
    if (len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t inv_len = 1.0f / len;
    res->x = v->x * inv_len;
    res->y = v->y * inv_len;
    res->z = v->z * inv_len;
}

/**
 * @brief Compute cross-product of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec3_cross_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = a->y * b->z - a->z * b->y;
    res->y = a->z * b->x - a->x * b->z;
    res->z = a->x * b->y - a->y * b->x;
}

/**
 * @brief Component-wise minimum of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec3_min_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = VECMAT_FMIN(a->x, b->x);
    res->y = VECMAT_FMIN(a->y, b->y);
    res->z = VECMAT_FMIN(a->z, b->z);
}

/**
 * @brief Component-wise maximum of two vectors.
 *
 * @param res Result vector.
 * @param a First vector.
 * @param b Second vector.
 */
void vec3_max_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = VECMAT_FMAX(a->x, b->x);
    res->y = VECMAT_FMAX(a->y, b->y);
    res->z = VECMAT_FMAX(a->z, b->z);
}

/**
 * @brief Component-wise sign of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_sign_ptr(vector3 *res, const vector3 *v)
{
    res->x = (v->x > 0.0f) - (v->x < 0.0f);
    res->y = (v->y > 0.0f) - (v->y < 0.0f);
    res->z = (v->z > 0.0f) - (v->z < 0.0f);
}

/**
 * @brief Component-wise floor of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_floor_ptr(vector3 *res, const vector3 *v)
{
    res->x = VECMAT_FLOOR(v->x);
    res->y = VECMAT_FLOOR(v->y);
    res->z = VECMAT_FLOOR(v->z);
}

/**
 * @brief Component-wise ceil of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_ceil_ptr(vector3 *res, const vector3 *v)
{
    res->x = VECMAT_CEIL(v->x);
    res->y = VECMAT_CEIL(v->y);
    res->z = VECMAT_CEIL(v->z);
}

/**
 * @brief Component-wise round of a vector.
 *
 * @param res Result vector.
 * @param v Vector.
 */
void vec3_round_ptr(vector3 *res, const vector3 *v)
{
    res->x = VECMAT_ROUND(v->x);
    res->y = VECMAT_ROUND(v->y);
    res->z = VECMAT_ROUND(v->z);
}

/**
 * @brief Compute reflection of the incident vector over normal.
 *
 * @param res Result vector.
 * @param incident Incident vector.
 * @param normal Normal vector.
 */
void vec3_reflect_ptr(vector3 *res, const vector3 *incident, const vector3 *normal)
{
    const vm_float_t dot = vec3_dot(*incident, *normal);
    res->x = incident->x - 2.0f * dot * normal->x;
    res->y = incident->y - 2.0f * dot * normal->y;
    res->z = incident->z - 2.0f * dot * normal->z;
}

/**
 * @brief Compute refraction of the incident vector through normal with eta.
 *
 * @param res Result vector.
 * @param incident Incident vector.
 * @param normal Normal vector.
 * @param eta Refraction index ratio.
 */
void vec3_refract_ptr(vector3 *res, const vector3 *incident, const vector3 *normal, const vm_float_t eta)
{
    const vm_float_t dot = vec3_dot(*incident, *normal);
    const vm_float_t k = 1.0f - eta * eta * (1.0f - dot * dot);
    if (k < 0.0f) {
        res->x = res->y = res->z = 0.0f;
        return;
    }
    const vm_float_t factor = eta * dot + VECMAT_SQRT(k);
    res->x = eta * incident->x - factor * normal->x;
    res->y = eta * incident->y - factor * normal->y;
    res->z = eta * incident->z - factor * normal->z;
}

/**
 * @brief Linear interpolation between two vectors.
 *
 * @param res Result vector.
 * @param a Start vector.
 * @param b End vector.
 * @param t Interpolation factor.
 */
void vec3_lerp_ptr(vector3 *res, const vector3 *a, const vector3 *b, const vm_float_t t)
{
    res->x = a->x + t * (b->x - a->x);
    res->y = a->y + t * (b->y - a->y);
    res->z = a->z + t * (b->z - a->z);
}

/**
 * @brief Clamp vector components between min and max.
 *
 * @param res Result vector.
 * @param v Vector.
 * @param min Minimum bounds.
 * @param max Maximum bounds.
 */
void vec3_clamp_ptr(vector3 *res, const vector3 *v, const vector3 *min, const vector3 *max)
{
    res->x = VECMAT_FMAX(min->x, VECMAT_FMIN(max->x, v->x));
    res->y = VECMAT_FMAX(min->y, VECMAT_FMIN(max->y, v->y));
    res->z = VECMAT_FMAX(min->z, VECMAT_FMIN(max->z, v->z));
}

/**
 * @brief Scales the vector by a scalar.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec3_scale_ptr(vector3 *res, const vector3 *v, const vm_float_t s)
{
    vec3_mul_scalar_ptr(res, v, s);
}

/**
 * @brief Divides two vectors component-wise.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec3_div_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    res->x = (b->x == 0.0f) ? 0.0f : a->x / b->x;
    res->y = (b->y == 0.0f) ? 0.0f : a->y / b->y;
    res->z = (b->z == 0.0f) ? 0.0f : a->z / b->z;
}

/**
 * @brief Adds a scalar to each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec3_add_scalar_ptr(vector3 *res, const vector3 *v, const vm_float_t s)
{
    res->x = v->x + s;
    res->y = v->y + s;
    res->z = v->z + s;
}

/**
 * @brief Subtracts a scalar from each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param s Scalar value.
 */
void vec3_sub_scalar_ptr(vector3 *res, const vector3 *v, const vm_float_t s)
{
    res->x = v->x - s;
    res->y = v->y - s;
    res->z = v->z - s;
}

/**
 * @brief Clamps each component to the scalar range [min, max].
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param min Lower bound.
 * @param max Upper bound.
 */
void vec3_clamp_scalar_ptr(vector3 *res, const vector3 *v, const vm_float_t min, const vm_float_t max)
{
    res->x = VECMAT_FMIN(VECMAT_FMAX(v->x, min), max);
    res->y = VECMAT_FMIN(VECMAT_FMAX(v->y, min), max);
    res->z = VECMAT_FMIN(VECMAT_FMAX(v->z, min), max);
}

/**
 * @brief Clamps each component to the range [0, 1].
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec3_saturate_ptr(vector3 *res, const vector3 *v)
{
    vec3_clamp_scalar_ptr(res, v, 0.0f, 1.0f);
}

/**
 * @brief Returns the fractional part of each component.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec3_fract_ptr(vector3 *res, const vector3 *v)
{
    res->x = v->x - VECMAT_FLOOR(v->x);
    res->y = v->y - VECMAT_FLOOR(v->y);
    res->z = v->z - VECMAT_FLOOR(v->z);
}

/**
 * @brief Projects a onto b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec3_project_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    const vm_float_t denom = vec3_dot(*b, *b);
    if (denom == 0.0f) {
        res->x = 0.0f;
        res->y = 0.0f;
        res->z = 0.0f;
        return;
    }
    const vm_float_t scale = vec3_dot(*a, *b) / denom;
    res->x = b->x * scale;
    res->y = b->y * scale;
    res->z = b->z * scale;
}

/**
 * @brief Removes the component of v along normal.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param normal Surface normal.
 */
void vec3_slide_ptr(vector3 *res, const vector3 *v, const vector3 *normal)
{
    const vm_float_t d = vec3_dot(*v, *normal);
    res->x = v->x - normal->x * d;
    res->y = v->y - normal->y * d;
    res->z = v->z - normal->z * d;
}

/**
 * @brief Returns the component of a orthogonal to b.
 *
 * @param res Output vector.
 * @param a First input vector.
 * @param b Second input vector.
 */
void vec3_reject_ptr(vector3 *res, const vector3 *a, const vector3 *b)
{
    vector3 projected;
    vec3_project_ptr(&projected, a, b);
    res->x = a->x - projected.x;
    res->y = a->y - projected.y;
    res->z = a->z - projected.z;
}

/**
 * @brief Rotates v around axis by angle radians.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param axis Rotation axis.
 * @param radians Angle in radians.
 */
void vec3_rotate_axis_ptr(vector3 *res, const vector3 *v, const vector3 *axis, const vm_float_t radians)
{
    vector3 k;
    vec3_normalize_ptr(&k, axis);
    const vm_float_t cs = VECMAT_COS(radians);
    const vm_float_t sn = VECMAT_SIN(radians);
    const vm_float_t d = vec3_dot(*v, k);
    res->x = v->x * cs + (k.y * v->z - k.z * v->y) * sn + k.x * d * (1.0f - cs);
    res->y = v->y * cs + (k.z * v->x - k.x * v->z) * sn + k.y * d * (1.0f - cs);
    res->z = v->z * cs + (k.x * v->y - k.y * v->x) * sn + k.z * d * (1.0f - cs);
}

/**
 * @brief Component-wise rotation of a vector around an axis by a given angle in degrees.
 *
 * @param res Result vector.
 * @param v Vector to rotate.
 * @param axis Rotation axis.
 * @param degrees Angle in degrees.
 */
void vec3_rotate_axis_deg_ptr(vector3 *res, const vector3 *v, const vector3 *axis, const vm_float_t degrees)
{
    vec3_rotate_axis_ptr(res, v, axis, deg_to_rad(degrees));
}

/**
 * @brief Builds a higher-dimension vector from a vector2.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param z Z component.
 */
void vec3_from_vec2_ptr(vector3 *res, const vector2 *v, const vm_float_t z)
{
    res->x = v->x;
    res->y = v->y;
    res->z = z;
}

/**
 * @brief Returns the x and y components as a 2D vector.
 *
 * @param res Output vector.
 * @param v Input vector.
 */
void vec3_xy_ptr(vector2 *res, const vector3 *v)
{
    res->x = v->x;
    res->y = v->y;
}

/**
 * @brief Moves current toward target by at most max_delta.
 *
 * @param res Output vector.
 * @param current Current position.
 * @param target Target position.
 * @param max_delta Maximum distance to move.
 */
void vec3_move_toward_ptr(vector3 *res, const vector3 *current, const vector3 *target, const vm_float_t max_delta)
{
    const vm_float_t dx = target->x - current->x;
    const vm_float_t dy = target->y - current->y;
    const vm_float_t dz = target->z - current->z;
    const vm_float_t dist = VECMAT_SQRT(dx * dx + dy * dy + dz * dz);
    if (dist <= max_delta || dist == 0.0f) {
        *res = *target;
        return;
    }
    const vm_float_t scale = max_delta / dist;
    res->x = current->x + dx * scale;
    res->y = current->y + dy * scale;
    res->z = current->z + dz * scale;
}

/**
 * @brief Clamps the vector length to max_len.
 *
 * @param res Output vector.
 * @param v Input vector.
 * @param max_len Maximum length.
 */
void vec3_limit_length_ptr(vector3 *res, const vector3 *v, const vm_float_t max_len)
{
    const vm_float_t len = vec3_length(*v);
    if (len <= max_len || len == 0.0f) {
        *res = *v;
        return;
    }
    const vm_float_t scale = max_len / len;
    res->x = v->x * scale;
    res->y = v->y * scale;
    res->z = v->z * scale;
}

/**
 * @brief Builds a tangent and bitangent orthonormal to n.
 *
 * @param n Unit normal.
 * @param t Output tangent.
 * @param b Output bitangent.
 */
void vec3_orthonormal_basis_ptr(const vector3 *n, vector3 *t, vector3 *b)
{
    vector3 nn;
    vec3_normalize_ptr(&nn, n);
    const vector3 axis = (VECMAT_FABS(nn.x) > 0.9f)
        ? (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f}
        : (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f};
    vec3_cross_ptr(t, &nn, &axis);
    vec3_normalize_ptr(t, t);
    vec3_cross_ptr(b, &nn, t);
}
