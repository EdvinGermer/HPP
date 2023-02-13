#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>






/*############### Define a struct for the particles ##############*/
typedef struct particle{
    double x;
    double y;
    double m;
    double vx;
    double vy;
    double brightness;
} particle_t;




/*############### Define update function ##############*/
void update_particle(particle_t* particles, int i, double N, double dt, double G, double e0)
{
    double Fx=0;
    double Fy=0;
    double r2;

    // Calculate force
    for (int j=0; j<N; j++)   // Iterate over all particles
    {
        if (j!=i)             // Do not calculate for the same particle
        {
            // Calculate r^2
            r2 = pow(particles[i].x-particles[j].x,2) + pow(particles[i].y-particles[j].y,2);
            
            // Sum up all contributions in x and y directions
            Fx += (particles[j].m/r2) * (particles[i].x-particles[j].x);
            Fy += (particles[j].m/r2) * (particles[i].y-particles[j].y);
        }
    }
    Fx = -G*particles[i].m*Fx;
    Fy = -G*particles[i].m*Fy;

    // Update velocity
    particles[i].vx = particles[i].vx + dt*(Fx/particles[i].m);
    particles[i].vy = particles[i].vy + dt*(Fy/particles[i].m);

    // Update position
    particles[i].x = particles[i].x + dt*particles[i].vx;
    particles[i].y = particles[i].y + dt*particles[i].vy;
};




int main(int argc, char *argv[])
{
   
    /*############### Setup ##############*/
    
    int N = 10;
    int nsteps = 200;
    int idx = 0;

    char* filename1= "/home/edge9521/HPP/A3/input_data/ellipse_N_00010.gal";
    char* filename2= "/home/edge9521/HPP/A3/ref_output_data/ellipse_N_00010_after200steps.gal";


    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    if (particles == NULL) 
    {printf("ERROR: Could not allocate memory for %d particles\n", N);}


    /*############### Read data ##############*/
    FILE *input_file = fopen(filename1, "rb");
    if (input_file == NULL)
    {printf("ERROR: Could not open file \n");}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, input_file);}
    fclose(file);



    /*############### Run simulation ##############*/
    for (i=0; i<nsteps; i++)         // For every time step
    {
        for (int idx=0;idx<N;idx++)  // Update every particle
        {update_particle(particles,idx, N, dt, G, e0);}
    }
   

    /*############### Print my final data ##############*/
    
    particle_t *correct = malloc(N * sizeof(particle_t));

    FILE *correct_file = fopen(filename2, "rb");
    if (correct_file == NULL)
    {printf("ERROR: Could not open file \n");}

    for (i=0;i<N;i++)
    {fread(&correct[i], sizeof(particle_t), 1, correct_file);}
    fclose(correct_file);
    
    printf("\nMy final data\n");
    printf("    Particle: %d\n", i);
    printf("         x          = %f\n", correct[idx].x);
    printf("         y          = %f\n", correct[idx].y);
    printf("         m          = %f\n", correct[idx].m);
    printf("         vx         = %f\n", correct[idx].vx);
    printf("         vy         = %f\n", correct[idx].vy);
    printf("         brigthness = %f\n", correct[idx].brightness);






/*############### Print correct final data ##############*/
   
   
   
   
   
   
    printf("\nThe correct final data\n");
    printf("    Particle: %d\n", i);
    printf("         x          = %f\n", particles[idx].x);
    printf("         y          = %f\n", particles[idx].y);
    printf("         m          = %f\n", particles[idx].m);
    printf("         vx         = %f\n", particles[idx].vx);
    printf("         vy         = %f\n", particles[idx].vy);
    printf("         brigthness = %f\n", particles[idx].brightness);


    /*############### Free memory ##############*/
    free(particles);

    return 0;
}