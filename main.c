#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
#define vec3(x,y,z) create_vec3(x,y,z)
#define vec2(x,y) create_vec2(x,y);
#define vertex(x,y,z) create_vertex(x,y,z)



// int main(int args_count, char* args[])
// {

//     //read from file meshs

//     SDL_Window* my_window;
//     SDL_Renderer* renderer;
//     int width = 1200;
//     int height = 1200;

//     SDL_Init(SDL_INIT_VIDEO);
    
//     SDL_CreateWindowAndRenderer(width,height,0,&my_window,&renderer);
//     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

//     //add mesh 
//     camera_t camera;
//     camera.position = create_vec3(0,0,-10);
//     camera.fov_y = 60 * (float) M_PI/180; //p grego / 180

//     struct doge_vec3 a = create_vec3(100,100,0);
//     struct doge_vec3 b = create_vec3(0,200,0);
//     struct doge_vec3 c = create_vec3(200,200,0);
    
//     point_light_t light = create_light(vec3(10,0,-10));

//     mesh_t* mesh = parse_obj("suzanne.obj");
//     (*mesh).scale = create_vec3(1,1,1);
//     (*mesh).position = create_vec3(0,0,0);
//     (*mesh).rotation = doge_quat_create(0.0,0,0,1.0);
//     // struct doge_vec3 v = create_vec3(100,0,0);
//     // normalize_vec3(&v);
//     // (*mesh).rotation = doge_quat_from_axis_angle(0,1,0,90);
    
    
//     mesh_t* fuck = parse_obj("fuck.obj");
//     (*fuck).scale = create_vec3(0.2,0.2,0.2);
//     (*fuck).position = create_vec3(0,0,0);
//     (*fuck).rotation = doge_quat_create(0,0,0,1.0);
    
//     world_t* w = malloc(sizeof(world_t));
//     w = memset(w,0,sizeof(world_t));
    

//     SDL_Event game_event;
//     int quit = 0;
//     while(!quit)
//     {
//         static float rotation_degrees = 0;
//         rotation_degrees+= 0.05;
//         if(rotation_degrees > 360)
//             rotation_degrees = 0;
        
//         //Handle events on queue
//             while( SDL_PollEvent( &game_event ) != 0 )
//             {
//                 //User requests quit
//                 if( game_event.type == SDL_QUIT )
//                 {
//                     quit = 1;
//                 }
//                 manage_position(mesh,&game_event);
//             }
//         SDL_SetRenderDrawColor(renderer,0,0,0,255);
        
//         init_depth_buffer(w);
//         SDL_RenderClear(renderer);

//         // rotate_around(mesh->position,fuck,10,rotation_degrees);
        
//         mesh_draw(w,*mesh,camera,renderer,width, height);
//         // mesh_draw(*fuck,camera,renderer,width,height);


//         // triangle_draw_no_owner(*t,camera,renderer,width,height);

//         SDL_RenderPresent(renderer);
//         // SDL_Delay(5000);
//     }
//     SDL_DestroyWindow(my_window);
//     SDL_Quit();
//     // char* path = malloc(12);
//     // path = "suzanne.obj";
//     // parse_obj(path);
//     //sdl_renderdrawline
//     //sdl_renderclear
//     //sdl_renderpresent

//     return 0;
// }
vertex_t create_vertex(vec3_t pos,vec3_t norm,vec2_t uv)
{
    vertex_t v;
    v.pos = pos;
    v.normal = norm;
    v.uv = uv;
    return v;
}
point_light_t create_light(vec3_t position)
{
    point_light_t l;
    l.position = position;
    return l;
}

void vertex_project2D(camera_t camera, vertex_t* v, vec3_t position, vec3_t scale,float screen_width, float screen_height)
{
    float half_w = screen_width/2;
    float half_h = screen_height/2;

    v->world_position = (sum_vec3(scale_vec3(v->pos,scale),v->pos));
    v->world_normal = normalized_vec3(v->normal);  //must be multiplied with rotation
    v->world_view_position = sub_vec3(v->world_position, camera.position);


            // point in NDC coordinates (-1 -- 1)
            float yp = v->world_view_position.y / (v->world_view_position.z * (float)tan(camera.fov_y / 2));
            float xp = v->world_view_position.x / (v->world_view_position.z * (float)tan(camera.fov_y / 2));

            // to pixel coordinates
            int nxp = (int)(xp * half_w + half_w);
            int nyp = (int)(-yp * half_h + half_h);

            v->projected_position = create_vec2(nxp,nyp);
        }


void triangle_draw(world_t* w, triangle_t t,camera_t camera,vec3_t pos, vec3_t scale, SDL_Renderer* renderer,float window_w, float window_h)
{
    vertex_project2D(camera,&t.a,pos,scale,window_w,window_h);
    vertex_project2D(camera,&t.b,pos,scale,window_w,window_h);
    vertex_project2D(camera,&t.c,pos,scale,window_w,window_h);

            // we need to order the three vertices from highest (on y) to lowest (on y)
            vertex_t p1 = t.a;
            vertex_t p2 = t.b;
            vertex_t p3 = t.c;

            if (p2.projected_position.y < p1.projected_position.y)
            {
                vertex_t tmp = p1;
                p1 = p2;
                p2 = tmp;
            }

            if (p3.projected_position.y < p2.projected_position.y)
            {
                vertex_t tmp = p2;
                p2 = p3;
                p3 = tmp;
            }

            if (p2.projected_position.y < p1.projected_position.y)
            {
                vertex_t tmp = p1;
                p1 = p2;
                p2 = tmp;
            }


            // compute the slope of P1P2 and P1P3 to know if P2 is on the left or on the right
            float iSlopeP1P2 = (p2.projected_position.x - p1.projected_position.x) / (p2.projected_position.y - p1.projected_position.y);
            float iSlopeP1P3 = (p3.projected_position.x - p1.projected_position.x) / (p3.projected_position.y - p1.projected_position.y);


            // P2 on the left
            if (iSlopeP1P2 < iSlopeP1P3)
            {
                // iterate from P1.Y to P3.Y
                for (int y = (int)p1.projected_position.y; y <= (int)p3.projected_position.y; y++)
                {
                    if (y < p2.projected_position.y)
                    {
                        // phase 1
                        triangle_scanline(w,y, p1, p2, p1, p3,renderer);
                    }
                    else
                    {
                        // phase 2
                        triangle_scanline(w,y, p2, p3, p1, p3,renderer);
                    }
                }
            }
            else
            {
                // P2 on the right
                // iterate from P1.Y to P3.Y
                for (int y = (int)p1.projected_position.y; y <= (int)p3.projected_position.y; y++)
                {
                    if (y < p2.projected_position.y)
                    {
                        // phase 1
                        triangle_scanline(w,y, p1, p3, p1, p2,renderer);
                    }
                    else
                    {
                        // phase 2
                        triangle_scanline(w,y, p1, p3, p2, p3,renderer);
                    }
                }
            }

            /*DrawLineBresenham((int)a.ProjectedPosition.X, (int)a.ProjectedPosition.Y, (int)b.ProjectedPosition.X, (int)b.ProjectedPosition.Y);
            DrawLineBresenham((int)b.ProjectedPosition.X, (int)b.ProjectedPosition.Y, (int)c.ProjectedPosition.X, (int)c.ProjectedPosition.Y);
            DrawLineBresenham((int)a.ProjectedPosition.X, (int)a.ProjectedPosition.Y, (int)c.ProjectedPosition.X, (int)c.ProjectedPosition.Y);*/
    
}
void triangle_scanline(world_t* w, int y, vertex_t left_a,vertex_t left_b,vertex_t right_a,vertex_t right_b,SDL_Renderer* renderer)
{
    // get the gradient of the y (gradient == 0 if y == leftA.y, gradient == 1 if y == leftB.y)

            float leftGradient = (y - left_a.projected_position.y) / (left_b.projected_position.y - left_a.projected_position.y);
            float rightGradient = (y - right_a.projected_position.y) / (right_b.projected_position.y - right_a.projected_position.y);

            // singularity 
            if (left_a.projected_position.y == left_b.projected_position.y)
                leftGradient = 1;

            if (right_a.projected_position.y == right_b.projected_position.y)
                rightGradient = 1;

            // compute left pixel and right pixel using gradient
            float leftX = interpolate_float(left_a.projected_position.x, left_b.projected_position.x, leftGradient);
            float rightX = interpolate_float(right_a.projected_position.x, right_b.projected_position.x, rightGradient);


            // compute the z for the DepthBuffer
            float leftZ = interpolate_float(left_a.world_view_position.z, left_b.world_view_position.z, leftGradient);
            float rightZ = interpolate_float(right_a.world_view_position.z, right_b.world_view_position.z, rightGradient);

            // pixel world position
            vec3_t leftWorld = interpolate_vec3(left_a.world_position, left_b.world_position, leftGradient);
            vec3_t rightWorld = interpolate_vec3(right_a.world_position, right_b.world_position, rightGradient);

            // pixel normal
            vec3_t leftNormal = interpolate_vec3(left_a.world_normal, left_b.world_normal, leftGradient);
            vec3_t rightNormal = interpolate_vec3(right_a.world_normal, right_b.world_normal, rightGradient);

            // // compute pixel UV
            // vec2_t uvLeft = Interpolate(leftA.Uv, leftB.Uv, leftGradient);
            // vec2_t uvRight = Interpolate(rightA.Uv, rightB.Uv, rightGradient);

            // // compute pixel in camera/eye space
            vec3_t leftWorldEye = interpolate_vec3(left_a.world_view_position, left_b.world_view_position, leftGradient);
            vec3_t rightWorldEye = interpolate_vec3(right_a.world_view_position, right_b.world_view_position, rightGradient);


            // fill pixels managing light
            for (int x = (int)leftX; x < (int)rightX; x++)
            {
                // gradient of x
                float xGradient = (x - leftX) / (float)(rightX - leftX);

                vec3_t pixel_world = interpolate_vec3(leftWorld, rightWorld, xGradient);
                vec3_t pixel_world_eye = interpolate_vec3(leftWorldEye, rightWorldEye, xGradient);
                vec3_t pixel_normal = interpolate_vec3(leftNormal, rightNormal, xGradient);

                point_light_t light = create_light(vec3(0,0,-10));
                
                vec3_t pixel_light_direction = normalized_vec3(sub_vec3(light.position, pixel_world));

                // vec2_t pixelUv = Interpolate(uvLeft, uvRight, xGradient);

                // int textureX = (int)Math.Abs((pixelUv.X * (Mesh.Texture.width - 1)) % (Mesh.Texture.width - 1));
                // int textureY = (int)Math.Abs((pixelUv.Y * (Mesh.Texture.height - 1)) % (Mesh.Texture.height - 1));

                // int textureIndex = (Mesh.Texture.width * textureY + textureX) * 4;

                // byte r = Mesh.Texture.bitmap[textureIndex];
                // byte g = Mesh.Texture.bitmap[textureIndex + 1];
                // byte b = Mesh.Texture.bitmap[textureIndex + 2];

                vec3_t texture_color = vec3( 0, 255, 0);

                // lambert for diffuse component of Phong
                float lambert = MAX(dot_vec3(pixel_light_direction,pixel_normal),0);
                // if (lambert > 0.7f)
                //   lambert = 1;

                // lambert = 1;

                float shininess = 0.4;

                // // reflect vector for specular Phong
                vec3_t reflected_light = reflect_vec3(multiply_vec3(pixel_light_direction,-1),pixel_normal);
                // fprintf(stdout,"%f %f %f \n",reflected_light.x,reflected_light.y,reflected_light.z);
                float specular = dot_vec3(reflected_light,multiply_vec3(normalized_vec3(pixel_world_eye),-1));
                if(specular < 0) 
                    specular = 0;
                // fprintf(stdout,"specular: %f \n",specular);
                
                vec3_t specular_color = multiply_vec3(multiply_vec3(create_vec3(1,1,1), specular),  shininess);// (float)Math.Pow(specular, shininess);
                vec3_t diffuse_color = multiply_vec3(texture_color, lambert);

                float z = interpolate_float(leftZ, rightZ, xGradient);

                vec3_t ambient_color = vec3(0,0,0);

                // // compute Phong (will be in hdr)
                vec3_t hdr_color = sum_vec3(diffuse_color ,sum_vec3(specular_color,ambient_color));
                float max_value = MAX(MAX(MAX(hdr_color.x,hdr_color.y),hdr_color.z),1);
                
                //CHECK ALL PIXELS AND GET HIGHEST AND LOWEST RED, GREEN, BLUE, AND ASSERT THAT THE HIGHEST IS 1 AND NORMALIZE ALL IN HIGHEST ORDER


                // // normalize color to LDR
                vec3_t final_color = divide_vec3(hdr_color,max_value);

                // // manage outline
                // float outline = Vector3.Dot(pixelNormal, -pixelWorldEye.Normalized());
                // if (outline > -0.2f && outline < 0.2f)
                // {
                //     finalColor = new Vector3(1, 0, 0);
                // }

                if(!check_depth_buffer(w,z,x,y,1200))  //1200 = window width
                {
                    // Phong
                    SDL_SetRenderDrawColor(renderer,final_color.x,final_color.y,final_color.z,255);
                    SDL_RenderDrawPoint(renderer,x,y);
                    // Device.PutPixel(x, y, z, finalColor);
                }
            }
}
int check_depth_buffer(world_t* w,float new_z,float x, float y, float window_w)
{
    int z_buffer_index = window_w * y + x;
    // fprintf(stdout,"current z = %f  new z = %f \n",w->depth_buffer[z_buffer_index],new_z);
    if (w->depth_buffer[z_buffer_index] < new_z)
        return -1;
    w->depth_buffer[z_buffer_index] = new_z; 
    return 0;
}
void init_depth_buffer(world_t* w)
{
    
    w = memset(w,INT_MAX,sizeof(world_t));
}


void mesh_draw(world_t* w, mesh_t mesh,camera_t camera,SDL_Renderer* rend,float window_w,float window_h)
{
    list_item_t* item = mesh.triangles->head;
    while(item)
    {
        triangle_draw(w,*(triangle_t*)item->item,camera,mesh.position,mesh.scale, rend,window_w,window_h);
        // fprintf(stdout,"rendered triangle %f/%f/%f %f/%f/%f %f/%f/%f\n",t->a.x,t->a.y,t->a.z,t->b.x,t->b.y,t->b.z,t->c.x,t->c.y,t->c.z);
        item = item->next;
    }
}

triangle_t* create_triangle(vertex_t a,vertex_t b,vertex_t c)
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
    t->owner = mesh;
    list_append(mesh->triangles,t);
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
void list_destory(list_t* l)
{
    list_item_t* item = l->head;
    while(item)
    {
        free(item->item);
        item = item->next;
    }
    free(l);
}
void* get_item_at_index(list_t* list,int index)
{
    list_item_t* item = list->head;
    int counter = 0;
    while(item)
    {
        if(counter == index)
            return item->item;
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
    return list_len(*mesh.triangles);
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



list_t* string_split(char* string, const char* delim)
{
    list_t* l = create_list();
    char *original_line = string;
    char* token = malloc(128);
    strcpy(token, original_line);
    char* item = malloc(128);
    item = memset(item,0,128);
    while((item = strsep(&token, delim)))
    {
        list_append(l,item);
    } 
    return l;
}
obj_parser_context_t* obj_parser_init()
{
    obj_parser_context_t* context = malloc(sizeof(obj_parser_context_t));
    context = memset(context,0,sizeof(obj_parser_context_t));
    context->list_f = create_list();
    context->list_v = create_list();
    context->list_vn = create_list();
    context->list_vt = create_list();
    
    return context;
}
void obj_parser_destroy(obj_parser_context_t* context)
{
    free(context); 
}
int obj_parse_line_vertex(obj_parser_context_t* ctx, char* line) //change test and return int
{
    list_t* floats = string_split(line," "); //char list
    float x = atof((char*)get_item_at_index(floats,0));
    float y = atof((char*)get_item_at_index(floats,1));
    float z = atof((char*)get_item_at_index(floats,2));
    vec3_t* v = malloc(sizeof(vec3_t)); 
    if(!v)
        return -1;
    *v = vec3(x,y,z);
    ctx->counter_v++;
    list_append(ctx->list_v,v);
    list_destory(floats);
    return 0;
}
int obj_parse_line_uv(obj_parser_context_t* ctx, char* line) //change test and return int
{
    list_t* floats = string_split(line," ");
    float u = atof((char*)get_item_at_index(floats,0));
    float v = atof((char*)get_item_at_index(floats,1));
    vec2_t* vec = malloc(sizeof(vec2_t));
    if(!vec)
        return -1;
    *vec = vec2(u,v);
    ctx->counter_vt++;
    list_append(ctx->list_vt,vec);
    list_destory(floats);
    return 0;
}
int obj_parse_line_normal(obj_parser_context_t* ctx,char* line) //change test nad return int
{
    list_t* floats = string_split(line," "); //char list
    float x = atof((char*)get_item_at_index(floats,0));
    float y = atof((char*)get_item_at_index(floats,1));
    float z = atof((char*)get_item_at_index(floats,2));
    vec3_t* n = malloc(sizeof(vec3_t)); 
    if(!n)
        return -1;
    *n = vec3(x,y,z);
    ctx->counter_vn++;
    list_append(ctx->list_vn,n);
    list_destory(floats);
    return 0;
}

int obj_parse_indexes_vertex(obj_parser_context_t* ctx, char* line) //change test 
{
    list_t* ints = string_split(line,"/"); //3 integers
    int v_index = atoi((char*)get_item_at_index(ints,0));   //vertex index
    int uv_index = atoi((char*)get_item_at_index(ints,1));  //uv index
    int vn_index = atoi((char*)get_item_at_index(ints,2));  //normal index

    if(v_index-1 > ctx->counter_v || uv_index-1 > ctx->counter_vt || vn_index-1 > ctx->counter_vn)
    {
        fprintf(stdout,"index out of range during vertex parsing \n");
        return -1;
    }

    vertex_t* vertex = malloc(sizeof(vertex_t));
    if(!vertex)
        return -1;
    vertex = memset(vertex,0,sizeof(vertex_t));
    *vertex = create_vertex(*(vec3_t*)get_item_at_index(ctx->list_v,v_index-1), *(vec3_t*)get_item_at_index(ctx->list_vn,vn_index-1),*(vec2_t*)get_item_at_index(ctx->list_vt,uv_index-1));
    list_append(ctx->list_f,vertex);
    list_destory(ints);
    return 0;
}
int obj_parse_line_triangle(obj_parser_context_t* ctx,mesh_t* mesh, char* line) //change test
{
    list_t* v_indexes = string_split(line," ");
    vertex_t v1 = obj_parse_indexes_vertex(ctx,(char*)get_item_at_index(v_indexes,0));
    vertex_t v2 = obj_parse_indexes_vertex(ctx,(char*)get_item_at_index(v_indexes,1));
    vertex_t v3 = obj_parse_indexes_vertex(ctx,(char*)get_item_at_index(v_indexes,2));
    
    triangle_t* t = create_triangle(v1,v2,v3);
    if(!t)
    {
        fprintf(stdout,"unable to alloc memory for triangle\n");
        return -1;    
    } 
    mesh_add_triangle(mesh,t);
    list_destory(v_indexes);
    return 0;
    //NEED TO DO TEST
}
int obj_parse_line(obj_parser_context_t* ctx, mesh* mesh, char* line)
{
    //write test
    //check if is v, vt, vn or f and call right methods
}
int obj_parse_from_file(obj_parser_context_t* ctx, mesh* mesh, char* line) //mesh must be previously allocated
{
    //write test <--
    //open file
    //read line by line a file
    //parse line
    //close file
}











// mesh_t* parse_obj(char* obj_path)
// {
//     //to return
//     mesh_t* my_mesh = malloc(sizeof(mesh_t));
//     memset(my_mesh,0,sizeof(mesh_t));


//     FILE* file;
//     file=fopen(obj_path,"r");
//     char* line = malloc(128);
//     int buffer_size = 128;
//     list_t* all_points_of_mesh = create_list(); 
//     list_t* all_normals_of_mesh = create_list();
//     list_t* all_uv_of_mesh = create_list();

//         while(file)
//         {

//             if(!fgets(line,buffer_size,file))
//                 break;
            
//             char *original_line = line;
//             char** token = &original_line;
//             char* item;

//             int is_f = 0;
//             int is_v = 0;
//             int is_vn = 0;
//             int is_vt = 0; //uv coords

//             list_t* float_for_vector = create_list();
//             list_t* list_3_vec_index = create_list(); //int
//             //new list floatindex normal (3)
//             list_t* list_3_normal_index = create_list();
//             //new list floatindex uv (2)
//             list_t* list_3_uv_index = create_list();
            
//             static int added = 0;
//             while((item = strsep(token, " ")))
//             {

//                 if(!strcmp("f",item)) //is the start of an f line (next 3 elements must be splitted for "/" and the first may be taked)
//                 {
//                     is_f = 1;
//                     is_v = 0;
//                     is_vn = 0;
//                     is_vt = 0;
//                     continue;                    
//                 }
//                 else if(!strcmp("v", item)) //is the start of an v line (next 3 elements are float of vector)
//                 {
//                     is_v = 1;
//                     is_f = 0;
//                     is_vn = 0;
//                     is_vt = 0;
//                     continue;                    
//                 }
//                 else if(!strcmp("vn",item))
//                 {
//                     is_v = 0;
//                     is_f = 0;
//                     is_vn = 1;
//                     is_vt = 1;
//                 }
//                 else if(!strcmp("vt", item))
//                 {
//                     is_v = 0;
//                     is_f = 0;
//                     is_vn = 0;
//                     is_vt = 1;
//                 }
//                 else
//                 {
//                     if(is_v)
//                     {
//                         float* value = malloc(sizeof(float)); 
//                         *value = (float)atof(item); //transform string into floating point number
//                         // fprintf(stdout,"added point %f when item is %s\n",*value, item);
//                         list_append(float_for_vector,value); //add to my list
//                     }
//                     if(is_vn)
//                     {
//                         float* value = malloc(sizeof(float)); 
//                         // fprintf(stdout,"%s \n",item);
//                         *value = (float)atof(item); //transform string into floating point number
//                         // fprintf(stdout,"added point %f when item is %s\n",*value, item);
//                         list_append(float_for_vector,value); //add to my normal list
//                     }
//                     if(is_vt)
//                     {
//                         float* value = malloc(sizeof(float)); 
//                         *value = (float)atof(item); //transform string into floating point number
//                         // fprintf(stdout,"added point %f when item is %s\n",*value, item);
//                         list_append(float_for_vector,value); //add to my normal list
//                     }
//                     if(is_f)
//                     {
//                         char** line_copy = &item;
//                         // fprintf(stdout,"line copied \n");
//                         char* index_vertex = strsep(line_copy,"/");  //get first element
//                         // fprintf(stdout,"line splited and return value %s \n",index);
//                         int* n_index_vertex = malloc(sizeof(int));
//                         *n_index_vertex = (atoi(index_vertex)); //convert to integer 
//                         // fprintf(stdout,"transformed in value %d \n",*n_index);

//                         char* index_uv = strsep(line_copy,"/");
//                         int* n_index_uv = malloc(sizeof(int));
//                         *n_index_uv = (atoi(index_uv));


//                         char* index_normal = strsep(line_copy,"/");
//                         int* n_index_normal = malloc(sizeof(int));
//                         *n_index_normal = (atoi(index_normal));
                        
//                         list_append(list_3_vec_index,n_index_vertex); //add to list
//                         list_append(list_3_uv_index,n_index_uv);
//                         list_append(list_3_normal_index,n_index_normal);
                        
//                     }
//                 }
//             }
//             //create vector and add to point list
//             if(is_v)
//             {
//                 static int counter;
                
//                 struct doge_vec3* v = malloc(sizeof(struct doge_vec3)); 
//                 if(!v) return NULL;
//                 *v = create_vec3(*((float*)float_for_vector->head->item),*((float*)float_for_vector->head->next->item),*((float*) float_for_vector->head->next->next->item)*-1);     
//                 list_append(all_points_of_mesh,v);
//                 fprintf(stdout,"normal %d = %f %f %f\n",++counter,*((float*)float_for_vector->head->item),*((float*)float_for_vector->head->next->item),*((float*) float_for_vector->head->next->next->item)*-1);   
                
//             }
//             if(is_vn)
//             {
//                 static int counter;
//                 struct doge_vec3* v = malloc(sizeof(struct doge_vec3)); 
//                 if(!v) return NULL;
//                 *v = create_vec3(*((float*)float_for_vector->head->item),*((float*)float_for_vector->head->next->item),*((float*) float_for_vector->head->next->next->item)*-1);     
//                 list_append(all_normals_of_mesh,v);     
//                 // fprintf(stdout,"normal %d = %f %f %f\n",++counter,v->x,v->y,v->z);   
//             }
//             if(is_vt)
//             {
//                 struct doge_vec2* v = malloc(sizeof(struct doge_vec2)); 
//                 if(!v) return NULL;
//                 *v = create_vec2(*((float*)float_for_vector->head->item),*((float*)float_for_vector->head->next->item));     
//                 list_append(all_uv_of_mesh,v);  
//             }
//             if(is_f)
//             {
//                 //create triangle 
//                 int first_index = *(int*)get_item_at_index(list_3_vec_index,0)->item;
//                 int second_index = *(int*)get_item_at_index(list_3_vec_index,1)->item;
//                 int third_index = *(int*)get_item_at_index(list_3_vec_index,2)->item;
                
//                 vec3_t v1_pos = *(struct doge_vec3*)(get_item_at_index(all_points_of_mesh,first_index-1)->item); //p1 point
//                 vec3_t v2_pos = *(struct doge_vec3*)(get_item_at_index(all_points_of_mesh,second_index-1)->item); //p2 point
//                 vec3_t v3_pos = *(struct doge_vec3*)(get_item_at_index(all_points_of_mesh,third_index-1)->item);

//                 vec3_t v1_norm = *(struct doge_vec3*)(get_item_at_index(all_normals_of_mesh,first_index-1)->item);
//                 vec3_t v2_norm = *(struct doge_vec3*)(get_item_at_index(all_normals_of_mesh,second_index-1)->item);
//                 vec3_t v3_norm = *(struct doge_vec3*)(get_item_at_index(all_normals_of_mesh,third_index-1)->item);
                
//                 vec2_t v1_uv = *(vec2_t*)(get_item_at_index(all_uv_of_mesh,first_index-1)->item);
//                 vec2_t v2_uv = *(vec2_t*)(get_item_at_index(all_uv_of_mesh,second_index-1)->item);
//                 vec2_t v3_uv = *(vec2_t*)(get_item_at_index(all_uv_of_mesh,third_index-1)->item);

//                 triangle_t* t = create_triangle(create_vertex(v1_pos,v1_norm,v1_uv), create_vertex(v2_pos,v2_norm,v2_uv), create_vertex(v3_pos,v3_norm,v3_uv));
                
//                 //use meshaddtriangle
//                 mesh_add_triangle(my_mesh,t);
//                 // fprintf(stdout," triangle added to mesh  %f/%f/%f %f/%f/%f %f/%f/%f\n",t.a.x,t.a.y,t.a.z,t.b.x,t.b.y,t.b.z,t.c.x,t.c.y,t.c.z);
                
//             }
            
//             free(float_for_vector);
//             free(list_3_vec_index);
//             free(list_3_normal_index);
//             free(list_3_uv_index);
            
//         }
//         //free all_points_of_mesh
//         fclose(file);
//     return my_mesh;
// }

