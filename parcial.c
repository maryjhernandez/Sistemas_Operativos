#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESOS 10
#define NUM_COLAS 3
#define TIME_CU_0 5
#define TIME_CU_1 10
#define TIME_CU_2 20

typedef struct {
    int id;
    int time_ra;
    int prioridad;
    int time_us;
} Proceso;

typedef struct {
    Proceso *cola[MAX_PROCESOS];
    int frente, final;
} Cola;

void inicializarCola(Cola *q) {
    q->frente = q->final = -1;
}

int estaVacía(Cola *q) {
    return (q->frente == -1);
}

void encolar(Cola *q, Proceso *proceso) {
    if (q->final == MAX_PROCESOS - 1) {
        printf("La cola está llena\n");
        exit(EXIT_FAILURE);
    }

    if (q->frente == -1)
        q->frente = 0;
    q->final++;
    q->cola[q->final] = proceso;
}

Proceso *desencolar(Cola *q) {
    if (estaVacía(q)) {
        printf("La cola está vacía\n");
        exit(EXIT_FAILURE);
    }

    Proceso *proceso = q->cola[q->frente];
    if (q->frente == q->final)
        q->frente = q->final = -1;
    else
        q->frente++;
    return proceso;
}

void ejecutarMLFQ(Cola colas[NUM_COLAS]) {
    int cola_actual = 0;
    while (1) {
        // Encuentra la primera cola no vacía a partir de la cola actual
        for (int i = cola_actual; i < NUM_COLAS; i++) {
            if (!estaVacía(&colas[i])) {
                cola_actual = i;
                break;
            }
            if (i == NUM_COLAS - 1) {
                // Todas las colas están vacías, hemos terminado
                return;
            }
        }

        Proceso *p_actual = desencolar(&colas[cola_actual]);
        int time_cu = (cola_actual == 0) ? TIME_CU_0 : (cola_actual == 1) ? TIME_CU_1 : TIME_CU_2;
        while (p_actual->time_us < time_cu && p_actual->time_ra > 0) {
            p_actual->time_ra--;
            p_actual->time_us++;
        }
        if (p_actual->time_ra > 0) {
            if (cola_actual < NUM_COLAS - 1) {
                p_actual->prioridad++;
                encolar(&colas[cola_actual + 1], p_actual);
            } else {
                encolar(&colas[cola_actual], p_actual);
            }
        } else {
            printf("Proceso %d completado.\n", p_actual->id);
            free(p_actual);
        }
        cola_actual = (cola_actual + 1) % NUM_COLAS;
    }
}

int main() {
    Cola colas[NUM_COLAS];
    for (int i = 0; i < NUM_COLAS; i++) {
        inicializarCola(&colas[i]);
    }

    Proceso *proceso1 = malloc(sizeof(Proceso));
    proceso1->id = 1;
    proceso1->time_ra = 10;
    proceso1->prioridad = 0;
    proceso1->time_us = 0;
    Proceso *proceso2 = malloc(sizeof(Proceso));
    proceso2->id = 2;
    proceso2->time_ra = 5;
    proceso2->prioridad = 0;
    proceso2->time_us = 0;
    Proceso *proceso3 = malloc(sizeof(Proceso));
    proceso3->id = 3;
    proceso3->time_ra = 15;
    proceso3->prioridad = 0;
    proceso3->time_us = 0;

    encolar(&colas[0], proceso1);
    encolar(&colas[0], proceso2);
    encolar(&colas[0], proceso3);

    clock_t inicio = clock();
    ejecutarMLFQ(colas);
    clock_t fin = clock();

    double tiempo_total = (double)(fin - inicio) / CLOCKS_PER_SEC;
    double tiempo_real = (double)fin / CLOCKS_PER_SEC;

    printf("Tiempo:\n");
    printf("Tiempo total de ejecución en C: %f segundos\n", tiempo_total);
    printf("Tiempo real de ejecución en C: %f segundos\n", tiempo_real);

    return 0;
}