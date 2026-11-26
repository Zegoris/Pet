module;
#include <iosfwd>
#include <string>
#include <vector>

export module tamagotchi.achievements;

import tamagotchi.types;
import tamagotchi.pet;
import tamagotchi.world;

export namespace tama {
    class AchievementManager {
    public:
        AchievementManager();

        void check(const Pet &, GameWorld &);

        void print(std::ostream &) const;

        std::vector<std::string> unlockedIds() const;

        void restoreUnlocked(const std::vector<std::string> &ids);

        void restoreLegacy(const Pet &);

    private:
        std::vector<AchievementData> data_;

        void unlock(const std::string &, GameWorld &);
    };
}
