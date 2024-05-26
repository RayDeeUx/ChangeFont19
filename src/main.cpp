#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCLabelBMFont.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCSpriteBatchNode.hpp>
#include <Geode/cocos/sprite_nodes/CCSpriteBatchNode.h>

using namespace geode::prelude;

/*
	below code is adapted from alphalaneous' happy textures mod.
	per github licensing rules, alphalaneous' happy textures repo assumes the ARR (all rights reserved) license
	as there are no other licenses in the happy textures repo.

	however, i have been granted explicit permission/consent by alphalaneous themself to re-use their code.
	proof: https://discord.com/channels/911701438269386882/911702535373475870/1206674252896542790

	minecraft.ttf font provided by @bitzelyt on twitter: https://twitter.com/bitzelyt/status/1351621941443125255

	massive thanks to yellowcat98 for recommending the sprite font builder application, or which this project would not have been possible.

	all rights for the original minecraft font goes to mojang.

	-- erymanthus | raydeeux
*/

bool s_isInCreateTextLayers = false;

class $modify(GJBaseGameLayer) {
	
	void createTextLayers(){

		s_isInCreateTextLayers = true;
		GJBaseGameLayer::createTextLayers();
		s_isInCreateTextLayers = false;

	}
};


class $modify(CCSpriteBatchNode) {
	bool initWithTexture(CCTexture2D* texture, unsigned int capacity){

		if(s_isInCreateTextLayers && texture == CCTextureCache::sharedTextureCache()->addImage("gjFont18.png", false)){
			return CCSpriteBatchNode::initWithTexture(CCTextureCache::sharedTextureCache()->addImage("gjFont18.png"_spr, false), capacity);
		}

		return CCSpriteBatchNode::initWithTexture(texture, capacity);
	}
};


class $modify(CCLabelBMFont) {
	
	static CCLabelBMFont* createBatched(const char* str, const char* fntFile, CCArray* a, int a1){

		if(strcmp(fntFile, "gjFont18.fnt") == 0){
			fntFile = "gjFont18.fnt"_spr;
		}

		return CCLabelBMFont::createBatched(str, fntFile, a, a1);
	}
};

class $modify(GameManager) {

	const char* getFontTexture(int val){

		if(val > 58){
			val = 59;
		}

		loadFont(val);

		if(val != 18){
			CCString* str = CCString::createWithFormat("gjFont%02d.png", val);
			return str->getCString();
		}
		
		return "gjFont18.png"_spr;
	}

	void loadFont(int val){

		if(val > 58){
			val = 59;
		}

		if(val != m_loadedFont){

			if(m_loadedFont != 18){
				CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat("gjFont%02d.png", m_loadedFont)->getCString());
			}
			if(val == 18){
				CCTextureCache::sharedTextureCache()->addImage("gjFont18.png"_spr, false);
			}
			else {
				CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat("gjFont%02d.png", val)->getCString(), false);
			}
		}
		m_loadedFont = val;
	}

};
