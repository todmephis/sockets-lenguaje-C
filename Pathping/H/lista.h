#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #include <unistd.h>

typedef struct node {
    unsigned char IPlist[4];
    unsigned int TTLlist;
    unsigned int contestados_ttl_c;
    unsigned int contestados_ttl_system;
    float tiempo_ttl_c;
    float tiempo_ttl_system;
    struct node * siguiente;
} node_t;
void push (node_t *, unsigned char *, unsigned int);
void print_list(node_t *);
int pop(node_t **);
void chingateLaLista(node_t **);
void imprimeResultados(node_t *);
int cuentaNodos(node_t *);