#include <stdio.h>
#include <unistd.h>

int compute_speed(int distance) {
    if(distance < 20) {
        return 0;
    }
    else{
        return 50;
    }

}

int main(){
    FILE *fp;
    int distance;

    while(1){
        fp = fopen("comm.txt", "r");
        
        if(fp != NULL) {
            if (fscanf(fp, "%d", &distance) == 1){
                int speed = compute_speed(distance);
                printf("[MOTOR] Distance: %d -> Speed: %d\n", distance, speed);
            }
            else {
                printf("[MOTOR] Failed to read valid data\n");
            }
            fclose(fp);          
        }
    
        sleep(1);
    
    }
    
    return 0;

}