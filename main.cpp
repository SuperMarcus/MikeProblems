//
// Created by Marcus on 10/21/16.
//

#include <iostream>
#include "expressionCalculator.h"

using namespace std;

int main() {
    char str[2048];
    printf("Please input a line of expression to evaluate:\n");
    if(fgets(str, 2048, stdin) != NULL){
        l_frac_block *block = parse(str);
        _trace_block(block);
        fraction f;
        block->solve(&f);
        if(f.south == 1)
            printf("\n------------------\nResult %lld", f.north);
        else
            printf("\n------------------\nResult %lld/%lld", f.north, f.south);
    }

    /**
     * EXAMPLE FOR ONLY TWO FRACTIONS
     *
     * fraction f;
     * frac2_monomial f2m;
     * do{
     *     scanf("(%lld/%lld)%c(%lld/%lld)", &f2m.left.north, &f2m.left.south, (char *) &f2m.m_operator,
     *         &f2m.right.north, &f2m.right.south);
     *     r_frac2_monomial(f2m, &f);
     *     if(f.m_errno)
     *         printf("Wrong Number!\n");
     *     else{
     *         printf("The result is %lld/%lld", f.north, f.south);
     *     }
     * }while (f.m_errno);
     */
    return 0;
}
