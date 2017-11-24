#include "math.h"
#include <SDL2/SDL.h>

typedef struct triangle
{
    struct doge_vec3 a;
    struct doge_vec3 b;
    struct doge_vec3 c;

    struct triangle* next;
    struct triangle* prev;
}triangle_t;

typedef struct mesh
{
    triangle_t* head;
    triangle_t* tail;
}mesh_t;

triangle_t create_triangle(struct doge_vec3 ,struct doge_vec3 ,struct doge_vec3 );
struct doge_vec3 create_vec3(float,float,float);

void mesh_add_triangle(mesh_t*,triangle_t*);

mesh_t parse_obj(char* );

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
//in main algorithm create list of float and after the wile cycle create a vector and create triangle and add to mesh