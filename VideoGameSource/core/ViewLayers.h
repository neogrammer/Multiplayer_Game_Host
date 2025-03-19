#pragma once

enum class ViewLayers
{
	Back0, Back1, Back2, Behind, Player, Front, Foreground, HUD
};

// Back0 - Furthest back part of parallaxing map that can span 0 1 and 2, state will keep track of how many layers are used for its bg and if it scrolls or if it stays put ,only following the player movement
//   Back1 Back2
//  Behind - Decor in front of the bg in the scene but not interactible, garnish
// Player - layer where all the collidables exist
// Front - any decor in front of the player
// Foreground - Damage numbers, text in game etc.
// HUD - Heads up display Layer

// all Layers are views