#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <unistd.h>
#include <pthread.h>


// ./galsim 10 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_00010.gal 2000 0.00001 1 8
// ./galsim 100 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_00100.gal 2000 0.00001 1 8
// ./galsim 500 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_00500.gal 2000 0.00001 1 8
// ./galsim 3000 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_03000.gal 1000 0.00001 1 8

// time ./galsim 3000 /home/edge9521/HPP/A4/pthreads/input_data/ellipse_N_03000.gal 100 0.00001 0 8


/*############### Define global variables ##############*/
int NUM_THREADS;
pthread_mutex_t m;

int N;                  
char* filename;   
double nsteps;  
double dt;      
int graphics;          
double G; 
double e0;

/*############### Define structs ##############*/
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
} temp_pos_pos_t;

typedef struct{
    particle_t* particles;
    temp_pos_pos_t* temp_pos;
    double** distances;
    int start,end;
}update_args_t;

typedef struct{
    particle_t* particles;
    double** distances;
    int thread; // which thread index
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

    // Calculate
    for(int i=t; i<N-1; i+=NUM_THREADS)
        for(int j=i+1; j<N; j++)
        {
            distances[i][j] = sqrt((particles[i].x-particles[j].x)*(particles[i].x-particles[j].x) + (particles[i].y-particles[j].y)*(particles[i].y-particles[j].y));
            distances[j][i] = distances[i][j];
        }
    return NULL;
} 


/*############### Define update function for each particle ##############*/
void* update_particle(void* input)
{
    // Unpack arguments
    update_args_t* update_args = (update_args_t*) input;
    
    particle_t* particles = update_args->particles;
    temp_pos_pos_t* temp_pos = update_args->temp_pos;
    double** distances = update_args->distances;
    int start = update_args->start;
    int end = update_args->end;

    for(int i=start;i<end;i++) // For all particles in segment
    {
        // Set inital forces
        double Fx=0.0;
        double Fy=0.0;
        double r3, r_x, r_y;

        // Calculate force
        for (int j=0; j<N; j++)   // Iterate over all particles
            if (j!=i)             // Do not calculate for the same particle
            {
                r_x = particles[i].x - particles[j].x; 
                r_y = particles[i].y - particles[j].y; 
                r3 = (distances[i][j]+e0)*(distances[i][j]+e0)*(distances[i][j]+e0);  

                // Sum up all contributions in x and y directions
                Fx += (particles[j].m/r3) * r_x;
                Fy += (particles[j].m/r3) * r_y;
            }

        // Update velocity
        particles[i].vx += dt*Fx*-G;
        particles[i].vy += dt*Fy*-G;

        // Update position
        pthread_mutex_lock(&m);
        temp_pos[i].x = particles[i].x + dt*particles[i].vx;
        temp_pos[i].y = particles[i].y + dt*particles[i].vy;
        pthread_mutex_unlock(&m);
    }
    return NULL;
};

int main(int argc, char *argv[])
{
    /*############### Read arguments ##############*/
    if (argc != 7)
    {printf("Error: This function takes 6 arguments, but got %d\n", argc - 1);}
    
    N = atoi(argv[1]);                   // Number of stars
    filename = argv[2];                  // filename
    nsteps = strtod(argv[3], NULL);      // How many steps to simulate
    dt = strtod(argv[4], NULL);          // Timestep
    graphics = atoi(argv[5]);            // Graphics on or off
    NUM_THREADS = atoi(argv[6]);         // How many threads to use
    G = 100.0/N;                         // Gravity      
    e0 = 0.001;                          // Gravity correctional term

    /*############### Allocate memory ##############*/
    particle_t* particles = malloc(N * sizeof(particle_t));
    temp_pos_pos_t* temp_pos = malloc(N * sizeof(temp_pos_pos_t));  // temp_posorary array for storing results

    double** distances = (double**)malloc(N * sizeof(double*));
    for (int i=0;i<N; i++)
        distances[i] = (double*)malloc(N * sizeof(double));

    pthread_t threads[NUM_THREADS]; // Create threads

    update_args_t update_args[NUM_THREADS]; // Array of arguments for update function
    dist_args_t dist_args[NUM_THREADS];    // Array of arguments for distance function


    /*############### Read data ##############*/
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {printf("ERROR: Could not open file %s\n", filename);}

    for (int i=0;i<N;i++)
    {fread(&particles[i], sizeof(particle_t), 1, file);}
    fclose(file);

    /*############### Prepare Arguments ##############*/
    for(int t=0; t<NUM_THREADS; t++)    
        {
            update_args[t].particles = particles;
            update_args[t].distances = distances;
            update_args[t].temp_pos = temp_pos;

            dist_args[t].particles = particles;
            dist_args[t].distances = distances;
        }

    /*############### Run Simulation ##############*/
    pthread_mutex_init(&m, NULL);
    
    if (graphics == 0)
    {
        for (int i=0; i<nsteps; i++)         // For every time step
        {
            
            // GET DISTANCES
            for(int t=0;t<NUM_THREADS;t++) // Deploy workers
            {
                dist_args[t].thread = t; 
                pthread_create(&threads[t], NULL, get_dist, &dist_args[t]);
            }

            // Wait for all threads to finish
            for(int t=0; t<NUM_THREADS; t++) 
                pthread_join(threads[t], NULL);


            // GET PARTICLE UPDATES
            for(int t=0; t<NUM_THREADS; t++)    
            {
                int start = t*(N/NUM_THREADS);
                int end = (t+1)*(N/NUM_THREADS);
                if (t == NUM_THREADS - 1)
                    end = N;

                update_args[t].start = start;
                update_args[t].end = end;

                pthread_create(&threads[t], NULL, update_particle, &update_args[t]);
            }

            // Wait for all threads to finish
            for(int t=0; t<NUM_THREADS; t++) 
			    pthread_join(threads[t], NULL);

            // SAVE RESULT FROM THIS TIMESTEP AND PLOT
            for (int j=0;j<N;j++) 
            {                
                particles[j].x = temp_pos[j].x;
                particles[j].y = temp_pos[j].y;
            }
        }
    }

    else if (graphics == 1) 
    {
        InitializeGraphics(argv[0],windowWidth,windowWidth);
        SetCAxes(0,1);
        for (int i=0; i<nsteps; i++)         // For every time step
        {
            // GET DISTANCES
            for(int t=0;t<NUM_THREADS;t++) // Deploy workers
            {
                dist_args[t].thread = t; 
                pthread_create(&threads[t], NULL, get_dist, &dist_args[t]);
            }

            // Wait for all threads to finish
            for(int t=0; t<NUM_THREADS; t++) 
                pthread_join(threads[t], NULL);

            // GET PARTICLE UPDATES
            for(int t=0; t<NUM_THREADS; t++)    
            {
                int start = t*(N/NUM_THREADS);
                int end = (t+1)*(N/NUM_THREADS);
                if (t == NUM_THREADS - 1)
                    end = N;

                update_args[t].start = start;
                update_args[t].end = end;

                pthread_create(&threads[t], NULL, update_particle, &update_args[t]);
            }

            // Wait for all threads to finish
            for(int t=0; t<NUM_THREADS; t++) 
			    pthread_join(threads[t], NULL);

            // SAVE RESULT FROM THIS TIMESTEP AND PLOT
            ClearScreen();
            for (int j=0;j<N;j++)
            {
                DrawCircle(particles[j].x, particles[j].y, L, W, circleRadius, circleColor);
                particles[j].x = temp_pos[j].x;
                particles[j].y = temp_pos[j].y;
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

    pthread_mutex_destroy(&m);
    /*############### Create output file ##############*/
    FILE *file_res = fopen("result.gal", "wb"); // Open file
    if (file_res==NULL)
    {printf("ERROR: Could not create file result.gal\n");}

    for (int i = 0; i < N; i++)
    {fwrite(&particles[i], sizeof(particle_t), 1, file_res);}  // Write to the file

    fclose(file_res);  // Close the file

    /*############### Free memory ##############*/
    free(particles);
    free(temp_pos);
    
    
    for (int i = 0; i < N; i++)
        free(distances[i]);
    free(distances);

    return 0;
}