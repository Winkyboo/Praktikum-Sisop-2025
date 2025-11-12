#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UNZIP_DIR "/home/youruser/Downloads/film_unzip"

int main() {
    // Unduh file ZIP
    system("curl -c ./cookie -s -L \"https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B\" > /tmp/intermediate.html");
    system("CONFIRM_CODE=$(grep -o 'confirm=[^&]*' /tmp/intermediate.html | sed 's/confirm=//') && curl -Lb ./cookie \"https://drive.google.com/uc?export=download&confirm=$CONFIRM_CODE&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B\" -o film_list.zip");

    // Unzip
    mkdir(UNZIP_DIR, 0755);
    system("unzip -o film_list.zip -d " UNZIP_DIR " >/dev/null");

    printf("File ZIP berhasil diunduh dan diekstrak ke %s\n", UNZIP_DIR);
    return 0;
}
