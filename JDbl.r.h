#ifndef JDBL_R_H
#define JDBL_R_H

#include <Obj.r.h>
#include "jab.r.h"

#ifdef __cplusplus
extern 'C'
{
#endif

    typedef struct JDblClass *JDblClass_t;
    typedef struct JDblClass JDblClass_st;
    typedef struct JDbl JDbl_st;
    struct JDblClass
    {
        ObjClass_st _;
        // other members go here
    };
    struct JDbl
    {
        const Obj_st _;
        // other members go here
        const double n;
    };

#define setN(jbl_, val) mut(((JDbl_t)jbl_)->n, double, val)

#ifdef __cplusplus
}
#endif
#endif