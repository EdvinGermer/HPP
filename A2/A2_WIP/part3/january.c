#include <stdio.h>
#include <stdlib.h>


struct node {
    int index;
    float min;
    float max;
    struct node *next;
};


struct node *addNode(int index, float min, float max)
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
    struct node *temp = head;
    
    if (head==NULL)
    {
        printf("List is empty\n");
    }

    else
    {
       printf("idx     min           max\n");
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
    struct node *temp = NULL;

    while (iter!=NULL)
    {
        temp = iter;
        iter = iter->next;
        free(temp);
    }
}



int main(void)
{
    int loop=1;
    int index;
    char command;
    float min,max;


    struct node *head = NULL;  // head is a struct node pointer, pointing to NULL
    struct node *iter = NULL;  // iter is a struct node pointer, pointing to NULL

    
    while (loop==1)
    {
        printf("\nEnter command: ");
        scanf(" %c", &command);


        // If add element to list
        if (command=='A') 
        {
            scanf(" %d %f %f", &index, &min, &max);

            if (head==NULL)  // If linked list is empty
            {
                head = addNode(index, min, max);
                iter = head;
            }
            
            else   // Add new node
            {
                iter = head;  //reset iter
                

                if (index > iter->index) // if new node has larger index
                    {
                        struct node *previous = iter;

                        while ((index > iter->index) && iter->next != NULL) // Iterate until new index < than current node index
                        {
                            previous = iter;
                            iter = iter->next;
                        }

                        if (index == iter->index) // if index already exist
                        {
                            iter->min = min;
                            iter->max = max;
                        }

                        else if (index < iter->index) // add new node before current node
                        {
                            previous->next = addNode(index, min, max);
                            previous->next->next = iter;
                        }
                        
                        else // if index is the largest
                        {
                            iter->next = addNode(index, min, max);
                        }
                        
                    }
                
                else if (index < iter->index) // if new node has smaller index
                {
                    struct node *temp = head; // copy of old head

                    head = addNode(index,min,max); // set new head
                    head->next = temp;  // add old head after new head
                }
                
                else  // if new node has same index as the first node
                {
                    head->min = min;
                    head->max = max;
                }
            }
        }
        
        
        // If print out entire list
        else if (command == 'P')
        {
            printList(head);
        }


        // If delete a node
        else if (command == 'D')
        {
            scanf(" %d", &index);
            struct node *iter = head;
            struct node *previous = NULL;
            
            if (head == NULL)
            {
                // Do nothing
            }

            else if (head->index == index) // If delete head
            {
                previous = head;
                head = head->next;
                free(previous);
            }

            else
            {
                while (iter != NULL)
                {
                    if (iter->index ==index)
                    {
                        if (previous == NULL) // If remove first element in list
                        {
                            head = head->next;
                        }
                        else
                        {
                            previous->next = iter->next;
                            free(iter);
                        }
                        
                    }

                    else 
                    {
                        previous = iter;
                        iter = iter->next;
                    }
                }

            }
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