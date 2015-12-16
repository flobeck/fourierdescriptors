#ifndef FOURIERDESCRIPTORS_H_
#define FOURIERDESCRIPTORS_H_

#include <math.h>
#include <complex.h>
#include "uthash.h"
#include <stdlib.h>
#include <stdio.h>

static int H = 200;
static int W = 200;

int Ncontour_points();
void get_boundary(double complex* b, int N);

struct _Z_{
  int arg;
  double complex res;
  UT_hash_handle hh;
};

void add_Z(struct _Z_ *_z_);
struct _Z_* get_value(int arg);
double complex Z(int N, double complex* z, int k);
void compute_FD(double complex* fourierdesc, int N, double complex z[N]);
double complex z_(int N, double complex z[N], int M, int m);
void compute_z_(double complex* zs, int N, double complex z[N], int M);
void fd_pixels(int result_image[static H][W], int N, double complex* z);
char* png_filename(char* path, char* type, int file_number);


#endif  /* FOURIERDESCRIPTORS_H */

