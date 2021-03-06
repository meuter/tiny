# Intro

This project is my own "tiny" take on _thebennybox_'s 3D game engine tutorial series:

https://www.youtube.com/playlist?list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5

It is written entirely in C++11 with SDL/Glew/OpenGL, and has so far only been compiled with clang v3.5 and tested on an iMac with an ATI Radeon HD6970M.

# Compiling

Using homebrew, you can very easily bootstrap the compilation:

```
brew install scons glew sdl2 boost
```

Then:

```
scons
```

# Example

```c++
int main(int argc, char **argv)
{
	Game game(Window(1900, 1200, "MyGame", Window::OPENGL));	
	Context context(game.window());
	Scene scene;

	scene.addMesh("ground")
		.fromFiles("res/models/ground.obj", "res/models/ground.mtl")
		.moveTo(0,-2,0);

	scene.addMesh("box")
		.fromFiles("res/models/box.obj", "res/models/box.mtl")
		.attachTo(scene.getMesh("ground"))
		.moveTo(0,6,0);

	scene.addMesh("sphere")
		.fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl")
		.attachTo(scene.getMesh("ground"))
		.moveTo(0,2,0);

	scene.setAmbient(vec3(1,1,1) * 0.2f);	
	scene.camera()
		.withPerspective(toRadian(70), game.window().aspect(), 0.01f, 1000.0f)
		.moveTo(0,0,7)
		.aimAt(0,0,0);

	scene.addLight("d1").directional(vec3(1,1,1) * 0.7f, vec3(1,-1,1));
	scene.addLight("d2").directional(vec3(0,1,0) * 0.5f, vec3(-1,-1,-1));

	scene.addLight("p1").point(vec3(1,0,0) * 0.4f, vec3(1,-1.5f,1));
	scene.addLight("p2").point(vec3(0,1,0) * 0.6f, vec3(2,-1.5f,2));
	scene.addLight("p3").point(vec3(0,0,1) * 0.6f, vec3(3,-1.5f,3));

	scene.addLight("s1").spot(vec3(1,1,0) * 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1), 0.6f, 10);
	scene.addLight("s2").spot(vec3(0,1,1) * 1.0f, vec3(-3,-1.9,3), vec3(1,0,-1), 0.6f, 10);

 	context.vsync(false);			
 
 	game.add(FPSCounter());
 	game.add(WindowControl());
 	game.add(KeyboardControl(scene.camera()));
 	game.add(MouseControl(scene.camera()));
 	game.add(Renderer(context, scene));

 	game.start();
 	return EXIT_SUCCESS;
}
```


![alt tag](https://raw.github.com/meuter/tiny/master/res/pictures/snapshot.png)
