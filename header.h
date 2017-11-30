// #pragma once
#include "math.h"
#include <SDL2/SDL.h>
#include "doge_quat.h"

typedef struct camera
{
    struct doge_vec3 position;
    float fov_y;
}camera_t;
typedef struct vertex
{
    struct doge_vec3 pos;
    struct doge_vec3 normal;
    struct doge_vec2 uv;
    
    // the vertex projected on the screen
    struct doge_vec2 projected_position;

        // the vertex transformed by the mesh transform
    struct doge_vec3 world_position;
        // WorldPosition moved to camera transform
    struct doge_vec3  world_view_position;

        // the vertex normal in world space
    struct doge_vec3 world_normal;
} vertex_t;
vertex_t create_vertex(vec3_t,vec3_t,vec2_t);
void vertex_project2D(camera_t, vertex_t*,vec3_t , vec3_t, float, float);
typedef struct world
{
    float depth_buffer[1200*1200];
} world_t;
typedef struct triangle
{
    vertex_t a;
    vertex_t b;
    vertex_t c;

    struct triangle* next;
    struct triangle* prev;

    void* owner;
}triangle_t;
// private void Scanline(int y, Vertex leftA, Vertex leftB, Vertex rightA, Vertex rightB)
void triangle_scanline(world_t*,int , vertex_t,vertex_t,vertex_t,vertex_t,SDL_Renderer*);


typedef struct mesh
{
    triangle_t* head;
    triangle_t* tail;

    struct doge_vec3 scale;
    struct doge_vec3 position;
    doge_quat_t rotation;
}mesh_t;
int mesh_triangles_count(mesh_t);

void triangle_draw(world_t*,triangle_t,camera_t,vec3_t,vec3_t, SDL_Renderer*,float ,float );
void triangle_draw_no_owner(triangle_t,camera_t, SDL_Renderer*,float ,float );

void mesh_draw(world_t*,mesh_t,camera_t,SDL_Renderer*,float,float);

triangle_t* create_triangle(vertex_t,vertex_t ,vertex_t );
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

typedef struct point_light
{
    vec3_t position;
} point_light_t;
point_light_t create_light(vec3_t);


//implement create list and append (like alredy implemented)
list_t* create_list();
list_item_t* get_item_at_index(list_t*,int);
list_item_t* create_list_item(void*); //take data
void list_append(list_t*, void*);
int list_len(list_t);

void manage_scale(mesh_t*, SDL_Event*);
void manage_position(mesh_t*,SDL_Event*);
void rotate_around(struct doge_vec3 , mesh_t*,float ,float);

float sign(struct doge_vec3,struct doge_vec3,struct doge_vec3);
void RasterizationBoundingBox(struct doge_vec3,struct doge_vec3,struct doge_vec3, SDL_Renderer*);
int PointInTriangle(struct doge_vec3,struct doge_vec3,struct doge_vec3,struct doge_vec3);


void init_depth_buffer(world_t*);
//take new z and modify the current. return 0 if you can draw pixel
int check_depth_buffer(world_t*,float,float,float,float);