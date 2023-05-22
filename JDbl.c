#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <Flow.h>
#include "JDbl.h"
#include "JDbl.r.h"
#define UNIT_NAME "JDbl"

static void *jdbl(double d)
{
    return neu(JDbl, d);
}

static void cstr(void *obj, va_list *arg)
{
    JDbl_t o = obj;
    setN(o, va_arg(*arg, double));
}
static int rpr(const void *b, char *str, int length)
{
    JDbl_t o = (JDbl_t)b;
    return snprintf(str, length, "%g", o->n);
}
static JDblClass_st Class;
const void *JDbl = 0;

static void __attribute__((constructor)) jdblClassf()
{
    if (JDbl)
        return;
    // assigning inheritance and overrides
    mut(JDbl, void *, &Class);
    memcpy((void *)JDbl, Obj, sizeof(ObjClass_st));
    (*(ObjClass_t)&Class).size = sizeof(JDbl_st);
    static const char name[] = UNIT_NAME;
    (*(ObjClass_t)&Class).name = name;
    (*(ObjClass_t)&Class).super = Obj;
    (*(ObjClass_t)&Class).cstr = cstr;
    (*(ObjClass_t)&Class).rpr = rpr;
    jlc();
    mutVoidPtr((void **)&jab_->jDbl, jdbl);
    jblc();
}

#ifndef AS_LIB

#endif