#include <stdlib.h>
#include <math.h>
#include "math.h"

//VECTOR 2
struct doge_vec2 sum_vec2(struct doge_vec2 a,struct doge_vec2 b)
{
    struct doge_vec2 v;
    v.x = a.x+b.x;
    v.y = a.y+b.y;
    return v;
}
struct doge_vec2 sub_vec2(struct doge_vec2 a,struct doge_vec2 b)
{
    struct doge_vec2 v;
    v.x = a.x-b.x;
    v.y = a.y-b.y;
    return v;
}
struct doge_vec2 scale_vec2(struct doge_vec2 a, struct doge_vec2 b)
{
    struct doge_vec2 v;
    v.x = a.x*b.x;
    v.y = a.y*b.y;
    return v;
}
float slow_len_vec2(struct  doge_vec2 a)
{
   return (float)sqrt(pow(a.x,2)+pow(a.y,2));
}
float fast_len_vec2(struct doge_vec2 a)
{
   return (pow(a.x,2)+pow(a.y,2));
}
float distance_vec2(struct doge_vec2 a,struct doge_vec2 b)
{
    struct doge_vec2 c = sub_vec2(a,b);
    return slow_len_vec2(c);
}
int normalize_vec2(struct doge_vec2* a)
{
    float a_len = slow_len_vec2(*a);
    a->x = a->x / a_len;
    a->y = a->y / a_len;
    return 0;
}
struct doge_vec2 create_vec2(float x,float y)
{
    struct doge_vec2 v;
    v.x = x;
    v.y = y;
    return v;
}
struct doge_vec2 multiply_vec2(struct doge_vec2 v, float n)
{
    struct doge_vec2 new_v;
    new_v.x = v.x*n;
    new_v.y = v.y*n;
    return new_v;    
}

struct doge_vec2 interpolate_vec2(struct doge_vec2 a,struct doge_vec2 b, float gradient)
{
    return sum_vec2(a,multiply_vec2(sub_vec2(b,a),gradient));
}



//VECTOR 3
struct doge_vec3 sum_vec3(struct doge_vec3 a, struct doge_vec3 b)
{
    struct doge_vec3 v;
    v.x = a.x+b.x;
    v.y = a.y+b.y;
    v.z = a.z+b.z;
    return v;
}
struct doge_vec3 sub_vec3(struct doge_vec3 a, struct doge_vec3 b)
{
    struct doge_vec3 v;
    v.x = a.x-b.x;
    v.y = a.y-b.y;
    v.z = a.z-b.z;
    return v;
}
struct doge_vec3 scale_vec3(struct doge_vec3 a, struct doge_vec3 b)
{
    struct doge_vec3 v;
    v.x = a.x*b.x;
    v.y = a.y*b.y;
    v.z = a.z*b.z;
    return v;
}
float slow_len_vec3(struct  doge_vec3 a)
{
   return (float)sqrt(pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
}
float fast_len_vec3(struct doge_vec3 a)
{
   return (pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
}
float distance_vec3(struct doge_vec3 a, struct doge_vec3 b)
{
    struct doge_vec3 c = sub_vec3(a,b);
    return slow_len_vec3(c);
}
int normalize_vec3(struct doge_vec3* a)
{
    float a_len = slow_len_vec3(*a);
    a->x = a->x / a_len;
    a->y = a->y / a_len;
    a->z = a->z / a_len;
    return 0;
}
struct doge_vec3 multiply_vec3(struct doge_vec3 v,float n)
{
    struct doge_vec3 new_v;
    new_v.x = new_v.x * n;
    new_v.y = new_v.y * n;
    new_v.z = new_v.z * n;
    return new_v;
}
float dot_vec3(struct doge_vec3 v1, struct doge_vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

struct doge_vec3 normalized_vec3(struct doge_vec3 v)
{
    float len = slow_len_vec3(v);
    struct doge_vec3 new_v;
    new_v.x = v.x/len;
    new_v.y = v.y/len;
    new_v.z = v.z/len;
    return new_v;
}
struct doge_vec3 reflect_vec3(struct doge_vec3 v1, struct doge_vec3 v2)
{
    float dot = dot_vec3(v1,v2);
    return sub_vec3(v1,multiply_vec3(v2,2*dot)); 
    // return v - 2f * Vector3.Dot(v, n) * n;
}
struct doge_vec3 interpolate_vec3(struct doge_vec3 a,struct doge_vec3 b, float gradient)
{
    return sum_vec3(a,multiply_vec3(sub_vec3(b,a),gradient));
}


//float
float interpolate_float(float a,float b,float gradient)
{
    return a + (b - a) * gradient;
}

vec3_t divide_vec3(vec3_t v,float n)
{
    vec3_t new_v;
    new_v.x = v.x/n;
    new_v.y = v.y/n;
    new_v.z = v.z/n;
    return new_v;    
}

