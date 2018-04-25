/*
#############################################################################################
# Rusty Hearts Trainer v1										
# ========================================================================================= #
# File: main.cpp																		
# ========================================================================================= #
# Credits: R4i,xxlilloxx			
# Thanks: b-rabbit															
# 											
# ========================================================================================= #
# This work is licensed under the															
# Ergasm Creative Commons Corporation inc.																
############################################################################################# 
*/


#include <tchar.h>
#include <windows.h> 
#include <TlHelp32.h> 
#include <iostream> 
#include <stdio.h>

using namespace std; 

DWORD dwGetModuleBaseAddress(DWORD dwProcessIdentifier, TCHAR *lpszModuleName) 
{ 
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessIdentifier); 
   DWORD dwModuleBaseAddress = 0; 
   if(hSnapshot != INVALID_HANDLE_VALUE) 
   { 
      MODULEENTRY32 ModuleEntry32 = {0}; 
      ModuleEntry32.dwSize = sizeof(MODULEENTRY32); 
      if(Module32First(hSnapshot, &ModuleEntry32)) 
      { 
         do 
         { 
            if(_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) 
            { 
               dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr; 
               break; 
            } 
         } 
         while(Module32Next(hSnapshot, &ModuleEntry32)); 
      } 
      CloseHandle(hSnapshot); 
   } 
   return dwModuleBaseAddress; 
} 

int main() 
{ 
   HWND window = FindWindow(0, _T("Rusty Hearts Online ")); 
   if( window == 0 ){ 
      printf("Window not found!\n"); 
     std::cin.get();
      return 0; 
   } 

  
   printf("\# ======================= #\n\
# Credits: R4i,xxlilloxx\n\
# Thanks: b-rabbit\n\
#======================== #\n\
# This work is licensed under the\n\
# Ergasm Creative Commons Corporation inc.\n\
###################################\n\n\nUtilizzo:Right Shift disattiva\n\t Right Shift+Invio riattiva\n\t Invio esci dal programma\n\n\n");

   DWORD pID = 0; 
   GetWindowThreadProcessId(window, &pID); 
    
   DWORD baseAddr = dwGetModuleBaseAddress(pID, _T("rustyhearts.exe")); 
   DWORD staticOffset = 0x00CA8FB8; 
    
   HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, 0, pID);
   
   

   DWORD constphysDefense=20000000; 
   DWORD numBytesRead;
   DWORD value;
   DWORD addressPhysD;
   DWORD physDefense;
   
   ReadProcessMemory(handle, (LPCVOID)(baseAddr+staticOffset), &value, sizeof(DWORD), &numBytesRead); //1 catena legge il valore all'indirizzo baseAddr+static pointer 

   value+=0x44; //somma il valore letto (indirizzo di memoria) l'offset 0x13c

   ReadProcessMemory(handle, (LPCVOID)value, &value, sizeof(DWORD), &numBytesRead); // 2 catena lettura all' indirizzo prelevato+offset

   value+=0x41c; //Points offset , somma il valore letto dalla RPM l'offset 0x41c

   addressPhysD=value; //value ora contiene indirizzo nel quale si trova il valore da sostituire

   ReadProcessMemory(handle, (LPCVOID)value, &value, sizeof(DWORD), &numBytesRead); //questa read legge il valore contenuto all'indirizzo prelevato in precedenza e lo inserisce in value (può essere omessa)
  
   WriteProcessMemory(handle, (LPVOID)addressPhysD, &constphysDefense, (DWORD)sizeof(constphysDefense), NULL); //scrittura valore
   
   printf("Valore trovato: %d\n\nRiscrittura eseguita...\n\n", value);
   do
   {
   while(true){
	   if(ReadProcessMemory(handle, (LPCVOID)addressPhysD, &physDefense, sizeof(DWORD), &numBytesRead) && GetAsyncKeyState(VK_RSHIFT)==0) //lettura valore scritto e controllo pressione right-shift
		{  
			switch(physDefense)
			{
				case(20000000):
				break;

			default:
				WriteProcessMemory(handle, (LPVOID)addressPhysD, &constphysDefense, (DWORD)sizeof(constphysDefense), NULL);
				printf("Valore reimpostato\n");
				break;
			}
			
		}
		else
			break;
	}
 
	printf("Trainer Disattivato\n\n");
	WriteProcessMemory(handle, (LPVOID)addressPhysD, &value, (DWORD)sizeof(constphysDefense), NULL);
	ReadProcessMemory(handle, (LPCVOID)addressPhysD, &value, sizeof(DWORD), &numBytesRead);
	std::cin.get();
   }while(GetAsyncKeyState(VK_RSHIFT)==1);
	CloseHandle(handle); 
	printf("Valore default: %d\n\nSwitch arma per reset\n\n", value);
	std::cin.get();
}