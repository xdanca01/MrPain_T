# XXX
from statistics import median, mean

def breadth(tree):
    last_level = [1]
    widths = []

    while last_level:
        next_level = []
        for i in last_level:
            next_level += tree[i]

        widths.append( len( last_level ) )
        last_level = next_level

    return mean( widths ), median( widths ), max( widths )
