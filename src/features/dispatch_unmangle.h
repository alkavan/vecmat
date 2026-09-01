// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_DISPATCH_UNMANGLE_H
#define VECMAT_DISPATCH_UNMANGLE_H

/*
 * vecmat_abi.h #defines public names (vec4_add_ptr → vec4_add_ptr32).
 * VECMAT_DISPATCH_LIST(X) writes those tokens as macro arguments.
 */
#undef vec4_add_ptr
#undef vec4_sub_ptr
#undef vec4_mul_ptr
#undef vec4_mul_scalar_ptr
#undef vec4_div_scalar_ptr
#undef vec4_neg_ptr
#undef vec4_abs_ptr
#undef vec4_normalize_ptr
#undef vec4_min_ptr
#undef vec4_max_ptr
#undef vec4_lerp_ptr
#undef vec4_clamp_ptr
#undef vec4_div_ptr
#undef vec4_add_scalar_ptr
#undef vec4_sub_scalar_ptr
#undef vec4_clamp_scalar_ptr
#undef vec4_saturate_ptr
#undef vec4_sign_ptr
#undef vec4_floor_ptr
#undef vec4_ceil_ptr
#undef vec4_round_ptr
#undef vec4_fract_ptr
#undef vec4_homogenize_ptr
#undef mat4_mul_ptr
#undef mat4_transpose_ptr
#undef mat4_mul_vec4_ptr
#undef mat4_mul_vec3_ptr
#undef quat_mul_ptr
#undef quat_normalize_ptr
#undef vm_gemm_ukernel

#endif /* VECMAT_DISPATCH_UNMANGLE_H */
