-- Local
local cameraModes = {
    CAMERA_CUSTOM = 0,
    CAMERA_FREE = 1,
    CAMERA_ORBITAL = 2,
    CAMERA_FIRST_PERSON = 3,
    CAMERA_THIRD_PERSON = 4
}

-- Output to c++
windowWidth = 1980
windowHeight = 1080
windowName = "Raylib Lua Engine"
style = "dark"
cameraMode = cameraModes.CAMERA_THIRD_PERSON
