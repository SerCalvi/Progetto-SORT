#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/************* Funzioni locali *********************************/

/* Confronta due elementi della lista:
   - ritorna un valore >0 se item1 > item2;
   - ritorna un valore <0 se item1 < item2;
   - ritorna un valore ==0 se item1 == item2.
  NB: puo' essere utilizzata nelle funzioni di ricerca e/o ordinamento
*/


/* alloca un nuovo nodo per la lista, copiandone all'interno l'elemento item */
NODE* createNode(ItemType item) {
    NODE* p = (NODE*)malloc(sizeof(NODE));
    assert(p != NULL);

    p->item = item;
    p->next = NULL;
    return p;
}

/* dealloca il nodo p */
void deleteNode(NODE* p) {
    free(p);
}

/********** Funzioni standard *******************************/

/* Inizializza una nuova lista */
LIST NewList() {
    return NULL;
}

int itemCompare(ItemType item1, ItemType item2) {
    if (strcmp(item1.nome, item2.nome) == 0) /*** esempio ***/
        return 0;
    else
        return 1;
}

/* Azzera la lista */
LIST DeleteList(LIST l) {
    LIST tmp = l;

    /* dealloca tutti i nodi della lista */
    while (!isEmpty(tmp)) {
        NODE* todel = tmp;
        tmp = tmp->next;
        deleteNode(todel);
    }

    /* la lista e' ora vuota, e puo' essere utilizzata di nuovo */
    return NewList();
}

/* ritorna TRUE se la lista e' vuota */
BOOL isEmpty(LIST l) {
    return l == NULL;
}

/* ritorna il numero di elementi presenti nella lista */
int getLength(LIST l) {
    int size = 0;
    LIST tmp = l;

    while (!isEmpty(tmp)) {
        ++size;
        tmp = tmp->next;
    }

    return size;
}

/* Restituisce l'elemento in testa alla lista, senza modificarla */
ItemType getHead(LIST l) {
    assert(!isEmpty(l));
    return l->item;
}

/* Restituisce l'elemento in coda alla lista, senza modificarla */
ItemType getTail(LIST l) {
    NODE* tmp = l;
    assert(!isEmpty(l));

    while (!isEmpty(tmp->next))
        tmp = tmp->next;

    return tmp->item;
}

/* Cerca l'elemento item nella lista e ne restituisce il puntatore
 ritorna NULL se non lo trova */
LIST Find(LIST l, ItemType item) {
    LIST tmp = l;
    /*if (tmp == NULL) {
      return NULL;
    }*/
    while (tmp != NULL && itemCompare(tmp->item, item) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        return NULL;
    }
    return tmp;
}

/* Inserisce un elemento nella prima posizione della lista */
LIST EnqueueFirst(LIST l, ItemType item) {

    NODE* newNode = createNode(item);
    newNode->next = l;

    return newNode;
}

/* Inserisce un elemento nell'ultima posizione della lista */
LIST EnqueueLast(LIST l, ItemType item) {
    NODE* new_node = createNode(item);

    if (isEmpty(l)) {
        /* Lista vuota: inserimento in testa */
        l = new_node;
    } else {
        LIST tmp = l;
        while (!isEmpty(tmp->next))
            tmp = tmp->next;

        tmp->next = new_node;
    }

    return l;
}

/* Inserisce un elemento mantenendo la lista ordinata */
/*
LIST EnqueueOrdered(LIST l, ItemType item) {
    NODE* new_node = createNode(item);

    if (isEmpty(l)) {
        l = new_node;
        printf("ho inserito il primo elemento\n");
    } else {
        int i = 0;
        int c = 0;
        LIST tmp = l;
        LIST tmp_prev = tmp;
        while(!isEmpty(tmp)){
            if(tmp->item.value > new_node->item.value && i == 0 && c == 0){
                new_node->next = tmp;
                l = new_node;
                c = 1;
            }
            else if(tmp->item.value > new_node->item.value && i != 0 && c == 0){
                new_node->next = tmp;
                tmp_prev->next = new_node;
                c = 1;
            }
            tmp_prev = tmp;
            tmp = tmp->next;
            i++;
        }
        printf("%d\n", i);
        if(c == 0){
            tmp_prev->next = new_node;
        }
    }
    return l;
}
*/
/* Toglie il primo elemento della lista (se non e' vuota) */
LIST DequeueFirst(LIST l) {
    if (!isEmpty(l)) {
        NODE* todel = l;
        l = l->next;
        deleteNode(todel);
    }
    return l;
}

/* Toglie l'ultimo elemento della lista (se non e' vuota) */
LIST DequeueLast(LIST l) {
    if (!isEmpty(l)) {
        assert(FALSE);

        /* TODO */
    }

    return l;
}

/* Toglie l'elemento item dalla lista (se e' presente) */
LIST Dequeue(LIST l, ItemType item) {
    if (!isEmpty(l)) {
        LIST tmp = l;
        LIST tmp_prev;
        int c = 0;
        while (!isEmpty(tmp)){
            if(itemCompare(tmp->item, item) == 0 && c == 0){
                NODE* todel = l;
                l = l->next;
                deleteNode(todel);
            }
            else if(itemCompare(tmp->item, item) == 0 && c!= 0){
                NODE* todel = tmp;
                tmp_prev->next = tmp->next;
                deleteNode(todel);
            }
            c++;
            tmp_prev = tmp;
            tmp = tmp->next;
        }
        
    }
    return l;
}

int ThereIsItem(LIST l, ItemType item){
    int c = 0;
    LIST tmp = l;
    while(!isEmpty(tmp)){
        if(itemCompare(tmp->item, item) == 0)
            c = 1;
        tmp = tmp->next;
    }
    return c;
}

/* Stampa a video un elemento della lista */
void PrintItem(ItemType item) {
    /*** esempio ***/
    printf("%s ha fatto %d partite e %d punti\n", item.nome, item.partite, item.punti);
}

/* Stampa a video la lista (esegue PrintItem() su tutti gli elementi) */
void PrintList(LIST l) {
    LIST tmp = l;
    printf("\n");
    while (!isEmpty(tmp)) {
        PrintItem(tmp->item);
        tmp = tmp->next;
    }
    printf("\n");
}
