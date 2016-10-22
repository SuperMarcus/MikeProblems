//
// Created by Marcus on 10/21/16.
//

#ifndef MIKEPROBLEMS_EXPRESSIONCALCULATOR_H
#define MIKEPROBLEMS_EXPRESSIONCALCULATOR_H

#include "types.h"

typedef struct _number_buf{
public:
    void push(int64_t c);
    int64_t val();
    void clear();
    bool modified();
    void negative();
private:
    int64_t buf = 0;
    bool m = false;
    int8_t co = 1;
} number_buf;

int64_t fast_gcd(int64_t a, int64_t b);
int64_t gcd(int64_t a, int64_t b);
int64_t lcm(int64_t a, int64_t b);
l_frac_block *parse(const char *string_expression);
void r_frac2_monomial(frac2_monomial m, fraction *result);
void _trace_block(l_frac_block *block);

#endif //MIKEPROBLEMS_EXPRESSIONCALCULATOR_H
