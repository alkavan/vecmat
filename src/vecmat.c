// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>

/**
 * @brief Converts degrees to radians.
 *
 * @param degrees Angle in degrees.
 * @return The angle in radians.
 */
vm_float_t deg_to_rad(const vm_float_t degrees)
{
    return (vm_float_t)((double)degrees * VM_DEG_TO_RAD);
}

/**
 * @brief Converts radians to degrees.
 *
 * @param radians Angle in radians.
 * @return The angle in degrees.
 */
vm_float_t rad_to_deg(const vm_float_t radians)
{
    return (vm_float_t)((double)radians * VM_RAD_TO_DEG);
}

/**
 * @brief Returns true if two vectors are equal within `VECMAT_EPSILON`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are approximately equal.
 */
bool vec2_eq(const vector2 a, const vector2 b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if two vectors are equal within `VECMAT_EPSILON`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are approximately equal.
 */
bool vec3_eq(const vector3 a, const vector3 b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON
        && VECMAT_FABS(a.z - b.z) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if two vectors are equal within `VECMAT_EPSILON`.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are approximately equal.
 */
bool vec4_eq(const vector4 a, const vector4 b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON
        && VECMAT_FABS(a.z - b.z) < VECMAT_EPSILON
        && VECMAT_FABS(a.w - b.w) < VECMAT_EPSILON;
}

/**
 * @brief Returns true if two vectors are exactly equal.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are equal.
 */
bool vec2i_eq(const vector2i a, const vector2i b)
{
    return a.x == b.x && a.y == b.y;
}

/**
 * @brief Returns true if two vectors are exactly equal.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are equal.
 */
bool vec3i_eq(const vector3i a, const vector3i b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

/**
 * @brief Returns true if two vectors are exactly equal.
 *
 * @param a First vector.
 * @param b Second vector.
 * @return True if a and b are equal.
 */
bool vec4i_eq(const vector4i a, const vector4i b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

/**
 * @brief Returns true if two matrices are equal within `VECMAT_EPSILON`.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are approximately equal.
 */
bool mat2_eq(const matrix2 a, const matrix2 b)
{
    for (int i = 0; i < 4; i++) {
        if (VECMAT_FABS(a.v[i] - b.v[i]) >= VECMAT_EPSILON) return false;
    }
    return true;
}

/**
 * @brief Returns true if two matrices are equal within `VECMAT_EPSILON`.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are approximately equal.
 */
bool mat3_eq(const matrix3 a, const matrix3 b)
{
    for (int i = 0; i < 9; i++) {
        if (VECMAT_FABS(a.v[i] - b.v[i]) >= VECMAT_EPSILON) return false;
    }
    return true;
}

/**
 * @brief Returns true if two matrices are equal within `VECMAT_EPSILON`.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are approximately equal.
 */
bool mat4_eq(const matrix4 a, const matrix4 b)
{
    for (int i = 0; i < 16; i++) {
        if (VECMAT_FABS(a.v[i] - b.v[i]) >= VECMAT_EPSILON) return false;
    }
    return true;
}

/**
 * @brief Returns true if two matrices are exactly equal.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are equal.
 */
bool mat2i_eq(const matrix2i a, const matrix2i b)
{
    for (int i = 0; i < 4; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

/**
 * @brief Returns true if two matrices are exactly equal.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are equal.
 */
bool mat3i_eq(const matrix3i a, const matrix3i b)
{
    for (int i = 0; i < 9; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

/**
 * @brief Returns true if two matrices are exactly equal.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True if a and b are equal.
 */
bool mat4i_eq(const matrix4i a, const matrix4i b)
{
    for (int i = 0; i < 16; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

/**
 * @brief Returns true if two quaternions are equal within `VECMAT_EPSILON`.
 *
 * @param a First quaternion.
 * @param b Second quaternion.
 * @return True if a and b are approximately equal.
 */
bool quat_eq(const quaternion a, const quaternion b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON
        && VECMAT_FABS(a.z - b.z) < VECMAT_EPSILON
        && VECMAT_FABS(a.w - b.w) < VECMAT_EPSILON;
}

/**
 * @brief Constructs a vector2 from x and y.
 *
 * @param x X component.
 * @param y Y component.
 * @return The resulting vector2.
 */
vector2 vec2(const vm_float_t x, const vm_float_t y)
{
    return (vector2){ .x = x, .y = y };
}

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
void vec2_assign(vector2 *dest, const vector2 *src)
{
    dest->x = src->x;
    dest->y = src->y;
}

/**
 * @brief Assigns x and y to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 */
void vec2_assign_xy(vector2 *dest, const vm_float_t x, const vm_float_t y)
{
    dest->x = x;
    dest->y = y;
}

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
void vec2_add_assign(vector2 *dest, const vector2 *src)
{
    dest->x += src->x;
    dest->y += src->y;
}

/**
 * @brief Converts a `vector2i` to a `vector2`.
 *
 * @param v Input integer vector.
 * @return The resulting vector2.
 */
vector2 vec2_from(const vector2i *v)
{
    return (vector2){ .x = (vm_float_t)v->x, .y = (vm_float_t)v->y };
}

/**
 * @brief Constructs a `vector2i` from x and y.
 *
 * @param x X component.
 * @param y Y component.
 * @return The resulting vector2i.
 */
vector2i vec2i(const vm_int_t x, const vm_int_t y)
{
    return (vector2i){ .x = x, .y = y };
}

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
void vec2i_assign(vector2i *dest, const vector2i *src)
{
    dest->x = src->x;
    dest->y = src->y;
}

/**
 * @brief Assigns x and y to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 */
void vec2i_assign_xy(vector2i *dest, const vm_int_t x, const vm_int_t y)
{
    dest->x = x;
    dest->y = y;
}

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
void vec2i_add_assign(vector2i *dest, const vector2i *src)
{
    dest->x += src->x;
    dest->y += src->y;
}

/**
 * @brief Converts a `vector2` to a `vector2i` by truncation.
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
vector2i vec2i_from(const vector2 *v)
{
    return (vector2i){ .x = (vm_int_t)v->x, .y = (vm_int_t)v->y };
}

/**
 * @brief Converts a `vector2` to a `vector2i` by flooring each component.
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
vector2i vec2i_from_floored(const vector2 *v)
{
    return (vector2i){ .x = (vm_int_t)VECMAT_FLOOR(v->x), .y = (vm_int_t)VECMAT_FLOOR(v->y) };
}

/**
 * @brief Converts a `vector2` to a `vector2i` by rounding each component.
 *
 * @param v Input vector.
 * @return The resulting vector2i.
 */
vector2i vec2i_from_rounded(const vector2 *v)
{
    return (vector2i){ .x = (vm_int_t)VECMAT_ROUND(v->x), .y = (vm_int_t)VECMAT_ROUND(v->y) };
}

/**
 * @brief Constructs a `vector3` from x, y, and z.
 *
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 * @return The resulting vector3.
 */
vector3 vec3(const vm_float_t x, const vm_float_t y, const vm_float_t z)
{
    return (vector3){ .x = x, .y = y, .z = z };
}

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
void vec3_assign(vector3 *dest, const vector3 *src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

/**
 * @brief Assigns x, y, and z to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
void vec3_assign_xyz(vector3 *dest, const vm_float_t x, const vm_float_t y, const vm_float_t z)
{
    dest->x = x;
    dest->y = y;
    dest->z = z;
}

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
void vec3_add_assign(vector3 *dest, const vector3 *src)
{
    dest->x += src->x;
    dest->y += src->y;
    dest->z += src->z;
}

/**
 * @brief Converts a vector3i to a vector3.
 *
 * @param v Input integer vector.
 * @return The resulting vector3.
 */
vector3 vec3_from(const vector3i *v)
{
    return (vector3){
        .x = (vm_float_t)v->x,
        .y = (vm_float_t)v->y,
        .z = (vm_float_t)v->z
    };
}

/**
 * @brief Constructs a `vector3i` from x, y, and z.
 *
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 * @return The resulting vector3i.
 */
vector3i vec3i(const vm_int_t x, const vm_int_t y, const vm_int_t z)
{
    return (vector3i){ .x = x, .y = y, .z = z };
}

/**
 * @brief Copies src into dest.
 *
 * @param dest Destination vector.
 * @param src Source vector.
 */
void vec3i_assign(vector3i *dest, const vector3i *src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

/**
 * @brief Assigns x, y, and z to dest.
 *
 * @param dest Destination vector.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
void vec3i_assign_xyz(vector3i *dest, const vm_int_t x, const vm_int_t y, const vm_int_t z)
{
    dest->x = x;
    dest->y = y;
    dest->z = z;
}

/**
 * @brief Adds src to dest in place.
 *
 * @param dest Destination vector.
 * @param src Vector to add.
 */
void vec3i_add_assign(vector3i *dest, const vector3i *src)
{
    dest->x += src->x;
    dest->y += src->y;
    dest->z += src->z;
}

/**
 * @brief Converts a `vector3` to a `vector3i` by truncation.
 *
 * @param v Input vector.
 * @return The resulting vector3i.
 */
vector3i vec3i_from(const vector3 *v)
{
    return (vector3i){
        .x = (vm_int_t)v->x,
        .y = (vm_int_t)v->y,
        .z = (vm_int_t)v->z
    };
}

/**
 * @brief Converts a `vector3` to a `vector3i` by flooring each component.
 *
 * @param v Input vector.
 * @return The resulting vector3i.
 */
vector3i vec3i_from_floored(const vector3 *v)
{
    return (vector3i){
        .x = (vm_int_t)VECMAT_FLOOR(v->x),
        .y = (vm_int_t)VECMAT_FLOOR(v->y),
        .z = (vm_int_t)VECMAT_FLOOR(v->z)
    };
}

/**
 * @brief Converts a `vector3` to a `vector3i` by rounding each component.
 *
 * @param v Input vector.
 * @return The resulting vector3i.
 */
vector3i vec3i_from_rounded(const vector3 *v)
{
    return (vector3i){
        .x = (vm_int_t)VECMAT_ROUND(v->x),
        .y = (vm_int_t)VECMAT_ROUND(v->y),
        .z = (vm_int_t)VECMAT_ROUND(v->z)
    };
}
