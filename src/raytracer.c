#include "../include/math.h"
#include "../include/raytracer.h"
#include <math.h>



struct doge_vec3 doge_vector_direction(struct doge_vec3 a, struct doge_vec3 b) //return the direction between this 2 vectors
{
    return sub_vec3(b,a);
}
float doge_vector_dot(struct doge_vec3 a, struct doge_vec3 b) //return the dot product between 2 vectors x1*x2 + y1*y2 + z1*z2
{
    float x = a.x* b.x;
    float y = a.y * b.y;
    float z = a.z * b.z;
    return x+y+z;
}
int doge_add_sphere(struct doge_world* w, struct doge_sphere* s)
{
    if(w->head_sphere == NULL) //list is empty
    {
        w->head_sphere = s;
        w->tail_sphere = s;        
    }
    else
    {
        w->tail_sphere->next = s;
        s->prev = w->tail_sphere;
        w->tail_sphere = s;
    }
    return 0;
}

struct doge_vec3 doge_intersect(struct doge_world w, struct doge_ray r) //foreach ray and sphere
{
    struct doge_sphere* s = w.head_sphere;
    while(s)
    {
        struct doge_vec3 l = sub_vec3(s->center,r.origin); 
        float tca = doge_vector_dot(l,r.direction);
        if(tca > 0)
        {
            float d = sqrt(slow_len_vec3(l)*slow_len_vec3(l) - tca * tca);
            if(d <= s->radius)
            {
                float thc = sqrt(s->radius*s->radius - d*d);
                float t0 = tca - thc;
                float t1 = tca + thc;
                //compute distance
                return s->color;
            }
        }

        s = s->next;
    }
    struct doge_vec3 c; //must be black
    c.x = 0;
    c.y = 0;
    c.z = 0;
    return c;
}

int doge_draw(float width, float height, struct doge_vec3 camera, struct doge_world world, SDL_Renderer* renderer)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            struct doge_vec3 pixel;
            pixel.x = j;
            pixel.y = i;
            pixel.z = 0;

            struct doge_vec3 dir = doge_vector_direction(camera,pixel);
            struct doge_ray ray;
            ray.origin = camera;
            ray.direction = dir;

            struct doge_vec3 color = doge_intersect(world,ray);
            //set pixel as color color
            SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
            SDL_RenderDrawPoint(renderer, j, i); 
        }
    }
            SDL_RenderPresent(renderer);

    //move spheres if you want
    return 0;
}

int main(int args_number, char* args[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
    
    struct doge_sphere sphere1;
    struct doge_vec3 center;
    center.x = 10;
    center.y = 0;
    center.z = 10;
    sphere1.center = center;
    sphere1.radius = 400;
    struct doge_vec3 color;
    color.x = 255;
    color.y = 0;
    color.z = 0;
    sphere1.color = color;

    struct doge_sphere sphere2;
    center.z = 5;
    sphere2.center = center;
    sphere2.radius = 500;
    color.x = 0;
    color.y = 255;
    sphere2.color = color;

    struct doge_world world;
    // doge_add_sphere(&world,&sphere1);
    doge_add_sphere(&world,&sphere2);
    
    struct doge_vec3 cam;
    cam.x = 0;
    cam.y = 2;
    cam.z = 0;

    SDL_Event event;
    // while(1)
    // {
        // if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        //     break;
        doge_draw(800,600,cam,world,renderer);
    // }
    SDL_Delay(8000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  return 0;
}



