// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>

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
