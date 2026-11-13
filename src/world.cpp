module;
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
module tamagotchi.world;
namespace tama {
    GameWorld::GameWorld() : random_(std::random_device{}()) {
    }

    int GameWorld::day() const { return day_; }
    int GameWorld::hour() const { return hour_; }
    int GameWorld::minute() const { return minute_; }
    int GameWorld::coins() const { return coins_; }
    void GameWorld::addCoins(int n) { coins_ = std::max(0, coins_ + n); }

    bool GameWorld::spendCoins(int n) {
        if (n < 0 || coins_ < n)return false;
        coins_ -= n;
        return true;
    }

    void GameWorld::advance(int n) {
        if (n < 0)return;
        minute_ += n;
        while (minute_ >= 60) {
            minute_ -= 60;
            ++hour_;
            if (hour_ >= 24) {
                hour_ = 0;
                ++day_;
            }
        }
    }

    int GameWorld::randomInt(int a, int b) {
        std::uniform_int_distribution<int> d(a, b);
        return d(random_);
    }

    bool GameWorld::chance(int p) { return randomInt(1, 100) <= p; }

    std::string GameWorld::timeString() const {
        std::ostringstream s;
        s << std::setfill('0') << std::setw(2) << hour_ << ':' << std::setw(2) << minute_;
        return s.str();
    }

    std::string GameWorld::randomEvent() {
        switch (randomInt(1, 6)) {
            case 1: addCoins(10);
                return "Ты нашёл 10 монет!";
            case 2: return "Солнечный день поднял настроение.";
            case 3: return "Питомец увидел бабочку.";
            case 4: return "Тихий день без происшествий.";
            case 5: addCoins(5);
                return "Сосед подарил 5 монет.";
            default: return "Питомец исследовал комнату.";
        }
    }

    void GameWorld::restore(int d, int h, int m, int c) {
        day_ = std::max(1, d);
        hour_ = std::clamp(h, 0, 23);
        minute_ = std::clamp(m, 0, 59);
        coins_ = std::max(0, c);
    }
}
