# Language overview

## Storage

There are two storage mechanisms in HE, each one is implemented as a ring buffer of 256 positions. These will be referred to as Tapes in these docs. Since they are two, we'll have a right/left, but use the Latin style D/L to refer to them (Dextro/Levo). So they are the D-Tape and L-Tape.

Canonically (at least, when the program execution starts) the program is expected to be loaded in the L-Tape and the D-Tape will be used to store any state that the program needs. There are commands that allow for execution to switch Tapes, however so that starting assumption might not always be true.

For example, the cell diagram below the top Tape is the D-Tape and bottom is the L-Tape at the start of execution. In the example, the program has been entered as:
```
insert 3
decrement
add 2
insert 5
noop
```

```
Value: 0 0 0 0 0 0 0 0
Cells: 1 2 3 4 5 6 7 8
       |      
Cells: 1 2 3 4 5 6 7 8
Value: i 3 d a 2 i 5 n
```

As normal program execution progresses, the pointer will shift to each successive instruction in the current execution Tape, the pointer will continue to point to the same cell in the state Tape until some operation is specifically performed to move the pointer.

After each step of execution, the overall program state will look like this. (The data is shifted, the program has been left in place to visualize.)

```
Value: 0 0 3 0 0 0 0 0  |  0 0 0 2 0 0 0 0  |  0 0 0 0 0 4 0 0  |  4 0 0 0 0 0 0 5  
Cells: 7 8 1 2 3 4 5 6  |  6 7 8 1 2 3 4 5  |  4 5 6 7 8 1 2 3  |  2 3 4 5 6 7 8 1 
           |            |        |          |            |      |                | 
Cells: 1 2 3 4 5 6 7 8  |  1 2 3 4 5 6 7 8  |  1 2 3 4 5 6 7 8  |  1 2 3 4 5 6 7 8
Value: i 3 d a 2 i 5 n  |  i 3 d a 2 i 5 n  |  i 3 d a 2 i 5 n  |  i 3 d a 2 i 5 n 
```

A couple things to note here. The pointer to the data Tape does not move when program execution steps to the next instruction. It stays aligned with its current value. Program execution steps to the next instruction (more on what is considered an instruction later). WHen a new value is inserted into the data Tape, that moves the other values. (That's a bit simplistic, actually... the implementation will actually write the value back one position and shift the pointer. Since all position instructions are relative to the current pointer, so the absolute value isn't really relevant.) Also of note, the "last" value is dropped when this happens (since this is a ring buffer, not actually an infinite Tape).


## Instructions

Instructions may or may not have arguments associated with them. Arguments are never optional, as the arguments take up positions on the Tape and the execution pointer needs to know how far to move. 

For example, an INSERT takes a single argument, so the pointer will move two steps (1 for the Instruction and 1 for the argument) to find the next instruction.

The real catch, though, is that Instructions are NOT represented in the Tape with letters, they are also numerical values when stored. Furhtermore, in order to make sure each cell that needs to store an Instruction value is valid, every value maps to one of the instructions. There are far more potential values than there are Instuctions, so the mapping is many-to-one. 

### Instruction Set

#### INSERT [val]

Copies the *val* in the next cell (ie: the argument) to the current Data cell, pushing any value currently there to the next position in the Tape.

#### ADD val

Adds the *val* in the next language cell to the current value of the data Tape.

#### PSWAP

Swaps the Execution and Data Pointers - the Tapes will now take on opposite roles and Program execution will step the new Tape.

#### JUMP relative_position

Jumps the Execution pointer to a new *relative_location* based on the given value.

#### DJUMP

Jumps the Executin pointer to a new position based on the current value of the data pointer. 

#### NOOP

Does nothing. Execution pointer simply shifts to the next cell.

#### GT comparrison false_position

Compares the *comparrison* value to the current value stored on the Data Tape. If the argument value is greater than the data value execution continues after the next cell (ie: skip the cell the *false_position* is stored in). If the comparison is false, skip to the position indicated by *false_position* to continue execution.

#### LT comparrison false_position

Same as `GT` but uses less than instead of greater than.

#### DUPE

Duplicates the current value pointed to on the data Tape. The data pointer is sifted to point to the first one.

#### MOVE relative_position

Moves the data pointer the amount specified by *relative_position*

#### INC

Increment the current data value by 1

#### DEC

Decrement the current data value by 1

#### COPY start_position length

Copies data from the program Tape to the data Tape starting at *start_position* and running for *length*
