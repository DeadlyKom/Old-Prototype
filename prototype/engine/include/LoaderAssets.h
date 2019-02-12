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
		// �����������
									LoaderAssets();
		// ����������
		virtual						~LoaderAssets();

		// �������� ������ � ������� (������)
		void						BindingAssetCameraExecute(string camera_name, function<bool()> lamda_binding_asset_execute);
		// �������� ������ � ������� (������������)
		void						BindingAssetCameraRelease(string camera_name, function<void()> lamda_binding_asset_release);
		// ����� �������� ������� 
		bool						ExecuteAssetCamera(string camera_name);
		// ����� ������������ ������� 
		void						ReleaseAssetCamera(string camera_name);

	protected:
		map<string, function<bool()>>	m_binds_assets_execute;
		map<string, function<void()>>	m_binds_assets_release;
	};
}