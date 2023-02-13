#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
    int N = 10;
    int nsteps = 200;
    int i;

    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    if (particles == NULL) 
    {printf("ERROR: Could not allocate memory for %d particles\n", N);}


    /*############### Read data ##############*/    
    FILE *input_file = fopen("/home/edge9521/HPP/A3/input_data/ellipse_N_00010.gal", "rb");
    if (input_file == NULL)
    {printf("ERROR: Could not open file\n");}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, input_file);}
    fclose(input_file);


    /*############### Run simulation ##############*/
    double G = 100/N;  // Gravity 
    double e0 = 10e-3; // Gravity correctional term
    double dt = 10e-5; // Timestep

    
    for (i=0; i<nsteps; i++)         // For every time step
    {
        for (int idx=0;idx<N;idx++)  // Update every particle
        {update_particle(particles,idx, N, dt, G, e0);}
    }
    
   

    /*############### Print my final position ##############*/
    printf("\n\nPRINT MY FINAL POINT:\n");
    printf("    Particle: %d\n", 0);
    printf("         x          = %f\n", particles[0].x);
    printf("         y          = %f\n", particles[0].y);
    printf("         m          = %f\n", particles[0].m);
    printf("         vx         = %f\n", particles[0].vx);
    printf("         vy         = %f\n", particles[0].vy);
    printf("         brigthness = %f\n", particles[0].brightness);




    /*############### Print correct final position ##############*/
    FILE *compare_file = fopen("/home/edge9521/HPP/A3/ref_output_data/ellipse_N_00010_after200steps.gal", "rb");
    if (compare_file == NULL)
    {printf("ERROR: Could not open file\n");}

    particle_t *correct = malloc(N*sizeof(particle_t));

    for (i=0;i<N;i++)
    {fread(&correct[i], sizeof(particle_t), 1, compare_file);}
    fclose(compare_file);

    printf("\n\nPRINT CORRECT FINAL POINT:\n");
    printf("    Particle: %d\n", 0);
    printf("         x          = %f\n", correct[0].x);
    printf("         y          = %f\n", correct[0].y);
    printf("         m          = %f\n", correct[0].m);
    printf("         vx         = %f\n", correct[0].vx);
    printf("         vy         = %f\n", correct[0].vy);
    printf("         brightness = %f\n", correct[0].brightness);





    /*############### Print difference in final position ##############*/
    printf("\n\nPRINT DIFFERENCE IN FINAL POINT:\n");
    printf("    Particle: %d\n", 0);
    printf("         dx          = %f\n", particles[0].x-correct[0].x);
    printf("         dy          = %f\n", particles[0].y-correct[0].y);
    printf("         dm          = %f\n", particles[0].m-correct[0].m);
    printf("         dvx         = %f\n", particles[0].vx-correct[0].vx);
    printf("         dvy         = %f\n", particles[0].vy-correct[0].vy);
    printf("         dbrightness = %f\n", particles[0].brightness-correct[0].brightness);





    /*############### Free memory ##############*/
    free(correct);

    return 0;
}