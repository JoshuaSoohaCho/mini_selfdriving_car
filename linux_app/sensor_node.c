#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    srand(time(NULL));

    while(1){
        int distance = rand() % 100;
        FILE *fp = fopen("comm_tmp.txt", "w");
        if (fp == NULL) {
            perror("File open failed");
            continue;
        }
        fprintf(fp, "%d\n", distance);
        fclose(fp);

        rename("comp_tmp.txt", "comm.txt");
        
        printf("[SENSOR] Sent Distance: %d\n", distance);

        sleep(1);

    }
    
    return 0;
    
}