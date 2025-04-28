local he = {}

he.program = {}
he.state = {}
he.pointer = 0

he.instructions = {
    push = function(value)
        table.insert( he.state, value )
    end,
    pop = function()
        return table.remove( he.state )
    end
}

he.execute = function()
    print("do something")
    he.state[1] = "first stack value"
    he.state[2] = "second value"
    he.state[3] = "third value"
end

return he
