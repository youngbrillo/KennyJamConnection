#include "raylib_script_extensions.h"
#include <LuaBridge/LuaBridge.h>

namespace raylib
{
	static Vector2* setVector2(Vector2* v, float x, float y) {
		v->x = x; v->y = y;
		return v;
	}
	Vector3* setVector3(Vector3* v, float x, float y, float z) {
		v->x = x; v->y = y; v->z = z;
		return v;
	}

	static Rectangle* setRectangle(Rectangle* v, float x, float y, float w, float h){
		v->x = x; v->y = y; v->width = w; v->height = h;
		return v;
	}

	static Color* setColor(Color* v, unsigned int hexValue)
	{
		*v = GetColor(hexValue);
		return v;
	}

	//MATERIAL_MAP_DIFFUSE == 0
	static Model* setModelTexture(Model* m, Texture2D texture, int mapIndex = 0, int material_map_index = MATERIAL_MAP_DIFFUSE)
	{
		m->materials[mapIndex].maps[material_map_index].texture = texture;

		return m;
	};
}


void raylib::script_extend(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginNamespace("raylib")
		.beginClass<Vector2>("Vector2")
			.addConstructor <void (*) ()>()
			//.addConstructor <void (*) (float, float)>()
			.addData("x", &Vector2::x).addData("y", &Vector2::y)
			.addFunction("set", setVector2)
		.endClass()
		.beginClass<Vector3>("Vector3")
			.addConstructor<void(*)()>()
			.addData("x", &Vector3::x).addData("y", &Vector3::y).addData("z", &Vector3::z)
			.addFunction("set", setVector3)
		.endClass()
		.beginClass<Rectangle>("Rectangle")
			.addConstructor <void (*) ()>()
			//.addConstructor <void (*) (float, float, float, float)>()
			.addData("x", &Rectangle::x).addData("y", &Rectangle::y)
			.addData("width", &Rectangle::width).addData("height", &Rectangle::height)
			.addFunction("set", setRectangle)
		.endClass()
		.beginClass<Image>("Image")
			.addData("width", &Image::width)
			.addData("height", &Image::height)
		.endClass()
		.beginClass<Texture2D>("Texture2D")
			.addData("id", &Texture2D::id)
			.addData("height", &Texture2D::height)
			.addData("width", &Texture2D::width)
		.endClass()
		.beginClass<Color>("Color")
			.addConstructor <void (*) ()>()
			.addData("r", &Color::r)
			.addData("g", &Color::g)
			.addData("b", &Color::b)
			.addData("a", &Color::a)
			//.addFunction("Alpha", &Color::Alpha)
			//.addFunction("ToInt", &Color::ToInt)
			.addFunction("set", raylib::setColor)
		.endClass()
		.beginClass<Camera2D>("Camera2D")
			.addData("offset", &Camera2D::offset)
			.addData("target", &Camera2D::target)
			.addData("zoom", &Camera2D::zoom)
			.addData("rotation", &Camera2D::rotation)
		.endClass()
		.beginClass<Camera3D>("Camera3D")
		.addData("position", &Camera3D::position)
		.addData("target", &Camera3D::target)
		.addData("up", &Camera3D::up)
		.addData("fovy", &Camera3D::fovy)
		.addData("projection", &Camera3D::projection)
		.endClass()
		//images
		.addFunction("LoadTexture", &LoadTexture)
		.addFunction("LoadImage", &LoadImage)
		//textures
		.addFunction("LoadImageFromTexture", &LoadImageFromTexture)
		.addFunction("unLoadTexture", &UnloadTexture)
		.addFunction("UnloadImage", &UnloadImage)
		.addFunction("SetWindowIcon", &SetWindowIcon)
		.addFunction("GetScreenWidth", &GetScreenWidth)
		.addFunction("GetScreenHeight", &GetScreenHeight)
		.addFunction("GetScreenToWorld2D", &GetScreenToWorld2D)
		.addFunction("GetWorldToScreen2D", &GetWorldToScreen2D)
		.addFunction("GetMousePosition", &GetMousePosition)
		.addFunction("IsKeyPressed", &IsKeyPressed)
		.addFunction("IsKeyDown", &IsKeyDown)
		.addFunction("IsKeyReleased", &IsKeyReleased)
		.addFunction("IsKeyUp", &IsKeyUp)
		.addFunction("GetKeyPressed", &GetKeyPressed)
		.addFunction("GetCharPressed", &GetCharPressed)
		.addFunction("SetExitKey", &SetExitKey)
		.addFunction("IsMouseButtonPressed", &IsMouseButtonPressed)
		.addFunction("IsMouseButtonDown", &IsMouseButtonDown)
		.addFunction("IsMouseButtonReleased", &IsMouseButtonReleased)
		.addFunction("IsMouseButtonUp", &IsMouseButtonUp)
		.addFunction("GetMousePosition", &GetMousePosition)
		.addFunction("GetMouseDelta", &GetMouseDelta)
		.addFunction("GetMouseWheelMove", &GetMouseWheelMove)
		//3d
		//models
		.beginClass<BoundingBox>("BoundingBox")
			.addData("min", &BoundingBox::min)
			.addData("max", &BoundingBox::max)
		.endClass()
		.beginClass<Model>("Model")
			.addFunction("AddTexture", &raylib::setModelTexture)
		.endClass()
		.addFunction("LoadModel", LoadModel)
		.addFunction("LoadModelFromMesh", LoadModelFromMesh)
		.addFunction("IsModelReady", IsModelReady)
		.addFunction("UnloadModel", UnloadModel)
		.addFunction("GetModelBoundingBox", GetModelBoundingBox)
		//meshes
		.beginClass<Mesh>("Mesh").endClass()
		// Mesh management functions
		//.addFunction("UploadMesh", UploadMesh)
		//.addFunction("UpdateMeshBuffer", UpdateMeshBuffer)
		.addFunction("UnloadMesh", UnloadMesh)
		.addFunction("DrawMesh", DrawMesh)
		//.addFunction("DrawMeshInstanced", DrawMeshInstanced)
		.addFunction("ExportMesh", ExportMesh)
		.addFunction("GetMeshBoundingBox",GetMeshBoundingBox)
		//.addFunction("GetMeshBoundingBox", GenMeshTangents)
		/*
		// Mesh generation functions
		Mesh GenMeshPoly(int sides, float radius);                                            // Generate polygonal mesh
		Mesh GenMeshPlane(float width, float length, int resX, int resZ);                     // Generate plane mesh (with subdivisions)
		Mesh GenMeshCube(float width, float height, float length);                            // Generate cuboid mesh
		Mesh GenMeshSphere(float radius, int rings, int slices);                              // Generate sphere mesh (standard sphere)
		Mesh GenMeshHemiSphere(float radius, int rings, int slices);                          // Generate half-sphere mesh (no bottom cap)
		Mesh GenMeshCylinder(float radius, float height, int slices);                         // Generate cylinder mesh
		Mesh GenMeshCone(float radius, float height, int slices);                             // Generate cone/pyramid mesh
		Mesh GenMeshTorus(float radius, float size, int radSeg, int sides);                   // Generate torus mesh
		Mesh GenMeshKnot(float radius, float size, int radSeg, int sides);                    // Generate trefoil knot mesh
		Mesh GenMeshHeightmap(Image heightmap, Vector3 size);                                 // Generate heightmap mesh from image data
		Mesh GenMeshCubicmap(Image cubicmap, Vector3 cubeSize);                               // Generate cubes-based map mesh from image data

		// Material loading/unloading functions
		Material* LoadMaterials(const char* fileName, int* materialCount);                    // Load materials from model file
		Material LoadMaterialDefault(void);                                                   // Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
		bool IsMaterialReady(Material material);                                              // Check if a material is ready
		void UnloadMaterial(Material material);                                               // Unload material from GPU memory (VRAM)
		void SetMaterialTexture(Material * material, int mapType, Texture2D texture);          // Set texture for a material map type (MATERIAL_MAP_DIFFUSE, MATERIAL_MAP_SPECULAR...)
		void SetModelMeshMaterial(Model * model, int meshId, int materialId);                  // Set material for a mesh

		// Model animations loading/unloading functions
		ModelAnimation* LoadModelAnimations(const char* fileName, int* animCount);            // Load model animations from file
		void UpdateModelAnimation(Model model, ModelAnimation anim, int frame);               // Update model animation pose
		void UnloadModelAnimation(ModelAnimation anim);                                       // Unload animation data
		void UnloadModelAnimations(ModelAnimation * animations, int animCount);                // Unload animation array data
		bool IsModelAnimationValid(Model model, ModelAnimation anim);                         // Check model animation skeleton match
		*/
		.beginClass<Ray>("Ray")
			.addData("position", &Ray::position)
			.addData("direction", &Ray::direction)
		.endClass()
		.beginClass<RayCollision>("RayCollision")
		.addData("hit", &RayCollision::hit)
			.addData("distance", &RayCollision::distance)
			.addData("point", &RayCollision::point)
			.addData("normal", &RayCollision::normal)
		.endClass()
		// Collision detection functions
		.addFunction("CheckCollisionSpheres", CheckCollisionSpheres)
		.addFunction("CheckCollisionBoxes", CheckCollisionBoxes)
		.addFunction("CheckCollisionBoxSphere", CheckCollisionBoxSphere)
		.addFunction("GetRayCollisionSphere", GetRayCollisionSphere)
		.addFunction("GetRayCollisionBox", GetRayCollisionBox)
		.addFunction("GetRayCollisionMesh", GetRayCollisionMesh)
		.addFunction("GetRayCollisionTriangle", GetRayCollisionTriangle)
		.addFunction("GetRayCollisionQuad", GetRayCollisionQuad)
	.endNamespace();
}
