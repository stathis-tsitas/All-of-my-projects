#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"
//MAIN IDEES APO PERYSI
int SLOTS;
library_t library = {0};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\r\n")] = '\0';


        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0' || *p == '#') continue;


        char cmd[8];
        if (sscanf(p, " %7s", cmd) != 1) {
            printf("IGNORED\n");
            continue;
        }

        if (strcmp(cmd, "S") == 0) {
            int slots;
            if (sscanf(p, "S %d", &slots) == 1) {
                lib_S(&library, slots);
            } else {
                printf("IGNORED\n");
            }
        } else if (strcmp(cmd, "G") == 0) {
            int gid;
            char name[NAME_MAX];

            if (sscanf(p, "G %d \"%63[^\"]\"", &gid, name) == 2 ||
                sscanf(p, "G %d %63[^\n]", &gid, name) == 2) {
                lib_G(&library, gid, name);
            } else {
                printf("IGNORED\n");
            }
        } else if (strcmp(cmd, "BK") == 0) {
            int bid, gid;
            char title[TITLE_MAX];

            if (sscanf(p, "BK %d %d \"%127[^\"]\"", &bid, &gid, title) == 3 ||
                sscanf(p, "BK %d %d %127[^\n]", &bid, &gid, title) == 3) {
                lib_BK(&library, bid, gid, title);
            } else {
                printf("IGNORED\n");
            }
        } else if (strcmp(cmd, "M") == 0) {
            int sid;
            char name[NAME_MAX];
            if (sscanf(p, "M %d \"%63[^\"]\"", &sid, name) == 2 ||
                sscanf(p, "M %d %63[^\n]", &sid, name) == 2) {
                lib_M(&library, sid, name);
            } else {
                printf("INGORED\n");
            }
        } else if (strcmp(cmd, "L") == 0) {
            int sid, bid;
            if (sscanf(line, "L %d %d", &sid, &bid) == 2) {
                lib_L(&library, sid, bid);
            } else {
                printf("INGORED\n");
            }
        } else if (strcmp(cmd, "R") == 0) {
            int sid, bid, score;
            char status[16];
            if (sscanf(line, "R %d %d %d %15s", &sid, &bid, &score, status) == 4) {
                lib_R(&library, sid, bid, score, status);
            } else {
                printf("IGNORE\n");
            }

        }else if(strcmp(cmd, "D") == 0) {
            lib_D(&library);
        }else if(strncmp(cmd, "PG", 2) == 0) {
            int gid;
            if (sscanf(line, "PG %d", &gid) == 1) {
                lib_PG(&library, gid);
            }
        }else if(strcmp(cmd, "PM") == 0) {
            int sid;
            if (sscanf(line, "PM %d", &sid) == 1) {
                lib_PM(&library, sid);
            }
        }else if(strcmp(cmd, "PD") == 0) {
            lib_PD(&library);
        }else if(strcmp(cmd, "D") == 0) {
            lib_PDD(&library);
        } else {
            printf("IGNORE\n");
        }
    }
    fclose(fp);
    return 0;
}



























