#pragma once
#ifndef DOGE_QUAT_H
#define DOGE_QUAT_H

#include "math.h"

#define DOGE_2PI        6.28318530718;
#define DOGE_3PI_OVER2  4.71238898038;
#define DOGE_PI         3.14159265359;
#define DOGE_PI_OVER2   1.57079632679;
#define DOGE_PI_OVER3   1.0471975512;
#define DOGE_PI_OVER4   0.78539816339;   
#define DOGE_PI_OVER6   0.52359877559;   

typedef struct doge_quat
{
	float w;
    struct doge_vec3 v;
}doge_quat_t;

// const doge_quat_t indentity = { 1.0f, 0.0f, 0.0f, 0.0f};



doge_quat_t doge_quat_create(float, float, float, float);

//first around y, then around z, then around x
doge_quat_t doge_quat_from_euler(float, float, float);

//assumes normalized axis
doge_quat_t doge_quat_from_axis_angle(float, float, float, float);

doge_quat_t doge_quat_conjugated(doge_quat_t);

void doge_quat_conjugate(doge_quat_t *);

// the second is applied first
// w = w1 * w2 - v1 . v2, v = w1 * v2 + w2 * v1 + v1 x v2
doge_quat_t doge_quat_mul(doge_quat_t, doge_quat_t);

struct doge_vec3 doge_quat_rotated_vec(struct doge_vec3, doge_quat_t);

doge_quat_t doge_quat_slerp(doge_quat_t, doge_quat_t, float);

doge_quat_t doge_quat_rot_from_to(struct doge_vec3, struct doge_vec3);

#endif