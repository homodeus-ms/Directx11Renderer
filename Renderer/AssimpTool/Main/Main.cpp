// AssimpTool.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "Main.h"
#include "Utils/Converter.h"

void CreateConsole()
{
	// Show Console for debug
	AllocConsole();
	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	std::ios::sync_with_stdio();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CreateConsole();

	// Converter : fbx������ �о ������ ���� �� ���� ������(�޽�.mesh, ���׸���.xml)
	// 
	// <Material XML�� Texture(diffuse, normal ��) ��ũ>
	// 1. fbx�� ��ũ�� ��������� OK
	// 2. �׷��� ���� ���
	//    - ��ҹ��� ���о��� 'diffuse', 'normal', 'specular' ��� �ܾ �� ������ ��������
	//      xml ��ũ�� ����� ���Ե� 

	Converter converter;

	/*converter.LoadRawAssetFile(L"House/House.fbx");
	converter.ExportMaterialData(L"House/House");
	converter.ExportModelData(L"House/House");
	converter.Cleanup();*/

	converter.LoadRawAssetFile(L"Zelda/Zelda.fbx");
	converter.ExportMaterialData(L"Zelda/Zelda");
	converter.ExportModelData(L"Zelda/Zelda");
	converter.Cleanup();

	/*converter.LoadRawAssetFile(L"Tank/Tank.fbx");
	converter.ExportModelData(L"Tank/Tank");
	converter.ExportMaterialData(L"Tank/Tank");
	converter.Cleanup();*/

	/*converter.LoadRawAssetFile(L"Tower/Tower.fbx");
	converter.ExportMaterialData(L"Tower/Tower");
	converter.ExportModelData(L"Tower/Tower");
	converter.Cleanup();*/

	/*converter.LoadRawAssetFile(L"Cottage/Cottage.fbx");
	converter.ExportModelData(L"Cottage/Cottage");
	converter.ExportMaterialData(L"Cottage/Cottage");
	converter.Cleanup();*/
	
	

	LOG(Log, "Fbx Load Complete");

	return 0;
}

