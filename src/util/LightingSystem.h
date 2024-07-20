#pragma once
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <raylib/rlgl.h>
#include <functional>
#include <imgui.h>

class LightingSystem
{
public:
	Shader shader;
	Camera3D camera = Camera3D{ 0 };
	Vector3 lightDirection = Vector3Normalize(Vector3{ 0.35f, -1.0f, -0.35f });
	Color lightColor = WHITE;
	Vector4 AmbientLight = { 0.1f, 0.1f, 0.1f, 1.0f };
public:
	LightingSystem() {};
	~LightingSystem()
	{
		if (shadowMap.id > 0)
		{
			// NOTE: Depth texture/renderbuffer is automatically
			// queried and deleted before deleting framebuffer
			rlUnloadFramebuffer(shadowMap.id);
		}
		UnloadShader(shader);
	}
	inline void Initialize(const char* vertexPath, const char* fragPath, int resolution = 1024);
	inline void Update(Camera3D& scene_camera, const float& dt);
	inline void DrawShadowMap(Color& backgroundColor, std::function<void()> drawfunc);
	inline void LoadShadowmapRenderTexture(int width, int height);
	inline void Inspect(const char* title = "Global Lighting System");
private:
	int lightDirLoc, lightColLoc, ambientLoc, lightVPLoc, shadowMapLoc, shadowMapResolution;
	RenderTexture2D shadowMap;
};



inline void LightingSystem::Initialize(const char* vertexPath, const char* fragPath, int resolution)
{
	//init shadow shader
	shader = LoadShader(vertexPath, fragPath);
	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

	Vector4 lightColorNormalized = ColorNormalize(lightColor);
	lightDirLoc = GetShaderLocation(shader, "lightDir");
	lightColLoc = GetShaderLocation(shader, "lightColor");
	SetShaderValue(shader, lightDirLoc, &lightDirection, SHADER_UNIFORM_VEC3);
	SetShaderValue(shader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
	ambientLoc = GetShaderLocation(shader, "ambient");
	SetShaderValue(shader, ambientLoc, &AmbientLight, SHADER_UNIFORM_VEC4);
	lightVPLoc = GetShaderLocation(shader, "lightVP");
	shadowMapLoc = GetShaderLocation(shader, "shadowMap");
	//init shadow map
	shadowMapResolution = resolution;
	SetShaderValue(shader, GetShaderLocation(shader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);
	LoadShadowmapRenderTexture(shadowMapResolution, shadowMapResolution);
	//init the camera
	// For the shadowmapping algorithm, we will be rendering everything from the light's point of view
	camera.position = Vector3Scale(lightDirection, -15.0f);
	camera.target = Vector3Zero();
	// Use an orthographic projection for directional lights
	camera.projection = CAMERA_ORTHOGRAPHIC;
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 20.0f;
}
inline void LightingSystem::Update(Camera3D& scene_camera, const float& dt)
{
	Vector3 cameraPos = scene_camera.position;
	SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
	//can place code to move light around here
	// ---- 
	// ---- 
	// ---- 
	lightDirection = Vector3Normalize(lightDirection);
	camera.position = Vector3Scale(lightDirection, -15.0f);
	SetShaderValue(shader, lightDirLoc, &lightDirection, SHADER_UNIFORM_VEC3);
}
inline void LightingSystem::DrawShadowMap(Color& backgroundColor, std::function<void()> drawfunc)
{
	// Record the light matrices for future use!
	Matrix lightView;
	Matrix lightProj;

	BeginTextureMode(shadowMap);
	ClearBackground(backgroundColor);
	BeginMode3D(camera);
	lightView = rlGetMatrixModelview();
	lightProj = rlGetMatrixProjection();
	drawfunc();
	EndMode3D();
	EndTextureMode();

	Matrix lightViewProj = MatrixMultiply(lightView, lightProj);
	//ClearBackground(backgroundColor);
	SetShaderValueMatrix(shader, lightVPLoc, lightViewProj);

	rlEnableShader(shader.id);
	int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
	rlActiveTextureSlot(10);
	rlEnableTexture(shadowMap.depth.id);
	rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

}
inline void LightingSystem::LoadShadowmapRenderTexture(int width, int height)
{
	shadowMap = { 0 };
	shadowMap.id = rlLoadFramebuffer(width, height); // Load an empty framebuffer
	shadowMap.texture.width = width;
	shadowMap.texture.height = height;

	if (shadowMap.id > 0)
	{
		rlEnableFramebuffer(shadowMap.id);

		// Create depth texture
		// We don't need a color texture for the shadowmap
		shadowMap.depth.id = rlLoadTextureDepth(width, height, false);
		shadowMap.depth.width = width;
		shadowMap.depth.height = height;
		shadowMap.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
		shadowMap.depth.mipmaps = 1;

		// Attach depth texture to FBO
		rlFramebufferAttach(shadowMap.id, shadowMap.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

		// Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(shadowMap.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", shadowMap.id);

		rlDisableFramebuffer();
	}
	else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");
}
void LightingSystem::Inspect(const char* title)
{

	if (ImGui::TreeNode(title))
	{

		Vector4 lightColorNormalized = ColorNormalize(lightColor);
		if (ImGui::ColorEdit4("Light Color", &lightColorNormalized.x))
		{
			lightColor = ColorFromNormalized(lightColorNormalized);
			SetShaderValue(shader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
		}

		if (ImGui::ColorEdit4("Ambient", &AmbientLight.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float))
		{
			SetShaderValue(shader, ambientLoc, &AmbientLight, SHADER_UNIFORM_VEC4);
		}

		ImGui::SliderFloat3("Light Direction", &lightDirection.x, -1.0f, 1.0f);

		ImGui::Text("Light Direction [%d]\nLight Color [%d]\nLight VP [%d]\nShadow Map [%d]\nResolution [%d]\n",
			lightDirLoc, lightColLoc, lightVPLoc, shadowMapLoc, shadowMapResolution
		);

		ImGui::TreePop();
	}

}