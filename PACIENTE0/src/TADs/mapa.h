#ifndef MAPA_H
#define MAPA_H
#include "habpers.h"
#include "objeto.h"

typedef struct _Mapa Mapa;

Mapa * mapa_init ();

void mapa_free (Mapa *m);

Habitacion *mapa_getHab (const Mapa *m, long nId);

Bool mapa_areConnected (const Mapa *m, const long nId1, const long nId2);

int mapa_getPos(const Mapa *m);

int mapa_setPos(Mapa *m, int a);

int mapa_sumPos(Mapa *m, int a);

void mapa_setConnections(Mapa *m, long nId1, long nId2);


#endif /* GRAPH_H */
