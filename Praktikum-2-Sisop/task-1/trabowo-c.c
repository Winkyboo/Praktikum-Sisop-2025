#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define UNZIP_DIR "/home/youruser/Downloads/film_unzip"
#define FILM_DIR "/home/youruser/film"
#define RECAP_FILE "/home/youruser/film/recap.txt"
#define TOTAL_FILE "/home/youruser/film/total.txt"

typedef struct {
    char **files;
    int start;
    int end;
    const char *person;
} ThreadArgs;

void *move_files(void *args_void) {
    ThreadArgs *args = (ThreadArgs *)args_void;
    FILE *recap = fopen(RECAP_FILE, "a");

    for (int i = args->start; i < args->end; i++) {
        char *filename = strrchr(args->files[i], '/');
        if (!filename) continue;
        filename++;

        char target_path[512];
        const char *genre_dir = NULL;
        if (strstr(filename, "Horror") || strstr(filename, "horror")) genre_dir = "FilmHorror";
        else if (strstr(filename, "Animasi") || strstr(filename, "animasi")) genre_dir = "FilmAnimasi";
        else if (strstr(filename, "Drama") || strstr(filename, "drama")) genre_dir = "FilmDrama";
        else continue;

        sprintf(target_path, "%s/%s/%s", FILM_DIR, genre_dir, filename);
        rename(args->files[i], target_path);

        fprintf(recap, "[...] %s: %s telah dipindahkan ke %s\n", args->person, filename, genre_dir);
    }

    fclose(recap);
    pthread_exit(NULL);
}

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
            char *fullpath = malloc(512);
            snprintf(fullpath, 512, "%s/%s", UNZIP_DIR, dir->d_name);
            files[count++] = fullpath;
        }
    }
    closedir(d);

    int mid = count / 2;

    pthread_t trabowo_thread, peddy_thread;
    ThreadArgs trabowo_args = { files, 0, mid, "Trabowo" };
    ThreadArgs peddy_args = { files, mid, count, "Peddy" };

    mkdir(FILM_DIR, 0755);
    mkdir(FILM_DIR "/FilmHorror", 0755);
    mkdir(FILM_DIR "/FilmAnimasi", 0755);
    mkdir(FILM_DIR "/FilmDrama", 0755);
    FILE *f = fopen(RECAP_FILE, "w"); fclose(f);

    pthread_create(&trabowo_thread, NULL, move_files, &trabowo_args);
    pthread_create(&peddy_thread, NULL, move_files, &peddy_args);

    pthread_join(trabowo_thread, NULL);
    pthread_join(peddy_thread, NULL);

    system("find " FILM_DIR "/FilmHorror -iname \"*.jpg\" | wc -l > /tmp/count_horror");
    system("find " FILM_DIR "/FilmAnimasi -iname \"*.jpg\" | wc -l > /tmp/count_animasi");
    system("find " FILM_DIR "/FilmDrama -iname \"*.jpg\" | wc -l > /tmp/count_drama");

    int count_horror = 0, count_animasi = 0, count_drama = 0;
    FILE *fp = fopen("/tmp/count_horror", "r");
    fscanf(fp, "%d", &count_horror);
    fclose(fp);
    fp = fopen("/tmp/count_animasi", "r");
    fscanf(fp, "%d", &count_animasi);
    fclose(fp);
    fp = fopen("/tmp/count_drama", "r");
    fscanf(fp, "%d", &count_drama);
    fclose(fp);

    char *genre = "horror";
    int max = count_horror;
    if (count_animasi > max) { max = count_animasi; genre = "animasi"; }
    if (count_drama > max) { max = count_drama; genre = "drama"; }

    fp = fopen(TOTAL_FILE, "w");
    fprintf(fp, "Jumlah film horror: %d\n", count_horror);
    fprintf(fp, "Jumlah film animasi: %d\n", count_animasi);
    fprintf(fp, "Jumlah film drama: %d\n", count_drama);
    fprintf(fp, "Genre dengan jumlah film terbanyak: %s\n", genre);
    fclose(fp);

    for (int i = 0; i < count; i++) free(files[i]);
    printf("Film berhasil diorganisir dan dicatat.\n");
    return 0;
}
