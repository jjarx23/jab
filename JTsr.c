#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "JTsr.h"
#include "JTsr.r.h"
#define UNIT_NAME "JTnsr"

static JTsrClass_st Class;
const void *JTsr = 0;

// assigning inheritance and overrides
static void __attribute__((constructor)) jtsrClassf()
{
    if (JTsr)
        return;
    mut(JTsr, void *, &Class);
    memcpy((void *)JTsr, Obj, sizeof(ObjClass_st));
    (*(ObjClass_t)&Class).size = sizeof(JTsr_st);
    (*(ObjClass_t)&Class).super = Obj;
    (*(ObjClass_t)&Class).rpr = 0;    
    static const char name[] = UNIT_NAME;
    (*(ObjClass_t)&Class).name = name;
    // rpr;
}
