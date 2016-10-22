//
// Created by Marcus on 10/21/16.
//

#ifndef MIKEPROBLEMS_TYPES_H
#define MIKEPROBLEMS_TYPES_H

#include <iostream>

typedef uint8_t OP;

#define OP_DIVIDE      '/'
#define OP_SUBTRACT    '-'
#define OP_ADD         '+'
#define OP_MULTIPLY    '*'
#define VALID_OP(a)    (a == OP_DIVIDE || a == OP_SUBTRACT || a == OP_ADD || a == OP_MULTIPLY)

typedef struct _fraction fraction;
typedef struct _l_frac_monomial l_frac_monomial;
typedef struct _l_frac_block l_frac_block;
typedef struct _l_frac_list_node l_frac_list_node;
typedef struct _frac2_monomial frac2_monomial;
typedef union _l_content l_content;

struct _fraction{
public:
    int64_t north;
    int64_t south;
    int8_t m_errno;

    _fraction(int64_t value): north(value), south(1), m_errno(0){}
    _fraction(int64_t N, int64_t S): north(N), south(S), m_errno(0){}
    _fraction(): north(1), south(1), m_errno(0){}
    inline struct _fraction operator+(struct _fraction a);
    inline struct _fraction operator-(struct _fraction a);
    inline struct _fraction operator*(struct _fraction a);
    inline struct _fraction operator/(struct _fraction a);
};

struct _frac2_monomial{
public:
    fraction left;
    OP m_operator;
    fraction right;
};

union _l_content{
public:
    fraction frac;
    l_frac_block *block = nullptr;
    _l_content(): frac() {}
};

struct _l_frac_list_node{
public:
    bool is_block;
    l_content content;
    _l_frac_list_node(): is_block(false) {}
};

struct _l_frac_monomial{
public:
    OP m_operator = OP_ADD;
    l_frac_list_node node;
    l_frac_monomial *next = nullptr;

    _l_frac_monomial(){}
    _l_frac_monomial(l_frac_block *b, OP o);
    _l_frac_monomial(fraction f, OP o);
    ~_l_frac_monomial();
};

struct _l_frac_block{
public:
    l_frac_monomial *first;

    _l_frac_block();
    ~_l_frac_block();
    void solve(fraction *result);
    l_frac_block *append(fraction f, OP o);
    l_frac_block *append(l_frac_monomial *m);
    l_frac_block *append(l_frac_block *b, OP o);
};

#endif //MIKEPROBLEMS_TYPES_H
