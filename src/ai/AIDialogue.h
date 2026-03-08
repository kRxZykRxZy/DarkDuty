#pragma once
#include "AsyncAIRequest.h"
#include "AIRequest.h"
#include <string>
#include <queue>

class AIDialogue {
public:
    AIDialogue() = default;

    void requestMissionBriefing(const std::string& missionName, const std::string& missionDesc) {
        std::string prompt = AIPrompts::missionBriefing(missionName, missionDesc);
        async_.submit(prompt, [this](const std::string& r) {
            if (!r.empty()) messages_.push(r);
        });
    }

    void requestEnemyTaunt(const std::string& enemyType) {
        std::string prompt = AIPrompts::enemyTaunt(enemyType);
        async_.submit(prompt, [this](const std::string& r) {
            if (!r.empty()) messages_.push(r);
        });
    }

    void requestVictoryMessage(const std::string& missionName) {
        std::string prompt = AIPrompts::victoryMessage(missionName);
        async_.submit(prompt, [this](const std::string& r) {
            if (!r.empty()) messages_.push(r);
        });
    }

    void requestHint(const std::string& situation) {
        std::string prompt = AIPrompts::hint(situation);
        async_.submit(prompt, [this](const std::string& r) {
            if (!r.empty()) messages_.push(r);
        });
    }

    void update(float dt) {
        if (cooldown_ > 0.f) cooldown_ -= dt;
        async_.update();
    }

    bool hasPendingMessages() const {
        return !messages_.empty();
    }

    std::string popNextMessage() {
        if (messages_.empty()) return "";
        std::string m = messages_.front();
        messages_.pop();
        return m;
    }

private:
    AsyncAIRequest      async_;
    std::queue<std::string> messages_;
    float               cooldown_ = 0.f;
};
