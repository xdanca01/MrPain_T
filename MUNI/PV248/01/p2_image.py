# You are given a function ‹f› which takes a single integer
# argument, and a list of closed intervals ‹domain›. For instance:
#
#     f = lambda x: x // 2
#     domain = [ ( 1, 7 ), ( 3, 12 ), ( -2, 0 ) ]
#
# Find the «image» of the set represented by ‹domain› under ‹f›, as
# a list of disjoint, closed intervals, sorted in ascending order.
# Produce the shortest list possible.
#
# Values which are not in the image must not appear in the result
# (e.g. if the image is ⟦{1, 2, 4}⟧, the intervals would be ⟦(1, 2),
# (4, 4)⟧ – not ⟦(1, 4)⟧ nor ⟦(1, 1), (2, 2), (4, 4)⟧).

def image( f, domain ):
    result = []
    numbers = {}
    l_numbers = []
    # Get all numbers from domains
    for i in domain:
        if type(i) == tuple:
            x1 = f(i[0])
            x2 = f(i[1])
            for i in range(x1, x2+1):
                numbers[i] = 0
    # Dict keys to list 
    for i in numbers.keys():
        l_numbers.append(i)
    # Sort all numbers
    l_numbers.sort()
    # Create closed intervals from numbers
    smallest = None
    for i in l_numbers:
        if smallest is None:
            smallest = ( i, i )
        elif smallest[1]+1 == i:
            smallest = ( smallest[0] , i)
        else:
            result.append(smallest)
            smallest = ( i, i )
    if smallest is not None:
        result.append(smallest)
    return result

def test_main():
    f = lambda x: x
    g = lambda x: x // 2
    assert image( f, {} ) == []
    assert image( f, [ ( 1, 1 ) ] ) == [ ( 1, 1 ) ]
    assert image( g, [ ( 1, 3 ) ] ) == [ ( 0, 1 ) ]
    assert image( g, [ ( 1, 4 ) ] ) == [ ( 0, 2 ) ]
    assert image( g, [ ( 1, 1 ), ( 7, 8 ) ] ) \
            == [ ( 0, 0 ), ( 3, 4 ) ]
    assert image( g, [ ( 1, 7 ), ( -1, 1 ) ] ) == [ ( -1, 3 ) ]
    assert image( g, [ ( 1, 1 ), ( 3, 4 ) ] ) == [ ( 0, 2 ) ]
    assert image( g, [ ( 7, 7 ), ( 3, 4 ), ( 5, 5 ) ] ) == \
            [ ( 1, 3 ) ]
    assert image( g, [ ( 1, 10 ), ( 100, 200 ), ( 50, 80 ) ] ) == \
            [ ( 0, 5 ), ( 25, 40 ), ( 50, 100 ) ]

if __name__ == "__main__":
    test_main()
