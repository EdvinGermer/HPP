#include <stdio.h>
#include <stdlib.h>


struct node {
    int index;
    struct node *next;
};


struct node *addNode(int index)
{
    struct node *newNode = NULL;
    newNode = malloc(sizeof(struct node));
    
    newNode->index = index;
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
       while (temp !=NULL)
       {
        printf("%d->", temp->index);
        temp = temp->next;
       }
    }
}



int main(void)
{
    int loop=1;
    int index;
    char command;


    struct node *head = NULL;  // head is a struct node pointer, pointing to NULL
    struct node *iter = NULL;  // iter is a struct node pointer, pointing to NULL

    
    while (loop==1)
    {
        printf("\nEnter command: ");
        scanf(" %c", &command);


        // If add element to list
        if (command=='A') 
        {
            scanf(" %d", &index);

            if (head==NULL)  // If linked list is empty
            {
                head = addNode(index);
                iter = head;
            }
            
            else            // Add to end of list
            {
                iter->next = addNode(index);
                iter = iter->next;
            }
        }
        
        
        // If print out entire list
        else if (command == 'P')
        {
            printList(head);
        }

        // Exit loop
        else
        {
            loop = 0;
        }

    }

}