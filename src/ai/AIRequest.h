#pragma once
#include <string>

struct AIRequest {
    std::string prompt;
    std::string response;
    bool pending = false;
    bool done    = false;
    bool error   = false;
};

namespace AIPrompts {

inline std::string missionBriefing(const std::string& name, const std::string& desc) {
    return "You are a military tactical AI. Give a one-paragraph mission briefing for operation '"
        + name + "'. Mission details: " + desc
        + ". Be concise, serious, and tactical. Max 3 sentences.";
}

inline std::string enemyTaunt(const std::string& type) {
    return "You are a hostile " + type
        + " enemy soldier in a tactical FPS game. Say one short aggressive taunt to the player. Max 1 sentence.";
}

inline std::string victoryMessage(const std::string& mission) {
    return "Mission '" + mission
        + "' completed successfully. Give a short military-style congratulatory message. Max 2 sentences.";
}

inline std::string hint(const std::string& situation) {
    return "Tactical hint for a player in this situation: " + situation
        + ". Give one concise tactical advice. Max 1 sentence.";
}

} // namespace AIPrompts
