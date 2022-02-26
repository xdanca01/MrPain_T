# In the first exercise, we will implement a simple RPN (Reverse
# Polish Notation) evaluator.

# The only argument the evaluator takes is a list with two kinds of
# objects in it: numbers (of type ‹int›, ‹float› or similar) and
# operators (for simplicity, these will be of type ‹str›).  To
# evaluate an RPN expression, we will need a stack (which can be
# represented using a ‹list›, which has useful ‹append› and ‹pop›
# methods).

# Implement the following unary operators: ‹neg› (for negation, i.e.
# unary minus) and ‹recip› (for reciprocal, i.e. the multiplicative
# inverse). The entry point will be a single function, with the
# following prototype:

def rpn_unary( rpn ):
    operators = []
    result = []
    for i in range(len(rpn)):
        typ = type(rpn[i])
        if typ == int or typ == float:
            if len(operators) <= 0:
                operators.append(rpn[i])
            else:
                result.append(apply_operators(operators))
                operators = [ rpn[i] ]
        elif len(operators) > 0 and (type(operators[0]) == float or type(operators[0]) == int):
            operators.append(rpn[i])
    if len(operators) > 0:
        result.append(apply_operators(operators))
    return result

def apply_operators( my_list ):
    numb = my_list[0]
    for i in range(1, len(my_list)):
        if my_list[i] == 'neg':
            if numb < 0:
                numb = abs(numb)
            else:
                numb = -numb
        elif my_list[i] == 'recip' and numb != 0:
            numb = 1/numb
    return numb

# The second part of the exercise is now quite simple: extend the
# ‹rpn_unary› evaluator with the following binary operators: ‹+›,
# ‹-›, ‹*›, ‹/›, ‹**› and two ‘greedy’ operators, ‹sum› and ‹prod›,
# which reduce the entire content of the stack to a single number.
# Think about how to share code between the two evaluators.

# Note that we write the stack with ‘top’ to the right, and
# operators take arguments from left to right in this ordering (i.e.
# the top of the stack is the right argument of binary operators).
# This is important for non-commutative operators.

def rpn_binary( rpn ):
    stack = []
    for i in range (len(rpn)):
        if type(rpn[i]) == str and len(stack) > 1:
            # TOP (napravo)
            numb2 = stack.pop()
            # 2. znak (nalevo)
            numb1 = stack.pop()
            if rpn[i] == '+':
                stack.append(numb1 + numb2)
            elif rpn[i] == '-':
                stack.append(numb1 - numb2)
            elif rpn[i] == '*':
                stack.append(numb1 * numb2)
            elif rpn[i] == '/':
                stack.append(numb1 / numb2)
            elif rpn[i] == '**':
                stack.append(numb1 ** numb2)
            elif rpn[i] == 'sum':
                stack.append(numb1 + numb2)
                numb = 0
                for i in stack:
                    numb += i
                stack = [ numb ] 
            elif rpn[i] == 'prod':
                numb = numb1 * numb2
                for i in stack:
                    numb *= i
                stack = [ numb ]
            elif rpn[i] == 'recip' or rpn[i] == "neg":
                numb2 = rpn_unary([ numb2, rpn[i] ]).pop()
                stack.append(numb1)
                stack.append(numb2)
            else:
                stack.append(numb1)
                stack.append(numb2)
        elif type(rpn[i]) == int or type(rpn[i]) == float:
            stack.append(rpn[i])
    return stack

# Some test cases are included below. Write a few more to convince
# yourself that your code works correctly.

def test_unary():
    rpn_num = [ 5 ]
    assert rpn_unary( rpn_num ) == [ 5 ]

    rpn_neg = [ 1, "neg" ]
    assert rpn_unary( rpn_neg ) == [ -1 ]

    rpn_rec = [ 2, "recip" ]
    assert rpn_unary( rpn_rec ) == [ 1/2 ]

    rpn_n = [ -1/7, "recip" ]
    assert rpn_unary( rpn_n ) == [ -7 ]

    rpn_simp = [ 1, "recip", "neg" ]
    assert rpn_unary( rpn_simp ) == [ -1 ]

    rpn = [ 4, "neg", "recip", "neg", "neg", "recip", "neg",
            "recip", "recip" ]
    assert rpn_unary( rpn ) == [ 4 ]

    rpn_nums = [ 5, 1/9, "recip", 2, "neg", "recip", -1, "neg" ]
    assert rpn_unary( rpn_nums ) == [ 5, 9, -1/2, 1 ]

def test_binary():
    rpn = [ 2, -2, '+' ]
    assert rpn_binary( rpn ) == [ 0 ]

    rpn = [ 3, 7, '*' ]
    assert rpn_binary( rpn ) == [ 21 ]

    rpn = [ 8, 2, "recip", '/' ]
    assert rpn_binary( rpn ) == [ 16 ]

    rpn = [ -1, 3, '-', 2, '+', 4, "neg", 2, '**' ]
    assert rpn_binary( rpn ) == [ -2, 16 ]

    rpn = [ 3, -1, 9, '*', 22, 100, "neg", "sum" ]
    assert rpn_binary( rpn ) == [ -84 ]

    rpn = [1, 2, 3, 4, 5, "prod" ]
    assert rpn_binary( rpn ) == [ 120 ]


if __name__ == "__main__":
    test_unary()
    test_binary()
