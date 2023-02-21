#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>
#include <pthread.h>

// time ./galsim 3000 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_03000.gal 100 0.00001 0

#define NUM_THREADS	8   // No improvement over 8 threads
pthread_mutex_t m;

/*############### Define a struct for the particles ##############*/
typedef struct{
    double x;
    double y; 
    double m;
    double vx;
    double vy;
    double brightness;
} particle_t;

typedef struct{
    double x;
    double y;
} temp_pos_t;

typedef struct{
    particle_t* particles;
    temp_pos_t* temp;
    double** distances;
    int start,end;
    double N,dt,G,e0; 
}update_args_t;

/*############### Graphics settings ##############*/
const float circleRadius=0.005, circleColor=0;
const int windowWidth=800;
const int L=1, W=1;

/*############### Define get distance function ##############*/
double** get_dist(double** restrict distances, particle_t* restrict particles, const double N)
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
void* update_particle(void* input)
{
    // Unpack arguments
    update_args_t* args = (update_args_t*) input;

    particle_t* particles = args->particles;
    temp_pos_t* temp = args->temp;
    double** distances = args->distances;
    int start = args->start;
    int end = args->end;
    double N = args->N;
    double dt= args->dt;
    double G = args->G;
    double e0 = args->e0;

    for(int i=start;i<end;i++) // For all particles in segment
    {
        // Set inital forces
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
        pthread_mutex_lock(&m);
        temp[i].x = particles[i].x + dt*particles[i].vx;
        temp[i].y = particles[i].y + dt*particles[i].vy;
        pthread_mutex_unlock(&m);
    }

    return NULL;
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
    particle_t* particles = malloc(N * sizeof(particle_t));
    temp_pos_t* temp = malloc(N * sizeof(temp_pos_t));  // temporary array for storing results

    double** distances = (double**)malloc(N * sizeof(double*));
    for (int i=0;i<N; i++)
        distances[i] = (double*)malloc(N * sizeof(double));


    pthread_t threads[NUM_THREADS]; // Create threads
    update_args_t* args = malloc(NUM_THREADS * sizeof(update_args_t));; // Allocate memory for argument structs


    /*############### Read data ##############*/
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", filename);}

    for (i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);

    /*############### Prepare Arguments ##############*/
    for(int t=0; t<NUM_THREADS; t++)    
        {
            args[t].particles = particles;
            args[t].distances = distances;
            args[t].temp = temp;
            args[t].G = G; 
            args[t].N = N;
            args[t].e0 = e0;
            args[t].dt = dt;
        }

    /*############### Run Simulation ##############*/
    pthread_mutex_init(&m, NULL);
    
    if (graphics == 0)
    {
        for (i=0; i<nsteps; i++)         // For every time step
        {
            // get all distances
            distances = get_dist(distances, particles, N);

            // Update every particle in parallel
            for(int t=0; t<NUM_THREADS; t++)    
            {
                int start = t*(N/NUM_THREADS);
                int end = (t+1)*(N/NUM_THREADS);
                if (t == NUM_THREADS - 1)
                    end = N;

                args[t].start = start;
                args[t].end = end;

                pthread_create(&threads[t], NULL, update_particle, &args[t]);
            }

            // Wait for all threads to finish
            for(t=0; t<NUM_THREADS; t++) 
			    pthread_join(threads[t], NULL);
            
            // Copy over result from temp to particles
            for (int j=0;j<N;j++)  // Update every particle
            {
                particles[j].x = temp[j].x;
                particles[j].y = temp[j].y;
            }
        }
    }

    else
    {printf("ERROR: Invalid input for graphics '%d'\n", graphics);}

    pthread_mutex_destroy(&m);
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
    free(args);

    return 0;
}