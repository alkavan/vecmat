// SPDX-FileCopyrightText: 2025-2026 Igal Alkon
// SPDX-FileCopyrightText: 2026 ALKONTEK <git@alkontek.com>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef VECMAT_CLIP_H
#define VECMAT_CLIP_H

/**
 * @file
 * @brief Clip-space projections, look-at / look-from, viewport, and world ↔ window helpers.
 * @ingroup vecmat_core
 */

#include "vecmat/types.h"
#include "vecmat/abi.h"

/**
 * @brief Constructs an orthographic projection matrix.
 *
 * @see mat4_ortho_ptr
 *
 * @param left Left clipping plane.
 * @param right Right clipping plane.
 * @param bottom Bottom clipping plane.
 * @param top Top clipping plane.
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return The orthographic matrix4.
 */
VEC_API matrix4 mat4_ortho(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);

/**
 * @brief Constructs a view matrix from eye position, target, and up vector.
 *
 * @see mat4_look_at_ptr
 *
 * @param position Eye position.
 * @param target Target position.
 * @param up Up vector.
 * @return The view matrix4.
 */
VEC_API matrix4 mat4_look_at(vector3 position, vector3 target, vector3 up);

/**
 * @brief Constructs a perspective projection matrix.
 *
 * @see mat4_perspective_ptr
 *
 * @param fov Field of view (radians).
 * @param aspect Aspect ratio (width/height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return The perspective matrix4.
 */
VEC_API matrix4 mat4_perspective(vm_float_t fov, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Constructs a perspective projection matrix using FOV, width, and height.
 *
 * @see mat4_perspective_fov_ptr
 *
 * @param fov Vertical field of view (radians).
 * @param w Viewport width.
 * @param h Viewport height.
 * @param n Near plane.
 * @param f Far plane.
 *
 * @return The perspective matrix4.
 */

VEC_API matrix4 mat4_perspective_fov(vm_float_t fov, vm_float_t w, vm_float_t h, vm_float_t n, vm_float_t f);

/**
 * @brief Constructs an infinite far-plane perspective projection matrix.
 *
 * @see mat4_perspective_infinite_ptr
 *
 * @param fov_y Vertical field of view (radians).
 * @param aspect Aspect ratio.
 * @param n Near plane distance.
 * @return The perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_infinite(vm_float_t fov_y, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite-far perspective with an explicit clip convention.
 *
 * @see mat4_perspective_infinite_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_infinite_clip(vm_float_t fov_y, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Infinite reverse-Z perspective (RH + ZO by default).
 *
 * @see mat4_infinite_reverse_z_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_infinite_reverse_z(vm_float_t fov_y, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite reverse-Z perspective with an explicit clip convention.
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_infinite_reverse_z_clip(vm_float_t fov_y, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Constructs a 4x4 perspective projection matrix from vertical field of view in degrees.
 *
 * @param fov_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @return The perspective projection matrix.
 */
VEC_API matrix4 mat4_perspective_deg(vm_float_t fov_deg, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Constructs a 4x4 perspective projection matrix from vertical field of view in degrees.
 *
 * @param fov_deg Vertical field of view in degrees.
 * @param w Width of the viewport.
 * @param h Height of the viewport.
 * @param n Distance to the near clipping plane.
 * @param f Distance to the far clipping plane.
 *
 * @return The perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_fov_deg(vm_float_t fov_deg, vm_float_t w, vm_float_t h, vm_float_t n, vm_float_t f);

/**
 * @brief Constructs a 4x4 infinite perspective projection matrix using vertical field of view in degrees.
 *
 * @param fov_y_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 *
 * @return The infinite perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_infinite_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite-far perspective from a FOV in degrees.
 *
 * @see mat4_perspective_infinite_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_infinite_clip_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Infinite reverse-Z perspective from a FOV in degrees.
 *
 * @see mat4_infinite_reverse_z_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_infinite_reverse_z_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite reverse-Z perspective (degrees + clip convention).
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_infinite_reverse_z_clip_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Constructs a 4x4 perspective projection matrix.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @param clip Clip space convention.
 * @return The perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_clip(vm_float_t fov_y, vm_float_t aspect, vm_float_t near, vm_float_t far, vm_clip_t clip);

/**
 * @brief Perspective projection from a vertical FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @param clip Clip-space convention.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_clip_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t near, vm_float_t far, vm_clip_t clip);

/**
 * @brief OpenGL-style RH perspective, clip z in [-1, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_rh_no(vm_float_t fov_y, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Vulkan-style RH perspective, clip z in [0, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_rh_zo(vm_float_t fov_y, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Direct3D-style LH perspective, clip z in [0, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_lh_zo(vm_float_t fov_y, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Left-handed perspective, clip z in [-1, 1].
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_lh_no(vm_float_t fov_y, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief OpenGL-style RH perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_rh_no_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Vulkan-style RH perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_rh_zo_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Direct3D-style LH perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_lh_zo_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Left-handed NO perspective from a FOV in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Near plane distance.
 * @param far Far plane distance.
 * @return Perspective matrix4.
 */
VEC_API matrix4 mat4_perspective_lh_no_deg(vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Orthographic projection with an explicit clip convention.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @param clip Clip-space convention.
 * @return Orthographic matrix4.
 */
VEC_API matrix4 mat4_ortho_clip(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far, vm_clip_t clip);

/**
 * @brief OpenGL-style RH orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
VEC_API matrix4 mat4_ortho_rh_no(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);

/**
 * @brief Vulkan-style RH orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
VEC_API matrix4 mat4_ortho_rh_zo(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);

/**
 * @brief Direct3D-style LH orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
VEC_API matrix4 mat4_ortho_lh_zo(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);

/**
 * @brief Left-handed NO orthographic projection.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param left Left plane.
 * @param right Right plane.
 * @param bottom Bottom plane.
 * @param top Top plane.
 * @param near Near plane.
 * @param far Far plane.
 * @return Orthographic matrix4.
 */
VEC_API matrix4 mat4_ortho_lh_no(vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);

/**
 * @brief Look-at view matrix with an explicit clip convention.
 *
 * @see mat4_look_at_clip_ptr
 *
 * @param position Eye position.
 * @param target Point the camera looks at.
 * @param up World up direction.
 * @param clip Clip-space convention.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_at_clip(vector3 position, vector3 target, vector3 up, vm_clip_t clip);

/**
 * @brief Right-handed look-at view matrix.
 *
 * @see mat4_look_at_clip_ptr
 *
 * @param position Eye position.
 * @param target Point the camera looks at.
 * @param up World up direction.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_at_rh(vector3 position, vector3 target, vector3 up);

/**
 * @brief Left-handed look-at view matrix.
 *
 * @see mat4_look_at_clip_ptr
 *
 * @param position Eye position.
 * @param target Point the camera looks at.
 * @param up World up direction.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_at_lh(vector3 position, vector3 target, vector3 up);

/**
 * @brief View matrix from eye position and a look direction.
 *
 * @see mat4_look_from_dir_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_from_dir(vector3 position, vector3 direction, vector3 up);

/**
 * @brief Look-from-direction view matrix with an explicit clip convention.
 *
 * @see mat4_look_from_dir_clip_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @param clip Clip-space convention.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_from_dir_clip(vector3 position, vector3 direction, vector3 up, vm_clip_t clip);

/**
 * @brief Right-handed look-from-direction view matrix.
 *
 * @see mat4_look_from_dir_clip_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_from_dir_rh(vector3 position, vector3 direction, vector3 up);

/**
 * @brief Left-handed look-from-direction view matrix.
 *
 * @see mat4_look_from_dir_clip_ptr
 *
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 * @return View matrix4.
 */
VEC_API matrix4 mat4_look_from_dir_lh(vector3 position, vector3 direction, vector3 up);

/**
 * @brief Pixel-box viewport matrix (NDC xy/z [-1,1] to window + depth [0,1]).
 *
 * @see mat4_viewport_ptr
 *
 * @param x Viewport origin X.
 * @param y Viewport origin Y.
 * @param width Viewport width.
 * @param height Viewport height.
 * @return Viewport matrix4.
 */
VEC_API matrix4 mat4_viewport(vm_float_t x, vm_float_t y, vm_float_t width, vm_float_t height);

/**
 * @brief Viewport matrix with an explicit depth range.
 *
 * @see mat4_viewport_depth_ptr
 *
 * @param x Viewport origin X.
 * @param y Viewport origin Y.
 * @param width Viewport width.
 * @param height Viewport height.
 * @param n Near depth mapped from clip z.
 * @param f Far depth mapped from clip z.
 * @return Viewport matrix4.
 */
VEC_API matrix4 mat4_viewport_depth(vm_float_t x, vm_float_t y, vm_float_t width, vm_float_t height, vm_float_t n, vm_float_t f);

/**
 * @brief Projects a world space point to window coordinates.
 *
 * @see vec3_world_to_window_ptr
 *
 * @param world World space position to project.
 * @param model Model matrix.
 * @param projection Projection matrix.
 * @param viewport Viewport rectangle (x, y, width, height).
 *
 * @return The projected window space vector3.
 */
VEC_API vector3 vec3_world_to_window(vector3 world, matrix4 model, matrix4 projection, vector4 viewport);

/**
 * @brief Un-projects a window coordinate back to world space.
 *
 * @see vec3_window_to_world_ptr
 *
 * @param window The window coordinate to un-project.
 * @param model The model matrix.
 * @param projection The projection matrix.
 * @param viewport The viewport (x, y, width, height).
 * @return The unprojected world space vector3.
 */
VEC_API vector3 vec3_window_to_world(vector3 window, matrix4 model, matrix4 projection, vector4 viewport);

/**
 * @brief Projects a world-space point into window coordinates using model,
 * projection and viewport.
 *
 * @see vec3_world_to_window_clip_ptr.
 *
 * @param world World-space position to project.
 * @param model Model matrix.
 * @param projection Projection matrix.
 * @param viewport Viewport rectangle (x, y, width, height).
 * @param clip Clip-space convention to use.
 * @return The projected window-space coordinates.
 */
VEC_API vector3 vec3_world_to_window_clip(vector3 world, matrix4 model, matrix4 projection, vector4 viewport, vm_clip_t clip);

/**
 * @brief Un-projects a window coordinate using model, projection,
 * viewport and clip space.
 *
 * @see vec3_window_to_world_clip_ptr
 *
 * @param window The window coordinate to un-project.
 * @param model The model matrix.
 * @param projection The projection matrix.
 * @param viewport The viewport (x, y, width, height).
 * @param clip The clip space convention.
 * @return The unprojected vector3.
 */
VEC_API vector3 vec3_window_to_world_clip(vector3 window, matrix4 model, matrix4 projection, vector4 viewport, vm_clip_t clip);

/**
 * @brief Sets the matrix to an orthographic projection matrix.
 *
 * @see mat4_ortho_clip_ptr
 *
 * @param res Pointer to the matrix4 to set to the orthographic projection matrix.
 * @param left The left clipping plane coordinate.
 * @param right The right clipping plane coordinate.
 * @param bottom The bottom clipping plane coordinate.
 * @param top The top clipping plane coordinate.
 * @param near The near clipping plane coordinate.
 * @param far The far clipping plane coordinate.
 */
VEC_API void mat4_ortho_ptr(matrix4 *res, vm_float_t left, vm_float_t right, vm_float_t bottom, vm_float_t top, vm_float_t near, vm_float_t far);

/**
 * @brief Constructs a view matrix for a camera positioned at the given location,
 *        looking towards a target, with a specified up direction.
 *
 * This function computes a 4x4 view matrix that orients the camera at the position vector,
 * directing it towards the target vector, while aligning the up direction.
 * The matrix is stored in column-major order.
 *
 * @param res Pointer to the matrix4 where the result is stored.
 * @param position Pointer to the vector3 representing the camera's position.
 * @param target Pointer to the vector3 representing the point the camera is looking at.
 * @param up Pointer to the vector3 representing the up direction for the camera.
 */
VEC_API void mat4_look_at_ptr(matrix4 *res, const vector3 *position, const vector3 *target, const vector3 *up);

/**
 * @brief View matrix from eye position and a look direction (no target point).
 *
 * Equivalent to `mat4_look_at(position, position + direction, up)` with a
 * default forward if @p direction is near zero.
 *
 * @param res View matrix.
 * @param position Eye position.
 * @param direction Look direction.
 * @param up World up direction.
 */
VEC_API void mat4_look_from_dir_ptr(matrix4 *res, const vector3 *position, const vector3 *direction, const vector3 *up);

/**
 * @brief Clip-aware look-from-direction view matrix.
 *
 * @param res View matrix.
 * @param position Eye position.
 * @param direction World-space look direction (need not be unit).
 * @param up World up hint.
 * @param clip Handedness / depth convention.
 */
VEC_API void mat4_look_from_dir_clip_ptr(matrix4 *res, const vector3 *position, const vector3 *direction,
                                         const vector3 *up, vm_clip_t clip);

/**
 * @brief Viewport matrix from origin `(x, y)` and size `(width, height)`.
 *
 * Maps NDC to window coordinates with depth in `[0, 1]`.
 * @see mat4_viewport_depth_ptr
 *
 * @param res    Output 4×4 matrix.
 * @param x      Viewport origin x.
 * @param y      Viewport origin y.
 * @param width  Viewport width.
 * @param height Viewport height.
 */
VEC_API void mat4_viewport_ptr(matrix4 *res, vm_float_t x, vm_float_t y, vm_float_t width, vm_float_t height);

/**
 * @brief Viewport matrix from origin, size, and depth range `[n, f]`.
 *
 * Maps NDC to window coordinates with depth in `[n, f]`.
 *
 * @param res    Output 4×4 matrix.
 * @param x      Viewport origin x.
 * @param y      Viewport origin y.
 * @param width  Viewport width.
 * @param height Viewport height.
 * @param n      Near depth.
 * @param f      Far depth.
 */
VEC_API void mat4_viewport_depth_ptr(matrix4 *res, vm_float_t x, vm_float_t y, vm_float_t width, vm_float_t height, vm_float_t n, vm_float_t f);

/**
 * @brief Creates a perspective projection matrix.
 *
 * This function computes a right-handed perspective projection matrix based on the given field of view,
 * aspect ratio, and near and far clipping planes. The matrix is stored in the provided result pointer.
 *
 * @param res Pointer to the matrix4 to store the perspective projection matrix.
 * @param fov Field of view in radians.
 * @param aspect Aspect ratio of the viewport (width divided by height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 */
VEC_API void mat4_perspective_ptr(matrix4 *res, vm_float_t fov, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Sets the matrix to a perspective projection matrix.
 *
 * This function constructs a right-handed perspective projection matrix using the specified field of view,
 * viewport width and height, near clipping plane, and far clipping plane.
 *
 * @param res Pointer to the matrix4 to set to the perspective projection matrix.
 * @param fov Field of view angle in radians.
 * @param w Viewport width.
 * @param h Viewport height.
 * @param n Near clipping plane distance.
 * @param f Far clipping plane distance.
 */
VEC_API void mat4_perspective_fov_ptr(matrix4 *res, vm_float_t fov, vm_float_t w, vm_float_t h, vm_float_t n, vm_float_t f);

/**
 * @brief Sets the matrix to an infinite perspective projection matrix.
 *
 * This function constructs a perspective projection matrix with an infinite
 * far plane, which is useful for rendering scenes where depth precision is less
 * critical beyond the near plane. The matrix is set such that the field of view
 * and aspect ratio are applied, with the near plane at distance n.
 *
 * @param res Pointer to the matrix4 to set to the infinite perspective projection.
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio of the viewport (width / height).
 * @param n Distance to the near clipping plane.
 */
VEC_API void mat4_perspective_infinite_ptr(matrix4 *res, vm_float_t fov_y, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite perspective matrix for a chosen clip convention.
 *
 * No far plane. `fov_y` is in radians.
 *
 * @param res    Output 4×4 matrix.
 * @param fov_y  Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n      Near plane distance.
 * @param clip   Clip space convention (handedness and ZO/NO depth).
 */
VEC_API void mat4_perspective_infinite_clip_ptr(matrix4 *res, vm_float_t fov_y, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Infinite reverse-Z perspective matrix (RH, [0, 1] depth).
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param res    Output 4×4 matrix.
 * @param fov_y  Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n      Near plane distance.
 */
VEC_API void mat4_infinite_reverse_z_ptr(matrix4 *res, vm_float_t fov_y, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite reverse-Z perspective matrix for a chosen clip convention.
 *
 * No far plane; depth is reversed for better precision. `fov_y` is in radians.
 *
 * @param res    Output 4×4 matrix.
 * @param fov_y  Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param n      Near plane distance.
 * @param clip   Clip space convention (handedness and ZO/NO depth).
 */
VEC_API void mat4_infinite_reverse_z_clip_ptr(matrix4 *res, vm_float_t fov_y, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Constructs a perspective projection matrix with configurable clip space.
 *
 * Computes a 4x4 perspective projection matrix based on vertical field of view,
 * aspect ratio, near and far planes, and the desired clip space convention.
 *
 * @param res Pointer to the matrix4 to receive the computed projection matrix.
 * @param fov_y Vertical field of view in radians.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @param clip Clip space convention to use (VM_CLIP_RH_NO, VM_CLIP_RH_ZO, VM_CLIP_LH_ZO or VM_CLIP_LH_NO).
 */
VEC_API void mat4_perspective_clip_ptr(matrix4 *res, vm_float_t fov_y, vm_float_t aspect,
                                       vm_float_t near, vm_float_t far, vm_clip_t clip);

/**
 * @brief Sets the matrix to a perspective projection matrix using field of view in degrees.
 *
 * @see mat4_perspective_ptr
 *
 * @param res Pointer to the matrix4 to receive the perspective projection.
 * @param fov_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 */
VEC_API void mat4_perspective_deg_ptr(matrix4 *res, vm_float_t fov_deg, vm_float_t aspect, vm_float_t near, vm_float_t far);

/**
 * @brief Sets the matrix to a perspective projection matrix using vertical field of view in degrees.
 *
 * @see mat4_perspective_fov_ptr
 *
 * @param res Pointer to the matrix4 to be set.
 * @param fov_deg Vertical field of view angle in degrees.
 * @param w Width of the viewport.
 * @param h Height of the viewport.
 * @param n Distance to the near clipping plane.
 * @param f Distance to the far clipping plane.
 */
VEC_API void mat4_perspective_fov_deg_ptr(matrix4 *res, vm_float_t fov_deg, vm_float_t w, vm_float_t h, vm_float_t n, vm_float_t f);

/**
 * @brief Sets the matrix to an infinite perspective projection matrix using
 *        vertical field of view in degrees.
 *
 * @param res Pointer to the matrix4 to store the resulting projection matrix.
 * @param fov_y_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param n Near clipping plane distance.
 */
VEC_API void mat4_perspective_infinite_deg_ptr(matrix4 *res, vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite perspective matrix from FOV in degrees and clip space.
 *
 * @see mat4_perspective_infinite_clip_ptr
 *
 * @param res       Output 4×4 matrix.
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect    Aspect ratio (width / height).
 * @param n         Near plane distance.
 * @param clip      Clip space convention (handedness and ZO/NO depth).
 */
VEC_API void mat4_perspective_infinite_clip_deg_ptr(matrix4 *res, vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Infinite reverse-Z perspective matrix from a vertical FOV in degrees.
 *
 * @see mat4_infinite_reverse_z_ptr
 *
 * @param res       Output 4×4 matrix.
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect    Aspect ratio (width / height).
 * @param n         Near plane distance.
 */
VEC_API void mat4_infinite_reverse_z_deg_ptr(matrix4 *res, vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n);

/**
 * @brief Infinite reverse-Z perspective matrix from FOV in degrees and clip space.
 *
 * @see mat4_infinite_reverse_z_clip_ptr
 *
 * @param res       Output 4×4 matrix.
 * @param fov_y_deg Vertical field of view in degrees.
 * @param aspect    Aspect ratio (width / height).
 * @param n         Near plane distance.
 * @param clip      Clip space convention (handedness and ZO/NO depth).
 */
VEC_API void mat4_infinite_reverse_z_clip_deg_ptr(matrix4 *res, vm_float_t fov_y_deg, vm_float_t aspect, vm_float_t n, vm_clip_t clip);

/**
 * @brief Sets a perspective projection matrix using vertical field of view in degrees.
 *
 * @see mat4_perspective_clip_ptr
 *
 * @param res Pointer to the matrix4 that will receive the computed projection matrix.
 * @param fov_y_deg Vertical field of view angle in degrees.
 * @param aspect Aspect ratio (width / height).
 * @param near Distance to the near clipping plane.
 * @param far Distance to the far clipping plane.
 * @param clip Clip space convention to use.
 */
VEC_API void mat4_perspective_clip_deg_ptr(matrix4 *res, vm_float_t fov_y_deg, vm_float_t aspect,
                                           vm_float_t near, vm_float_t far, vm_clip_t clip);

/**
 * @brief Sets the matrix to an orthographic projection matrix.
 *
 * Constructs a 4x4 orthographic projection matrix based on the provided frustum bounds
 * and the selected clip space convention.
 *
 * @param res Pointer to the matrix4 to store the result.
 * @param left Left clipping plane.
 * @param right Right clipping plane.
 * @param bottom Bottom clipping plane.
 * @param top Top clipping plane.
 * @param near Near clipping plane.
 * @param far Far clipping plane.
 * @param clip Clip space convention to use (one of the VM_CLIP_* values).
 */
VEC_API void mat4_ortho_clip_ptr(matrix4 *res, vm_float_t left, vm_float_t right, vm_float_t bottom,
                                 vm_float_t top, vm_float_t near, vm_float_t far, vm_clip_t clip);

/**
 * @brief Constructs a look-at view matrix with support for different clip space conventions.
 *
 * Computes a 4x4 view matrix that positions the camera at @p position looking towards @p target,
 * with @p up defining the world up direction. The resulting matrix is affected by the selected
 * clip space handedness and depth range via the @p clip parameter.
 *
 * For right-handed clip conventions the function delegates to mat4_look_at_ptr. For left-handed
 * conventions a dedicated basis is constructed where the camera forward direction points along
 * positive Z.
 *
 * @param res Pointer to the matrix4 that will receive the computed view matrix.
 * @param position Camera position in world space.
 * @param target Target point in world space the camera is looking at.
 * @param up World up direction vector.
 * @param clip Clip space convention that determines handedness and depth range.
 */
VEC_API void mat4_look_at_clip_ptr(matrix4 *res, const vector3 *position, const vector3 *target,
                                   const vector3 *up, vm_clip_t clip);

/**
 * @brief Projects a world-space point to window coordinates (OpenGL clip).
 *
 * @see vec3_world_to_window_clip_ptr
 *
 * @param res Window-space point.
 * @param world World-space position.
 * @param model Model matrix.
 * @param projection Projection matrix.
 * @param viewport Viewport rectangle (x, y, width, height).
 */
VEC_API void vec3_world_to_window_ptr(vector3 *res, const vector3 *world, const matrix4 *model,
                                      const matrix4 *projection, const vector4 *viewport);

/**
 * @brief Un-projects a window-space point to world coordinates using model,
 *        projection, and viewport.
 *
 * Convenience wrapper around `vec3_window_to_world_clip_ptr` with
 * `VM_CLIP_RH_NO` (right-handed, [-1, 1] depth).
 *
 * @param res        Output world-space point.
 * @param window     Window-space point (x, y, depth).
 * @param model      Model matrix.
 * @param projection Projection matrix.
 * @param viewport   Viewport rectangle (x, y, width, height).
 */
VEC_API void vec3_window_to_world_ptr(vector3 *res, const vector3 *window, const matrix4 *model,
                                      const matrix4 *projection, const vector4 *viewport);

/**
 * @brief Projects a world-space point into window coordinates using model,
 *        projection, viewport, and clip depth range.
 *
 * Applies MVP, perspective-divides to NDC, then maps x/y into the viewport.
 * Depth (`res->z`) is stored in the selected clip convention (ZO or NO).
 * If `w` is near zero, writes `(0, 0, 0)`.
 *
 * @param res        Output window-space point (x, y, depth).
 * @param world      World-space position.
 * @param model      Model matrix.
 * @param projection Projection matrix.
 * @param viewport   Viewport rectangle (x, y, width, height).
 * @param clip       Clip depth range (ZO or NO).
 */
VEC_API void vec3_world_to_window_clip_ptr(vector3 *res, const vector3 *world, const matrix4 *model,
                                           const matrix4 *projection, const vector4 *viewport, vm_clip_t clip);

/**
 * @brief Un-projects a window-space point to world coordinates using model,
 *        projection, viewport, and clip depth range.
 *
 * Maps window x/y into NDC, converts depth with the selected clip convention
 * (ZO or NO), then applies the inverse MVP and perspective-divides.
 * If the viewport size or `w` is near zero, writes `(0, 0, 0)`.
 *
 * @param res        Output world-space point.
 * @param window     Window-space point (x, y, depth).
 * @param model      Model matrix.
 * @param projection Projection matrix.
 * @param viewport   Viewport rectangle (x, y, width, height).
 * @param clip       Clip depth range (ZO or NO).
 */
VEC_API void vec3_window_to_world_clip_ptr(vector3 *res, const vector3 *window, const matrix4 *model,
                                           const matrix4 *projection, const vector4 *viewport, vm_clip_t clip);

#endif //VECMAT_CLIP_H
