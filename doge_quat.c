#include "doge_quat.h"
#include <math.h>

#include "header.h"

//TODO: implement from euler angle, slerp and rotation between vectors
// consider normalizing the result of mul to avoid floating point issues
doge_quat_t doge_quat_create(float x, float y, float z, float w)
{
    doge_quat_t q;
    q.v = create_vec3(x,y,z);
    q.w = w;
    return q;
}

doge_quat_t doge_quat_from_axis_angle(float ax, float ay, float az, float angle)
{
    doge_quat_t q;
    float s = sin(angle/2);
    q.v = create_vec3(ax*s,ay*s,az*s);
    q.w = cos(angle/2);
    return q;
}

doge_quat_t doge_quat_conjugated(doge_quat_t q)
{
    q.v = multiply_vec3(q.v,-1);
    return q;
}

void doge_quat_conjugate(doge_quat_t * q)
{
    q->v.x *= -1;
    q->v.x *= -1;
    q->v.z *= -1;
}

doge_quat_t doge_quat_mul(doge_quat_t q1, doge_quat_t q2)
{
    doge_quat_t q;
    q.w = q1.w * q2.w - dot_vec3(q1.v, q2.v);
    q.v = sum_vec3(sum_vec3(multiply_vec3(q1.v,q2.w),multiply_vec3(q2.v,q1.w)),scale_vec3(q1.v, q2.v));
    // q.v = doge_vec3_add(doge_vec3_add(doge_vec3_scaled(q1.v, q2.w), doge_vec3_scaled(q2.v, q1.w)), doge_vec3_cross(q1.v, q2.v));
    return q;
}

struct doge_vec3 doge_quat_rotated_vec(struct doge_vec3 v, doge_quat_t q)
{
    doge_quat_t v_quat;
    v_quat.w = 0;
    v_quat.v = q.v;

    struct doge_vec3 cross = scale_vec3(q.v,v);

    return sum_vec3(sum_vec3(multiply_vec3(cross,q.w*2),v), multiply_vec3(scale_vec3(q.v,cross),2));
    // return doge_vec3_add(doge_vec3_add(doge_vec3_scaled(cross, q.w * 2), v), doge_vec3_scaled(doge_vec3_cross(q.v, cross), 2));
}