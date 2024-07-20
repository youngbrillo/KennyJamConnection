#include "../Scene.h"

#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>

#include "../Application.h"

#define PLATFORM_DESKTOP
#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            120
#endif
#define SHADOWMAP_RESOLUTION 1024

RenderTexture2D LoadShadowmapRenderTexture(int width, int height);
void UnloadShadowmapRenderTexture(RenderTexture2D target);

class ShadowMapScene : public Scene
{
public:
	Camera3D cam = Camera3D{ 0 };
	Shader shadowShader;
	Vector3 lightDir = Vector3Normalize(Vector3{ 0.35f, -1.0f, -0.35f });
	Color lightColor = WHITE;
	int lightDirLoc, lightColLoc, ambientLoc, lightVPLoc, shadowMapLoc, shadowMapResolution;

	Model cube, robot;
	int animCount = 0;
	int fc = 0;
	ModelAnimation* robotAnimations = NULL;
	RenderTexture2D shadowMap;
	Camera3D lightCam = Camera3D{ 0 };
public:
	ShadowMapScene() : Scene("Shaders Shadow Map", "")
	{

		cam.position = Vector3{ 10.0f, 10.0f, 10.0f };
		cam.target = Vector3Zero();
		cam.projection = CAMERA_PERSPECTIVE;
		cam.up = Vector3{ 0.0f, 1.0f, 0.0f };
		cam.fovy = 45.0f;


		//init shadow shader

		shadowShader = LoadShader(TextFormat("resources/shaders/glsl%i/shadowmap.vs", GLSL_VERSION),
			TextFormat("resources/shaders/glsl%i/shadowmap.fs", GLSL_VERSION));
		shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shadowShader, "viewPos");

		Vector4 lightColorNormalized = ColorNormalize(lightColor);
		lightDirLoc = GetShaderLocation(shadowShader, "lightDir");
		lightColLoc = GetShaderLocation(shadowShader, "lightColor");
		SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
		SetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
		ambientLoc = GetShaderLocation(shadowShader, "ambient");
		float ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		SetShaderValue(shadowShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
		lightVPLoc = GetShaderLocation(shadowShader, "lightVP");
		shadowMapLoc = GetShaderLocation(shadowShader, "shadowMap");
		shadowMapResolution = SHADOWMAP_RESOLUTION;
		SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);
		
		// init models
		cube = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
		cube.materials[0].shader = shadowShader;
		robot = LoadModel("resources/models/robot.glb");

		for (int i = 0; i < robot.materialCount; i++)
		{
			robot.materials[i].shader = shadowShader;
		}

		animCount = 0;
		robotAnimations = LoadModelAnimations("resources/models/robot.glb", &animCount);


		//init shadow map
		shadowMap = LoadShadowmapRenderTexture(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);

		// For the shadowmapping algorithm, we will be rendering everything from the light's point of view
		lightCam.position = Vector3Scale(lightDir, -15.0f);
		lightCam.target = Vector3Zero();
		// Use an orthographic projection for directional lights
		lightCam.projection = CAMERA_ORTHOGRAPHIC;
		lightCam.up = Vector3{ 0.0f, 1.0f, 0.0f };
		lightCam.fovy = 20.0f;

		//SetTargetFPS(60);
	}
	~ShadowMapScene()
	{
		//SetTargetFPS(120);
		UnloadShader(shadowShader);
		UnloadModel(cube);
		UnloadModel(robot);
		UnloadModelAnimations(robotAnimations, animCount);
		UnloadShadowmapRenderTexture(shadowMap);

	}
	virtual void initialize();
	virtual void update(const float& dt);
	virtual void fixedUpdate(const float& timestep);
	virtual void Render(Color& clearColor, DrawHook onInspect) override;
	virtual void DrawWorld();
	virtual void poll();
	virtual void Inspect() {};

	static ShadowMapScene* Create(std::string, std::string) { return new ShadowMapScene(); }
};

void ShadowMapScene::initialize()
{
}

void ShadowMapScene::update(const float& dt)
{

	Vector3 cameraPos = cam.position;
	SetShaderValue(shadowShader, shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
	UpdateCamera(&cam, CAMERA_ORBITAL);

	fc++;
	fc %= (robotAnimations[0].frameCount);
	UpdateModelAnimation(robot, robotAnimations[0], fc);

	const float cameraSpeed = 0.05f;
	if (IsKeyDown(KEY_LEFT))
	{
		if (lightDir.x < 0.6f)
			lightDir.x += cameraSpeed * 60.0f * dt;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		if (lightDir.x > -0.6f)
			lightDir.x -= cameraSpeed * 60.0f * dt;
	}
	if (IsKeyDown(KEY_UP))
	{
		if (lightDir.z < 0.6f)
			lightDir.z += cameraSpeed * 60.0f * dt;
	}
	if (IsKeyDown(KEY_DOWN))
	{
		if (lightDir.z > -0.6f)
			lightDir.z -= cameraSpeed * 60.0f * dt;
	}
	lightDir = Vector3Normalize(lightDir);
	lightCam.position = Vector3Scale(lightDir, -15.0f);
	SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

}

void ShadowMapScene::fixedUpdate(const float& timestep)
{
}

void ShadowMapScene::Render(Color& clearColor, DrawHook onInspect)
{
	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();
	// First, render all objects into the shadowmap
	// The idea is, we record all the objects' depths (as rendered from the light source's point of view) in a buffer
	// Anything that is "visible" to the light is in light, anything that isn't is in shadow
	// We can later use the depth buffer when rendering everything from the player's point of view
	// to determine whether a given point is "visible" to the light

	// Record the light matrices for future use!
	Matrix lightView;
	Matrix lightProj;
	BeginTextureMode(shadowMap);
		ClearBackground(App::settings.window_color);
		BeginMode3D(lightCam);
			lightView = rlGetMatrixModelview();
			lightProj = rlGetMatrixProjection();
			//DrawScene(cube, robot);
			DrawWorld();
		EndMode3D();
	EndTextureMode();
	Matrix lightViewProj = MatrixMultiply(lightView, lightProj);

	ClearBackground(App::settings.window_color);

	SetShaderValueMatrix(shadowShader, lightVPLoc, lightViewProj);

	rlEnableShader(shadowShader.id);
	int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
	rlActiveTextureSlot(10);
	rlEnableTexture(shadowMap.depth.id);
	rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

	BeginMode3D(cam);
		// Draw the same exact things as we drew in the shadowmap!
		//DrawScene(cube, robot);
		DrawWorld();
	EndMode3D();

	DrawText("Shadows in raylib using the shadowmapping algorithm!", GetScreenWidth() - 320, GetScreenHeight() - 20, 10, GRAY);
	DrawText("Use the arrow keys to rotate the light!", 10, 10, 30, RED);



	onInspect();
	EndDrawing();
}


void ShadowMapScene::DrawWorld()
{
	DrawModelEx(cube, Vector3Zero(), Vector3{ 0.0f, 1.0f, 0.0f }, 0.0f, Vector3{ 20.0f, 1.0f, 20.0f }, BLUE);
	DrawModelEx(cube, Vector3{ 1.5f, 1.0f, -1.5f }, Vector3{ 0.0f, 1.0f, 0.0f }, 0.0f, Vector3One(), WHITE);
	DrawModelEx(robot, Vector3{ 0.0f, 0.5f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, 0.0f, Vector3{ 1.0f, 1.0f, 1.0f }, RED);
}

void ShadowMapScene::poll()
{
	if (IsKeyPressed(KEY_F))
	{
		TakeScreenshot("assets/shaders_shadowmap.png");
	}
}



RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
{
	RenderTexture2D target = { 0 };

	target.id = rlLoadFramebuffer(width, height); // Load an empty framebuffer
	target.texture.width = width;
	target.texture.height = height;

	if (target.id > 0)
	{
		rlEnableFramebuffer(target.id);

		// Create depth texture
		// We don't need a color texture for the shadowmap
		target.depth.id = rlLoadTextureDepth(width, height, false);
		target.depth.width = width;
		target.depth.height = height;
		target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
		target.depth.mipmaps = 1;

		// Attach depth texture to FBO
		rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

		// Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

		rlDisableFramebuffer();
	}
	else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

	return target;
}

// Unload shadowmap render texture from GPU memory (VRAM)
void UnloadShadowmapRenderTexture(RenderTexture2D target)
{
	if (target.id > 0)
	{
		// NOTE: Depth texture/renderbuffer is automatically
		// queried and deleted before deleting framebuffer
		rlUnloadFramebuffer(target.id);
	}
}

static int entry = SceneEntry::Register("raylib", "shaders shadow map", "", ShadowMapScene::Create);