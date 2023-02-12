#include <stdio.h>
#include <stdlib.h>


int main()
{
    FILE *my_file;  // Assign FILE pointer called my_file

    int integer;
    double floating_point_double;
    char character;
    float floating_point;

    my_file = fopen("little_bin_file", "rb");  // Open the file as my_file

    if (my_file == NULL)
    {
        printf("The file could not be located");
    }

    // Read all lines and assign to variables
    fread(&integer, sizeof(integer), 1, my_file);
    fread(&floating_point_double, sizeof(floating_point_double), 1, my_file);
    fread(&character, sizeof(character), 1, my_file);
    fread(&floating_point, sizeof(floating_point), 1, my_file);

    // Print all data
    printf("%d\n", integer);
    printf("%lf\n", floating_point_double);
    printf("%c\n", character);
    printf("%f\n", floating_point);

    fclose(my_file);   // Close the file 

    return 0;
}