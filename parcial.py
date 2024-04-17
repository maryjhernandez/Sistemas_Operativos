import time

class Proceso:
    def __init__(self, pid, tiempo_ra):
        self.pid = pid
        self.tiempo_ra = tiempo_ra
        self.time_res = tiempo_ra
        self.prioridad = 0

def ejecutar_proceso(proce, time_cua):
    tiempo_inicio = time.time()
    while proce.time_res > 0:
        if proce.time_res > time_cua:
            proce.time_res -= time_cua
            proce.prioridad += 1
        else:
            proce.time_res = 0
    tiempo_fin = time.time()
    return tiempo_fin - tiempo_inicio

def mlfq(procesos, time_cua):
    colas = [[] for _ in range(3)]
    for proceso in procesos:
        colas[0].append(proceso)
    tiempo_total = 0
    while any(cola for cola in colas):
        for cola_idx in range(3):
            if colas[cola_idx]:
                proceso = colas[cola_idx].pop(0)
                tiempo_ejecucion = ejecutar_proceso(proceso, time_cua)
                tiempo_total += tiempo_ejecucion
                if proceso.time_res > 0:
                    colas[min(cola_idx + 1, 2)].append(proceso)
    return tiempo_total

procesos = [Proceso(1, 5), Proceso(2, 3), Proceso(3, 7)]
time_cua = 2

tiempo_inicio = time.time()
tiempo_total = mlfq(procesos, time_cua)
tiempo_fin = time.time()

print("Tiempos:")
print(f"Tiempo total de ejecución en Python: {tiempo_total:.6f} segundos")
print(f"Tiempo real de ejecución en Python: {tiempo_fin - tiempo_inicio:.6f} segundos")