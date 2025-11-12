#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>      
#include <pthread.h>

void Folder(const char *foldername){
    pid_t pid = fork();
    if (pid == 0) {
        execlp("mkdir", "mkdir", "-p", foldername, NULL);
        perror("mkdir failed");
        exit(EXIT_FAILURE);
    } 
    else wait(NULL);
}

void fetchJSON(const char *id){
    pid_t pid = fork();
    if (pid == 0){
        char url[256];
        snprintf(url, sizeof(url), "https://api.jikan.moe/v4/manga/%s/full", id);
        execlp("curl", "curl", "-s", "-o", "response.json", url, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } 
    else wait(NULL);
}

void cleanTitle(char *dest, const char *src){
    while (*src){
        if (isalnum(*src))*dest++ = *src;
        else if (*src == ' ')*dest++ = '_';
        src++;
    }
    *dest = '\0';
}

void parseAndSave(){
    FILE *fp = fopen("response.json", "r");
    if (!fp) {
        perror("Buka response.json gagal");
        return;
    }
    char buffer[8192];
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);
    // Title
    char *judulPtr = strstr(buffer, "\"title\"");
    char title[256] = "Unknown";
    if (judulPtr) sscanf(judulPtr, "\"title\": \"%[^\"]", title);
    // Status
    char *statusPtr = strstr(buffer, "\"status\"");
    char status[128] = "Unknown";
    if (statusPtr) sscanf(statusPtr, "\"status\": \"%[^\"]", status);
    // Release Date
    char *tglPtr = strstr(buffer, "\"from\"");
    char release[128] = "Unknown";
    if (tglPtr){
        char fullDate[128];
        sscanf(tglPtr, "\"from\": \"%[^\"]", fullDate);
        // ambil cmn 10 karakter pertama
        strncpy(release, fullDate, 10);
        release[10] = '\0'; 
    }
    // Genre
    char *genrePtr = strstr(buffer, "\"genres\":");
    char genre[512] = "";
    if (genrePtr){
        char *g = strstr(genrePtr, "\"name\":");
        while (g && g < strstr(buffer, "\"themes\"")){
            char temp[64];
            sscanf(g, "\"name\": \"%[^\"]", temp);
            strcat(genre, temp);
            strcat(genre, ", ");
            g = strstr(g + 1, "\"name\":");
        }
        if (strlen(genre)) genre[strlen(genre) - 2] = '\0';  
    }
    // Theme
    char *themePtr = strstr(buffer, "\"themes\":");
    char theme[256] = "";
    if (themePtr){
        char *themeEnd = strstr(themePtr, "\"demographics\"");
        char *t = strstr(themePtr, "\"name\":");
        while (t && (!themeEnd || t < themeEnd)){
            char temp[64];
            sscanf(t, "\"name\": \"%[^\"]", temp);
            strcat(theme, temp);
            strcat(theme, ", ");
            t = strstr(t + 1, "\"name\":");
        }
        if (strlen(theme)) theme[strlen(theme) - 2] = '\0'; 
    }
    // Author
    char *authorPtr = strstr(buffer, "\"authors\":");
    char authors[256] = "";
    if (authorPtr){
        char *authorEnd = strstr(authorPtr, "\"serializations\"");
        char *a = strstr(authorPtr, "\"name\":");
        while (a && (!authorEnd || a < authorEnd)){
            char temp[64];
            sscanf(a, "\"name\": \"%[^\"]", temp);
            strcat(authors, temp);
            strcat(authors, ", ");
            a = strstr(a + 1, "\"name\":");
        }
        if (strlen(authors)) authors[strlen(authors) - 2] = '\0';
    }

    // Save ke file
    char filename[256];
    cleanTitle(filename, title);
    char filepath[300];
    sprintf(filepath, "Manhwa/%s.txt", filename);
    FILE *out = fopen(filepath, "w");
    if (!out){
        perror("Gagal menulis file output");
        return;
    }
    fprintf(out, "Title: %s\n", title);
    fprintf(out, "Status: %s\n", status);
    fprintf(out, "Release: %s\n", release);
    fprintf(out, "Genre: %s\n", genre);
    fprintf(out, "Theme: %s\n", theme);
    fprintf(out, "Author: %s\n", authors);
    fclose(out);
}

void Uppercase(char *dest, const char *src) {
    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) if (isupper(src[i]))dest[j++] = src[i];
    dest[j] = '\0';
}

// Upprcase_Heroin
typedef struct {
    const char* heroine;
    const char* manhwa_code;
} HeroineInfo;
HeroineInfo heroines[] = {{"Adelia",  "NIOCTP"}, {"Artezia", "TVLA"}, {"Lia",     "MMDW"}, {"Ophelia", "DWCD"}};

void Zipmanga() {
    Folder("Archive");
    
    // Buka direct
    DIR *dir = opendir("Manhwa");
    if (!dir){
        perror("Cannot open Manhwa directory");
        return;
    }
    
   struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_type == DT_DIR || !strstr(entry->d_name, ".txt"))continue;

        printf("Found file: %s\n", entry->d_name);

        char filename[256];
        // delete ekstensi file
        strncpy(filename, entry->d_name, sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
        // path
        char source[512];
        snprintf(source, sizeof(source), "Manhwa/%s", entry->d_name);
        char *dot = strrchr(filename, '.');
        if (dot) *dot = '\0';
        char uppercase[256] = {0};
        Uppercase(uppercase, filename);
        char dest[512];
        snprintf(dest, sizeof(dest), "Archive/%s.zip", uppercase);
        printf("Zipping %s to %s\n", source, dest);

        // fork & exec untuk zip
        pid_t pid = fork();
        if (pid == 0) {
            execlp("zip", "zip", "-j", dest, source, NULL);
            perror("Zip failed");
            exit(EXIT_FAILURE);
        } 
        else wait(NULL);
    }
}

void ZipAkhir() {
    Folder("Archive/Images");

    const int n = sizeof(heroines) / sizeof(HeroineInfo);

    for (int i = 0; i < n; i++) {
        const char* heroine = heroines[i].heroine;
        const char* manhwa_code = heroines[i].manhwa_code;
        char zipname[256];
        snprintf(zipname, sizeof(zipname), "Archive/Images/%s_%s.zip", manhwa_code, heroine);
        char folder[256];
        snprintf(folder, sizeof(folder), "Heroines/%s", heroine);

        // fork & exec untuk zip
        pid_t pid = fork();
        if (pid == 0){
            execlp("zip", "zip", "-r", zipname, folder, NULL);
            perror("Zip failed");
            exit(EXIT_FAILURE);
        } 
        else wait(NULL);
    }
    for (int i = 0; i < n; i++) {
        const char* heroine = heroines[i].heroine;

        char folder[256];
        snprintf(folder, sizeof(folder), "Heroines/%s", heroine);

        DIR* dir = opendir(folder);
        if (!dir) {
            perror("opendir failed");
            continue;
        }

        struct dirent* entry;
        // delete isi direct
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".jpg")) {
                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s/%s", folder, entry->d_name);
                remove(filepath); 
            }
        }

        closedir(dir);
    }
}

// Buat folder (Soal C only)
void makeFolder(const char* path) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", path);
    system(cmd);
}

// Link" Heroine
typedef struct {
    const char* heroine;   
    const char* image_url; 
    int count;             
} DownloadTask;

void* downloadImages(void* arg) {
    DownloadTask* task = (DownloadTask*)arg;
    // utk folder heroine
    char folder[256];
    snprintf(folder, sizeof(folder), "Heroines/%s", task->heroine);
    makeFolder(folder);
    //donlod
    for (int i = 0; i < task->count; i++) {
        char filename[300];
        snprintf(filename, sizeof(filename), "Heroines/%s/%s_%d.jpg",
                 task->heroine, task->heroine, i + 1);
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "curl -s %s -o \"%s\"",
                 task->image_url, filename);
        system(cmd);
    }
    pthread_exit(NULL);
}

int main() {
    fetchJSON("168827");  // Mistake
    parseAndSave();
    fetchJSON("147205");  // The vilainess
    parseAndSave();
    fetchJSON("169731");  // No, i charmed
    parseAndSave();
    fetchJSON("175521");  // Divorce
    parseAndSave();
    Zipmanga();
    makeFolder("Heroines");
    const char* url_lia     = "https://i.imgur.com/CJuHmrb.png";
    const char* url_artezia = "https://i.imgur.com/5c9Lcs5.png";
    const char* url_adelia  = "https://i.imgur.com/zF3Jq8s.png";
    const char* url_ophelia = "https://i.imgur.com/5GyLoVN.png";
    Folder("Manhwa");
    DownloadTask tasks[] = {
        {"Lia",     url_lia,     3}, // Maret
        {"Artezia", url_artezia, 6}, // June
        {"Adelia",  url_adelia,  4}, // April
        {"Ophelia", url_ophelia, 10}  // Oktober
    };

    const int n = sizeof(tasks) / sizeof(DownloadTask);

    for (int i = 0; i < n; i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, downloadImages, (void*)&tasks[i]);
        pthread_join(tid, NULL);  // Ensures they download in order
    }

    ZipAkhir();

    return 0;
}
