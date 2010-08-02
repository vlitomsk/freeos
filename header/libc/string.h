#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

extern int streql(char* str1, char* str2);
extern int streqln(char* str1, char* str2, int n);
extern void upcase(char* str);
extern char** split(char* s, int* reslen, int* pieces_len, char sep); // если не знаешь, как устроено внутри, лучше не запускать.

#endif
