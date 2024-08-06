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
Source code based on Alpahalaneous' code
for the "Pusab Font Fix" feature in Happy Textures.
Reused with permission granted by GPLv3.

Bindings found by https://github.com/hiimjustin000.
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
	{"Stronger Stroke", "stroke_"},
	{"Shadow", "shadow_"},
	{"Monospace", "mono_"},
	{"Monospace Shadow", "monoShadow_"}
};

std::string prefix = "";

std::string getFileName(std::string_view extension) {
	return fmt::format("{}gjFont18.{}"_spr, prefix, extension);
}

class $modify(MenuLayer) {
	bool init() {
		bool result = MenuLayer::init();
		if (hasCalledAlready || !prefix.empty()) { return result; }
		hasCalledAlready = true;
		prefix = Mod::get()->getSettingValue<std::string>("fontStyle");
		return result;
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
			return CCSpriteBatchNode::initWithTexture(CCTextureCache::sharedTextureCache()->addImage(getFileName("png").c_str(), false), capacity);
		}
		return CCSpriteBatchNode::initWithTexture(texture, capacity);
	}
};

class $modify(CCLabelBMFont) {
	static CCLabelBMFont* createBatched(const char* str, const char* fntFile, CCArray* a, int a1) {
		if (!prefix.empty() && strcmp(fntFile, "gjFont18.fnt") == 0) { fntFile = getFileName("fnt").c_str(); }
		return CCLabelBMFont::createBatched(str, fntFile, a, a1);
	}
};

class $modify(CCTextureCache) {
	CCTexture2D* addImage(const char* fileimage, bool p1) {
		CCTexture2D* ret = nullptr;
		bool didChange = false;
		if (!prefix.empty() && strcmp(fileimage, "gjFont18.png") == 0) {
			if (PlayLayer::get() || LevelEditorLayer::get()) {
				didChange = true;
				ret = CCTextureCache::addImage(getFileName("png").c_str(), p1);
			}
		}
		if (!didChange) {
			ret = CCTextureCache::addImage(fileimage, p1);
		}
		return ret;
	}
};
