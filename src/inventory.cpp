module;
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
module tamagotchi.inventory;
namespace tama {
    Inventory::Inventory() {
        shop_ = {
            {"apple", "Яблоко", "Лёгкий перекус", ItemType::Food, 8, 18, 0},
            {"fish", "Рыбка", "Сытная еда", ItemType::Food, 15, 32, 0},
            {"cake", "Пирог", "Поднимает настроение", ItemType::Food, 22, 20, 0},
            {"ball", "Мячик", "Игрушка для игры", ItemType::Toy, 18, 22, 0},
            {"yarn", "Клубок", "Мягкая игрушка", ItemType::Toy, 12, 16, 0},
            {"medicine", "Аптечка", "Восстанавливает здоровье", ItemType::Medicine, 25, 40, 0},
            {"soap", "Шампунь", "Средство для купания", ItemType::Cleaning, 10, 45, 0},
            {"star", "Жетон", "Коллекционный предмет", ItemType::Special, 50, 5, 0}
        };
        add(shop_[0], 3);
        add(shop_[3], 1);
    }

    const std::vector<Item> &Inventory::items() const { return items_; }
    const std::vector<Item> &Inventory::shop() const { return shop_; }

    void Inventory::add(const Item &i, int q) {
        if (q <= 0)return;
        auto it = std::find_if(items_.begin(), items_.end(), [&](const Item &x) { return x.id == i.id; });
        if (it == items_.end()) {
            items_.push_back(i);
            items_.back().quantity = q;
        } else it->quantity += q;
    }

    bool Inventory::buy(std::size_t n, int &coins) {
        if (n >= shop_.size() || coins < shop_[n].price)return false;
        coins -= shop_[n].price;
        add(shop_[n]);
        return true;
    }


    bool Inventory::use(std::size_t n, Pet &p, const NeedsSystem &needs) {
        if (n >= items_.size() || items_[n].quantity <= 0 || p.sleeping()) {
            return false;
        }

        Item &i = items_[n];
        bool used = false;

        switch (i.type) {
            case ItemType::Food:
                used = needs.feed(p, i.power);
                break;

            case ItemType::Toy:
                used = needs.play(p, i.power);
                break;

            case ItemType::Medicine:
                used = needs.heal(p, i.power);
                break;

            case ItemType::Cleaning:
                used = needs.clean(p);
                break;

            case ItemType::Special:
                if (p.stats().happiness < 100) {
                    p.apply(0, i.power, 0, 0, 0);
                    p.addExperience(10);
                    p.addAction();
                    used = true;
                }
                break;
        }

        if (!used) return false;

        --i.quantity;
        return true;
    }


    void Inventory::print(std::ostream &o) const {
        o << "\nИНВЕНТАРЬ\n";
        bool any = false;
        for (const auto &i: items_)
            if (i.quantity > 0) {
                any = true;
                o << i.name << " x" << i.quantity << " (" << itemTypeName(i.type) << ")\n";
            }
        if (!any)o << "Пусто.\n";
    }

    void Inventory::printShop(std::ostream &o) const {
        o << "\nМАГАЗИН\n";
        for (std::size_t n = 0; n < shop_.size(); ++n)
            o << n + 1 << ". " << shop_[n].name << " — " << shop_[n].price << " монет\n";
    }

    void Inventory::restoreItems(const std::vector<Item> &v) { items_ = v; }
}
