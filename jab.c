#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Flow.h>
#include "jab.h"
#include "jab.r.h"
/*---------------------declarations---------------------*/
static JDbl_st one = {NULL, 1, 1};
static JDbl_st zero = {NULL, 1, 0};
static JDbl_st minusone = {NULL, 1, -1};

static void *add(void *a, void *b);
static void *dAdd(void *a, void *b, void *op);
static void *subtract(void *a, void *b);
static void *dSubtract(void *a, void *b, void *op);
static void *mul(void *a, void *b);
static void *dMul(void *a, void *b, void *op);
static void *divide(void *a, void *b);
static void *dDiv(void *a, void *b, void *op);

/*---------------------declarations---------------------*/

/*---------------------constructor---------------------*/
static jab_t jab__ = {NULL};
const jab_t *jab_ = &jab__;
static void __attribute__((constructor)) jabClassf()
{
    if (jab__.jDbl == NULL)
    {
        jab__.jDbl = (void *)-1;
        mutVoidPtr(&Flow.constants.one, &one);
        mutVoidPtr(&Flow.constants.zero, &zero);

        mutVoidPtr(&Flow.util.stringOf, stringOf);
        const struct FlowOp *ops = &Flow.math;
        const struct FlowOp *dops = &Flow.deriv;
        mutVoidPtr(&ops->add, add);
        mutVoidPtr(&dops->add, dAdd);
        mutVoidPtr(&ops->subtract, subtract);
        mutVoidPtr(&dops->subtract, dSubtract);
        mutVoidPtr(&ops->multiply, mul);
        mutVoidPtr(&dops->multiply, dMul);
        mutVoidPtr(&ops->divide, divide);
        mutVoidPtr(&dops->divide, dDiv);
    }
}
void static jdblConst()
{
    //*(void **)one._.class = *(void **)zero._.class = *(void **)minusone._.class = JDbl;
    mutVoidPtr(&one._.class, JDbl);
    mutVoidPtr(&minusone._.class, JDbl);
    mutVoidPtr(&zero._.class, JDbl);
}
const fn_t jlc = jabClassf;
const fn_t jblc = jdblConst;
/*---------------------constructor---------------------*/

/*---------------------Utils---------------------*/
int validateShape(JTsr_t a, JTsr_t b, int *rank, int **shape, int *len, double **res)
{
    return 0;
}
int broadI(int *index, int rank, int *shape)
{
    return 0;
}
void indexI(int *index, int rank)
{
    return;
}
static void *jabOp(void *a, void *b, double (*op)(double a, double b))
{
    if ((*(void **)a) == JDbl)
    {
        JDbl_t A = a;
        if ((*(void **)b) == JDbl)
        {
            JDbl_t B = b;
            return neu(JDbl, op(A->n, B->n));
        }
        else
        {
            JTsr_t B = b;
            size_t size;
            int *shape = malloc(size = B->rank * sizeof(int));
            memcpy(shape, B->shape, size);
            double *res = malloc(B->len * sizeof(double));
            for (int i = 0; i < B->len; i++)
            {
                res[i] = op(A->n, B->tsr[i]);
            }
            return neu(JTsr, B->rank, shape, res);
        }
    }
    else
    {
        JTsr_t A = a;
        if ((*(void **)b) == JDbl)
        {
            JDbl_t B = b;
            size_t size;
            int *shape = malloc(size = A->rank * sizeof(int));
            memcpy(shape, A->shape, size);
            double *res = malloc(A->len * sizeof(double));
            for (int i = 0; i < A->len; i++)
            {
                res[i] = op(A->tsr[i], B->n);
            }
            return neu(JTsr, A->rank, shape, res);
        }
        else
        {
            JTsr_t B = b;
            int len;
            int *shape, rank;
            double *res;
            int broadcast = validateShape(a, b, &rank, &shape, &len, &res);
            if (!broadcast)
                for (int i = 0; i < len; i++)
                {
                    res[i] = op(A->tsr[i], B->tsr[i]);
                }
            else
            {
                int index[rank];
                memset(index, 0, sizeof(index));
                if (broadcast == 1)
                    for (int i = 0; i < len; i++)
                    {
                        res[i] = op(A->tsr[broadI(index, A->rank, A->shape)], B->tsr[i]);
                        indexI(index, rank);
                    }
                else if (broadcast == 2)
                    for (int i = 0; i < len; i++)
                    {
                        res[i] = op(A->tsr[i], B->tsr[broadI(index, B->rank, B->shape)]);
                        indexI(index, rank);
                    }
                else
                    for (int i = 0; i < len; i++)
                    {
                        res[i] = op(A->tsr[broadI(index, A->rank, A->shape)], B->tsr[broadI(index, B->rank, B->shape)]);
                        indexI(index, rank);
                    }
            }
            return neu(JTsr, rank, shape, res);
        }
    }
}
/*---------------------Utils---------------------*/

/*---------------------BEDMAS---------------------*/
static double addfn(double a, double b)
{
    return (a + b);
}
static void *add(void *a, void *b)
{
    if ((a == &one && b == &minusone) || (a == &minusone && b == &one))
        return &zero;
    else if (a == &zero)
    {
        return b;
    }
    else if (b == &zero)
    {
        return a;
    }

    return jabOp(a, b, addfn);
}
static void *dAdd(void *a, void *b, void *op)
{
    if (op == a || op == b)
    {
        return &one;
    }
    else
    {
        return (void *)-1;
    }
}

static double subtfn(double a, double b)
{
    return (a - b);
}
static void *subtract(void *a, void *b)
{
    if (a == b)
        return &zero;
    else if (b == &zero)
        return a;
    return jabOp(a, b, subtfn);
}
static void *dSubtract(void *a, void *b, void *op)
{
    if (op == a)
        return &one;
    else if (op == b)
        return &minusone;
    else
    {
        return (void *)-1;
    }
}

static double divfn(double a, double b)
{
    return (a / b);
}
static void *divide(void *a, void *b)
{
    if (a == b)
        return &one;
    else if (b == &one)
        return a;
    else if (a == &zero)
    {
        return &zero;
    }
    else if (b == &zero)
    {
        fatalErr("jab: division by zero\n");
    }
    return jabOp(a, b, divfn);
}
static void *dDiv(void *a, void *b, void *op)
{
    if (op == a)
    {
        return neu(JDbl, 1 / ((JDbl_t)b)->n);
    }
    else if (op == b)
        return neu(JDbl, ((JDbl_t)a)->n / -(((JDbl_t)b)->n * ((JDbl_t)b)->n));
    else
    {
        return (void *)-1;
    }
}

static double mulfn(double a, double b)
{
    return (a * b);
}
static void *mul(void *a, void *b)
{
    if (a == &one)
        return b;
    else if (b == &one)
        return a;
    else if (a == &zero || b == &zero)
        return &zero; /*
     else if (a == &minusone)
         return neg(b);
     else if (b == &minusone)
         return neg(a);*/
    return jabOp(a, b, mulfn);
}
static void *dMul(void *a, void *b, void *op)
{
    if (op == a)
        return b;
    else if (op == b)
        return a;
    else
    {
        return (void *)-1;
    }
}
/*---------------------BEDMAS---------------------*/