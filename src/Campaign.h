#pragma once
#include "Level.h"
#include "Weapon.h"
#include <vector>
#include <string>

// ─── Campaign mission definition ─────────────────────────────────────────────
struct Mission {
    int         id;
    std::string codename;
    std::string title;
    std::string briefing;    // multi-line briefing text (newlines = '\n')
    Level       level;
    std::vector<WeaponID> loadout; // available weapons
};

// ─── Campaign state ───────────────────────────────────────────────────────────
struct Campaign {
    std::vector<Mission> missions;
    int currentMission = 0;
    std::vector<bool> completed; // per mission

    Campaign() { buildMissions(); }

    Mission& current() { return missions[currentMission]; }
    int      count()   const { return static_cast<int>(missions.size()); }
    bool     isComplete() const {
        for (bool b : completed) if (!b) return false;
        return true;
    }

private:
    void buildMissions();
};

// ─── Level data (maps defined as ASCII art) ───────────────────────────────────
inline void Campaign::buildMissions() {
    missions.reserve(3);
    completed.assign(3, false);

    // ── Mission 1 : "Extraction" ─────────────────────────────────────────────
    {
        Mission m;
        m.id       = 0;
        m.codename = "EXTRACTION";
        m.title    = "Op. Extraction";
        m.briefing =
            "Intel confirms a high-value target is being held\n"
            "in a fortified compound. Eliminate all hostiles,\n"
            "then reach the extraction point.\n\n"
            "Weapons: Pistol, M4A1\n"
            "Threat Level: MODERATE";
        m.loadout  = { WeaponID::PISTOL, WeaponID::M4A1 };
        m.level    = buildLevel({
            "##################################################",
            "#................................................#",
            "#..BBB.......1.......1.......1..........1.......#",
            "#................................................#",
            "#....BBB.......##########.....BBB...............#",
            "#..1.....1.....#........#.......................1.#",
            "#..............#...1....#.....BBB...............#",
            "#..............#........#.......................#",
            "#....BBB.......##########.....BBB...............#",
            "#..1............1.......1..........1............#",
            "#................................................#",
            "#..BBB...BBB...........................BBB......#",
            "#................................................#",
            "#..1.......1.....1.....1.....1.....1............#",
            "#................................................#",
            "#.P...........................................E...#",
            "#................................................#",
            "##################################################",
        }, ObjType::ELIMINATE_ALL, "Eliminate all enemies, then reach exit");
        missions.push_back(std::move(m));
    }

    // ── Mission 2 : "Siege" ───────────────────────────────────────────────────
    {
        Mission m;
        m.id       = 1;
        m.codename = "SIEGE";
        m.title    = "Op. Siege";
        m.briefing =
            "Enemy forces have fortified a multi-room complex.\n"
            "Breach and clear each room. Heavies and snipers\n"
            "are defending key chokepoints. Stay behind cover.\n\n"
            "Weapons: Pistol, M4A1, MP5, Shotgun\n"
            "Threat Level: HIGH";
        m.loadout  = { WeaponID::PISTOL, WeaponID::M4A1, WeaponID::MP5, WeaponID::SHOTGUN };
        m.level    = buildLevel({
            "##################################################",
            "#.P..............................................#",
            "#....BBB.........................................#",
            "###########.###########.######################.#",
            "#...1.......#...2.......#......................#.#",
            "#.BBB.......#....BBB....#...1...1...........BBB.#",
            "#...1.......#...........#......................#.#",
            "###########.###########.#....BBB...............#",
            "#...........#...........#......................#.#",
            "#.BBB...1...#..3..BBB...#...............3......#",
            "#...........#...........#......................#.#",
            "###########.###########.######################.#",
            "#.....................................................#",
            "#..BBB...1......2.......1......BBB.....2........#",
            "#.....................................................#",
            "#..1.........................................1...#",
            "#....BBB.....................................E...#",
            "##################################################",
        }, ObjType::ELIMINATE_ALL, "Clear all rooms and extract");
        missions.push_back(std::move(m));
    }

    // ── Mission 3 : "Dark Duty" (Final) ──────────────────────────────────────
    {
        Mission m;
        m.id       = 2;
        m.codename = "DARK DUTY";
        m.title    = "Op. Dark Duty";
        m.briefing =
            "DEFCON 1. An enemy general is fortified in the\n"
            "command bunker with elite forces. You are the\n"
            "last operative standing. Full loadout authorized.\n"
            "Survive the waves and eliminate the general.\n\n"
            "Weapons: FULL LOADOUT\n"
            "Threat Level: EXTREME";
        m.loadout  = {
            WeaponID::PISTOL, WeaponID::M4A1, WeaponID::MP5,
            WeaponID::SHOTGUN, WeaponID::SNIPER, WeaponID::RPG
        };
        m.level    = buildLevel({
            "##################################################",
            "#................................................#",
            "#.1..BBB...1..BBB...1..BBB...1..BBB...1..BBB..1.#",
            "#................................................#",
            "###.#####.####.#####.####.#####.####.#######.###",
            "#.............................................E..#",
            "#..BBB.....BBB...........BBB.....BBB............#",
            "#................................................#",
            "#.1..1..BBB..2..1..BBB..2..1..BBB..2..1..BBB..1#",
            "#................................................#",
            "###.#####.####.#####.####.#####.####.#######.###",
            "#................................................#",
            "#..BBB.....1.....2....BBB....1.....2....BBB.....#",
            "#................................................#",
            "#.1.....1.....1.....1.....1.....1.....1.....1...#",
            "#................................................#",
            "#.P..............................................#",
            "##################################################",
        }, ObjType::SURVIVE_WAVES, "Survive 3 waves and reach extraction", 3);
        missions.push_back(std::move(m));
    }
}
