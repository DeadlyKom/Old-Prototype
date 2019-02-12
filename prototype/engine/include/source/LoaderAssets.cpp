#include "..\LoaderAssets.h"

namespace SystemEngine
{
	// �����������
	LoaderAssets::LoaderAssets()
	{
	}

	// ����������
	LoaderAssets::~LoaderAssets()
	{
	}

	// �������� ������ � ������� (������)
	void LoaderAssets::BindingAssetCameraExecute(string camera_name, function<bool()> lamda_binding_asset_execute)
	{
		m_binds_assets_execute[camera_name] = lamda_binding_asset_execute;
	}

	// �������� ������ � ������� (������������)
	void LoaderAssets::BindingAssetCameraRelease(string camera_name, function<void()> lamda_binding_asset_release)
	{
		m_binds_assets_release[camera_name] = lamda_binding_asset_release;
	}

	// ����� �������� ������� 
	bool LoaderAssets::ExecuteAssetCamera(string camera_name)
	{
		return m_binds_assets_execute[camera_name]();
	}

	// ����� ������������ ������� 
	void LoaderAssets::ReleaseAssetCamera(string camera_name)
	{
		m_binds_assets_release[camera_name]();
	}
}
