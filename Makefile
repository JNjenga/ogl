breakout:
	g++ tests/breakout.cpp -I lib/glfw3/ -I lib/glm/ -I lib/glew/include/ -I lib/stb -I src -lGL -lglfw -lGLEW -o build/breakout
clean:
	rm -rf build/*
