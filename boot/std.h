#pragma once

#define NORETURN _Noreturn 
#define STDCALL _stdcall 
#define CDECL _cdecl 
#define FASTCALL _fastcall 

#define OFFSET_OF(str, mem) ((unsigned long long)&((str*)0)->mem)
#define OFFSET_AT(ptr, offset) (((unsigned long long)ptr) + offset) 

void* memset(void* ptr, int val, unsigned int n);
void* memcpy(void* dest, const void* src, unsigned int n);
int memcmp(const void* ptr1, const void* ptr2, unsigned int n);


int root(int a, int b);
int log(int a, int b);
int pow(int a, int b);
int abs(int a);

int min(int a, int b);
int max(int a, int b);
