//
//  helb.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/6/25.
//

typedef Instruction char;

struct Program {
    langTape Instruction
}

int test(int x);

typedef int (*lua_CFunction) (lua_State *L);
static int l_double (lua_State *L);
