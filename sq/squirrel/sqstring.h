/*  see copyright notice in squirrel.h */
#ifndef _SQSTRING_H_
#define _SQSTRING_H_

inline size_t _hashstr (const SQChar *s, size_t l)
{
        size_t h = (size_t)l;  /* seed */
        size_t step = (l>>5)|1;  /* if string is too long, don't hash all its chars */
        for (; l>=step; l-=step)
            h = h ^ ((h<<5)+(h>>2)+(unsigned short)*(s++));
        return h;
}

struct SQString : public SQRefCounted
{
    SQString(){}
    ~SQString(){}
public:
    static SQString *Create(SQSharedState *ss, const SQChar *, isize_t len = -1 );
    isize_t Next(const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval);
    void Release();
    SQSharedState *_sharedstate;
    SQString *_next; //chain for the string table
    isize_t _len;
    size_t _hash;
    SQChar _val[1];
};



#endif //_SQSTRING_H_
