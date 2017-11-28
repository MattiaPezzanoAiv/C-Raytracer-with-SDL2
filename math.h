#pragma once

#include <stdio.h>



typedef struct doge_vec2
{
	float x;
	float y;
}vec2_t;

typedef struct doge_vec3
{
	float x;
	float y;
	float z;
} vec3_t;

union doge_vec
{
	struct doge_vec2 vec2;
	struct doge_vec3 vec3;
};

//float
float interpolate_float(float,float,float);

//vector 2
struct doge_vec2 sum_vec2(struct doge_vec2, struct doge_vec2);
struct doge_vec2 sub_vec2(struct doge_vec2, struct doge_vec2);
struct doge_vec2 scale_vec2(struct doge_vec2, struct doge_vec2);
float slow_len_vec2(struct  doge_vec2); //use this for precise misuration of distance
float fast_len_vec2(struct doge_vec2); //use this for fast comparison 
float distance_vec2(struct doge_vec2,struct doge_vec2);
int normalize_vec2(struct doge_vec2*); //normalize this vector (non alloc)
struct doge_vec2 create_vec2(float,float);
struct doge_vec2 interpolate_vec2(struct doge_vec2,struct doge_vec2, float );
struct doge_vec2 multiply_vec2(struct doge_vec2, float );

//vector 3
struct doge_vec3 sum_vec3(struct doge_vec3, struct doge_vec3);
struct doge_vec3 sub_vec3(struct doge_vec3, struct doge_vec3);
struct doge_vec3 scale_vec3(struct doge_vec3, struct doge_vec3);
float slow_len_vec3(struct  doge_vec3);
float fast_len_vec3(struct doge_vec3);
float distance_vec3(struct doge_vec3, struct doge_vec3);
int normalize_vec3(struct doge_vec3*);
struct doge_vec3 multiply_vec3(struct doge_vec3,float);
float dot_vec3(struct doge_vec3 , struct doge_vec3 );
struct doge_vec3 normalized_vec3(struct doge_vec3);
struct doge_vec3 interpolate_vec3(struct doge_vec3,struct doge_vec3, float );
struct doge_vec3 reflect_vec3(struct doge_vec3 , struct doge_vec3 );
