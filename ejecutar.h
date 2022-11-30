#ifndef EJECUTAR_H
#define EJECUTAR_H
void ejecutar_linea_ordenes(const char *orden);
int crear_pipes(int nordenes);
pid_t ejecutar_orden(const char *orden, int *pbackgr);
#endif
