// ---------------- Minecraft commands functions ----------------
char* getBlockNameFromState(char state);
char* getMinecraftCmd_Bit(int entityCoo[3], int operationCoo[3], char state);
char* getMinecraftCmd_Byte(int entityCoo[3], int operationCoo[8][3], char* states);






// -------------- Execution -------------
char* precompile(char* text);
