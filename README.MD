Just another library inspired by Mike!
======================================

Everybody know how smart mike is! His
amazing ideas inspired thousands of
people everyday!

**************************************

NOTICE: This library is extremely
buggy right now, just don't use it in
production.

This library parse math expressions to
`l_frac_block` form linked list. You
can use internal APIs to calculate
them. The major purpose for this lib
is education, so it just can't be as
stable and complex as professional
software. It provides very basic
calculations in fractions, which means
everything inside will be calculate
and result with fractions.

EXAMPLE
-------

```c++
//Result will be stored here
fraction f;

//Buffer of two fractions
frac2_monomial f2m;

do{
    //Request input from user
    //Write fractions in such form:
    //  (A/B)<operator>(C/D)
    scanf(
         "(%lld/%lld)%c(%lld/%lld)",
         &f2m.left.north,
         &f2m.left.south,
         (char *) &f2m.m_operator,
         &f2m.right.north,
         &f2m.right.south);
   
    //Call calculate API
    r_frac2_monomial(f2m, &f);
    
    //If error, notice first
    if(f.m_errno)
        printf("Wrong Number!\n");
    
    else printf(
        "The result is %lld/%lld",
        //North is always the number
        //on the top
        f.north,
        //North is always the number
        //on the buttom
        f.south
    );

//If error detected
//request fraction again
}while (f.m_errno);
```