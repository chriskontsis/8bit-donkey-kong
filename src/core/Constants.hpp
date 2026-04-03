#pragma once

namespace Constants
{

// --- Window
constexpr int WINDOW_WIDTH{768};
constexpr int WINDOW_HEIGHT{672};

// --- Physics
constexpr float GRAVITY{900.0f};          // px/s^2
constexpr float MARIO_RUN_SPEED{130.0f};  // px/s
constexpr float MARIO_JUMP_VY{-400.0f};   // px/s (neg = up)
constexpr float MARIO_CLIMB_SPD{90.0f};   // px/s on ladders

// --- Entity sizes
constexpr float MARIO_W{24.0f};
constexpr float MARIO_H{30.0f};
constexpr float DK_W{64.0f};
constexpr float DK_H{64.0f};
constexpr float BARREL_W{20.0f};
constexpr float BARREL_H{20.0f};
constexpr float PAULINE_W{20.0f};
constexpr float PAULINE_H{30.0f};
constexpr float FIRE_W{20.0f};
constexpr float FIRE_H{20.0f};

// ── Gameplay
constexpr int   MARIO_START_LIVES{3};
constexpr float BARREL_ROLL_SPEED{110.0f};
constexpr float DK_BARREL_INTERVAL{2.8f};    // seconds between
constexpr float DEATH_DELAY{1.5f};           // seconds before respawn
constexpr float LEVEL_COMPLETE_DELAY{2.5f};  // pause before

// ── Scoring
constexpr int POINTS_LEVEL_CLEAR{1000};
constexpr int POINTS_RIVET{200};

// ── Timing
constexpr int   TARGET_FPS{60};
constexpr float FIXED_DT{1.0f / TARGET_FPS};  // ~0.01667s
constexpr float MAX_ACCUM{0.1f};              // cap to prevent spiral

}  // namespace Constants