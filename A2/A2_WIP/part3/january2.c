#include <stdio.h>
#include <stdlib.h>


struct node {
    int index;
    float min;
    float max;
    struct node *next;
};


struct node *getNode(int index, float min, float max)
{
    struct node *newNode = NULL;
    newNode = malloc(sizeof(struct node));
    
    newNode->index = index;
    newNode->min = min;
    newNode->max = max;
    newNode->next = NULL;

    return newNode;
}


void printList(struct node* head)
{
    struct node *temp = head;  // temp is a pointer to address given by "head"
    
    if (temp==NULL)
    {
        printf("List is empty\n");
    }

    else
    {
       printf("idx    min           max\n");
       while (temp !=NULL)
       {
        printf("%d      %f      %f \n", temp->index, temp->min, temp->max);
        //printf("Idx:%d  ->  ", temp->index);
        temp = temp->next;
       }
    }
}


void freeMemory(struct node *head)
{
    struct node *iter = head;
    struct node *temp;

    while (iter!=NULL)
    {
        temp = iter;
        iter = iter->next;
        free(temp);
    }
}

// use **head if you may need to change the first node

void insertNode(struct node **head, int index, float min, float max) //head is a pointer to the org head pointer
{
    if (*head ==NULL) // if head points to nothing
    {
        // Create node
        struct node *newNode = getNode(index, min, max); 
        
        *head = newNode; // go to head address and place pointer to newNode there
    }

    else  // head is not NULL
    {
        struct node *temp = *head; // temp is a copy of the org head
        
        if (temp->index == index) //if newNode has same index as head
        {
            printf("Error message passed\n");
            temp->min = min;
            temp->max = max;
        }

        else if (index < temp->index) // if newNode has smallar index than head
        {
            // Create node
            struct node *newNode = getNode(index, min, max);

            //Insert before head
            *head = newNode;
            newNode->next = temp;
        }

        else // if newNode has larger index than head
        {
            insertNode(&(temp->next), index, min, max);
        }
    }
}


void deleteNode(struct node **head, int index)
{
    struct node *temp = *head;  // temp is a pointer that is a copy of head
    struct node *previous;

    if (temp->index == index) // if delete the first node
    {
        *head = temp->next;
    }

    else // if delete some nother further down list
    {
        while (temp!=NULL) // iterate through the entire list
        {
            if (temp->index == index) // if index is found
            {   
                if (temp->next == NULL) // if delete last node
                {
                    previous->next = NULL;
                    free(temp);
                }
                
                else  // if delete middle node
                {
                    previous->next = temp->next;  
                    free(temp);
                }
                break; // If node has been found and deleted, stop the search.
            }
            previous = temp;
            temp = temp->next;
        }
    }
}



int main(void)
{
    int loop=1;
    int index;
    char command;
    float min,max;


    struct node *head = NULL;  // head is a struct node pointer, pointing to NULL

    
    while (loop==1)
    {
        printf("\nEnter command: ");
        scanf(" %c", &command);

        // If add element to list
        if (command=='A') 
        {
            scanf(" %d %f %f", &index, &min, &max);
            printf("Add node\n");

            // Insert new node into list
            insertNode(&head, index, min, max);
        }
        
        // If print out entire list
        else if (command == 'P')
        {
            printList(head);  // address of the first node goes into the function
        }

        // If delete a node
        else if (command == 'D')
        {
            scanf(" %d", &index);
            printf("Delete node\n");
            deleteNode(&head, index);
        }

        // Exit loop
        else if (command == 'Q')
        {
            freeMemory(head);
            loop = 0;
        }

        else
        {
            printf("Invalid input\n");
        }
    }
}