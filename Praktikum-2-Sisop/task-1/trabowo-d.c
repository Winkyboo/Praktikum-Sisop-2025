#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILM_DIR "/home/youruser/film"
#define ZIP_OUTPUT_DIR "/home/youruser/film_zip"

int main() {
    mkdir(ZIP_OUTPUT_DIR, 0755);

    system("cd " FILM_DIR " && zip -r " ZIP_OUTPUT_DIR "/FilmHorror.zip FilmHorror >/dev/null");
    system("cd " FILM_DIR " && zip -r " ZIP_OUTPUT_DIR "/FilmAnimasi.zip FilmAnimasi >/dev/null");
    system("cd " FILM_DIR " && zip -r " ZIP_OUTPUT_DIR "/FilmDrama.zip FilmDrama >/dev/null");

    system("rm -r " FILM_DIR "/FilmHorror " FILM_DIR "/FilmAnimasi " FILM_DIR "/FilmDrama");

    printf("Film berhasil diarsipkan ke %s\n", ZIP_OUTPUT_DIR);
    return 0;
}
