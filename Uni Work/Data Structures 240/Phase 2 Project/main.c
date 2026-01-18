#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"

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

    library.genres = NULL;
    library.members = NULL;
    library.activity = NULL;

    library.recommendations = malloc(sizeof(RecHeap));
    if(library.recommendations){
        library.recommendations->size = 0;
    } else {
        perror("Initial Malloc Failed");
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = '\0';
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0' || *p == '#') continue;

        char cmd[8];
        if (sscanf(p, " %7s", cmd) != 1) continue;

        if (strcmp(cmd, "S") == 0) {
            int slots;
            if (sscanf(p, "S %d", &slots) == 1) lib_S(&library, slots);
        } else if (strcmp(cmd, "G") == 0) {
            int gid; char name[NAME_MAX];
            if (sscanf(p, "G %d \"%63[^\"]\"", &gid, name) == 2 || sscanf(p, "G %d %63[^\n]", &gid, name) == 2)
                lib_G(&library, gid, name);
        } else if (strcmp(cmd, "BK") == 0) {
            int bid, gid; char title[TITLE_MAX];
            if (sscanf(p, "BK %d %d \"%127[^\"]\"", &bid, &gid, title) == 3 || sscanf(p, "BK %d %d %127[^\n]", &bid, &gid, title) == 3) {
                lib_BK(&library, bid, gid, title);
            } else {
                printf("IGNORED\n");
            }
        } else if (strcmp(cmd, "M") == 0) {
            int sid; char name[NAME_MAX];
            if (sscanf(p, "M %d \"%63[^\"]\"", &sid, name) == 2 || sscanf(p, "M %d %63[^\n]", &sid, name) == 2)
                lib_M(&library, sid, name);
        } else if (strcmp(cmd, "L") == 0) {
            int sid, bid;
            if (sscanf(line, "L %d %d", &sid, &bid) == 2) lib_L(&library, sid, bid);
        } else if (strcmp(cmd, "R") == 0) {
            int sid, bid, score = -1; char status[16]; char score_str[16];
            if (sscanf(line, "R %d %d %15s %15s", &sid, &bid, score_str, status) == 4) {
                if(strcmp(score_str, "NA") == 0) score = -1;
                else score = atoi(score_str);
                lib_R(&library, sid, bid, score, status);
            }
        } else if(strcmp(cmd, "D") == 0) {
            lib_D(&library);
        } else if(strncmp(cmd, "PG", 2) == 0) {
            int gid;
            if (sscanf(line, "PG %d", &gid) == 1) lib_PG(&library, gid);
        } else if(strcmp(cmd, "PM") == 0) {
            int sid;
            if (sscanf(line, "PM %d", &sid) == 1) lib_PM(&library, sid);
        } else if(strcmp(cmd, "PD") == 0) {
            lib_PD(&library);
        } else if(strcmp(cmd, "F") == 0) {
            char title[TITLE_MAX];
            if (sscanf(p, "F \"%127[^\"]\"", title) == 1 || sscanf(p, "F %127[^\n]", title) == 1)
                lib_F(&library, title);
        } else if(strcmp(cmd, "TOP") == 0) {
            int k;
            if (sscanf(line, "TOP %d", &k) == 1) lib_TOP(&library, k);
        } else if(strcmp(cmd, "AM") == 0) {
            lib_AM(&library);
        } else if(strcmp(cmd, "U") == 0) {
            int bid; char new_title[TITLE_MAX];
            if (sscanf(p, "U %d \"%127[^\"]\"", &bid, new_title) == 2 || sscanf(p, "U %d %127[^\n]", &bid, new_title) == 2)
                lib_U(&library, bid, new_title);
        } else if(strcmp(cmd, "X") == 0) {
            lib_X(&library);
        } else if(strcmp(cmd, "BF") == 0) {
            lib_BF(&library);
        } else {
            printf("IGNORED\n");
        }
    }
    fclose(fp);
    return 0;
}