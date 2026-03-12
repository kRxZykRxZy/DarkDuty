#include "LoadingScreen.h"
#include <string>

void LoadingScreen::draw(float progress) {
    if (!r_) return;
    int sw = r_->screenW(), sh = r_->screenH();

    // Dark background
    r_->drawRect(0, 0, (float)sw, (float)sh, 0.02f, 0.04f, 0.02f, 1.f);

    // Subtle grid lines
    for (int i = 0; i < 20; i++) {
        float x = (float)(i * (sw / 20));
        r_->drawLine(x, 0, x, (float)sh, 0.f, 0.12f, 0.f, 0.2f);
    }
    for (int i = 0; i < 15; i++) {
        float y = (float)(i * (sh / 15));
        r_->drawLine(0, y, (float)sw, y, 0.f, 0.12f, 0.f, 0.2f);
    }

    auto fL = dt_ ? dt_->fontLarge()  : nullptr;
    auto fM = dt_ ? dt_->fontMedium() : nullptr;
    auto fS = dt_ ? dt_->fontSmall()  : nullptr;

    // Title
    r_->drawText("DARK DUTY",
        (float)(sw / 2), (float)(sh / 2 - 140),
        fL, 1.f, 0.65f, 0.f, 1.f, true);

    // Sub-label
    r_->drawText("Loading Mission...",
        (float)(sw / 2), (float)(sh / 2 - 75),
        fM, 0.65f, 0.65f, 0.55f, 1.f, true);

    // Progress bar
    float barW = (float)(sw * 6 / 10);
    float barH = 24.f;
    float barX = (float)(sw / 2) - barW * 0.5f;
    float barY = (float)(sh / 2 - 20);

    r_->drawRect(barX, barY, barW, barH, 0.08f, 0.08f, 0.06f, 1.f);
    r_->drawRectOutline(barX, barY, barW, barH, 0.55f, 0.38f, 0.f, 1.f);

    float pct = (progress < 0.f) ? 0.f : (progress > 100.f ? 100.f : progress);
    float fillW = (barW - 2.f) * (pct / 100.f);
    if (fillW > 0.f)
        r_->drawRect(barX + 1.f, barY + 1.f, fillW, barH - 2.f,
                     0.85f, 0.55f, 0.f, 1.f);

    // Percentage label
    std::string pctStr = std::to_string((int)pct) + "%";
    r_->drawText(pctStr,
        (float)(sw / 2), barY + barH + 18.f,
        fM, 1.f, 0.78f, 0.25f, 1.f, true);

    // Phase status message
    const char* status =
        (pct < 25.f)  ? "Initializing mission data..." :
        (pct < 55.f)  ? "Loading level and spawn data..." :
        (pct < 85.f)  ? "Pre-rendering map geometry..." :
                        "Finalizing...";

    r_->drawText(status,
        (float)(sw / 2), barY + barH + 44.f,
        fS, 0.55f, 0.6f, 0.45f, 1.f, true);
}
