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

/*############### Graphics settings ##############*/
const float circleRadius=0.005, circleColor=0;
const int windowWidth=800;
const int L=1, W=1;

/*############### Print function ##############*/
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
void update_particle(particle_t* particles, particle_t* temp, int i, double N, double dt, double G, double e0)
{
    double Fx=0.0;
    double Fy=0.0;
    double r, r3, r_x, r_y;

    // Calculate force
    for (int j=0; j<N; j++)   // Iterate over all particles
    {
        if (j!=i)             // Do not calculate for the same particle
        {
            r_x = particles[i].x - particles[j].x;    // (x_i - x_j)
            r_y = particles[i].y - particles[j].y;    // (y_i - y_j)

            r = sqrt( (pow(r_x,2)) + pow(r_y,2) );
            r3 = pow(r+e0,3);

            // Sum up all contributions in x and y directions
            Fx += (particles[j].m/r3) * r_x;
            Fy += (particles[j].m/r3) * r_y;
        }
    }
    Fx *= -G*particles[i].m;
    Fy *= -G*particles[i].m;

    // Update velocity
    particles[i].vx += dt*(Fx/particles[i].m);
    particles[i].vy += dt*(Fy/particles[i].m);

    // Update position
    temp[i].x = particles[i].x + dt*particles[i].vx;
    temp[i].y = particles[i].y + dt*particles[i].vy;


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
    double e0 = 0.001; // Gravity correctional term

    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    if (particles == NULL) 
    {printf("ERROR: Could not allocate memory for %d particles\n", N);}

    particle_t *temp = malloc(N * sizeof(particle_t));  // temporary array for storing results

    /*############### Read data ##############*/
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", filename);}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);


    /*############### Update positions ##############*/
    if (graphics == 0)
    {
        for (i=0; i<nsteps; i++)         // For every time step
        {
            // Update every particle
            for (int j=0;j<N;j++)  
            {update_particle(particles, temp, j, N, dt, G, e0);}

            // Copy over result from temp to particles
            for (int j=0;j<N;j++)  // Update every particle
            {
                particles[j].x = temp[j].x;
                particles[j].y = temp[j].y;
            }
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
                update_particle(particles, temp, idx, N, dt, G, e0);
            }

            // Copy over result from temp to particles
            for (int j=0;j<N;j++)  // Update every particle
            {
                particles[j].x = temp[j].x;
                particles[j].y = temp[j].y;
            }

            Refresh();
            usleep(3000); // avoid screen flickering
        }  

        // Close display when done
        FlushDisplay();
        CloseDisplay();
    }

    else
    {printf("ERROR: Invalid input for graphics '%d'\n", graphics);}


    /*############### Create output file ##############*/
    // Open file
    FILE *file_res = fopen("result.gal", "wb");
    if (file_res==NULL)
    {printf("ERROR: Could not create file result.gal\n");}

    // Write to the file
    for (i = 0; i < N; i++)
    {fwrite(&particles[i], sizeof(particle_t), 1, file_res);}

    // Close the file
    fclose(file_res);  

    /*############### Free memory ##############*/
    free(particles);
    free(temp);

    return 0;
}