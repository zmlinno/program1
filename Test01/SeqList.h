//
//  SeqList.h
//  Test01
//
//  Created by 张木林 on 2023/11/02.
//

#ifndef SeqList_h
#define SeqList_h

#include <stdio.h>

#endif /* SeqList_h */
// SeqList.h
#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int SLDateType;
typedef struct SeqList
{
    SLDateType* a;
    int size;
    int capacity;
}SeqList;

// 对数据的管理:增删查改
void SeqListInit(SeqList* ps);
void SeqListDestroy(SeqList* ps);

void SeqListPrint(SeqList* ps);
void SeqListPushBack(SeqList* ps, SLDateType x);
void SeqListPushFront(SeqList* ps, SLDateType x);
void SeqListPopFront(SeqList* ps);
void SeqListPopBack(SeqList* ps);

// 顺序表查找
int SeqListFind(SeqList* ps, SLDateType x);
// 顺序表在pos位置插入x
void SeqListInsert(SeqList* ps, int pos, SLDateType x);
// 顺序表删除pos位置的值
void SeqListErase(SeqList* ps, int pos);
