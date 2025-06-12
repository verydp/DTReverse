#ifndef PE_POINTER_H
#define PE_POINTER_H




//PEpointerº¯Êý

int* ptre_lfanew(char* fbuffer) {}

char* ptrPE(char* fbuffer) {}

char* ptrOptionPE(char* fbuffer) {}

int* ptrSizeOfImage(char* fbuffer) {}

int* ptrSizeOfHeaders(char* fbuffer) {}

short* ptrSizeOfOptionalHeader(char* fbuffer) {}

char* ptrSection(char* fbuffer) {}

int SectionOffset(char* fbuffer) {}

int* ptrVirtualAddress(char* ptrSection) {}

int* ptrSizeOfRawData(char* ptrSection) {}

int* ptrPointerToRawData(char* ptrSection) {}

int* ptrCharacteristics(char* ptrSection) {}

short* ptrNumberOfSection(char* fbuffer) {}

int* ptrImageBase(char* fbuffer) {}
#endif // !PE_POINTER
