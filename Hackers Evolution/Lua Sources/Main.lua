-- HELa


-- Use this function to perform your initial setup
function setup()
    he = HELa:new("=H =e =l =l =o")
    
    he:execute()
    print("width: "..WIDTH)
    print("height: "..HEIGHT)
    
    print(he:print())
end

-- This function gets called once every frame
function draw()
    -- This sets a dark background color 
    background(40, 40, 50)

    -- This sets the line thickness
    strokeWidth(5)
    
    -- fill(255, 0, 0)
    rect(10, 10, WIDTH/2, HEIGHT/4)

    -- Do your drawing here
    
end

