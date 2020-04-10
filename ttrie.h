/* Adrian Argint
    313 CB      */

#ifndef TRIE
#define TRIE
#include "struct.h"



int CHAR_TO_INDEX(char c);
char INDEX_TO_CHAR(int index);
TTrie InitNod(void);
TTrie searchWord(TTrie treeNode, char * word);
void insert(TTrie root, char *key);
void free_all(TTrie root);

#endif