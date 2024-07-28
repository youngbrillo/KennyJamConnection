
textures = {
    {path="car7_black.png"},
    {path="car7_red.png"},
    {path="car7_green.png"},
    {path="car7_grey.png"},
};

function onStart(scene, ecs)
    loadResources();
    local s = 250;
    local ground = core.AddModel(ecs, 0, 
        raylib.Vector3():set(0,-1,0), 
        raylib.Vector3():set(s, 0.5, s), 
        raylib.Color():set(0x4c4c5eff));

    scene.physics:AddBoxBody(ecs, ground, true);

    LoadModels(scene, ecs)
    
    -- scene.draw_grid = false;
    -- scene.postProcessor:AddShader("assets/shaders/pixelizer.fs");
    scene.draw_shadows = false;
    scene.scene_camera.position:set(0, 118, 118);
    -- scene.scene_camera.position:set(-33, 10, -14);
    -- scene.scene_camera.target:set(91, -28, 35);

    scene.scene_camera.position:set(-25, 10, 20);
    scene.scene_camera.position:set(-9, 14, -40);
    scene.scene_camera.target:set(-8, 0, -4);
    scene.camera_mode = 2
    scene.enable_camera_controller = false;
end

function onEnd()
    unloadResources();
end


function loadResources()
    for k, v in ipairs(textures) do
        local path = "assets/models/vehicles/"..v.path;
        v.texture = raylib.LoadTexture(path)
    end

    ModelManager.AddModelsFromDirectory("assets/models/vehicles", 0, -1);
end

function unloadResources()
    for k, v in ipairs(textures) do
        raylib.unLoadTexture(v.texture);
    end
    -- ModelManager.Clear();
end


function LoadModels(scene, ecs)
    local columns = 3;
    local spacing = 8;
    local start = -(columns * (spacing + 1))/2
    local z = start;
    local x = start;
    local y = 2;
    local ii = 1;

    local rc_index = 7;

    for i = 6, ModelManager.GetModelCount()-1, 1 do
        local mod = ModelManager.GetModel(i);
        -- mod:AddTexture(textures[1].texture, 0,0);

        if( mod ~= nil) then
            local e = core.AddModel(ecs, i, 
                raylib.Vector3():set(x,y,z),
                raylib.Vector3():set(1,1,1), 
                raylib.Color():set(0xffffffff)
            );

            -- scene.physics:AddBoxBody(ecs, e, false);
            local rb = scene.physics:AddBoxBodyEX(e, 1, 0.5, 3, false);

            if rc_index == i then

                local vc = core.AttachRigidBodyVehicleController(e, 20, 8);
                vc.brake_key = 264;
                print "Attached Vechicle controller!"
                local material = rb.collider:getMaterial();
                material.friction = 0.2;
                material.bounciness = 0.75;
            end
        end
        ii = ii + 1;
        x = x + 1 + spacing;
        if(ii > columns) then
            x = start;
            z = z + 1 + spacing;         
            ii = 1;   
        end        


    end
end
