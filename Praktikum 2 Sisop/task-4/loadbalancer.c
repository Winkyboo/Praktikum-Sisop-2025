#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define SHM_KEY 1234
#define MSG_KEY 5678
#define MAX_MESSAGE 256
#define MAX_WORKERS 3

typedef struct {
    char message[MAX_MESSAGE];
    int count;
} SharedData;

typedef struct {
    long mtype;
    char mtext[MAX_MESSAGE];
} Message;

int main() {

    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);
    
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

    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
  
    if (msgid == -1) 
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    FILE *logfile = fopen("sistem.log", "a");
   
    if (logfile == NULL)
    {
        perror("Gagal membuka sistem.log");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < data->count; i++) 
    {
        fprintf(logfile, "Received at lb: %s (#message %d)\n", data->message, i + 1);
        fflush(logfile);

        Message msg;
        msg.mtype = (i % MAX_WORKERS) + 1; \
        strcpy(msg.mtext, data->message);

        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) 
        {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
    }

    fclose(logfile);

    shmdt(data);

    return 0;
}
