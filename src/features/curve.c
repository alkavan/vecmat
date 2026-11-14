// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "vecmat/curve.h"

vm_float_t vm_bezier2(const vm_float_t p0, const vm_float_t p1, const vm_float_t p2, const vm_float_t t)
{
    const vm_float_t u = VM_F(1.0) - t;
    return u * u * p0 + VM_F(2.0) * u * t * p1 + t * t * p2;
}

vm_float_t vm_bezier3(const vm_float_t p0, const vm_float_t p1, const vm_float_t p2, const vm_float_t p3,
                      const vm_float_t t)
{
    const vm_float_t u = VM_F(1.0) - t;
    const vm_float_t uu = u * u;
    const vm_float_t tt = t * t;
    return uu * u * p0 + VM_F(3.0) * uu * t * p1 + VM_F(3.0) * u * tt * p2 + tt * t * p3;
}

vm_float_t vm_hermite(const vm_float_t p0, const vm_float_t p1, const vm_float_t m0, const vm_float_t m1,
                      const vm_float_t t)
{
    const vm_float_t t2 = t * t;
    const vm_float_t t3 = t2 * t;
    const vm_float_t h00 = VM_F(2.0) * t3 - VM_F(3.0) * t2 + VM_F(1.0);
    const vm_float_t h10 = t3 - VM_F(2.0) * t2 + t;
    const vm_float_t h01 = VM_F(-2.0) * t3 + VM_F(3.0) * t2;
    const vm_float_t h11 = t3 - t2;
    return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
}

vm_float_t vm_catmull_rom(const vm_float_t p0, const vm_float_t p1, const vm_float_t p2, const vm_float_t p3,
                          const vm_float_t t)
{
    const vm_float_t t2 = t * t;
    const vm_float_t t3 = t2 * t;
    return VM_F(0.5) * ((VM_F(2.0) * p1)
                        + (-p0 + p2) * t
                        + (VM_F(2.0) * p0 - VM_F(5.0) * p1 + VM_F(4.0) * p2 - p3) * t2
                        + (-p0 + VM_F(3.0) * p1 - VM_F(3.0) * p2 + p3) * t3);
}

vector2 vec2_bezier2(const vector2 p0, const vector2 p1, const vector2 p2, const vm_float_t t)
{
    return vec2(vm_bezier2(p0.x, p1.x, p2.x, t), vm_bezier2(p0.y, p1.y, p2.y, t));
}

vector2 vec2_bezier3(const vector2 p0, const vector2 p1, const vector2 p2, const vector2 p3, const vm_float_t t)
{
    return vec2(vm_bezier3(p0.x, p1.x, p2.x, p3.x, t),
                vm_bezier3(p0.y, p1.y, p2.y, p3.y, t));
}

vector2 vec2_hermite(const vector2 p0, const vector2 p1, const vector2 m0, const vector2 m1, const vm_float_t t)
{
    return vec2(vm_hermite(p0.x, p1.x, m0.x, m1.x, t),
                vm_hermite(p0.y, p1.y, m0.y, m1.y, t));
}

vector2 vec2_catmull_rom(const vector2 p0, const vector2 p1, const vector2 p2, const vector2 p3, const vm_float_t t)
{
    return vec2(vm_catmull_rom(p0.x, p1.x, p2.x, p3.x, t),
                vm_catmull_rom(p0.y, p1.y, p2.y, p3.y, t));
}

vector3 vec3_bezier2(const vector3 p0, const vector3 p1, const vector3 p2, const vm_float_t t)
{
    return vec3(vm_bezier2(p0.x, p1.x, p2.x, t),
                vm_bezier2(p0.y, p1.y, p2.y, t),
                vm_bezier2(p0.z, p1.z, p2.z, t));
}

vector3 vec3_bezier3(const vector3 p0, const vector3 p1, const vector3 p2, const vector3 p3, const vm_float_t t)
{
    return vec3(vm_bezier3(p0.x, p1.x, p2.x, p3.x, t),
                vm_bezier3(p0.y, p1.y, p2.y, p3.y, t),
                vm_bezier3(p0.z, p1.z, p2.z, p3.z, t));
}

vector3 vec3_hermite(const vector3 p0, const vector3 p1, const vector3 m0, const vector3 m1, const vm_float_t t)
{
    return vec3(vm_hermite(p0.x, p1.x, m0.x, m1.x, t),
                vm_hermite(p0.y, p1.y, m0.y, m1.y, t),
                vm_hermite(p0.z, p1.z, m0.z, m1.z, t));
}

vector3 vec3_catmull_rom(const vector3 p0, const vector3 p1, const vector3 p2, const vector3 p3, const vm_float_t t)
{
    return vec3(vm_catmull_rom(p0.x, p1.x, p2.x, p3.x, t),
                vm_catmull_rom(p0.y, p1.y, p2.y, p3.y, t),
                vm_catmull_rom(p0.z, p1.z, p2.z, p3.z, t));
}
