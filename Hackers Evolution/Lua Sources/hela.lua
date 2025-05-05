local LIMIT = 127

-- Hacker's Evolution Language

-- this wasn't loading correctly
-- local Tape = require("tape")
HELa = {}

HELa.__index = HELa

-- sets the initial conditions for the language tape
function HELa:setProgram(str)
    for i=1, #str do
        -- our tape is sort of 0 indexed, normal lua arrays are 1-indexed
        self.langTape[i] = str:byte(i)
    end
end

-- create a new program object
function HELa:new(prog)
    o = {}
    setmetatable(o, self)    -- sets this up Tape as the prototype for o
    self.__index = self      -- creates inheritance (ie: lookup) behavior
    
    o.d_tape = Tape:new()
    o.l_tape = Tape:new()
    
    o.dataTape = o.d_tape
    o.langTape = o.l_tape
    
    if prog then
        o:setProgram(prog)
    end
    
    return o
end


-- does nothing, just moves to the next instruction
function HELa:noop()
    -- print("noop")
    -- this space intentionally left blank
end
HELa[' '] = HELa.noop

-- inserts a value into the data tape
function HELa:insert()
    local val = self.langTape:next()
    -- print("insert( "..val.." )")
    self.dataTape:insert(val)
end
HELa['='] = HELa.insert

-- addes a value to the current data tape position
function HELa:add()
    local val = self.langTape:next()
    -- print("add( "..val.." )")
    local curr = self.dataTape:read()
    local newVal = val + curr
    self.dataTape:insert(newVal, 1)
end
HELa['+'] = HELa.add

-- swaps the language and data tape
function HELa:pswap()
    -- print("pswap")
    self.dataTape, self.langTape = self.langTape, self.dataTape
end
HELa['%'] = HELa.pswap

-- jumps the language pointer to a new location
function HELa:jump()
    local relativePos = self.langTape:next()
    -- print("jump( "..relativePos.." )")
    self.langTape:move(relativePos)
end
HELa['!'] = HELa.jump

-- jumps the data pointer to a new location
function HELa:djump()
    local relativePos = self.dataTape:read()
    -- print("djump( "..relativePos.." )")
    self.langTape:move(relativePos)
end
HELa['|'] = HELa.djump

-- convinience method, not part of the 'lanugage'
-- prints out the ascii equivilent of the data tape
-- starts at -1 or 'end' of the tape
-- ends at current position
-- (runs backwards for convinience)
function HELa:print()
    bytes = {}
    for i=1, LIMIT do
        -- 1-indexed arrays in lua!
        bytes[i] = self.dataTape:read(-i)
    end
    
    -- convert to ascii, filter out non-printables
    s = ""
    for _,v in ipairs(bytes) do
        if v > 31 and v < 127 then
            s = s .. string.char(v)
        end
    end
    
    return s
end

function HELa:execute()
    continue = 1
    for i=1, LIMIT do
        func = string.char( self.langTape:read() )
        
        if self[func] then else func = "noop" end
        
        self[func](self)
        val = self.langTape:next()
        if val==0 then continue = 0 end
    end
    -- print("--- program execution complete ---")
end

helaSample = HELa:new("=H=e=l=l=o")

-- using a global, not "proper" but it works
return HELa
