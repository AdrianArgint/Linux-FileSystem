// C implementation of search and insert operations 
// on Trie 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 

// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (28) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 

// trie node 
typedef struct nod 
{ 
	struct nod *children[ALPHABET_SIZE]; 

	// isEndOfWord is true if the node represents 
	// end of a word 
	bool isEndOfWord; 
}TNod, *TTrie;

// Returns new trie node (initialized to NULLs) 
TTrie InitNod(void) 
{ 
	TTrie nod; 

	nod = (TTrie)malloc(sizeof(TNod)); 

	if (nod) 
	{ 
		int i; 

		nod->isEndOfWord = false; 

		for (i = 0; i < ALPHABET_SIZE; i++) 
			nod->children[i] = NULL; 
	} 

	return nod; 
} 

// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
void insert(TTrie root, const char *key) 
{ 
	int level; 
	int length = strlen(key); 
	int index; 

	TTrie p = root; 

	for (level = 0; level < length; level++) 
	{ 
		index = CHAR_TO_INDEX(key[level]);
		if(index == -50) index = 26;
		if(index == -55) index = 27;
		if (!p->children[index])
				p->children[index] = InitNod(); 
		p = p->children[index]; 
	} 

	// mark last node as leaf 
	p->isEndOfWord = true; 
} 

// Returns true if key presents in trie, else false 
bool search(TTrie root, const char *key) 
{ 
	int level; 
	int length = strlen(key); 
	int index; 
	TTrie p = root; 

	for (level = 0; level < length; level++) 
	{ 
		index = CHAR_TO_INDEX(key[level]); 
		if(index == -50) index = 26;
		if(index == -55) index = 27;		
		if (!p->children[index]) 
			return false; 

		p = p->children[index]; 
	} 

	return (p != NULL && p->isEndOfWord); 
} 


bool isLeafNode(TTrie root) 
{ 
    return root->isEndOfWord != false; 
} 
  

// function to display the content of Trie 
void display(TTrie root, char str[], int level) 
{ 
    // If node is leaf node, it indiicates end 
    // of string, so a null charcter is added 
    // and string is displayed 
    if (isLeafNode(root))  
    { 
        str[level] = '\0'; 
        printf("%s\n", str);
    } 
  
    int i; 
    for (i = 0; i < ALPHABET_SIZE; i++)  
    { 
        // if NON NULL child is found 
        // add parent key to str and 
        // call the display function recursively 
        // for child node 
        if (root->children[i])  
        { 
            str[level] = i + 'a';
            if(i == 26) str[level] = '/';
            if(i == 27) str[level] = '*';
            display(root->children[i], str, level + 1); 
        } 
    } 
} 

// Driver 
int main() 
{ 
	// Input keys (use only 'a' through 'z' and lower case) 
	char keys[][8] = {"the/", 
					"by", "bye", "their*"}; 

	char output[][32] = {"Not present in trie", "Present in trie"}; 

	printf("TEST %d\n", CHAR_TO_INDEX('*'));
	TTrie root = InitNod(); 

	// Construct trie 
	int i; 
	// for (i = 0; i < ARRAY_SIZE(keys); i++) 
	// 	insert(root, keys[i]); 

	// Search for different keys 
	printf("%s --- %s\n", "the/", output[search(root, "the/")] ); 
	printf("%s --- %s\n", "these", output[search(root, "these")] ); 
	printf("%s --- %s\n", "their", output[search(root, "their*")] ); 
	printf("%s --- %s\n", "thaw", output[search(root, "thaw")] );
	// Display content
	char str[20];
	int level = 0;
	printf("Content:\n");
	display(root, str, level);
	printf("\n");
	char *line;
	size_t len;
	ssize_t read;
	FILE *fin;
	fin = fopen("input.in", "r");
	while ((read = getline(&line, &len, fin)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }
    fclose(fin);
	return 0; 
} 
