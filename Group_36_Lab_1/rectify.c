/* Jonah Ryan-Davis | 260518588
 * Joanna Halpern   | 260410862
 * ECSE 420 | Lab 1 | McGill
 * Oct 3 2016 
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "lodepng.h"

void process(char* input_filename, char* output_filename, int thread_cnt)
{
    /* Load png */
    unsigned error;
    unsigned char *image, *new_image;
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, input_filename);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    int image_size_pixels = width * height;
    int image_size_bytes = image_size_pixels * 4;
    new_image = malloc(image_size_bytes * sizeof(unsigned char));


    /* Process image */
    int idx;
    #pragma omp parallel for num_threads(thread_cnt)
    for (idx = 0; idx < image_size_bytes; idx+=4)
    {
        new_image[idx + 0] = (image[idx + 0] < 127 ? 127 : image[idx + 0]); //R
        new_image[idx + 1] = (image[idx + 1] < 127 ? 127 : image[idx + 1]); //g
        new_image[idx + 2] = (image[idx + 2] < 127 ? 127 : image[idx + 2]); //b
        new_image[idx + 3] = image[idx + 3];
    }
    

    /* Store png */
    lodepng_encode32_file(output_filename, new_image, width, height);

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
