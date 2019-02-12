#include "prototype\include\SceneObject.h"

#include "prototype\include\Behavior.h"
#include "prototype\include\Material.h"
#include "prototype\include\Mesh.h"
#include "prototype\include\MeshFilter.h"
#include "prototype\include\MeshRender.h"
#include "prototype\include\Shader.h"

#include "prototype\engine\include\Editor.h"
#include "prototype\engine\include\ObjectRegistry.h"
#include "prototype\engine\include\ResourceManager.h"
#include "prototype\engine\include\ShaderRes.h"
#include "prototype\engine\include\TextureRes.h"

namespace SystemEngine
{
	// Конструктор
	Editor::Editor()
	{
		m_resource_manadger = ResourceManager::GetInstance();

		m_object_registry	= ObjectRegistry::GetInstance();

		m_file_manager		= FileManager::GetInstance();
	}

	// Деструктор
	Editor::~Editor()
	{
		for (auto& it_resource : m_list_index_resource)
		{ m_resource_manadger->DeleteResource(it_resource); }

		m_file_manager.reset();

		m_object_registry.reset();

		m_resource_manadger.reset();
	}

	// Инициализация редактора
	bool Editor::Initialization()
	{
		int index_file, index_resource;
		// Создание ресурса шейдера
		shared_ptr<ShaderRes> shader = m_resource_manadger->CreateResourcePtr<ShaderRes>("test", index_resource);
		// Сохраним его для последующего удаления
		m_list_index_resource.push_back(index_resource);
		// Добавим вершинный шейдер
		if (m_file_manager->AddFile(L"ms-appx:///vertex_shader.cso", index_file, false))		{ return true; }
		else { shader->SetFile(ShaderRes::TShader::Vertex, index_file); }
		// Добавим геометрический шейдер
		if (m_file_manager->AddFile(L"ms-appx:///geometry_shader.cso", index_file, false))		{ return true; }
		else { shader->SetFile(ShaderRes::TShader::Geometry, index_file); }
		// Добавим пиксельный шейдер
		if (m_file_manager->AddFile(L"ms-appx:///pixel_shader.cso", index_file, false))			{ return true; }
		else { shader->SetFile(ShaderRes::TShader::Pixel, index_file); }
		//
		vector<String^> names = {L"Strawberries.png", L"5c3402a3c4847c7d7259d4eab07032c5.png", L"644e5e96b384bdf35fefc7abfb41f66b.jpg", L"ash_uvgrid01.png", L"ash_uvgrid02.png", L"ash_uvgrid03.png", L"ash_uvgrid04.png", L"modo_uv_checker.jpg", L"original (1).png", L"original.png", L"PNG_img.png"};
		//
		vector<string> names_texture = { "strawberries", "texture 1", "texture 2", "texture 3", "texture 4", "texture 5", "texture 6", "texture 7", "texture 8", "texture 9", "texture 10" };
		//
		int position = 0;
		//
		for (auto& it_name : names)
		{
			//
			shared_ptr<TextureRes> texture = m_resource_manadger->CreateResourcePtr<TextureRes>(names_texture[position], index_resource, TResource::Texture);
			//
			String^ patch(L"Assets\\Texture\\");
			//
			patch += it_name;
			// Добавим текстуру
			if (m_file_manager->AddFile(patch, index_file, true)) { return true; }
			else { texture->SetFile(TextureRes::TTexture::T2D, index_file); }

			position++;
		}
		// Добавим для шейдера информацию о вершинах
		shader->AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0);
		shader->AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
		shader->AddInputLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 24);
		//Создание объекта сцены
		shared_ptr<ScriptEngine::SceneObject> so = ScriptEngine::SceneObject::Create("Test 2", false, TPrimetive::Tor);
		//Создание скрипта
		shared_ptr<ScriptEngine::Behavior> script = so->AddComponent<ScriptEngine::Behavior>("Test 1");
		
		m_cube = make_shared<Cube>(script);
		srand(602);
		for(int a = 0; a < 10; a++)
		{
			shared_ptr<ScriptEngine::SceneObject> so = ScriptEngine::SceneObject::Create("Test 2", false, TPrimetive::Sphere);
			
			shared_ptr<Material> material = ScriptEngine::Material::Create("Test");
			// Настройка материала
			material->SetShader(ScriptEngine::Shader::Find("test"));

			shared_ptr<Texture2D> texture = ScriptEngine::Texture2D::Create(names_texture[a + 1]);
			//
			material->SetTexture(texture);

			shared_ptr<ScriptEngine::MeshRender> mesh_render = so->GetComponent<MeshRender>();
			//
			mesh_render->SetMaterial(material);

			float pos_x = (float)(rand() & 3) * 0.15f;

			float pos_y = (float)(rand() & 3) * 0.15f;

			float pos_z = (float)(rand() & 3) * 0.15f;

			so->GetTransform()->SetTranslate(Vector3(-pos_x, -pos_y, -2.f - pos_z));
		}
		return false;
	}
}