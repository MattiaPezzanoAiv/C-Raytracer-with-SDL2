#include <stdlib.h>
#include <math.h>
#include "../include/math.h"

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

