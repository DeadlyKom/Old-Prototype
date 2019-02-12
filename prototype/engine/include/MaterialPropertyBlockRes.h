#pragma once

#include <array>
#include <d3d11_4.h>
#include <map>
#include <memory>
#include <ppltasks.h>
#include <string>
#include <vector>

#include "prototype\include\Type.h"

#include "ResourceManager.h"
#include "ShaderRes.h"

using namespace std;
using namespace Concurrency;

namespace SystemEngine
{
	class PropertyBlockRes : public IResource
	{
	public:
		// Перечеслние свойств константныйх значений шейдера
		enum class TPropertyShader
		{
			NeverChanges = 0,			// данный тип устанавливается единажды и не меняется
			ChangeOnResize,				// данный тип изменяется только при изменении размеров окна
			ChangeEveryFrame,			// данный тип изменяется один раз за кадр
			ChangeEveryPrim				// данный тип изменяется перед отрисовкой примитива
		};

		enum class TVariable : unsigned int
		{
			Float	= 1,
			Float2	= 2,
			Float3	= 3,
			Float4	= 4,
			Matrix	= 16
		};

		// Структура информации о свойстве
		struct PropertyInfo
		{
			TVariable	type_variable;
		};

	private:
		// Структура хранения информации о свойстве
		struct Property
		{
			TPropertyShader			property_shader;
			TVariable				type_variable;
			int						count_variable;
		};

	public:
									PropertyBlockRes() = delete;
		// Конструктор
									PropertyBlockRes(string name);
		// Деструктор
		virtual						~PropertyBlockRes();
		// Перезагрузить
		virtual task<void>			Reload() override;
		// Освободить
		virtual void				Release() override;
		// Добавить свойство
		int							AddProperty(string name, ShaderRes::TShader type_shader, TPropertyShader property_shader, TVariable type_variable, int count_variable = 1);
		// Обновление константных значений при изменении размера окна
		void						UpdateChangeOnResize(ShaderRes::TShader type_shader);
		// Обновление константных значений раз во фрейм
		bool						UpdateChangeEveryFrame(ShaderRes::TShader type_shader);
		// Обновление константных значений перед отрисовкой примитива
		void						UpdateChangeEveryPrim(ShaderRes::TShader type_shader);
		// Установка константных значений в массив по имени
		void						SetConstantBufferName(ShaderRes::TShader type_shader, TPropertyShader property_shader, string name, float* value, int size);
		// Установка константных значений в массив по индексу
		void						SetConstantBufferIndex(ShaderRes::TShader type_shader, TPropertyShader property_shader, int index, float* value, int size);

		// Создание константного буфера
		bool						CreateConstantBuffer(ID3D11Buffer*& buffer, int size, bool dynamic = false);
		// Установка константного буфера в список общих констант
		//bool						SetConstantBuffer(int start_slot, shared_ptr<vector<ID3D11Buffer*>> array_constants);
		//// Обновить значения константного буфера
		//void						UpdateConstantBuffer(ShaderRes::TShader type_shader);
		// Установить текущий константный буфер
		bool						IncludeConstantBuffer(ShaderRes::TShader type_shader, TPropertyShader property_shader, uint32 start_slot = 0, uint32 count_buffers = 1);

	protected:
		// Системные переменные
		shared_ptr<Device>							m_device;				// ссылка на интерфейс устройства

		// Внутренние переменные

		// 0 - вершинный,
		// 1 - геометрический,
		// 2 - пиксельный
		array<map<string, int>, 3>					m_list_property_name;		// список информационных данных для каждого свойств
		array<array<vector<float>, 4>, 3>			m_list_constants_buffer;	// список буферов
		array<array<ID3D11Buffer*, 4>, 3>			m_array_buffer_shaders;		// массив буферов вершинного шейдера
	};

	typedef PropertyBlockRes MatPropBlok;
}