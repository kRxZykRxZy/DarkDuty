#pragma once
// Forward declare Renderer2D to avoid circular dependency
class Renderer2D;

namespace DrawPrimitives {
// Immediate-mode 2D drawing helpers – push colored quads/lines to the batch renderer.
// All draw calls require a valid Renderer2D that has beginBatch() already called.

// (Actual drawing is handled directly by Renderer2D::drawRect / drawLine etc.)
// This namespace provides thin aliases for clarity in game code.

} // namespace DrawPrimitives
