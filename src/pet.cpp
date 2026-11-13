module;
#include <algorithm>
#include <utility>
#include <string>
module tamagotchi.pet;
namespace tama {
    Pet::Pet(std::string n, std::string s) : name_(std::move(n)), species_(std::move(s)) {
    }

    const std::string &Pet::name() const { return name_; }
    const std::string &Pet::species() const { return species_; }
    const Stats &Pet::stats() const { return stats_; }
    bool Pet::sleeping() const { return sleeping_; }
    int Pet::ageDays() const { return age_; }
    int Pet::actions() const { return actions_; }
    int Pet::feedCount() const { return feeds_; }
    int Pet::playCount() const { return plays_; }
    void Pet::rename(const std::string &v) { if (!v.empty())name_ = v; }
    void Pet::setSleeping(bool v) { sleeping_ = v; }
    void Pet::ageOneDay() { ++age_; }
    void Pet::addAction() { ++actions_; }
    void Pet::addFeed() { ++feeds_; }
    void Pet::addPlay() { ++plays_; }

    void Pet::apply(int h, int joy, int e, int clean, int hp) {
        stats_.hunger = clampStat(stats_.hunger + h);
        stats_.happiness = clampStat(stats_.happiness + joy);
        stats_.energy = clampStat(stats_.energy + e);
        stats_.cleanliness = clampStat(stats_.cleanliness + clean);
        stats_.health = clampStat(stats_.health + hp);
    }

    void Pet::addExperience(int v) {
        if (v <= 0)return;
        stats_.experience += v;
        while (stats_.experience >= stats_.level * 100) {
            stats_.experience -= stats_.level * 100;
            ++stats_.level;
            apply(0, 5, 0, 0, 10);
        }
    }

    Mood Pet::mood() const {
        if (sleeping_)return Mood::Sleeping;
        if (stats_.health < 30)return Mood::Sick;
        if (stats_.happiness < 20)return Mood::Sad;
        if (stats_.hunger < 20)return Mood::Angry;
        if (stats_.happiness < 45 || stats_.energy < 25)return Mood::Bored;
        if (stats_.happiness > 75 && stats_.health > 70)return Mood::Happy;
        return Mood::Calm;
    }

    bool Pet::critical() const { return stats_.health <= 0 || stats_.hunger <= 0; }

    std::string Pet::greeting() const {
        switch (mood()) {
            case Mood::Happy: return "Ура! Я рад тебя видеть!";
            case Mood::Calm: return "Привет! Чем займёмся?";
            case Mood::Bored: return "Мне скучно...";
            case Mood::Sad: return "Побудь со мной.";
            case Mood::Angry: return "Я очень голоден!";
            case Mood::Sick: return "Мне нехорошо. Помоги мне.";
            case Mood::Sleeping: return "Хр-р-р...";
        }
        return "Мяу!";
    }

    void Pet::restore(const Stats &s, bool sl, int age, int actions, int feeds, int plays) {
        stats_ = s;
        sleeping_ = sl;
        age_ = std::max(0, age);
        actions_ = std::max(0, actions);
        feeds_ = std::max(0, feeds);
        plays_ = std::max(0, plays);
    }
}
