#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <cstring>
using namespace std;


extern "C"
{

	void  outnum(int value)
	{
		std::cout << value;

	}
	void outnumln(int value)
	{
		std::cout << value<<endl;
		
	}

	void outstr(char* ptr)
	{
		setlocale(0, "");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
		{
			std::cout << ptr[i];
		}
			

	}

	void outstrln(char* ptr)
	{
		setlocale(0, "");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		cout << endl;
	}

	void  outchar(char* ptr) {
		setlocale(0, "");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
	}

	void  outcharln(char* ptr) {
		setlocale(0, "");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		cout << endl;
	}


	char* concat(const char* ptr, const char* ptr2) {
		char result[100];
		strcpy(result, ptr);
		strcat(result, ptr2); 
		return result;
	}

	char* strcp(char* ptr1, char* ptr2) {
		return ptr2;
	}
}
