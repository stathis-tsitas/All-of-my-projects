#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library.h"

/* dendro avl*/

int height(BookNode *N) {
    if (N == NULL) return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}
/* neos komvos */
BookNode* newNode(book_t* book) {
    BookNode* node = (BookNode*)malloc(sizeof(BookNode));
    if(!node) { perror("AVL Node malloc"); exit(1); }
    strncpy(node->title, book->title, TITLE_MAX);
    node->title[TITLE_MAX-1] = '\0';
    node->book = book;
    node->lc = NULL;
    node->rc = NULL;
    node->height = 1;
    return node;
}
/* deksio rotate */
BookNode *rightRotate(BookNode *y) {
    BookNode *x = y->lc;
    BookNode *T2 = x->rc;
    x->rc = y;
    y->lc = T2;
    y->height = max(height(y->lc), height(y->rc)) + 1;
    x->height = max(height(x->lc), height(x->rc)) + 1;
    return x;
}
/* aristero rotate */
BookNode *leftRotate(BookNode *x) {
    BookNode *y = x->rc;
    BookNode *T2 = y->lc;
    y->lc = x;
    x->rc = T2;
    x->height = max(height(x->lc), height(x->rc)) + 1;
    y->height = max(height(y->lc), height(y->rc)) + 1;
    return y;
}

int getBalance(BookNode *N) {
    if (N == NULL) return 0;
    return height(N->lc) - height(N->rc);
}

/* eisagwgh sto avl  */
BookNode* insertAVL(BookNode* node, book_t* book) {
    if (node == NULL) return newNode(book);

    int cmp = strcmp(book->title, node->title);
    if (cmp < 0)
        node->lc = insertAVL(node->lc, book);
    else if (cmp > 0)
        node->rc = insertAVL(node->rc, book);
    else return node;
    /* eisagwgh me bash ta rotate rr rl ll lr*/
    node->height = 1 + max(height(node->lc), height(node->rc));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(book->title, node->lc->title) < 0)
        return rightRotate(node);
    if (balance < -1 && strcmp(book->title, node->rc->title) > 0)
        return leftRotate(node);
    if (balance > 1 && strcmp(book->title, node->lc->title) > 0) {
        node->lc = leftRotate(node->lc);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(book->title, node->rc->title) < 0) {
        node->rc = rightRotate(node->rc);
        return leftRotate(node);
    }
    return node;
}

BookNode *minValueNode(BookNode* node) {
    BookNode* current = node;
    while (current->lc != NULL) current = current->lc;
    return current;
}

/* diagraphi node  */
BookNode* deleteNodeAVL(BookNode* root, char* title) {
    if (root == NULL) return root;

    int cmp = strcmp(title, root->title);
    if (cmp < 0)
        root->lc = deleteNodeAVL(root->lc, title);
    else if (cmp > 0)
        root->rc = deleteNodeAVL(root->rc, title);
    else {
        if ((root->lc == NULL) || (root->rc == NULL)) {
            BookNode *temp = root->lc ? root->lc : root->rc;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            BookNode *temp = minValueNode(root->rc);
            strncpy(root->title, temp->title, TITLE_MAX);
            root->book = temp->book;
            root->rc = deleteNodeAVL(root->rc, temp->title);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->lc), height(root->rc));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->lc) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->lc) < 0) {
        root->lc = leftRotate(root->lc);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->rc) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->rc) > 0) {
        root->rc = rightRotate(root->rc);
        return leftRotate(root);
    }
    return root;
}
/* anazhthsh se log n*/
BookNode* searchAVL(BookNode* root, const char* title) {
    if (root == NULL || strcmp(root->title, title) == 0)
        return root;
    if (strcmp(title, root->title) < 0)
        return searchAVL(root->lc, title);
    return searchAVL(root->rc, title);
}
/*free olo to alv dendro */
void freeAVL(BookNode* root) {
    if(root) {
        freeAVL(root->lc);
        freeAVL(root->rc);
        free(root);
    }
}

/* Heap */
int is_greater(book_t* a, book_t* b) {
    if (a->avg > b->avg) return 1;
    if (a->avg < b->avg) return 0;
    return (a->bid < b->bid);
}

void swap_heap(RecHeap* rh, int i, int j) {
    book_t* temp = rh->heap[i];
    rh->heap[i] = rh->heap[j];
    rh->heap[j] = temp;
    rh->heap[i]->heap_pos = i;
    rh->heap[j]->heap_pos = j;
}

void heapify_up(RecHeap* rh, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (is_greater(rh->heap[idx], rh->heap[parent])) {
            swap_heap(rh, idx, parent);
            idx = parent;
        } else {
            break;
        }
    }
}

void heapify_down(RecHeap* rh, int idx) {
    int size = rh->size;
    while (1) {
        int largest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < size && is_greater(rh->heap[left], rh->heap[largest]))
            largest = left;
        if (right < size && is_greater(rh->heap[right], rh->heap[largest]))
            largest = right;

        if (largest != idx) {
            swap_heap(rh, idx, largest);
            idx = largest;
        } else {
            break;
        }
    }
}
/* eisagwgh sthn swro*/
void heap_insert(RecHeap* rh, book_t* b) {
    if (b->heap_pos != -1) {
        heapify_up(rh, b->heap_pos);
        heapify_down(rh, b->heap_pos);
        return;
    }
    if (rh->size >= HEAP_CAPACITY) return;

    int idx = rh->size;
    rh->heap[idx] = b;
    b->heap_pos = idx;
    rh->size++;
    heapify_up(rh, idx);
}
/* diagraphi*/
void heap_remove(RecHeap* rh, book_t* b) {
    if (b->heap_pos == -1) return;

    int idx = b->heap_pos;
    swap_heap(rh, idx, rh->size - 1);
    rh->size--;
    b->heap_pos = -1;

    if (idx < rh->size) {
        heapify_up(rh, idx);
        heapify_down(rh, idx);
    }
}

/* palies + kainoyrgies synarthseis */

void lib_S(library_t* lib, int slots){
    if(slots < 0) slots = 0;
    SLOTS = slots;
    printf("DONE\n");
}

void lib_G(library_t* lib, int gid, const char* name){
    if(!lib || !name) return;
    genre_t* cur = lib->genres;
    genre_t* prev = NULL;
    while(cur){
        if(cur->gid == gid) { printf("IGNORED\n"); return; }
        if(cur->gid > gid) break;
        prev = cur;
        cur = cur->next;
    }

    genre_t* ng = malloc(sizeof(genre_t));
    if (!ng) exit(1);

    ng->gid = gid;
    strncpy(ng->name, name, NAME_MAX);
    ng->name[NAME_MAX - 1] = '\0';
    ng->books = NULL;
    ng->lost_count = 0;
    ng->invalid_count = 0;
    ng->slots = 0;
    ng->display = NULL;
    ng->next = NULL;


    ng->bookIndex = NULL;

    if(!prev){
        ng->next = lib->genres;
        lib->genres = ng;
    }else {
        ng->next = prev->next;
        prev->next = ng;
    }
    printf("DONE\n");
}

void lib_BK(library_t* lib, int bid, int gid, const char* title) {
    if (!lib || !title) return;

    genre_t* g = lib->genres;
    while (g && g->gid != gid) g = g->next;
    if (!g) { printf("IGNORED\n"); return; }

    book_t* cur = g->books;
    while (cur) {
        if (cur->bid == bid) { printf("IGNORED\n"); return; }
        cur = cur->next;
    }

    if(searchAVL(g->bookIndex, title)) {
        printf("IGNORED\n"); return;
    }

    book_t* nb = malloc(sizeof(book_t));
    if (!nb) exit(1);

    nb->bid = bid;
    nb->gid = gid;
    strncpy(nb->title, title, TITLE_MAX);
    nb->title[TITLE_MAX - 1] = '\0';
    nb->sum_scores = 0;
    nb->n_reviews = 0;
    nb->avg = 0;
    nb->lost_flag = 0;
    nb->heap_pos = -1;
    nb->next = NULL;
    nb->prev = NULL;

    if(!g->books){
        g->books = nb;
    }else{
        book_t* temp = g->books;
        while(temp->next) temp = temp->next;
        temp->next = nb;
        nb->prev = temp;
    }

    g->bookIndex = insertAVL(g->bookIndex, nb);

    printf("DONE\n");
}

void lib_M(library_t* lib, int sid, const char* name){
    if(!lib || !name) return;
    member_t* current = lib->members;
    member_t* prev = NULL;

    while(current){
        if(current->sid == sid){ printf("IGNORED\n"); return; }
        if(current->sid > sid) break;
        prev = current;
        current = current->next;
    }

    member_t* newmember = malloc(sizeof(member_t));
    if(!newmember) exit(1);

    newmember->sid = sid;
    strncpy(newmember->name, name, NAME_MAX);
    newmember->name[NAME_MAX - 1] = '\0';

    loan_t* sentinel = malloc(sizeof(loan_t));
    if(!sentinel) exit(1);
    sentinel->next = NULL;
    newmember->loans = sentinel;

    MemberActivity* act = malloc(sizeof(MemberActivity));
    if(!act) exit(1);
    act->sid = sid;
    act->loans_count = 0;
    act->reviews_count = 0;
    act->score_sum = 0;

    act->next = lib->activity;
    lib->activity = act;

    newmember->activity = act;

    if(!prev){
        newmember->next = lib->members;
        lib->members = newmember;
    }else{
        newmember->next = prev->next;
        prev->next = newmember;
    }
    printf("DONE\n");
}

void lib_L(library_t* lib, int sid, int bid){
    if(!lib) return;

    member_t* member = lib->members;
    while(member && member->sid != sid) member = member->next;
    if(!member) { printf("IGNORED\n"); return; }

    genre_t* genre = lib->genres;
    book_t* book = NULL;
    while(genre && !book){
        book_t* current = genre->books;
        while(current){
            if(current->bid == bid){ book = current; break; }
            current = current->next;
        }
        genre = genre->next;
    }
    if(!book) { printf("IGNORED\n"); return; }
    if(book->lost_flag) { printf("IGNORED\n"); return; }

    loan_t* cl = member->loans->next;
    while(cl){
        if(cl->bid == bid) { printf("IGNORED\n"); return; }
        cl = cl->next;
    }

    loan_t* newloan = malloc(sizeof(loan_t));
    if(!newloan) exit(1);
    newloan->sid = sid;
    newloan->bid = bid;
    newloan->next = member->loans->next;
    member->loans->next = newloan;

    if(member->activity) member->activity->loans_count++;

    printf("DONE\n");
}

void lib_R(library_t* lib, int sid ,int bid , int score, const char* status) {
    if (!lib || !status) return;

    member_t *member = lib->members;
    while (member && member->sid != sid) member = member->next;
    if (!member) { printf("IGNORED\n"); return; }

    loan_t *prev = member->loans;
    loan_t *curr = member->loans->next;
    while (curr && curr->bid != bid) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) { printf("IGNORED\n"); return; }

    genre_t *genre = lib->genres;
    book_t *book = NULL;
    while (genre) {
        book = genre->books;
        while (book) {
            if (book->bid == bid) break;
            book = book->next;
        }
        if (book) break;
        genre = genre->next;
    }

    prev->next = curr->next;
    free(curr);

    if (strcmp(status, "lost") == 0) {
        book->lost_flag = 1;
        genre->lost_count++;
        heap_remove(lib->recommendations, book);
        genre->bookIndex = deleteNodeAVL(genre->bookIndex, book->title);
        printf("DONE\n");
    } else if (strcmp(status, "ok") == 0) {
        if (score >= 0 && score <= 10) {
            book->sum_scores += score;
            book->n_reviews++;
            book->avg = (book->sum_scores / book->n_reviews);

            if(member->activity) {
                member->activity->reviews_count++;
                member->activity->score_sum += score;
            }

            heap_insert(lib->recommendations, book);


            if (book->prev) book->prev->next = book->next;
            else genre->books = book->next;
            if (book->next) book->next->prev = book->prev;

            book_t *scan = genre->books;
            while (scan && scan->avg >= book->avg) scan = scan->next;

            if (!scan) {
                if (!genre->books) {
                    genre->books = book; book->prev = NULL; book->next = NULL;
                } else {
                    book_t *tail = genre->books;
                    while (tail->next) tail = tail->next;
                    tail->next = book; book->prev = tail; book->next = NULL;
                }
            } else {
                book->next = scan;
                book->prev = scan->prev;
                if (scan->prev) scan->prev->next = book;
                else genre->books = book;
                scan->prev = book;
            }
            printf("DONE\n");
        } else if (score == -1) {
            printf("DONE\n");
        } else {
            genre->invalid_count++;
            printf("IGNORED\n");
        }
    } else {
        printf("IGNORED\n");
    }
}
/* boithitiko struct */
typedef struct {
    genre_t* g; int points; int current_slots; int remainder;
} dist_helper_t;

void lib_D(library_t* lib){
    if(!lib) return;
    int genre_count = 0;
    for(genre_t* g = lib->genres; g; g = g->next){
        if(g->display){ free(g->display); g->display = NULL; }
        g->slots = 0;
        genre_count++;
    }
    if (genre_count == 0 || SLOTS == 0) { printf("DONE\n"); return; }

    dist_helper_t* helpers = malloc(sizeof(dist_helper_t) * genre_count);
    if (!helpers) exit(1);

    int total_valid = 0;
    int idx = 0;
    for(genre_t* g = lib->genres; g; g = g->next){
        int p = 0;
        for(book_t* b = g->books; b; b = b->next){
            if(b->n_reviews > 0 && b->lost_flag == 0) p += b->sum_scores;
        }
        helpers[idx].g = g; helpers[idx].points = p;
        helpers[idx].current_slots = 0; helpers[idx].remainder = 0;
        total_valid += p;
        idx++;
    }

    if (total_valid == 0) { free(helpers); printf("DONE\n"); return; }
    int quota = total_valid / SLOTS;
    int used_slots = 0;
    if (quota > 0) {
        for(int i=0; i<genre_count; i++){
            helpers[i].current_slots = helpers[i].points / quota;
            helpers[i].remainder = helpers[i].points - (helpers[i].current_slots * quota);
            used_slots += helpers[i].current_slots;
        }
    } else {
        for(int i=0; i<genre_count; i++) helpers[i].remainder = helpers[i].points;
    }

    int remaining = SLOTS - used_slots;
    while (remaining > 0) {
        int max_rem = -1; int best_idx = -1;
        for(int i=0; i<genre_count; i++){
            if (helpers[i].remainder > max_rem) { max_rem = helpers[i].remainder; best_idx = i; }
        }
        if (best_idx == -1 || max_rem <= 0) break;
        helpers[best_idx].current_slots++;
        helpers[best_idx].remainder = -1;
        remaining--;
    }

    for(int i=0; i<genre_count; i++){
        genre_t* g = helpers[i].g;
        g->slots = helpers[i].current_slots;
        if(g->slots > 0){
            g->display = malloc(sizeof(book_t*) * g->slots);
            int count = 0;
            for(book_t* b = g->books; b && count < g->slots; b = b->next){
                if(b->lost_flag == 0 && b->n_reviews > 0) g->display[count++] = b;
            }
            while(count < g->slots) g->display[count++] = NULL;
        }
    }
    free(helpers);
    printf("DONE\n");
}

void lib_PG(library_t* lib, int gid){
    if(!lib) return;
    genre_t* genre = lib->genres;
    while(genre && genre->gid != gid) genre = genre->next;
    if(!genre){ printf("IGNORED\n"); return; }
    printf("Genre %d: %s\n", genre->gid, genre->name);
    book_t* book = genre->books;
    while(book){
        printf("%d \"%s\" avg=%d reviews=%d sum=%d\n",
               book->bid, book->title, book->avg, book->n_reviews, book->sum_scores);
        book = book->next;
    }
}

void lib_PM(library_t* lib, int sid){
    if(!lib) return;
    member_t* member = lib->members;
    while(member && member->sid != sid) member = member->next;
    if(!member){ printf("IGNORED\n"); return; }
    printf("Loans:\n");
    loan_t* cur = member->loans->next;
    while(cur){ printf("%d\n", cur->bid); cur = cur->next; }
}

void lib_PD(library_t* lib){
    if(!lib) return;
    printf("Display:\n");
    genre_t* genre = lib->genres;
    while(genre){
        printf("Genre %d: %s\n", genre->gid , genre->name);
        if(genre->display && genre->slots > 0){
            for(int i = 0; i < genre->slots; i++){
                if(genre->display[i]){
                    book_t* book = genre->display[i];
                    printf("%d \"%s\" avg=%d reviews=%d sum=%d\n", book->bid, book->title, book->avg, book->n_reviews, book->sum_scores);
                }
            }
        }
        genre = genre->next;
    }
}

/* anazitisi book me titlto sto avl*/
void lib_F(library_t* lib, const char* title) {
    if (!lib || !title) return;
    genre_t* g = lib->genres;
    BookNode* res = NULL;

    while (g) {
        res = searchAVL(g->bookIndex, title);
        if (res) break;
        g = g->next;
    }

    if (res && res->book) {
        printf("Book %d \"%s\" avg=%d\n", res->book->bid, res->book->title, res->book->avg);
    } else {
        printf("NOT FOUND\n");
    }
}
/* emfanisi top vivliwn */
void lib_TOP(library_t* lib, int k) {
    if (!lib || !lib->recommendations) return;
    RecHeap* rh = lib->recommendations;

    printf("Top Books:\n");
    if (rh->size == 0) return;

    book_t** temp_arr = malloc(sizeof(book_t*) * rh->size);
    if (!temp_arr) exit(1);
    for(int i=0; i<rh->size; i++) temp_arr[i] = rh->heap[i];

    int temp_size = rh->size;
    int limit = (k < temp_size) ? k : temp_size;

    for(int i=0; i<limit; i++) {
        int max_idx = i;
        for(int j=i+1; j<temp_size; j++) {
            if (is_greater(temp_arr[j], temp_arr[max_idx])) {
                max_idx = j;
            }
        }
        book_t* tmp = temp_arr[i];
        temp_arr[i] = temp_arr[max_idx];
        temp_arr[max_idx] = tmp;

        printf("%d \"%s\" avg=%d\n", temp_arr[i]->bid, temp_arr[i]->title, temp_arr[i]->avg);
    }
    free(temp_arr);
}
/* anazitisi melwn mebash to activity*/
void lib_AM(library_t* lib) {
    if (!lib) return;
    printf("Active Members:\n");

    int count = 0;
    MemberActivity* temp = lib->activity;
    while(temp) { count++; temp = temp->next; }

    if(count == 0) return;

    MemberActivity** arr = malloc(sizeof(MemberActivity*) * count);
    temp = lib->activity;
    int i = 0;
    while(temp) { arr[i++] = temp; temp = temp->next; }

    for(int i=0; i<count-1; i++){
        for(int j=0; j<count-i-1; j++){
            int scoreA = arr[j]->loans_count + arr[j]->reviews_count;
            int scoreB = arr[j+1]->loans_count + arr[j+1]->reviews_count;
            if(scoreB > scoreA || (scoreB == scoreA && arr[j+1]->sid < arr[j]->sid)){
                MemberActivity* swp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = swp;
            }
        }
    }

    for(int i=0; i<count; i++){
        int score = arr[i]->loans_count + arr[i]->reviews_count;
        if(score > 0) {
            member_t* m = lib->members;
            while(m && m->sid != arr[i]->sid) m = m->next;
            if(m) {
                printf("%d %s loans=%d reviews=%d\n",
                       m->sid, m->name, arr[i]->loans_count, arr[i]->reviews_count);
            }
        }
    }
    free(arr);
}
/* ananewsh avl dendroy me title  */

void lib_U(library_t* lib, int bid, const char* new_title) {
    if(!lib || !new_title) return;

    genre_t* g = lib->genres;
    book_t* book = NULL;
    while(g){
        book_t* b = g->books;
        while(b){
            if(b->bid == bid){ book = b; break; }
            b = b->next;
        }
        if(book) break;
        g = g->next;
    }

    if(!book) { printf("IGNORED\n"); return; }

    g->bookIndex = deleteNodeAVL(g->bookIndex, book->title);

    strncpy(book->title, new_title, TITLE_MAX);
    book->title[TITLE_MAX-1] = '\0';

    g->bookIndex = insertAVL(g->bookIndex, book);

    printf("DONE\n");
}
/* stoixeia vivliothikhs */

void lib_X(library_t* lib) {
    int books = 0;
    int members = 0;
    int loans = 0;
    double total_avg = 0;
    int rated_books = 0;

    genre_t* g = lib->genres;
    while(g){
        book_t* b = g->books;
        while(b){
            books++;
            if(b->n_reviews > 0) {
                total_avg += b->avg;
                rated_books++;
            }
            b = b->next;
        }
        g = g->next;
    }

    member_t* m = lib->members;
    while(m){
        members++;
        loan_t* l = m->loans->next;
        while(l) { loans++; l = l->next; }
        m = m->next;
    }

    double global_avg = (rated_books > 0) ? (total_avg / rated_books) : 0.0;

    printf("Stats: books=%d members=%d loans=%d avg=%.2f\n", books, members, loans, global_avg);
}
/* free structs kai domwn */

void lib_BF(library_t* lib) {
    if(!lib) return;

    genre_t* g = lib->genres;
    while(g){
        genre_t* next_g = g->next;

        freeAVL(g->bookIndex);
        if(g->display) free(g->display);

        book_t* b = g->books;
        while(b){
            book_t* next_b = b->next;
            free(b);
            b = next_b;
        }
        free(g);
        g = next_g;
    }
    lib->genres = NULL;

    member_t* m = lib->members;
    while(m){
        member_t* next_m = m->next;
        loan_t* l = m->loans;
        while(l){
            loan_t* next_l = l->next;
            free(l);
            l = next_l;
        }
        free(m);
        m = next_m;
    }
    lib->members = NULL;

    MemberActivity* ma = lib->activity;
    while(ma){
        MemberActivity* next_ma = ma->next;
        free(ma);
        ma = next_ma;
    }
    lib->activity = NULL;

    if(lib->recommendations) free(lib->recommendations);
    lib->recommendations = NULL;

    printf("DONE\n");
}