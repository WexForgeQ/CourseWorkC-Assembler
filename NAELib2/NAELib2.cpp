#include <iostream>
#include <Windows.h>
#include <cstring>
using namespace std;


extern "C"
{
	void outnum(int a) {
		std::cout << a;
	}


	void outstr(string ptr) {
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		if (ptr == "") {
			cout << endl;
			return;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			cout << ptr[i];
	}

	void outchar(char ptr) {
		cout << ptr;
	}


	string concat(string ptr, string ptr2) {
		return ptr + ptr2;
	}

	string strcp(string ptr, string ptr2) {
		ptr = ptr2;
		return ptr;
	}
}
	