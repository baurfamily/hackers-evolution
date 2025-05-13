# Language Examples

## Hello

Note: This example shows how to run it via lua, after this example we'll only have the HELa code, without the boilerplate

```
hela = HELa:new()

hela:setProgram([[
= H
= e
= l
= l
= o
]])

hela:execute()
str = hela:print()
```

`str` will container "Hello"

or, more succinctly:

```
hela = HELa:new(""= H = e = l = l = o"")
hela:execute()
str = hela:print()
```

## 
