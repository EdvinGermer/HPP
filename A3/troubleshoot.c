#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

// gcc -o troubleshoot troubleshoot.c -lm

/*############### Define a structs ##############*/
typedef struct particle{
    double x;
    double y;
    double m;
    double vx;
    double vy;
    double brightness;
} particle_t;

typedef struct temp{
    double x;
    double y;
} temp_t;


/*############### Define update function ##############*/
void update_particle(particle_t* particles, temp_t* temp, int i, double N, double dt, double G, double e0)
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
    // CHANGE THIS TO TEST DIFFERENT CONFIGURATION
    int idx = 1;   //Which particle to check
    int N = 500;
    int nsteps = 200;
    char* input_dir= "/home/edge9521/HPP/A3/input_data/ellipse_N_00500.gal";
    char* correct_dir= "/home/edge9521/HPP/A3/ref_output_data/ellipse_N_00500_after200steps.gal";

    /*############### Inital Setup ##############*/
    int i;
    double G = 100.0/N;    // Gravity 
    double e0 = 0.001;   // Gravity correctional term
    double dt = 0.00001; // Time step

    /*############### Allocate memory for particles ##############*/
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

    /*############### Read and print correct final data ##############*/
    particle_t *correct = malloc(N * sizeof(particle_t));

    FILE *correct_file = fopen(correct_dir, "rb");
    if (correct_file == NULL)
    {printf("ERROR: Could not open file \n");}

    for (i=0;i<N;i++)
    {fread(&correct[i], sizeof(particle_t), 1, correct_file);}
    fclose(correct_file);
    
    /*############### Run simulation ##############*/
    temp_t *temp = malloc(N * sizeof(temp_t));  // temporary array for storing results
    
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


    /*############### Find worse performing datapoint ##############*/
    int worse_idx=0;
    double temp2;
    double max=0;
    for (int j=0;j<N;j++)  
    {
        temp2 = sqrt((particles[j].x-correct[j].x)*(particles[j].x-correct[j].x)) + (particles[j].y-correct[j].y)*(particles[j].y-correct[j].y);
        if (temp2>max)
        {
            max = temp2;
            worse_idx = j;
        }
    }
    printf("pos_maxdiff = %16.16f\n",max);


    /*############### Create troubleshoot output file ##############*/
    // Open file
    FILE *file_res = fopen("test_result.gal", "wb");
    if (file_res==NULL)
    {printf("ERROR: Could not create file test_result.gal\n");}

    // Write to the file
    for (i = 0; i < N; i++)
    {fwrite(&particles[i], sizeof(particle_t), 1, file_res);}

    // Close the file
    fclose(file_res);  


    /*############### Load the troubleshoot output file ##############*/
     particle_t *result = malloc(N * sizeof(particle_t));

    FILE *result_file = fopen("./test_result.gal", "rb");
    if (result_file == NULL)
    {printf("ERROR: Could not open file \n");}

    for (i=0;i<N;i++)
    {fread(&result[i], sizeof(particle_t), 1, result_file);}
    fclose(result_file);




    /*############### Print difference of worse index ##############*/
    printf("\nDifference in output\n");
    printf("    Particle: %d\n", worse_idx);
    printf("         d_x          = %0.15f\n", result[worse_idx].x-correct[worse_idx].x);
    printf("         d_y          = %0.15f\n", result[worse_idx].y-correct[worse_idx].y);
    printf("         d_m          = %0.15f\n", result[worse_idx].m-correct[worse_idx].m);
    printf("         d_vx         = %0.15f\n", result[worse_idx].vx-correct[worse_idx].vx);
    printf("         d_vy         = %0.15f\n", result[worse_idx].vy-correct[worse_idx].vy);
    printf("         d_brigthness = %0.15f\n", result[worse_idx].brightness-correct[worse_idx].brightness);


    /*############### Free memory ##############*/
    free(particles);
    free(correct);
    free(temp);
    free(result);

    return 0;
}