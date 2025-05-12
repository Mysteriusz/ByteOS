
void* memset(void* ptr, int val, unsigned int n){
    unsigned char* p = ptr;
    while (n--) *p++ = (unsigned char)val;
    return ptr;
}
void* memcpy(void* dest, const void* src, unsigned int n){
	unsigned char* dp = (unsigned char*)dest;
	const unsigned char* sp = (const unsigned char*)src;

	for (unsigned int i = 0; i < n; i++) {
		dp[i] = sp[i];
	}
	return dest;
}
int memcmp(const void* ptr1, const void* ptr2, unsigned int n) {
	const unsigned char* p1 = (const unsigned char*)ptr1;
	const unsigned char* p2 = (const unsigned char*)ptr2;

	for (unsigned int i = 0; i < n; i++, p1++, p2++) {
		if (*p1 < *p2) return -1;
		if (*p1 > *p2) return 1;
	}

	return 0;
}
