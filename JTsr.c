#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "JTsr.h"
#include "JTsr.r.h"

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
    // rpr;
}
