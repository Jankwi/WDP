#include <vector>
#include <cstdlib>
#include "fu.h"

using namespace std;

struct fu
{
    int label;
    find_union up;
    int rank, size;
    vector<find_union> elems;
};

find_union create_fu(int label)
{
    find_union res = (find_union)malloc(sizeof(fu));
    res->label = label;
    res->up = NULL;
    res->rank = 0;
    res->size = 1;
    res->elems= {};
    return res;
}

find_union go_up(find_union p)
{
    if (p->up == NULL)
        return p;
    return (p->up = go_up(p->up));
}

int find(find_union p)
{
    return go_up(p)->label;
}

bool equivalent(find_union p, find_union q)
{
    return go_up(p) == go_up(q);
}

void union_fu(find_union p, find_union q)
{
    find_union rp = go_up(p);
    find_union rq = go_up(q);
    if (rp == rq)
        return;
    if (rp->rank > rq->rank)
    {
        rq->up = rp;
        rp->size += rq->size;
        rp->elems.push_back(rq);
    }
    else
    {
        rp->up = rq;
        rq->size += rp->size;
        rq->elems.push_back(rp);
        if (rq->rank == rp->rank)
            rq->rank++;
    }
}

int size(find_union p)
{
    return go_up(p)->size;
}

void _traverse(find_union p, vector<int> &res)
{
    res.push_back(p->label);
    for (find_union q : p->elems)
    {
        _traverse(q, res);
    }
}

vector<int> elements(find_union p)
{
    vector<int> res;
    _traverse(go_up(p), res);
    return res;
}