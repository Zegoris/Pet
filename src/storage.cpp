module;
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <system_error>
#include <vector>

module tamagotchi.storage;

import tamagotchi.types;
import tamagotchi.pet;
import tamagotchi.world;
import tamagotchi.inventory;
import tamagotchi.achievements;

namespace tama {
    bool Storage::save(const std::string &path, const Pet &pet, const GameWorld &world,
                       const Inventory &inventory,
                       const AchievementManager &achievements) const {
        const std::filesystem::path target(path);
        const auto directory = target.parent_path();
        std::error_code error;

        if (!directory.empty()) {
            std::filesystem::create_directories(directory, error);
            if (error) return false;
        }

        auto temporary = target;
        temporary += ".tmp";

        std::ofstream out(temporary);
        if (!out) return false;

        const Stats &stats = pet.stats();
        out << "TAMA_SAVE_2\n" << pet.name() << '\n' << pet.species() << '\n';
        out << stats.hunger << ' ' << stats.happiness << ' ' << stats.energy << ' '
            << stats.cleanliness << ' ' << stats.health << ' '
            << stats.experience << ' ' << stats.level << '\n';
        out << pet.sleeping() << ' ' << pet.ageDays() << ' ' << pet.actions() << ' '
            << pet.feedCount() << ' ' << pet.playCount() << '\n';
        out << world.day() << ' ' << world.hour() << ' ' << world.minute() << ' '
            << world.coins() << '\n';

        out << inventory.items().size() << '\n';
        for (const auto &item : inventory.items()) {
            out << item.id << '\n' << item.name << '\n' << item.description << '\n'
                << static_cast<int>(item.type) << ' ' << item.price << ' '
                << item.power << ' ' << item.quantity << '\n';
        }

        const auto unlocked = achievements.unlockedIds();
        out << unlocked.size() << '\n';
        for (const auto &id : unlocked) {
            out << id << '\n';
        }

        out.close();
        if (!out) {
            std::filesystem::remove(temporary, error);
            return false;
        }

        std::filesystem::rename(temporary, target, error);
        if (error) {
            std::filesystem::remove(temporary, error);
            return false;
        }

        return true;
    }

    bool Storage::load(const std::string &path, Pet &pet, GameWorld &world,
                       Inventory &inventory, AchievementManager &achievements) const {
        std::ifstream in(path);
        if (!in) return false;

        std::string marker;
        if (!std::getline(in, marker)) return false;

        const bool legacy = marker == "TAMA_SAVE_1";
        if (!legacy && marker != "TAMA_SAVE_2") return false;

        std::string name;
        std::string species;
        if (!std::getline(in, name) || !std::getline(in, species)) return false;

        Stats stats;
        if (!(in >> stats.hunger >> stats.happiness >> stats.energy
                 >> stats.cleanliness >> stats.health >> stats.experience >> stats.level)
            || stats.level < 1) {
            return false;
        }

        bool sleeping = false;
        int age = 0, actions = 0, feeds = 0, plays = 0;
        if (!(in >> sleeping >> age >> actions >> feeds >> plays)) return false;

        int day = 0, hour = 0, minute = 0, coins = 0;
        if (!(in >> day >> hour >> minute >> coins)) return false;

        std::size_t itemCount = 0;
        if (!(in >> itemCount) || itemCount > 500) return false;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<Item> items;
        items.reserve(itemCount);
        for (std::size_t index = 0; index < itemCount; ++index) {
            Item item;
            int type = 0;

            if (!std::getline(in, item.id) || !std::getline(in, item.name)
                || !std::getline(in, item.description)
                || !(in >> type >> item.price >> item.power >> item.quantity)) {
                return false;
            }

            if (type < static_cast<int>(ItemType::Food)
                || type > static_cast<int>(ItemType::Special)) {
                return false;
            }

            item.type = static_cast<ItemType>(type);
            items.push_back(item);
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::vector<std::string> unlocked;
        if (!legacy) {
            std::size_t achievementCount = 0;
            if (!(in >> achievementCount) || achievementCount > 500) return false;

            unlocked.reserve(achievementCount);
            for (std::size_t index = 0; index < achievementCount; ++index) {
                std::string id;
                if (!(in >> id)) return false;
                unlocked.push_back(id);
            }
        }

        pet = Pet(name, species);
        pet.restore(stats, sleeping, age, actions, feeds, plays);
        world.restore(day, hour, minute, coins);
        inventory.restoreItems(items);

        if (legacy) {
            achievements.restoreLegacy(pet);
        } else {
            achievements.restoreUnlocked(unlocked);
        }

        return true;
    }
}