// ---------------- Importations ----------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"






// ---------------- String functions ----------------
char* mallocStr(int len){
	if(len <= 0){
		printf("RUNTIME ERROR > IAPC8_Useful.c : mallocStr() : Cannot allocate less than 1 character\n");
		return NULL;
	}

	//allocation
	char* returnStr = malloc(len+1);
	if(returnStr == NULL){
		printf("FATAL ERROR > IAPC8_Useful.c : mallocStr() : Computer refused to give more memory\n");
		exit(EXIT_FAILURE);
	}

	//string initialization
	for(int a=0; a <= len; a++)
		returnStr[a] = '\0';

	return returnStr;
}

void freeStr(char* str, int len){
	for(int a=0; a <= len; a++)
		str[a] = '\0';
	free(str);
}

int charInStr(char c, const char* str){
	for(int a=0; a < strlen(str); a++)
		if(c == str[a])
			return 1;
	return 0;
}

int strcmpN(char* s1, char* s2, int len){ //compare the len first characters
	//error cases
	if(len <= 0){
		printf("RUNTIME ERROR > IAPC8_Useful.c : strcmpN() : Incorrect length %i given to compare strings \"%s\" and \"%s\"\n", len, s1,s2);
		return 0;
	}

	//compare each character
	for(int a=0; a < len; a++){
		if(s1[a] != s2[a]) //problem found
			return 0;
	}
	return 1;
}

void strcatExe(char* dest, char* pc_var, char* arg){
	//error cases
	if(dest == NULL){
		printf("RUNTIME ERROR > IAPC8_Useful.c : strcatExe() : Destination string is NULL.");
		return;
	}
	if(strlen(pc_var) != 2 || strlen(arg) < 8){
		printf("RUNTIME ERROR > IAPC8_Useful.c : strcatExe() : Command line is not correct : \"%s%8s\".", pc_var, arg);
		return;
	}

	//concatenate
	strcat(dest, pc_var);
	int dest_len = strlen(dest);
	dest[dest_len++] = arg[0]; dest[dest_len++] = arg[1];
	dest[dest_len++] = arg[2]; dest[dest_len++] = arg[3];
	dest[dest_len++] = arg[4]; dest[dest_len++] = arg[5];
	dest[dest_len++] = arg[6]; dest[dest_len++] = arg[7];
	dest[dest_len  ] = '\n';
}

void printOnN(char* str, int len){
	//error cases
	if(strlen(str) < len)
		printf("\nRUNTIME ERROR > IAPC8_Useful.c : printOnN() : String to print is too short\n");

	//print
	for(int i=0; i < len; i++)
		printf("%c", str[i]);
}






// ---------------- Conversions ----------------
int char2int(char c){
	switch(c){
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
	}

	//integer equivalence not found
	printf("FATAL ERROR > IAPC8_Useful.c : char2int() : Incorrect decimal number \"%c\"\n",c);
	exit(EXIT_FAILURE);
	return 0; //will never be executed
}

char* hex24bin(char h){
	switch(h){
		case 'F':
		case 'f':
			return "1111";
		case 'E':
		case 'e':
			return "1110";
		case 'D':
		case 'd':
			return "1101";
		case 'C':
		case 'c':
			return "1100";
		case 'B':
		case 'b':
			return "1011";
		case 'A':
		case 'a':
			return "1010";
		case '9':
			return "1001";
		case '8':
			return "1000";
		case '7':
			return "0111";
		case '6':
			return "0110";
		case '5':
			return "0101";
		case '4':
			return "0100";
		case '3':
			return "0011";
		case '2':
			return "0010";
		case '1':
			return "0001";
		case '0':
			return "0000";
	}

	//hexadecimal equivalence not found
	printf("FATAL ERROR > IAPC8_Useful.c : hex24bin() : Incorrect hexadecimal number \"%c\" (must be written on 2 digits)\n",h);
	exit(EXIT_FAILURE);
	return "0000"; //will never be executed
}

char* dec28bin(char* d){
	//decimal equivalence not found
	if(strlen(d) < 3){
		printf("FATAL ERROR > IAPC8_Useful.c : dec28bin() : Incorrect decimal number \"%s\" (must be written on 3 digits)\n",d);
		exit(EXIT_FAILURE);
	}

	//convert to int
	int i = char2int(d[0])*100 + char2int(d[1])*10 + char2int(d[2]);
	if(i < 0 || i > 255){
		printf("FATAL ERROR > IAPC8_Useful.c : dec28bin() : Decimal number is out of range : %i (must be between 0 and 255 included)\n",i);
		exit(EXIT_FAILURE);
	}

	//convert to hex string
	char* hexStr = mallocStr(2);
	sprintf(hexStr,"%x",i);

	//convert to bin string
	char* binStr = mallocStr(8);
	if(i > 15){
		sprintf(binStr,"%4s%4s", hex24bin(hexStr[0]), hex24bin(hexStr[1]) );
	}else
		sprintf(binStr,"0000%4s", hex24bin(hexStr[0]) );
	freeStr(hexStr,2);

	return binStr;
}

int _8bin2dec(char* b){
	//error cases
	if(strlen(b) < 8){
		printf("FATAL ERROR > IAPC8_Useful.c : _8bin2dec() : Incorrect binary number \"%s\" (must be written on 8 bits)\n",b);
		exit(EXIT_FAILURE);
	}
	if(
		(b[0] != '1' && b[0] != '0') ||
		(b[1] != '1' && b[1] != '0') ||
		(b[2] != '1' && b[2] != '0') ||
		(b[3] != '1' && b[3] != '0') ||
		(b[4] != '1' && b[4] != '0') ||
		(b[5] != '1' && b[5] != '0') ||
		(b[6] != '1' && b[6] != '0') ||
		(b[7] != '1' && b[7] != '0')
	){
		printf("FATAL ERROR > IAPC8_Useful.c : _8bin2dec() : Incorrect binary number \"%s\"\n",b);
		exit(EXIT_FAILURE);
	}

	//conversion
	return
		128*char2int(b[0]) +
		 64*char2int(b[1]) +
		 32*char2int(b[2]) +
		 16*char2int(b[3]) +
		  8*char2int(b[4]) +
		  4*char2int(b[5]) +
		  2*char2int(b[6]) +
		    char2int(b[7])
	;
}






// ---------------- File management ----------------
char* readFromFile(char* fileName){
	//incorrect file name
	if(fileName == NULL){
		printf("RUNTIME ERROR > IAPC8_Useful.c : readFromFile() : Incorrect file name\n");
		return NULL;
	}

	//opening file
	FILE* f = fopen(fileName,"r");
	if(f == NULL){
		printf("RUNTIME ERROR > IAPC8_Useful.c : readFromFile() : No file \"%s\" found\n",fileName);
		return NULL;
	}

	//get file length
	fseek(f, 0L, SEEK_END);
	int file_len = ftell(f);

	//read from file
	fseek(f, 0L, SEEK_SET); //<=> rewind(f);
	char* code = mallocStr(file_len);
	fscanf(f,"%[^!]",code);
	fclose(f);

	return code;
}

void writeToFile(char* fileName, char* code){
	//error cases
	if(strlen(fileName) <= 0){
		printf("RUNTIME ERROR > IAPC8_Useful.c : writeToFile() : Incorrect file name\n");
		return;
	}
	if(code == NULL){
		printf("RUNTIME ERROR > IAPC8_Useful.c : writeToFile() : Incorrect text to write in \"%s\"\n",fileName);
		return;
	}

	//write into file
	FILE* f = fopen(fileName,"w");
	if(f == NULL){
		printf("RUNTIME ERROR > IAPC8_Useful.c : writeToFile() : Unable to create file \"%s\"\n",fileName);
		return;
	}
	fprintf(f,"%s",code);
	fflush(f);
	fclose(f);
}
