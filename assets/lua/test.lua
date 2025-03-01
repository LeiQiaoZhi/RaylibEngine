print("test.lua loaded")

time = 0

function update()
    time = time + DeltaTime();
    -- print("time: " .. time)

    localPos = GetLocalPosition()
    if (IsKeyDown(65)) then
        -- print("A key pressed")
        localPos:SetX(localPos:X() - 1.0 * DeltaTime())
    elseif (IsKeyDown(68)) then
        -- print("D key pressed")
        localPos:SetX(localPos:X() + 1.0 * DeltaTime())
    elseif (IsKeyDown(87)) then
        -- print("W key pressed")
        localPos:SetZ(localPos:Z() + 1.0 * DeltaTime())
    elseif (IsKeyDown(83)) then
        -- print("S key pressed")
        localPos:SetZ(localPos:Z() - 1.0 * DeltaTime())
    end

    SetLocalPosition(localPos)

end
