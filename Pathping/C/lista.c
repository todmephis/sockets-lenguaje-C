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
	actual->siguiente->tiempo_ttl_c=0;
	actual->siguiente->tiempo_ttl_system=0;
	actual->siguiente->siguiente=NULL;
	}

void print_list(node_t * lista){
	node_t * actual = lista;

	while(actual != NULL){
		printf("IP >> %d.%d.%d.%d | ", actual->IPlist[0], actual->IPlist[1], actual->IPlist[2], actual->IPlist[3]);
		printf("TTL >> %d | ", actual->TTLlist);
		printf("Contestados >> %d | Contestados TTl sistema >> %d | ", actual->contestados_ttl_c, actual->contestados_ttl_system);
		printf("Tiempo Acumulado ttl c >> %f | Tiempo Acumulador ttl sys >> %f\n\n", actual->tiempo_ttl_c, actual->tiempo_ttl_system);
		actual = actual->siguiente;
	}

}

int pop(node_t ** head) {
    int retval = -1;
    node_t * next_node = NULL;
    if (*head == NULL)
        return -1;
    next_node = (*head)->siguiente;
    free(*head);
    *head = next_node;
    return retval;
}

void chingateLaLista(node_t **head){
	node_t * actual = *head;
	node_t * next_node;
	while(actual != NULL){
		next_node = actual->siguiente;
		free(actual);
		actual=next_node;
	}
	*head=NULL;
}
void imprimeResultados(node_t * lista){
	node_t * actual = lista;
    while(actual !=NULL){
        sleep(1);
        printf("%d \t     ",actual->TTLlist);
        if(actual->contestados_ttl_c == 0)
        	printf("%d/100\t\t-------\t\t", 100-(actual->contestados_ttl_c));
        else
        	printf("%d/100\t\t%.2fms\t\t", 100-(actual->contestados_ttl_c), actual->tiempo_ttl_c/(float)actual->contestados_ttl_c);

        if(actual->contestados_ttl_system == 0)
        	printf("%d/100\t\t-------\t  ", 100-(actual->contestados_ttl_system));
        else
        	printf("%d/100\t\t%.2fms\t  ", 100-(actual->contestados_ttl_system), actual->tiempo_ttl_system/(float)actual->contestados_ttl_system);
        printf("%d.%d.%d.%d\n", actual->IPlist[0], actual->IPlist[1], actual->IPlist[2], actual->IPlist[3]);
        
        actual = actual->siguiente;
    }
}

int cuentaNodos(node_t *temp){
 if(temp == NULL)
     return(0);
 return(1 + cuentaNodos(temp->siguiente));
}