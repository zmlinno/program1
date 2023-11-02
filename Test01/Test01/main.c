//
//  main.c
//  Test01
//
//  Created by 张木林 on 2023/11/02.
//

#include <stdio.h>
#include "SeqList.h"
void Test1()
{
    SeqList list;
    SeqListInit(&list);
    SeqListPushBack(&list, 1);
    SeqListPushBack(&list, 2);
    SeqListPushBack(&list, 3);
    
    SeqListPrint(&list);
    
    SeqListPushFront(&list, 2);
    SeqListPrint(&list);
    
    SeqListPopFront(&list);
    SeqListPrint(&list);
    
    SeqListPopBack(&list);
    SeqListPrint(&list); 
}


int main(void)
{
    Test1();
    
    return 0;
}
