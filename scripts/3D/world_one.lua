

CAMERA_CUSTOM = 0
CAMERA_FREE = 1                   
CAMERA_ORBITAL = 2                
CAMERA_FIRST_PERSON = 3
CAMERA_THIRD_PERSON = 4

function onStart(scene, ecs)
    print "[world_one] Hello !";
    math.random(os.time());
    -- local color = raylib.Color()


    local bounds = 10;
    local ground = core.AddCube(ecs, raylib.Vector3():set(0,-2.0,0),raylib.Vector3():set(bounds*3,0.5,bounds*3), raylib.Color():set(0x5e5e5e7c));
    scene.physics:AddBoxBody(ecs, ground, true);

    for i = 1, 50, 1 do
        local x = math.random(-bounds, bounds) + math.random()
        local y = math.random(0, bounds*2) + math.random()
        local z = math.random(-bounds, bounds) + math.random()
        local color = raylib.Color();
            color.r = math.random(0, 255)
            color.g = math.random(0, 255)
            color.b = math.random(0, 255)
            color.a = math.random(75, 255)
        local e = core.AddCube(ecs, 
            raylib.Vector3():set(x,y,z),
            raylib.Vector3():set(1,1,1), 
           color
        );
        scene.physics:AddBoxBody(ecs, e, false);

    end

    for i = 1, 50, 1 do
        local x = math.random(-bounds, bounds)
        local y = math.random(0, bounds *2)
        local z = math.random(-bounds, bounds)
        local color = raylib.Color();
            color.r = math.random(0, 255)
            color.g = math.random(0, 255)
            color.b = math.random(0, 255)
            color.a = math.random(75, 255)
        local e = core.AddSphere(ecs, raylib.Vector3():set(x,y,z),1.0, color);
        scene.physics:AddSphereBody(ecs, e);

    end

    -- LoadModels(ecs)
    scene.camera_mode = CAMERA_ORBITAL;

    -- scene.draw_grid = true;
    scene.grid_slices = 12;
    -- scene.postProcessor:AddShader("assets/shaders/grayscale.fs");
    -- scene.postProcessor:AddShader("assets/shaders/posterization.fs");
    -- scene.postProcessor:AddShader("assets/shaders/dream_vision.fs");
    -- scene.postProcessor:AddShader("assets/shaders/pixelizer.fs");
    -- scene.postProcessor:AddShader("assets/shaders/cross_hatching.fs");
    -- scene.postProcessor:AddShader("assets/shaders/cross_stitching.fs");
    -- scene.postProcessor:AddShader("assets/shaders/predator.fs");
    -- scene.postProcessor:AddShader("assets/shaders/scanlines.fs");
    -- scene.postProcessor:AddShader("assets/shaders/fisheye.fs");
    -- scene.postProcessor:AddShader("assets/shaders/sobel.fs");
    -- scene.postProcessor:AddShader("assets/shaders/bloom.fs");
    -- scene.postProcessor:AddShader("assets/shaders/blur.fs");

    scene.draw_shadows = false;
end
models = {};
textures = {};
function LoadModels(ecs)
    models = {
        {model =0, tindex=1, path="assets/models/city/building-sample-house-a.obj"},
        {model =0, tindex=1, path="assets/models/city/building-sample-house-b.obj"},
        {model =0, tindex=1, path="assets/models/city/building-sample-house-c.obj"},
        {model =0, tindex=1, path="assets/models/city/building-sample-tower-a.obj"},
        {model =0, tindex=1, path="assets/models/city/building-sample-tower-b.obj"},
        {model =0, tindex=1, path="assets/models/city/building-sample-tower-c.obj"},
        {model =0, tindex=1, path="assets/models/city/building-sample-tower-d.obj"},
    }
    

    textures = {
       {texture = 0, path="assets/models/city/Textures/colormap.png"},
    }
    
    for k, v in ipairs(textures) do
        v.texture = raylib.LoadTexture(v.path)
    end

    for k, v in ipairs(models) do
        v.model = raylib.LoadModel(v.path);
        v.model:AddTexture(textures[v.tindex].texture, 0,0);
    end


    local bounds = 5;
    for i = 1, #models, 1 do
        local x = math.random(-bounds, bounds) + 0.5
        local y = 0;
        local z = math.random(-bounds, bounds) + 0.5

        core.AddModel(ecs, models[i].model, 
            raylib.Vector3():set(x,y,z),
            raylib.Vector3():set(1,1,1), 
            raylib.Color():set(0xffffffff)
        );
    end
end

function onEnd(scene)
    for k, v in ipairs(models) do
        raylib.UnloadModel(v.model);
    end
    for k, v in ipairs(textures) do
        raylib.unLoadTexture(v.texture);
    end
end