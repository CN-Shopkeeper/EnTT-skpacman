#pragma once

#include "core/maze.hpp"
#include "core/renderer.hpp"
#include "pch.hpp"
void RenderMap(Maze& maze);

void RenderPacman(entt::registry& reg, const size_t frame);

void RenderGhost(entt::registry& reg, const size_t frame);