// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <stdlib.h>
#include <vecmat.h>

vm_float_t deg_to_rad(const vm_float_t degrees)
{
    return (vm_float_t)((double)degrees * VM_DEG_TO_RAD);
}

vm_float_t rad_to_deg(const vm_float_t radians)
{
    return (vm_float_t)((double)radians * VM_RAD_TO_DEG);
}

bool vec2_eq(const vector2 a, const vector2 b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON;
}

bool vec3_eq(const vector3 a, const vector3 b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON
        && VECMAT_FABS(a.z - b.z) < VECMAT_EPSILON;
}

bool vec4_eq(const vector4 a, const vector4 b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON
        && VECMAT_FABS(a.z - b.z) < VECMAT_EPSILON
        && VECMAT_FABS(a.w - b.w) < VECMAT_EPSILON;
}

bool vec2i_eq(const vector2i a, const vector2i b)
{
    return a.x == b.x && a.y == b.y;
}

bool vec3i_eq(const vector3i a, const vector3i b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool vec4i_eq(const vector4i a, const vector4i b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

bool mat2_eq(const matrix2 a, const matrix2 b)
{
    for (int i = 0; i < 4; i++) {
        if (VECMAT_FABS(a.v[i] - b.v[i]) >= VECMAT_EPSILON) return false;
    }
    return true;
}

bool mat3_eq(const matrix3 a, const matrix3 b)
{
    for (int i = 0; i < 9; i++) {
        if (VECMAT_FABS(a.v[i] - b.v[i]) >= VECMAT_EPSILON) return false;
    }
    return true;
}

bool mat4_eq(const matrix4 a, const matrix4 b)
{
    for (int i = 0; i < 16; i++) {
        if (VECMAT_FABS(a.v[i] - b.v[i]) >= VECMAT_EPSILON) return false;
    }
    return true;
}

bool mat2i_eq(const matrix2i a, const matrix2i b)
{
    for (int i = 0; i < 4; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

bool mat3i_eq(const matrix3i a, const matrix3i b)
{
    for (int i = 0; i < 9; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

bool mat4i_eq(const matrix4i a, const matrix4i b)
{
    for (int i = 0; i < 16; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

bool quat_eq(const quaternion a, const quaternion b)
{
    return VECMAT_FABS(a.x - b.x) < VECMAT_EPSILON
        && VECMAT_FABS(a.y - b.y) < VECMAT_EPSILON
        && VECMAT_FABS(a.z - b.z) < VECMAT_EPSILON
        && VECMAT_FABS(a.w - b.w) < VECMAT_EPSILON;
}

vector2 vec2(const vm_float_t x, const vm_float_t y)
{
    return (vector2){ .x = x, .y = y };
}

void vec2_assign(vector2 *dest, const vector2 *src)
{
    dest->x = src->x;
    dest->y = src->y;
}

void vec2_assign_xy(vector2 *dest, const vm_float_t x, const vm_float_t y)
{
    dest->x = x;
    dest->y = y;
}

void vec2_add_assign(vector2 *dest, const vector2 *src)
{
    dest->x += src->x;
    dest->y += src->y;
}

vector2 vec2_from(const vector2i *v)
{
    return (vector2){ .x = (vm_float_t)v->x, .y = (vm_float_t)v->y };
}

vector2i vec2i(const vm_int_t x, const vm_int_t y)
{
    return (vector2i){ .x = x, .y = y };
}

void vec2i_assign(vector2i *dest, const vector2i *src)
{
    dest->x = src->x;
    dest->y = src->y;
}

void vec2i_assign_xy(vector2i *dest, const vm_int_t x, const vm_int_t y)
{
    dest->x = x;
    dest->y = y;
}

void vec2i_add_assign(vector2i *dest, const vector2i *src)
{
    dest->x += src->x;
    dest->y += src->y;
}

vector2i vec2i_from(const vector2 *v)
{
    return (vector2i){ .x = (vm_int_t)v->x, .y = (vm_int_t)v->y };
}

vector2i vec2i_from_floored(const vector2 *v)
{
    return (vector2i){ .x = (vm_int_t)VECMAT_FLOOR(v->x), .y = (vm_int_t)VECMAT_FLOOR(v->y) };
}

vector2i vec2i_from_rounded(const vector2 *v)
{
    return (vector2i){ .x = (vm_int_t)VECMAT_ROUND(v->x), .y = (vm_int_t)VECMAT_ROUND(v->y) };
}

vector3 vec3(const vm_float_t x, const vm_float_t y, const vm_float_t z)
{
    return (vector3){ .x = x, .y = y, .z = z };
}

void vec3_assign(vector3 *dest, const vector3 *src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

void vec3_assign_xyz(vector3 *dest, const vm_float_t x, const vm_float_t y, const vm_float_t z)
{
    dest->x = x;
    dest->y = y;
    dest->z = z;
}

void vec3_add_assign(vector3 *dest, const vector3 *src)
{
    dest->x += src->x;
    dest->y += src->y;
    dest->z += src->z;
}

vector3 vec3_from(const vector3i *v)
{
    return (vector3){
        .x = (vm_float_t)v->x,
        .y = (vm_float_t)v->y,
        .z = (vm_float_t)v->z
    };
}

vector3i vec3i(const vm_int_t x, const vm_int_t y, const vm_int_t z)
{
    return (vector3i){ .x = x, .y = y, .z = z };
}

void vec3i_assign(vector3i *dest, const vector3i *src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

void vec3i_assign_xyz(vector3i *dest, const vm_int_t x, const vm_int_t y, const vm_int_t z)
{
    dest->x = x;
    dest->y = y;
    dest->z = z;
}

void vec3i_add_assign(vector3i *dest, const vector3i *src)
{
    dest->x += src->x;
    dest->y += src->y;
    dest->z += src->z;
}

vector3i vec3i_from(const vector3 *v)
{
    return (vector3i){
        .x = (vm_int_t)v->x,
        .y = (vm_int_t)v->y,
        .z = (vm_int_t)v->z
    };
}

vector3i vec3i_from_floored(const vector3 *v)
{
    return (vector3i){
        .x = (vm_int_t)VECMAT_FLOOR(v->x),
        .y = (vm_int_t)VECMAT_FLOOR(v->y),
        .z = (vm_int_t)VECMAT_FLOOR(v->z)
    };
}

vector3i vec3i_from_rounded(const vector3 *v)
{
    return (vector3i){
        .x = (vm_int_t)VECMAT_ROUND(v->x),
        .y = (vm_int_t)VECMAT_ROUND(v->y),
        .z = (vm_int_t)VECMAT_ROUND(v->z)
    };
}
