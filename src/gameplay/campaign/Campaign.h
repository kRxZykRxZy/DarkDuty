#pragma once
#include "Mission.h"
#include <vector>

// Forward declaration
class MissionFactory;

class Campaign {
public:
    Campaign();

    int      count()   const { return (int)missions_.size(); }
    Mission& current()       { return missions_[idx_]; }
    Mission& get(int i)      { return missions_[i]; }

    void advance() {
        if (idx_ < (int)missions_.size() - 1) {
            completed_[idx_] = true;
            ++idx_;
        } else {
            completed_[idx_] = true;
        }
    }

    void reset() { idx_ = 0; for (auto& c : completed_) c = false; }

    bool isComplete() const {
        for (bool c : completed_) if (!c) return false;
        return !completed_.empty();
    }

    int  currentIndex() const { return idx_; }
    std::vector<bool>& completed() { return completed_; }

private:
    std::vector<Mission> missions_;
    int                  idx_       = 0;
    std::vector<bool>    completed_;

    void buildMissions();
};
