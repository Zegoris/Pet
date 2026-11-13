module;
#include <algorithm>
#include <string>
module tamagotchi.types;
namespace tama {
    int clampStat(int v) { return std::clamp(v, 0, 100); }

    std::string moodName(Mood m) {
        switch (m) {
            case Mood::Happy: return "Счастлив";
            case Mood::Calm: return "Спокоен";
            case Mood::Bored: return "Скучает";
            case Mood::Sad: return "Грустит";
            case Mood::Angry: return "Сердится";
            case Mood::Sick: return "Болеет";
            case Mood::Sleeping: return "Спит";
        }
        return "Неизвестно";
    }

    std::string itemTypeName(ItemType t) {
        switch (t) {
            case ItemType::Food: return "Еда";
            case ItemType::Toy: return "Игрушка";
            case ItemType::Medicine: return "Лекарство";
            case ItemType::Cleaning: return "Уход";
            case ItemType::Special: return "Особое";
        }
        return "Другое";
    }

    std::string progressBar(int value, int width) {
        value = clampStat(value);
        int n = value * width / 100;
        return "[" + std::string(static_cast<std::size_t>(n), '#') +
               std::string(static_cast<std::size_t>(width - n), '-') + "]";
    }
}
