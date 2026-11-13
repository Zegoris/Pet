module;

export module tamagotchi.needs;
import tamagotchi.pet;

export namespace tama {
    class NeedsSystem {
    public:
        bool feed(Pet &, int strength = 25) const;

        bool play(Pet &, int fun = 18) const;

        bool clean(Pet &) const;

        bool heal(Pet &, int amount = 30) const;

        bool sleep(Pet &) const;

        bool wake(Pet &) const;

        bool train(Pet &) const;

        bool pet(Pet &) const;

        void tick(Pet &) const;

        bool canPlay(const Pet &) const;

        bool needsFood(const Pet &) const;

        bool needsRest(const Pet &) const;
    };
}
