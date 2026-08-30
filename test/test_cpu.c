// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <vecmat.h>
#include "unitest.h"

TEST_CASE(cpu_features_test, "[cpu]") {
    vm_cpu_init();

    const vm_cpu_features_t compiled = vm_cpu_compiled_features();
    const vm_cpu_features_t runtime = vm_cpu_runtime_features();
    const vm_cpu_features_t selected = vm_cpu_selected_features();

    REQUIRE((compiled & VM_CPU_SCALAR) == VM_CPU_SCALAR);
    REQUIRE((runtime & VM_CPU_SCALAR) == VM_CPU_SCALAR);
    REQUIRE(selected == VM_CPU_SCALAR ||
            selected == VM_CPU_AVX ||
            selected == VM_CPU_AVX2 ||
            selected == VM_CPU_SVE ||
            selected == VM_CPU_AVX512 ||
            selected == VM_CPU_SVE2);
    REQUIRE((selected & compiled) == selected);
    REQUIRE((selected & runtime) == selected);
    REQUIRE(vm_cpu_name(selected) != NULL);
    REQUIRE(vm_cpu_name(VM_CPU_SCALAR)[0] == 's');
    REQUIRE(vm_cpu_name(VM_CPU_AVX512)[0] == 'a');

    if ((compiled & VM_CPU_AVX512) && (runtime & VM_CPU_AVX512))
        REQUIRE(selected == VM_CPU_AVX512);
    if ((compiled & VM_CPU_SVE2) && (runtime & VM_CPU_SVE2))
        REQUIRE(selected == VM_CPU_SVE2);
    REQUIRE(vm_cpu_name(VM_CPU_SVE2)[0] == 's');
}

TEST_CASE(cpu_dispatched_vec4_add_matches_scalar_shape, "[cpu][vector4]") {
    const vector4 a = {.x = 1.0f, .y = -2.0f, .z = 3.5f, .w = 0.25f};
    const vector4 b = {.x = 4.0f, .y = 5.0f, .z = -1.5f, .w = 2.0f};
    vector4 res;
    vec4_add_ptr(&res, &a, &b);
    REQUIRE(FLOAT_EQ(res.x, 5.0f, EPSILON));
    REQUIRE(FLOAT_EQ(res.y, 3.0f, EPSILON));
    REQUIRE(FLOAT_EQ(res.z, 2.0f, EPSILON));
    REQUIRE(FLOAT_EQ(res.w, 2.25f, EPSILON));
}
