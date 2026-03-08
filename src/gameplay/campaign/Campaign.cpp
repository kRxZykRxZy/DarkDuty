#include "Campaign.h"
#include "MissionFactory.h"

Campaign::Campaign() {
    buildMissions();
}

void Campaign::buildMissions() {
    missions_.clear();
    missions_.push_back(MissionFactory::make1_Extraction());
    missions_.push_back(MissionFactory::make2_Siege());
    missions_.push_back(MissionFactory::make3_DarkDuty());
    completed_.assign(missions_.size(), false);
    idx_ = 0;
}
