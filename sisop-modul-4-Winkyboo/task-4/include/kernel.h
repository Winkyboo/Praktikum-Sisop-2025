
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void printString(char* str);
void readString(char* buf);
void clearScreen();

void handleCommand(char* input);
void handleEcho(char* cmd, char* pipeCmd);
void handleGrep(char* input, char* pattern);
void handleWC(char* input);
