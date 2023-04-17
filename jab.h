#ifndef JAB_H
#define JAB_H

#ifdef __cplusplus
extern 'C'
{
#endif
    typedef struct jab
    {
        void *(*jDbl)(double x);
        void *(*Tsr)(int shape);
    } jab_t;

    extern const jab_t *jab_;
#define jab (*jab_)
#ifdef __cplusplus
}
#endif
#endif