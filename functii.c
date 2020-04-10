/* Adrian Argint
    313 CB      */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ttrie.h"
#include "struct.h"
#include "functii.h"
#include <stdbool.h> 

void display(TTrie root, char str[], int level, int *ok, int f)
/*f = 1 -> ls -F
  f = 0 -> ls */
{ 
    // end of string for directories
    if (root->children[26])  
    { 
    	str[level] = '\0';
        FILE *output_file = fopen("output.out", "a");
        if(!output_file) return;
        if(f) strcat(str, "/");
        fprintf(output_file, "%s ", str);
        fclose(output_file);
    } 

    // end of string for files
    if (root->children[27])  
    { 
        str[level] = '\0';
        FILE *output_file = fopen("output.out", "a");
        if(!output_file) return;
        if(f) strcat(str, "*");
        fprintf(output_file, "%s ", str);
        fclose(output_file);
    }


    int i; 
    for (i = 0; i < ALPHABET_SIZE; i++)  
    {
        if(i == 26 || i == 27) continue;
        if (root->children[i])  
        { 	
        	*ok = 1;
            str[level] = INDEX_TO_CHAR(i);
            display(root->children[i], str, level + 1, ok, f); 
        } 
    }
    
} 


// delete words 
void removeFile(TTrie trieTree, char * word, int ok)
{
    TTrie trieNode = searchWord(trieTree, word);
    if (trieNode == NULL ) {
        // Word not found
        FILE *error_file = fopen("error.out", "a");
        if(!error_file) return;
        fprintf(error_file, "%s: No such file or directory\n", word);
        fclose(error_file);
        return;
    }
    int i;

    if(ok)
        trieNode = trieNode->children[26];
    else
        trieNode = trieNode->children[27];

    // 'noChild' indicates if the node is a leaf node
    bool noChild = true;
    int childCount = 0;

    // childCount indicates how many children a node has
    for (i = 0; i < ALPHABET_SIZE; ++i) {
        if (trieNode->children[i] != NULL) {
            noChild = false;
            ++childCount;
        }
    }

    // if the node has children, it means it's not empty
    if (!noChild) {
        FILE *error_file = fopen("error.out", "a");
        if(!error_file) return;
        fprintf(error_file, "%s: Directory not empty\n", word);
        fclose(error_file);
        return;
    }

    TTrie parentNode = trieNode->parent;
    while (parentNode != NULL && childCount == 0) {
        childCount = 0;
        parentNode = trieNode->parent;
        for (i = 0; i < ALPHABET_SIZE; ++i) {
            if (parentNode->children[i] != NULL) {
                if (trieNode == parentNode->children[i]) {
                    // if we reached the node, mark it with NULL
                    // and free it
                    parentNode->children[i] = NULL;
                    free(trieNode);
                    trieNode = parentNode;
                } else {
                    ++childCount;
                }
            }
        }
    }
}
 

// return to previous directory
void CD_return(TTrie root, TTrie *CD){
    if((*CD)->parent == NULL){
        return;
    }

    TTrie trie = *CD;
    trie = trie->parent;
    TTrie parent = trie->parent;
    while(parent->children[26] != trie && parent->parent){
        trie = parent;
        parent = parent->parent;
    }
    if(!parent->parent) *CD = parent;
    else *CD = trie;
}

// reverse a string
void reverse(char *str) {
    char* reversed = (char*) malloc(sizeof(char) * strlen(str) + 1);
    int j = 0, i;
    for (i = strlen(str) - 1; i >= 0; i--, j++) {
        reversed[j] = str[i];
    }
    reversed[j] = '\0';
    memcpy(str, reversed, strlen(reversed));
    free(reversed);
}

// print working directory
void pwd(TTrie CD){
    FILE *output_file = fopen("output.out", "a");
    if(output_file == NULL){
        fprintf(stderr, "File not found\n");
        return;
    }

    if(CD->parent == NULL){
        fprintf(output_file, "/\n");
        fclose(output_file);
        return;
    }
    char *str;
    str = (char*)malloc(sizeof(char) * 20);
    int level = 0, i;
    TTrie parentNode = CD->parent;
    while(CD->parent){
        for (i = 0; i < ALPHABET_SIZE; ++i)
        {
            if(parentNode->children[i] == CD){
                str[level] = INDEX_TO_CHAR(i);
                level++;
                CD = parentNode;
                parentNode = CD->parent;
                break;
            }
        }
    }
    str[level] = '\0';
    if(level)
        memmove(str, &str[1], strlen(str));
    reverse(str);
    fprintf(output_file, "/%s\n",str);
    free(str);
    fclose(output_file);
}

//1 -> file exists, 0 -> file doesn't exist
int test_file(char *str, TTrie CD, TTrie root){
    if(str[0] == '/') {
        memmove(str, &str[1], strlen(str));
        CD = root;
    }

    int i;
    for (i = 0; i < strlen(str); ++i)
    {
        if(str[i] == '/'){
            str[i] = '\0';
            if(searchWord(CD, str) == NULL){
                return 1;
            }
            str[i] ='/';
        }
    }

    return 0;
}


void move(TTrie root, TTrie CD, TTrie file_path, char *path){
    TTrie move_path;
    char auxstr[20];
    strcpy(auxstr, path);
    if(!strcmp(path, "/")) CD = root;
    else if(path[0] == '/'){
        strcpy(path, &path[1]);
        CD = root;
    }
    move_path = searchWord(CD, path);
    if(!move_path){
        FILE *error_file = fopen("error.out", "a");
        if(!error_file){
            return;
        } 
        fprintf(error_file, "%s: No such file or directory\n", auxstr);
        fclose(error_file);   
        return;
    }
    if(!move_path->children[26]){
        FILE *error_file = fopen("error.out", "a");
        if(!error_file){
            return;
        } 
        fprintf(error_file, "%s: Not a directory\n", auxstr);
        fclose(error_file);   
        return;
    }
    
    char str[20];
    int level = 0;
    TTrie parentNode = file_path->parent;
    int i;
    while(file_path->parent && parentNode->children[26] != file_path){
        for (i = 0; i < ALPHABET_SIZE; ++i)
        {
            if(parentNode->children[i] == file_path){
                parentNode->children[i] = NULL;
                free(file_path);
                str[level] = INDEX_TO_CHAR(i);
                level++;
                file_path = parentNode;
                parentNode = file_path->parent;
                break;
            }
        }
    }
    str[level] = '\0';
    reverse(str);
    strcat(str, "*");
    insert(move_path, str);

}

void mv(TTrie root, TTrie CD, char *file, char *path){
    char auxstr[20];
    TTrie auxCD = CD;
    if(file[0] == '/'){
        strcpy(auxstr, &file[1]);
        auxCD = root;
    }
    else strcpy(auxstr, file);
    TTrie file_path;
    file_path = searchWord(auxCD, auxstr);
    

    if(file_path){
        if(file_path->children[26]){
            FILE *error_file = fopen("error.out", "a");
            if(!error_file){
                return;
            } 
            fprintf(error_file, "%s: No such file or directory\n", file);
            fclose(error_file);
        }
        else if(file_path->children[27]){
            move(root, CD, file_path, path);
        }
        else{
            FILE *error_file = fopen("error.out", "a");
            if(!error_file){
                return;
            } 
            fprintf(error_file, "%s: No such file or directory\n", file);
            fclose(error_file);
        }
    }
    else{
        FILE *error_file = fopen("error.out", "a");
        if(!error_file){
            return;
        } 
        fprintf(error_file, "%s: No such file or directory\n", file);
        fclose(error_file);
    }

}

void copy(TTrie CD, char *path, TTrie root, TTrie copy_path){
    TTrie trie_path;
    if(path[0] == '/'){
        strcpy(path, &path[1]);
        trie_path = searchWord(root, path);
    }
    else{
        trie_path = searchWord(CD, path);
    }

    if(!trie_path) {
        FILE *error_file = fopen("error.out", "a");
        if(!error_file){
            return;
        } 
        fprintf(error_file, "%s: No such file or directory\n", path);
        fclose(error_file);
    }
    else{
        if(!trie_path->children[26]){
            printf("Nu e director\n");
            return;
        }
        trie_path = trie_path->children[26];
        char str[20];
        int level = 0;
        TTrie parentNode = copy_path->parent;
        int i;
        while(copy_path->parent && parentNode->children[26] != copy_path){
            for (i = 0; i < ALPHABET_SIZE; ++i)
            {
                if(parentNode->children[i] == copy_path){
                    str[level] = INDEX_TO_CHAR(i);
                    level++;
                    copy_path = parentNode;
                    parentNode = copy_path->parent;
                    break;
                }
            }
        }
        str[level] = '\0';
        reverse(str);
        strcat(str, "*");
        insert(trie_path, str);
    }
}

void cp(TTrie root, TTrie CD, char *file, char *path){
    char auxstr[20];
    TTrie copy_path = CD;
    if(file[0] == '/'){
        strcpy(auxstr, &file[1]);
        copy_path = root;
    }
    else  strcpy(auxstr, file);
    copy_path = searchWord(copy_path, auxstr);
    if(copy_path){
        if(copy_path->children[27]){
            copy(CD, path, root, copy_path);
        }
        else{
            FILE *error_file = fopen("error.out", "a");
            if(!error_file){
                return;
            } 
            fprintf(error_file, "%s: No such file or directory\n", file);
            fclose(error_file);
        }
    }
    else{
        FILE *error_file = fopen("error.out", "a");
        if(!error_file){
            return;
        } 
        fprintf(error_file, "%s: No such file or directory\n", file);
        fclose(error_file);
    }
}