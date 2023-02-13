#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>


// gcc -o troubleshoot troubleshoot.c -lm

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

    double r, r_x, r_y;

    // Calculate force
    for (int j=0; j<N; j++)   // Iterate over all particles
    {
        if (j!=i)             // Do not calculate for the same particle
        {
            r_x = particles[i].x - particles[j].x;    // (x_i - k_j)
            r_y = particles[i].y - particles[j].y;    // (y_i - y_j)

            r = sqrt( pow(r_x,2) + pow(r_y,2) );
            
            // Sum up all contributions in x and y directions
            Fx += (particles[j].m/pow(r + e0,3)) * r_x;
            Fy += (particles[j].m/pow(r + e0,3)) * r_y;
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
    // CHANGE THIS TO TEST DIFFERENT CONFIGURATION
    int idx = 0;   //Which particle to check
    int N = 10;
    int nsteps = 200;
    char* input_dir= "/home/edge9521/HPP/A3/input_data/ellipse_N_00010.gal";
    char* correct_dir= "/home/edge9521/HPP/A3/ref_output_data/ellipse_N_00010_after200steps.gal";


    /*############### Inital Setup ##############*/
    int i;
    double G = 100/N;  // Gravity 
    double e0 = 10e-3; // Gravity correctional term
    double dt = 10e-5; // Time step


    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    if (particles == NULL) 
    {printf("ERROR: Could not allocate memory for %d particles\n", N);}


    /*############### Read raw input data ##############*/
    FILE *input_file = fopen(input_dir, "rb");
    if (input_file == NULL)
    {printf("ERROR: Could not open file \n");}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, input_file);}
    fclose(input_file);


    /*############### Print correct final data ##############*/
    particle_t *correct = malloc(N * sizeof(particle_t));

    FILE *correct_file = fopen(correct_dir, "rb");
    if (correct_file == NULL)
    {printf("ERROR: Could not open file \n");}

    for (i=0;i<N;i++)
    {fread(&correct[i], sizeof(particle_t), 1, correct_file);}
    fclose(correct_file);
    
    printf("\nCorrect final data\n");
    printf("    Particle: %d\n", idx);
    printf("         x          = %f\n", correct[idx].x);
    printf("         y          = %f\n", correct[idx].y);
    printf("         m          = %f\n", correct[idx].m);
    printf("         vx         = %f\n", correct[idx].vx);
    printf("         vy         = %f\n", correct[idx].vy);
    printf("         brigthness = %f\n", correct[idx].brightness);



    /*############### Run simulation ##############*/
    for (i=0; i<nsteps; i++)         // For every time step
    {
        for (int j=0;j<N;j++)  // Update every particle
        {update_particle(particles,j, N, dt, G, e0);}
    }
   

   /*############### Print my final data ##############*/
    printf("\nMy final data\n");
    printf("    Particle: %d\n", idx);
    printf("         x          = %f\n", particles[idx].x);
    printf("         y          = %f\n", particles[idx].y);
    printf("         m          = %f\n", particles[idx].m);
    printf("         vx         = %f\n", particles[idx].vx);
    printf("         vy         = %f\n", particles[idx].vy);
    printf("         brigthness = %f\n", particles[idx].brightness);


    /*############### Print difference ##############*/
    printf("\nDifference in output\n");
    printf("    Particle: %d\n", idx);
    printf("         d_x          = %f\n", particles[idx].x-correct[idx].x);
    printf("         d_y          = %f\n", particles[idx].y-correct[idx].y);
    printf("         d_m          = %f\n", particles[idx].m-correct[idx].m);
    printf("         d_vx         = %f\n", particles[idx].vx-correct[idx].vx);
    printf("         d_vy         = %f\n", particles[idx].vy-correct[idx].vy);
    printf("         d_brigthness = %f\n", particles[idx].brightness-correct[idx].brightness);


    /*############### Free memory ##############*/
    free(particles);
    free(correct);

    return 0;
}