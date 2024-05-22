#include <stdlib.h>
#include <stdio.h>

int main() {
	FILE* fp;
    char output[1024];

    fp = popen("sudo ./pfcount -i zc:ens2f1", "r");
    if (fp == NULL) {
        printf("Error executing command\n");
        return 1;
    }

    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
    }

    pclose(fp);
	return 0;
}