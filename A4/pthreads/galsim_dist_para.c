#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 16


// time ./galsim 10 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_00010.gal 200 0.00001 0
// time ./galsim 100 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_00100.gal 200 0.00001 0
// time ./galsim 500 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_00500.gal 200 0.00001 0
// time ./galsim 1000 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_01000.gal 200 0.00001 0

// time ./galsim 3000 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_03000.gal 100 0.00001 0


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
} temp_pos_t; 

typedef struct{
    particle_t* particles;
    double** distances;
    int thread; // which thread index
    double N; // Matrix size
} dist_args_t;

/*############### Graphics settings ##############*/
const float circleRadius=0.005, circleColor=0;
const int windowWidth=800;
const int L=1, W=1;


/*############### Define distance function ##############*/
void* get_dist(void* input)
{
    //Unpack
    dist_args_t* dist_args = (dist_args_t*) input;

    particle_t* particles = dist_args->particles;
    double** distances = dist_args->distances;
    int t = dist_args->thread;
    double N = dist_args->N;

    // Calculate
    for(int i=t; i<N-1; i+=NUM_THREADS)
    {
        for(int j=i+1; j<N; j++)
        {
            distances[i][j] = sqrt((particles[i].x-particles[j].x)*(particles[i].x-particles[j].x) + (particles[i].y-particles[j].y)*(particles[i].y-particles[j].y));
            distances[j][i] = distances[i][j];
        }
    }
} 


/*############### Define update function for each particle ##############*/
void update_particle(particle_t* restrict particles, temp_pos_t* restrict temp_pos, double** restrict distances, int i, const double N, const double dt, const double G, const double e0)
{
    double Fx=0.0;
    double Fy=0.0;
    double r3, r_x, r_y;

    // Calculate force
    for (int j=0; j<N; j++)   // Iterate over all particles
    {
        if (j!=i)             // Do not calculate for the same particle
        {
            r_x = particles[i].x - particles[j].x; 
            r_y = particles[i].y - particles[j].y; 
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
    temp_pos[i].x = particles[i].x + dt*particles[i].vx;
    temp_pos[i].y = particles[i].y + dt*particles[i].vy;
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
    int i,t;
    const double G = 100.0/N;  // Gravity      
    const double e0 = 0.001; // Gravity correctional term

    /*############### Allocate memory ##############*/
    particle_t *particles = malloc(N * sizeof(particle_t));
    temp_pos_t *temp_pos = malloc(N * sizeof(temp_pos_t));  // temporary array for storing results

    double** distances = (double**)malloc(N * sizeof(double*));
    for (int i=0;i<N; i++)
        distances[i] = (double*)malloc(N * sizeof(double));

    dist_args_t dist_args[NUM_THREADS];  // Array of arguments for each thread
    pthread_t threads[NUM_THREADS]; // Create threads

    /*############### Read data ##############*/
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", filename);}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);

    /*############### Prepare arguments ##############*/

    for(t=0;t<NUM_THREADS;t++)
    {
        dist_args[t].particles = particles;
        dist_args[t].distances = distances;
        dist_args[t].N = N; 
    }
    
    /*############### Run Simulation ##############*/

    if (graphics == 0)
    {
        for (int time=0; time<nsteps; time++)         // For every time step
        {
            // GET DISTANCES
            for(t=0;t<NUM_THREADS;t++) // Deploy workers
            {
                dist_args[t].thread = t; 
                pthread_create(&threads[t], NULL, get_dist, &dist_args[t]);
            }

            // Wait for all threads to finish
            for(t=0; t<NUM_THREADS; t++) 
                pthread_join(threads[t], NULL);
    
 
            // UPDATE EVERY PARTICLE
            for (int j=0;j<N;j++)  
                update_particle(particles, temp_pos, distances, j, N, dt, G, e0);

            // Copy over result from temp_pos to particles
            for (int j=0;j<N;j++)  // Update every particle
            {
                particles[j].x = temp_pos[j].x;
                particles[j].y = temp_pos[j].y;
            }
        }
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
    free(temp_pos);

    for (int i = 0; i < N; i++) {
    free(distances[i]);
    }
    free(distances);
    return 0;
}