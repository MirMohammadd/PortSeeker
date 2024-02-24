#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#ifndef TYPES_H
#define TYPES_H
#ifndef MAX_SERVICE_NAME
#define MAX_SERVICE_NAME 80
#endif

// Define a struct for a linked list node
typedef struct node {
    int val;
    struct node *next;
} node_t;

// Function to print the linked list
void print_linked_list(node_t *head) {
    node_t *current = head;
    while (current != NULL) {
        printf("%d -> ", current->val);
        current = current->next;
    }
    printf("NULL\n");
}

typedef struct ip_address_list{
    char ip[INET_ADDRSTRLEN];
    struct ipAddress_t *next;
}ipAddress_t;


typedef struct port_list{
    int portno;
    struct port_list *next;
}port_t;


typedef struct  job_list
{
    char ip[INET_ADDRSTRLEN];
    int portno;
    int scan_type;
    struct job_list *next;
}job_t;


typedef struct result_list{
    
    char ip[INET_ADDRSTRLEN];
	char service[MAX_SERVICE_NAME];
	char result[6];
	int conclusion;
	int portno;
	int scan_type;
	struct result_list *next;
}result_t;

typedef struct service_version{
	char ip[INET_ADDRSTRLEN];
	char version[6][50];
	struct service_version *next;
}servver_result_t;




#endif

// int main() {
//     // Create the head node
//     node_t *head = NULL;

//     // Add nodes to the linked list
//     head = (node_t*)malloc(sizeof(node_t));
//     head->val = 1;
//     head->next = NULL;

//     node_t *second = (node_t*)malloc(sizeof(node_t));
//     second->val = 2;
//     second->next = NULL;
//     head->next = second;

//     node_t *third = (node_t*)malloc(sizeof(node_t));
//     third->val = 3;
//     third->next = NULL;
//     second->next = third;

//     // Print the linked list
//     printf("Linked List: ");
//     print_linked_list(head);

//     // Free memory
//     free(head);
//     free(second);
//     free(third);

//     return 0;
// }
