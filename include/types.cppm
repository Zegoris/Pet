module;
#include <string>
#include <vector>
#include <iosfwd>
export module tamagotchi.types;
export namespace tama {
    enum class Mood { Happy, Calm, Bored, Sad, Angry, Sick, Sleeping };

    enum class ItemType { Food, Toy, Medicine, Cleaning, Special };

    struct Stats {
        int hunger = 75, happiness = 70, energy = 80, cleanliness = 90, health = 100;
        int experience = 0, level = 1;
    };

    struct Item {
        std::string id, name, description;
        ItemType type = ItemType::Food;
        int price = 0, power = 0, quantity = 0;
    };

    struct AchievementData {
        std::string id, title, description;
        bool unlocked = false;
    };

    int clampStat(int value);

    std::string moodName(Mood mood);

    std::string itemTypeName(ItemType type);

    std::string progressBar(int value, int width = 20);
}
