local LIMIT = 10

-- start in the middle... just 'cause
local Tape = { pos = math.floor(LIMIT/2) }

-- set our array to all zeroes
for i=0, LIMIT-1 do
  Tape[i] = 0
end
Tape.__index = Tape

function Tape:new(o)
    o = o or {}              -- create object if user does not provide one
    setmetatable(o, self)    -- sets this up Tape as the prototype for o
    self.__index = self      -- creates inheritance (ie: lookup) behavior
    return o
end

-- instead of shifting elements, shift our position down one and overwrite the value there
-- inserts at current position, or at relative position
-- returns the replaced value
function Tape:insert(val, relativePos)
    relativePos = (relativePos or 0) % LIMIT
    self.pos = (self.pos + relativePos - 1) % LIMIT
    local v = self[self.pos]
    self[self.pos] = val
    return v
end

function Tape:move(relativePos)
    relativePos = relativePos or 1
    self.pos = (self.pos + relativePos) % LIMIT
end

-- read an element relative to where we are currently pointed in the ring
function Tape:read(relativePos)
    relativePos = relativePos or 0
    local position = (self.pos + relativePos) % LIMIT
    return self[position]
end

function Tape:dump()
    local a = {}
    for i=0, LIMIT-1 do
        a[i] = self[i]
    end
    return a
end

-- same as skip, don't need both?
function Tape:next()
    self.pos = (self.pos + 1) % LIMIT
    return self[self.pos]
end

return Tape
