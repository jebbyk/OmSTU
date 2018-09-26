#include <windows.h>
#include <stdio.h>

//realy it's a crutch
void Wait(HANDLE out)
{
	int a;
	DWORD actLength;
	WriteFile(out, "\npress any key and enter to end\n", 31, &actLength, NULL );
	scanf("%d", &a); // whait for input, then close
	exit(0);
}

void main()
{
	int run = 1;
	while(run == 1)
	{

		char buffer[128];
		HANDLE hstdin, hstdout, hstderror, outFileHandle, inputFileHandle;
		DWORD length, actLength;
	

		//get handles numbers
		hstdin = GetStdHandle(STD_INPUT_HANDLE);
		hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		hstderror = GetStdHandle(STD_ERROR_HANDLE);
		printf("\nInput handle - %d ", hstdin);
		printf("\nOutput handle - %d ", hstdout);
		printf("\nError handle - %d ", hstderror);
		fflush(hstdout);

		//open files to work with ones
		outFileHandle = CreateFile("output.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		inputFileHandle = CreateFile("input.txt", GENERIC_READ, 0, 0, OPEN_ALWAYS, 0, 0);
		if(outFileHandle ==  INVALID_HANDLE_VALUE)
		{
			WriteFile(hstdout, "\n Error creating output file \n", 30, &actLength, NULL );
			Wait(hstdout);
		}
		if(inputFileHandle ==  INVALID_HANDLE_VALUE)
		{
			WriteFile(hstdout, "\n Error creating input file \n", 29, &actLength, NULL);
			Wait(hstdout);
		}

		//some kind of controlling buffer
		char c;
		WriteFile(hstdout, "\n1 - read from keyboard, 0 - read from file\n", 44, &actLength, NULL);
		c = getchar();
		
		//read from keyboard
		if(c == '1')
		{
			WriteFile(hstdout, "\nEnter your text\n",17, &actLength, NULL);
			getchar();
			ReadFile(hstdin, buffer, 128, &length, NULL);
			WriteFile(hstdout, "\nNow this text in a buffer: ", 28, &actLength, NULL);
			WriteFile(hstdout, buffer, length, &actLength, NULL);
			WriteFile(hstdout, "\n1 - write to file, 0 - do nothing\n", 35, &actLength, NULL);
		}
		//read from file
		if(c == '0')
		{
			ReadFile(inputFileHandle, buffer, 128, &length, NULL);
			WriteFile(hstdout, "\nText was readed from file", 26, &actLength, NULL);
			//WriteFile(hstdout, buffer, length, &actLength, NULL);
			WriteFile(hstdout, "\n1 - write to file, 0 - do nothing\n", 35, &actLength, NULL);
			getchar();
		}
		//some controls again
		c = getchar();
		//if 1 - write it to file, else do nothing
		if(c == '1') 
		{
			WriteFile(outFileHandle, "\nFrom program: ", 15, &actLength, NULL);
			WriteFile(outFileHandle, buffer, length, &actLength, NULL);
			CloseHandle(outFileHandle);
			CloseHandle(inputFileHandle);
			WriteFile(hstdout, "\nNow this text in a file lab1out.txt:", 37, &actLength, NULL);
		}
		//some kind of crutch i gues (see the begining of the program)
		Wait(hstdout);	
	}
	return;
}