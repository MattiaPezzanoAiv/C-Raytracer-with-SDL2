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

void init_depth_buffer(world_t*);
//take new z and modify the current. return 0 if you can draw pixel
int check_depth_buffer(world_t*,float,float,float,float);


typedef struct obj_parser_context
{
    int counter_v;
    int counter_vn;
    int counter_vt;
    int counter_f; //numbers of triangle

    list_t* list_v;
    list_t* list_vn;
    list_t* list_vt;
    list_t* list_f; 
} obj_parser_context_t;
obj_parser_context_t obj_parser_init();
void obj_parser_destroy(); //free memory

//return an int parse from string
int obj_parse_int(char*); 

//parse a line in to vec3 separated from space
vec3_t obj_parse_line_vec3(char*);

//parse a line in to a vec2 (uv) separated from space
vec2_t obj_parse_line_uv(char*);

//parse a string separated from / returning a vertex
vertex_t obj_parse_vertex(char*);

//parse line returning a new triangle
triangle_t obj_parse_line_triangle(char*);

//parse an entire line checking line header (v, vn, vt, f)
void obj_parse_line(char*, obj_parser_context_t*);

//get file path and parse obj format file
mesh_t* obj_parse(char*); 
