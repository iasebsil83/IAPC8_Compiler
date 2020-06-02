// ---------------- Importations ----------------
#include <stdio.h>
#include <stdlib.h>
#include "IAPC8_Useful.h"
#include "IAPC8_Compiler.h"






// ---------------- Declarations ----------------
#define FILENAME_LENGTH 40






// ---------------- Execution ----------------
int main(){
	//source file management
	char* fileName = mallocStr(FILENAME_LENGTH);
	char* code;
	char* precompCode;



	//get raw code
	do{
		printf("Enter the name of the file to compile > \n");
		scanf("%s", fileName);
		code = readFromFile(fileName);
	}while(code == NULL);



	//precompilation
	precompCode = precompile(code);



	//exportation
	writeToFile(fileName, precompCode);



	//free all program data
	freeStr(fileName);
	freeStr(code);
	freeStr(precompCode);

	return 0;
}
