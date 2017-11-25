// #pragma once
#include "math.h"
#include <SDL2/SDL.h>
#include "doge_quat.h"

typedef struct triangle
{
    struct doge_vec3 a;
    struct doge_vec3 b;
    struct doge_vec3 c;

    struct triangle* next;
    struct triangle* prev;

    void* owner;
}triangle_t;

typedef struct mesh
{
    triangle_t* head;
    triangle_t* tail;

    struct doge_vec3 scale;
    struct doge_vec3 position;
    doge_quat_t rotation;
}mesh_t;
int mesh_triangles_count(mesh_t);
typedef struct camera
{
    struct doge_vec3 position;
    float fov_y;
}camera_t;
void triangle_draw(triangle_t,camera_t, SDL_Renderer*,float ,float );
void mesh_draw(mesh_t,camera_t,SDL_Renderer*,float,float);

triangle_t* create_triangle(struct doge_vec3 ,struct doge_vec3 ,struct doge_vec3 );
struct doge_vec3 create_vec3(float,float,float);

void mesh_add_triangle(mesh_t*,triangle_t*);

mesh_t* parse_obj(char* );


typedef struct list_item
{
    void* item;
    struct list_item* next;
    struct list_item* prev;
    
} list_item_t;
typedef struct list
{
    list_item_t* head;
    list_item_t* tail;
} list_t;

//implement create list and append (like alredy implemented)
list_t* create_list();
list_item_t* get_item_at_index(list_t*,int);
list_item_t* create_list_item(void*); //take data
void list_append(list_t*, void*);
int list_len(list_t);

void manage_scale(mesh_t*, SDL_Event*);
void manage_position(mesh_t*,SDL_Event*);
void rotate_around(struct doge_vec3 , mesh_t*,float ,float);
