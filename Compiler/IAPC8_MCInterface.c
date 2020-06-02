// ----------------- Importations ----------------
//standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"
#include "IAPC8_MCInterface.h"






// ----------------- Declarations ----------------
// ---- Program constants ----
#define MINECRAFT_COMMAND_LENGTH 100

// ---- IAPC8 coordinates ----
//CUU
const int CUU_IN[8][3] = { //useless ?
	{ 1,18,122}, //msb
	{ 3,18,122},
	{ 5,18,122},
	{ 7,18,122},
	{ 9,18,122},
	{11,18,122},
	{13,18,122},
	{15,18,122}
};
const int CUU_OUT[8][3] = {
	{  0,17,122}, //msb
	{ -2,17,122},
	{ -4,17,122},
	{ -6,17,122},
	{ -8,17,122},
	{-10,17,122},
	{-13,17,122},
	{-15,17,122}
};
const int CUU_ADR[8][3] = {
	{ -1,21,122}, //msb
	{ -3,21,122},
	{ -5,21,122},
	{ -7,21,122},
	{ -9,21,122},
	{-11,21,122},
	{-13,21,122},
	{-15,21,122}
};
//DAT
const int CUU_DAT_S[3]  = { -8,19,122};
const int CUU_DAT_G[3]  = {-10,19,122};
const int CUU_DAT_IG[3] = { -4,19,122};
const int CUU_DAT_OG[3] = { -6,19,122};
//TMP
#define DAT_TMP_0 00001111 //last memory case
#define DAT_TMP_1 00001110
//ALU
const int CUU_ALU_IS[3] = {-12,19,122};
const int CUU_ALU_OS[3] = {-14,19,122};
const int CUU_ALU_OP[3] = {-16,19,122};
const int CUU_ALU_OG[3] = {-17,17,122};
const int CUU_ALU_OF[3] = { -2,20,122};
//PCO
const int CUU_PCO_OF[3]  = {-17,26,126};
const int CUU_PCO_S[3]   = {-17,26,128};
const int CUU_PCO_B[3]   = {-17,26,130};
const int CUU_PCO_ADD[3] = {-17,26,132};
const int CUU_PCO_IN[8][3] = {
	{-16,24,145}, //msb
	{-16,24,147},
	{-16,24,149},
	{-16,24,151},
	{-16,24,153},
	{-16,24,155},
	{-16,24,157},
	{-16,24,159}
};
const int CUU_UOP_P[3] = {};
const int CUU_UOP_D[3] = {};






// ---------------- Minecraft commands functions ----------------
char* getBlockNameFromState(char state){
	if(state == '1')
		return "redstone_block";
	return "air";
}

char* getMinecraftCmd_Bit(int entityCoo[3], int operationCoo[3], char state){
	char* cmd = mallocStr(MINECRAFT_COMMAND_LENGTH);
	sprintf(
		cmd,
		"/summon %i %i %i MinecartCommandBlock {Command:\"/setblock %i %i %i %s\"}",
		   entityCoo[0],    entityCoo[1],    entityCoo[2],
		operationCoo[0], operationCoo[1], operationCoo[2],
		getBlockNameFromState(state)
	);

	//shorten the allocation of the command string (if possible)
	int cmd_len = strlen(cmd);
	char* returnCmd = mallocStr(cmd_len);
	strcpy(returnCmd,cmd);
	freeStr(cmd);

	return returnCmd;
}

char* getMinecraftCmd_Byte(int entityCoo[3], int operationCoo[8][3], char* states){
	char* cmd = mallocStr(MINECRAFT_COMMAND_LENGTH);
	sprintf(
		cmd,"/summon %i %i %i\
\
MinecartCommandBlock {\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\",Riding:{\
\
id:MinecartCommandBlock,\
Command:\"/setblock %i %i %i %s\"\
\
}}}}}}}",
		   entityCoo   [0],   entityCoo   [1],   entityCoo   [2],
		operationCoo[0][0],operationCoo[0][1],operationCoo[0][2], getBlockNameFromState(states[0]),
		operationCoo[1][0],operationCoo[1][1],operationCoo[1][2], getBlockNameFromState(states[1]),
		operationCoo[2][0],operationCoo[2][1],operationCoo[2][2], getBlockNameFromState(states[2]),
		operationCoo[3][0],operationCoo[3][1],operationCoo[3][2], getBlockNameFromState(states[3]),
		operationCoo[4][0],operationCoo[4][1],operationCoo[4][2], getBlockNameFromState(states[4]),
		operationCoo[5][0],operationCoo[5][1],operationCoo[5][2], getBlockNameFromState(states[5]),
		operationCoo[6][0],operationCoo[6][1],operationCoo[6][2], getBlockNameFromState(states[6]),
		operationCoo[7][0],operationCoo[7][1],operationCoo[7][2], getBlockNameFromState(states[7])
	);

	//shorten the allocation of the command string (if possible)
	int cmd_len = strlen(cmd);
	char* returnCmd = mallocStr(cmd_len);
	strcpy(returnCmd,cmd);
	freeStr(cmd);

	return returnCmd;
}
