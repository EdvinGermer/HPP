#include <stdio.h>
#include <stdlib.h>

struct sPerson {
    int age;
    struct sPerson *nextInLine;
};

void printPerson(const struct sPerson *person, const char *comment)
{
    if (person == NULL)
    {
        printf("%s is NULL\n", comment);
    }
    else 
    {
        printf("%s: age:%d adress:%p nextInLine adress:%p\n",
                comment,
                person->age,
                person,
                person->nextInLine);
    }
}



struct sPerson *get_new_person(const int age)   // return a pointer to a "struct sPerson"
{
    struct sPerson *newperson = NULL; // create a pointer to "struct sPerson" and set the adress to NULL
    newperson = malloc(sizeof(struct sPerson));  // adress = place in memory of size "struct sPerson"
    
    if (newperson != NULL)
    {
        newperson->age = age; // At address newperson, set age = age
        newperson->nextInLine = NULL;
        printf("    created new person at: %p \n", newperson);  // print the adress to where in memory the data is stored
    }
    else
    {
        printf("Allocation failure\n");
    }

    return newperson;
}




int main()
{
    printf("\n\n ** START ** \n\n");
    
    // Create 2 pointers towards sPerson and set adress to NULL
    struct sPerson *first = NULL;
    struct sPerson *added = NULL;



    printf("\n\n");
    return 0;
}