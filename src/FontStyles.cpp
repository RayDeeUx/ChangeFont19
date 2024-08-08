/*
This file is part of ChangeFont19.

ChangeFont19 is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

ChangeFont19 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ChangeFont19. If not, see <https://www.gnu.org/licenses/>.
*/

/*
Source code based on and adapted from Alpahalaneous' code
for the "Pusab Font Fix" feature in Happy Textures.
Reused with permission granted by GPLv3.

Bindings found by https://github.com/hiimjustin000,
and verified from building HappyTextures manually
and enabling "Pusab Fix" in-game.
*/

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCLabelBMFont.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCSpriteBatchNode.hpp>
#include <Geode/modify/CCTextureCache.hpp>

using namespace geode::prelude;

bool isInCreateTextLayers = false;
bool hasCalledAlready = false;

std::map<std::string, std::string> settingToPrefix = {
	{"N/A (Classic)", ""},
	{"Stronger Stroke", "stroke"},
	{"Shadow", "shadow"},
	{"Monospace", "mono"},
	{"Monospace Shadow", "monoShadow"}
};

std::string prefix = "";

$on_mod(Loaded) {
	log::info("setting: {}", Mod::get()->getSettingValue<std::string>("fontStyle"));
	log::info("prefix: {}", settingToPrefix.find(Mod::get()->getSettingValue<std::string>("fontStyle"))->second);
	prefix = settingToPrefix.find(Mod::get()->getSettingValue<std::string>("fontStyle"))->second;
	auto directoryVector = std::vector<std::string>{ Mod::get()->getResourcesDir().string() };
	log::info("MAKING TEXTURE PACK USING DIRECTORY: {}", Mod::get()->getResourcesDir().string());
	auto texturePack = CCTexturePack {
		.m_id = Mod::get()->getID(), // they're the same ID so it doesnt matter
		.m_paths = directoryVector
	};
	CCFileUtils::get()->addTexturePack(texturePack);
}

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) { return false; }
		if (hasCalledAlready || !prefix.empty()) { return true; }
		hasCalledAlready = true;
		prefix = settingToPrefix.find(Mod::get()->getSettingValue<std::string>("fontStyle"))->second;
		return true;
	}
};

class $modify(GJBaseGameLayer) {
	void createTextLayers() {
		isInCreateTextLayers = true;
		GJBaseGameLayer::createTextLayers();
		isInCreateTextLayers = false;
	}
};

class $modify(CCSpriteBatchNode) {
	bool initWithTexture(CCTexture2D* texture, unsigned int capacity) {
		if (isInCreateTextLayers && texture == CCTextureCache::sharedTextureCache()->addImage("gjFont18.png", false)) {
			const char* toReplace = "gjFont18.png"_spr;
			if (prefix == "stroke") { toReplace = "strokegjFont18.png"_spr; }
			else if (prefix == "shadow") { toReplace = "shadowgjFont18.png"_spr; }
			else if (prefix == "mono") { toReplace = "monogjFont18.png"_spr; }
			else if (prefix == "monoShadow") { toReplace = "monoShadowgjFont18.png"_spr; }
			return CCSpriteBatchNode::initWithTexture(CCTextureCache::sharedTextureCache()->addImage(toReplace, false), capacity);
		}
		return CCSpriteBatchNode::initWithTexture(texture, capacity);
	}
};

class $modify(CCLabelBMFont) {
	static CCLabelBMFont* createBatched(const char* str, const char* fntFile, CCArray* a, int a1) {
		if (strcmp(fntFile, "gjFont18.fnt") == 0) {
			const char* toReplace = "gjFont18.fnt"_spr;
			if (prefix == "stroke") { toReplace = "strokegjFont18.fnt"_spr; }
			else if (prefix == "shadow") { toReplace = "shadowgjFont18.fnt"_spr; }
			else if (prefix == "mono") { toReplace = "monogjFont18.fnt"_spr; }
			else if (prefix == "monoShadow") { toReplace = "monoShadowgjFont18.fnt"_spr; }
			fntFile = toReplace;
		}
		return CCLabelBMFont::createBatched(str, fntFile, a, a1);
	}
};

class $modify(CCTextureCache) {
	CCTexture2D* addImage(const char* fileimage, bool p1) {
		CCTexture2D* ret = nullptr;
		bool didChange = false;
		if (strcmp(fileimage, "gjFont18.png") == 0) {
			if (PlayLayer::get() || LevelEditorLayer::get()) {
				didChange = true;
				const char* toReplace = "gjFont18.png"_spr;
				if (prefix == "stroke") { toReplace = "strokegjFont18.png"_spr; }
				else if (prefix == "shadow") { toReplace = "shadowgjFont18.png"_spr; }
				else if (prefix == "mono") { toReplace = "monogjFont18.png"_spr; }
				else if (prefix == "monoShadow") { toReplace = "monoShadowgjFont18.png"_spr; }
				ret = CCTextureCache::addImage(toReplace, p1);
			}
		}
		if (!didChange) {
			ret = CCTextureCache::addImage(fileimage, p1);
		}
		return ret;
	}
};