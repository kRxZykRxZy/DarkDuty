#pragma once
#include "Types.h"
#include "Entity.h"
#include "Level.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cmath>

// ─── Renderer helper ──────────────────────────────────────────────────────────
class Renderer {
public:
    SDL_Renderer* sdl = nullptr;
    TTF_Font*     fontLarge  = nullptr;
    TTF_Font*     fontMedium = nullptr;
    TTF_Font*     fontSmall  = nullptr;

    void clear(SDL_Color c = Color::DARK_BG) {
        Color::set(sdl, c);
        SDL_RenderClear(sdl);
    }

    void present() { SDL_RenderPresent(sdl); }

    // ── Primitives ────────────────────────────────────────────────────────────
    void fillRect(int x, int y, int w, int h, SDL_Color c) {
        Color::set(sdl, c);
        SDL_Rect r{x, y, w, h};
        SDL_RenderFillRect(sdl, &r);
    }

    void drawRect(int x, int y, int w, int h, SDL_Color c) {
        Color::set(sdl, c);
        SDL_Rect r{x, y, w, h};
        SDL_RenderDrawRect(sdl, &r);
    }

    void drawLine(int x1, int y1, int x2, int y2, SDL_Color c) {
        Color::set(sdl, c);
        SDL_RenderDrawLine(sdl, x1, y1, x2, y2);
    }

    // Filled circle (midpoint algorithm)
    void fillCircle(int cx, int cy, int r, SDL_Color c) {
        Color::set(sdl, c);
        for (int dy = -r; dy <= r; ++dy) {
            int dx = (int)std::sqrt((float)(r*r - dy*dy));
            SDL_RenderDrawLine(sdl, cx - dx, cy + dy, cx + dx, cy + dy);
        }
    }

    void drawCircle(int cx, int cy, int r, SDL_Color c) {
        Color::set(sdl, c);
        int x = r, y = 0;
        int err = 0;
        while (x >= y) {
            SDL_RenderDrawPoint(sdl, cx+x, cy-y);
            SDL_RenderDrawPoint(sdl, cx+x, cy+y);
            SDL_RenderDrawPoint(sdl, cx-x, cy-y);
            SDL_RenderDrawPoint(sdl, cx-x, cy+y);
            SDL_RenderDrawPoint(sdl, cx+y, cy-x);
            SDL_RenderDrawPoint(sdl, cx+y, cy+x);
            SDL_RenderDrawPoint(sdl, cx-y, cy-x);
            SDL_RenderDrawPoint(sdl, cx-y, cy+x);
            if (err <= 0) { ++y; err += 2*y + 1; }
            if (err > 0)  { --x; err -= 2*x + 1; }
        }
    }

    // ── Text ──────────────────────────────────────────────────────────────────
    SDL_Texture* makeTextTexture(const std::string& text, TTF_Font* font,
                                  SDL_Color fg, int* w = nullptr, int* h = nullptr)
    {
        if (!font || text.empty()) return nullptr;
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), fg);
        if (!surf) return nullptr;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(sdl, surf);
        if (w) *w = surf->w;
        if (h) *h = surf->h;
        SDL_FreeSurface(surf);
        return tex;
    }

    void drawText(const std::string& text, int x, int y, TTF_Font* font,
                  SDL_Color color, bool centred = false)
    {
        if (!font || text.empty()) return;
        int tw = 0, th = 0;
        SDL_Texture* tex = makeTextTexture(text, font, color, &tw, &th);
        if (!tex) return;
        if (centred) x -= tw / 2;
        SDL_Rect dst{x, y, tw, th};
        SDL_RenderCopy(sdl, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }

    void drawTextShadow(const std::string& text, int x, int y, TTF_Font* font,
                        SDL_Color color, bool centred = false)
    {
        drawText(text, x+2, y+2, font, {0,0,0,180}, centred);
        drawText(text, x,   y,   font, color,        centred);
    }

    // ── Level tiles ───────────────────────────────────────────────────────────
    void drawLevel(const Level& lv, int camX, int camY, int screenW, int screenH) {
        int startTX = camX / TILE_SIZE;
        int startTY = camY / TILE_SIZE;
        int endTX   = startTX + screenW / TILE_SIZE + 2;
        int endTY   = startTY + screenH / TILE_SIZE + 2;

        for (int ty = startTY; ty < endTY; ++ty) {
            for (int tx = startTX; tx < endTX; ++tx) {
                TileType tile = lv.tileAt(tx, ty);
                int sx = tx * TILE_SIZE - camX;
                int sy = ty * TILE_SIZE - camY;

                switch (tile) {
                    case TileType::FLOOR: {
                        SDL_Color base = ((tx + ty) % 2 == 0) ? Color::FLOOR_A : Color::FLOOR_B;
                        fillRect(sx, sy, TILE_SIZE, TILE_SIZE, base);
                        break;
                    }
                    case TileType::WALL:
                        fillRect(sx, sy, TILE_SIZE, TILE_SIZE, Color::WALL);
                        // Draw brick lines
                        drawLine(sx, sy + TILE_SIZE/2, sx+TILE_SIZE, sy+TILE_SIZE/2,
                                 {30,32,28,255});
                        if ((ty % 2 == 0)) {
                            drawLine(sx+TILE_SIZE/2, sy, sx+TILE_SIZE/2, sy+TILE_SIZE/2,
                                     {30,32,28,255});
                        } else {
                            drawLine(sx+TILE_SIZE/4, sy+TILE_SIZE/2, sx+TILE_SIZE/4,
                                     sy+TILE_SIZE, {30,32,28,255});
                            drawLine(sx+3*TILE_SIZE/4, sy+TILE_SIZE/2, sx+3*TILE_SIZE/4,
                                     sy+TILE_SIZE, {30,32,28,255});
                        }
                        break;
                    case TileType::COVER:
                        fillRect(sx+2, sy+2, TILE_SIZE-4, TILE_SIZE-4, Color::COVER);
                        drawRect(sx+2, sy+2, TILE_SIZE-4, TILE_SIZE-4, {100,90,70,255});
                        // Highlight top
                        drawLine(sx+3, sy+3, sx+TILE_SIZE-5, sy+3, {130,115,90,255});
                        break;
                    case TileType::WATER:
                        fillRect(sx, sy, TILE_SIZE, TILE_SIZE, {20,40,100,200});
                        drawLine(sx, sy+TILE_SIZE/2, sx+TILE_SIZE, sy+TILE_SIZE/2,
                                 {40,80,160,150});
                        break;
                    case TileType::EXIT:
                        fillRect(sx, sy, TILE_SIZE, TILE_SIZE, {20,80,20,255});
                        drawRect(sx+2, sy+2, TILE_SIZE-4, TILE_SIZE-4, {0,255,0,200});
                        break;
                    default:
                        break;
                }
            }
        }
    }

    // ── Entity sprites ────────────────────────────────────────────────────────
    void drawPlayer(const Player& p, int camX, int camY, bool isLocalPlayer = true) {
        int sx = (int)p.pos.x - camX;
        int sy = (int)p.pos.y - camY;
        int r  = (int)Player::RADIUS;

        // Shadow
        fillCircle(sx+2, sy+2, r, {0,0,0,80});

        // Body
        SDL_Color bodyCol = isLocalPlayer ? Color::MIL_GREEN : Color::BLUE;
        if (p.invincTimer > 0.f) bodyCol = Color::WHITE;
        fillCircle(sx, sy, r, bodyCol);
        drawCircle(sx, sy, r, {200,200,200,200});

        // Vest detail
        fillCircle(sx, sy, r - 4, {(Uint8)(bodyCol.r*0.7f),(Uint8)(bodyCol.g*0.7f),
                                    (Uint8)(bodyCol.b*0.7f),200});

        // Direction indicator (gun barrel)
        float angle = p.facing;
        int   gx    = sx + (int)((r+6) * std::cos(angle));
        int   gy    = sy + (int)((r+6) * std::sin(angle));
        int   b1x   = sx + (int)(5 * std::cos(angle + 1.57f));
        int   b1y   = sy + (int)(5 * std::sin(angle + 1.57f));
        int   b2x   = sx + (int)(5 * std::cos(angle - 1.57f));
        int   b2y   = sy + (int)(5 * std::sin(angle - 1.57f));
        // Gun body
        Color::set(sdl, {60,60,60,255});
        SDL_Point pts[] = {
            {b1x, b1y}, {b2x, b2y},
            {gx+(int)(2*std::cos(angle-1.57f)), gy+(int)(2*std::sin(angle-1.57f))},
            {gx+(int)(2*std::cos(angle+1.57f)), gy+(int)(2*std::sin(angle+1.57f))},
            {b1x, b1y}
        };
        SDL_RenderDrawLines(sdl, pts, 5);
        // Barrel tip
        drawLine(sx, sy, gx, gy, {80,80,80,255});

        // Health bar
        if (!isLocalPlayer) {
            int barW = r * 2;
            int barX = sx - r;
            int barY = sy - r - 8;
            fillRect(barX, barY, barW, 4, Color::DARK_RED);
            int fill = (int)(barW * p.hp / (float)p.maxHp);
            fillRect(barX, barY, fill, 4, Color::MIL_GREEN);
        }
    }

    void drawEnemy(const Enemy& e, int camX, int camY) {
        int sx = (int)e.pos.x - camX;
        int sy = (int)e.pos.y - camY;
        int r  = (int)Enemy::RADIUS;

        if (e.state == AIState::DEAD) {
            // Death animation – fading pool
            float t = e.deathTimer / Enemy::DEATH_DURATION;
            Uint8 alpha = (Uint8)(180 * (1.f - t));
            fillCircle(sx, sy, r, {120, 0, 0, alpha});
            return;
        }

        // Shadow
        fillCircle(sx+2, sy+2, r, {0,0,0,80});

        SDL_Color col;
        switch (e.type) {
            case EnemyType::HEAVY:   col = {160,  20,  20, 255}; r += 4; break;
            case EnemyType::SNIPER:  col = {120,  20, 120, 255}; break;
            case EnemyType::RUSHER:  col = {200,  80,   0, 255}; r -= 2; break;
            default:                 col = {150,  30,  30, 255}; break;
        }

        // Alert state flash
        if (e.state == AIState::ATTACK) {
            static float t = 0.f; t += 0.1f;
            if ((int)(t*6) % 2 == 0) col = {255,60,60,255};
        }

        fillCircle(sx, sy, r, col);
        drawCircle(sx, sy, r, {200, 80, 80, 200});

        // Health bar
        int barW = r * 2;
        int barX = sx - r;
        int barY = sy - r - 8;
        fillRect(barX, barY, barW, 4, {60,0,0,200});
        int fill = (int)(barW * e.hp / (float)e.maxHp);
        fillRect(barX, barY, fill, 4, Color::RED);

        // State icon
        if (e.state == AIState::ALERT || e.state == AIState::ATTACK) {
            drawText("!", sx - 3, sy - r - 16, fontSmall, Color::AMBER);
        }
    }

    void drawBullet(const Bullet& b, int camX, int camY) {
        int sx = (int)b.pos.x - camX;
        int sy = (int)b.pos.y - camY;
        SDL_Color c = b.fromPlayer ? Color::YELLOW : Color::ORANGE;
        fillCircle(sx, sy, (int)b.radius, c);
        // Tracer line
        Vec2 trail = b.vel * (-0.025f);
        drawLine(sx, sy, sx+(int)trail.x, sy+(int)trail.y,
                 {c.r, c.g, c.b, 100});
    }

    void drawExplosion(const Explosion& ex, int camX, int camY) {
        int sx = (int)ex.pos.x - camX;
        int sy = (int)ex.pos.y - camY;
        float t = ex.age / ex.lifetime;
        SDL_Color inner  = Color::lerp({255,200, 50,200}, {200,50,  0,  0}, t);
        SDL_Color outer  = Color::lerp({200, 80,  0,150}, {50, 50, 50,  0}, t);
        int maxR = (int)ex.radius;
        fillCircle(sx, sy, maxR,       outer);
        fillCircle(sx, sy, maxR * 2/3, inner);
    }

    // ── Minimap ───────────────────────────────────────────────────────────────
    void drawMinimap(const Level& lv, const Player& player,
                     const std::vector<Enemy>& enemies,
                     int screenW, int screenH)
    {
        const int MAP_W  = 160, MAP_H = 100;
        const int MAP_X  = screenW - MAP_W - 10;
        const int MAP_Y  = 10;
        const float scaleX = MAP_W / (float)lv.pixelWidth();
        const float scaleY = MAP_H / (float)lv.pixelHeight();

        // Background
        SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_BLEND);
        fillRect(MAP_X-2, MAP_Y-2, MAP_W+4, MAP_H+4, {0,0,0,220});
        drawRect(MAP_X-2, MAP_Y-2, MAP_W+4, MAP_H+4, Color::AMBER);

        // Tiles (simplified)
        for (int ty = 0; ty < lv.rows; ++ty) {
            for (int tx = 0; tx < lv.cols; ++tx) {
                if (lv.tiles[ty][tx] == TileType::WALL) {
                    int mx = MAP_X + (int)(tx * TILE_SIZE * scaleX);
                    int my = MAP_Y + (int)(ty * TILE_SIZE * scaleY);
                    int mw = std::max(1, (int)(TILE_SIZE * scaleX));
                    int mh = std::max(1, (int)(TILE_SIZE * scaleY));
                    fillRect(mx, my, mw, mh, Color::WALL);
                }
            }
        }

        // Enemies
        for (const auto& e : enemies) {
            if (!e.isAlive() || !e.spawned) continue;
            int mx = MAP_X + (int)(e.pos.x * scaleX);
            int my = MAP_Y + (int)(e.pos.y * scaleY);
            fillRect(mx-1, my-1, 3, 3, Color::RED);
        }

        // Player
        int px = MAP_X + (int)(player.pos.x * scaleX);
        int py = MAP_Y + (int)(player.pos.y * scaleY);
        fillCircle(px, py, 3, Color::CYAN);

        // Exit points
        for (const auto& ep : lv.exitPoints) {
            int ex2 = MAP_X + (int)(ep.x * scaleX);
            int ey2 = MAP_Y + (int)(ep.y * scaleY);
            fillRect(ex2-2, ey2-2, 4, 4, {0,255,0,255});
        }

        SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_NONE);
    }

    // ── HUD ───────────────────────────────────────────────────────────────────
    void drawHUD(const Player& p, const Objective& obj,
                 int screenW, int screenH, float waveTimer = 0.f, int currentWave = 0,
                 int totalWaves = 0)
    {
        SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_BLEND);

        // ── Bottom HUD bar ──────────────────────────────────────────────────
        const int HUD_H = 80;
        fillRect(0, screenH - HUD_H, screenW, HUD_H, Color::HUD_BG);
        drawLine(0, screenH - HUD_H, screenW, screenH - HUD_H, Color::AMBER);

        // Health
        const int HP_W = 200, HP_H = 18;
        int hpX = 12, hpY = screenH - HUD_H + 12;
        drawText("HEALTH", hpX, hpY, fontSmall, Color::LIGHT_GRAY);
        hpY += 16;
        fillRect(hpX, hpY, HP_W, HP_H, {40,0,0,255});
        int hpFill = (int)(HP_W * p.hp / (float)p.maxHp);
        SDL_Color hpCol = (p.hp > 60) ? Color::MIL_GREEN :
                          (p.hp > 30) ? Color::AMBER      : Color::RED;
        fillRect(hpX, hpY, hpFill, HP_H, hpCol);
        drawRect(hpX, hpY, HP_W, HP_H, Color::AMBER);
        drawTextShadow(std::to_string(p.hp) + "/" + std::to_string(p.maxHp),
                       hpX + HP_W/2, hpY + 1, fontSmall, Color::WHITE, true);

        // Weapon info
        if (!p.weapons.empty()) {
            const Weapon& w = p.weapon();
            int wx = 260;
            int wy = screenH - HUD_H + 8;
            drawText(w.def.name, wx, wy, fontMedium, Color::AMBER);
            wy += 24;
            // Ammo bar
            std::string ammoStr = std::to_string(w.ammoInMag) + " / " +
                                  std::to_string(w.reserveAmmo);
            drawText(ammoStr, wx, wy, fontSmall, Color::WHITE);
            wy += 2;
            // Reload flash
            if (w.isReloading) {
                float prog = 1.f - (w.reloadTimer / w.def.reloadTime);
                fillRect(wx, wy + 16, (int)(80 * prog), 4, Color::AMBER);
                drawRect(wx, wy + 16, 80, 4, Color::GRAY);
                drawText("RELOADING", wx + 88, wy + 12, fontSmall, Color::AMBER);
            }
        }

        // Weapon slots
        {
            int slotX = 500;
            int slotY = screenH - HUD_H + 8;
            for (int i = 0; i < (int)p.weapons.size(); ++i) {
                bool sel = (i == p.currentWeapon);
                SDL_Color bg = sel ? Color::MIL_GREEN : SDL_Color{20,20,20,200};
                fillRect(slotX + i*60, slotY, 56, 28, bg);
                drawRect(slotX + i*60, slotY, 56, 28, sel ? Color::AMBER : Color::GRAY);
                drawText(std::to_string(i+1), slotX + i*60 + 4, slotY + 2,
                         fontSmall, sel ? Color::WHITE : Color::GRAY);
                // Shortened weapon name
                std::string wn = p.weapons[i].def.name;
                if (wn.size() > 4) wn = wn.substr(0,4);
                drawText(wn, slotX + i*60 + 14, slotY + 2, fontSmall,
                         sel ? Color::AMBER : Color::GRAY);
            }
        }

        // ── Objective (top-left) ────────────────────────────────────────────
        fillRect(0, 0, 360, 36, Color::HUD_BG);
        drawLine(0, 36, 360, 36, Color::AMBER);
        SDL_Color objCol = obj.complete ? Color::MIL_GREEN : Color::AMBER;
        std::string objText = (obj.complete ? "[DONE] " : "[OBJ] ") + obj.label;
        drawTextShadow(objText, 8, 8, fontSmall, objCol);

        // Wave timer (mission 3)
        if (totalWaves > 0) {
            int wy2 = 42;
            drawText("WAVE " + std::to_string(currentWave) + "/" +
                     std::to_string(totalWaves), 8, wy2, fontSmall, Color::ORANGE);
            if (waveTimer > 0.f) {
                drawText("Next wave: " + std::to_string((int)waveTimer+1) + "s",
                         8, wy2+16, fontSmall, Color::AMBER);
            }
        }

        SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_NONE);
    }

    // ── Crosshair ─────────────────────────────────────────────────────────────
    void drawCrosshair(int mx, int my, float spread) {
        int gap = 8 + (int)(spread * 120.f);
        int len = 10;
        SDL_Color c = {0, 255, 0, 220};
        // Outer cross
        drawLine(mx - gap - len, my, mx - gap, my, c);
        drawLine(mx + gap,       my, mx + gap + len, my, c);
        drawLine(mx, my - gap - len, mx, my - gap, c);
        drawLine(mx, my + gap,       mx, my + gap + len, c);
        // Centre dot
        fillRect(mx - 1, my - 1, 3, 3, {0, 255, 0, 200});
    }

    // ── Semi-transparent overlay ──────────────────────────────────────────────
    void drawOverlay(SDL_Color c, int screenW, int screenH) {
        SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_BLEND);
        fillRect(0, 0, screenW, screenH, c);
        SDL_SetRenderDrawBlendMode(sdl, SDL_BLENDMODE_NONE);
    }
};
