-- Hacker's Evolution Language

local Tape = require("tape")

local HELa = {
    d_tape = Tape:new(),
    l_tape = Tape:new()
}

HELa.__index = HELa

-- create a new program object
function HELa:new(o)
    o = o or {}              -- create object if user does not provide one
    setmetatable(o, self)    -- sets this up Tape as the prototype for o
    self.__index = self      -- creates inheritance (ie: lookup) behavior
    
    o.dataTape = d_tape
    o.langTape = l_tape
    
    return o
end

-- does nothing, just moves to the next instruction
function HELa:noop()
   -- this space intentionally left blank
end

-- inserts a value into the data tape
function HELa:insert()
    local val = self.langTape:next()
    self.dataTape:insert(val)
end

-- addes a value to the current data tape position
function HELa:add()
    local val = self.langTape:next()
    local curr = self.dataTape:read()
    local newVal = val + curr
    self.dataTape:insert(newVal, 1)
end

-- swaps the language and data tape
function HELa:pswap()
    self.dataTape, self.langTape = self.langTape, self.dataTape
end

-- jumps the language pointer to a new location
function HELa:jump()
    local relativePos = self.langTape:next()
    self.langTape:move(relativePos)
end

-- jumps the data pointer to a new location
function HELa:djump()
    local relativePos = self.dataTape:read()
    self.langTape:move(relativePos)
end

-- convinience method, not part of the 'lanugage'
-- prints out the ascii equivilent of the data tape
-- starts from current position
-- ends at 0 or 'end' of the tape
function HELa:print()
    bytes = {}
    for i=0, LIMIT do
        -- 1-indexed arrays in lua!
        bytes[i+1] = dataTape[(dataTape.pos + i)%LIMIT]
    end
    
    -- convert to ascii, filter out non-printables
    s = ""
    for _,v in ipairs(bytes) do
        if v > 31 and v < 127 then
            s = s .. string.char(v)
        end
    end
end

hela.execute = function()
    print("do something to execute")
end

return hela
