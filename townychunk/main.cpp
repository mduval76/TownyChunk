#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"

int main() {
	Engine engine;
	engine.SetMaxFps(60);
	engine.Start("townychunk", WINDOW_WIDTH, WINDOW_HEIGHT, false);
}
