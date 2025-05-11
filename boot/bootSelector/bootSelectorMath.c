#include "bootSelectorMath.h"

UINT32 root(IN UINT32 a, IN UINT32 b) {
	UINT32 x = a / b;
	UINT32 y;

	while (1) {
		y = (x + a / x) / b;
		if (x <= y) break;
		x = y;
	}

	return x;
}
UINT32 log(IN UINT32 a, IN UINT32 b) {
	if (a <= 1 || b == 0) return 0;

	UINT32 x = 0;
	while (a >= b) {
		a /= b;
		x++;
	}

	return x;
}
UINT32 pow(IN UINT32 a, IN UINT32 b) {
	if (b == 0) return 1;
		
	UINT32 x = a;

	while (--b > 0) x *= a;

	return x;
}
UINT32 abs(IN INT32 a) {
	return (a < 0) ? -a : a;
}

UINT32 min(IN UINT32 a, IN UINT32 b){
	return (a < b) ? a : b;
}
UINT32 max(IN UINT32 a, IN UINT32 b) {
	return (a < b) ? b : a;
}

UINT16* UInt32ToChar16(UINT32 i) {
	static UINT16 buffer[11];

	if (i == 0) {
		buffer[0] = u'0';
		buffer[1] = u'\0';
		return buffer;
	}

	INT32 j = 0;
	while (i > 0) {
		buffer[j++] = (i % 10) + u'0';
		i /= 10;
	}

	buffer[j] = u'\0';

	for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
		UINT16 temp = buffer[k];
		buffer[k] = buffer[l];
		buffer[l] = temp;
	}

	return buffer;
}
