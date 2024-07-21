
textures = {
    {path="variation-a.png"},
    {path="variation-b.png"},
    {path="variation-c.png"},
};

function onStart(scene, ecs)
    loadResources();
    LoadModels(ecs)
    
    scene.draw_grid = true;
    scene.grid_slices = 40;
    -- scene.postProcessor:AddShader("assets/shaders/pixelizer.fs");
    scene.draw_shadows = false;
end

function onEnd()
    unloadResources();
end


function loadResources()
    for k, v in ipairs(textures) do
        local path = "assets/models/marbles/Textures/"..v.path;
        v.texture = raylib.LoadTexture(path)
    end

    ModelManager.AddModelsFromDirectory("assets/models/marbles", 0, -1);
end

function unloadResources()
    for k, v in ipairs(textures) do
        raylib.unLoadTexture(v.texture);
    end
    -- ModelManager.Clear();
end


function LoadModels(ecs)
    local columns = 10;
    local spacing = 4;
    local start = -20;
    local z = -16;
    local x = start;
    local y = 0;
    local ii = 1;
    for i = 6, ModelManager.GetModelCount()-1, 1 do
        local mod = ModelManager.GetModel(i);
        mod:AddTexture(textures[1].texture, 0,0);

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
            ii = 0;   
        end        
    end
end
