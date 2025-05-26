# Language Examples

## HELC Language examples

### Bugs

* Currently, if you insert a # as the only instruction in the REPL, it will insert a 1 intead of a 0.

## Comments

Not considered part of the language, per se, but you can use `:` and `;` to mark the beginning/end of comments. Whitespace is already considered no-ops, so you can have newlines in the program and/or comments.

Comment markers are not paired, so any number are starting `:s are still ended by a single `;` (also, that means individual `;`s are ignored).

Currently the parser removes all comemnts before executing the program, so they won't affect running code in any way.

Looping
```
#A,(#1%1,1)0%0
```
```
#A,("
```

Leaves stack as:
```
[ 1 1 1 1 1 1 1 1 1 1 ]
```

Similar compressed version (inserts 0s):

```
#A(#%,)%0
```

General loop strategy: `#A(0 ------ %1,1)0%0`

* `#A` - insert 10 (loop variable))
* `(0` - start loop (value is ignored)
* `#1` - insert 1 (substitute other logic here)
* `%1` - swap two stack values, so the loop var. is on top
* `,1` - decrement the loop var by 1
* `)0` - mark the end of the loop (value is ignored)
* `%0` - drop the loop variable, as it's no longer needed

## Working with the tape.

## Math

Adding numbers 1-4:

```
#1#2#3#4+++
```


### Clearing the stack:

```
(%0)
```

This starts a loop, checking against the current stack value, drops that value and ten loops back. This will clear out any-non-zero values up to the first zero. Expanded version:

```
(0%0)0 
```

To clear the entire stack, you can use the stack-based loop, which continues as long as there are stack variables:

```
(1%0)1
```

## Hello, world

### The easy way

Using the data and read instructions:

```
.DHello, world.!(1,)$
```

This can be run on the command line with:
```
he --execute=".DHello, world.!(1,)$0"
```

Let's break this down into four main steps.

#### Insert data

The `.D` instruction causes the program to interpret the next 13 bytes as ASCII characters (`D` is 13 in hexidecimal). These are inserted into the program but skipped over stepping through instructions.

#### Read data into the tape

The `!` instruction says to read characters after the first data instruction and insert the ASCII value into the tape. Since no argument was given to this instruction, it reads "all" the caracters after the data instructin. After each insertion, the tape pointer is moved 1, so after all the insertions the pointer will be just after the last element, pointing to an empty tape slot.

#### Rewind loop

The next few instruction cause the tape pointer to rewind. `(1` starts the loop by looking at the tape value just before the current one, which currently contains the last ASCII value (for the "." at the end). As long as the value inspected is non-zero, the loop will continue - otherwise it will jump to just after the closing `)`

The `,` simply moves the tape pointer back one slot. 

Instead of using a loop, since we know the number of characters, we could have been specific and told it to back 12 spaces without the loop with just `,D`

#### Output string

Finally, the `$` tells the program to output a string from the ASCII values stored in the tape, starting from the current value up to the first empty slot (ie: storing 0). (If there was a non-zero number after the `$`  it would have only outputted that count of characters).

### A word about defaults

There are a couple places in which we do not have a number after the instruction. In these cases, a reasonable default is assumed, which may be different depending on the instruction.

As mentioned above, the default for `!` is 0, which casues it to deletgate to the `.` value to know how many characters to read.

For `,` the default is 1. For `$` the default is 0 (ie: read all). For `(` the normal default is 0, but we overrode that to 1 to not have to move the pointer before starting. Instead of `(1,)` we could have just as easily done `,(,)` for the same result.


## Another Hello, world


The ASCII values need to print "Hello, world" are listed below. Decimal and hexidecimal.
```
 H   e   l   l   o   ,       w   o   r   l   d 
72  101 108 108 111 44  32  119 111 114 108 100
48  65  6C  6C  6F  2C  20  77  6F  72  6C  64
```

With the hex values, we can use the `AND` operator to enter in the values somewhat directly and use the `OUT` operator to print:

```
&4#8&6#5&6#C&6#C&6#F&2#C&2#0&7#7&6#F&7#2&6#C&6#4$0
```

The other strategy, just to play with loops, is to do some math on the values. If we take the lowest value (the space) as a baseline, we can enter in the values and then add that value back

```
40 69 76 76 79 12 0 87 11 79 82 76 68
```

These values are too big to enter directly. Since we're trying a differnt method than the direct entry above, let's use some more addition and multiplication to get there

* 40 = 4*10 = `#4#A*1`
* 69 = 9 + 6*10 = `#9#6#A*1+1`

... etc. Spaces added for convienience. If we can get to the value more directly, that style is shown.

```
#4#A,*1'
#9#6#A,*1+1'
#6#7#A,*1+1'
#6#7#A,*1+1'
#9#7#A,*1+1'
#C'
#0
#7#8#A,*1+1'
#B
#9#7#A,*1+1'
#2#8#A,*1+1'
#6#7#A,*1+1'
#8#6#A,*1+1'
```

Conviniently, 32 is 16 doubled, so just to be different we'll insert 15, double it (add to itself) and add 2: `#F+0'#2,+`.

But we need to do this to each value in the existing stack, which will require a loop. Within the loop, we can DUP the value, we don't need to re-calculate it every time.

```

```

## adding things...

Put two numbers on the stack and add them:
```
#2#3+1
```

Add a number to itself"

```
#5+0
```

Add top number to third number:


```
#1#2#3#4#5#6#7#8#9
+2
```

