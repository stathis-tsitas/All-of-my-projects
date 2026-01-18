#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"

void lib_S(library_t* lib, int slots){ // ΟΡΙΖΩ ΤΙΣ ΘΕΣΕΙΣ
    (void)lib;
    if(slots < 0) slots = 0;
    SLOTS = slots;
    printf("DONE S \n");
}

void lib_G(library_t* lib, int gid, const char* name){ // ΦΤΙΑΧΝΩ ΜΙΑ ΚΑΤΗΓΟΡΙΑ GENRE ΜΕ GID ΚΑΙ ΝΑΜΕ ΑΝ ΕΧΩ ΗΔΗ ΓΙΔ ΚΑΝΩ IGNORE
    if(!lib || !name) return;

    genre_t* cur = lib->genres;
    genre_t* prev = NULL;
    while(cur){
        if(cur->gid == gid){
            return;
        }
        if(cur->gid > gid) break;
        prev = cur;
        cur = cur->next;
    }

    genre_t* ng = malloc(sizeof(genre_t));
    if (!ng) {
        perror("MEMORY ALLOCATION ERORR");
        exit(1);
    }

    ng->gid = gid;
    strncpy(ng->name, name,NAME_MAX);
    ng->name[NAME_MAX - 1] = '\0';
    ng->books = NULL;
    ng->lost_count = 0;
    ng->invalid_count = 0;
    ng->slots = 0;
    ng->display = NULL;
    ng->next = NULL;

    if(!prev){
        ng->next = lib->genres;
        lib->genres = ng;
    }else {
        ng->next = prev->next;
        prev->next = ng;
    }

    printf("DONE G\n");
}

void lib_BK(library_t* lib, int bid, int gid, const char* title) { // BAZW ENA BIBLIO STO GID ME TITLE
    if (!lib || !title) return;


    genre_t* g = lib->genres;
    while (g && g->gid != gid) {
        g = g->next;
    }
    if (!g) {
        printf("IGNORED NO GENRE\n"); // δεν υπάρχει αυτό το genre
        return;
    }


    book_t* cur = g->books;
    while (cur) {
        if (cur->bid == bid) {
            printf("IGNORED BK\n");
            return;
        }
        cur = cur->next;
    }

    book_t* nb = malloc(sizeof(book_t));
    if (!nb) {
        perror("malloc");
        exit(1);
    }

    nb->bid = bid;
    nb->gid = gid;
    strncpy(nb->title, title, TITLE_MAX);
    nb->title[TITLE_MAX - 1] = '\0';

    nb->sum_scores = 0;
    nb->n_reviews = 0;
    nb->avg =0;
    nb->lost_flag = 0;
    nb->next = NULL;
    nb->prev = NULL;

    if(!g->books){
        g->books = nb;
    }else{
        book_t* cur = g->books;
        while(cur->next){
            cur = cur->next;
        }
        cur->next = nb;
        nb->prev = cur;
    }

    printf("DONE BK\n");
}

void lib_M(library_t* lib, int sid, const char* name){ // FTIAXNW ENA MELOS ME SID

    if(!lib || !name) {
        return;
    }
    member_t* current = lib->members;
    member_t* prev = NULL;

    while(current){
        if(current->sid == sid){
            printf("IGNORED\n");
            return;
        }
        if(current->sid > sid) break;
            prev = current;
            current = current->next;
        }

    //new melos

    member_t* newmember = malloc(sizeof(member_t));
    if(!newmember){
        perror("FAILED TO ALLOCATE MEMBER MEMORY");
        exit(1);
    }

    newmember->sid = sid;
    strncpy(newmember->name, name , NAME_MAX);
    newmember->name[NAME_MAX - 1] = '\0';

    loan_t* sentinel = malloc(sizeof(loan_t));
    if(!sentinel){
        fprintf(stderr, "FAILED TO ALLOCATE LOAN MEMORY\n");
        free(newmember);
        exit(1);
    }

    sentinel->next = NULL;
    newmember->loans = sentinel;
    newmember->loans->next = NULL;

    if(!prev){
        newmember->next = lib->members;
        lib->members = newmember;
    }else{
        newmember->next = prev->next;
        prev->next = newmember;
    }

    printf("DONE M\n");

}

void lib_L(library_t* lib, int sid, int bid){ //DANEIZW BIBLIO
    if(!lib) return;

    member_t* member =lib->members;
    while(member && member->sid != sid) member = member->next;
    if(!member){
        printf("INGORE\n");
        return;
    }


    genre_t* genre = lib->genres;
    book_t* book = NULL;

    while(genre && !book){
        book_t* current = genre->books;
        while(current){
            if(current->bid == bid){
                book = current;
                break;
            }
            current = current->next;
        }
        genre = genre->next;
    }

    if(!book){
        printf("IGNORED BOOK L\n");
        return;
    }

    loan_t* currentloan = member->loans->next;
    while(currentloan){
        if(currentloan->bid == bid){
            printf("IGNORED LOAN L\n");
            return;
        }
        currentloan = currentloan->next;
    }

    loan_t* newloan = malloc(sizeof(loan_t));
    if(!newloan){
        fprintf(stderr, "MEMORY ALLOCATION FAIL FOR LOAN\n");
        exit(1);
    }

    newloan->sid = sid;
    newloan->bid = bid;

    newloan->next = member->loans->next;
    member->loans->next = newloan;

    printf("DONE L\n");
}


void lib_R(library_t* lib, int sid ,int bid , int score, const char* status) { // EPISTREFW BIBLIO ME STATUS

    if (!lib || !status) return;

    member_t *member = lib->members;
    while (member && member->sid != sid) {
        member = member->next;
    }
    if (!member) {
        printf("IGNORE\n");
        return;

    }

    loan_t *prev = member->loans;
    loan_t *current = NULL;

    if (prev != NULL) {
        current = prev->next;
    }


    while (current) {
        if (current->bid == bid) {
            break;

        }
        prev = current;
        current = current->next;
    }
    genre_t *genre = lib->genres;
    book_t *book = NULL;

    while (genre && !book) {
        book = genre->books;
        while (book && book->bid != bid) {
            book = book->next;
        }

        if (!book) {
            genre = genre->next;
        }
    }

    if (!book || !genre) {
        printf("IGNORED\n");
        return;
    }

    prev->next = current->next;
    free(current);

    if (strcmp(status, "ok") == 0) {
       if(score >= 1 && score <= 10){
           book->sum_scores += score;
           book->n_reviews++;
           book->avg = (book->sum_scores / book->n_reviews);
       } else{
           genre->invalid_count++;
       }
    }else if(strcmp(status, "lost") == 0){
        book->lost_flag = 1;
        genre->lost_count++;
    }else{
        printf("IGNORE\n");
        return;
    }
    printf("DONE R\n");
}

void lib_D(library_t* lib){
    if(!lib) return;

    for(genre_t* genre = lib->genres; genre; genre = genre->next){
        if(genre->display){
            free(genre->display);
            genre->display= NULL;
        }
        genre->slots = 0;
    }
    int VALID = 0;
    for(genre_t* genre = lib->genres; genre; genre = genre->next){
        int points = 0;
        for(book_t* book = genre->books; book; book = book->next){
            if(book->n_reviews > 0 && book->lost_flag ==0){
                points += book->sum_scores;
            }
        }
        genre->invalid_count = points;
        VALID += points;
    }

    if (VALID == 0 || SLOTS == 0) {
        printf("DONE\n");
        return;
    }

    int quota = VALID / SLOTS;
    if (quota == 0) {
        printf("DONE\n");
        return;
    }

    int used_slots = 0;
    for(genre_t* genre = lib->genres; genre; genre = genre->next){
        int points = genre->invalid_count;
        genre->slots = points/quota;
        int remainder = points - (genre->slots * quota);
        genre->lost_count = remainder;
        used_slots += genre->slots;

    }

    int remainingslots = SLOTS - used_slots;

    if(!lib->genres){
        printf("DONE\n");
        return;
    }
    while(remainingslots > 0 ){
        genre_t* best = NULL;
        for(genre_t* genre = lib->genres; genre; genre = genre->next){
            if(!best){
                best = genre;
            }else if(genre->lost_count > best->lost_count || (genre->lost_count == best->lost_count && genre->gid < best->gid)){
                best = genre;
            }
        }
        if(!best || best->lost_count == 0) break;

        best->slots++;
        best->lost_count = 0;
        remainingslots--;
    }

    for(genre_t* genre = lib->genres; genre; genre = genre->next){
        if(genre->slots ==  0) continue;
        genre->display = malloc(sizeof(book_t*) * genre->slots);
        if(!genre->display){
            perror("MEMORY ALLOCATION FAILED");
            exit(1);
        }

        int count = 0;
        for(book_t* book = genre->books; book && count < genre->slots; book = book->next){
            if(book->lost_flag == 0 && book->n_reviews > 0){
                genre->display[count++] = book;
            }

        }

        while(count < genre->slots){
            genre->display[count++] = NULL;
        }
    }
    printf("DONE D\n");

}

void lib_PG(library_t* lib, int gid){ // EMFANIZW BIBLIA KATHGORIAS ME AVG SOCRES KTL
    if(!lib) return;

    genre_t* genre = lib->genres;
    while(genre){
        if(genre->gid == gid) break;
        genre = genre->next;
    }

    if(!genre){
        printf("IGNORE\n");
        return;
    }

    printf("Genre %d: %s\n", genre->gid, genre->name);

    book_t* book = genre->books;
    if(!book){
        printf("No books \nDONE\n");
        return;
    }

    while(book){
        printf("%d \"%s\" avg=%d reviews=%d sum = %d\n",book->bid, book->title, book->n_reviews, book->sum_scores);
        book = book->next;
    }

    printf("DONE PG\n");
}

void lib_PM(library_t* lib, int sid){
    if(!lib) return;

    member_t* member = lib->members;
    while(member){
        if(member->sid == sid ) break;
        member = member->next;
    }

    if(!member){
        printf("IGNORED\n");
        return;
    }

    printf("Loans:\n");

    if(!member->loans || !member->loans->next){
        return;
    }

    loan_t* current = member->loans->next;
    while(current){
        printf("%d\n",current->bid);
        current = current->next;
    }

    printf("DONE PM\n");
}

void lib_PD(library_t* lib){
    if(!lib) return;

    genre_t* genre = lib->genres;
    while(genre){
        printf("Genre %d: %s\n", genre->gid , genre->name);
        if(genre->display && genre->slots > 0){
            for(int i = 0; i < genre->slots; i++){
                if(genre->display[i]){
                    book_t* book = genre->display[i];
                    printf("%d \"%s\" avg = %d reviews = %d sum = %d\n", book->bid, book->title, book->avg, book->n_reviews, book->sum_scores);
                }
            }
        }
        genre = genre->next;
    }

    printf("DONE PD\n");
}

void lib_PDD(library_t* lib){
    /*if (!lib || SLOTS == 0) return;


    genre_t* g = lib->genres;
    while (g) {
        if (g->display) {
            free(g->display);
            g->display = NULL;
            g->slots = 0;
        }
        g = g->next;
    }


    int total_points = 0;
    g = lib->genres;
    while (g) {
        int points = 0;
        book_t* b = g->books;
        while (b) {
            if (b->n_reviews > 0 && b->lost_flag == 0) {
                points += b->sum_scores;
            }
            b = b->next;
        }
        g->invalid_count = points;
        total_points += points;
        g = g->next;
    }

    if (total_points == 0) return;


    int quota = total_points / SLOTS;

    if (quota == 0) return;


    g = lib->genres;
    while (g) {
        int points = g->invalid_count;
        g->slots = points / quota;
        g->invalid_count = points - (g->slots * quota);
        g = g->next;
    }


    int remaining_slots = SLOTS;
    g = lib->genres;
    while (g) {
        remaining_slots -= g->slots;
        g = g->next;
    }

    while (remaining_slots > 0) {

        genre_t* best = NULL;
        g = lib->genres;
        while (g) {
            if (g->invalid_count > 0) {
                if (!best || g->invalid_count > best->invalid_count ||
                    (g->invalid_count == best->invalid_count && g->gid < best->gid)) {
                    best = g;
                }
            }
            g = g->next;
        }
        if (!best) break;
        best->slots += 1;
        best->invalid_count = 0;
        remaining_slots--;
    }

    g = lib->genres;
    while (g) {
        if (g->slots > 0) {
            g->display = malloc(sizeof(book_t*) * g->slots);
            if (!g->display) {
                perror("MEMORY ALLOCATION FAIL");
                exit(1);
            }
            int count = 0;
            book_t* b = g->books;
            while (b && count < g->slots) {
                if (b->lost_flag == 0) {
                    g->display[count++] = b;
                }
                b = b->next;
            }
            g->slots = count;
        }
        g = g->next;
    }

    printf("DONE\n");

*/
}