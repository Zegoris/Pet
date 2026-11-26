module;
#include <string>

export module tamagotchi.storage;

import tamagotchi.pet;
import tamagotchi.world;
import tamagotchi.inventory;
import tamagotchi.achievements;

export namespace tama {
    class Storage {
    public:
        bool save(const std::string &path, const Pet &, const GameWorld &,
                  const Inventory &, const AchievementManager &) const;

        bool load(const std::string &path, Pet &, GameWorld &,
                  Inventory &, AchievementManager &) const;
    };
}
