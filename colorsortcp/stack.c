#include "platform_utils.h"
#include "stack.h"

Node* create_list(int size)
{
    Node* head = NULL;
    for (int i = 0; i < size; i++)
    {
        Node* new_node = malloc(sizeof(Node));
        new_node->data = COLOR_RED + i;
        new_node->next = head;
        head = new_node;
    }
    return head;
}

void free_list(Node* head)
{
    while (head != NULL)
    {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

void printlist(Node* head)
{
    int i = 0;
    while (head != NULL)
    {
        printf("current: %d, index on size: %d\n", head->data, i);
        head = head->next;
        i++;
    }
}

void push(Node** head, int val)
{
    Node* new = malloc(sizeof(Node));
    new->data = val;
    new->next = *head;
    *head = new;
}

int pull(Node** head, int* top)
{
    if (*head == NULL)
    {
        return 1; // No more data to pull.
    }
    else
    {
        int res = (*head)->data;
        *head = (*head)->next;
        *top = res;
        return 0;
    }
}

int list_size(Node* head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

int in_stack(Node* head, int val) {
    while (head != NULL) {
        if (head->data == val) {
            return 1; // Found the value in the stack
        }
        head = head->next;
    }
    return 0; // Value not found in the stack
}