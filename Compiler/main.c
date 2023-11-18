// ---------------- Importations ----------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IAPC8_Useful.h"
#include "IAPC8_Compiler.h"
#include "IAPC8_MCInterface.h"






// ---------------- Declarations ----------------
#define FILENAME_LENGTH 80
#define INPUT_LENGTH    FILENAME_LENGTH-12






// ---------------- Execution ----------------
int main(){
	//files management
	char* source_fileName = mallocStr(FILENAME_LENGTH);
	char* dest1_fileName  = mallocStr(FILENAME_LENGTH);
	char* dest2_fileName  = mallocStr(FILENAME_LENGTH);



	//text codes
	char* rawCode;    //.iasm
	char* compCode;   //.iax
	char* MCcompCode; //.iamc
	char* MCgatheredCode;



	//get raw code
	char* input = mallocStr(INPUT_LENGTH);
	do{
		printf("Enter the name of the file to compile > ");
		scanf("%s", input);
		sprintf(source_fileName, "1_IASM/%s.iasm", input);
		rawCode = readFromFile(source_fileName);
	}while(rawCode == NULL);



	//compilation
	compCode = compile(rawCode);
	sprintf(dest1_fileName, "2_IAX/%s.iax", input);
	writeToFile(dest1_fileName, compCode);



	//MC interfacing + gathering
	MCcompCode = MCtranslate(compCode);
	MCgatheredCode = MCgather(MCcompCode);
	sprintf(dest2_fileName, "3_IAMC/%s.iamc", input);
	writeToFile(dest2_fileName, MCgatheredCode);

	//free main allocated strings
	freeStr(source_fileName, FILENAME_LENGTH);
	freeStr(dest1_fileName, FILENAME_LENGTH);
	freeStr(dest2_fileName, FILENAME_LENGTH);

	return EXIT_SUCCESS;
}
