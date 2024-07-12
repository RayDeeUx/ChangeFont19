using namespace geode::prelude;

$on_mod(Loaded) {
    CCFileUtils::get()->addTexturePack(CCTexturePack {
        .m_id = Mod::get()->getID(), // they're the same ID so it doesnt matter
        .m_paths = { Mod::get()->getResourcesDir() }
    });
}