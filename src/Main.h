#pragma once
#include <cocos2d.h>
#include "gd.h"
#include "MinHook.h"
#include "json/json.h"

using namespace cocos2d;

namespace CreatorLayer {
	inline bool(__thiscall* init)(gd::CreatorLayer* init);
	bool __fastcall initHook(gd::CreatorLayer* init);
}

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* GJGameLevel);
	bool __fastcall initHook(gd::PlayLayer* self, int edx, gd::GJGameLevel* GJGameLevel);

	inline void(__thiscall* update)(gd::PlayLayer* self, float deltatime);
	void __fastcall updateHook(gd::PlayLayer* self, int edx, float deltatime);

	inline void(__thiscall* levelComplete)(gd::PlayLayer* self);
	void __fastcall levelCompleteHook(gd::PlayLayer* self);

	inline void(__thiscall* resetLevel)(gd::PlayLayer* self);
	void __fastcall resetLevelHook(gd::PlayLayer* self);

	inline void(__thiscall* onQuit)(gd::PlayLayer* self);
	void __fastcall onQuitHook(gd::PlayLayer* self);
}

namespace InfiniteLayer {
	class Callbacks {
	public:
		void initBtn(CCObject*);
		void play(CCObject*);
		void recieveLevel(extension::CCHttpClient* client, extension::CCHttpResponse* response);
	};

	void init();
}

void runMod();