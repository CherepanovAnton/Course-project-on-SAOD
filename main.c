#include <stdio.h>
#include <stdlib.h>

#define COUNT_PEOPLE 4000

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

tLE *CreateList(FILE *f, short int *cnt) {
    tLE *head = NULL;
    tLE *tail = head;
    struct person buf;
    while (fread(&buf, sizeof(struct person), 1, f) == 1) {
        tLE *newElement = malloc(sizeof(tLE));
        if (!newElement) {
            perror("malloc");
            exit(2);
        }
        newElement->data = buf;
        newElement->next = NULL;

        if (head == NULL) {
            head = newElement;
            tail = newElement;
        } else {
            tail->next = newElement;
            tail = newElement;
        }
        (*cnt)++;
    }
    return head;
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

    freeList(head);
    fclose(f);
}
