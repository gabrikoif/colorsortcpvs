#pragma once

typedef struct Node
{
    int data;
    struct Node* next;
} Node;

Node* create_list(int size);
void free_list(Node* head);
void printlist(Node* head);
void push(Node** head, int val);
int pull(Node** head, int* top);
int list_size(Node* head);
int is_empty(Node* head, int val);