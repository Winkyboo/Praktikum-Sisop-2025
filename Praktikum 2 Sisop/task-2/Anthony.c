#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_LINE 2048
#define MAX_TITLE 500
#define MAX_YEAR 20
#define MAX_DIRECTOR 500

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    char title[MAX_TITLE];
    char year[MAX_YEAR];
    char director[MAX_DIRECTOR];
} Film;

typedef struct {
    Film *films;
    int count;
    const char *type;
} ThreadData;

int execute_command(char *const argv[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process
        execvp(argv[0], argv);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void write_log(const char *category, const char *title) {
    pthread_mutex_lock(&log_mutex);
    
    FILE *log = fopen("log.txt", "a");
    if (log) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(log, "[%02d:%02d:%02d] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n",
                t->tm_hour, t->tm_min, t->tm_sec, category, title);
        fclose(log);
    }
    
    pthread_mutex_unlock(&log_mutex);
}

void create_directory(const char *name) {
    struct stat st = {0};
    if (stat(name, &st) == -1) {
        mkdir(name, 0777);
    }
}

void *sort_by_title(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    create_directory("judul");
    
    for (int i = 0; i < data->count; i++) {
        Film *film = &data->films[i];
        write_log("Abjad", film->title);
        
        char c = '\0';
        for (int j = 0; film->title[j] != '\0'; j++) {
            if (film->title[j] != '"' && film->title[j] != ' ') {
                c = toupper(film->title[j]);
                break;
            }
        }
        
        char filename[30];
        if (isalnum(c)) {
            sprintf(filename, "judul/%c.txt", c);
        } else {
            sprintf(filename, "judul/#.txt");
        }
        
        FILE *f = fopen(filename, "a");
        if (f) {
            fprintf(f, "%s - %s - %s\n", film->title, film->year, film->director);
            fclose(f);
        }
    }
    
    return NULL;
}

void *sort_by_year(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    create_directory("tahun");
    
    for (int i = 0; i < data->count; i++) {
        Film *film = &data->films[i];
        write_log("Tahun", film->title);
        
        char filename[50];
        sprintf(filename, "tahun/%s.txt", film->year);
        
        FILE *f = fopen(filename, "a");
        if (f) {
            fprintf(f, "%s - %s - %s\n", film->title, film->year, film->director);
            fclose(f);
        }
    }
    
    return NULL;
}

int load_films(const char *filename, Film **films) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open CSV file");
        return -1;
    }
    
    int line_count = 0;
    char line[MAX_LINE];
    
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        line_count++;
    }
    
    *films = malloc(line_count * sizeof(Film));
    if (*films == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return -1;
    }

    rewind(file);
    fgets(line, sizeof(line), file); 
    
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < line_count) {
        char *token;
        char *remainder = line;

        if (line[0] == '"') {
            token = strtok_r(remainder, "\"", &remainder);
            strncpy((*films)[i].title, token, MAX_TITLE - 1);
            (*films)[i].title[MAX_TITLE - 1] = '\0';
            remainder++; 
        } else {
            token = strtok_r(remainder, ",", &remainder);
            strncpy((*films)[i].title, token, MAX_TITLE - 1);
            (*films)[i].title[MAX_TITLE - 1] = '\0';
        }
        
        token = strtok_r(remainder, ",", &remainder);
        if (token) {
            strncpy((*films)[i].year, token, MAX_YEAR - 1);
            (*films)[i].year[MAX_YEAR - 1] = '\0';
            char *p = (*films)[i].year;
            while (*p == ' ') p++;
            memmove((*films)[i].year, p, strlen(p) + 1);
        } else {
            strcpy((*films)[i].year, "Unknown");
        }
        
        if (remainder) {
            strncpy((*films)[i].director, remainder, MAX_DIRECTOR - 1);
            (*films)[i].director[MAX_DIRECTOR - 1] = '\0';
            char *nl = strchr((*films)[i].director, '\n');
            if (nl) *nl = '\0';
            char *p = (*films)[i].director;
            while (*p == ' ') p++;
            memmove((*films)[i].director, p, strlen(p) + 1);
        } else {
            strcpy((*films)[i].director, "Unknown");
        }
        
        i++;
    }
    
    fclose(file);
    return i; 
}

int main() {
    FILE *log = fopen("log.txt", "w");
    if (log) fclose(log);
    
    // 1. Download ZIP
    printf("Downloading the Netflix data ZIP file...\n");
    char *download_args[] = {
        "wget", 
        "https://docs.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J", 
        "-O", 
        "netflixData.zip", 
        NULL
    };
    
    if (execute_command(download_args) != 0) {
        fprintf(stderr, "Failed to download the ZIP file\n");
        return EXIT_FAILURE;
    }
    
    // 2. Extract ZIP
    printf("Extracting the ZIP file...\n");
    char *unzip_args[] = {"unzip", "-j", "-o", "netflixData.zip", NULL};
    
    if (execute_command(unzip_args) != 0) {
        fprintf(stderr, "Failed to extract the ZIP file\n");
        return EXIT_FAILURE;
    }
    
    // 3. Delete ZIP
    printf("Removing the ZIP file...\n");
    char *delete_args[] = {"rm", "netflixData.zip", NULL};
    
    if (execute_command(delete_args) != 0) {
        fprintf(stderr, "Warning: Failed to remove the ZIP file\n");
        // Continue execution even if deletion fails
    }
    
    // 4. Process the CSV file
    printf("Processing the Netflix data...\n");
    Film *films = NULL;
    int film_count = load_films("netflixData.csv", &films);
    
    if (film_count <= 0) {
        fprintf(stderr, "No films found or error loading films\n");
        return EXIT_FAILURE;
    }
    
    printf("Loaded %d films. Sorting in parallel...\n", film_count);
    
    ThreadData title_data = {films, film_count, "Abjad"};
    ThreadData year_data = {films, film_count, "Tahun"};
    
    pthread_t title_thread, year_thread;
    
    if (pthread_create(&title_thread, NULL, sort_by_title, &title_data) != 0) {
        perror("Failed to create title sorting thread");
        free(films);
        return EXIT_FAILURE;
    }
    
    if (pthread_create(&year_thread, NULL, sort_by_year, &year_data) != 0) {
        perror("Failed to create year sorting thread");
        pthread_cancel(title_thread);
        pthread_join(title_thread, NULL);
        free(films);
        return EXIT_FAILURE;
    }
    
    pthread_join(title_thread, NULL);
    pthread_join(year_thread, NULL);
    
    free(films);
    pthread_mutex_destroy(&log_mutex);
    
    printf("All processes completed successfully!\n");
    printf("Files are organized in the 'judul/' and 'tahun/' directories.\n");
    
    return EXIT_SUCCESS;
}
