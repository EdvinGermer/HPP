#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>

// N = 100
// nsteps = 1000;
// ./galsim 100 /home/edge9521/HPP/A3/input_data/ellipse_N_00100.gal 1000 10e-5 1

// N = 10
// nsteps = 1000;
// ./galsim 10 /home/edge9521/HPP/A3/input_data/ellipse_N_00010.gal 1000 10e-5 1


// N = 2
// nsteps = 1000;
// ./galsim 2 /home/edge9521/HPP/A3/input_data/circles_N_2.gal 1000 10e-5 1



/*############### Define a struct for the particles ##############*/
// 6 doubles for each particle
// Total size = N * 6 
typedef struct particle{
    double x;
    double y;
    double m;
    double vx;
    double vy;
    double brightness;
} particle_t;

/*############### Graphics settings ##############*/
const float circleRadius=0.010, circleColor=0;
const int windowWidth=800;
const int L=1, W=1;

/*############### Graphics function ##############*/
void print_pos(particle_t* particles, int N)
{
    printf("Read particle data:\n");
    for (int i=0;i<N;i++)
    {
        printf("    Particle: %d\n", i);
        printf("         x          = %f\n", particles[i].x);
        printf("         y          = %f\n", particles[i].y);
        printf("         m          = %f\n", particles[i].m);
        printf("         vx         = %f\n", particles[i].vx);
        printf("         vy         = %f\n", particles[i].vy);
        printf("         brigthness = %f\n", particles[i].brightness);
    }
}

/*############### Define update function ##############*/
void update_particle(particle_t* particles, int i, double N, double dt, double G, double e0)
{
    // i is current particle index

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
            Fx += (particles[j].m/r2) * (particles[i].x-particles[j].y);
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
    /*############### Read arguments ##############*/
    if (argc != 6)
    {printf("Error: This function takes 5 arguments, but got %d\n", argc - 1);}
    
    int N = atoi(argv[1]);                   // Number of stars
    char* filename = argv[2];                // filename
    double nsteps = strtod(argv[3], NULL);   // How many steps to simulate
    double dt = strtod(argv[4], NULL);       // Timestep
    int graphics = atoi(argv[5]);            // Graphics on or off
    
    /*############### Initialize other variables ##############*/
    int i;
    double G = 100/N;  // Gravity 
    double e0 = 10e-3; // Gravity correctional term

    printf("\nIntial setup:\n");
    printf("    N        = %d\n", N);
    printf("    filename = %s\n", filename);
    printf("    nsteps   = %f\n", nsteps);
    printf("    dt       = %f\n", dt);
    printf("    graphics = %d\n", graphics);
    printf("    G        = %f\n", G);
    printf("    e0       = %f\n\n", e0);

    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    if (particles == NULL) 
    {printf("ERROR: Could not allocate memory for %d particles\n", N);}

    /*############### Read data ##############*/
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", filename);}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);

    /*############### Print initial data ##############*/
    printf("Read initial data:\n");
    print_pos(particles,N);

    /*############### Update positions ##############*/
    if (graphics == 0)
    {
        for (i=0; i<nsteps; i++)         // For every time step
        {
            for (int idx=0;idx<N;idx++)  // Update every particle
            {update_particle(particles,idx, N, dt, G, e0);}
        }
    }

    else if (graphics == 1)
    {
        InitializeGraphics(argv[0],windowWidth,windowWidth);
        SetCAxes(0,1);
        for (i=0; i<nsteps; i++)         // For every time step
        {
            ClearScreen();
            for (int idx=0;idx<N;idx++)  // Update every particle
            {
                DrawCircle(particles[idx].x, particles[idx].y, L, W, circleRadius, circleColor);
                update_particle(particles,idx, N, dt, G, e0);
            }

            Refresh();
            usleep(3000); // avoid screen flickering

            usleep(10000); // sleep for microseconds
        }  

        // Close display when done
        FlushDisplay();
        CloseDisplay();
    }

    else
    {printf("ERROR: Invalid input for graphics '%d'\n", graphics);}



    /*############### Print final data ##############*/
    printf("Read final data:\n");
    print_pos(particles,N);


    /*############### Create output file ##############*/
    // Open file
    FILE *file_res = fopen("result.gal", "wb");
    if (file==NULL)
    {printf("ERROR: Could not create file %s\n", filename);}

    // Write to the file
    for (i = 0; i < N; i++)
    {fwrite(&particles[i], sizeof(particle_t), 1, file_res);}

    // Close the file
    fclose(file_res);  


    /*############### Free memory ##############*/
    free(particles);

    return 0;
}