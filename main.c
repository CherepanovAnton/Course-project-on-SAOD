#include <stdio.h>
#include <stdlib.h>

struct person {
    char *fullName;
    char *address;
    int numHome;
    int numFlat;
    char* dateSettlement;
};

int main() {
    FILE *f = fopen("testBase4.dat", "rb");
    if (!f) { 
        perror("fopen"); 
        exit(1); 
    }


    int cnt = 0;
    char buf[20];
    while (fread(&buf, sizeof(buf), 1, f) == 1) {
        printf("%s\n", buf);
        if (cnt == 1) {
            break;
        }
        cnt++;
    }
    fclose(f);
}