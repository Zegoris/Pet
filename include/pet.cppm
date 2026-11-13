module;
#include <string>
export module tamagotchi.pet;
import tamagotchi.types;
export namespace tama {
    class Pet {
    public:
        explicit Pet(std::string name = "Пиксель", std::string species = "Котик");

        const std::string &name() const;

        const std::string &species() const;

        const Stats &stats() const;

        bool sleeping() const;

        int ageDays() const;

        int actions() const;

        int feedCount() const;

        int playCount() const;

        void rename(const std::string &value);

        void setSleeping(bool value);

        void ageOneDay();

        void addAction();

        void addFeed();

        void addPlay();

        void apply(int hunger, int happiness, int energy, int cleanliness, int health);

        void addExperience(int value);

        Mood mood() const;

        bool critical() const;

        std::string greeting() const;

        void restore(const Stats &, bool, int, int, int, int);

    private:
        std::string name_, species_;
        Stats stats_;
        bool sleeping_ = false;
        int age_ = 0, actions_ = 0, feeds_ = 0, plays_ = 0;
    };
}
