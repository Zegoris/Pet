module;
#include <string>
#include <vector>
#include <iosfwd>
#include <cstddef>
#include <vector>
export module tamagotchi.inventory;
import tamagotchi.types;
import tamagotchi.pet;
import tamagotchi.needs;
export namespace tama {
    class Inventory {
    public:
        Inventory();

        const std::vector<Item> &items() const;

        const std::vector<Item> &shop() const;

        void add(const Item &, int quantity = 1);

        bool buy(std::size_t index, int &coins);

        bool use(std::size_t index, Pet &, const NeedsSystem &);

        void print(std::ostream &) const;

        void printShop(std::ostream &) const;

        void restoreItems(const std::vector<Item> &);

    private:
        std::vector<Item> items_, shop_;
    };
}
