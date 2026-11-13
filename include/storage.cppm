module;
#include <string>
export module tamagotchi.storage;
import tamagotchi.types;
import tamagotchi.pet;
import tamagotchi.world;
import tamagotchi.inventory;
export namespace tama {
    class Storage {
    public:
        bool save(const std::string &, const Pet &, const GameWorld &, const Inventory &) const;

        bool load(const std::string &, Pet &, GameWorld &, Inventory &) const;
    };
}
