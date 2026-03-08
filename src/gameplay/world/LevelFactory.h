#pragma once
#include "Level.h"
#include <vector>
#include <string>

// Builds a Level from an ASCII art map string array.
// Legend:
//   '#' WALL   '.' FLOOR   'B' COVER  'W' WATER  'E' EXIT
//   'P' player spawn
//   '1' soldier  '2' heavy  '3' sniper  '4' rusher  (wave 0)
//   '5' soldier  '6' heavy  (wave 1)
//   '7' soldier  '8' heavy  (wave 2)
class LevelFactory {
public:
    static Level build(const std::vector<std::string>& lines,
                       const std::string& name,
                       ObjType objType,
                       const std::string& objLabel,
                       const std::string& lightPreset = "urban",
                       int surviveWaves = 0);
};
