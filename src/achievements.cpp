module;
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
module tamagotchi.achievements;
namespace tama {
    AchievementManager::AchievementManager() : data_{
        {"feed", "Первый обед", "Покормить питомца", false}, {"plays", "Весельчак", "Сыграть 10 раз", false},
        {"level", "Взросление", "Достичь уровня 5", false}, {"week", "Неделя вместе", "Прожить семь дней", false},
        {"actions", "Заботливый хозяин", "Совершить 50 действий", false}
    } {
    }

    void AchievementManager::unlock(const std::string &id, GameWorld &w) {
        auto it = std::find_if(data_.begin(), data_.end(), [&](const auto &a) { return a.id == id; });
        if (it != data_.end() && !it->unlocked) {
            it->unlocked = true;
            w.addCoins(10);
            std::cout << "Достижение открыто: " << it->title << " (+10 монет)\n";
        }
    }

    void AchievementManager::check(const Pet &p, GameWorld &w) {
        if (p.feedCount() >= 1)unlock("feed", w);
        if (p.playCount() >= 10)unlock("plays", w);
        if (p.stats().level >= 5)unlock("level", w);
        if (p.ageDays() >= 7)unlock("week", w);
        if (p.actions() >= 50)unlock("actions", w);
    }

    void AchievementManager::print(std::ostream &o) const {
        for (const auto &a: data_)o << (a.unlocked ? "[✓] " : "[ ] ") << a.title << " — " << a.description << '\n';
    }
}
