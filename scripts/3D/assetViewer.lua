
textures = {
    {path="car7_black.png"},
    {path="car7_red.png"},
    {path="car7_green.png"},
    {path="car7_grey.png"},
};

function onStart(scene, ecs)
    loadResources();
    LoadModels(ecs)
    local s = 50;
    local ground = core.AddModel(ecs, 4, 
        raylib.Vector3():set(0,0,0), 
        raylib.Vector3():set(s, s, s), 
        raylib.Color():set(0x4c4c5eff));
    
    -- scene.draw_grid = false;
    -- scene.postProcessor:AddShader("assets/shaders/pixelizer.fs");
    scene.draw_shadows = false;
    scene.scene_camera.position:set(0, 118, 118);
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


function LoadModels(ecs)
    local columns = 3;
    local spacing = 8;
    local start = -(columns * (spacing + 1))/2
    local z = start;
    local x = start;
    local y = 0;
    local ii = 1;
    for i = 6, ModelManager.GetModelCount()-1, 1 do
        local mod = ModelManager.GetModel(i);
        -- mod:AddTexture(textures[1].texture, 0,0);

        if( mod ~= nil) then
            core.AddModel(ecs, i, 
                raylib.Vector3():set(x,y,z),
                raylib.Vector3():set(1,1,1), 
                raylib.Color():set(0xffffffff)
            );
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
