#executable
exe: main.o IAPC8_Useful.o IAPC8_Compiler.o IAPC8_MCInterface.o
	gcc -Wall -o ../run main.o IAPC8_Useful.o IAPC8_Compiler.o IAPC8_MCInterface.o
	rm -f *.o



#object files
main.o: main.c IAPC8_Compiler.h IAPC8_MCInterface.h IAPC8_Useful.h
	gcc -c -Wall main.c

IAPC8_MCInterface.o: IAPC8_MCInterface.c IAPC8_MCInterface.h IAPC8_Useful.h
	gcc -c -Wall IAPC8_MCInterface.c

IAPC8_Compiler.o: IAPC8_Compiler.c IAPC8_Compiler.h IAPC8_Useful.h
	gcc -c -Wall IAPC8_Compiler.c

IAPC8_Useful.o: IAPC8_Useful.c IAPC8_Useful.h
	gcc -c -Wall IAPC8_Useful.c

