// ---------------- Minecraft commands functions ----------------
char* getBlockNameFromState(char state);
char* getMinecraftCmd_Bit(int entityCoo[3], const int operatorCoo[3], char state);
char* getMinecraftCmd_Byte(int entityCoo[3], const int operatorCoo[8][3], char* states);








// ---------------- MC-Compilation functions ----------------
int check8bits(char* str);
void MCcompileLine(char*source, char* dest);








// ---------------- Execution ----------------
char* MCcompile(char* code);