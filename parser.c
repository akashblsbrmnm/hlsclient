#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *file_path;
    int count=0;
    FILE *file;
    char line[1024];
    char link[100][100];
    file_path = argv[1];
    file_path="master.m3u8";

    file = fopen("master.m3u8", "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", file_path);
        return 1;
    }
    int i=0,k=0;
    while (fgets(line, sizeof(line), file)) {
        if(strstr(line, "#EXT-X-STREAM-INF")){
            //printf("Found a media stream with the following attributes:\n");
            //printf("%s", line);
            
        }
        if(strstr(line, ".m3u8")){
            //printf("Found a media segment at: %s\n",line);
            while(line[i]!='\0') {
                link[k][i] = line[i];
                i++;
            }
            count++;
            k++;
            i=0;
        }
    }
    for(int i=0;i<k;i++)
    {
        for(int j=0;j<100;j++)
        {
            printf("%c",link[i][j]);
        }
        printf("\n");
    }
    fclose(file);
    printf("%d count  ",count);
    return 0;
}
