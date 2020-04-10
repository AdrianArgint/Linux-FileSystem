/* Adrian Argint
    313 CB      */ 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include "struct.h"
#include "ttrie.h"
#include "functii.h"

#define IZI 30

// Driver 
int main()
{   
	char * line = NULL;
    size_t len = 0;

	// Open input file for reading
	FILE *input_file = fopen("input.in", "r");

	if (input_file == NULL) {
		fprintf(stderr, "File input not found\n");
		return -2;
	}

	//Open output file for writing
	FILE *output_file = fopen("output.out", "w");

	if(output_file == NULL){
		fprintf(stderr, "File output not found\n");
		return -2;
	}

	//Open error file for writing
	FILE *error_file = fopen("error.out", "w");

	if(error_file == NULL){
		fprintf(stderr, "File error not found\n");
		return -2;
	}
	fclose(output_file);
	fclose(error_file);

	int useless_number;
	fscanf(input_file, "%d", &useless_number);

	int i;
	TTrie root = InitNod();
	TTrie CD = root;
    char **mat;
    mat = (char **)malloc(sizeof(char*) * IZI);
    for (i = 0; i < IZI; ++i)
    {
        mat[i] = (char*) malloc(sizeof(char) * IZI);
    }

    //Usefull variables(aux)
	TTrie CDaux;
	char str[IZI];
	int level = 0;
	int ok = 0, j;
    char auxstr[IZI];
    getline(&line, &len, input_file);

	for (j = 0; j < useless_number; ++j)
    {
        getline(&line, &len, input_file);

		char *token;
 		int numar = 0;
  		token = strtok(line, "\n ");
  		while (token != NULL){
   			strcpy(mat[numar], token);
			numar++;
    		token = strtok(NULL, "\n ");
  		}


        switch(mat[0][0]){
        	case 'm':
                if(!strcmp(mat[0], "mkdir")){
            		for (i = 1	; i < numar; ++i)
            		{
                        strcpy(auxstr, mat[i]);
                        if(test_file(auxstr, CD, root)) {
                            error_file = fopen("error.out", "a");
                            if(!error_file) return -1;
                            fprintf(error_file, "%s: No such file or directory\n", mat[i]);
                            fclose(error_file);
                            continue;
                        }
            			if((searchWord(CD, mat[i]) && mat[i][0] != '/') ||
                            (searchWord(root, &mat[i][1]) && mat[i][0] == '/')) {
                            error_file = fopen("error.out", "a");
                            if(!error_file) return -1;
                            fprintf(error_file, "%s: already exists\n", mat[i]);
                            fclose(error_file);
                            continue;
                        }
            			strcat(mat[i], "/");
            			if(mat[i][0] == '/') {
                            if(strcmp(mat[i], "/"))
            				    memmove(mat[i], &mat[i][1], strlen(mat[i]));
            				insert(root, mat[i]);
            			}
            			else
            				insert(CD, mat[i]);
            		}
                }
                else if (!strcmp(mat[0], "mv")){
                    if(numar < 3){
                        strcpy(line, mat[0]);
                        for (i = 1; i < numar; ++i)
                        {
                            strcat(line, " ");
                            strcat(line, mat[i]);
                        }
                        error_file = fopen("error.out", "a");
                        if(!error_file) return -1;
                        fprintf(error_file, "%s: missing operand\n", line);
                        fclose(error_file);
                        break;
                    }
                    mv(root, CD, mat[1], mat[2]);
                }
        		break;
			case 't':
                if(strcmp(mat[0], "touch")){
                    error_file = fopen("error.out", "a");
                    if(!error_file){
                        return -1;
                    } 
                    fprintf(error_file, "%s: command not found\n", mat[0]);
                    fclose(error_file);
                    break;
                }
        		for (i = 1	; i < numar; ++i)
        		{
        			if(searchWord(CD, mat[i])) continue;
        			strcat(mat[i], "*");
        			insert(CD, mat[i]);
        		}
        		break;

        	case 'c':
                if(!strcmp(mat[0], "cd")){
            		if(!strcmp(mat[1], "..")){
            			if(CD == root){
            				error_file = fopen("error.out", "a");
    						if(error_file == NULL){
    							fprintf(stderr, "File error not found\n");
    							return -2;
    						}
            				fprintf(error_file, "..: No such file or directory\n");
            				fclose(error_file);
            				break;
            			}
            			CD_return(root, &CD);
            			break;
            		}
                    strcpy(auxstr, mat[1]);
                    if(mat[1][0] == '/') {
                        memmove(mat[1], &mat[1][1], strlen(mat[1]));
                        CDaux = searchWord(root, mat[1]);
                    }
                    else{
            		  CDaux = searchWord(CD, mat[1]);
                    }
            		if(!CDaux)
            		{
    					error_file = fopen("error.out", "a");
                        if(error_file == NULL){
                            fprintf(stderr, "File error not found\n");
                            return -2;
                        }
                        fprintf(error_file, "%s: No such file or directory\n", auxstr);
                        fclose(error_file);
            		}
                    else if(!CDaux->children[26]){
                         
                        error_file = fopen("error.out", "a");
                        if(error_file == NULL){
                            fprintf(stderr, "File error not found\n");
                            return -2;
                        }
                        fprintf(error_file, "%s: Not a directory\n", auxstr);
                        fclose(error_file);
                    }
                    else{
                        CD = CDaux->children[26];
                    }
                }
                else{
                    cp(root, CD, mat[1], mat[2]);
                }
        		break;

        	case 'r':
        		if(!strcmp(mat[0], "rmdir")){
                    for (i= 1; i < numar; ++i)
                    {
                        removeFile(CD, mat[i], 1);
                    }
                }
                else if (!strcmp(mat[0], "rm")){
                    for (i= 1; i < numar; ++i)
                    {
                        removeFile(CD, mat[i], 0);
                    }
                }
                else{
                    printf("Nu exista comanda\n");
                }
        		break;

        	case 'l':
				if(numar == 2){ // daca se da ls -F sau ls [director]
					if(!strcmp(mat[1], "-F")){
						display(CD, str, level, &ok, 1);
					}
					else { // director
                        if(!strcmp(mat[1], ".."))
                        {
                            CDaux = CD;
                            CD_return(root, &CDaux);
                            display(CDaux, str, level, &ok, 0);
                            FILE *output_file = fopen("output.out", "a");
                            if(!output_file) return -1;
                            fprintf(output_file, "\n");
                            fclose(output_file);
                            break;
                        }
                        else if(!strcmp(mat[1], "/")){
                            display(root, str, level, &ok, 0);
                            FILE *output_file = fopen("output.out", "a");
                            if(!output_file) return -1;
                            fprintf(output_file, "\n");
                            fclose(output_file);
                            break;
                        }
                        else
                            CDaux = searchWord(root, mat[1]);

                        if(!CDaux){
                            error_file = fopen("error.out", "a");
                            if(error_file == NULL){
                                fprintf(stderr, "File error not found\n");
                                return -2;
                            }
                            fprintf(error_file, "%s: Not a directory\n", mat[1]);
                            fclose(error_file);
                            break;
                        }
                        if(CDaux->children[26])
                            CDaux = CDaux->children[26];
                        else {
                            printf("Nu e director\n");
						    break;
                        }
                        display(CDaux, str, level, &ok, 0);
					}
				}
				else if(numar == 3){// ls [director] -F
                    if(!strcmp(mat[2], "."))
                        CDaux = CD;
                    else
					   CDaux = searchWord(root, mat[1]);
					display(CDaux, str, level, &ok, 1);
				}
				else if(numar > 3){
                    error_file = fopen("error.out", "a");
                    if(!error_file){
                        return -1;
                    } 
                    fprintf(error_file, "%s %s %s %s: too many arguments\n",
                     mat[0], mat[1], mat[2], mat[3]);
                    fclose(error_file);              
                    break;  
                }
				else {
                    display(CD, str, level, &ok, 0);
                }

                FILE *output_file = fopen("output.out", "a");
                if(!output_file) return -1;
                fprintf(output_file, "\n");
                fclose(output_file);

				break;
        	case 'p':
        		pwd(CD);
        		break;

        	default:
        		break;
    	}
    }

	// Dealocate all memory
    if (line){
        free(line);
    }
	fclose(input_file);
    free_all(root);
    for (i = 0; i < IZI; ++i)
    {
        free(mat[i]);
    }
    free(mat);
	return 0; 
} 
