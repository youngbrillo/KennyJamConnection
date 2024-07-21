function onStart(scene, ecs)
    local ground = core.AddModel(ecs, 4, raylib.Vector3():set(0,0,0), raylib.Vector3():set(10, 10, 10), raylib.Color():set(0x7c7c00ff));

    scene.camera_mode = 2 --orbit
end

function onEnd(scene, ecs)

end