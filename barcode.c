#include "barcode.h"
#include <stdio.h>
#include <string.h>


struct image* barcode(char* data, int width, int height) {
    data = data; // This line is just to avoid unused parameter warning. You can remove it . Why is this needed?
    
    struct image* img = malloc(sizeof(struct image));
    img->width = width;
    img->height = height;
    img->pixels = malloc(width * height * sizeof(int)); 
    memset(img->pixels, 0, width * height * sizeof(int));
    return img;
}

