module;
#include <string>
#include <iosfwd>
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

    private:
        std::vector<AchievementData> data_;

        void unlock(const std::string &, GameWorld &);
    };
}
