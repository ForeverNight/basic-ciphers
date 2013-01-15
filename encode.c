#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define START_ROT47_1 '!'
#define END_ROT47_1 'N'
#define START_ROT47_2 'P'
#if defined _WIN32 || defined _WIN64
#define END_ROT47_2 '}'
#endif
#ifdef unix
#define END_ROT47_2 '~'
#endif
#if defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef char bool;
#define true 1
#define false 0
#endif


char *rot47(char *s);
char rot47char(char);
char *rot13(char *s);
char rot13char(char);

int main(int argc, char *argv[]){
	FILE *output, *input;
	bool bool_input = false,			/* are we using an input file? */
		bool_output = false,			/* are we using an output file? */
		bool_exit = false,				/* variable for exit in for and switch */
		bool_rot13 = false;				/* are we using rot13? */
	int i, j, k, m, n;											/* counters */
	int l,														/* length of string for options */
		ch;														/* character "buffer" */
	if (argc < 2){
		printf("Running in batch mode\n");
		input = fopen ("input.txt", "r");
		if (input == NULL){
			printf("\n\nCouldn't open inputfile, assuming output\n\n");
			output = fopen("output.txt", "r");
			while ((ch = fgetc(output)) != EOF){
				putchar(ch);
			}
			printf("\n\nEOF\n");
			fclose(output);
		}
		else{
			output = fopen("output.txt", "w+");
			while ((ch = fgetc(input)) != EOF){
				fputc(rot47char(ch), output);
			}
			fclose(input);
			fclose(output);
		}
		if (output == NULL){
			printf("\nI couldn't open output.txt\nDoes it exist?\n");
			exit(0);
		}
	}
	else{
		m = -1;
		n = -1;
		for (j = 1; j < argc; ++j){
			switch ((int)argv[j][0]){
			case '-':
			case '/':
				l = strlen(argv[j]);
				for (k = 1; k < l; ++k){
					ch = (int)argv[j][k];
					switch(ch){
					case '1':
						if ((int)argv[j][k+1] == '3'){
							bool_rot13 = true;
						}
						else{
							printf("Invalid Argument: %c\n", ch);
						}
						k += 2;
						break;
					case 'x':
						if (bool_rot13){
							for (i = j + 1; i < argc; ++i){
								printf("%s ", rot13(argv[i]));
							}
							printf("\n");
						}
						else{
							for (i = j + 1; i < argc; ++i){
								printf("%s ", rot47(argv[i]));
							}
							printf("\n");
						}
						bool_exit = true;
						return 0;
						break;
					case 'b':
						input = fopen ("input.txt", "r");
						output = fopen("output.txt", "w+");
						if (input == NULL){
							printf("\nERROR, can't open inputfile, aborting\n");
							fclose(input);
							exit(-1);
						}
						if (bool_rot13){
							while ((ch = fgetc(input)) != EOF){
								fputc(rot13char(ch), output);
							}
						}
						else{
							while ((ch = fgetc(input)) != EOF){
								fputc(rot47char(ch), output);
							}
						}
						fclose(input);
						fclose(output);
						bool_exit = true;
						break;
					case 'i':
						if (bool_output){
							input = fopen(argv[j+2], "r");
							bool_input = true;
							n = j + 2;
							if (input == NULL){
								printf("Couldn't open file %s does it exist?\n", argv[j+2]);
								bool_input = false;
							}
						}
						else{
							input = fopen(argv[j+1], "r");
							bool_input = true;
							n = j + 1;
							if (input == NULL){
								printf("Couldn't open file %s does it exist?\n", argv[j+2]);
								bool_input = false;
							}
						}
						break;
					case 'o':
						if (bool_input){
							output = fopen(argv[j+2], "w+");
							bool_output = true;
							m = j + 3;
						}
						else{
							output = fopen(argv[j+1], "w+");
							bool_output = true;
							m = j + 2;
						}
						break;
					case 'h':
						printf("Usage: [-13] [-b] [-io file file] [-x <phrase>]\nConverts a phrase or a file into a cipher\n");
						break;
					default:
						printf("Invalid Argument: %c\n", ch);
						bool_exit = true;
						return -1;		/* dirty exit, invalid option */
						break;
					}
					if (bool_exit){
						break;
					}
				}
				if (bool_exit){
					j = argc;
				}
				break;
			default:
				if (!(bool_exit || bool_input || bool_output || bool_rot13)){
					for (i = j; i < argc; ++i){
						printf("%s ", rot47(argv[i]));
					}
					printf("\n");
					return 0;
				}
				if ((!(bool_exit || bool_input || bool_output)) && bool_rot13){
					for (i = j; i < argc; ++i){
						printf("%s ", rot13(argv[i]));
					}
					printf("\n");
					return 0;
				}
			}
			if (bool_input){
				if (bool_output){
					if (bool_rot13){
						while ((ch = fgetc(input)) != EOF){
							fputc(rot13char(ch), output);
						}
					}
					else{
						while ((ch = fgetc(input)) != EOF){
							fputc(rot47char(ch), output);
						}
					}
					fclose(output);
				}
				else{
					if (bool_rot13){
						while ((ch = fgetc(input)) != EOF){
							putchar(rot13char(ch));
						}
					}
					else{
						while ((ch = fgetc(input)) != EOF){
							putchar(rot47char(ch));
						}
					}
				}
				fclose(input);
			}
			else if(bool_output){
				if (bool_rot13){
					for (i = m; i < argc; ++i){
						fprintf(output, "%s ", rot13(argv[i]));
					}
				}
				else{
					for (i = m; i < argc; ++i){
						fprintf(output, "%s ", rot47(argv[i]));
					}
				}
				fclose(output);
			}
		} 
	}
	return 0;
}

char *rot13(char *s){
	char *p = s;
	while (*p){
		if( *p >= 'A'  && *p <= 'M'){
			*p = *p + 13;
		}
		else if (*p >= 'N' && *p <= 'Z'){
			*p = *p - 13;
		}
		else if (*p >= 'a' && *p <= 'm'){
			*p = *p + 13;
		}
		else if (*p >= 'n' && *p <= 'z'){
			*p = *p - 13;
		}
		p++;
	}
	return s;
}

char rot13char(char s){
	if( s >= 'A'  && s <= 'M'){
		s = s + 13;
	}
	else if (s >= 'N' && s <= 'Z'){
		s = s - 13;
	}
	else if (s >= 'a' && s <= 'm'){
		s = s + 13;
	}
	else if (s >= 'n' && s <= 'z'){
		s = s - 13;
	}
	return s;
}

char *rot47(char *s){
	char *p = s;
	while (*p){
		if (*p >= START_ROT47_1 && *p <= END_ROT47_1){
			*p = ((*p + 47) % 127);
		}
		else if (*p >= START_ROT47_2 && *p <= END_ROT47_2){
			*p = ((*p - 47) % 127);
		}
		p++;
	}
	return s;
}

char rot47char(char s){
	if (s >= START_ROT47_1 && s <= END_ROT47_1){
		s = ((s + 47) % 127);
	}
	else if (s >= START_ROT47_2 && s <= END_ROT47_2){
		s = ((s - 47) % 127);
	}
	return s;
}