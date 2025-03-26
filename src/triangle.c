#include "triangle.h"
#include "display.h"

void int_swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my, uint32_t color){
    float inverse_slope_start = (float)(x1 - x0) / (y1 - y0);
    float inverse_slope_end = (float)(mx- x0) / (my - y0);

    int x_start = x0;
    int x_end = x0;

    for(int y = y0; y <= my; y++){
        draw_line(x_start, y, x_end, y, color);

        x_start += inverse_slope_start;
        x_end += inverse_slope_end;
    }
}

void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2, uint32_t color){

}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    if(y0 > y1){
        int_swap(&y0, &y1);
    }

    if(y1 > y2){
        int_swap(&y1, &y2);
    }

    if(y0 > y1){
        int_swap(&y0, &y1);
    }

    // calculates new vector (mx, my) using triangle similarity
    int my = y1;
    int mx = ( ( (float)(x2 - x0) * (y1 - y0) ) / (float)(y2 - y0) ) + x0;


    fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);

    fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);

}
