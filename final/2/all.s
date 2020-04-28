// Joey Lemon
// Question 2
a:
    ld [fp+12] -> %r0       / get p's value
    ld [fp+16] -> %r1       / get b's value
    ld [r1] -> %r1          / dereference b

    mov #4 -> %r2
    mul %r1, %r2 -> %r1      / *b times the size of int (4)

    add %r0, %r1 -> %r0     / get (p + *b)
    ld [r0] -> %r0          / dereference (p + *b)
    ret

c:
    push #4             / allocate d (located at [fp])

    mov #6 -> %r0       / allocate arg2 for f
    st %r0 -> [sp]--

    mov #5 -> %r0       / allocate arg1 for f
    st %r0 -> [sp]--

    jsr f               / call f
    pop #8              / pop f's args

    mov #4 -> %r1       / add 4 to f's result
    add %r0, %r1 -> %r0

    st %r0 -> [sp]--    / allocate arg1 for e
    jsr e               / call e
    pop #4              / pop e's args

    st %r0 -> [fp]      / store the result into d
    ret

g:
    ld [fp+12] -> %r0       / get h's address

    mov #10 -> %r1          / offset by 10 integers (4 bytes each)
    mov #4 -> %r2
    mul %r1, %r2 -> %r1

    add %r0, %r1 -> %r2     / add 10*4 bytes to h's address

    mov #5 -> %r1
    st %r1 -> [r2]          / h[10] = 5
    
    ld [r2] -> %r2          / deference h[10]
    mov #4 -> %r1
    mul %r1, %r2 -> %r3
    add %r0, %r3 -> %r0     / &(h[h[10]])
    ret