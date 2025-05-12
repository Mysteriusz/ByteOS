#pragma once

#define OFFSET_OF(str, mem) ((unsigned long long)&((str*)0)->mem)

void* memset(void* ptr, int val, unsigned int n);
void* memcpy(void* dest, const void* src, unsigned int n);
int memcmp(const void* ptr1, const void* ptr2, unsigned int n);
