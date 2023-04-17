#ifndef JTSR_R_H
#define JTSR_R_H

#include <Obj.r.h>

#ifdef __cplusplus
extern 'C'
{
#endif

    typedef struct JTsrClass *JTsrClass_t;
    typedef struct JTsrClass JTsrClass_st;
    typedef struct JTsr JTsr_st;
    struct JTsrClass
    {
        ObjClass_st _;
        // other members go here
    };
    struct JTsr
    {
        const Obj_st _;
        // other members go here
        double *tsr;
        int *shape;
        int len;
        int rank;
    };

#ifdef __cplusplus
}
#endif
#endif