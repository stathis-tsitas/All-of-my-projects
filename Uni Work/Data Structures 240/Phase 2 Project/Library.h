#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>

#define TITLE_MAX 128
#define NAME_MAX  64
#define HEAP_CAPACITY 64

extern int SLOTS;


typedef struct BookNode {
    char title[TITLE_MAX];
    struct book *book;
    struct BookNode *lc;
    struct BookNode *rc;
    int height;
} BookNode;


typedef struct MemberActivity {
    int sid;
    int loans_count;
    int reviews_count;
    int score_sum;
    struct MemberActivity *next;
} MemberActivity;


typedef struct loan {
    int sid;
    int bid;
    struct loan *next;
} loan_t;


typedef struct book {
    int  bid;
    int  gid;
    char title[TITLE_MAX];

    int sum_scores;
    int n_reviews;
    int avg;
    int lost_flag;


    int heap_pos;   /* Θέση στον σωρό (-1 αν δεν υπάρχει) */

    struct book *prev;
    struct book *next;
} book_t;


typedef struct RecHeap {
    book_t *heap[HEAP_CAPACITY];
    int size;
} RecHeap;


typedef struct member {
    int  sid;
    char name[NAME_MAX];
    loan_t* loans;


    MemberActivity* activity;

    struct member *next;
} member_t;


typedef struct genre {
    int  gid;
    char name[NAME_MAX];
    book_t* books;


    BookNode* bookIndex;

    int lost_count;
    int invalid_count;

    int slots;
    book_t **display;
    struct genre *next;
} genre_t;


typedef struct library {
    genre_t  *genres;
    member_t *members;


    RecHeap  *recommendations;
    MemberActivity *activity;
} library_t;


void lib_S(library_t* lib, int slots);
void lib_G(library_t* lib, int gid, const char* name);
void lib_BK(library_t* lib, int bid, int gid, const char* title);
void lib_M(library_t* lib, int sid, const char* name);
void lib_L(library_t* lib, int sid, int bid);
void lib_R(library_t* lib, int sid ,int bid , int score, const char* status);
void lib_D(library_t* lib);
void lib_PG(library_t* lib, int gid);
void lib_PM(library_t* lib, int sid);
void lib_PD(library_t* lib);

/*PHASE 2 NEA COMMANDS*/
void lib_F(library_t* lib, const char* title);
void lib_TOP(library_t* lib, int k);
void lib_AM(library_t* lib);
void lib_U(library_t* lib, int bid, const char* new_title);
void lib_X(library_t* lib);
void lib_BF(library_t* lib);

#endif