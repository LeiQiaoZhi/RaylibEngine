print("test.lua loaded")

properties = {
    speed = 1.0,
    health = 100,
    name = "test",
    bool = true,
}

time = 0

function update()
    print(properties.bool)
    speed = properties.speed

    time = time + DeltaTime();
    -- print("time: " .. time)

    localPos = GetLocalPosition()
    if (IsKeyDown(65)) then
        -- print("A key pressed")
        localPos:SetX(localPos:X() - speed * DeltaTime())
    elseif (IsKeyDown(68)) then
        -- print("D key pressed")
        localPos:SetX(localPos:X() + speed * DeltaTime())
    elseif (IsKeyDown(87)) then
        -- print("W key pressed")
        localPos:SetZ(localPos:Z() + speed * DeltaTime())
    elseif (IsKeyDown(83)) then
        -- print("S key pressed")
        localPos:SetZ(localPos:Z() - speed * DeltaTime())
    end

    SetLocalPosition(localPos)

end
