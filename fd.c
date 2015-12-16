/*
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng15 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <png.h>
#include <math.h>
#include <complex.h>
#include "uthash.h"
#include "fourierdescriptors.h"

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} pixel_t;

typedef struct  {
  pixel_t *pixels;
  size_t width;
  size_t height;
} bitmap_t;

/* Given "bitmap", this returns the pixel of bitmap at the point
   ("x", "y"). */

static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y){
  return bitmap->pixels + bitmap->width * y + x;
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error. */

static int save_png_to_file (bitmap_t *bitmap, const char *path){
  FILE * fp;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  size_t x, y;
  png_byte ** row_p = NULL;
  /* "status" contains the return value of this function. At first
            it is set to a value which means 'failure'. When the routine
                   has finished its work, it is set to a value which means
                   'success'. */
  int status = -1;
  /* The following number is set by trial and error only. I cannot
            see where it it is documented in the libpng manual.
  */
  int pixel_size = 3;
  int depth = 8;

  fp = fopen (path, "wb");
  if (! fp) {
    goto fopen_failed;
  }

  png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
    goto png_create_write_struct_failed;
  }

  info_ptr = png_create_info_struct (png_ptr);
  if (info_ptr == NULL) {
    goto png_create_info_struct_failed;

  }
  /* Set up error handling. */
  if (setjmp (png_jmpbuf (png_ptr))) {
    goto png_failure;
  }
  /* Set image attributes. */

  png_set_IHDR (png_ptr,
                info_ptr,
                bitmap->width,
                bitmap->height,
                depth,
                PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);

  /* Initialize rows of PNG. */
  row_p =png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
  for (y = 0; y < bitmap->height; ++y) {
            png_byte *row =
              png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * pixel_size);
            row_p[y] = row;
            for (x = 0; x < bitmap->width; ++x) {
              pixel_t * pixel = pixel_at (bitmap, x, y);
              *row++ = pixel->red;
              *row++ = pixel->green;
              *row++ = pixel->blue;
            }
  }

  /* Write the image data to "fp". */
  png_init_io (png_ptr, fp);
  png_set_rows (png_ptr, info_ptr, row_p);
  png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  /* The routine has successfully written the file, so we set
     "status" to a value which indicates success. */
  status = 0;

  for (y = 0; y < bitmap->height; y++) {
    png_free (png_ptr, row_p[y]);
  }
  png_free (png_ptr, row_p);

 png_failure:
 png_create_info_struct_failed:
  png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
  fclose (fp);
 fopen_failed:
  return status;  
}

int main(int argc, char *argv[]) {
  system("python2 contour.py");
  int N = Ncontour_points();
  
  double complex* fourierdesc = malloc(sizeof(complex double)*N);
  double complex* contour_points_complex = malloc(sizeof(complex double)*N);

  get_boundary(contour_points_complex,N);
  compute_FD(fourierdesc,N,contour_points_complex);

  int result_image[H][W];

  for(int M = 2; M < N/4; M+=1){

    memset(result_image, 0, sizeof(int)*H*W); 
    printf("Progress: %d / %d \n",M,N/4);

    double complex* zs  = malloc(sizeof(complex double)*N);
    compute_z_(zs,N,contour_points_complex,M);
    fd_pixels(result_image, N, zs);

    bitmap_t result;
    int x, y;

    result.width = W-1;
    result.height = H-1;
    result.pixels = calloc (sizeof (pixel_t), result.width * result.height);
    
    for (y = 0; y < result.height; y++) {
      for (x = 0; x < result.width; x++) {
        pixel_t * pixel = pixel_at (&result, x, y);
	
        if(result_image[y][x]){
	  pixel->red = pixel->green = pixel->blue = 0;
	}else{
          pixel->red = pixel->green = pixel->blue = 230;
        }
      }
    }

    char *png_fname = png_filename("./result/", ".png", M);
    save_png_to_file (&result, png_fname);    

    free(png_fname);
    free(zs);
    
  }

  free(contour_points_complex);
  free(fourierdesc);


  return 0;
}
