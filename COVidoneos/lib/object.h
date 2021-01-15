#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"

#define TAM_MALETIN 4

typedef enum {
    MASCARILLA=0,
    PCR=1,
    GUANTES=2,
    GEL_HIDROALCOHOLICO=3
} ObjectId;

typedef struct _Object Object;

Object** setObjects();

char* ObjectGetName(Object *obj);

int ObjectUse(Object *obj);

Bool ObjectIsUsed(Object *obj);

void ObjectFree(Object *obj);

void MaletinFree(Object **maletin);

#endif
