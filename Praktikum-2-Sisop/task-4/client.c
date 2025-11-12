#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234       
#define MAX_MESSAGE 256    

typedef struct {
    char message[MAX_MESSAGE];
    int count;
} SharedData;

int main() {

    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) 
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    SharedData *data = (SharedData *) shmat(shmid, NULL, 0);

    if (data == (void *) -1) 
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    char input[300];
 
    printf("Masukkan pesan dan jumlah pesan (format: pesan;jumlah):\n");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = 0;

    char *token = strtok(input, ";");
   
    if (token == NULL) 
    {
        printf("Format input salah.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(data->message, token);

    token = strtok(NULL, ";");

    if (token == NULL) 
    {
        printf("Format input salah.\n");
        exit(EXIT_FAILURE);
    }
    data->count = atoi(token);

    FILE *logfile = fopen("sistem.log", "a");

    if (logfile == NULL) 
    {
        perror("Gagal membuka sistem.log");
        exit(EXIT_FAILURE);
    }

    fprintf(logfile, "Message from client: %s\n", data->message);
    fprintf(logfile, "Message count: %d\n", data->count);
    fclose(logfile);

    printf("Pesan berhasil dikirim ke shared memory.\n");

    shmdt(data);

    return 0;
}
