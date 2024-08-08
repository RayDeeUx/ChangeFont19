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

const char* getPng() {
	if (prefix == "stroke") { return "strokegjFont18.png"_spr; }
	if (prefix == "shadow") { return "shadowgjFont18.png"_spr; }
	if (prefix == "mono") { return "monogjFont18.png"_spr; }
	if (prefix == "monoShadow") { return "monoShadowgjFont18.png"_spr; }
	return "gjFont18.png"_spr;
}

const char* getFnt() {
	if (prefix == "stroke") { return "strokegjFont18.fnt"_spr; }
	if (prefix == "shadow") { return "shadowgjFont18.fnt"_spr; }
	if (prefix == "mono") { return "monogjFont18.fnt"_spr; }
	if (prefix == "monoShadow") { return "monoShadowgjFont18.fnt"_spr; }
	return "gjFont18.fnt"_spr;
}

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
		if (!prefix.empty() && isInCreateTextLayers && texture == CCTextureCache::sharedTextureCache()->addImage("gjFont18.png", false)) {
			return CCSpriteBatchNode::initWithTexture(CCTextureCache::sharedTextureCache()->addImage(getPng(), false), capacity);
		}
		return CCSpriteBatchNode::initWithTexture(texture, capacity);
	}
};

class $modify(CCLabelBMFont) {
	static CCLabelBMFont* createBatched(const char* str, const char* fntFile, CCArray* a, int a1) {
		if (!prefix.empty() && strcmp(fntFile, "gjFont18.fnt") == 0) { fntFile = getFnt(); }
		return CCLabelBMFont::createBatched(str, fntFile, a, a1);
	}
	void setFntFile(const char* fntFile) {
		if (!prefix.empty() && strcmp(fntFile, "gjFont18.fnt") == 0 && Mod::get()->getSettingValue<std::string>("fontStyleAggression") == "Levels + Mods") {
			return CCLabelBMFont::setFntFile(getFnt());
		}
		CCLabelBMFont::setFntFile(fntFile);
	}
};

class $modify(CCTextureCache) {
	CCTexture2D* addImage(const char* fileimage, bool p1) {
		CCTexture2D* ret = nullptr;
		bool didChange = false;
		if (!prefix.empty() && strcmp(fileimage, "gjFont18.png") == 0) {
			if (PlayLayer::get() || LevelEditorLayer::get()) {
				didChange = true;
				ret = CCTextureCache::addImage(getPng(), p1);
			}
		}
		if (!didChange) {
			ret = CCTextureCache::addImage(fileimage, p1);
		}
		return ret;
	}
};