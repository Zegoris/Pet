module;
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

module tamagotchi.achievements;

import tamagotchi.types;
import tamagotchi.pet;
import tamagotchi.world;

namespace tama {
    namespace {
        std::vector<std::string> earnedIds(const Pet &pet) {
            std::vector<std::string> ids;

            if (pet.feedCount() >= 1) ids.emplace_back("feed");
            if (pet.playCount() >= 10) ids.emplace_back("plays");
            if (pet.stats().level >= 5) ids.emplace_back("level");
            if (pet.ageDays() >= 7) ids.emplace_back("week");
            if (pet.actions() >= 50) ids.emplace_back("actions");

            return ids;
        }
    }

    AchievementManager::AchievementManager() : data_{
        {"feed", "Первый обед", "Покормить питомца", false},
        {"plays", "Весельчак", "Сыграть 10 раз", false},
        {"level", "Взросление", "Достичь уровня 5", false},
        {"week", "Неделя вместе", "Прожить семь дней", false},
        {"actions", "Заботливый хозяин", "Совершить 50 действий", false}
    } {
    }

    void AchievementManager::unlock(const std::string &id, GameWorld &world) {
        const auto it = std::find_if(data_.begin(), data_.end(), [&](const auto &achievement) {
            return achievement.id == id;
        });

        if (it == data_.end() || it->unlocked) return;

        it->unlocked = true;
        world.addCoins(10);
        std::cout << "Достижение открыто: " << it->title << " (+10 монет)\n";
    }

    void AchievementManager::check(const Pet &pet, GameWorld &world) {
        for (const auto &id: earnedIds(pet)) {
            unlock(id, world);
        }
    }

    void AchievementManager::print(std::ostream &out) const {
        for (const auto &achievement: data_) {
            out << (achievement.unlocked ? "[✓] " : "[ ] ")
                    << achievement.title << " — " << achievement.description << '\n';
        }
    }

    std::vector<std::string> AchievementManager::unlockedIds() const {
        std::vector<std::string> ids;

        for (const auto &achievement: data_) {
            if (achievement.unlocked) {
                ids.push_back(achievement.id);
            }
        }

        return ids;
    }

    void AchievementManager::restoreUnlocked(const std::vector<std::string> &ids) {
        for (auto &achievement: data_) {
            achievement.unlocked =
                    std::find(ids.begin(), ids.end(), achievement.id) != ids.end();
        }
    }

    void AchievementManager::restoreLegacy(const Pet &pet) {
        restoreUnlocked(earnedIds(pet));
    }
}
