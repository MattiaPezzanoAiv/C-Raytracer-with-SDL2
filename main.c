#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(int args_count, char* args[])
{
    //read from file meshs

    SDL_Window* my_window;
    SDL_Renderer* renderer;
    int width = 1200;
    int height = 1200;

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_CreateWindowAndRenderer(width,height,0,&my_window,&renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    //add mesh 
    camera_t camera;
    camera.position = create_vec3(0,0,-25);
    camera.fov_y = 60;

    struct doge_vec3 a = create_vec3(100,100,0);
    struct doge_vec3 b = create_vec3(0,200,0);
    struct doge_vec3 c = create_vec3(200,200,0);
    
    // triangle_t t = create_triangle(a,b,c);
    mesh_t* mesh = parse_obj("suzanne.obj");
    (*mesh).scale = create_vec3(1,1,1);
    (*mesh).position = create_vec3(0,0,0);
    (*mesh).rotation = doge_quat_create(0.0,0,0,1.0);
    // struct doge_vec3 v = create_vec3(100,0,0);
    // normalize_vec3(&v);
    // (*mesh).rotation = doge_quat_from_axis_angle(0,1,0,90);
    
    
    
    mesh_t* fuck = parse_obj("fuck.obj");
    (*fuck).scale = create_vec3(0.2,0.2,0.2);
    (*fuck).position = create_vec3(0,0,0);
    (*fuck).rotation = doge_quat_create(0,0,0,1.0);
    
    

    SDL_Event game_event;
    int quit = 0;
    while(!quit)
    {
        static float rotation_degrees = 0;
        rotation_degrees+= 0.05;
        if(rotation_degrees > 360)
            rotation_degrees = 0;
        
        //Handle events on queue
            while( SDL_PollEvent( &game_event ) != 0 )
            {
                //User requests quit
                if( game_event.type == SDL_QUIT )
                {
                    quit = 1;
                }
                manage_position(mesh,&game_event);
            }
        fprintf(stdout,"loop  \n");
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        fprintf(stdout,"render color set  \n");
        
        SDL_RenderClear(renderer);
        fprintf(stdout,"clear screen  \n");

        // mesh->position = sum_vec3(mesh->position, create_vec3(0.01,0,0));
        rotate_around(mesh->position,fuck,10,rotation_degrees);

        mesh_draw(*mesh,camera,renderer,width, height);
        mesh_draw(*fuck,camera,renderer,width,height);



        // triangle_draw(t,camera,renderer,width,height);
        SDL_RenderPresent(renderer);
        // SDL_Delay(5000);
    }
    SDL_DestroyWindow(my_window);
    SDL_Quit();
    // char* path = malloc(12);
    // path = "suzanne.obj";
    // parse_obj(path);
    //sdl_renderdrawline
    //sdl_renderclear
    //sdl_renderpresent

    return 0;
}

void triangle_draw(triangle_t t,camera_t camera, SDL_Renderer* renderer,float window_w, float window_h)
{
    fprintf(stdout,"called traingle draw trainagle value is %f/%f/%f %f/%f/%f %f/%f/%f\n",t.a.x,t.a.y,t.a.z,t.b.x,t.b.y,t.b.z,t.c.x,t.c.y,t.c.z);
    float fov_y = camera.fov_y * (float) M_PI/180; //p grego / 180

    mesh_t owner = *(mesh_t*)t.owner;

    struct doge_vec3 ac = (scale_vec3(t.a,owner.scale));
    ac = doge_quat_rotated_vec(ac,owner.rotation);
    ac = sub_vec3(sum_vec3(ac,owner.position),camera.position);
    struct doge_vec3 bc = (scale_vec3(t.b,owner.scale));
    bc = doge_quat_rotated_vec(bc,owner.rotation);
    bc = sub_vec3(sum_vec3(bc,owner.position),camera.position);
    struct doge_vec3 cc = (scale_vec3(t.c,owner.scale));
    cc = doge_quat_rotated_vec(cc,owner.rotation);
    cc = sub_vec3(sum_vec3(cc,owner.position),camera.position);

    // struct doge_vec3 ac = sub_vec3(scale_vec3(sum_vec3(t.a,owner.position),owner.scale),camera.position);
    // ac = doge_quat_rotated_vec(ac,owner.rotation);
    // struct doge_vec3 bc = sub_vec3(scale_vec3(sum_vec3(t.b,owner.position),owner.scale),camera.position);
    // bc = doge_quat_rotated_vec(ac,owner.rotation);
    // struct doge_vec3 cc = sub_vec3(scale_vec3(sum_vec3(t.c,owner.position),owner.scale),camera.position);
    // cc = doge_quat_rotated_vec(ac,owner.rotation);
        
    // // point1
    float yp1 = ac.y / (ac.z * (float)tan(fov_y / 2));
    float xp1 = ac.x / (ac.z * (float)tan(fov_y / 2));

    // from -1/1 to 0/window.width (NDC)
    int nxp1 = (int)(xp1 * window_w/2 + window_h/2);
    int nyp1 = (int)(-yp1 * window_w/2 + window_h/2);

    float yp2 = bc.y / (bc.z * (float)tan(fov_y / 2));
    float xp2 = bc.x / (bc.z * (float)tan(fov_y / 2));

    int nxp2 = (int)(xp2 * window_w/2 + window_h/2);
    int nyp2 = (int)(-yp2 * window_w/2 + window_h/2);

    float yp3 = cc.y / (cc.z * (float)tan(fov_y / 2));
    float xp3 = cc.x / (cc.z * (float)tan(fov_y / 2));

    int nxp3 = (int)(xp3 * window_w/2 + window_h/2);
    int nyp3 = (int)(-yp3 * window_w/2 + window_h/2);

    //draw 3 lines
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderDrawLine(renderer,nxp1,nyp1,nxp2,nyp2);
    SDL_RenderDrawLine(renderer,nxp1,nyp1,nxp3,nyp3);
    SDL_RenderDrawLine(renderer,nxp3,nyp3,nxp2,nyp2);
    
    
}
void mesh_draw(mesh_t mesh,camera_t camera,SDL_Renderer* rend,float window_w,float window_h)
{
    fprintf(stdout,"starting drawing mesh  \n");
    triangle_t* t = mesh.head;
    fprintf(stdout,"taked first triangle reference\n");
    while(t)
    {
        triangle_draw(*t,camera,rend,window_w,window_h);
        fprintf(stdout,"rendered triangle %f/%f/%f %f/%f/%f %f/%f/%f\n",t->a.x,t->a.y,t->a.z,t->b.x,t->b.y,t->b.z,t->c.x,t->c.y,t->c.z);
        t = t->next;
    }
}

triangle_t* create_triangle(struct doge_vec3 a,struct doge_vec3 b,struct doge_vec3 c)
{
    triangle_t* t = malloc(sizeof(triangle_t));
    if(!t) return NULL;
    t->a = a;
    t->b = b;
    t->c = c;
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
    t->owner = mesh;
    // fprintf(stdout,"added triangle with values %f/%f/%f %f/%f/%f %f/%f/%f\n",t->a.x,t->a.y,t->a.z,t->b.x,t->b.y,t->b.z,t->c.x,t->c.y,t->c.z);
}

list_t* create_list()
{
    list_t* list = malloc(sizeof(list_t));
    if(!list)
        return NULL;
    list = memset(list,0,sizeof(list_t));
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
void list_append(list_t* list, void* item)
{
    list_item_t* _item = create_list_item(item);
    if (list->head == NULL) //list is empty
    {
        list->head = _item;
        list->tail = _item;
    }
    else
    {
        list->tail->next = _item;
        _item->prev = list->tail;
        list->tail = _item;
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
int list_len(list_t list)
{
    list_item_t* item = list.head;
    int counter = 0;
    while(item)
    {
        counter++;
        item = item->next;
    }
    return counter;
}
int mesh_triangles_count(mesh_t mesh)
{
    triangle_t* t = mesh.head;
    int counter = 0;
    while(t)
    {
        counter++;
        t = t->next;
    }
    return counter;
}
void manage_position(mesh_t* mesh,SDL_Event* event)
{
     if(event->type ==  SDL_KEYDOWN)
     {
         
        switch (event->key.keysym.sym)
        {
            case SDLK_LEFT:  mesh->position = sum_vec3(mesh->position,create_vec3(-0.05,0,0)); break;
            case SDLK_RIGHT: mesh->position = sum_vec3(mesh->position,create_vec3(0.05,0,0)); break;
            case SDLK_UP:    mesh->position = sum_vec3(mesh->position,create_vec3(0,0.05,0)); break;
            case SDLK_DOWN:  mesh->position = sum_vec3(mesh->position,create_vec3(0,-0.05,0)); break;
        }
         
        
     }
}
void rotate_around(struct doge_vec3 point, mesh_t* mesh, float distance, float angle)
{
    float new_x = cos(angle);
    float new_y = sin(angle);
    mesh->position = multiply_vec3(sum_vec3(point, create_vec3(new_x,new_y,mesh->position.z)),distance);
}
mesh_t* parse_obj(char* obj_path)
{
    //to return
    mesh_t* my_mesh = malloc(sizeof(mesh_t));
    memset(my_mesh,0,sizeof(mesh_t));


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
            static int added = 0;
            while((item = strsep(token, " ")))
            {
                if(is_v)
                    fprintf(stdout,"%s\n", item);             
                
                if(!strcmp("f",item)) //is the start of an f line (next 3 elements must be splitted for "/" and the first may be taked)
                {
                    is_f = 1;
                    is_v = 0;
                    continue;                    
                }
                else if(!strcmp("v", item)) //is the start of an v line (next 3 elements are float of vector)
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
                        *value = (float)atof(item); //transform string into floating point number
                        // fprintf(stdout,"added point %f when item is %s\n",*value, item);
                        list_append(float_for_vector,value); //add to my list
                    }
                    if(is_f)
                    {
                        char** line_copy = &item;
                        // fprintf(stdout,"line copied \n");
                        char* index = strsep(line_copy,"/");  //get first element
                        // fprintf(stdout,"line splited and return value %s \n",index);
                        int* n_index = malloc(sizeof(int));
                        *n_index = (atoi(index)); //convert to integer 
                        // fprintf(stdout,"transformed in value %d \n",*n_index);
                        
                        list_append(list_3_vec_index,n_index); //add to list
                    }
                }
            }
            //create vector and add to point list
            if(is_v)
            {
                struct doge_vec3* v = malloc(sizeof(struct doge_vec3)); 
                if(!v) return NULL;
                *v = create_vec3(*((float*)float_for_vector->head->item),*((float*)float_for_vector->head->next->item),*((float*) float_for_vector->head->next->next->item)*-1);     
                list_append(all_points_of_mesh,v);
                fprintf(stdout," vector  %f %f %f  added to mesh %d\n",v->x,v->y,v->z, ++added);
            }
            if(is_f)
            {
                //create triangle 
                int first_index = *(int*)get_item_at_index(list_3_vec_index,0)->item;
                int second_index = *(int*)get_item_at_index(list_3_vec_index,1)->item;
                int third_index = *(int*)get_item_at_index(list_3_vec_index,2)->item;
                fprintf(stdout,"vertex 1 %d vertex 2 %d vertex 3 %d \n",first_index,second_index,third_index);
                

                triangle_t* t = create_triangle(*(struct doge_vec3*)(get_item_at_index(all_points_of_mesh,first_index-1)->item),
                *(struct doge_vec3*)(get_item_at_index(all_points_of_mesh,second_index-1)->item),
                *(struct doge_vec3*)(get_item_at_index(all_points_of_mesh,third_index-1)->item));
                //use meshaddtriangle
                mesh_add_triangle(my_mesh,t);
                // fprintf(stdout," triangle added to mesh  %f/%f/%f %f/%f/%f %f/%f/%f\n",t.a.x,t.a.y,t.a.z,t.b.x,t.b.y,t.b.z,t.c.x,t.c.y,t.c.z);
                
            }
            
            free(float_for_vector);
            free(list_3_vec_index);
            
        }
        fprintf(stdout," mesh have %d points\n",list_len(*all_points_of_mesh));
        fprintf(stdout," mesh have %d triangles\n",mesh_triangles_count(*my_mesh));
        //free all_points_of_mesh
        fclose(file);
    return my_mesh;
}