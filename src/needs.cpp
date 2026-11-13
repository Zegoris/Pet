module;

module tamagotchi.needs;
import tamagotchi.types;

namespace tama {
    namespace {
        bool canIncrease(int value, int amount) {
            return amount > 0 && value < 100;
        }
    }

    bool NeedsSystem::feed(Pet &p, int strength) const {
        if (p.sleeping()) return false;

        const Stats &s = p.stats();
        if (!canIncrease(s.hunger, strength) &&
            !canIncrease(s.happiness, 3)) {
            return false;
        }

        p.apply(strength, 3, -2, -1, 0);
        p.addFeed();
        p.addExperience(8);
        p.addAction();
        return true;
    }

    bool NeedsSystem::play(Pet &p, int fun) const {
        if (!canPlay(p)) return false;

        const Stats &s = p.stats();
        if (!canIncrease(s.hunger, 5) &&
            !canIncrease(s.happiness, fun)) {
            return false;
        }

        p.apply(5, fun, -12, -3, 0);
        p.addPlay();
        p.addExperience(12);
        p.addAction();
        return true;
    }

    bool NeedsSystem::clean(Pet &p) const {
        if (p.sleeping()) return false;

        const Stats &s = p.stats();
        if (!canIncrease(s.happiness, 7) &&
            !canIncrease(s.cleanliness, 35) &&
            !canIncrease(s.health, 2)) {
            return false;
        }

        p.apply(0, 7, -3, 35, 2);
        p.addExperience(6);
        p.addAction();
        return true;
    }

    bool NeedsSystem::heal(Pet &p, int amount) const {
        if (p.sleeping()) return false;

        const Stats &s = p.stats();

        if (!canIncrease(s.health, amount)) {
            return false;
        }

        p.apply(0, 0, 0, 0, amount);
        p.addExperience(5);
        p.addAction();

        return true;
    }

    bool NeedsSystem::sleep(Pet &p) const {
        if (p.sleeping()) return false;

        p.setSleeping(true);
        p.addAction();
        return true;
    }

    bool NeedsSystem::wake(Pet &p) const {
        if (!p.sleeping()) return false;

        p.setSleeping(false);
        p.apply(0, 0, 35, 0, 0);
        p.addAction();
        return true;
    }

    bool NeedsSystem::train(Pet &p) const {
        if (!canPlay(p)) return false;

        const Stats &s = p.stats();
        if (!canIncrease(s.hunger, 4) &&
            !canIncrease(s.happiness, 5) &&
            !canIncrease(s.health, 1)) {
            return false;
        }

        p.apply(4, 5, -18, -2, 1);
        p.addExperience(20);
        p.addAction();
        return true;
    }

    bool NeedsSystem::pet(Pet &p) const {
        if (p.sleeping()) return false;
        if (!canIncrease(p.stats().happiness, 12)) return false;

        p.apply(0, 12, 0, 0, 0);
        p.addExperience(3);
        p.addAction();
        return true;
    }

    void NeedsSystem::tick(Pet &p) const {
        if (p.sleeping()) {
            p.apply(-1, 1, 8, 0, 0);
            return;
        }

        p.apply(-3, -2, -2, -2, 0);

        if (p.stats().hunger < 15 || p.stats().cleanliness < 10) {
            p.apply(0, -2, 0, 0, -3);
        }

        if (p.stats().health < 35) {
            p.apply(0, -1, 0, 0, -1);
        }
    }

    bool NeedsSystem::canPlay(const Pet &p) const {
        return !p.sleeping() &&
               p.stats().energy >= 15 &&
               p.stats().health > 10;
    }

    bool NeedsSystem::needsFood(const Pet &p) const {
        return p.stats().hunger < 45;
    }

    bool NeedsSystem::needsRest(const Pet &p) const {
        return p.stats().energy < 25;
    }
}
