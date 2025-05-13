//
//  helb.c
//  Hackers Evolution
//
//  Created by Eric Baur on 5/6/25.
//

#import "lua.h"

int test(int x) {
    return x*x;
}


static int l_double (lua_State *L) {
  double d = lua_tonumber(L, 1);  /* get argument */
  lua_pushnumber(L, d*d);  /* push result */
  return 1;  /* number of results */
}
