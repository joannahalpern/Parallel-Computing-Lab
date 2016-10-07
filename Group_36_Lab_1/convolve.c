/* Jonah Ryan-Davis | 260518588
 * Joanna Halpern   | 260410862
 * ECSE 420 | Lab 1 | McGill
 * Oct 3 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "lodepng.h"
#include "wm.h"

void process(char* input_filename, char* output_filename, int thread_cnt)
{
    /* Load png */
    unsigned error;
    unsigned char *image, *new_image;
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, input_filename);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    int new_image_width = width - 2;
    int new_image_height = height - 2;
    int new_image_size_pixels = (new_image_width) * (new_image_height);
    new_image = malloc(new_image_size_pixels * 4 *sizeof(unsigned char));

    /* Process image */
    int idx;
    #pragma omp parallel for num_threads(thread_cnt)
    for (idx = 0; idx < new_image_size_pixels; idx++)
    {
        int new_image_x = (idx % new_image_width);
        int new_image_y = (idx / new_image_width);
        int image_x = (new_image_x + 1);
        int image_y = (new_image_y + 1);
        
        int c = 0;
        for(c = 0; c < 3; c++)
        {
            int new_pixel = 0;
            int i, j;
            for (i = 0; i < 3; i++) 
            {
                for (j = 0; j < 3; j++)
                {
                    new_pixel += image[4*(image_x - 1 + i) + 4*width*(image_y - 1 + j) + c] * w[j][i];
                }
            }
            if (new_pixel < 0) {
                new_pixel = 0;
            }
            if (new_pixel > 255) {
                new_pixel = 255;
            }
            new_image[idx*4 + c] = new_pixel;
        }
        new_image[idx*4 + 3] = (unsigned char)255;
    }
    

    /* Store png */
    lodepng_encode32_file(output_filename, new_image, new_image_width, new_image_height);

    free(image);
    free(new_image);
}

int main(int argc, char *argv[])
{
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    int thread_cnt = atoi(argv[3]);

    process(input_filename, output_filename, thread_cnt);

    return 0;
}
