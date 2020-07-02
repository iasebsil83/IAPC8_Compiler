// ---------------- Precompiler functions ----------------
void addFormatedLine(char* dest, char* source, int source_index);








// ---------------- Compiler functions ----------------
void compileCommand(char* source, char* dest);








// ---------------- Execution ----------------
char* precompile(char* text);
char* compile(char* code); //code will be set free (memory improvement)
