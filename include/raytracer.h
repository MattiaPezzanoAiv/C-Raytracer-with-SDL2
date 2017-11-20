#include "math.h"
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

struct doge_vec3 doge_vector_direction(struct doge_vec3, struct doge_vec3); //return the direction between this 2 vectors
float doge_vector_dot(struct doge_vec3, struct doge_vec3); //return the dot product between 2 vectors x1*x2 + y1*y2 + z1*z2

struct doge_sphere
{
    struct doge_vec3 center;
    float radius;
    struct doge_vec3 color;

    struct doge_sphere* next;
    struct doge_sphere* prev;
};
struct doge_ray
{
    struct doge_vec3 origin;
    struct doge_vec3 direction;
};
struct doge_world
{
    // struct doge_ray* head_ray;
    // struct doge_ray* tail_ray;

    struct doge_sphere* head_sphere;
    struct doge_sphere* tail_sphere;
};
struct doge_vec3 doge_intersect(struct doge_world,struct doge_ray); //foreach ray and sphere
int doge_add_sphere(struct doge_world*, struct doge_sphere*);
int doge_draw(float, float, struct doge_vec3,struct doge_world, SDL_Renderer*  );
