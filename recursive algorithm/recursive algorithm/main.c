//
//  main.c
//  recursive algorithm
//
//  Created by 张木林 on 2/21/24.
//

#include<stdio.h>

int factorial(int n)
{
    if(n == 0 || n == 1)
    {
        return 1;
    }
    else
    {
        return n* factorial(n-1);
    }
}
int main()
{
    
    int num = 6;
    printf("Factorial of %d is %d\n",num,factorial(num));
    
    return 0;
}
