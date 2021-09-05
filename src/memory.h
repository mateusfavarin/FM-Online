#include <string>
#include <Windows.h>

using std::string;

void ReadMem(uintptr_t psxAddr, void * pcAddr, int size);
void WriteMem(uintptr_t psxAddr, void * pcAddr, int size);
void GetPSXRAM(uintptr_t start, int size);
void FindBaseAddress(char * szModuleName);
bool GetProcessSettings(string windowName, char * szModuleName);