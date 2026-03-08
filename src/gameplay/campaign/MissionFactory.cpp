#include "MissionFactory.h"
#include "../../gameplay/world/LevelFactory.h"

Mission MissionFactory::make1_Extraction() {
    static const std::vector<std::string> MAP = {
        "##################################################",
        "#................................................#",
        "#..BBB.......1.......1.......1..........1.......#",
        "#................................................#",
        "#....BBB.......##########.....BBB...............#",
        "#..1.....1.....#........#.......................1#",
        "#..............#...1....#.....BBB...............#",
        "#..............#........#.......................#",
        "#....BBB.......##########.....BBB...............#",
        "#..1............1.......1..........1............#",
        "#................................................#",
        "#..BBB...BBB...........................BBB......#",
        "#................................................#",
        "#..1.......1.....1.....1.....1.....1............#",
        "#................................................#",
        "#.P...........................................E..#",
        "#................................................#",
        "##################################################",
    };

    Mission m;
    m.id           = 1;
    m.codename     = "EXTRACTION";
    m.title        = "Operation Extraction";
    m.briefing     = "Enemy forces have occupied a key urban zone. Eliminate all hostiles and extract safely.";
    m.aiDescriptor = "urban extraction mission with soldiers";
    m.loadout      = { WeaponID::PISTOL, WeaponID::M4A1 };
    m.level        = LevelFactory::build(MAP, "EXTRACTION",
                         ObjType::ELIMINATE_ALL,
                         "Eliminate all enemies",
                         "urban", 0);
    return m;
}

Mission MissionFactory::make2_Siege() {
    static const std::vector<std::string> MAP = {
        "##################################################",
        "#.P..............................................#",
        "#....BBB.........................................#",
        "###########.###########.######################.#",
        "#...1.......#...2.......#..BBB.....BBB..BBB....#",
        "#.BBB.......#....BBB....#...1...1..............#",
        "#...1.......#...........#......1...BBB.........#",
        "###########.###########.#.......................#",
        "#...........#...........#...1...............3..#",
        "#.BBB...1...#..3..BBB...#...BBB................#",
        "#...........#...........#.......................#",
        "###########.###########.#...1...1..1...........#",
        "#...............................................#",
        "#..BBB...1......2.......1......BBB.....2........#",
        "#...............................................#",
        "#..1.........................................1..#",
        "#....BBB.....................................E..#",
        "##################################################",
    };

    Mission m;
    m.id           = 2;
    m.codename     = "SIEGE";
    m.title        = "Operation Siege";
    m.briefing     = "A fortified compound must be cleared. Heavy resistance expected. All targets must be neutralised.";
    m.aiDescriptor = "bunker siege mission with heavy enemies and snipers";
    m.loadout      = { WeaponID::PISTOL, WeaponID::M4A1, WeaponID::MP5, WeaponID::SHOTGUN };
    m.level        = LevelFactory::build(MAP, "SIEGE",
                         ObjType::ELIMINATE_ALL,
                         "Eliminate all enemies",
                         "bunker", 0);
    return m;
}

Mission MissionFactory::make3_DarkDuty() {
    static const std::vector<std::string> MAP = {
        "##################################################",
        "#................................................#",
        "#.1..BBB...1..BBB...1..BBB...1..BBB...1..BBB..1#",
        "#................................................#",
        "###.#####.####.#####.####.#####.####.#######.###",
        "#.............................................E.#",
        "#..BBB.....BBB...........BBB.....BBB...........#",
        "#................................................#",
        "#.1..1..BBB..2..1..BBB..2..1..BBB..2..1..BBB.1#",
        "#................................................#",
        "###.#####.####.#####.####.#####.####.#######.###",
        "#................................................#",
        "#..BBB.....1.....2....BBB....1.....2....BBB....#",
        "#................................................#",
        "#.5.....6.....5.....6.....7.....8.....7.....8..#",
        "#................................................#",
        "#.P..............................................#",
        "##################################################",
    };

    Mission m;
    m.id           = 3;
    m.codename     = "DARK DUTY";
    m.title        = "Operation Dark Duty";
    m.briefing     = "Final stand. The enemy is everywhere. Survive three waves of relentless assault. This is your Dark Duty.";
    m.aiDescriptor = "final endgame survival mission with all enemy types across 3 waves";
    m.loadout      = {
        WeaponID::PISTOL, WeaponID::M4A1, WeaponID::MP5,
        WeaponID::SHOTGUN, WeaponID::SNIPER, WeaponID::RPG
    };
    m.level = LevelFactory::build(MAP, "DARK DUTY",
                  ObjType::SURVIVE_WAVES,
                  "Survive 3 waves",
                  "endgame", 3);
    return m;
}
