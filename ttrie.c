/* Adrian Argint
    313 CB      */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ttrie.h"
#include "struct.h"



// Converts key current character into index 
// use only 'a' through 'z' and lower case 

int CHAR_TO_INDEX(char c){
	int index = (int)c - (int)'a';
	if(index == -50) return 26; // Pentru '/'
	if(index == -55) return 27; // Pentru '*'
	if(index > -50 && index < 0)  return (index + 77);
	if(index == -51) return 39;
	return index;
}

char INDEX_TO_CHAR(int index){
	if(index < 26) 	return 'a' + index;
	if(index == 26) return '/'; // Pentru '/'
	if(index == 27) return '*'; // Pentru '*'
	if(index == 39) return '.';
	if(index > 27) 	return '0' + index - 28;
	return '-';
}

// Returns new trie node (initialized to NULLs) 
TTrie InitNod(void) 
{ 
	TTrie nod; 

	nod = (TTrie)malloc(sizeof(TNod)); 

	if (nod) 
	{ 
		int i; 

		for (i = 0; i < ALPHABET_SIZE; i++) 
			nod->children[i] = NULL; 
		nod->parent = NULL;

	} 
	return nod; 
} 


// Searches for word and return a pointer 
// at the last letter or NULL if not found
TTrie searchWord(TTrie treeNode, char * word)
{	
    // Function is very similar to insert() function
    while (*word != '\0') {
        if (treeNode->children[CHAR_TO_INDEX(word[0])] != NULL) {
            treeNode = treeNode->children[CHAR_TO_INDEX(word[0])];
            ++word;
        } else {
            break;
        }
    }
 
    if (*word == '\0' && (treeNode->children[26] || treeNode->children[27])) {
        // Word found
       		return treeNode;
    } else {
        // Word not found
        return NULL;
    }
}


// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
void insert(TTrie root, char *key) 
{
	int level; 
	int length = strlen(key); 
	int index; 

	TTrie p = root; 

	for (level = 0; level < length; level++) 
	{ 
		index = CHAR_TO_INDEX(key[level]);
		if (!p->children[index]){
				p->children[index] = InitNod(); 
				p->children[index]->parent = p;
			}
		p = p->children[index]; 
	} 
} 

void free_all(TTrie root)
{
    int i;

    // condition to exit the recursive function
    if(!root) return;  
    // recursive case (go to end of trie)
    for (i = 0; i < ALPHABET_SIZE; i++)
       free_all(root->children[i]);


    // base case
    free(root);
}