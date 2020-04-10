/* Adrian Argint
    313 CB      */

#ifndef FILE_FUNCTION
#define FILE_FUNCTION
#include "struct.h"
#include "ttrie.h"


void display(TTrie root, char str[], int level, int *ok, int f);
void removeFile(TTrie trieTree, char * word, int ok);
void CD_return(TTrie root, TTrie *CD);
void reverse(char *str);
void pwd(TTrie CD);
int test_file(char *str, TTrie CD, TTrie root);
void move(TTrie root, TTrie CD, TTrie file_path, char *path);
void mv(TTrie root, TTrie CD, char *file, char *path);
void cp(TTrie root, TTrie CD, char *file, char *path);
void copy(TTrie CD, char *path, TTrie root, TTrie copy_path);


#endif