module;
#include <iostream>
#include <string>
module tamagotchi.game;
namespace tama {
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

    void Game::run() {
        std::cout << "Как назовём питомца? (Enter — Пиксель): ";
        std::string n;
        std::getline(std::cin, n);
        if (!n.empty())pet_.rename(n);
        while (running_) {
            header();
            std::cout <<
                    "\n1. Состояние\n2. Забота\n3. Инвентарь\n4. Магазин\n5. Достижения\n6. Переименовать\n7. Сохранить\n8. Загрузить\n9. Справка\n0. Выход\n";
            int c = readChoice(0, 9);
            switch (c) {
                case 1: status();
                    pause();
                    break;
                case 2: careMenu();
                    break;
                case 3: inventoryMenu();
                    pause();
                    break;
                case 4: shopMenu();
                    break;
                case 5: achievementsMenu();
                    break;
                case 6: renamePet();
                    pause();
                    break;
                case 7: std::cout << (storage_.save(saveFile_, pet_, world_, inventory_)
                                          ? "Сохранено.\n"
                                          : "Ошибка сохранения.\n");
                    pause();
                    break;
                case 8: std::cout << (storage_.load(saveFile_, pet_, world_, inventory_)
                                          ? "Загружено.\n"
                                          : "Не удалось загрузить.\n");
                    pause();
                    break;
                case 9: help();
                    break;
                case 0: std::cout << "Сохранить перед выходом? (1 — да, 0 — нет):\n";
                    if (readChoice(0, 1))storage_.save(saveFile_, pet_, world_, inventory_);
                    running_ = false;
                    std::cout << "До встречи!\n";
                    break;
            }
        }
    }
}
