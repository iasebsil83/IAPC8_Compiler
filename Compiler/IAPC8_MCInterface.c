// ----------------- Importations ----------------
//standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"
#include "IAPC8_MCInterface.h"






// ----------------- Declarations ----------------
// ---- Program constants ----
#define MC_COMMAND_LENGTH 800
#define MC_CODE_LENGTH 80000
#define MC_FINAL_CODE_LENGTH 100000
#define LINES_PER_COMMAND 32
//#define CLEARIFY //uncoment to clearify MC codes (making them unusable)
//#define DEBUG_ON //uncomment to enable debug messages

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
const int PC_coo_start[3] = {-21,27,138}; //coordinates at execution number 0
const int PC_coo_IASM_stepX = 2; //step to apply to PC_coo to go to the next IASM command
const int PC_coo_IAX_stepZ  = 2; //step to apply to PC_coo to go to the next IAX command
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
	char* cmd = mallocStr(MC_COMMAND_LENGTH);
	sprintf(
		cmd,
		"/summon MinecartCommandBlock %i %i %i {Command:\"/setblock %i %i %i %s\"}",
		  entityCoo[0],   entityCoo[1],   entityCoo[2],
		operatorCoo[0], operatorCoo[1], operatorCoo[2],
		getBlockNameFromState(state)
	);

	return cmd;
}

char* getMinecraftCmd_Byte(int entityCoo[3], const int operatorCoo[8][3], char* states){
	char* cmd = mallocStr(MC_COMMAND_LENGTH);
	sprintf(cmd,"\
/summon MinecartCommandBlock %i %i %i {\
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
}}}}}}}}",
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








// ---------------- MCTranslation functions ----------------
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

void MCtranslateLine(char* source, char* dest, char sepChar){
	//check 8 bits format
	if(!check8bits(source+2)){
		printf("RUNTIME ERROR > IAPC8_MCInterface.c : MCcompileLine() : Incorrect format for agrgument at line %i : \"", lineNbr);
		printOnN(source, 10);
		printf("\"\n");
		return;
	}

	//PC-Variable detection
	char* temp;
	switch(source[0]){
		case '0':
			switch(source[1]){
				case '0':
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : CUU_IN is a computer to user PC-Variable (at line %i : \"", lineNbr);
					printOnN(source, 10);
					printf("\").\n");
					printf("Computer to user PC-variables are not available for now (This includes CUU_IN, CUU_ALU_OF and CUU_PCO_OF).\n");
					return;
				break;
				case '1': //CUU_OUT
					temp = getMinecraftCmd_Byte(PC_coo, CUU_OUT, source+2);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '2': //CUU_DAT_A
					temp = getMinecraftCmd_Byte(PC_coo, CUU_DAT_A, source+2);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '3': //CUU_DAT_S
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_S, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '4': //CUU_DAT_G
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_G, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '5': //CUU_DAT_IG
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_IG, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '6': //CUU_DAT_OG
					temp = getMinecraftCmd_Bit(PC_coo, CUU_DAT_OG, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '7': //CUU_ALU_SS
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_SS, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '8': //CUU_ALU_OS
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_OS, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '9': //CUU_ALU_PS
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_PS, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				default:
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect PC-Variable id : \"%c%c\" (at line %i : \"", source[0], source[1], lineNbr);
					printOnN(source, 10);
					printf("\").\n");
					return;
			}
		break;
		case '1':
			switch(source[1]){
				case '0': //CUU_ALU_OG
					temp = getMinecraftCmd_Bit(PC_coo, CUU_ALU_OG, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '1': //CUU_ALU_OF
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : CUU_ALU_OF is a computer to user PC-Variable (at line %i : \"", lineNbr);
					printOnN(source, 10);
					printf("\").\n");
					printf("Computer to user PC-variables are not available for now (This includes CUU_IN, CUU_ALU_OF and CUU_PCO_OF).\n");
					return;
				break;
				case '2': //CUU_PCO_OF
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : CUU_PCO_OF is a computer to user PC-Variable (at line %i : \"", lineNbr);
					printOnN(source, 10);
					printf("\").\n");
					printf("Computer to user PC-variables are not available for now (This includes CUU_IN, CUU_ALU_OF and CUU_PCO_OF).\n");
					return;
				break;
				case '3': //CUU_PCO_S
					temp = getMinecraftCmd_Bit(PC_coo, CUU_PCO_S, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '4': //CUU_PCO_B
					temp = getMinecraftCmd_Bit(PC_coo, CUU_PCO_B, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '5': //CUU_PCO_A
					temp = getMinecraftCmd_Bit(PC_coo, CUU_PCO_A, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '6': //CUU_PCO_I
					temp = getMinecraftCmd_Byte(PC_coo, CUU_PCO_I, source+2);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '7': //CUU_UOP_S
					temp = getMinecraftCmd_Bit(PC_coo, CUU_UOP_S, source[9]);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				case '8': //CUU_UOP_I
					temp = getMinecraftCmd_Byte(PC_coo, CUU_UOP_I, source+2);
					strcat(dest, temp);
					freeStr(temp,MC_COMMAND_LENGTH);
				break;
				default:
					printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect PC-Variable id : \"");
					printOnN(source, 2);
					printf("\" (at line %i).\n", lineNbr);
					return;
			}
		break;
		default:
			printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect PC-Variable id : \"");
			printOnN(source, 2);
			printf("\" (at line %i).\n", lineNbr);
			return;
	}
	strcat(dest,"\n");

	//Program counter coordinates increase
	switch(sepChar){
		case '\n':
			PC_coo[0] += PC_coo_IASM_stepX;
			PC_coo[2] = PC_coo_start[2];
		break;
		case ' ':
			PC_coo[2] += PC_coo_IAX_stepZ;
		break;
		default:
			printf("RUNTIME ERROR > MCInterface.c : MCcompileLine() : Incorrect separation char '%c' after command \"", sepChar);
			printOnN(source, 10);
			printf("\"(at line %i).\n", lineNbr);
			exit(EXIT_FAILURE);
	}
}








// ---------------- Execution ----------------
char* MCtranslate(char* code){ //code will be set free (memory improvement)
	//error cases
	if(code == NULL){
		printf("RUNTIME ERROR > IAPC8_MCInterface.c : MCtranslate() : Unable to read source code\n");
		return NULL;
	}

	//caution message
	printf("MCInterfacer > IAPC8_MCInterface.c : MCtranslate() : MCInterfacer is only in SET mode so CUU_IN, CUU_ALU_OF and CUU_PCO_OF are disabled\n");

	//prepare MCtranslation
	char* mc_code = mallocStr(MC_CODE_LENGTH);
	int mc_code_index = 0;
	PC_coo[0] = PC_coo_start[0];
	PC_coo[1] = PC_coo_start[1];
	PC_coo[2] = PC_coo_start[2];
	lineNbr = 1;
	int commandNbrInLine = 0;

	//source code reading
	for(int c=0; c < strlen(code); c++){

		//line by line
		if(code[c] == '\n' || code[c] == ' '){
			commandNbrInLine++;

			//format check
			if(c-mc_code_index != 10){
				printf("FATAL ERROR > IAPC8_MCInterface.c : MCtranslate() : Incorrect line format at line %i (10 characters needed, %i found)\n", lineNbr, c-mc_code_index);
				exit(EXIT_FAILURE);
			}

			#ifdef DEBUG_ON
			printf("DEBUG > IAPC8_MCInterface.c : MCtranslate() : Translating IAX command %i at line %i : \"", commandNbrInLine, lineNbr);
			printOnN(code+mc_code_index, 10);
			printf("\"\n");
			#endif

			MCtranslateLine(code+mc_code_index, mc_code, code[c]);
			#ifdef CLEARIFY
			strcat(mc_code,"\n");
			#endif

			if(code[c] == '\n'){
				lineNbr++;
				commandNbrInLine = 0;
			}
			mc_code_index = c+1;
		}
	}

	//free code
	freeStr(code, strlen(code));

	//shorten the MCcode string
	int mc_code_len = strlen(mc_code);
	char* returnCode = mallocStr(mc_code_len);
	strcpy(returnCode,mc_code);
	freeStr(mc_code, MC_CODE_LENGTH);

	printf("MCInterfacer > IAPC8_MCInterface.c : MCtranslate() : MC translation done.\n");

	return returnCode;
}

char* MCgather(char* code){ //code will be set free (memory improvement)
	//error cases
	if(code == NULL){
		printf("RUNTIME ERROR > IAPC8_MCInterface.c : MCgather() : Unable to read source code.\n");
		return NULL;
	}

	//prepare mc_compilation
	char* mc_final_code = mallocStr(MC_FINAL_CODE_LENGTH);
	int mc_final_code_index = 0;
	lineNbr = 0;
	int localLineNbr = 0;
	int endBracketNbr = 0;

	//source code reading
	for(int c=0; c < strlen(code); c++){

		//line by line
		if(code[c] == '\n'){
			#ifdef CLEARIFY
			if(code[c+1] == '\n')
				continue;
			#endif

			lineNbr++;
			localLineNbr++;
			if(localLineNbr >= LINES_PER_COMMAND){ //CONSTANT
				localLineNbr = 1;

				//add final double quotes + all end brackets
				strcat(mc_final_code, "\"");
				for(int b=0; b < endBracketNbr; b++)
					strcat(mc_final_code,"}");
				endBracketNbr = 0;
				strcat(mc_final_code, "\n\n\n");
			}

			#ifdef DEBUG_ON
			printf("DEBUG > IAPC8_MCInterface.c : MCgather() : Gathering IAX command %i : \"", lineNbr);
			printUntilChar(code+mc_final_code_index, '\n');
			printf("\"\n");
			#endif

			//gather
			if(localLineNbr == 1){ //first line is interpreted differently
				strcat(mc_final_code, "/summon MinecartCommandBlock ~ ~2 ~ {Command:\"");

				#ifdef CLEARIFY
				strcat(mc_final_code,"\n\n\t");
				#endif

				strcatUntilChar(mc_final_code, code+mc_final_code_index, '\n');

				#ifdef CLEARIFY
				strcat(mc_final_code,"\n\n");
				#endif

				endBracketNbr = 1;
			}else{
				strcat(mc_final_code, "\",Riding:{id:MinecartCommandBlock,Command:\"");

				#ifdef CLEARIFY
				strcat(mc_final_code,"\n\n\t");
				#endif

				strcatUntilChar(mc_final_code, code+mc_final_code_index, '\n');

				#ifdef CLEARIFY
				strcat(mc_final_code,"\n\n");
				#endif

				endBracketNbr++;
			}

			//go to next line
			mc_final_code_index = c+1;
		}
	}

	//free code
	freeStr(code, strlen(code));

	//add final double quote + all end brackets
	strcat(mc_final_code, "\"");
	for(int b=0; b < endBracketNbr; b++)
		strcat(mc_final_code,"}");

	//shorten the final MCcode string
	int mc_final_code_len = strlen(mc_final_code);
	char* returnCode = mallocStr(mc_final_code_len);
	strcpy(returnCode,mc_final_code);
	freeStr(mc_final_code, MC_FINAL_CODE_LENGTH);

	printf("MCInterfacer > IAPC8_MCInterface.c : MCgather() : MC gathering done.\n");

	return returnCode;
}