#pragma once

#include <stdio.h>



struct doge_vec2
{
	float x;
	float y;
};

struct doge_vec3
{
	float x;
	float y;
	float z;
};

union doge_vec
{
	struct doge_vec2 vec2;
	struct doge_vec3 vec3;
};

//vector 2
struct doge_vec2 sum_vec2(struct doge_vec2, struct doge_vec2);
struct doge_vec2 sub_vec2(struct doge_vec2, struct doge_vec2);
struct doge_vec2 scale_vec2(struct doge_vec2, struct doge_vec2);
float slow_len_vec2(struct  doge_vec2); //use this for precise misuration of distance
float fast_len_vec2(struct doge_vec2); //use this for fast comparison 
float distance_vec2(struct doge_vec2,struct doge_vec2);
int normalize_vec2(struct doge_vec2*); //normalize this vector (non alloc)

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

