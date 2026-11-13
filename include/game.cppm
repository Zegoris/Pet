module;
#include <string>
export module tamagotchi.game;
import tamagotchi.types;
import tamagotchi.pet;
import tamagotchi.needs;
import tamagotchi.world;
import tamagotchi.inventory;
import tamagotchi.storage;
import tamagotchi.achievements;
export namespace tama {
    class Game {
    public:
        Game();

        void run();

    private:
        Pet pet_;
        GameWorld world_;
        NeedsSystem needs_;
        Inventory inventory_;
        Storage storage_;
        AchievementManager achievements_;
        bool running_ = true;
        int eventCounter_ = 0;
        std::string saveFile_ = "saves/tamagotchi.sav";

        int readChoice(int, int) const;

        void pause() const;

        void header() const;

        void status() const;

        void meter(const std::string &, int) const;

        void advanceTime(int);

        void careMenu();

        void shopMenu();

        void inventoryMenu();

        void renamePet();

        void help() const;

        void achievementsMenu() const;
    };
}
