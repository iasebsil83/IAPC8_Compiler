// ---------------- String functions ----------------
char* mallocStr(int len);
void freeStr(char* str, int len);
int charInStr(char c, const char* str);
int strcmpN(char* s1, char* s2, int len);
void strcatExe(char* dest, char* pc_var, char* arg);
void strcatUntilChar(char* dest, char* source, char endChar);
void printOnN(char* str, int len);
void printUntilChar(char* str, char endChar);








// ---------------- Conversions ----------------
int char2int(char c);
char* hex24bin(char h);
char* dec28bin(char* d);
int _8bin2dec(char* b);








// ---------------- File management ----------------
char* readFromFile(char* fileName);
void writeToFile(char* fileName, char* code);
