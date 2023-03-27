#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} node;

void linked_list_add(node** head, int value) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->data = value;
    new_node->next = *head;
    *head = new_node;
}

void linked_list_print(node** head) {
    node* curr = *head;
    printf("Print linked list\n");
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
}

void linked_list_free(node** head) {
    node* curr = *head;
    while (curr != NULL) {
        node* temp = curr;
        curr = curr->next;
        free(temp);
    }
    *head = NULL;
}

int main() {
    node* head = NULL;

    printf("%lu\n", sizeof(int));

    for (int i = 0; i < 100; i++) {
        linked_list_add(&head, i);
    }

    linked_list_print(&head);

    linked_list_free(&head);

    return 0;
}
