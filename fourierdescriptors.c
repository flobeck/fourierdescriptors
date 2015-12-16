/*
 Fourier Descriptors
 flobeck
 
*/

#include "fourierdescriptors.h"

int Ncontour_points(){
  FILE *file = fopen("./f", "r");
  int lines = 0;
  int c = 0;

  while(!(feof(file))){
    c = fgetc(file);
    if(c == '\n')
      lines++;
  }
  fclose(file);
                  // divide by two because two
  return lines/2; // subsequent lines are x and y coord.
}

//double complex *
void get_boundary(double complex* b, int N) {
  int lines = 0;
  int k = 0;
  FILE *file = fopen("./f", "r");
  int coords[N*2];

  int num;
  while(fscanf(file, "%d", &num) > -1) {
    coords[lines] = num;
    lines++;
  }
  fclose(file);

  for(int j = 0; j<lines; j+=2){
    b[k] = coords[j] + coords[j+1]*I;
    k++;
  }
}

struct _Z_ *Z_hash = NULL;

struct _Z_* get_value(int arg){
  struct _Z_ *z;
  HASH_FIND_INT(Z_hash, &arg, z);
  return z;
}

void add_Z(struct _Z_ *_z_){
  HASH_ADD_INT(Z_hash,arg,_z_);  // _z_ is the struct containing key and values pair
}

double complex Z(int N, double complex* z, int k){
  if(get_value(k) != NULL)
    return get_value(k)->res;
  
  double complex sum = 0.0 + 0.0*I;
  
  for(int m = 0; m < N; m++)
    sum += z[m]*cexp(-I*2.0*M_PI*m*k/(double)N);
  
  struct _Z_ *_z_ = malloc(sizeof(struct _Z_));  
  _z_->arg = k;
  _z_->res = sum/(double)N;
  
  add_Z(_z_);  
  return sum/(double)N;
}

void compute_FD(double complex* fourierdesc, int N, double complex z[N]){
  for (int i = 0; i < N; i++)
    fourierdesc[i] = Z(N,z,i);
}

double complex z_(int N, double complex z[N], int M, int m){
  double complex sum = 0.0 + 0.0*I;

  for(int k = -M/2; k < M/2; k++)
    sum += Z(N, z, k) * cexp(I*2.0*M_PI*m*k/(double)N);

  return sum;
}

void compute_z_(double complex* zs, int N, double complex z[N], int M){
  for (int i = 0; i < N; i++)
    zs[i] = z_(N,z,M,i);
}

void fd_pixels(int result_image[static H][W], int N, double complex* z){
  int x, y;
  for(int i = 0; i < N; i++){    
    x = floor(creal(z[i]));
    y = floor(cimag(z[i]));
    result_image[y%H][x%W] = 1;
  }
}

char* png_filename(char* path, char* type, int file_number){
  int digits = floor(log10 (abs(file_number)))+1;
  char png_filename[digits];
  sprintf(png_filename, "%04d", file_number);
  char *res = malloc(1+strlen(path)+strlen(png_filename)+strlen(type));
  strcpy(res, path);
  strcat(res, png_filename);
  strcat(res, type);
  printf("%s\n", res);
  return res;
}
