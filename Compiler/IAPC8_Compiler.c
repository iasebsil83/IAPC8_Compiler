/* ================================ IAPC8 Compiler [0.1.2] ================================

Version details :

0.1.0 > Precompilation steps 1,2 and 3 :
        - Basic text formating into fixed number of characters per line like this :
              CMDpXXXXXXXXpXXXXXXXX
              CMD : iassembly command
              p   : prefix (a,p or r)
              X   : 0 or 1
        - Creation of IAPC8_MCInterface library including IAPC8 system
          coordinates and getMinecraftCmd functions

0.1.2 > Compilation (command matching) :
        - Command ASG added with compatibility : v-v   v-r   v-a (v:value, a:address, r:result, p:program counter)
        - Command INV added with compatibility : v-    a-
        - Command AND added with compatibility : v-v   v-a   a-v   a-a
        - Command LSH added but disabled
        - Command RSH added but disabled
        - Command AD1 added with compatibility : v-    a-
        - Command ADD added with compatibility : v-v   v-a   a-v   a-a
        - Command MUL added with compatibility : v-v   v-a   a-v   a-a
        - Command DIV added but disabled
        - Command EZ_ added with compatibility : v-    a-
        - Command EN_ added with compatibility : v-v   v-a   a-v   a-a
        - Command LZ_ added with compatibility : v-    a-
        - Command GZ_ added with compatibility : v-    a-
        - Command IF_ added with compatibility : a-v   r-v
        - Command JMP added with compatibility : v-
        - Command PXL added with compatibility : v-    a-    r-
        Added MCInterface compiling from IAX to IAMC (each IAX command into a Minecraft command)

0.1.3 > Renaming from "MCcompilation" into "MCtranslation" in MCInterface.
        Full gathering function created in MCInterface.
        Improving and fixing a lot of stuff.
        Switching from single IAX command per line into several (depending on the IASM command)

======================================================================================== */






// ----------------- Importations ----------------
//standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"
#include "IAPC8_Compiler.h"






// ----------------- Declarations ----------------
// ---- Compiler variables ----
//#define DEBUG_ON //uncomment to enable debug messages
static int lineNbr = 0;

// ---- Compiler constants ----
#define PRECOMP_CODE_LENGTH 4000
#define COMP_CODE_LENGTH 32000
const char* STR_TEXT = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\n";

// ---- PC-Variables id ---- (CUU_IN, CUU_PCO_OF and CUU_ALU_OF are disabled for the moment)
//CUU
#define CUU_IN "00"
#define CUU_OUT "01"
//PCO
#define CUU_PCO_I "16"
//#define CUU_PCO_OF "12"
#define CUU_PCO_S "13"
#define CUU_PCO_B "14"
#define CUU_PCO_A "15"
//UOP
#define CUU_UOP_S "17"
#define CUU_UOP_I "18"
//DAT
#define CUU_DAT_A "02"
#define CUU_DAT_S "03"
#define CUU_DAT_G "04"
#define CUU_DAT_IG "05"
#define CUU_DAT_OG "06"
//ALU
#define CUU_ALU_SS "07"
#define CUU_ALU_OS "08"
#define CUU_ALU_PS "09"
#define CUU_ALU_OG "10"
//#define CUU_ALU_OF "11"








// ---------------- Precompiler functions ----------------
void addFormatedLine(char* dest, char* source, int source_index){
	//get destination index
	int dest_index = strlen(dest);

	//command
	dest[dest_index++] = source[source_index++];
	dest[dest_index++] = source[source_index++];
	dest[dest_index++] = source[source_index++];

	//arg1
	//set prefix (p)
	int setValue = 0;
	switch(source[source_index]){
		case 'r':
		case 'p':
			dest[dest_index++] = source[source_index++];
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
		break;
		case 'a':
			dest[dest_index++] = source[source_index++];
			setValue = 1;
		break;
		default:
			dest[dest_index++] = 'v';
			setValue = 1;
	}

	//set bin value (XXXXXXXX)
	if(setValue){
		//convert value string to binary string
		switch(source[source_index]){
			case 'x':
				strcat(dest, hex24bin(source[++source_index]) );
				strcat(dest, hex24bin(source[++source_index]) );
				source_index++;
				dest_index += 8;
			break;
			case 'd':
				strcat(dest, dec28bin(source+source_index+1) );
				source_index += 4;
				dest_index += 8;
			break;
			case '0': //already in binary format
			case '1':
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
			break;
			default:
				printf("FATAL ERROR > IAPC8_Compiler.c : addFormatedLine() : Invalid number prefix detected '%c' (at line %i : \"", source[source_index], lineNbr);
				printUntilChar(source, '\n');
				printf("\")\n");
				exit(EXIT_FAILURE);
		}
	}

	//arg2
	//set prefix (p)
	setValue = 0;
	switch(source[source_index]){
		case 'r':
		case 'p':
			dest[dest_index++] = source[source_index++];
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
			dest[dest_index++] = '0'; dest[dest_index++] = '0';
		break;
		case 'a':
			dest[dest_index++] = source[source_index++];
			setValue = 1;
		break;
		default:
			dest[dest_index++] = 'v';
			setValue = 1;
	}

	//set bin value (XXXXXXXX)
	if(setValue){
		//convert value string to binary string
		switch(source[source_index]){
			case 'x':
				strcat(dest, hex24bin(source[++source_index]) );
				strcat(dest, hex24bin(source[++source_index]) );
				//no need to increase source_index and dest_index, nothing comes after
			break;
			case 'd':
				strcat(dest, dec28bin(source+source_index+1) );
				//no need to increase source_index and dest_index, nothing comes after
			break;
			case '0': //already in binary format
			case '1':
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index  ] = source[source_index  ];
			break;
			default:
				printf("FATAL ERROR > IAPC8_Compiler.c : addFormatedLine() : Invalid number prefix detected '%c' (at line %i : \"", source[source_index], lineNbr);
				printUntilChar(source, '\n');
				printf("\")\n");
				exit(EXIT_FAILURE);
		}
	}
}








// ---------------- Compiler functions ----------------
void compileCommand(char* source, char* dest){

	//all commands (VALUE 'p' NOT AVAILABLE YET)
	if(source[3] == 'p'){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
		printf("Invalid first prefix 'p' for command \"");
		printOnN(source, 21);
		printf("\" (at line %i)\n", lineNbr);
		return;
	}
	if(source[12] == 'p'){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
		printf("Invalid second prefix 'p' for command \"");
		printOnN(source, 21);
		printf("\" (at line %i)\n", lineNbr);
		return;
	}

	//DAT
	//ASG command
	if(strcmpN(source, "ASG", 3)){
		switch(source[3]){ //first prefix
			case 'v':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_IG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_S,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_S,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_IG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_IG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_S,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_S,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_ALU_OG, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_IG, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'a' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("Command ASG does not support pointers yet (dynamic addresses).\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("Command ASG does not support pointers yet (dynamic addresses).\n");
				return;
			case 'a':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid second prefix 'a' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("Command ASG does not support pointers yet (dynamic addresses).\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//ALU
	//INV command
	}else if(strcmpN(source, "INV", 3)){
		switch(source[3]){ //first prefix
			case 'v':
				strcatExe(dest, CUU_OUT,    "00001100"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, "\n"); //reset
			break;
			case 'a':
				strcatExe(dest, CUU_OUT,    "00001100"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, "\n");
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//AND command
	}else if(strcmpN(source,"AND",3)){
		switch(source[3]){ //first prefix
			case 'v':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00001101"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00001101"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'a':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00001101"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00001101"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//LSH command
	}else if(strcmpN(source,"LSH",3)){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
		printf("Command LSH not available yet (\"%21s\" at line %i)\n", source, lineNbr);
		return;



	//RSH command
	}else if(strcmpN(source,"RSH",3)){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
		printf("Command RSH not available yet (\"%21s\" at line %i)\n", source, lineNbr);
		return;



	//AD1 command
	}else if(strcmpN(source,"AD1",3)){
		switch(source[3]){ //first prefix
			case 'v':
				strcatExe(dest, CUU_OUT,    "00001010"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, "\n"); //reset
			break;
			case 'a':
				strcatExe(dest, CUU_OUT,    "00001010"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, "\n");
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//ADD command
	}else if(strcmpN(source,"ADD",3)){
		switch(source[3]){ //first prefix
			case 'v':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00001001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00001001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'a':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00001001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00001001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//MUL command
	}else if(strcmpN(source,"MUL",3)){
		switch(source[3]){ //first prefix
			case 'v':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00001011"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00001011"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'a':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00001011"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00001011"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//DIV command
	}else if(strcmpN(source,"DIV",3)){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
		printf("Command DIV not available yet (\"%21s\" at line %i)\n", source, lineNbr);
		return;



	//EZ_ command
	}else if(strcmpN(source, "EZ_", 3)){
		switch(source[3]){ //first prefix
			case 'v':
				//default state of CUU_OUT is 00000000
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, "\n"); //reset
			break;
			case 'a':
				//default state of CUU_OUT is 00000000
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, "\n");
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//EN_ command
	}else if(strcmpN(source,"EN_",3)){
		switch(source[3]){ //first prefix
			case 'v':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'a':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_OUT,    "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,     source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'a':
						strcatExe(dest, CUU_OUT,    "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,   source+13); strcat(dest, " ");
						//...calculating...
						strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_SS, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
					break;
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix 'r' for command \"");
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("All ALU commands do not support operation on themselves yet.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//GZ_ command
	}else if(strcmpN(source,"GZ_",3)){
		switch(source[3]){ //first prefix
			case 'v':
				strcatExe(dest, CUU_OUT,    "00000010"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, "\n"); //reset
			break;
			case 'a':
				strcatExe(dest, CUU_OUT,    "00000010"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, "\n");
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
			printOnN(source, 21);
			printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//LZ_ command
	}else if(strcmpN(source,"LZ_",3)){
		switch(source[3]){ //first prefix
			case 'v':
				strcatExe(dest, CUU_OUT,    "00000011"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,     source+4 ); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, "\n"); //reset
			break;
			case 'a':
				strcatExe(dest, CUU_OUT,    "00000011"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_PS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
				//...calculating...
				strcatExe(dest, CUU_ALU_OS, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OS, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_OUT,    "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, "\n");
			break;
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("All ALU commands do not support operation on themselves yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//PCO
	//IF_ command
	}else if(strcmpN(source, "IF_", 3)){
		switch(source[3]){ //first prefix
			case 'a':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_I,   source+13); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_B,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_S,  "00000001"); strcat(dest, " ");
						//normally, no need to put it back to 00000000 (auto reset)
						//but in this case, if condition is true, there will be no set
						//so we have to reset it manually
						strcatExe(dest, CUU_PCO_S,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_I,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_B,  "00000000"); strcat(dest, "\n");
					break;
					case 'a':
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("PCO only accepts constant values.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'r':
				switch(source[12]){ //second prefix
					case 'v':
						strcatExe(dest, CUU_ALU_OG, "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_I,   source+13); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_B,  "00000001"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_S,  "00000001"); strcat(dest, " ");
						//normally, no need to put it back to 00000000 (auto reset)
						//but in this case, if condition is true, there will be no set
						//so we have to reset it manually
						strcatExe(dest, CUU_PCO_S,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_ALU_OG, "00000000"); strcat(dest, " "); //reset
						strcatExe(dest, CUU_PCO_I,  "00000000"); strcat(dest, " ");
						strcatExe(dest, CUU_PCO_B,  "00000000"); strcat(dest, "\n");
					break;
					case 'a':
					case 'r':
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						printf("PCO only accepts constant values.\n");
						return;
					default:
						printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
						printf("Invalid second prefix '%c' for command \"", source[12]);
						printOnN(source, 21);
						printf("\" (at line %i)\n", lineNbr);
						return;
				}
			break;
			case 'v':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'v' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("Constant conditions are not supported yet.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//JMP command
	}else if(strcmpN(source,"JMP",3)){
		switch(source[3]){ //first prefix
			case 'v':
				strcatExe(dest, CUU_PCO_I,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_PCO_A,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_PCO_S,  "00000001"); strcat(dest, " ");
				//no need to put it back to 00000000 (auto reset)
				strcatExe(dest, CUU_PCO_I,  "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_PCO_A,  "00000000"); strcat(dest, "\n");
			break;
			case 'a':
			case 'r':
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix 'r' for command \"");
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				printf("PCO only accepts constant values.\n");
				return;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//UOP
	//PXL command
	}else if(strcmpN(source,"PXL",3)){
		switch(source[3]){ //first prefix
			case 'v':
				strcatExe(dest, CUU_UOP_I,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_S,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_S,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_I,  "00000000"); strcat(dest, "\n"); //reset
			break;
			case 'a':
				strcatExe(dest, CUU_DAT_A,   source+4 ); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_G,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_S,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_S,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_A,  "00000000"); strcat(dest, " "); //reset
				strcatExe(dest, CUU_DAT_G,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_DAT_OG, "00000000"); strcat(dest, "\n");
			break;
			case 'r':
				strcatExe(dest, CUU_ALU_OG, "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_S,  "00000001"); strcat(dest, " ");
				strcatExe(dest, CUU_UOP_S,  "00000000"); strcat(dest, " ");
				strcatExe(dest, CUU_ALU_OG, "00000000"); strcat(dest, "\n"); //reset
			break;
			default:
				printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : ");
				printf("Invalid first prefix '%c' for command \"", source[3]);
				printOnN(source, 21);
				printf("\" (at line %i)\n", lineNbr);
				return;
		}



	//command not found
	}else{
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compileCommand() : Undefined command \"");
		printOnN(source, 3);
		printf("\" (at line %i)\n", lineNbr);
		return;
	}
}








// ---------------- Execution ----------------
char* precompile(char* text){
	if(text == NULL){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : precompile() : Unable to read source code\n");
		return NULL;
	}

	//preparation for step 1
	char* code_step1 = mallocStr(PRECOMP_CODE_LENGTH);
	int code_step1_index = 0;
	int comment = 0;



	//STEP 1
	//load only text characters to code and replace v with x00, all this excluding comments
	for(int c=0; c < strlen(text); c++){
		//text length limit
		if(code_step1_index > PRECOMP_CODE_LENGTH-3){
			printf("RUNTIME ERROR > IAPC8_Compiler.c : precompile() : Source code is too long\n");
			break;
		}

		if(comment){
			//go back to code interpretation
			if(text[c] == '#' || text[c] == '\n'){
				comment = 0;
				if(text[c] == '\n')
					code_step1[code_step1_index++] = '\n';
			}
		}else{
			//go to comment interpretation
			if(text[c] == '#'){
				comment = 1;
				continue;
			}

			//code interpretation
			if(charInStr(text[c],STR_TEXT)){
				if(text[c] == 'v'){
					code_step1[code_step1_index++] = 'x';
					code_step1[code_step1_index++] = '0';
					code_step1[code_step1_index++] = '0';
				}else
					code_step1[code_step1_index++] = text[c];
			}
		}
	}
	#ifdef DEBUG_ON
	printf("DEBUG > IAPC8_Compiler.c : precompile() : Step 1 done successfully.\n");
	#endif



	//preparation for step 2
	char* code_step2 = mallocStr(PRECOMP_CODE_LENGTH);
	int code_step2_index = 0;
	int returnMode = 1;

	//STEP 2
	//remove double line returns
	for(int c=1; c < code_step1_index; c++){
		//code interpretation
		if(code_step1[c] != '\n'){
			code_step2[code_step2_index++] = code_step1[c];
			returnMode = 0;
		}else{
			if(!returnMode){
				code_step2[code_step2_index++] = '\n';
				returnMode = 1;
			}
		}
	}
	#ifdef DEBUG_ON
	printf("DEBUG > IAPC8_Compiler.c : precompile() : Step 2 done successfully.\n");
	#endif

	//preparation for step 3
	char* code_step3 = mallocStr(PRECOMP_CODE_LENGTH);
	int lineIndex = 0; //index of the begining of the current line
	#ifdef DEBUG_ON
	int lineNbr = 1;
	#endif



	//STEP 3
	//format arguments in 9 characters (1 address and 8 bits)
	for(int c=0; c < code_step2_index; c++){
		//line by line (if reach end of current line)
		if(code_step2[c] == '\n'){

			#ifdef DEBUG_ON
			//print current line
			printf("DEBUG > IAPC8_Compiler.c : precompile() : Precompiling line %i : ", lineNbr++);
			for(int a=lineIndex; a <= c; a++)
				printf("%c",code_step2[a]);
			#endif

			//risk of core dumped if code is too long (if it occurs, increase PRECOMP_CODE_LENGTH)
			strcat(code_step3,"\n");
			addFormatedLine(
				code_step3, //line starts at code_step2[lineIndex] and the formated line will be added to code_step3
				code_step2,
				lineIndex
			);
			lineIndex = c+1;
		}
	}
	#ifdef DEBUG_ON
	printf("DEBUG > IAPC8_Compiler.c : precompile() : Step 3 done successfully.\n");
	#endif



	//shorten precompiled code string
	int code_step3_len = strlen(code_step3);
	char* returnCode = mallocStr(code_step3_len-1); //ignore the first character : this is a '\n'
	strcpy(returnCode,code_step3+1);
	freeStr(code_step1, PRECOMP_CODE_LENGTH);
	freeStr(code_step2, PRECOMP_CODE_LENGTH);
	freeStr(code_step3, PRECOMP_CODE_LENGTH);

	printf("Compiler > IAPC8_Compiler.c : precompile() : Precompilation done.\n");

	return returnCode;
}



char* compile(char* code){ //code will be set free (memory improvement)
	//precompile
	char* precomp = precompile(code);
	int precomp_len = strlen(precomp);

	//error cases
	if(precomp == NULL){
		printf("RUNTIME ERROR > IAPC8_Compiler.c : compile() : Unable to read source code\n");
		return NULL;
	}

	//prepare compilation
	char* compCode = mallocStr(COMP_CODE_LENGTH);
	int compCode_index = 0;
	lineNbr = 0;

	//command matching
	for(int c=0; c <= precomp_len; c++){
		//line by line
		if(precomp[c] == '\n' || precomp[c] == '\0'){
			lineNbr++;

			//format check
			if(c-compCode_index != 21){
				printf("FATAL ERROR > IAPC8_Compiler.c : compile() : Incorrect line format at line %i (21 characters needed, %i found)\n", lineNbr, c-compCode_index);
				exit(EXIT_FAILURE);
			}

			#ifdef DEBUG_ON
			printf("DEBUG > IAPC8_Compiler.c : compile() : Compiling line %i : \"", lineNbr);
			printOnN(precomp+compCode_index, 21);
			printf("\"\n");
			#endif

			compileCommand(precomp+compCode_index, compCode);
			compCode_index = c+1;
		}
	}

	//free raw and precompiled code
	freeStr(code, strlen(code));
	freeStr(precomp, precomp_len);

	//shorten compiled code string
	int compCode_len = strlen(compCode);
	char* returnCode = mallocStr(compCode_len);
	strcpy(returnCode,compCode);
	freeStr(compCode, COMP_CODE_LENGTH);

	printf("Compiler > IAPC8_Compiler.c : compile() : Compilation done.\n");

	return returnCode;
}
