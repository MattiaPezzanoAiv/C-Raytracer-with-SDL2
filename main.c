#include "header.h"
#include <stdlib.h>
#include <stdio.h>

int main(int args_count, char* args[])
{
    //read from file meshs

    SDL_Window* my_window;
    SDL_Renderer* renderer;
    int width;
    int height;

    SDL_CreateWindowAndRenderer(width,height,0,&my_window,&renderer);
    SDL_Init(0);

    char* path = malloc(12);
    path = "suzanne.obj";
    parse_obj(path);
    //sdl_renderdrawline
    //sdl_renderclear
    //sdl_renderpresent

    return 0;
}


triangle_t create_triangle(struct doge_vec3 a,struct doge_vec3 b,struct doge_vec3 c)
{
    triangle_t t;
    t.a = a;
    t.b = b;
    t.c = c;
    return t;
}
struct doge_vec3 create_vec3(float x,float y,float z)
{
    struct doge_vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

void mesh_add_triangle(mesh_t* mesh,triangle_t* t)
{
    if (mesh->head == NULL) //list is empty
    {
        mesh->head = t;
        mesh->tail = t;
    }
    else
    {
        mesh->tail->next = t;
        t->prev = mesh->tail;
        mesh->tail = t;
    }
}

mesh_t parse_obj(char* obj_path)
{
    
    FILE* file;
    file=fopen(obj_path,"r");
    char* line = malloc(128);
    int buffer_size = 128;
   
        while(file)
        {

            if(!fgets(line,buffer_size,file))
                break;
            
            char *original_line = line;
            char** token = &original_line;
            char* item;

            int is_f = 0;
            int is_v = 0;
            while((item = strsep(token, " ")))
            {
                fprintf(stdout,"%s\n", item);             
                if(strcmp("f",item)) //is the start of an f line (next 3 elements must be splitted for "/" and the first may be taked)
                {
                    is_f = 1;
                    is_v = 0;
                    continue;                    
                }
                else if(strcmp("v", item)) //is the start of an v line (next 3 elements are float of vector)
                {
                    is_v = 1;
                    is_f = 0;
                    continue;                    
                }
                else
                {
                    if(is_v)
                    {
                        // float  (float)atoll(item);
                    }
            
                }
            }
            fprintf(stdout,"%s\n",token);
            
        }

        fclose(file);
    
}

