#include <stdio.h>
#include <stdlib.h>

struct NODE
{
    struct NODE* llink;
    int data;
    struct NODE* rlink;
};
struct NODE* head;
struct NODE* tail;

struct NODE* makenode(int value) {
    struct NODE *node = (struct NODE*)malloc(sizeof(struct NODE));
    node->llink = NULL;
    node->data = value;
    node->rlink = NULL;
    return node;
}

void init() {
    head = (struct NODE*)malloc(sizeof(struct NODE));
    tail = (struct NODE*)malloc(sizeof(struct NODE));
    head->data = 0;
    tail->data = 0;

    head->rlink = tail;
    head->llink = head;
    tail->rlink = tail;
    tail->llink = head;
}

void push_back(int value) { 
    struct NODE* newnode = makenode(value);
    struct NODE* p;
    p = tail;
    p->llink->rlink = newnode;
    newnode->llink = p->llink;
    p->llink = newnode;
    newnode->rlink = p;
}

void removenode(int value){
    struct NODE* p;
    p = head->rlink;
    while(p->rlink != tail){
        if(p->data == value){
            p->rlink->llink = p->llink;
            p->llink->rlink = p->rlink;
            free(p);
            return;
        }
        p = p->rlink;
    }
}

void print() {
    struct NODE *p;
    p = head;
    while(p->rlink != tail){
        printf("%d\n",p->data);
        p = p->rlink;
    }
    printf("%d",p->data);
}

int main() {
    init();
    printf("remove before\n\n");
    push_back(10);
    push_back(20);
    push_back(30);
    print();
    printf("\n\nremove after\n\n");
    removenode(20);
    print();
    return 0;
}