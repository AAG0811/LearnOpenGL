# Code Following **[LearnOpenGL](https://learnopengl.com)**
---
A personal repository to store changes between different tutorial chapters following the tutorials.
![Wooden and Steel box with demonic symbol](/assets/ss/engine-ss.png)
**Currently**: You can move around in the world using W,A,S,D
Space to fly up
Shift to fly down
Q to quit
Escape to show cursor
Tab to hide the cursor
Mouse to look around
Scroll to Zoom
Use Settings Panel to configure lighting

Add models to the assets/models folder and replace 'pistol' with your model name in the following line in the main.cpp file (approximately line 119)
``` Model ourModel("../assets/models/pistol/pistol.obj"); ```
## Dependencies
> GLFW
> Zlib
> minizip

*The project contains prebuilt binaries for assimp for linux and macos, windows binaries will be added eventually*
---
## Build the code in this Repo
1. Create a directory with any name - e.g. '*build*' and navigate to that folder in the Terminal.
2. Using **CMake** generate the build-files in the folder you just created by running the command:
  > `cmake ..`
3. Build the files and run the program (named '*app*')
> ###  To Build with Unix Systems
> While following the tutorials I have been using **Make** to build my application
> Run `make` in the directory created to build the application and then run it using `./app`.
