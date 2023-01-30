#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function

int main() 
{
    for (int i = 0; i < 6; i++) // loop for 6 iterations
    {
        char command[100];
        sprintf(command, "speedtest --bytes --simple"); //to copy the text to command variable
        char result[1000];
        FILE* fp = popen(command, "r"); //running the command stored in the command variable
        if (fp == NULL) 
        {
            printf("Failed to run command\n");
            exit(1);  //closes the program
        }
        while (fgets(result, sizeof(result)-1, fp) != NULL) 
        {
            printf("%s", result);
        }  
             //copy the output of the command to result variable and prints it 
        pclose(fp);
        sleep(10); // sleep for 10 seconds
    }
    return 0;
}
