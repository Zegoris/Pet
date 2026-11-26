module;
#include <iostream>
#include <string>
#include <array>
#include <filesystem>
#include <system_error>
module tamagotchi.game;
namespace tama {
    std::string Game::chooseSaveSlot() const {
        const std::array<std::string, 3> paths{
            "saves/tamagotchi.sav",
            "saves/tamagotchi_2.sav",
            "saves/tamagotchi_3.sav"
        };

        std::cout << "\nВыбери ячейку сохранения:\n";
        for (std::size_t index = 0; index < paths.size(); ++index) {
            std::error_code error;
            const bool exists = std::filesystem::exists(paths[index], error);

            std::cout << index + 1 << ". Ячейка " << index + 1 << " — ";
            if (error) {
                std::cout << "не удалось проверить\n";
            } else {
                std::cout << (exists ? "занята\n" : "пустая\n");
            }
        }

        std::cout << "0. Отмена\n";
        const int choice = readChoice(0, static_cast<int>(paths.size()));
        if (!std::cin || choice == 0) return {};

        return paths[static_cast<std::size_t>(choice - 1)];
    }

    bool Game::saveToSlot() const {
        const std::string path = chooseSaveSlot();
        if (path.empty()) return false;

        std::error_code error;
        const bool exists = std::filesystem::exists(path, error);
        if (error) {
            std::cout << "Не удалось проверить выбранную ячейку.\n";
            return false;
        }

        if (exists) {
            std::cout << "Ячейка занята. Перезаписать? (1 — да, 0 — нет):\n";
            const int choice = readChoice(0, 1);
            if (!std::cin || choice == 0) return false;
        }

        if (!storage_.save(path, pet_, world_, inventory_, achievements_)) {
            std::cout << "Ошибка сохранения.\n";
            return false;
        }

        std::cout << "Сохранено.\n";
        return true;
    }

    Game::Game() = default;

    void Game::header() const {
        std::cout <<
                "\n========================================\nМОЙ МАЛЕНЬКИЙ ТАМАГОЧИ\n========================================\nДень "
                << world_.day() << " | " << world_.timeString() << " | Монеты: " << world_.coins() << "\n";
    }

    void Game::meter(const std::string &label, int v) const {
        std::cout << label << ' ' << progressBar(v) << ' ' << v << "/100\n";
    }

    void Game::status() const {
        header();
        const auto &s = pet_.stats();
        std::cout << pet_.species() << " по имени " << pet_.name() << "\nНастроение: " << moodName(pet_.mood()) << "\n"
                << pet_.greeting() << "\n\n";
        meter("Сытость ", s.hunger);
        meter("Счастье ", s.happiness);
        meter("Энергия ", s.energy);
        meter("Чистота ", s.cleanliness);
        meter("Здоровье", s.health);
        std::cout << "Уровень: " << s.level << " | Опыт: " << s.experience << '/' << s.level * 100 << "\nВозраст: " <<
                pet_.ageDays() << " дней\n";
        if (pet_.critical())std::cout << "ВНИМАНИЕ: питомцу нужна помощь!\n";
    }

    int Game::readChoice(int lo, int hi) const {
        while (true) {
            std::cout << "> ";
            std::string line;
            if (!std::getline(std::cin, line))return hi;
            try {
                std::size_t n = 0;
                int v = std::stoi(line, &n);
                if (n == line.size() && v >= lo && v <= hi)return v;
            } catch (...) {
            }
            std::cout << "Введите число от " << lo << " до " << hi << ".\n";
        }
    }

    void Game::pause() const {
        std::cout << "\nEnter — продолжить...";
        std::string s;
        std::getline(std::cin, s);
    }

    void Game::advanceTime(int m) {
        int old = world_.day();
        world_.advance(m);
        needs_.tick(pet_);
        if (old != world_.day()) {
            pet_.ageOneDay();
            std::cout << "Наступил новый день!\n";
        }
        if (++eventCounter_ % 4 == 0)std::cout << "Событие: " << world_.randomEvent() << '\n';
        achievements_.check(pet_, world_);
    }


    void Game::careMenu() {
        while (true) {
            header();
            std::cout <<
                    "\n1. Покормить\n2. Поиграть\n3. Помыть\n4. Лечить\n5. Погладить\n6. Тренировка\n7. Спать\n8. Разбудить\n9. Предмет из инвентаря\n0. Назад\n";

            int c = readChoice(0, 9);
            if (!c) return;

            if (c == 9) {
                inventoryMenu();
                pause();
                continue;
            }

            bool acted = false;

            switch (c) {
                case 1:
                    acted = needs_.feed(pet_);
                    break;

                case 2:
                    acted = needs_.play(pet_);
                    if (!acted && !needs_.canPlay(pet_)) {
                        std::cout << "Питомец слишком устал или плохо себя чувствует.\n";
                    }
                    break;

                case 3:
                    acted = needs_.clean(pet_);
                    break;

                case 4:
                    acted = needs_.heal(pet_);
                    break;

                case 5:
                    acted = needs_.pet(pet_);
                    break;

                case 6:
                    acted = needs_.train(pet_);
                    if (!acted && !needs_.canPlay(pet_)) {
                        std::cout << "Недостаточно энергии для тренировки.\n";
                    }
                    break;

                case 7:
                    acted = needs_.sleep(pet_);
                    break;

                case 8:
                    acted = needs_.wake(pet_);
                    break;
            }

            if (acted) {
                advanceTime(30);
            } else {
                std::cout << "Сейчас это действие не принесёт пользы.\n";
            }

            pause();
        }
    }


    void Game::shopMenu() {
        while (true) {
            header();
            inventory_.printShop(std::cout);
            std::cout << "0. Назад\n";
            int c = readChoice(0, static_cast<int>(inventory_.shop().size()));
            if (!c)return;
            int money = world_.coins();
            if (inventory_.buy(static_cast<std::size_t>(c - 1), money)) {
                world_.spendCoins(world_.coins() - money);
                std::cout << "Покупка добавлена в инвентарь.\n";
                advanceTime(10);
            } else std::cout << "Недостаточно монет.\n";
            pause();
        }
    }

    void Game::inventoryMenu() {
        inventory_.print(std::cout);
        std::cout << "Номер предмета для использования (0 — отмена):\n";
        int c = readChoice(0, static_cast<int>(inventory_.items().size()));
        if (c > 0) {
            if (inventory_.use(static_cast<std::size_t>(c - 1), pet_, needs_)) {
                std::cout << "Предмет использован.\n";
                advanceTime(20);
            } else std::cout << "Не удалось использовать предмет.\n";
        }
    }

    void Game::renamePet() {
        std::cout << "Новое имя: ";
        std::string n;
        std::getline(std::cin, n);
        if (!n.empty() && n.size() <= 30)pet_.rename(n);
        else std::cout << "Имя должно быть длиной 1–30 символов.\n";
    }

    void Game::help() const {
        header();
        std::cout << "Следи за сытостью, счастьем, энергией, чистотой и здоровьем.\n" <<
                "Показатели меняются со временем. Заботься о питомце,\n" <<
                "зарабатывай опыт, покупай предметы и открывай достижения.\n";
        pause();
    }

    void Game::achievementsMenu() const {
        header();
        achievements_.print(std::cout);
        pause();
    }

    void Game::deleteSaveSlot() const {
        const std::string path = chooseSaveSlot();
        if (path.empty()) return;

        std::error_code error;
        const auto status = std::filesystem::symlink_status(path, error);

        if (status.type() == std::filesystem::file_type::not_found) {
            std::cout << "Ячейка уже пуста.\n";
            return;
        }

        if (error || !std::filesystem::is_regular_file(status)) {
            std::cout << "Не удалось открыть файл сохранения для удаления.\n";
            return;
        }

        std::cout << "Удалить сохранение из выбранной ячейки? (1 — да, 0 — нет):\n";
        const int choice = readChoice(0, 1);
        if (!std::cin || choice == 0) return;

        const bool removed = std::filesystem::remove(path, error);
        if (error) {
            std::cout << "Не удалось удалить сохранение.\n";
        } else if (!removed) {
            std::cout << "Ячейка уже пуста.\n";
        } else {
            std::cout << "Сохранение удалено.\n";
        }
    }

    void Game::run() {
        std::cout << "Как назовём питомца? (Enter — Пиксель): ";
        std::string name;
        std::getline(std::cin, name);
        if (!name.empty()) pet_.rename(name);

        while (running_) {
            header();
            std::cout
                    << "\n1. Состояние\n2. Забота\n3. Инвентарь\n4. Магазин\n"
                    << "5. Достижения\n6. Переименовать\n7. Сохранить\n"
                    << "8. Загрузить\n9. Справка\n10. Удалить сохранение\n0. Выход\n";

            const int choice = readChoice(0, 10);
            if (!std::cin) return;

            switch (choice) {
                case 1:
                    status();
                    pause();
                    break;
                case 2:
                    careMenu();
                    break;
                case 3:
                    inventoryMenu();
                    pause();
                    break;
                case 4:
                    shopMenu();
                    break;
                case 5:
                    achievementsMenu();
                    break;
                case 6:
                    renamePet();
                    pause();
                    break;
                case 7:
                    saveToSlot();
                    pause();
                    break;
                case 8: {
                    const std::string path = chooseSaveSlot();
                    if (path.empty()) break;

                    std::cout << (storage_.load(path, pet_, world_, inventory_, achievements_)
                                      ? "Загружено.\n"
                                      : "Не удалось загрузить. Ячейка пуста или файл недоступен.\n");
                    pause();
                    break;
                }
                case 10:
                    deleteSaveSlot();
                    pause();
                    break;
                case 9:
                    help();
                    break;
                case 0: {
                    std::cout << "Сохранить перед выходом? (1 — да, 0 — нет):\n";
                    const int saveChoice = readChoice(0, 1);
                    if (!std::cin) return;

                    if (saveChoice == 1 && !saveToSlot()) break;

                    running_ = false;
                    std::cout << "До встречи!\n";
                    break;
                }
            }
        }
    }
}
