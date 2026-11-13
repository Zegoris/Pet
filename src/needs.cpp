module;
#include <string>
module tamagotchi.needs;
namespace tama {
    void NeedsSystem::feed(Pet &p, int strength) const {
        if (p.sleeping())return;
        p.apply(-strength, 3, -2, -1, 0);
        p.addFeed();
        p.addExperience(8);
        p.addAction();
    }

    void NeedsSystem::play(Pet &p, int fun) const {
        if (p.sleeping())return;
        p.apply(5, fun, -12, -3, 0);
        p.addPlay();
        p.addExperience(12);
        p.addAction();
    }

    void NeedsSystem::clean(Pet &p) const {
        if (p.sleeping())return;
        p.apply(0, 7, -3, 35, 2);
        p.addExperience(6);
        p.addAction();
    }

    void NeedsSystem::heal(Pet &p, int amount) const {
        if (p.sleeping())return;
        p.apply(0, 3, 0, 0, amount);
        p.addExperience(5);
        p.addAction();
    }

    void NeedsSystem::sleep(Pet &p) const {
        if (!p.sleeping()) {
            p.setSleeping(true);
            p.addAction();
        }
    }

    void NeedsSystem::wake(Pet &p) const {
        if (p.sleeping()) {
            p.setSleeping(false);
            p.apply(0, 0, 35, 0, 0);
            p.addAction();
        }
    }

    void NeedsSystem::train(Pet &p) const {
        if (p.sleeping())return;
        p.apply(4, 5, -18, -2, 1);
        p.addExperience(20);
        p.addAction();
    }

    void NeedsSystem::pet(Pet &p) const {
        if (p.sleeping())return;
        p.apply(0, 12, 0, 0, 0);
        p.addExperience(3);
        p.addAction();
    }

    void NeedsSystem::tick(Pet &p) const {
        if (p.sleeping()) {
            p.apply(0, 1, 8, 0, 0);
            return;
        }
        p.apply(3, -2, -2, -2, 0);
        if (p.stats().hunger < 15 || p.stats().cleanliness < 10)p.apply(0, -2, 0, 0, -3);
        if (p.stats().health < 35)p.apply(0, -1, 0, 0, -1);
    }

    bool NeedsSystem::canPlay(const Pet &p) const {
        return !p.sleeping() && p.stats().energy >= 15 && p.stats().health > 10;
    }

    bool NeedsSystem::needsFood(const Pet &p) const { return p.stats().hunger < 45; }
    bool NeedsSystem::needsRest(const Pet &p) const { return p.stats().energy < 25; }
}
