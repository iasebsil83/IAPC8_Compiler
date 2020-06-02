// ---------------- Useful functions ----------------
char* mallocStr(int len);
void freeStr(char* str);
int charInStr(char c, const char* str);
int char2int(char c);
char* hex24bin(char h);
char* dec28bin(char* d);






// ---------------- File management ----------------
char* readFromFile(char* fileName);
void writeToFile(char* fileName, char* code);
