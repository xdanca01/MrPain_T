# Consider the following sequences:
#
#     s[0] = 1 1 2 3 5 8 13 21 …
#     s[1] = 1 1 1 2 5 21 233 10946 …
#     s[2] = 1 1 1 1 5 10946 2.2112⋅10⁴⁸ 1.6952⋅10²²⁸⁷ …
#     s[3] = 1 1 1 1 5 1.6952⋅10²²⁸⁷ …
#
# More generally:
#
#  • ‹s[0][k] = fib(k)› is the ⟦k⟧-th Fibonacci number,
#  • ‹s[1][k] = fib(fib(k))› is the ‹s[0][k]›-th Fibonacci number,
#  • ‹s[2][k] = fib³(k)› is the ‹s[0][s[1][k]]›-th Fibonacci number,
#  • and so on.
#
# Write ‹fibfib›, a function which computes ‹s[n][k]›.

def fibfib( n, k ):
    if k < 0:
        return 0
    result = fib(k)
    for i in range(n):
        result = fib(result)
    return result

def fib( k ):
    if k == 0:
        return 0
    elif k == 1:
        return 1
    else:
        numb1 = 0
        numb2 = 1
        for i in range(k-1):
            tmp = numb1
            numb1 = numb2
            numb2 += tmp
        return numb2

def test_main():
    assert fibfib( 0, 6 ) == 8
    assert fibfib( 0, 7 ) == 13
    for k in range( 20 ):
        assert fibfib( k, 1 ) == 1
        assert fibfib( k, 5 ) == 5
    assert fibfib( 1, 6 ) == fibfib( 0, 8 )
    assert fibfib( 2, 6 ) == fibfib( 1, 8 )
    assert fibfib( 3, 6 ) == fibfib( 2, 8 )
    assert fibfib( 2, 7 ) == fibfib( 1, 13 )
    assert fibfib( 2, 8 ) == fibfib( 1, 21 )

test_main()
