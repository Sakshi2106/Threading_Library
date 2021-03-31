#include "thread.h"

/*Insert a new node on the front of the list. */
void add(Node** head, thread_tcb new_tcb)
{
    Node* new_node
        = (Node*)malloc(sizeof(Node));
 
    new_node->tcb = new_tcb;
    new_node->next = (*head);
    new_node->prev = NULL;
 
    if ((*head) != NULL)
        (*head)->prev = new_node;
 
    (*head) = new_node;
}

/*Check doubly linked list is empty*/
int isEmpty(Node *head){
	if(!head)
		return 1;
	return 0;
}


//Delete node containing given tid value and return that node
thread_tcb* removeNodeWithTid(Node** head, int tid)
{
    
    if(*head == NULL){
         
        return NULL;
    }

    if((*head)->next == NULL){
       
        if((*head)->tcb.tid == tid){
            //head = NULL;
            return &(*head)->tcb;
        }
    }

    Node* temp = (*head);
    if((*head) -> tcb.tid == tid){

        (*head)->next->prev = NULL;
        (*head) = (*head) -> next;
        temp -> next = NULL;
        return &temp->tcb;
    }
    
    while(temp -> next != NULL)  {
       
       if(temp->tcb.tid == tid){
           
            temp -> prev -> next = temp -> next;
            temp -> next -> prev = temp -> prev;
            return &temp->tcb;
        }
        temp = temp->next;
    }
    
    if(temp -> next == NULL){
        temp -> prev -> next = temp -> next;
        temp -> prev = temp -> next;
        return &temp->tcb;
    }

}


//Return node containing given tid value
thread_tcb* getNodeUsingTid(Node*head, int tid){

    Node* temp = head;
   
    while (temp->tcb.tid != tid && temp->next != NULL) {
        temp = temp->next;
    }
 
    return &temp->tcb;
}


//Print doubly linked list
void printList(Node* node)
{
    while (node != NULL) {
        printf("%d \n", node->tcb.tid);
        node = node->next;
    }
}
 
 
// int main()
// {
//     /* Start with the empty list */
//     Node* head = NULL;
   
//     /* Let us create the doubly linked list 10<->8<->4<->2 */
//     thread_tcb *tcb = (thread_tcb *)malloc(sizeof(thread_tcb));
//     tcb->tid = 1;
//     thread_tcb *tcb1 = (thread_tcb *)malloc(sizeof(thread_tcb));
//     tcb1->tid = 2;
//     thread_tcb *tcb2 = (thread_tcb *)malloc(sizeof(thread_tcb));
//     tcb2->tid = 3;
//     thread_tcb *tcb3 = (thread_tcb *)malloc(sizeof(thread_tcb));
//     tcb3->tid = 4;
//     add(&head, *tcb);
//     add(&head, *tcb1);
//     add(&head, *tcb2);
//     add(&head, *tcb3);
    
//     //printList(head);
    
//     thread_tcb *req_tcb = getNodeUsingTid(head, 2);
   
 
//     req_tcb = removeNodeWithTid(&head, 1);
//     printf("Require Value:%d\n", req_tcb->tid);
   

//     req_tcb = removeNodeWithTid(&head, 3);
//     printf("Require Value:%d\n", req_tcb->tid);
   
//     req_tcb = removeNodeWithTid(&head, 4);
//     printf("Require Value:%d\n", req_tcb->tid);

//     req_tcb = removeNodeWithTid(&head, 2);
//     printf("Require Value:%d\n", req_tcb->tid);
   
// } 