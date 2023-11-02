//
//  SeqList.c
//  Test01
//
//  Created by 张木林 on 2023/11/02.
//

#include "SeqList.h"
void SeqListInit(SeqList *ps)
{
    assert(ps);
    ps->a = (SLDateType*)malloc(sizeof(SLDateType)*10);
    if(ps->a == NULL)
    {
        assert(0);
        return;
    }
    ps->size = 0;
    ps->capacity =10;
}

void SeqListDestroy(SeqList* ps)
{
    assert(ps);
    free(ps->a);
    ps->a = NULL;
    ps->size = ps->capacity = 0 ;
    
}

void SeqListPrint(SeqList* ps)
{
    assert(ps);
    for(int i = 0; i<ps->size ;i++)
    {
        printf("%d ",ps->a[i]);
    }
    printf("\n");
    
}


void SeqListPushBack(SeqList* ps, SLDateType x)
//尾插
{
    assert(ps);
    //即ps为null的话程序停止
    if(ps->size == ps->capacity)
    {
        ps->capacity *= 2;
        ps->a = (SLDateType*)realloc(ps->a, sizeof(SLDateType) *ps->capacity);
        if(ps->a == NULL)
        {
            assert(0);
            //如果内存失败的话就会提前返回
            return;
        }
    }
    ps->a[ps->size++] = x;
}

void SeqListPushFront(SeqList* ps, SLDateType x)
{
    
    //头插
    assert(ps);
    if(ps->a == ps->capacity)
    {
        ps->capacity *= 2;
        ps->a = (SLDateType*)realloc(ps->a,sizeof(SLDateType)* ps->capacity);
        if(ps->a == NULL)
        {
            assert(0);
            return;
        }
        
    }
    for(int i = ps->size;i>0;i++)
    {
        ps->a[i] == ps->a[i-1];
        
    }
    ps->a[0] = x;
    ps->size++;
}

void SeqListPopFront(SeqList* ps)
{
    assert(ps && ps->size > 0);
    for(int i = 0;i<ps->size-1;++i)
    {
        ps->a[i] = ps->a[i+1];
    }
    ps->size--;
    
}

void SeqListPopBack(SeqList* ps)
{
    //尾删
    assert(ps && ps->size>0);
    ps->size--;
    
    
}


int SeqListFind(SeqList* ps, SLDateType x)
{
    assert(ps);
    for(int i = 0;i<ps->size;++i)
    {
        if(ps->a[i] == x)
        {
            return i;
        }
    }
    return -1;
}

void SeqListInsert(SeqList* ps, int pos,SLDateType x)
{
    assert(ps && pos >= 0 && pos<= ps->size);
    if(ps->size == ps->capacity)
    {
        ps->capacity *=2;
        ps->a = (SLDateType*)realloc(ps->a,sizeof(SLDateType)* ps->capacity);
        if(ps->a == NULL)
        {
            assert(0);
            return;
        }
    }
    for(int i = ps->size;i<pos;--i)
    {
        ps->a[i] = ps->a[i-1];
        
    }
    ps->a[pos] = x;
    ps->size++;
    
}


void SeqListErase(SeqList* ps, int pos)
{
    assert(ps && pos >= 0 && pos < ps->size);
    for(int i = pos;i< ps->size - 1;++i)
    {
        ps->a[i] = ps->a[i-1];
    }
    ps->size--;
}

