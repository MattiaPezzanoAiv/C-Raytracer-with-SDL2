#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int args_count, char* args[])
{
    //read from file meshs

    SDL_Window* my_window;
    SDL_Renderer* renderer;
    int width;
    int height;

    SDL_CreateWindowAndRenderer(width,height,0,&my_window,&renderer);
    SDL_Init(0);
    //add mesh 

    //game loop and render mesh (start with a triangle)


    // char* path = malloc(12);
    // path = "suzanne.obj";
    // parse_obj(path);
    //sdl_renderdrawline
    //sdl_renderclear
    //sdl_renderpresent

    return 0;
}

void triangle_draw(triangle_t* triangle,camera_t* camera, SDL_Renderer* renderer,float window_w, float window_h)
{
    float fov_y = Camera->fov_y * (float) M_PI/180; //p grego / 180

    struct doge_vec3 ac =sub_vec3(triangle->a , camera->position);
    struct doge_vec3 bc = sub_vec3(triangle->b, camera->position);
    struct doge_vec3 cc = sub_vec3(triangle->c, camera->position);
    
    // // point1
    float yp1 = ac.y / (ac.z * (float)tan(fovy / 2));
    float xp1 = ac.x / (ac.z * (float)tan(fovy / 2));

    // from -1/1 to 0/window.width (NDC)
    int nxp1 = (int)(xp1 * window_w/2 + window_h/2);
    int nyp1 = (int)(-yp1 * window_w/2 + window_h/2);

    float yp2 = bc.y / (bc.z * (float)tan(fovy / 2));
    float xp2 = bc.x / (bc.z * (float)tan(fovy / 2));

    int nxp2 = (int)(xp2 * window_w/2 + window_h/2);
    int nyp2 = (int)(-yp2 * window_w/2 + window_h/2);

    float yp3 = cc.y / (cc.z * (float)tan(fovy / 2));
    float xp3 = cc.x / (cc.z * (float)tan(fovy / 2));

    int nxp3 = (int)(xp3 * window_w/2 + window_h/2);
    int nyp3 = (int)(-yp3 * window_w/2 + window_h/2);

    //draw 3 lines
    
}
void mesh_draw(mesh_t* mesh,camera_t* camera,SDL_Renderer* rend)
{
    triangle_t* t = mesh->head;
    while(t)
    {
        triangle_draw(t,camera,rend);
        t = t->next;
    }
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

list_t* create_list()
{
    list_t* list = malloc(sizeof(list_t));
    if(!list)
        return NULL;
    list = memset(0);
    return list;
}
list_item_t* create_list_item(void* data) //take data
{
    list_item_t* item = malloc(sizeof(list_item_t));
    if(!item)
        return NULL;
    item->item = data; 
    return item;
}
void list_append(list_t* list, list_item_t* item)
{
    if (list->head == NULL) //list is empty
    {
        list->head = item;
        list->tail = item;
    }
    else
    {
        list->tail->next = item;
        item->prev = list->tail;
        list->tail = item;
    }
}
list_item_t* get_item_at_index(list_t* list,int index)
{
    list_item_t* item = list->head;
    int counter = 0;
    while(item)
    {
        if(counter == index)
            return item;
        item = item->next;
        counter++;
    }
    // free(item);
    return NULL;
}

mesh_t parse_obj(char* obj_path)
{
    //to return
    mesh_t* my_mesh = malloc(sizeof(mesh_t));


    FILE* file;
    file=fopen(obj_path,"r");
    char* line = malloc(128);
    int buffer_size = 128;
    list_t* all_points_of_mesh = create_list(); 

        while(file)
        {

            if(!fgets(line,buffer_size,file))
                break;
            
            char *original_line = line;
            char** token = &original_line;
            char* item;

            int is_f = 0;
            int is_v = 0;

            list_t* float_for_vector = create_list();
            list_t* list_3_vec_index = create_list(); //float
            
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
                        float* value = malloc(sizeof(float)); 
                        *value = (float)atoll(item); //transform string into floating point number
                        list_append(float_for_vector,value); //add to my list
                    }
                    if(is_f)
                    {
                        char[] line_copy = *line;
                        char* index = strsep(line_copy,"/");  //get first element
                        int* n_index = atoi(index); //convert to integer 
                        list_append(list_3_vec_index,n_index); //add to list
                    }
                }
            }
            //create vector and add to point list
            if(is_v)
            {
                struct doge_vec3 v= create_vec3(float_for_vector->head->item,float_for_vector->head->next->item, float_for_vector->head->next->next->item);     
                list_append(all_points_of_mesh,v);
            }
            if(is_f)
            {
                //create triangle 
                int first_index = get_item_at_index(list_3_vec_index,0);
                int second_index = get_item_at_index(list_3_vec_index,1);
                int third_index = get_item_at_index(list_3_vec_index,2);
                
                triangle_t t = create_triangle(get_item_at_index(first_index),get_item_at_index(second_index),get_item_at_index(third_index));
                //use meshaddtriangle
                mesh_add_triangle(my_mesh,t);
            }
            
            
            fprintf(stdout,"%s\n",token);
            
        }

        fclose(file);
    return my_mesh;
}