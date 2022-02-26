# In this exercise, your task is to find the longest possible word
# chain constructible from the input words. The input is a set of
# words. Return the largest number of words that can be chained one
# after the other, such that the first letter of the second word is
# the same as the last letter of the first word. Repetition of words
# is not allowed.

# Examples:
#
#  • { goose, dog, ethanol } → 3 (dog – goose – ethanol)
#  • { why, new, neural, moon } → 3 (moon – new – why)

def word_chain( words ):
    pass

def test_main():

    s = { "why", "new", "neural", "moon" }
    assert word_chain( s ) == 3

    s = { "toad", "pear", "zoo", "apple",
          "wolf", "manatee", "u-turn" }
    assert word_chain( s ) == 1

    s = { "toad", "tandem", "pear", "zoo", "apple",
          "wolf", "manatee", "u-turn", "foo" }
    assert word_chain( s ) == 2

    s = { "turn", "never", "low", "three", "caravan",
          "yellow", "leek", "east", "rascal", "near",
          "west", "moot", "lie", "now", "eaten", "random",
          "why", "when", "pew", "tow", "war",
          "pear", "our", "eat", "two" }
    assert word_chain( s ) == 19

if __name__ == "__main__":
    test_main()

