# Hacker's Evolution

A project to play around with evolution similuations, Lua and Swift.

I don't really have any specific goals at this time, mostly just playing around.



## essotaric languages

I've always been a fan of essoterric languages (reading about them, not realluy using them much)... it's just a facinating subject. I've been thinking about writing my own and a small contest run by MX I saw years ago at a conference served as the inspiration.

## Hacker's Revenge

At the MX booth, they ran a contest with a small custom language using a simple positional "arena" to hold the code, a stack and a handful of instructions. Two programs were written to random locations in the arena and two pointers (one for each program) was used to execute the code in parallel. Each program could write back to area positions in an attempt to get the other program to fail (often by writing a "Halt and Catch Fire" instruction; but really any failure of the other program yielded a win).

I used the base language they started with an attempted to write a small evolutionary algorithm that would take an existing set of programs and mutate them slightly to try and "evolve" a better program. I never got it working efficeintly; but it was still a good excercise.

This got me thinking about better ways to simulate evolution with a small language. I wanted a even simpler language, one that would be easier to write and test.

## Other insprirations

A cool experiemnt using Brainfuck was done by a google engineer (https://arxiv.org/pdf/2406.19108) allowing him to simulate the move from self-assembly to self-replication. I'm not suren if I'll get around to replicating that setup, but its a facinating use case to considier.

## Conceptual foundation

I started to think more about what features I wanted out of a language. My main goal was a language that is easy to evolve.

* It should be able to write back to it's own code while running.
* A point mutation of part of the code shouldn't cause a syntax error (restated: all programs compile, even if they don't run)

The epifany I had was that I wanted to create something in which the distinction between "code" and "data" was blurred. I as insprited (vaguly) by the way DNA codes for amino acids. Each codeon is 3 base pairs and those map to amino acids. The thing is that it's not a perfect 1-1 mapping. Multiple sets map to the same acid. In this way, I realized I could mix and match data and code by having some values interpreted as integers and some interpreted as codepoints depending on how it was referenced.

For example, in the original Hacker's Revenge language, there is a PUSH command that takes an argument. That argument is pushed to the stack.

```
PUSH 4

```

When I implemented my evolutionary algorithm, I allowed the values (4 in this exmaple) and the instructions (PUSH) to be modified individually. Each instruction could be swapped out with a different one, and each value swapped with another value. The problem here is that different instructions had different amounts of arguments. If a PUSH mutated to, say, an INC it would retain the value (since INC takes a single argument) but if it mutated to a DROP the argumenet was no longer necessary.

In this case, `PUSH 4` is a single instruction sotred in a single cell in the arena. I want to try making it so that the `PUSH` and `4` are stored as seperate values in two cells:

| Cell 1 | Cell 2 |
| -------------- | --------------- |
| `PUSH` | `4` |

But instructions are now mapped values, so really this is something like (if `PUSH` maps from `2`):

| Cell 1 | Cell 2 |
| -------------- | --------------- |
| `2` | `4` |

Let's say the 2 changes, and now maps to `DROP` - the `4` is now orphaned from the `PUSH` and the code pointer now - when it moves to Cell 2 - will now interpret 4 as an instruction and not a value, causing a very different interpretation of the remainder of the program.

Maybe this will be disasterous... this so far is me writing down shower thoughts, I haven't tried impementing it yet. ;) So far, this is all just musings and wishlists.
