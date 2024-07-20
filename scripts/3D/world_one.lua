

CAMERA_CUSTOM = 0
CAMERA_FREE = 1                   
CAMERA_ORBITAL = 2                
CAMERA_FIRST_PERSON = 3
CAMERA_THIRD_PERSON = 4

function onStart(scene, ecs)
    print "[world_one] Hello !";
    math.random(os.time());
    -- local color = raylib.Color()

    core.AddCube(ecs, 
        raylib.Vector3():set(0,0,0),
        raylib.Vector3():set(1,1,1), 
        raylib.Color():set(0xffffffff)
    );

    local bounds = 5;
    for i = 0, 10, 1 do
        local x = math.random(-bounds, bounds)
        local y = math.random(-bounds, bounds)
        local z = math.random(-bounds, bounds)
        local color = raylib.Color();
            color.r = math.random(0, 255)
            color.g = math.random(0, 255)
            color.b = math.random(0, 255)
            color.a = math.random(75, 255)
        core.AddCube(ecs, 
            raylib.Vector3():set(x,y,z),
            raylib.Vector3():set(1,1,1), 
           color
        );
    end

    for i = 0, 10, 1 do
        local x = math.random(-bounds, bounds)
        local y = math.random(-bounds, bounds)
        local z = math.random(-bounds, bounds)
        local color = raylib.Color();
            color.r = math.random(0, 255)
            color.g = math.random(0, 255)
            color.b = math.random(0, 255)
            color.a = math.random(75, 255)
        core.AddSphere(ecs, 
            raylib.Vector3():set(x,y,z),
            0.5, 16, 16, 
           color
        );
    end
    scene.camera_mode = CAMERA_ORBITAL;

end


function onEnd(scene)

end