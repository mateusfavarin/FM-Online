#include <string>
#include <Windows.h>
#include <TlHelp32.h>

using std::string;

DWORD procID;
HANDLE handle;
uintptr_t baseAddress = 0;

void ReadMem(uintptr_t psxAddr, void * pcAddr, int size) {
  ReadProcessMemory(handle, (PBYTE *) (baseAddress + psxAddr), pcAddr, size, 0);
}

void WriteMem(uintptr_t psxAddr, void * pcAddr, int size) {
  WriteProcessMemory(handle, (PBYTE *) (baseAddress + psxAddr), pcAddr, size, 0);
}

void GetPSXRAM(uintptr_t start, int size)
{
  char * mednafen = new char[size];
  const char gameExe[] = "SLUS_014.11;1";
  const int gameExePSXOffset = 0x9244;
  ReadMem(start, &mednafen[0], size);
  for (int i = 0; i < size; i++)
  {
    if (!memcmp(&gameExe[0], &mednafen[i], sizeof(gameExe)))
    {
      baseAddress = start + i - gameExePSXOffset;
      break;
    }
  }
  delete[] mednafen;
}

// Function copied from https://stackoverflow.com/questions/48208782/read-memory-of-process-c
void FindBaseAddress(char* szModuleName) {
  uintptr_t ModuleBaseAddress = 0;
  int ModuleSize = 0;
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
  if (hSnapshot != INVALID_HANDLE_VALUE)
  {
    MODULEENTRY32 ModuleEntry32;
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnapshot, &ModuleEntry32))
    {
      do
      {
        if (strcmp(ModuleEntry32.szModule, szModuleName) == 0)
        {
          ModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
          ModuleSize = (int)ModuleEntry32.modBaseSize;
          break;
        }
      } while (Module32Next(hSnapshot, &ModuleEntry32));
    }
    CloseHandle(hSnapshot);
  }
  GetPSXRAM(ModuleBaseAddress, ModuleSize);
}

bool GetProcessSettings(string windowName, char * szModuleName)
{
  HWND hwnd = FindWindow(NULL, windowName.c_str());
  GetWindowThreadProcessId(hwnd, &procID);
  handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
  if (!handle)
    return false;

  FindBaseAddress(szModuleName);
  return true;
}