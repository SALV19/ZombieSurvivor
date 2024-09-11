#pragma once

#include <SFML/Graphics.hpp>
#include "Zombie.h"

using namespace sf;

int createBackground(VertexArray&, IntRect);
Zombie* createHorde(int numZombies, IntRect arena);
