# NotBrainFuck
My own version of BrainFuck. It adds 8 new symbols. These are all to make the language easier to use for beginners. Hence coining the name NotBrainFuck, since it goes against many of the original languages principals. I'm a first year CS major so I am happy to accept and learn from any criticisms.

# The original Brainfuck operators:
\>  Move the pointer to the right
  
\<  Move the pointer to the left
  
\-  Decrement the value in the current cell
  
\+  Increment the value in the current cell
  
\[ ] Performed actions encased repeatedly, until cell selected at the beginning of the loop is equal to 0

Hello World Program from https://esolangs.org/wiki/Brainfuck:
> ++++++++\[>++++\[>++>+++>+++>+<<<<-]>+>+>->>+\[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.

# My commands:
\*  Multiply current cell by its neighbour to the right

x   Multiply current cell by its neighbour to the left

/   Divide current cell by its neighbour to the right

\   Divide current cell by its neighbour to the left

|   Starts/ends a multiline comment (e.g. CODE|COMMENT HERE|CODE)

\#  Sets the cell to the value of the following symbol

^   Increments base

V   Decrements base

Hello World for NotBrainFuck:
>#H.#e.#l..#o.# .#W.#o.#r.#l.#d.#!.>+++>+++<*+.
