#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*############### Define a struct for the particles ##############*/
typedef struct particle{
    double x;
    double y;
    double m;
    double vx;
    double vy;
    double brightness;
} particle_t;


// gcc -o print_result print_result.c
int main(int argc, char *argv[])
{
    int N = 2;
    int i;

    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    if (particles == NULL) 
    {printf("ERROR: Could not allocate memory for %d particles\n", N);}

    /*############### Read data ##############*/
    FILE *file = fopen("result.gal", "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", "result.gal");}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);

    /*############### Print data ##############*/
    printf("Read final data:\n");
    for (i=0;i<N;i++)
    {
        printf("    Particle: %d\n", i);
        printf("         x          = %f\n", particles[i].x);
        printf("         y          = %f\n", particles[i].y);
        printf("         m          = %f\n", particles[i].m);
        printf("         vx         = %f\n", particles[i].vx);
        printf("         vy         = %f\n", particles[i].vy);
        printf("         brigthness = %f\n", particles[i].brightness);
    }

    /*############### Free memory ##############*/
    free(particles);

    return 0;
}