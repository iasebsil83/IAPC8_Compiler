/* ================================ IAPC8 Compiler [0.1.0] ================================

Version details :

0.1.0 >   Precompilation steps 1,2 and 3 :
          Basic text formating into fixed number of characters per line : 
                CMDpXXXXXXXXpXXXXXXXX
          CMD : iassembly command
          p   : prefix (a,p or r)
          X   : 0 or 1
        - Creation of IAPC8_MCInterface library including IAPC8 system
          coordinates and getMinecraftCmd functions

0.1.2 > Compilation

================================================================================ */






// ----------------- Importations ----------------
//standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"
#include "IAPC8_Compiler.h"






// ----------------- Declarations ----------------
// ---- Compiler variables ----
#define DEBUG_ON //comment to disable debug messages

// ---- Compiler constants ----
#define PRECOMP_CODE_LENGTH 4000
#define COMP_CODE_LENGTH 2000
const char* STR_TEXT = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\n";






// ---------------- Precompiler functions ----------------
void addFormatedLine(char* dest, char* source, int source_index){
	//get destination index
	int dest_index = strlen(dest);

	//command
	dest[dest_index++] = source[source_index++];
	dest[dest_index++] = source[source_index++];
	dest[dest_index++] = source[source_index++];

	//arg1
	//set prefix
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
		dest_index += 8;
		//convert value string to binary string
		switch(source[source_index++]){
			case 'x':
				strcat(dest, hex24bin(source[source_index++]) );
				strcat(dest, hex24bin(source[source_index++]) );
			break;
			case 'd':
				strcat(dest, dec28bin(source+source_index) );
				source_index += 3;
			break;
			default: //already in binary format
				source_index--;
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
		}
	}

	//arg2
	//set prefix
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
		switch(source[source_index++]){
			case 'x':
				strcat(dest, hex24bin(source[source_index++]) );
				strcat(dest, hex24bin(source[source_index]) );
			break;
			case 'd':
				strcat(dest, dec28bin(source+source_index) );
			break;
			default: //already in binary format
				source_index--;
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index++] = source[source_index++];
				dest[dest_index++] = source[source_index++]; dest[dest_index] = source[source_index];
		}
	}
}






// ---------------- Execution ----------------
char* precompile(char* text){
	if(text == NULL){
		printf("RUNTIME ERROR : IAPC8_Compiler.c : precompile() : Unable to read source code\n");
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
			printf("RUNTIME ERROR : IAPC8_Compiler.c : precompile() : Source code is too long\n");
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
	printf("MESSAGE : IAPC8_Compiler.c : precompile() : Step 1 done successfully.\n");
	#endif



	//preparation for step 2
	char* code_step2 = mallocStr(PRECOMP_CODE_LENGTH);
	int code_step2_index = 0;
	int returnMode = 0;

	//STEP 2
	//remove double line returns
	for(int c=0; c < code_step1_index; c++){
		//code interpretation
		if(code_step1[c] != '\n'){
			code_step2[code_step2_index++] = code_step1[c];
			returnMode = 0;
		}else{
			if(!returnMode)
				code_step2[code_step2_index++] = '\n';
			returnMode = 1;
		}
	}
	#ifdef DEBUG_ON
	printf("MESSAGE : IAPC8_Compiler.c : precompile() : Step 2 done successfully.\n");
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
			printf("MESSAGE : IAPC8_Compiler.c : precompile() : Precompiling line %i :\n", lineNbr++);
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
	printf("MESSAGE : IAPC8_Compiler.c : precompile() : Step 3 done successfully.\n");
	#endif



	//shorten precompiled code string
	int code_step3_len = strlen(code_step3);
	char* returnCode= mallocStr(code_step3_len);
	strcpy(returnCode,code_step3);
	freeStr(code_step1);
	freeStr(code_step2);
	freeStr(code_step3);

	return returnCode;
}
