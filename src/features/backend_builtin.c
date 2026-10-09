// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "cpu.h"

#include <stddef.h>
#include <vecmat.h>

#if defined(VECMAT_RUNTIME_DISPATCH) && defined(VECMAT_ENABLE_NEON)
static const vm_backend_ops vm_neon_ops = {
    vec4_add_ptr_neon,
    vec4_sub_ptr_neon,
    vec4_mul_ptr_neon,
    vec4_mul_scalar_ptr_neon,
    vec4_div_scalar_ptr_neon,
    vec4_neg_ptr_neon,
    vec4_abs_ptr_neon,
    vec4_normalize_ptr_neon,
    vec4_min_ptr_neon,
    vec4_max_ptr_neon,
    vec4_lerp_ptr_neon,
    vec4_clamp_ptr_neon,
    vec4_div_ptr_neon,
    vec4_add_scalar_ptr_neon,
    vec4_sub_scalar_ptr_neon,
    vec4_clamp_scalar_ptr_neon,
    vec4_saturate_ptr_neon,
    vec4_sign_ptr_neon,
    vec4_floor_ptr_neon,
    vec4_ceil_ptr_neon,
    vec4_round_ptr_neon,
    vec4_fract_ptr_neon,
    vec4_homogenize_ptr_neon,
    mat4_mul_ptr_neon,
    mat4_transpose_ptr_neon,
    mat4_mul_vec4_ptr_neon,
    mat4_mul_vec3_ptr_neon,
    quat_mul_ptr_neon,
    quat_normalize_ptr_neon,
    vm_gemm_ukernel_neon
};
#endif

const vm_backend_ops *vm_backend_builtin(const vm_cpu_features_t isa)
{
#if defined(VECMAT_RUNTIME_DISPATCH) && defined(VECMAT_ENABLE_NEON)
    if (isa == VM_CPU_NEON)
        return &vm_neon_ops;
#endif
    (void)isa;
    return NULL;
}
