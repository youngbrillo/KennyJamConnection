
function onStart(scene, ecs)

    local color = raylib.Color(); color:set(0xffffffff);
    local pos   = raylib.Vector3(); pos:set(0,0,0)
    local scale = raylib.Vector3(); scale:set(1,1,1);

    pos:set(0, -0.5, 0);
    scale:set(20, 1, 20)
    color:set(0x006f4fff);
    local ground = core.AddModel(ecs, 4, pos, scale, color);

    color:set(0x7e7e7eff)
    pos:set(1.5, 0.0, -1.5);
    scale:set(1, 1, 1);
    local cube = core.AddModel(ecs, 0, pos, scale, color);
    local cube = core.AddModel(ecs, 0, pos:set(-5, 3, 5), scale, color);


    local roboIndex = ModelManager.AddModel("resources/models/robot.glb");
    pos:set(0, -0.5, 0);
    scale:set(1, 1, 1);
    local robot = core.AddModel(ecs, roboIndex, pos, scale, color:set(0xff0000ff));


    scene.camera_mode = 2 --free
end

function onEnd(scene)

end


