#include <lista.h>
void push (node_t * cabeza, unsigned char * direccionIPNueva, unsigned int TTL){
	node_t * actual = cabeza;
	while(actual->siguiente != NULL){
		actual = actual->siguiente;
	}
	actual->siguiente= malloc(sizeof(node_t));
	memcpy(actual->siguiente->IPlist, direccionIPNueva, 4);
	actual->siguiente->TTLlist=TTL;
	actual->siguiente->contestados_ttl_c=0;
	actual->siguiente->contestados_ttl_system=0;
	actual->siguiente->siguiente=NULL;
	}

void print_list(node_t * lista){
	node_t * actual = lista;

	while(actual != NULL){
		printf("IP >> %d.%d.%d.%d ", actual->IPlist[0], actual->IPlist[1], actual->IPlist[2], actual->IPlist[3]);
		printf("TTL >> %d ", actual->TTLlist);
		printf("Contestados >> %d Contestados TTl sistema >> %d\n", actual->contestados_ttl_c, actual->contestados_ttl_system);
		actual = actual->siguiente;
	}

}

int pop(node_t ** head) {
    int retval = -1;
    node_t * next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_node = (*head)->siguiente;
    //retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}
