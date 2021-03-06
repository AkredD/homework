#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>

char last;
FILE *pf;
FILE *pr;

int checkcom(char *command, int l, int i) {
    if (l == 0) {
        int h = 0;
        int hh = 0;
        while (command[i] != ' ') {
            char te = command[i++];
            if ((te >= 'a' && te <= 'z') || (te >= 'A' && te <= 'Z')) {
                continue;
            } else {
                printf("incorrect input\n");
                return 0;
            }
        }
        ++i;
        int sk = 0;
        int tr = 0;
        while (command[i] != '\n') {
            char te = command[i++];
            if ((te >= '0' && te <= '9') || (te == '+') || (te == '(') || (te == ')') || (te == '-')) {
                if (te == '(') if (sk == 0) {
                    tr = 0;
                    sk = 1;
                } else {
                    printf("incorrect input\n");
                    return 0;
                }
                if (te == ')') if (sk == 1) {
                    tr = 0;
                    sk = 2;
                } else {
                    printf("incorrect input\n");
                    return 0;
                }
                if (te == '-') if (tr == 0) {
                    tr = 1;
                } else {
                    printf("incorrect input\n");
                    return 0;
                }
                tr = 0;
                continue;
            } else {
                printf("incorrect input\n");
                return 0;
            }
        }
    }
    if (l == 1) {
        int check = 1;
        int j = 0;
        if ((command[i] >= '0' && command[i] <= '9') || command[i] == '+' || (command[i] == '(') ||
            (command[i] == ')') || (command[i] == '-'))
            check = 0;
        while (command[i] != '\n') {
            char te = command[i++];
            if (check) {
                if ((te >= 'a' && te <= 'z') || (te >= 'A' && te <= 'Z')) {
                    continue;
                } else {
                    printf("incorrect input\n");
                    return 0;
                }
            } else {
                if ((te >= '0' && te <= '9') || (te == '+') || (te == '(') || (te == ')') || (te == '-')) {
                    continue;
                } else {
                    printf("incorrect input\n");
                    return 0;
                }
            }
        }
    }
    if (l == 2) {
        if (command[i] >= '0' && command[i + 1] == '\n') return 1;
        printf("incorrect input\n");
        return 0;
    }
    if (l == 3) {
        if (command[i] >= '0' && command[i + 1] == ' ') {
            i += 2;
            int check = 1;
            int j = 0;
            if ((command[i] >= '0' && command[i] <= '9') || command[i] == '+') check = 0;
            while (command[i] != '\n') {
                char te = command[i++];
                if (check) {
                    if ((te >= 'a' && te <= 'z') || (te >= 'A' && te <= 'Z')) {
                        continue;
                    } else {
                        printf("incorrect input\n");
                        return 0;
                    }
                } else {
                    if ((te >= '0' && te <= '9') || (te == '+') || (te == '(') || (te == ')') || (te == '-')) {
                        continue;
                    } else {
                        printf("incorrect input\n");
                        return 0;
                    }
                }
            }
        } else {
            printf("incorrect input\n");
            return 0;
        }
    }
    return 1;
}

int strings(char *s, char *c, int k) {
    int lens, lenc;
    int i,j;
    if (k == 2) {
        for (lens = 0; s[lens] != '$'; ++lens);
        for (lenc = 0; c[lenc] != '$'; ++lenc);
    }
    if (k == 1) {
        lenc = strlen(c);
        lens = strlen(s) - 1;
    }
    if (k == 0) {
        lenc = strlen(c);
        for (lens = 0; s[lens] != '$'; ++lens);
    }
    if (lenc == lens || lenc + 1 == lens) {
    	i = 0;
    	j = 0;
        for (; j < lenc; ++i, ++j) {
        	if (s[i] == '+') ++i;
            if (s[i] != c[j]) {
                return 0;
            }
        }
    } else {
        return 0;
    }

    return 1;
}

int checkstr(char *s, char *c) {
    int k = 0;
    int i, j, lenc, lens;
    for (lens = 0; s[lens] != '$'; ++lens);
    for (lenc = 0; c[lenc] != '$'; ++lenc);
    for (i = 0; i < lens - lenc + 1; ++i) {
        int d = 0;
        char *checkc = (char *) malloc((lenc) * sizeof(int));
        for (j = i; j < i + lenc; ++j) {
            if (s[j] >= 'A' && s[j] <= 'Z') s[j] = s[j] - ('A' - 'a');
            checkc[j - i] = s[j];
        }
        for (j = 0; j < lenc; ++j) {
            if (checkc[j] == c[j]) {
                d = 1;
            } else {
                d = 0;
                break;
            }
        }
        if (d == 1) ++k;
    }
    return k;
}

char *create(int i, char *command, char *merge) {
    int j = 0;
    merge[j] = '$';
    merge[++j] = last++;
    ++j;
    for (i++; command[i] != ' '; ++i, ++j) {
        merge[j] = command[i];
    }
    merge[j] = '$';
    ++j;
    for (i++; i < (strlen(command) - 1); ++i) {
        if ((command[i] >= '0' && command[i] <= '9') || command[i] == '+') {
            merge[j] = command[i];
            ++j;
        }

    }
    merge[j] = '$';
    return merge;
}

void find(char *command, int i) {
    char *str = (char *) malloc((BUFSIZ*10) * sizeof(char));
    int j = 0;
    int check = 1;
    char *test = (char *) malloc((BUFSIZ*10) * sizeof(char));
    if ((command[i] >= '0' && command[i] <= '9') || command[i] == '+') check = 0;
    while (command[i] != '\n') {
        test[j++] = command[i++];
        if (test[j - 1] >= 'A' && test[j - 1] <= 'Z') test[j - 1] = test[j - 1] - ('A' - 'a');
    }
    test[j] = '$';
    rewind(pf);
    while (fgets(str, BUFSIZ*10, pf)) {
        j = 0;
        i = 0;
        char *name = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char *number = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char id;
        if (str[j++] == '$') {
            id = str[j++];
            while (1) {
                if (str[j] != '$') {
                    name[i] = str[j++];
                    ++i;
                } else {
                    name[i] = '$';
                    ++j;
                    break;
                }
            }
            int nameM = i;
            i = 0;
            while (1) {
                if (str[j] != '$') {
                    number[i] = str[j++];
                    ++i;
                } else {
                    number[i] = '$';
                    break;
                }
            }
            int nameN = i;
            if (check) {
                if (checkstr(name, test)) {
                    name[nameM] = ' ';
                    number[nameN] = '\r';
                    printf("%c ", id);
                    for (i = 0; i < nameM;++i){
                    	printf("%c", name[i]);
					}
					printf(" ");
                    for (i = 0; i < nameN;++i){
                    	printf("%c", number[i]);
					}
					printf("\n");
                }
            } else {
                if (strings(number, test, 2)) {
                    number[nameN] = ' ';
                    name[nameM] = '\r';
                    printf("%c ", id);
                    for (i = 0; i < nameM;++i){
                    	printf("%c", name[i]);
					}
					printf(" ");
                    for (i = 0; i < nameN;++i){
                    	printf("%c", number[i]);
					}
					printf("\n");
                }
            }
        }
        free(name);
        free(number);
    }
    free(test);
    free(str);
    return;
}

void deletE(char idN	) {
    char *str = (char *) malloc((BUFSIZ*10) * sizeof(char));
    pr = fopen("new2.txt", "w");
    pr = fopen("new2.txt", "r+");
    rewind(pf);
    while (fgets(str, BUFSIZ, pf)) {
        int j = 0;
        int i = 0;
        char *name = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char *number = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char id;
        if (str[j++] == '$') {
            id = str[j++];
            while (1) {
                if (str[j] != '$') {
                    name[i] = str[j++];
                    ++i;
                } else {
                    name[i++] = '$';
                    ++j;
                    break;
                }
            }
            i = 0;
            while (1) {
                if (str[j] != '$') {
                    number[i] = str[j++];
                    ++i;
                } else {
                    number[i++] = '$';
                    break;
                }
            }
            if (id != idN) {
                fprintf(pr, "$%c%s%s\n", id, name, number);
            }
        }
        free(str);
        char *str = (char *) malloc((BUFSIZ*10) * sizeof(char));
        free(name);
        free(number);
    }
    free(str);
    rewind(pr);
    pf = fopen("new1.txt", "w");
    pf = fopen("new1.txt", "r+");
    char *line = (char *) malloc((BUFSIZ*10) * sizeof(char));
    while (fgets(line, BUFSIZ*10, pr)) {
        fprintf(pf, "%s\n", line);
        free(line);
        char *line = (char *) malloc((BUFSIZ*10) * sizeof(char));
    }
    free(line);
    fclose(pr);
    return;
}

void change(char *command, char idN, int i) {
    int check = 1;
    int j = 0;
    char *test = (char *) malloc((BUFSIZ*10) * sizeof(char));
    if ((command[i] >= '0' && command[i] <= '9') || command[i] == '+') check = 0;
    while (command[i] != '\n') {
        test[j++] = command[i++];
    }
    test[j] = '$';
    char *str = (char *) malloc((BUFSIZ*10) * sizeof(char));
    pr = fopen("new2.txt", "w");
    pr = fopen("new2.txt", "r+");
    rewind(pf);
    while (fgets(str, BUFSIZ*10, pf)) {
        j = 0;
        int i = 0;
        char *name = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char *number = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char id;
        if (str[j++] == '$') {
            id = str[j++];
            while (1) {
                if (str[j] != '$') {
                    name[i++] = str[j++];
                } else {
                    name[i] = '$';
                    ++j;
                    break;
                }
            }
            i = 0;
            while (1) {
                if (str[j] != '$') {
                    number[i++] = str[j++];
                } else {
                    number[i] = '$';
                    break;
                }
            }
            if (id == idN) {
                if (check) {
                    fprintf(pr, "$%c%s%s\n", id, test, number);
                } else {
                    fprintf(pr, "$%c%s%s\n", id, name, test);
                }
            } else {
                fprintf(pr, "$%c%s%s\n", id, name, number);
            }
        }
        free(str);
        char *str = (char *) malloc((BUFSIZ*10) * sizeof(char));
        free(name);
        free(number);
    }
    free(str);
    rewind(pr);
    pf = fopen("new1.txt", "w");
    pf = fopen("new1.txt", "r+");
    char *line = (char *) malloc((BUFSIZ*10) * sizeof(char));
    while (fgets(line, BUFSIZ*10, pr)) {
        if (line[0] == '\n') break;
        fprintf(pf, "%s\n", line);
        //printf("%s\n",line);
        free(line);
        char *line = (char *) malloc((BUFSIZ*10) * sizeof(char));
    }
    free(test);
    free(line);
    fclose(pr);
    return;
}

int main() {
    last = '0';
    pf = fopen("new1.txt", "w");
    pf = fopen("new1.txt", "r+");
    int i, j;

    while (1) {
        int ex = 1;
        fflush(stdin);
        fflush (stdout);
        char *command = (char *) malloc((BUFSIZ*10) * sizeof(char));
        char *com = (char *) malloc((BUFSIZ*10) * sizeof(char));
        fgets(command, BUFSIZ*10, stdin);
        if (strings(command, "exit", 1)) {
            free(command);
            free(com);
            ex = 0;
            break;
        }
        for (i = 0; command[i] != ' '; ++i) { //!!!11
            com[i] = command[i];
        }
        com[i] = '$';
        int h = i;
        if (strings(com, "create", 0)) {
            ex = 0;
            char *merge = (char *) malloc((BUFSIZ*10) * sizeof(char));
            if (checkcom(command, 0, i + 1)) fprintf(pf, "%s \n", create(i, command, merge));
            free(merge);
        }

        if (strings(com, "find", 0)) {
            ex = 0;
            if (checkcom(command, 1, i + 1)) find(command, i + 1);
        }
        if (strings(com, "delete", 0)) {
            ex = 0;
            char id = command[i + 1];
            if (checkcom(command, 2, i + 1)) deletE(id);
        }
        if (strings(com, "change", 0)) {
            ex = 0;
            char id = command[++i];
            if (checkcom(command, 3, i)) change(command, id, i + 2);
        }
        if (ex) printf("incorrect input\n");
        free(com);
    }
    fflush (stdout);
    fclose(pf);
    
    return 0;
}
