#include <stdio.h>
#include <stdlib.h>

#define COUNT_PEOPLE 4000
#define N 20

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
    tLE **tail;
} Queue;

tLE *CreateList(FILE *f, short int *cnt) {
    tLE *head = NULL;
    tLE **tail = &head;
    struct person buf;
    while (fread(&buf, sizeof(struct person), 1, f) == 1) {
        tLE *newElement = malloc(sizeof(tLE));
        if (!newElement) {
            perror("malloc");
            exit(2);
        }
        newElement->data = buf;
        newElement->next = NULL;

        *tail = newElement;
        tail = &newElement->next;
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
    printf("%-32.32s\t%-18.18s\t%d\t%d\t\t%.10s\n",
        human->data.fullName, human->data.address, human->data.numHome, human->data.numFlat, human->data.dateSettlement);
}

void PrintAllList(tLE *head) {
    tLE *cur = head;
    int index = 1;
    while (cur != NULL) {
        printf("%d. ", index++);
        PrintPerson(cur);
        cur = cur->next;
    }
}

tLE *SortByByte(tLE *head, int bytePos, int isAddress) {
    Queue Q[256];
    for (int i = 0; i < 256; i++) {
        Q[i].head = NULL;
        Q[i].tail = &Q[i].head;
    }

    tLE *p = head;
    while (p != NULL) {
        tLE *nextElement = p->next;
        p->next = NULL;

        unsigned char key;
        if (isAddress) {
            key = (unsigned char)p->data.address[bytePos];
        } else {
            key = (unsigned char)p->data.dateSettlement[bytePos];
        }

        *(Q[key].tail) = p;
        Q[key].tail = &p->next;
        p = nextElement;
    }

    head = NULL;
    tLE **tail = &head;
    for (int i = 0; i < 256; i++) {
        if (Q[i].head == NULL) continue;
        *tail = Q[i].head;
        tail = Q[i].tail;
    }
    return head;
}

tLE *DigitalSort(tLE *head) {
    int indexDate[] = {1, 0, 4, 3, 7, 6};
    for (int i = 17; i >= 0; i--) {
        head = SortByByte(head, i, 1);
    }
    for (int i = 0; i < 6; i++) {
        head = SortByByte(head, indexDate[i], 0);
    }

    return head;
}

void ControlPrintList(tLE *head) {
    char choice;
    tLE *cur = head;
    int globalIndex = 1;

    while (1) {
        printf("\nВывести всё = Q\nВывести 20 элементов = W\nВыход = E\n");
        printf("Выберите действие: ");
        scanf(" %c", &choice);

        if (choice == 'e' || choice == 'E') {
            break;
        }

        if (choice == 'q' || choice == 'Q') {
            PrintAllList(head);
        }

        if (choice == 'w' || choice == 'W') {
            if (cur == NULL) {
                printf("[The End]\n");
                continue;
            }

            printf("\n%-6s %-32s\t%-18s\t%s\t%s\t\t%s\n", "№", "ФИО", "Улица", "Дом", "Кв.", "Дата");
            printf("------------------------------------------------------------------------------------------------\n");

            for (int i = 0; i < N && cur != NULL; i++) {
                printf("%-5d ", globalIndex++);
                PrintPerson(cur);
                cur = cur->next;
            }
        }
    }
}

int main() {
    system("chcp 866 > nul");
    FILE *f = fopen("testBase4.dat", "rb");
    if (!f) {
        perror("fopen");
        exit(1);
    }

    short int count = 0;
    tLE *head = CreateList(f, &count);
    fclose(f);

    printf("Кол-во элементов: %d\n", count);

    char print;
    printf("Показать неотсортированную базу данных (y / n)?\n");
    scanf(" %c", &print);
    if (print == 'y' || print == 'Y') {
        ControlPrintList(head);
    }

    head = DigitalSort(head);

    printf("\nПоказать сортированную базу данных (y / n)?\n");
    scanf(" %c", &print);
    if (print == 'y' || print == 'Y') {
        ControlPrintList(head);
    }

    freeList(head);

}
