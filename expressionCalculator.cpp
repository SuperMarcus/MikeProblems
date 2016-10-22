//
// Created by Marcus on 10/21/16.
//

#include "expressionCalculator.h"

l_frac_block *_parse(char **pt){
    l_frac_block *block = new l_frac_block();
    number_buf buf;
    buf.clear();
    OP bufOP = ' ';
    while(**pt != '\0'){
        if(**pt >= '0' && **pt <= '9') {
            buf.push((int64_t) (**pt - '0'));
        }else{
            if(VALID_OP(**pt)){
                if(VALID_OP(bufOP)){
                    if(buf.modified()){
                        block->append(fraction(buf.val()), bufOP);
                        buf.clear();
                        bufOP = ' ';
                    } else if(**pt == OP_SUBTRACT)
                        buf.negative();
                }else{
                    if(buf.modified()){
                        block->append(fraction(buf.val()), OP_ADD);
                        buf.clear();
                    }
                    bufOP = (OP) **pt;
                }
            }else if(**pt == '(' || **pt == '[' || **pt == '{'){//New block
                if(buf.modified()){
                    if(!VALID_OP(bufOP))
                        bufOP = OP_ADD;
                    block->append(fraction(buf.val()), bufOP);
                    buf.clear();
                    bufOP = OP_MULTIPLY;
                }else if(!VALID_OP(bufOP)){
                    bufOP = OP_MULTIPLY;
                }
                ++*pt;
                block->append(_parse(pt), bufOP);
                buf.clear();
                bufOP = ' ';
            }else if(**pt == ')' || **pt == ']' || **pt == '}'){//End block
                if(buf.modified()){
                    if(!VALID_OP(bufOP))
                        bufOP = OP_ADD;
                    block->append(fraction(buf.val()), bufOP);
                    buf.clear();
                }
                return block;
            }
        }
        ++*pt;
    }
    return block;
}

l_frac_block *parse(const char *string_expression){
    char *sharedPointer = (char *) string_expression;
    return _parse(&sharedPointer);
}

void r_frac2_monomial(frac2_monomial m, fraction *result){
    result->m_errno = (uint8_t) (!VALID_OP(m.m_operator) || m.left.south == 0 || m.right.south == 0 || (m.m_operator == OP_DIVIDE && m.right.north == 0));
    if(!result->m_errno){
        if(m.m_operator == OP_ADD || m.m_operator == OP_SUBTRACT){
            int64_t tmpN = (m.left.north * m.right.south) + (m.m_operator == OP_SUBTRACT ? -(m.left.south * m.right.north) : m.left.south * m.right.north),
                    tmpS = m.left.south * m.right.south,
                    resGcd = gcd(tmpN, tmpS);
            result->north = tmpN / resGcd;
            result->south = tmpS / resGcd;
        }else{
            if(m.m_operator == OP_DIVIDE){
                int64_t bufN = m.right.north;
                m.right.north = m.right.south;
                m.right.south = bufN;
            }
            int64_t tmpN = m.right.north * m.left.north,
                    tmpS = m.right.south * m.left.south,
                    resGcd = gcd(tmpN, tmpS);
            result->north = tmpN / resGcd;
            result->south = tmpS / resGcd;
        }
    }
}

int64_t fast_gcd(int64_t a, int64_t b){
    while(b) b ^= a ^= b ^= a %= b;
    return a;
}

int64_t gcd(int64_t a, int64_t b){
    return fast_gcd(a, b);
}

int64_t lcm(int64_t a, int64_t b){
    return a * b / gcd(a, b);
}

struct _fraction _fraction::operator+(struct _fraction a) {
    frac2_monomial m;
    fraction result;
    m.left = *this;
    m.right = a;
    m.m_operator = OP_ADD;
    r_frac2_monomial(m, &result);
    return result;
}

struct _fraction _fraction::operator-(struct _fraction a) {
    frac2_monomial m;
    fraction result;
    m.left = *this;
    m.right = a;
    m.m_operator = OP_SUBTRACT;
    r_frac2_monomial(m, &result);
    return result;
}

struct _fraction _fraction::operator*(struct _fraction a) {
    frac2_monomial m;
    fraction result;
    m.left = *this;
    m.right = a;
    m.m_operator = OP_MULTIPLY;
    r_frac2_monomial(m, &result);
    return result;
}

struct _fraction _fraction::operator/(struct _fraction a) {
    frac2_monomial m;
    fraction result;
    m.left = *this;
    m.right = a;
    m.m_operator = OP_DIVIDE;
    r_frac2_monomial(m, &result);
    return result;
}

void _l_frac_block::solve(fraction *result) {
    l_frac_monomial *current = first;
    printf("+ Pass %lld/%lld\n", result->north, result->south);
    while (current){
        if(current->node.is_block){
            fraction *sub_buf_res = new fraction();
            current->node.content.block->solve(sub_buf_res);
            frac2_monomial m;
            m.left = *result;
            m.m_operator = current->m_operator;
            m.right = *sub_buf_res;
            r_frac2_monomial(m, result);
            printf("Solving step (%lld/%lld)%c(%lld/%lld)=(%lld/%lld)\n", m.left.north, m.left.south,
                   m.m_operator, m.right.north, m.right.south, sub_buf_res->north, sub_buf_res->south);
        } else{
            if(current == first){
                *result = current->node.content.frac;
                current = current->next;
                continue;
            }
            frac2_monomial m;
            m.left = *result;
            m.m_operator = current->m_operator;
            m.right = current->node.content.frac;
            r_frac2_monomial(m, result);
            printf("- Solving step (%lld/%lld)%c(%lld/%lld)=(%lld/%lld)\n", m.left.north, m.left.south,
                   m.m_operator, m.right.north, m.right.south, result->north, result->south);
        }
        current = current->next;
    }
}

l_frac_block *_l_frac_block::append(l_frac_monomial *m) {
    l_frac_monomial *current = first;
    if(current == nullptr){
        first = m;
        return this;
    }
    while (current->next != nullptr)
        current = current->next;
    current->next = m;
    return this;
}

l_frac_block *_l_frac_block::append(fraction f, OP o) {
    return this->append(new l_frac_monomial(f, o));
}

_l_frac_block::_l_frac_block() {
    first = nullptr;
}

_l_frac_block::~_l_frac_block() {
    if(first != nullptr)
        delete first;
}

l_frac_block *_l_frac_block::append(l_frac_block *b, OP o) {
    return this->append(new l_frac_monomial(b, o));
}

_l_frac_monomial::_l_frac_monomial(fraction f, OP o) {
    node.is_block = false;
    node.content.frac = f;
    m_operator = o;
    next = nullptr;
}

_l_frac_monomial::~_l_frac_monomial() {
    if(next != nullptr)
        delete next;
}

_l_frac_monomial::_l_frac_monomial(l_frac_block *b, OP o) {
    node.is_block = true;
    node.content.block = b;
    m_operator = o;
    next = nullptr;
}

void _number_buf::push(int64_t c) {
    m = true;
    buf = buf * 10 + c;
}

int64_t _number_buf::val() {
    return buf;
}

void _number_buf::clear() {
    m = false;
    buf = 0;
    co = 1;
}

bool _number_buf::modified() {
    return m;
}

void _number_buf::negative() {
    co = -co;
    m = true;
}

void _debug_trace(l_frac_block *block){
    l_frac_monomial *current = block->first;
    while (current){
        if(current != block->first || current->m_operator == OP_SUBTRACT)
            printf("%c", current->m_operator);
        if(current->node.is_block){
            printf("(");
            _debug_trace(current->node.content.block);
            printf(")");
        } else{
            if(current->node.content.frac.south == 1)
                printf("%lld", current->node.content.frac.north);
            else printf("(%lld/%lld)", current->node.content.frac.north, current->node.content.frac.south);
        }
        current = current->next;
    }
}

void _trace_block(l_frac_block *block){
    printf("Trace Block: ");
    _debug_trace(block);
    printf(" Finish trace\n");
}
