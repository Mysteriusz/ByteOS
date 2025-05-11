
void* memset(void* s, int c, unsigned long n) {
    unsigned char* p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}
void memcpy(void* dest, const void* src, unsigned int n) {
	char* dPtr = (char*)dest;
	const char* sPtr = (const char*)src;
	for (unsigned int i = 0; i < n; i++) {
		dPtr[i] = sPtr[i];
	}
	return;
}
