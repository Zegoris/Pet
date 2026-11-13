module;
#include <string>
#include <random>
export module tamagotchi.world;
export namespace tama {
    class GameWorld {
    public:
        GameWorld();

        int day() const;

        int hour() const;

        int minute() const;

        int coins() const;

        void addCoins(int);

        bool spendCoins(int);

        void advance(int);

        int randomInt(int, int);

        bool chance(int);

        std::string timeString() const;

        std::string randomEvent();

        void restore(int, int, int, int);

    private:
        int day_ = 1, hour_ = 8, minute_ = 0, coins_ = 100;
        std::mt19937 random_;
    };
}
