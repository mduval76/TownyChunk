#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"

int main() {
	Engine engine;
	engine.SetMaxFps(60);
	engine.Start("townychunk", 1200, 900, false);
}
