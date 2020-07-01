// ----------------- Importations ----------------
//standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"
#include "IAPC8_MCInterface.h"






// ----------------- Declarations ----------------
// ---- Program constants ----
#define MINECRAFT_COMMAND_LENGTH 800
#define MC_CODE_LENGTH 320000
//#define DEBUG_ON //comment to disable debug messages

// ---- IAPC8 PC-Variables coordinates ---- (CUU_IN, CUU_PCO_OF and CUU_ALU_OF are disabled for the moment)
//CUU
/*const int CUU_IN[8][3] = {
	{15,19,124}, //msb (most significant bit)
	{13,19,124},
	{11,19,124},
	{ 9,19,124},
	{ 7,19,124},
	{ 5,19,124},
	{ 3,19,124},
	{ 1,19,124}
};*/
const int CUU_OUT[8][3] = {
	{ -1,17,124}, //msb
	{ -3,17,124},
	{ -5,17,124},
	{ -7,17,124},
	{ -9,17,124},
	{-11,17,124},
	{-13,17,124},
	{-15,17,124}
};
//PCO
const int CUU_PCO_I[8][3] = {
	{ -1,23,124}, //msb
	{ -3,23,124},
	{ -5,23,124},
	{ -7,23,124},
	{ -9,23,124},
	{-11,23,124},
	{-13,23,124},
	{-15,23,124}
};
//const int CUU_PCO_OF[3] = {-19,19,124};
const int CUU_PCO_S[3]  = {-19,21,124};
const int CUU_PCO_B[3]  = {-17,21,124};
const int CUU_PCO_A[3]  = {-19,17,124};
//UOP
const int CUU_UOP_S[3]  = { -1,19,124};
const int CUU_UOP_I[8][3] = {
	{15,32,124}, //msb
	{13,32,124},
	{11,32,124},
	{ 9,32,124},
	{ 7,32,124},
	{ 5,32,124},
	{ 3,32,124},
	{ 1,32,124}
};
//DAT
const int CUU_DAT_A[8][3] = {
	{ -1,21,124}, //msb
	{ -3,21,124},
	{ -5,21,124},
	{ -7,21,124},
	{ -9,21,124},
	{-11,21,124},
	{-13,21,124},
	{-15,21,124}
};
const int CUU_DAT_S[3]  = { -9,19,124};
const int CUU_DAT_G[3]  = {-11,19,124};
const int CUU_DAT_IG[3] = { -5,19,124};
const int CUU_DAT_OG[3] = { -7,19,124};
//ALU
const int CUU_ALU_SS[3] = {-13,19,124};
const int CUU_ALU_OS[3] = {-15,19,124};
const int CUU_ALU_PS[3] = {-17,19,124};
const int CUU_ALU_OG[3] = {-17,17,124};
//const int CUU_ALU_OF[3] = { -3,19,124};

// ---- Program Counter coordinate ----
const int PC_coo_start[3] = {-21,25,136}; //coordinates at execution number 0
const int PC_coo_step[3]  = {  2, 0,  0}; //step to apply to PC_coo to go to the next line
static int PC_coo[3];
static int lineNbr = 0;







// ---------------- Minecraft commands functions ----------------
char* getBlockNameFromState(char state){
	if(state == '1')
		return "redstone_block";
	else if(state == '0')
		return "air";

	//error cases
	printf("FATAL ERROR > MCInterface.c : Incorrect binary number \"%c\"\n", state);
	exit(EXIT_FAILURE);
	return ""; //will never be executed
}

char* getMinecraftCmd_Bit(int entityCoo[3], const int operatorCoo[3], char state){
	char* cmd = mallocStr(MINECRAFT_COMMAND_LENGTH);
	sprintf(
		cmd,
		"/summon %i %i %i MinecartCommandBlock {Command:\"/setblock %i %i %i %s\"}",
		  entityCoo[0],   entityCoo[1],   entityCoo[2],
		operatorCoo[0], operatorCoo[1], operatorCoo[2],
		getBlockNameFromState(state)
	);

	return cmd;
}

char* getMinecraftCmd_Byte(int entityCoo[3], const int operatorCoo[8][3], char* states){
	char* cmd = mallocStr(MINECRAFT_COMMAND_LENGTH);
	sprintf(
		cmd,"/summon %i %i %i \
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
		  entityCoo   [0],  entityCoo   [1],  entityCoo   [2],
		operatorCoo[0][0],operatorCoo[0][1],operatorCoo[0][2], getBlockNameFromState(states[0]),
		operatorCoo[1][0],operatorCoo[1][1],operatorCoo[1][2], getBlockNameFromState(states[1]),
		operatorCoo[2][0],operatorCoo[2][1],operatorCoo[2][2], getBlockNameFromState(states[2]),
		operatorCoo[3][0],operatorCoo[3][1],operatorCoo[3][2], getBlockNameFromState(states[3]),
		operatorCoo[4][0],operatorCoo[4][1],operatorCoo[4][2], getBlockNameFromState(states[4]),
		operatorCoo[5][0],operatorCoo[5][1],operatorCoo[5][2], getBlockNameFromState(states[5]),
		operatorCoo[6][0],operatorCoo[6][1],operatorCoo[6][2], getBlockNameFromState(states[6]),
		operatorCoo[7][0],operatorCoo[7][1],operatorCoo[7][2], getBlockNameFromState(states[7])
	);

	return cmd;
}








// ---------------- MC-Compilation functions ----------------
int check8bits(char* str){
	return (
		(str[0] == '0' || str[0] == '1') &&
		(str[1] == '0' || str[1] == '1') &&
		(str[2] == '0' || str[2] == '1') &&
		(str[3] == '0' || str[3] == '1') &&
		(str[4] == '0' || str[4] == '1') &&
		(str[5] == '0' || str[5] == '1') &&
		(str[6] == '0' || str[6] == '1') &&
		(str[7] == '0' || str[7] == '1')
	);
}

void MCcompileLine(char* source, char* dest){
	//check 8 bits format
	if(!check8bits(source+2)){
		printf("RUNTIME ERROR > IAPC8_MCInterface.c : MCcompileLine() : Incorrect format for agrgument at line %i : \"%10s\"\n", lineNbr, source);
		return;
	}

	//PC-Variable detection
	char* temp;
	switch(source[0]){
		case '0':
			switch(source[1]){
				case '0':
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : CUU_IN is a computer to user PC-Variable (at line %i : \"%10s\").\n", lineNbr, source);
					printf("Computer to user PC-variables are not available for now (This includes CUU_IN, CUU_ALU_OF and CUU_PCO_OF).\n");
					return;
				break;
				case '1': //CUU_OUT
					temp = getMinecraftCmd_Byte(PC_coo, CUU_OUT, source+2);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '2': //CUU_DAT_A
					temp = getMinecraftCmd_Byte(PC_coo, CUU_DAT_A, source+2);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '3': //CUU_DAT_S
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_S, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '4': //CUU_DAT_G
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_G, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '5': //CUU_DAT_IG
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_IG, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '6': //CUU_DAT_OG
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_OG, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '7': //CUU_ALU_SS
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_SS, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '8': //CUU_ALU_OS
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_OS, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '9': //CUU_ALU_PS
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_PS, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				default:
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect PC-Variable id : \"%2s\" (at line %i)\n", source, lineNbr);
					return;
			}
		break;
		case '1':
			switch(source[1]){
				case '0': //CUU_ALU_OG
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_OG, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '1': //CUU_ALU_OF
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : CUU_ALU_OF is a computer to user PC-Variable (at line %i : \"%10s\").\n", lineNbr, source);
					printf("Computer to user PC-variables are not available for now (This includes CUU_IN, CUU_ALU_OF and CUU_PCO_OF).\n");
					return;
				break;
				case '2': //CUU_PCO_OF
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : CUU_PCO_OF is a computer to user PC-Variable (at line %i : \"%10s\").\n", lineNbr, source);
					printf("Computer to user PC-variables are not available for now (This includes CUU_IN, CUU_ALU_OF and CUU_PCO_OF).\n");
					return;
				break;
				case '3': //CUU_PCO_S
					temp = getMinecraftCmd_Bit(PC_coo, CUU_PCO_S, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '4': //CUU_PCO_B
					temp = getMinecraftCmd_Bit(PC_coo, CUU_PCO_B, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '5': //CUU_PCO_A
					temp = getMinecraftCmd_Bit(PC_coo, CUU_PCO_A, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '6': //CUU_PCO_I
					temp = getMinecraftCmd_Byte(PC_coo, CUU_PCO_I, source+2);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '7': //CUU_UOP_S
					temp = getMinecraftCmd_Bit(PC_coo, CUU_UOP_S, source[9]);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				case '8': //CUU_UOP_I
					temp = getMinecraftCmd_Byte(PC_coo, CUU_UOP_I, source+2);
					strcat(dest, temp);
					freeStr(temp,MINECRAFT_COMMAND_LENGTH);
				break;
				default:
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect PC-Variable id : \"%2s\"\n", source);
					return;
			}
		break;
		default:
			printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect PC-Variable id : \"%2s\"\n", source);
			return;
	}
	strcat(dest,"\n\n");

	//Program counter coordinates increase
	PC_coo[0] += PC_coo_step[0];
	PC_coo[1] += PC_coo_step[1];
	PC_coo[2] += PC_coo_step[2];
}








// ---------------- Execution ----------------
char* MCcompile(char* code){
	//error cases
	if(code == NULL){
		printf("RUNTIME ERROR > IAPC8_MCInterface.c : MCcompile() : Unable to read source code\n");
		return NULL;
	}

	//caution message
	printf("MCcompiler > MCcompiler is only in SET mode so CUU_IN, CUU_ALU_OF and CUU_PCO_OF are disabled\n");

	//prepare mc_compilation
	char* mc_code = mallocStr(MC_CODE_LENGTH);
	int mc_code_index = 0;
	PC_coo[0] = PC_coo_start[0];
	PC_coo[1] = PC_coo_start[1];
	PC_coo[2] = PC_coo_start[2];
	lineNbr = 0;

	//source code reading
	for(int c=0; c < strlen(code); c++){

		//line by line
		if(code[c] == '\n'){
			lineNbr++;

			//format check
			if(c-mc_code_index != 10){
				printf("FATAL ERROR > IAPC8_MCInterface.c : MCcompile() : Incorrect line format at line %i (10 characters needed, %i found)\n", lineNbr, c-mc_code_index);
				exit(EXIT_FAILURE);
			}

			#ifdef DEBUG_ON
			printf("DEBUG > IAPC8_MCInterface.c : MCcompile() : Compiling line %i : \"", lineNbr);
			printOnN(code+mc_code_index, 10);
			printf("\"\n");
			#endif

			MCcompileLine(code+mc_code_index, mc_code);
			mc_code_index = c+1;
		}
	}

	//shorten mc-compiled code string
	int mc_code_len = strlen(mc_code);
	char* returnCode = mallocStr(mc_code_len);
	strcpy(returnCode,mc_code);
	freeStr(mc_code, MC_CODE_LENGTH);

	printf("MCcompiler > IAPC8_Compiler.c : MCcompile() : MC compilation done.\n");

	return returnCode;
}
