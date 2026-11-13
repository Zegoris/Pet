module;

export module tamagotchi.needs;
import tamagotchi.pet;
export namespace tama {
    class NeedsSystem {
    public:
        void feed(Pet &, int strength = 25) const;

        void play(Pet &, int fun = 18) const;

        void clean(Pet &) const;

        void heal(Pet &, int amount = 30) const;

        void sleep(Pet &) const;

        void wake(Pet &) const;

        void train(Pet &) const;

        void pet(Pet &) const;

        void tick(Pet &) const;

        bool canPlay(const Pet &) const;

        bool needsFood(const Pet &) const;

        bool needsRest(const Pet &) const;
    };
}
