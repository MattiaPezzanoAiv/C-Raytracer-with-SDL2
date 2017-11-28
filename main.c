#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)



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
    camera.position = create_vec3(0,0,-10);
    camera.fov_y = 60 * (float) M_PI/180; //p grego / 180

    struct doge_vec3 a = create_vec3(100,100,0);
    struct doge_vec3 b = create_vec3(0,200,0);
    struct doge_vec3 c = create_vec3(200,200,0);
    
    // triangle_t* t = create_triangle(a,b,c);

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

        // rotate_around(mesh->position,fuck,10,rotation_degrees);
        
        mesh_draw(*mesh,camera,renderer,width, height);
        // mesh_draw(*fuck,camera,renderer,width,height);


        // triangle_draw_no_owner(*t,camera,renderer,width,height);

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

void vertex_project2D(camera_t camera, vertex_t* v, struct doge_vec3 position, struct doge_vec3 scale,float screen_width, float screen_height)
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


void triangle_draw(triangle_t t,camera_t camera,vec3_t pos, vec3_t scale, SDL_Renderer* renderer,float window_w, float window_h)
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
                        triangle_scanline(y, p1, p2, p1, p3,renderer);
                    }
                    else
                    {
                        // phase 2
                        triangle_scanline(y, p2, p3, p1, p3,renderer);
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
                        triangle_scanline(y, p1, p3, p1, p2,renderer);
                    }
                    else
                    {
                        // phase 2
                        triangle_scanline(y, p1, p3, p2, p3,renderer);
                    }
                }
            }

            /*DrawLineBresenham((int)a.ProjectedPosition.X, (int)a.ProjectedPosition.Y, (int)b.ProjectedPosition.X, (int)b.ProjectedPosition.Y);
            DrawLineBresenham((int)b.ProjectedPosition.X, (int)b.ProjectedPosition.Y, (int)c.ProjectedPosition.X, (int)c.ProjectedPosition.Y);
            DrawLineBresenham((int)a.ProjectedPosition.X, (int)a.ProjectedPosition.Y, (int)c.ProjectedPosition.X, (int)c.ProjectedPosition.Y);*/
    
}
void triangle_scanline(int y, vertex_t left_a,vertex_t left_b,vertex_t right_a,vertex_t right_b,SDL_Renderer* renderer)
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
            // Vector3 leftWorldEye = Interpolate(leftA.WorldViewPosition, leftB.WorldViewPosition, leftGradient);
            // Vector3 rightWorldEye = Interpolate(rightA.WorldViewPosition, rightB.WorldViewPosition, rightGradient);


            // fill pixels managing light
            for (int x = (int)leftX; x < (int)rightX; x++)
            {
                // gradient of x
                float xGradient = (x - leftX) / (float)(rightX - leftX);

                vec3_t pixelWorld = interpolate_vec3(leftWorld, rightWorld, xGradient);
                // vec3_t pixelWorldEye = interpolate_vec3(leftWorldEye, rightWorldEye, xGradient);
                vec3_t pixelNormal = interpolate_vec3(leftNormal, rightNormal, xGradient);

                // vec3_t pixelLightDirection = (Environment.Light.Position - pixelWorld).Normalized();

                // vec2_t pixelUv = Interpolate(uvLeft, uvRight, xGradient);

                // int textureX = (int)Math.Abs((pixelUv.X * (Mesh.Texture.width - 1)) % (Mesh.Texture.width - 1));
                // int textureY = (int)Math.Abs((pixelUv.Y * (Mesh.Texture.height - 1)) % (Mesh.Texture.height - 1));

                // int textureIndex = (Mesh.Texture.width * textureY + textureX) * 4;

                // byte r = Mesh.Texture.bitmap[textureIndex];
                // byte g = Mesh.Texture.bitmap[textureIndex + 1];
                // byte b = Mesh.Texture.bitmap[textureIndex + 2];

                // Vector3 textureColor = new Vector3(r / 255f, g / 255f, b / 255f);

                // lambert for diffuse component of Phong
                // float lambert = Math.Max(Vector3.Dot(pixelLightDirection, pixelNormal), 0);
                //if (lambert > 0.7f)
                //   lambert = 1f;

                //lambert = 1;

                // float shininess = 0.4f;

                // // reflect vector for specular Phong
                // Vector3 reflectedLight = Reflect(-pixelLightDirection, pixelNormal);

                // float specular = Math.Max(Vector3.Dot(reflectedLight, -pixelWorldEye.Normalized()), 0);

                // Vector3 specularColor = new Vector3(1, 1, 1) * specular * shininess;// (float)Math.Pow(specular, shininess);

                // Vector3 diffuseColor = textureColor * lambert;

                // float z = Interpolate(leftZ, rightZ, xGradient);

                // Vector3 ambientColor = new Vector3(0, 0f, 0);

                // // compute Phong (will be in hdr)
                // Vector3 HdrColor = diffuseColor + specularColor + ambientColor;
                // float maxValue = Math.Max(Math.Max(Math.Max(HdrColor.X, HdrColor.Y), HdrColor.Z), 1);

                // // normalize color to LDR
                // Vector3 finalColor = HdrColor / maxValue;

                // // manage outline
                // float outline = Vector3.Dot(pixelNormal, -pixelWorldEye.Normalized());
                // if (outline > -0.2f && outline < 0.2f)
                // {
                //     finalColor = new Vector3(1, 0, 0);
                // }

                // Phong
                SDL_RenderDrawPoint(renderer,x,y);
                // Device.PutPixel(x, y, z, finalColor);
            }
}

void mesh_draw(mesh_t mesh,camera_t camera,SDL_Renderer* rend,float window_w,float window_h)
{
    fprintf(stdout,"starting drawing mesh  \n");
    triangle_t* t = mesh.head;
    fprintf(stdout,"taked first triangle reference\n");
    while(t)
    {
        triangle_draw(*t,camera,mesh.position,mesh.scale, rend,window_w,window_h);
        // fprintf(stdout,"rendered triangle %f/%f/%f %f/%f/%f %f/%f/%f\n",t->a.x,t->a.y,t->a.z,t->b.x,t->b.y,t->b.z,t->c.x,t->c.y,t->c.z);
        t = t->next;
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
            //new list floatindex normal (3)
            list_t* list_3_normal_index = create_list();
            //new list floatindex uv (2)
            list_t* list_3_uv_index = create_list();
            
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
                        char* index_vertex = strsep(line_copy,"/");  //get first element
                        // fprintf(stdout,"line splited and return value %s \n",index);
                        int* n_index_vertex = malloc(sizeof(int));
                        *n_index_vertex = (atoi(index_vertex)); //convert to integer 
                        // fprintf(stdout,"transformed in value %d \n",*n_index);

                        char* index_uv = strsep(line_copy,"/");
                        int* n_index_uv = malloc(sizeof(int));
                        *n_index_uv = (atoi(index_uv));


                        char* index_normal = strsep(line_copy,"/");
                        int* n_index_normal = malloc(sizeof(int));
                        *n_index_normal = (atoi(index_normal));
                        
                        list_append(list_3_vec_index,n_index_vertex); //add to list
                        list_append(list_3_uv_index,n_index_uv);
                        list_append(list_3_normal_index,n_index_normal);
                        
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

        float Sign(struct doge_vec3 p1, struct doge_vec3 p2,struct doge_vec3 p3)
        {
            return (p1.x - p3.x)*(p2.y - p3.y) - (p2.x - p3.x ) * (p1.y - p3.y);
        }

        
        int PointInTriangle(struct doge_vec3 pt, struct doge_vec3 v1, struct doge_vec3 v2, struct doge_vec3 v3)
        {
            float b1, b2, b3;

            b1 = Sign(pt, v1, v2) < 0;
            b2 = Sign(pt, v2, v3) < 0;
            b3 = Sign(pt, v3, v1) < 0;

            if((b1 == b2) && (b2 == b3))
                return 0;

            return -1;
        }
         void RasterizationBoundingBox(struct doge_vec3 v1, struct doge_vec3 v2, struct doge_vec3 v3, SDL_Renderer* renderer)
        {
            // 1 triangle 20/25 fps (90 fps) no artifacts

            int maxX = (int)MAX(v1.x, MAX(v2.x, v3.x));
            int minX = (int)MIN(v1.x, MIN(v2.x, v3.x));
            int maxY = (int)MAX(v1.y, MAX(v2.y, v3.y));
            int minY = (int)MIN(v1.y, MIN(v2.y, v3.y));

            for (int x = minX; x <= maxX; x++)
            {
                for (int y = minY; y <= maxY; y++)
                {
                    struct doge_vec3 point =  create_vec3(x,y,0);

                    if (!PointInTriangle(point, v1, v2, v3))
                    {
                        SDL_RenderDrawPoint(renderer,(int)point.x, (int)point.y);
                    }
                }
            }
        }