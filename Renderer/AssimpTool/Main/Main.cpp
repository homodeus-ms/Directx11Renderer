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

	// Converter : fbx파일을 읽어서 데이터 정제 후 따로 저장함(메쉬.mesh, 머테리얼.xml)
	// 
	// <Material XML에 Texture(diffuse, normal 등) 링크>
	// 1. fbx의 링크가 살아있으면 OK
	// 2. 그렇지 않은 경우
	//    - 대소문자 구분없이 'diffuse', 'normal', 'specular' 라는 단어가 들어간 파일이 있을때만
	//      xml 링크에 제대로 포함됨 

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

