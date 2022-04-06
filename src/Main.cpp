#include "Main.h"

int points = 0;
int lives = 5;
bool firstStart = true;
CCMenu* infinitelayer = nullptr;

void InfiniteLayer::Callbacks::recieveLevel(extension::CCHttpClient* client, extension::CCHttpResponse* response) {
	// Parse Return Data
	auto resp = response->getResponseData();
	const char* data = resp->data();
	Json::Value root;
	Json::Reader reader;
	reader.parse(data, root);
	srand((int)time(0));
	int randLvl = (rand() % root.size()) + 1;

	// Get values
	std::string levelName = root[randLvl]["name"].asString();
	std::string creatorName = root[randLvl]["author"].asString();
	int songID = 0;
	std::stringstream sngid(root[randLvl]["songID"].asString());
	sngid >> songID;
	int levelID = 0;
	std::stringstream lvlid(root[randLvl]["id"].asString());
	lvlid >> levelID;
	auto level = gd::GJGameLevel::create();
	level->m_nLevelID_rand = levelID + 5;
	level->m_nLevelID_seed = 5;
	level->m_nLevelID = levelID;
	level->m_sLevelName = levelName;
	level->m_sCreatorName = creatorName;
	level->m_nSongID = songID;
	level->m_bLevelNotDownloaded = true;
	level->m_eLevelType = gd::GJLevelType::kGJLevelTypeSaved;

	// Create Layer
	auto layer = gd::LevelInfoLayer::create(level);
	layer->downloadLevel();
	auto scene = CCScene::create();
	scene->addChild(layer);
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
	infinitelayer->removeFromParent();
}

void InfiniteLayer::Callbacks::play(CCObject* pSender) {
	extension::CCHttpRequest* req = new extension::CCHttpRequest();

	srand((int)time(0));
	std::string page = std::to_string((rand() % 30) + 1);

	req->setUrl(("https://gdbrowser.com/api/search/*?diff=1&page="+page).c_str());
	if (points >= 5) req->setUrl(("https://gdbrowser.com/api/search/*?diff=2&page=" + page).c_str());
	if (points >= 10) req->setUrl(("https://gdbrowser.com/api/search/*?diff=3&page=" + page).c_str());
	if (points >= 15) req->setUrl(("https://gdbrowser.com/api/search/*?diff=4&page=" + page).c_str());
	if (points >= 20) req->setUrl(("https://gdbrowser.com/api/search/*?diff=5&page=" + page).c_str());
	if (points >= 25) req->setUrl(("https://gdbrowser.com/api/search/*?diff=-2&demonFilter=1&page=" + page).c_str());
	if (points >= 30) req->setUrl(("https://gdbrowser.com/api/search/*?diff=-2&demonFilter=2&page=" + page).c_str());
	if (points >= 35) req->setUrl(("https://gdbrowser.com/api/search/*?diff=-2&demonFilter=3&page=" + page).c_str());
	if (points >= 40) req->setUrl(("https://gdbrowser.com/api/search/*?diff=-2&demonFilter=4&page=" + page).c_str());
	if (points >= 45) req->setUrl(("https://gdbrowser.com/api/search/*?diff=-2&demonFilter=5&page=" + page).c_str());

	std::cout << req->getUrl() << std::endl;

	req->setRequestType(extension::CCHttpRequest::kHttpGet);
	req->setResponseCallback(pSender, httpresponse_selector(InfiniteLayer::Callbacks::recieveLevel));
	extension::CCHttpClient::getInstance()->send(req);
	req->release();

	firstStart = true;
}

void InfiniteLayer::Callbacks::initBtn(CCObject*) {
	InfiniteLayer::init();
}

void InfiniteLayer::init() {
	auto scene = CCDirector::sharedDirector()->getRunningScene();
	auto layer = (CCLayer*)scene->getChildren()->objectAtIndex(0);
	infinitelayer = CCMenu::create();
	infinitelayer->setPosition({ 0, 0 });
	auto director = CCDirector::sharedDirector();
	infinitelayer->setTag(69);
	infinitelayer->setZOrder(100);

	// Disable all buttons
	auto btnMenu = (CCMenu*)layer->getChildren()->lastObject();
	btnMenu->setEnabled(false);

	auto fade = CCSprite::create("square02_001.png");
	fade->setPosition({ director->getWinSize().width / 2, director->getWinSize().height / 2 });
	fade->setScale(10);
	fade->setOpacity(150);

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setPosition({ director->getWinSize().width / 2, director->getWinSize().height / 2 });
	bg->setContentSize({ 450, 250 });

	auto title = CCLabelBMFont::create("Infinite Mode", "bigFont.fnt");
	title->setScale(0.8f);
	title->setPosition({ director->getWinSize().width / 2, (director->getWinSize().height / 2) + 100 });

	auto underline = CCSprite::createWithSpriteFrameName("floorLine_001.png");
	underline->setPosition({ director->getWinSize().width / 2, (director->getWinSize().height / 2) + 70 });
	underline->setOpacity(100);

	auto sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
	auto playBtn = gd::CCMenuItemSpriteExtra::create(sprite, infinitelayer, menu_selector(InfiniteLayer::Callbacks::play));
	playBtn->setPosition({ director->getWinSize().width / 2, director->getWinSize().height / 2 });

	auto pointsText = CCLabelBMFont::create(("Points: " + std::to_string(points)).c_str(), "goldFont.fnt");
	pointsText->setPosition({ director->getWinSize().width / 2, (director->getWinSize().height / 2) - 80 });

	auto livesText = CCLabelBMFont::create(("Lives: " + std::to_string(lives)).c_str(), "goldFont.fnt");
	livesText->setPosition({ director->getWinSize().width / 2, (director->getWinSize().height / 2) - 100 });

	CCSprite* diffFace;
	diffFace = CCSprite::createWithSpriteFrameName("difficulty_01_btn_001.png"); // Easy
	if (points >= 5) diffFace = CCSprite::createWithSpriteFrameName("difficulty_02_btn_001.png"); // Normal
	if (points >= 10) diffFace = CCSprite::createWithSpriteFrameName("difficulty_03_btn_001.png"); // Hard
	if (points >= 15) diffFace = CCSprite::createWithSpriteFrameName("difficulty_04_btn_001.png"); // Harder
	if (points >= 20) diffFace = CCSprite::createWithSpriteFrameName("difficulty_05_btn_001.png"); // Insane
	if (points >= 25) diffFace = CCSprite::createWithSpriteFrameName("difficulty_07_btn2_001.png"); // Easy Demon
	if (points >= 30) diffFace = CCSprite::createWithSpriteFrameName("difficulty_08_btn2_001.png"); // Medium Demon
	if (points >= 35) diffFace = CCSprite::createWithSpriteFrameName("difficulty_06_btn2_001.png"); // Hard Demon
	if (points >= 40) diffFace = CCSprite::createWithSpriteFrameName("difficulty_09_btn2_001.png"); // Insane Demon
	if (points >= 45) diffFace = CCSprite::createWithSpriteFrameName("difficulty_10_btn2_001.png"); // Extreme Demon
	diffFace->setPosition({ (director->getWinSize().width / 2) - 125, director->getWinSize().height / 2 });
	diffFace->setScale(1.25f);

	pointsText->setTag(100);
	livesText->setTag(101);

	infinitelayer->addChild(fade);
	infinitelayer->addChild(bg);
	infinitelayer->addChild(title);
	infinitelayer->addChild(underline);
	infinitelayer->addChild(playBtn);
	infinitelayer->addChild(pointsText);
	infinitelayer->addChild(livesText);
	infinitelayer->addChild(diffFace);
	layer->addChild(infinitelayer);
}

bool __fastcall CreatorLayer::initHook(gd::CreatorLayer* self) {
	bool ret = CreatorLayer::init(self);
	if (!ret) return ret;

	auto menu = CCMenu::create();
	menu->setPosition({ 0, 0 });
	CCSprite* sprite = CCSprite::createWithSpriteFrameName("GJ_shardsBtn_001.png");
	sprite->setScale(1.3f);
	gd::CCMenuItemSpriteExtra* btn = gd::CCMenuItemSpriteExtra::create(sprite, menu, menu_selector(InfiniteLayer::Callbacks::initBtn));
	btn->setPosition({ 5 + (btn->getScaledContentSize().width / 2), CCDirector::sharedDirector()->getWinSize().height / 2 });
	menu->addChild(btn);

	self->addChild(menu);

	return ret;
}

bool __fastcall PlayLayer::initHook(gd::PlayLayer* self, int, gd::GJGameLevel* level) {
	bool ret = PlayLayer::init(self, level);
	if (!ret) return ret;

	auto director = CCDirector::sharedDirector();
	auto livesText = CCLabelBMFont::create(("Lives: " + std::to_string(lives)).c_str(), "goldFont.fnt");
	livesText->setScale(0.75f);
	livesText->setPosition({ director->getWinSize().width / 2, director->getWinSize().height - 30 });
	livesText->setOpacity(150);
	auto menu = CCMenu::create();
	menu->setPosition({ 0, 0 });
	menu->setTag(100);
	livesText->setTag(101);
	menu->setZOrder(1000);
	menu->addChild(livesText);
	self->addChild(menu);

	return ret;
}

void __fastcall PlayLayer::updateHook(gd::PlayLayer* self, int, float dt) {
	PlayLayer:update(self, dt);
	auto menu = (CCMenu*)self->getChildByTag(100);
	auto livesText = (CCLabelBMFont*)menu->getChildByTag(101);
	livesText->setString(("Lives: " + std::to_string(lives)).c_str());
}

void __fastcall PlayLayer::levelCompleteHook(gd::PlayLayer* self) {
	PlayLayer::levelComplete(self);
	points++;
	lives++;
}

void __fastcall PlayLayer::resetLevelHook(gd::PlayLayer* self) {
	PlayLayer::resetLevel(self);
	if (!firstStart) {
		lives--;
	}
	if (lives == 0) {
		points = 0;
		lives = 5;
		PlayLayer::onQuit(self);
	}
	firstStart = false;
}

void __fastcall PlayLayer::onQuitHook(gd::PlayLayer* self) {
	PlayLayer::onQuit(self);
}

void runMod() {
	auto base = gd::base;

	MH_CreateHook(
		(PVOID)(base + 0x01FB780),
		PlayLayer::initHook,
		(LPVOID*)&PlayLayer::init
	);

	MH_CreateHook(
		(PVOID)(base + 0x2029C0),
		PlayLayer::updateHook,
		(LPVOID*)&PlayLayer::update
	);

	MH_CreateHook(
		(PVOID)(base + 0x4DE40),
		CreatorLayer::initHook,
		(LPVOID*)&CreatorLayer::init
	);

	MH_CreateHook(
		(PVOID)(base + 0x1FD3D0),
		PlayLayer::levelCompleteHook,
		(LPVOID*)&PlayLayer::levelComplete
	);

	MH_CreateHook(
		(PVOID)(base + 0x20BF00),
		PlayLayer::resetLevelHook,
		(LPVOID*)&PlayLayer::resetLevel
	);

	MH_CreateHook(
		(PVOID)(base + 0x20D810),
		PlayLayer::onQuitHook,
		(LPVOID*)&PlayLayer::onQuit
	);
}