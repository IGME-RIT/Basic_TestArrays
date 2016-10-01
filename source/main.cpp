/*
TestArrays
(c) 2016
original authors: David I. Schwartz
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

This tutorial illustrates the memory allocation, array initialization, 
application of function GlobalMemoryStatusEx.

if the output shows "cmd.exe was started with the above path as the current directory.
unc paths are not supported.defaulting to windows directory."
try to put the project file other locations 
*/

#pragma region Setup
#include <Windows.h>
#include <iostream>
using namespace std;

#pragma region Leak Detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG
#pragma endregion

// Handy defs/constants for below:
typedef unsigned int uint;
const int ARRAY_SIZE = 3;

// function prototypes:
void print(int a[], uint size);
void fillArray(char* a, uint size);
void showSystemMemory();
#pragma endregion

int main() {
    
    // static vs dynamic arrays? see http://stackoverflow.com/questions/2672085/c-static-array-vs-dynamic-array

    cout << "A: " << endl;	// output: A:
    int a[ARRAY_SIZE];      // stack allocated array
    //cout << a[0] << endl; // aborts if you try to access...nothing inside a[...] yet
    for (int i=0 ; i < ARRAY_SIZE ; i++)
        a[i] = i;			
    print(a,ARRAY_SIZE);	// output: 0 1 2

    cout << "B: " << endl;	// output: B:
    int b[] = {1,2,3};      // initializer! http://msdn.microsoft.com/en-us/library/sdwe79a4.aspx
    print(b,ARRAY_SIZE);	// output: 1 2 3

    cout << "C: " << endl;	// output: C:
    int c[3] = {1};         // everything after 1st element is zero
    print(c,ARRAY_SIZE);	// output: 1 0 0

    cout << "D: " << endl;	// output: D:
    // char[] d = new char[ARRAY_SIZE];  // no Java or C# syntax
    // char d[] = new char[4];           // no Java or C# syntax
    // char d[] = new char[ARRAY_SIZE];  // no Java or C# syntax
    char* d = new char[ARRAY_SIZE];      // works because new returns address of newly created array
    delete d;

    cout << "\nBefore array: " << endl;	// output:			Before array:
    showSystemMemory();					// output (random): 10268
										//					2029
    char* e = new char[1048576]; // 1 MB of chars; sizeof(char)= 1 Byte heap allocated array
    fillArray(e,1048576);		 // call function fillArray(); pass argument pointer "e" and unsigned int 1048576 to function fillArray()

    cout << "\nBefore delete: " << endl;	// output: Before delete: 
    showSystemMemory();			// call function showSystemMemory();  output (random): 10266
								//													   2028
    delete e;					// deallocate the dynamic memory assigned by e.

    cout << "\nAfter delete: " << endl;	// output: After delete:
    showSystemMemory();					// output (random): 10267
										//					2029

	// Here are some notes about memory distribution in C++ program: 
	// stack: stores automatic variables
	// heap: one of two dynamic memory, allocated by malloc, freed by calloc (terminology from C language)
	// free store: the other dynamic memory, allocated by new, freed by delete
	// static/global: different area for variables that are uninitialized and initialized
	// see more: http://www.gotw.ca/gotw/009.htm

   _CrtDumpMemoryLeaks();
   return 0;
}

#pragma region Utility Functions

void print(int a[], uint size) {
    for (uint i=0; i < size;i++)
        cout << a[i] << ' ';	// print out array a
    cout << endl;
}

void fillArray(char* a, uint size) {
    for (uint i=0; i < size;i++)
        a[i] = ' ';				// assign ' ' to every members in array
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/aa366770%28v=vs.85%29.aspx
void showSystemMemory() {
    MEMORYSTATUSEX status; 
    status.dwLength = sizeof(status); // the size of the structure, in bytes
    GlobalMemoryStatusEx(&status); 
	// GlobalMemoryStatusEX function: 
	// Retrives information about the system's current usage of both physical and virtual memory
	

    // Skipping the variety of ways to check memory:
    //cout << status.dwLength << endl;
    //cout << status.dwMemoryLoad << endl;
    //cout << status.ullAvailExtendedVirtual << endl;
    //cout << status.ullTotalPageFile << endl;
    //cout << status.ullTotalPhys << endl;
    //cout << status.ullTotalVirtual << endl;
    //cout << status.ullAvailPhys/1048576 << endl;
    
    // Focus on 2 ways to check what happens before & after memory allocations/deletions:
    cout << status.ullAvailPageFile/1048576 << endl;   // 1048576 = 1024 * 1024 or 1 MB. The maximum amount of memory the current process can commit, in MB
    cout << status.ullAvailVirtual/1048576 << endl;    // The amount of unreserved and uncommitted memory currently in the user-mode portion 
													   // of the virtual address space of the calling process, in MB.

	
}
#pragma endregion