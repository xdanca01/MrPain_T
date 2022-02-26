def updated( x, y, cells ):
    count = 0
    alive = ( x, y ) in cells

    for dx in [ -1, 0, 1 ]:
        for dy in [ -1, 0, 1 ]:
            if dx or dy:
                count += ( x + dx, y + dy ) in cells

    return count in { 2, 3 } if alive else count == 3


def life( cells, n ):
    if n == 0:
        return cells

    todo = set()

    for x, y in cells:
        for dx in [ -1, 0, 1 ]:
            for dy in [ -1, 0, 1 ]:
                todo.add( ( x + dx, y + dy ) )

    ngen = { ( x, y ) for x, y in todo if updated( x, y, cells ) }
    return life( ngen , n - 1 )
