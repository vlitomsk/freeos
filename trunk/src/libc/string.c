#include <string.h>
#include <utils.h>
#include <stdlib.h>

char** split(char* s, int* reslen, int* pieces_len, char sep) { // ЕЩЕ ОТЛАДИТЬ ЩЕДРО!!	
	if (strlen(s) < 3) return NULL;
	*reslen = 0;
	int start = 0;
	int end = strlen(s);
	while (1) {	
		int chg = 0;
		if (s[start] == sep) {
			start++;
			chg = 1;
		}

		if (s[end - 1] == sep) {
			end--;	
			chg = 1;
		}

		if (chg == 0) break;
	}

	if (start >= end) return NULL;

	*reslen = 1;
	int i;
	for (i = start; i < end; ++i) 
		if (s[i] == sep) (*reslen)++;	

	int lastindex = start;
	char** result = (char**)malloc(sizeof(char*) * (*reslen));
	pieces_len = (int*)malloc(sizeof(int) * (*reslen));
	for (i = start; i < end; ++i) {
		if (s[i] == sep) {
			result[i] = (char*)malloc(sizeof(char) * (i - lastindex));
			memcpy(result[i], s, (i - lastindex));
			pieces_len[i] = i - lastindex;
			lastindex = i + 1;
		}
	}

	return result;
}

int streql(char* str1, char* str2) {
	if (strlen(str1) != strlen(str2)) return 0;
	int i;
	for (i = 0; i < strlen(str1); ++i) 
		if (str1[i] != str2[i]) return 0;

	return 1;
}

int streqln(char* str1, char* str2, int n) {
	if ((strlen(str1) <= n) || (strlen(str2) <= n)) return 0;
	int i;
	for (i = 0; i < n; ++i) 
		if (str1[i] != str2[i]) return 0;
	
	return 1;
}

void upcase(char* str) {
	int i;
	for (i = 0; i < strlen(str); ++i) 
		if ((str[i] >= 'a') && (str[i] <= 'z')) 
			str[i] -= 'a' - 'A';	
}

