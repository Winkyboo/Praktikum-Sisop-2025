
#include "std_lib.h"
#include "kernel.h"

void handleWC(char* input);

void printString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        interrupt(0x10, 0x0E00 + str[i], 0, 0, 0); // AH=0x0E, AL=char
        i++;
    }
}

void readString(char* buf) 
{
    int i = 0;
    char c;

    while (1) 
    {
        c = interrupt(0x16, 0x0000, 0, 0, 0) & 0xFF; 

        if (c == 0x0D)
         { // Enter
            buf[i] = '\0';
            break;
        } else if (c == 0x08) { 
            if (i > 0) 
            {
                i--;
                printString("\b \b");
            }
        } else {
            buf[i] = c;
            i++;
            interrupt(0x10, 0x0E00 + c, 0, 0, 0); 
        }
    }
}


void clearScreen() 
{
    interrupt(0x10, 0x0600, 0, 0x184F, 0x07);
    interrupt(0x10, 0x0200, 0, 0, 0);
}


void main() {
    char buf[128];

    clearScreen();
    printString("LilHabOS - D13");

    while (true) {
        printString("$> ");
        readString(buf);
        printString("\n");

        if (strlen(buf) > 0) {
            handleCommand(buf);
        }
    }
}

void handleCommand(char* input) 
{
    int i = 0, pipeIndex = -1;
    int j, k, l;

    while (input[i] != '\0') 
    {
        if (input[i] == '|') 
        {
            pipeIndex = i;
            break;
        }
        i++;
    }

    if (pipeIndex == -1) 
    {
        handleEcho(input, NULL);
    } else {
        char beforePipe[128], afterPipe[128];
        j = 0;
        for (j = 0; j < pipeIndex - 1; j++) beforePipe[j] = input[j];
        beforePipe[j] = '\0';

        k = pipeIndex + 1;
        while (input[k] == ' ') k++;
        l = 0;
        while (input[k] != '\0') afterPipe[l++] = input[k++];
        afterPipe[l] = '\0';

        handleEcho(beforePipe, afterPipe);
    }
}

void handleEcho(char* cmd, char* pipeCmd) 
{
    if (cmd[0]=='e' && cmd[1]=='c' && cmd[2]=='h' && cmd[3]=='o' && cmd[4]==' ') 
    {
        char* arg = cmd + 5;

        if (pipeCmd == NULL) 
        {
            printString(arg);
            printString("\n");
        } else {
            if (pipeCmd[0]=='g' && pipeCmd[1]=='r' && pipeCmd[2]=='e' && pipeCmd[3]=='p' && pipeCmd[4]==' ') 
            {
                char* pattern = pipeCmd + 5;
                handleGrep(arg, pattern);
            } else {
                printString("Perintah setelah pipe tidak dikenali\n");
            }
        }
    } else {
        printString("Perintah tidak dikenali\n");
    }
}

void handleGrep(char* input, char* pipeCmd) {
    int i, j, match;
    int k;
    int inputLen = strlen(input);
    int patternLen;
    char grepArg[128], nextCmd[128];
    int splitIndex = -1;

    for (i = 0; pipeCmd[i] != '\0'; i++) 
    {
        if (pipeCmd[i] == '|') 
        {
            splitIndex = i;
            break;
        }
    }

    if (splitIndex == -1) 
    {
        strcpy(pipeCmd, grepArg);
        nextCmd[0] = '\0';
    } else {
        for (j = 0; j < splitIndex - 1; j++) grepArg[j] = pipeCmd[j];
        grepArg[j] = '\0';

        k = splitIndex + 1;
        while (pipeCmd[k] == ' ') k++;

        j = 0;
        while (pipeCmd[k] != '\0') nextCmd[j++] = pipeCmd[k++];
        nextCmd[j] = '\0';
    }

    patternLen = strlen(grepArg);
    for (i = 0; i <= inputLen - patternLen; i++) 
    {
        match = 1;
        for (j = 0; j < patternLen; j++) 
        {
            if (input[i + j] != grepArg[j]) 
            {
                match = 0;
                break;
            }
        }
        if (match) 
        {
            if (nextCmd[0] != '\0' && nextCmd[0]=='w' && nextCmd[1]=='c') 
            {
                handleWC(input);
            } else {
                printString(input); printString("\n");
            }
            return;
        }
    }

    if (nextCmd[0] != '\0' && nextCmd[0]=='w' && nextCmd[1]=='c') 
    {
        handleWC(NULL);
    } else {
        printString("NULL\n");
    }
}

void handleWC(char* input)
{
    int i = 0, words = 0, chars = 0, lines = 1;
    int inWord = 0;
    char buffer[16];

    if (input == NULL || strlen(input) == 0) {
        printString("0 0 0\n");
        return;
    }

    while (input[i] != '\0') {
        chars++;
        if (input[i] == '\n') {
            lines++;
        }

        if (input[i] != ' ' && inWord == 0) {
            words++;
            inWord = 1;
        } else if (input[i] == ' ') {
            inWord = 0;
        }

        i++;
    }

    itoa(lines, buffer); printString(buffer); printString(" ");
    itoa(words, buffer); printString(buffer); printString(" ");
    itoa(chars, buffer); printString(buffer); printString("\n");
}
