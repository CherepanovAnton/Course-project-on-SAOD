#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_PEOPLE 4000
#define M 255
#define L 10

struct person {
    char fullName[32];
    char address[18];
    short int numHome;
    short int numFlat;
    char dateSettlement[10];
};

typedef struct tLE {
    struct person data;
    struct tLE *next;
} tLE;

typedef struct {
    tLE *head;
    tLE *tail;
} tQueue;

void DigitalSort(tLE **head) {
    short int indexData[6] = {1, 0, 4, 3, 7, 6};
    tQueue queue[M + 1];
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i <= M; i++) {
            queue[i].tail = NULL;
            queue[i].head = NULL;
        }
        tLE *p = *head;
        while (p != NULL) {
            tLE *next_node = p->next;
            unsigned char d = p->data.dateSettlement[indexData[j]];
            p->next = NULL;
            if (queue[d].head == NULL) {
                queue[d].head = p;
            } else {
                queue[d].tail->next = p;
            }
            queue[d].tail = p;
            p = next_node;
        }
        tLE node;
        node.next = NULL;
        p = &node;
        for (int i = 0; i <= M; i++) {
            if (queue[i].head != NULL) {
                p->next = queue[i].head;
                p = queue[i].tail;
            }
        }
        p->next = NULL;
        *head = node.next;
    }
}

tLE *CreateList(FILE *f, short int *cnt) {
    tQueue Q;
    Q.head = NULL;
    Q.tail = NULL;
    struct person buf;
    while (fread(&buf, sizeof(struct person), 1, f) == 1) {
        tLE *newElement = malloc(sizeof(tLE));
        if (!newElement) {
            perror("malloc");
            exit(2);
        }
        newElement->data = buf;
        newElement->next = NULL;
        if (Q.head == NULL) {
            Q.head = newElement;
        } else {
            Q.tail->next = newElement;
        }
        Q.tail = newElement;
        (*cnt)++;
    }
    return Q.head;
}

void freeList(tLE *head) {
    tLE *cur;
    while (head != NULL) {
        cur = head;
        head = head->next;
        free(cur);
    }
}

void PrintPerson(tLE *human) {
    printf("FIO: %s\n", human->data.fullName);
    printf("Street: %s\n", human->data.address);
    printf("House: %d\n", human->data.numHome);
    printf("Flat: %d\n", human->data.numFlat);
    printf("Date: %s\n", human->data.dateSettlement);
    printf("---------------------------\n");
}

int main() {
    FILE *f = fopen("testBase4.dat", "rb");
    if (!f) {
        perror("fopen");
        exit(1);
    }

    short int count = 0;
    tLE *head = CreateList(f, &count);
    printf("Count data: %d\n", count);

    char print;
    printf("Go show (y / n)?\n");
    scanf("%c", &print);
    if (print == 'y') {
        tLE *cur = head;
        for (int i = 0; i < 4; i++) {
            printf("Human %d\n", i + 1);
            PrintPerson(cur);
            cur = cur->next;
        }
    }

    DigitalSort(&head);
    tLE *cur = head;
    for (int i = 0; i < 30; i++) {
        printf("Human %d\n", i + 1);
        PrintPerson(cur);
        cur = cur->next;
    }

    freeList(head);
    fclose(f);
}
