def fibfib( n, k ):
    if n == 0:
        a = 1
        b = 1
        for i in range( k - 2 ):
            c = a + b
            a = b
            b = c
        return b
    else:
        return fibfib( 0, fibfib( n - 1, k ) )
