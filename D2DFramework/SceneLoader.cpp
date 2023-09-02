#include "SceneLoader.h"

#include "ObjectManager.h"
#include "GameObject.h"
#include "RenderManger.h"
#include "Scene.h"
#include "FrameInformation.h"

#include <fstream>
#include <codecvt>

namespace d2dFramework
{

	void SceneLoader::LoadScene(Scene* outScene)
	{

		const unsigned int outSceneID = outScene->GetId();

		///파일 패스 찾기!
		std::string mapFilePath;
		std::ifstream ifs(L"./json/MapList.json", std::ios::in);
		if (!ifs.is_open())
		{
			assert(false);
			throw std::runtime_error("Object not found."); // 에러를 발생시킴
		}
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();
		nlohmann::ordered_json MapListobj = nlohmann::ordered_json::parse(str);
		for (auto& JsonMapobj : MapListobj["MapList"])
		{
			unsigned int mapID = JsonMapobj["Map_ID"].get<unsigned int>();
			if (outSceneID == mapID)
			{
				mapFilePath = JsonMapobj["FilePath"].get<std::string>();
				break;
			}
		}
		if (!mapFilePath.empty())
		{
			std::ifstream ifs2(mapFilePath, std::ios::in);
			if (ifs2.is_open())
			{
				std::string mainstr((std::istreambuf_iterator<char>(ifs2)), std::istreambuf_iterator<char>());
				ifs2.close();
				nlohmann::ordered_json jsonGameObjectContainer = nlohmann::ordered_json::parse(mainstr);
				outScene->SerializeIn(jsonGameObjectContainer);
			}
		}

		///찾은 파일Path로 Json 열기 



	}


	void SceneLoader::SaveScene(Scene* outScene)
	{
		///지역변수 생성.
		unsigned int outSceneID = outScene->GetId();
		std::string mapFilePath;

		///맵리스트json열어 파일명 가져오기.
		std::ifstream ifs(L"./json/MapList.json", std::ios::in);
		if (!ifs.is_open())
		{
			assert(false);
			throw std::runtime_error("Object not found."); // 에러를 발생시킴
		}
		std::string strJsonMapList((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();
		nlohmann::ordered_json jsonMaplist = nlohmann::ordered_json::parse(strJsonMapList);

		for (auto& jsonMapObj : jsonMaplist["MapList"])
		{
			unsigned int mapID = jsonMapObj["Map_ID"].get<unsigned int>();
			if (outSceneID == mapID)
			{
				mapFilePath = jsonMapObj["FilePath"].get<std::string>();

				break;
			}
		}
		if (!mapFilePath.empty())
		{
			nlohmann::ordered_json jsonGameObjectContainer;
			std::ofstream ofsmapFilePath(mapFilePath);
			outScene->SerializeOut(jsonGameObjectContainer);
			// 제인슨 객체 파일에 저장
			// 파일 닫기
			ofsmapFilePath << jsonGameObjectContainer.dump(4);
			ofsmapFilePath.close();
		}
		else
		{
			nlohmann::ordered_json jsonNewMapInfo;
			std::string tempA = "json/Map/Map";
			int tempB = outSceneID;
			std::string tempC = ".json";
			mapFilePath = tempA + std::to_string(tempB) + tempC;
			jsonNewMapInfo["Map_ID"] = outSceneID;
			jsonNewMapInfo["FilePath"] = mapFilePath;
			jsonMaplist["MapList"].push_back(jsonNewMapInfo);
			std::ofstream ofsMapList(L"json/MapList.json");
			ofsMapList << jsonMaplist.dump(4);
			ofsMapList.close();

			nlohmann::ordered_json jsonGameObjectContainer;
			std::ofstream ofsmapFilePath(mapFilePath);
			outScene->SerializeOut(jsonGameObjectContainer);
			ofsmapFilePath << jsonGameObjectContainer.dump(4);
			ofsmapFilePath.close();
		}
	}

	void SceneLoader::LoadAllAnimationAssets()
	{

		// 애니메이션 리스트 열어서 리스트를 제이슨형태로 저장.
		std::ifstream ifsAnimationAssetList(L"./json/AnimationAssetList.json", std::ios::in);
		if (!ifsAnimationAssetList.is_open())
		{
			assert(false);
			throw std::runtime_error("Object not found."); // 에러를 발생시킴
		}
		std::string strAnimationList((std::istreambuf_iterator<char>(ifsAnimationAssetList)), std::istreambuf_iterator<char>());
		ifsAnimationAssetList.close();
		nlohmann::ordered_json jsonAnimationAssetList = nlohmann::ordered_json::parse(strAnimationList);

		//저장된 리스트제이슨을 순회하며 모든 애니메이션 에셋을 Load 하기
		for (auto jsonAnimationAssetFileInfo : jsonAnimationAssetList["AnimationAssetList"])
		{
			// AnimationAsset의 AnimationKey 와 Path 를 저장
			const std::string strAnimationKey = jsonAnimationAssetFileInfo["AnimationAssetKey"].get<std::string>();
			const std::string strAnimationAssetPath = jsonAnimationAssetFileInfo["FilePath"].get<std::string>();

			//Path를 통해 해당 AnimationAsset열어서 json형태로 저장
			std::ifstream ifsAnimationAsset(strAnimationAssetPath, std::ios::in);
			if (!ifsAnimationAsset.is_open())
				throw std::runtime_error("Object not found."); // 에러를 발생시킴
			std::string strAnimationAsset((std::istreambuf_iterator<char>(ifsAnimationAssetList)), std::istreambuf_iterator<char>());
			ifsAnimationAsset.close();
			nlohmann::ordered_json jsonAnimationAsset = nlohmann::ordered_json::parse(strAnimationAsset);

			// 애니메이션 상태 별 FrameInformation 정보(frameInformations vector)를 담을 vecFrameInformation.
			std::vector<std::vector<FrameInformation>> vecFrameInformation;
			for (auto jsonAnimationState : jsonAnimationAsset["AnimationState"])
			{
				//애니메이션 상태에 따른 FrameInformation정보를 닮을 frameInformations vector.
				std::vector<FrameInformation> frameInformations;
				for (auto jsonAnimationFrameInformation : jsonAnimationState["FrameInformations"])
				{
					FrameInformation frameInformation;
					frameInformation.AnimationTime = jsonAnimationFrameInformation["AnimationTime"];
					frameInformation.UVRectangle.left = jsonAnimationFrameInformation["UVRectangle_left"];
					frameInformation.UVRectangle.top = jsonAnimationFrameInformation["UVRectangle_top"];
					frameInformation.UVRectangle.right = jsonAnimationFrameInformation["UVRectangle_right"];
					frameInformation.UVRectangle.bottom = jsonAnimationFrameInformation["UVRectangle_left_bottom"];
					frameInformations.push_back(frameInformation);
				}

				vecFrameInformation.push_back(frameInformations);

			}

			// 저장된 애니메이션 키값, 파일경로, frameInformations를 사용하여 Asset 생성.
			const WCHAR* WCHARAnimationKey = reinterpret_cast<const WCHAR*>(strAnimationKey.c_str());
			const WCHAR* WCHARAnimePath = reinterpret_cast<const WCHAR*>(strAnimationAssetPath.c_str());
			RenderManager::GetInstance()->CreateAnimationAsset(WCHARAnimationKey, WCHARAnimePath, vecFrameInformation);
		}

	}

	void SceneLoader::LoadAllBitmaps()
	{
		// 애니메이션 리스트 열어서 리스트를 제이슨형태로 저장.
		std::ifstream ifsBitmapList(L"./json/BitmapList.json", std::ios::in);
		if (!ifsBitmapList.is_open())
		{
			assert(false);
			throw std::runtime_error("Object not found."); // 에러를 발생시킴
		}

		std::string strBitmapList((std::istreambuf_iterator<char>(ifsBitmapList)), std::istreambuf_iterator<char>());
		ifsBitmapList.close();
		nlohmann::ordered_json jsonBitmapList = nlohmann::ordered_json::parse(strBitmapList);

		//저장된 리스트제이슨을 순회하며 모든 애니메이션 에셋을 Load 하기
		for (auto jsonBitmapInfomations : jsonBitmapList["BitmapList"])
		{
			// AnimationAsset의 BitmapKey 와 BitmapPath 를 저장
			std::string strBitmapKey = jsonBitmapInfomations["BitmapKey"].get<std::string>();
			std::string strBitmapPath = jsonBitmapInfomations["FilePath"].get<std::string>();
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wstrBitmapKey = converter.from_bytes(strBitmapKey);
			std::wstring wstrBitmapPath = converter.from_bytes(strBitmapPath);


			const WCHAR* WCHARAnimationKey = reinterpret_cast<const WCHAR*>(wstrBitmapKey.c_str());
			const WCHAR* WCHARAnimePath = reinterpret_cast<const WCHAR*>(wstrBitmapPath.c_str());
			bool a = !jsonBitmapInfomations.contains("FramesPath");
			if (!jsonBitmapInfomations.contains("FramesPath"))
			{
				HRESULT hr = RenderManager::GetInstance()->CreateD2DBitmapFromFile(WCHARAnimationKey, WCHARAnimePath);
				assert(SUCCEEDED(hr));
			}
			else
			{
				const std::string strFramesPath = jsonBitmapInfomations["FramesPath"].get<std::string>();
				std::ifstream ifsBitmapFramesPath(L"json/Bit.json", std::ios::in);
				if (!ifsBitmapFramesPath.is_open())
					throw std::runtime_error("Object not found."); // 에러를 발생시킴
				std::string strBitmapInfo((std::istreambuf_iterator<char>(ifsBitmapFramesPath)), std::istreambuf_iterator<char>());
				ifsBitmapFramesPath.close();
				nlohmann::ordered_json jsonBitmapFrameInfo = nlohmann::ordered_json::parse(strBitmapInfo);
				int a = 0;
				for (auto jsonBitmapFrame : jsonBitmapFrameInfo["Frames"])
				{
					D2D1_RECT_F rectSize;
					rectSize.left = jsonBitmapFrame["Rect_left"];
					rectSize.top = jsonBitmapFrame["Rect_top"];
					rectSize.right = jsonBitmapFrame["Rect_right"];
					rectSize.bottom = jsonBitmapFrame["Rect_bottom"];
					strBitmapKey = strBitmapKey + std::to_string(a);
					const WCHAR* newWCHARAnimationKey = reinterpret_cast<const WCHAR*>(strBitmapKey.c_str());

					///renderManager->CreateD2DBitmapFromFile(WCHARAnimationKey, WCHARAnimePath,rectSize);
					a++;



				}
			}
		}

	}
}
