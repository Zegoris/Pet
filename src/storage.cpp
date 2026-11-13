module;
#include <fstream>
#include <limits>
#include <string>
#include <vector>
module tamagotchi.storage;
namespace tama {
    bool Storage::save(const std::string &path, const Pet &p, const GameWorld &w, const Inventory &i) const {
        std::ofstream o(path);
        if (!o)return false;
        const Stats &s = p.stats();
        o << "TAMA_SAVE_1\n" << p.name() << '\n' << p.species() << '\n';
        o << s.hunger << ' ' << s.happiness << ' ' << s.energy << ' ' << s.cleanliness << ' ' << s.health << ' ' << s.
                experience << ' ' << s.level << '\n';
        o << p.sleeping() << ' ' << p.ageDays() << ' ' << p.actions() << ' ' << p.feedCount() << ' ' << p.playCount() <<
                '\n';
        o << w.day() << ' ' << w.hour() << ' ' << w.minute() << ' ' << w.coins() << '\n' << i.items().size() << '\n';
        for (const auto &x: i.items())
            o << x.id << '\n' << x.name << '\n' << x.description << '\n' << static_cast<int>(x
                .type) << ' ' << x.price << ' ' << x.power << ' ' << x.quantity << '\n';
        return o.good();
    }

    bool Storage::load(const std::string &path, Pet &p, GameWorld &w, Inventory &i) const {
        std::ifstream in(path);
        if (!in)return false;
        std::string mark, name, species;
        std::getline(in, mark);
        if (mark != "TAMA_SAVE_1")return false;
        std::getline(in, name);
        std::getline(in, species);
        Stats s;
        if (!(in >> s.hunger >> s.happiness >> s.energy >> s.cleanliness >> s.health >> s.experience >> s.level))
            return
                    false;
        bool sl = false;
        int age = 0, actions = 0, feeds = 0, plays = 0;
        if (!(in >> sl >> age >> actions >> feeds >> plays))return false;
        int day = 1, hour = 8, minute = 0, coins = 100;
        if (!(in >> day >> hour >> minute >> coins))return false;
        std::size_t count = 0;
        if (!(in >> count) || count > 500)return false;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::vector<Item> loaded;
        for (std::size_t n = 0; n < count; ++n) {
            Item x;
            int type = 0;
            std::getline(in, x.id);
            std::getline(in, x.name);
            std::getline(in, x.description);
            if (!(in >> type >> x.price >> x.power >> x.quantity))return false;
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (type < 0 || type > 4)return false;
            x.type = static_cast<ItemType>(type);
            loaded.push_back(x);
        }
        p.rename(name);
        p.restore(s, sl, age, actions, feeds, plays);
        w.restore(day, hour, minute, coins);
        i.restoreItems(loaded);
        return true;
    }
}
