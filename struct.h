/* Adrian Argint
    313 CB      */

#ifndef STRUCTURI
#define STRUCTURI
#include <stdio.h>

// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (40) 

// trie node 
typedef struct nod 
{ 
	struct nod *parent;
	struct nod *children[ALPHABET_SIZE]; 
}TNod, *TTrie;


#endif