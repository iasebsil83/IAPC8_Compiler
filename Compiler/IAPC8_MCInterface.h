// ---------------- Minecraft commands functions ----------------
char* getBlockNameFromState(char state);
char* getMinecraftCmd_Bit(int entityCoo[3], const int operatorCoo[3], char state);
char* getMinecraftCmd_Byte(int entityCoo[3], const int operatorCoo[8][3], char* states);








// ---------------- MCTranslation functions ----------------
int check8bits(char* str);
void MCtranslateLine(char*source, char* dest, char sepChar);








// ---------------- Execution ----------------
char* MCtranslate(char* code); //code will be set free (memory improvement)
char* MCgather(char* code); //code will be set free (memory improvement)