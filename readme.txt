This is a simulator to mimic the behavior of SRT division technique (in C++).

Dividend length and divisor length are (8, 4), (12, 6), (16, 8), and (24, 12) bits, both fractions (dividend and divisor could be unnormalized).

It generates the result, execution time in term of ∆t, where ∆t is the delay of a basic logic gate (AND or OR).


*Uses carry select adder to perform addition,
*Shift operation is 3∆t
*2s complement operation is n*∆t (n is the operand length).
