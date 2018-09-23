#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

void delay(float number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    float milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds); 
} 

int main ()
{
    FILE *fptr;

    if ((fptr = fopen("./froggEEr.txt","r")) == NULL){
        printf("Error! opening file");
        exit(1);
    }

    char c = fgetc(fptr); 

    while (c != EOF) { 
        printf ("%c", c); 
        delay(0.3); 
        c = fgetc(fptr); 
    } 
    
    printf("\n");
    fclose(fptr);
    getchar();

    return 0;
}