#pragma once

#include <functional>
#include <map>
#include <string>

using namespace std;

namespace SystemEngine
{
	class LoaderAssets
	{
	public:
		// Конструктор
									LoaderAssets();
		// Деструктор
		virtual						~LoaderAssets();

		// Привязка асетов к камерам (запуск)
		void						BindingAssetCameraExecute(string camera_name, function<bool()> lamda_binding_asset_execute);
		// Привязка асетов к камерам (освобождение)
		void						BindingAssetCameraRelease(string camera_name, function<void()> lamda_binding_asset_release);
		// Вызов загрузки ассетов 
		bool						ExecuteAssetCamera(string camera_name);
		// Вызов освобождение ассетов 
		void						ReleaseAssetCamera(string camera_name);

	protected:
		map<string, function<bool()>>	m_binds_assets_execute;
		map<string, function<void()>>	m_binds_assets_release;
	};
}