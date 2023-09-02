#include "Loader.h"

#include "RenderManger.h"
#include "BeatManager.h"

#include "FrameInformation.h"
#include "ConstantTable.h"
#include "SoundManager.h"

#include <cassert>

namespace soulBeater
{
	void Loader::LoadD2DBitmap()
	{
		using namespace d2dFramework;
		HRESULT hr;

		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(STAGE_SPRITE_KEY, STAGE_SPRITE_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(PLAYER_NODE_SPRITE_KEY, PLAYER_NODE_SPRITE_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(CUTSCENE_BEALZEBUB_SPRITE_KEY, CUTSCENE_BEALZEBUB_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(CUTSCENE_EVE_ESS_SPRITE_KEY, CUTSCENE_EVE_ESS_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(CUTSCENE_LILITH_SPRITE_KEY, CUTSCENE_LILITH_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(GAMEOVER_SPRITE_KEY, GAMEOVER_SPRITE_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(ENDING_SPRITE_KEY, ENDING_SPRITE_PATH)); assert(SUCCEEDED(hr));
		hr = (RenderManager::GetInstance()->CreateD2DBitmapFromFile(HIT_EFFECT_SPRITE_KEY, HIT_EFFECT_SPRITE_PATH)); assert(SUCCEEDED(hr));
	}

	void Loader::LoadAnimationAsset(d2dFramework::BeatManager* beatmanager)
	{
		using namespace d2dFramework;

		std::vector<std::vector<FrameInformation>> belzebubFrames =
		{
			{ // idle
				{ 0,0,944,1069,0.1f},
				{ 944,0,1888,1069,0.1f },
				{ 1888,0,2832,1069,0.1f },
				{ 2832,0,3776,1069,0.1f },
				{ 3776,0,4720,1069,0.1f },
			},
			{ // move
				{ 0,1069,944,2138,0.1f },
				{ 944,1069,1888,2138,0.1f },
				{ 1888,1069,2832,2138,0.1f },
			},
			{ // attack
				{ 0,2138,944,3207,0.1f },
				{ 944,2138,1888,3207,0.1f },
				{ 1888,2138,2832,3207,0.1f },
				{ 2832,2138,3776,3207,0.1f },
				{ 3776,2138,4720,3207,0.1f },
			},
			{ // hit
				{ 0,3207,944,4276,0.1f },
				{ 944,3207,1888,4276,0.1f },
				{ 1888,3207,2832,4276,0.1f },
				{ 2832,3207,3776,4276,0.1f },
				{ 3776,3207,4720,4276,0.1f },
			},
			{ // death
				{ 0,4276,944,5345,0.1f },
				{ 944,4276,1888,5345,0.1f },
				{ 1888,4276,2832,5345,0.1f },
				{ 2832,4276,3776,5345,0.1f },
				{ 3776,4276,4720,5345,0.1f },
				{ 0,5345,944,6414,0.1f },
				{ 944,5345,1888,6414,0.1f },
				{ 1888,5345,2832,6414,0.1f },
				{ 2832,5345,3776,6414,0.1f },
				{ 3776,5345,4720,6414,0.1f },
				{ 0,6414,944,7483,0.1f },
				{ 944,6414,1888,7483,0.1f },
				{ 1888,6414,2832,7483,0.1f },
				{ 2832,6414,3776,7483,0.1f },
				{ 3776,6414,4720,7483,0.1f },
				{ 0,7483,944,8552,0.1f },
				{ 944,7483,1888,8552,0.1f },
				{ 1888,7483,2832,8552,0.1f },
			}
		};
		HRESULT hr;

		hr = RenderManager::GetInstance()->CreateAnimationAsset(BEALZEBUB_ANI_KEY, BEALZEBUB_ANI_PATH, belzebubFrames); assert(SUCCEEDED(hr));

		std::vector<std::vector<FrameInformation>> fly =
		{
			{//Idle
				{0, 7483, 944, 8552,0.2f		 } ,
				{944, 7483, 1888, 8552,0.2f	 } ,
				{1888, 7483, 2832, 8552,0.2f	 } ,
				{2832, 7483, 3776, 8552,0.2f	 }
			},
			{//Idle
				{0, 7483, 944, 8552,0.2f		 } ,
				{944, 7483, 1888, 8552,0.2f	 } ,
				{1888, 7483, 2832, 8552,0.2f	 } ,
				{2832, 7483, 3776, 8552,0.2f	 }
			},
			{//Idle
				{0, 7483, 944, 8552,0.2f		 } ,
				{944, 7483, 1888, 8552,0.2f	 } ,
				{1888, 7483, 2832, 8552,0.2f	 } ,
				{2832, 7483, 3776, 8552,0.2f	 }
			},
			{//Idle
				{0, 7483, 944, 8552,0.2f		 } ,
				{944, 7483, 1888, 8552,0.2f	 } ,
				{1888, 7483, 2832, 8552,0.2f	 } ,
				{2832, 7483, 3776, 8552,0.2f	 }

			}
		};
		hr=RenderManager::GetInstance()->CreateAnimationAsset(FLY_ANI_KEY, FLY_ANI_PATH, fly); assert(SUCCEEDED(hr));
		hr = RenderManager::GetInstance()->CreateAnimationAsset(FLY_ANI_KEY, FLY_ANI_PATH, fly); assert(SUCCEEDED(hr));

		//RenderManager::GetInstance()->CreateAnimationAsset(FLY_ANI_KEY, FLY_ANI_PATH, fly);
		std::vector<std::vector<FrameInformation>> succubus =
		{
			{ // Idle
				{0, 3207, 944, 4276, 0.2f		 },
				{944, 3207, 1888, 4276, 0.2f	 },
				{1888, 3207, 2832, 4276, 0.2f	 },
				{2832, 3207, 3776, 4276, 0.2f	 },
			},
			{ // Idle
				{0, 3207, 944, 4276, 0.2f		 },
				{944, 3207, 1888, 4276, 0.2f	 },
				{1888, 3207, 2832, 4276, 0.2f	 },
				{2832, 3207, 3776, 4276, 0.2f	 },
			},
			{ // Idle
				{0, 3207, 944, 4276, 0.2f		 },
				{944, 3207, 1888, 4276, 0.2f	 },
				{1888, 3207, 2832, 4276, 0.2f	 },
				{2832, 3207, 3776, 4276, 0.2f	 },
			},
			{//deathAttack
				{0, 4276, 944, 5345, 0.1f		 },
				{944, 4276, 1888, 5345, 0.1f	 },
				{1888, 4276, 2832, 5345, 0.1f	 },
				{2832, 4276, 3776, 5345, 0.1f	 },
				{3776, 4276, 4720, 5345, 0.1f	 },
				{4720, 4276, 5664, 5345, 0.1f	 },
				{5664, 4276, 6608, 5345, 0.1f	 },
				{6608, 4276, 7552, 5345, 0.1f	 },
				{7552, 4276, 8496, 5345, 0.1f	 },
				{8496, 4276, 9440, 5345, 0.1f	 },
			}
		};
		hr = RenderManager::GetInstance()->CreateAnimationAsset(SUCCUBUS_ANI_KEY, SUCCUBUS_ANI_PATH, succubus); assert(SUCCEEDED(hr));

		std::vector<std::vector<FrameInformation>> Knight =
		{
			{ // Idle
				{0, 5345, 944, 6414, 0.2f		 },
				{944, 5345, 1888, 6414, 0.2f	 },
				{1888, 5345, 2832, 6414, 0.2f	 },
				{2832, 5345, 3776, 6414, 0.2f	 },
			},
			{ // Move


				{0, 8552, 900, 9621},
				{900, 8552, 1800, 9621,0.2f },
				{1800, 8552, 2700, 9621,0.2f},
				{2700, 8552, 3600, 9621,0.2f}


			},
			{ // None
				{0, 8552, 900, 9621,0.2f},
				{900, 8552, 1800, 9621 ,0.2f},
				{1800, 8552, 2700, 9621,0.2f},
				{2700, 8552, 3600, 9621,0.2f}
			},
			{ //deathAttack
				{0, 6414, 944, 7483, 0.1f		 },
				{944, 6414, 1888, 7483, 0.1f	 } ,
				{1888, 6414, 2832, 7483, 0.1f	 } ,
				{2832, 6414, 3776, 7483, 0.1f	 } ,
				{3776, 6414, 4720, 7483, 0.1f	 } ,
				{4720, 6414, 5664, 7483, 0.1f	 } ,
				{5664, 6414, 6608, 7483, 0.1f	 } ,
				{6608, 6414, 7552, 7483, 0.1f	 } ,
				{7552, 6414, 8496, 7483, 0.1f	 } ,
				{8496, 6414, 9440, 7483, 0.1f	 } ,
				{9440, 6414, 10384, 7483, 0.1f },
				{10384, 6414, 11328, 7483, 0.1f},
				{11328, 6414, 12272, 7483, 0.1f },
			}
		};
		hr = RenderManager::GetInstance()->CreateAnimationAsset(KNIGHT_ANI_KEY, KNIGHT_ANI_PATH, Knight); assert(SUCCEEDED(hr));

		std::vector<std::vector<FrameInformation>> Ghost =
		{
			{//Idle
				{0, 0, 944, 1069, 0.2f },
			   {944, 0, 1888, 1069, 0.2f },
			   {1888, 0, 2832, 1069, 0.2f },
			   {2832, 0, 3776, 1069, 0.2f },
			},
			{//death
				{0, 1069, 944, 2138, 0.2f },
			   {944, 1069, 1888, 2138, 0.2f },
			   {1888, 1069, 2832, 2138, 0.2f },
			   {2832, 1069, 3776, 2138, 0.2f },
			},
			{//death
				{0, 1069, 944, 2138, 0.2f },
			   {944, 1069, 1888, 2138, 0.2f },
			   {1888, 1069, 2832, 2138, 0.2f },
			   {2832, 1069, 3776, 2138, 0.2f },
			   {3776, 1069, 4720, 2138, 0.2f },
			},
			{//deathAttack
				{0, 2138, 944, 3207, 0.2f	 },
			   {944, 2138, 1888, 3207, 0.2f },
			   {1888, 2138, 2832, 3207, 0.2f},
			   {2832, 2138, 3776, 3207, 0.2f},
			   {3776, 2138, 4720, 3207, 0.2f},
			   {4720, 2138, 5664, 3207, 0.2f},
			   {5664, 2138, 6608, 3207, 0.2f},
			   {6608, 2138, 7552, 3207, 0.2f},
			   {7552, 2138, 8496, 2850, 0.2f},
			}
		};
		hr = RenderManager::GetInstance()->CreateAnimationAsset(GHOST_ANI_KEY, GHOST_ANI_PATH, Ghost); assert(SUCCEEDED(hr));

		// ������ �ִϸ��̼�
		{
			std::vector<std::vector<FrameInformation>> lilithAnimationAsset;
			std::vector<FrameInformation> LilithAnmation;
			int animationChange = 0;
			int animationMoveCount = 0;
			int animationIdleCount = 0;
			int animationAttackCount = 0;
			int animationBeAttackedCount = 0;
			int animationDeathCount = 0;

			for (int i = 0; i < 8; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 8552.f / 8 * i;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5.f + animationwidth, 8552.f / 8.f + animationheight };

					switch (animationChange)
					{
					case 0:
					{
						animationMoveCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 1.f;
						LilithAnmation.push_back(animationinfo);

						if (animationMoveCount >= 1)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 1:
					{
						animationIdleCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 4.f;
						LilithAnmation.push_back(animationinfo);

						if (animationIdleCount >= 4)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 2:
					{
						animationAttackCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 5.f;
						LilithAnmation.push_back(animationinfo);

						if (animationAttackCount >= 5)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 3:
					{
						animationBeAttackedCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 5.f;
						LilithAnmation.push_back(animationinfo);

						if (animationBeAttackedCount >= 6)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 4:
					{
						animationDeathCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 12.f;
						LilithAnmation.push_back(animationinfo);

						if (animationDeathCount >= 12)
						{
							lilithAnimationAsset.push_back(LilithAnmation);
							LilithAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					}
				}
			}
			hr = RenderManager::GetInstance()->CreateAnimationAsset(LILITH_ANI_KEY, LILITH_ANI_PATH, lilithAnimationAsset); assert(SUCCEEDED(hr));
		}


		// �̺� �ִϸ��̼�
		{
			std::vector<std::vector<FrameInformation>> eveAnimationAsset;
			std::vector<FrameInformation> eveAnmation;
			int animationChange = 0;
			int animationMoveCount = 0;
			int animationIdleCount = 0;
			int animationAttackCount = 0;
			int animationBeAttackedCount = 0;
			int animationDeathCount = 0;

			for (int i = 0; i < 11; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 11759.f / 11 * i;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5.f + animationwidth, 11759.f / 11 + animationheight };

					switch (animationChange)
					{
					case 0:
					{
						animationMoveCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 1.f;
						eveAnmation.push_back(animationinfo);

						if (animationMoveCount >= 1)
						{
							eveAnimationAsset.push_back(eveAnmation);
							eveAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 1:
					{
						animationIdleCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 4.f;
						eveAnmation.push_back(animationinfo);

						if (animationIdleCount >= 4)
						{
							eveAnimationAsset.push_back(eveAnmation);
							eveAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 2:
					{
						animationAttackCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 10.f;
						eveAnmation.push_back(animationinfo);

						if (animationAttackCount >= 10)
						{
							eveAnimationAsset.push_back(eveAnmation);
							eveAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 3:
					{
						animationBeAttackedCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 9.f;
						eveAnmation.push_back(animationinfo);

						if (animationBeAttackedCount >= 9)
						{
							eveAnimationAsset.push_back(eveAnmation);
							eveAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 4:
					{
						animationDeathCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 22.f;
						eveAnmation.push_back(animationinfo);

						if (animationDeathCount >= 22)
						{
							eveAnimationAsset.push_back(eveAnmation);
							eveAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					}
				}
			}
			hr = RenderManager::GetInstance()->CreateAnimationAsset(EVE_ANI_KEY, EVE_ANI_PATH, eveAnimationAsset); assert(SUCCEEDED(hr));
		}


		// �̽� �ִϸ��̼�
		{
			std::vector<std::vector<FrameInformation>> ysAnimationAsset;
			std::vector<FrameInformation> ysAnmation;
			int animationChange = 0;
			int animationMoveCount = 0;
			int animationIdleCount = 0;
			int animationAttackCount = 0;
			int animationBeAttackedCount = 0;
			int animationDeathCount = 0;

			for (int i = 0; i < 12; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 12828.f / 12 * i;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5.f + animationwidth, 12828.f / 12 + animationheight };

					switch (animationChange)
					{
					case 0:
					{
						animationMoveCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 1.f;
						ysAnmation.push_back(animationinfo);

						if (animationMoveCount >= 1)
						{
							ysAnimationAsset.push_back(ysAnmation);
							ysAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 1:
					{
						animationIdleCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 4.f;
						ysAnmation.push_back(animationinfo);

						if (animationIdleCount >= 4)
						{
							ysAnimationAsset.push_back(ysAnmation);
							ysAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 2:
					{
						animationAttackCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 12.f;
						ysAnmation.push_back(animationinfo);

						if (animationAttackCount >= 12)
						{
							ysAnimationAsset.push_back(ysAnmation);
							ysAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 3:
					{
						animationBeAttackedCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 6.f;
						ysAnmation.push_back(animationinfo);

						if (animationBeAttackedCount >= 6)
						{
							ysAnimationAsset.push_back(ysAnmation);
							ysAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 4:
					{
						animationDeathCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 25.f;
						ysAnmation.push_back(animationinfo);

						if (animationDeathCount >= 25)
						{
							ysAnimationAsset.push_back(ysAnmation);
							ysAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					}
				}
			}
			hr = RenderManager::GetInstance()->CreateAnimationAsset(YS_ANI_KEY, YS_ANI_PATH, ysAnimationAsset);
			assert(SUCCEEDED(hr));
		}


		// Player �ִϸ��̼�
		{
			std::vector<std::vector<FrameInformation>> playerAnimationAsset;
			std::vector<FrameInformation> playerAnmation;
			int animationChange = 0;
			int animationMoveCount = 0;
			int animationIdleCount = 0;
			int animationAttackCount = 0;
			int animationBeAttackedCount = 0;
			int animationDeathCount = 0;

			for (int i = 0; i < 9; i++)
			{
				FrameInformation animationinfo;

				for (int j = 0; j < 5; j++)
				{
					float animationwidth = 4720.f / 5 * j;
					float animationheight = 9621.f / 9 * i;
					animationinfo.UVRectangle = { 0.f + animationwidth, 0.f + animationheight, 4720.f / 5.f + animationwidth, 9621.f / 9 + animationheight };

					switch (animationChange)
					{
					case 0:
					{
						animationMoveCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 1.f;
						playerAnmation.push_back(animationinfo);

						if (animationMoveCount >= 1)
						{
							playerAnimationAsset.push_back(playerAnmation);
							playerAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 1:
					{
						animationIdleCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 4.f;
						playerAnmation.push_back(animationinfo);

						if (animationIdleCount >= 4)
						{
							playerAnimationAsset.push_back(playerAnmation);
							playerAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 2:
					{
						animationAttackCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 10.f;
						playerAnmation.push_back(animationinfo);

						if (animationAttackCount >= 6)
						{
							playerAnimationAsset.push_back(playerAnmation);
							playerAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 3:
					{
						animationBeAttackedCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 5.f;
						playerAnmation.push_back(animationinfo);

						if (animationBeAttackedCount >= 9)
						{
							playerAnimationAsset.push_back(playerAnmation);
							playerAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					case 4:
					{
						animationDeathCount++;

						animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 12.f;
						playerAnmation.push_back(animationinfo);

						if (animationDeathCount >= 12)
						{
							playerAnimationAsset.push_back(playerAnmation);
							playerAnmation.clear();

							animationChange++;
							j = 5;
						}
					}
					break;
					}
				}
			}
			hr = RenderManager::GetInstance()->CreateAnimationAsset(PLAYER_ANI_KEY, PLAYER_ANI_PATH, playerAnimationAsset);
			assert(SUCCEEDED(hr));
		}

		// ��� �ִϸ��̼�
		{
			std::vector<std::vector<FrameInformation>> warningAnimationAsset;
			std::vector<FrameInformation> warningAnmation;
			for (int i = 0; i < 4; i++)
			{
				FrameInformation animationinfo;
				float animationwidth = 7680.f / 4 * i;
				animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 4.f;
				animationinfo.UVRectangle = { 0.f + animationwidth, 0.f, 7680.f / 4 + animationwidth, 1080.f };

				warningAnmation.push_back(animationinfo);
			}
			warningAnimationAsset.push_back(warningAnmation);
			hr = RenderManager::GetInstance()->CreateAnimationAsset(WARNING_ANI_KEY, WARNING_ANI_PATH, warningAnimationAsset);
			assert(SUCCEEDED(hr));
		}

		// ���� �ִϸ��̼�
		{
			std::vector<std::vector<FrameInformation>> warningAnimationAsset;
			std::vector<FrameInformation> warningAnmation;
			for (int i = 0; i < 10; i++)
			{
				FrameInformation animationinfo;
				float animationwidth = 600.f * i;
				animationinfo.AnimationTime = static_cast<float>(beatmanager->GetIntervalTime()) / 15.f;
				animationinfo.UVRectangle = { 0.f + animationwidth, 0.f, 600.f + animationwidth, 600.f };

				warningAnmation.push_back(animationinfo);
			}
			warningAnimationAsset.push_back(warningAnmation);
			hr = RenderManager::GetInstance()->CreateAnimationAsset(BOOM_ANI_KEY, BOOM_ANI_PATH, warningAnimationAsset); 
			assert(SUCCEEDED(hr));
		}
	}

	void Loader::LoadSoundAsset()
	{
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_TITLE_KEY, BGM_TITLE_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE1_1_KEY, BGM_STAGE1_1_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE1_2_KEY, BGM_STAGE1_2_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE1_3_KEY, BGM_STAGE1_3_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_GAMEOVER_KEY, BGM_GAMEOVER_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE2_1_KEY, BGM_STAGE2_1_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE2_2_KEY, BGM_STAGE2_2_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE2_3_KEY, BGM_STAGE2_3_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE3_1_KEY, BGM_STAGE3_1_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE3_2_KEY, BGM_STAGE3_2_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE3_3_KEY, BGM_STAGE3_3_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE1_BOSS_KEY, BGM_STAGE1_BOSS_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE2_BOSS_KEY, BGM_STAGE2_BOSS_PATH, true);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(BGM_STAGE3_BOSS_KEY, BGM_STAGE3_BOSS_PATH, true);

		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_PLAYERHIT_KEY, EFFECT_PLAYERHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_PLAYERMOVE_KEY, EFFECT_PLAYERMOVE_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_BEELZEBUBHIT_KEY, EFFECT_BEELZEBUBHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_PLAYERATTACK_KEY, EFFECT_PLAYERATTACK_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_EVEHIT_KEY, EFFECT_EVEHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_YSHIT_KEY, EFFECT_YSHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_KNIGHTHIT_KEY, EFFECT_KNIGHTHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_GHOSTHIT_KEY, EFFECT_GHOSTHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_SUCCUBUSHIT_KEY, EFFECT_SUCCUBUSHIT_PATH, false);
		d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_LILITHHIT_KEY, EFFECT_LILITHHIT_PATH, false);
		//d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_SOUND11_KEY, EFFECT_SOUND11_PATH, false);
		//d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_SOUND12_KEY, EFFECT_SOUND12_PATH, false);
		//d2dFramework::SoundManager::GetInstance()->CreateSoundAsset(EFFECT_SOUND13_KEY, EFFECT_SOUND13_PATH, false);
	}

}
