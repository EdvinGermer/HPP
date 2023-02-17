#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>

// testing the code with N = 500 and 200 steps seems to be sufficient to tell if performance is improved
// time ./galsim 500 /home/edge9521/HPP/A3/input_data/ellipse_N_05000.gal 200 0.00001 0

// Time to beat = 1,148s

/*############### Define a struct for the particles ##############*/
typedef struct{
    double x;
    double y; 
    const double m;
    double vx;
    double vy;
    const double brightness;
} particle_t;

typedef struct{
    double x;
    double y;
} temp_t;

/*############### Graphics settings ##############*/
const float circleRadius=0.005, circleColor=0;
const int windowWidth=800;
const int L=1, W=1;


/*############### Define get distance function ##############*/
double** get_dist(double** distances, particle_t* particles, const double N)
{
    for (int i=0;i<N-1;i++)
        for (int j=i+1;j<N;j++)
        {
            distances[i][j] = sqrt((particles[i].x-particles[j].x)*(particles[i].x-particles[j].x) + (particles[i].y-particles[j].y)*(particles[i].y-particles[j].y));
            distances[j][i] = distances[i][j];
        }
    return distances;
}


/*############### Define update function for each particle ##############*/
void update_particle(particle_t* particles, temp_t* temp, double** distances, int i, const double N, const double dt, const double G, const double e0)
{
    double Fx=0.0;
    double Fy=0.0;
    double r3, r_x, r_y;

    // Calculate force
    for (int j=0; j<N; j++)   // Iterate over all particles
    {
        if (j!=i)             // Do not calculate for the same particle
        {
            r_x = particles[i].x - particles[j].x;    // (x_i - x_j)
            r_y = particles[i].y - particles[j].y;    // (y_i - y_j)
       
            r3 = (distances[i][j]+e0)*(distances[i][j]+e0)*(distances[i][j]+e0);  

            // Sum up all contributions in x and y directions
            Fx += (particles[j].m/r3) * r_x;
            Fy += (particles[j].m/r3) * r_y;
        }
    }

    // Update velocity
    particles[i].vx += dt*Fx*-G;
    particles[i].vy += dt*Fy*-G;

    // Update position
    temp[i].x = particles[i].x + dt*particles[i].vx;
    temp[i].y = particles[i].y + dt*particles[i].vy;
};

int main(int argc, char *argv[])
{
    /*############### Read arguments ##############*/
    if (argc != 6)
    {printf("Error: This function takes 5 arguments, but got %d\n", argc - 1);}
    
    const int N = atoi(argv[1]);                   // Number of stars
    const char* filename = argv[2];                // filename
    const double nsteps = strtod(argv[3], NULL);   // How many steps to simulate
    const double dt = strtod(argv[4], NULL);       // Timestep
    const int graphics = atoi(argv[5]);            // Graphics on or off
    
    /*############### Initialize other variables ##############*/
    int i;
    const double G = 100.0/N;  // Gravity      
    const double e0 = 0.001; // Gravity correctional term

    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    temp_t *temp = malloc(N * sizeof(temp_t));  // temporary array for storing results

    int size = N;
    double** distances = (double**)malloc(size * sizeof(double*));
    for (int i=0;i<size; i++)
        distances[i] = (double*)malloc(size * sizeof(double));


    /*############### Read data ##############*/
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", filename);}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);

    /*############### Run Simulation ##############*/

    if (graphics == 0)
    {
        for (i=0; i<nsteps; i++)         // For every time step
        {
            // get all distances
            distances = get_dist(distances, particles, N);

            // Update every particle
            for (int j=0;j<N;j++)  
            {update_particle(particles, temp, distances, j, N, dt, G, e0);}

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
            // get all distances
            distances = get_dist(distances, particles, N);

            ClearScreen();
            for (int idx=0;idx<N;idx++)  // Update every particle
            {
                DrawCircle(particles[idx].x, particles[idx].y, L, W, circleRadius, circleColor);
                update_particle(particles, temp, distances, idx, N, dt, G, e0);
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
    FILE *file_res = fopen("result.gal", "wb"); // Open file
    if (file_res==NULL)
    {printf("ERROR: Could not create file result.gal\n");}

    for (i = 0; i < N; i++)
    {fwrite(&particles[i], sizeof(particle_t), 1, file_res);}  // Write to the file

    fclose(file_res);  // Close the file

    /*############### Free memory ##############*/
    free(particles);
    free(temp);
    free(distances);

    return 0;
}