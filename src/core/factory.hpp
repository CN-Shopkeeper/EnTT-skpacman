#pragma once

#include "pch.hpp"
#include "texture.hpp"

entt::entity MakePacman(entt::registry &, std::array<Image, 2> images);
entt::entity MakeBlinky(entt::registry &, Image image,
                        entt::entity);
entt::entity MakePinky(entt::registry &, Image image,
                       entt::entity);
entt::entity MakeInky(entt::registry &, Image image, entt::entity,
                      entt::entity);
entt::entity MakeClyde(entt::registry &, Image image,
                       entt::entity);