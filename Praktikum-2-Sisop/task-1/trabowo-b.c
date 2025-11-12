#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define UNZIP_DIR "/home/youruser/Downloads/film_unzip"

int main() {
    DIR *d;
    struct dirent *dir;
    d = opendir(UNZIP_DIR);
    if (!d) {
        perror("Failed to open directory");
        return 1;
    }

    char *files[1000];
    int count = 0;
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".jpg")) {
            files[count] = strdup(dir->d_name);
            count++;
        }
    }
    closedir(d);

    if (count == 0) {
        printf("Tidak ada file film yang ditemukan.\n");
        return 1;
    }

    srand(time(NULL));
    int random_index = rand() % count;
    printf("Film for Trabowo & Peddy: '%s'\n", files[random_index]);

    // Cleanup
    for (int i = 0; i < count; i++) free(files[i]);
    return 0;
}
