#include <stdio.h>
#include <string.h>
#include "header.h"

#define TEST(f) if(f()) return -1;
#define COMPARE(x,y) (x <= y+0.0001 && x >= y-0.00001) ? 0 : -1

int test_string_split()
{
    int ret = 0;
    char* string = "ciao mamma pippo";
    list_t* my_list = string_split(string," ");
    if(list_len(*my_list) != 3) ret = -1;
    free(my_list);
    return ret;
}

int test_parse_line_vertex()
{
    obj_parser_context_t* ctx = obj_parser_init();
    if(!ctx)
        return -1;
    int ret = 0;
    char* line = "1.34 0.32 5.4789";
    int v = obj_parse_line_vertex(ctx, line);
    if(v)
    {
        ret = -1;
        goto end;
    }
    if(COMPARE(*(*vec3_t)get_item_at_index(ctx.list_v,0).x,1.34) || COMPARE(*(*vec3_t)get_item_at_index(ctx.list_v,0).y, 0.32) || COMPARE(*(*vec3_t)get_item_at_index(ctx.list_v,0).z,5.4789))
        ret = -1;
    if(ctx->counter_v != 1)
    {
        ret = 1;
        fprintf(stdout,"counter not incremented \n");        
    }
    end:
    return ret;
}
int test_parse_line_vertex_negative()
{
    obj_parser_context_t* ctx = obj_parser_init();
    if(!ctx)
        return -1;
    int ret = 0;
    char* line = "-1.34 -0.32 5.4789";
    vec3_t v = obj_parse_line_vertex(ctx,line);
    if(COMPARE(v.x,-1.34) || COMPARE(v.y, -0.32) || COMPARE(v.z,5.4789))
        ret = -1;
    if(ctx->counter_v != 1)
    {
        ret = 1;
        fprintf(stdout,"counter not incremented \n");        
    }
    return ret;
}
int test_parse_line_uv()
{
    obj_parser_context_t* ctx = obj_parser_init();
    if(!ctx)
        return -1;
    int ret = 0;
    char* line = "-1.34 -0.32";
    vec2_t v = obj_parse_line_uv(ctx,line);
    if(COMPARE(v.x,-1.34) || COMPARE(v.y, -0.32))
        ret = -1;
    if(ctx->counter_vt != 1)
    {
        ret = 1;
        fprintf(stdout,"counter not incremented \n");        
    }
    return ret;
}
int test_parse_line_normal()
{
    obj_parser_context_t* ctx = obj_parser_init();
    if(!ctx)
        return -1;
    int ret = 0;
    char* line = "-1.34 -0.32 5.4789";
    vec3_t v = obj_parse_line_normal(ctx,line);
    if(COMPARE(v.x,-1.34) || COMPARE(v.y, -0.32) || COMPARE(v.z,5.4789))
        ret = -1;
    if(ctx->counter_vn != 1)
    {
        ret = 1;
        fprintf(stdout,"counter not incremented \n");        
    }
    return ret;
}
int test_obj_parse_indexes_vertex()
{
    obj_parser_context_t* ctx = obj_parser_init();
    if(!ctx)
        return -1;
    int ret = 0;
    char* line = "1/2/1"; //v/uv/vn

    char* line2 = "-1.34 -0.32 5.4789";
    char* line3 = "0.54 4.5687";
    char* line4 = "10.598 4.5687";
    
    vec3_t v = obj_parse_line_vertex(ctx,line2);
    vec3_t n = obj_parse_line_normal(ctx,line2);
    vec2_t uv = obj_parse_line_uv(ctx,line3);
    vec2_t uv2 = obj_parse_line_uv(ctx,line4);
    
    vertex_t vertex =  obj_parse_indexes_vertex(ctx,line);
    if(COMPARE(v.x, -1.34) || COMPARE(n.y, -0.32) || COMPARE(uv.x,10.598))
        ret = -1;
        
    return ret;    
}


int main (int argc, char* args[])
{
    TEST(test_string_split);
    TEST(test_parse_line_vertex);
    TEST(test_parse_line_vertex_negative);
    TEST(test_parse_line_uv);
    TEST(test_parse_line_normal);
    TEST(test_obj_parse_indexes_vertex);
    fprintf(stdout,"all test passed\n");
}

