def is_bipartite( graph ):
    colours = {}
    queue = []

    vertices = list( set( [ x for x,_ in graph ] ) )
    for v in vertices: # can be disconnected
        if v in colours:
            continue
        queue.append( v )
        colours[ v ] = 1
        colour = 1

        while queue:
            v = queue.pop( 0 )
            colour = 2 if colours[ v ] == 1 else 1
            for neighb in [ y for x,y in graph if x == v ]:
                if neighb in colours and \
                   colours[ neighb ] != colour:
                    return False
                if neighb not in colours:
                    colours[ neighb ] = colour
                    queue.append( neighb )
    return True
