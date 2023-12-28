## Running instructions

There are a few different Third Party libraries you have to build

Make sure that you compile and build 

```
freetype
assimp
```

For the assimp build, you will then need to grab the config.h that is generated in ThirdParty/assimp/build/include/assimp and bring it up to the ThirdParty/assimp/include/assimp folder. Just copy and paste it in


You will also need to install SDL2. If you are on windows, that is the most complicated:

```
Download SDL2 from here https://www.libsdl.org/
Extract the package into a directory
In the CMake GUI, you will need to set the variable SDL2_DIR to the cmake of that extracted folder, wherever that may be
It should generate and build correctly after that point! :)
If you are on windows, you will also need to grab the dll from the lib/x64 folder (SDL2.dll) and copy it into the build folder as well (Until I write something to automatically do this)
```

You will need to do the same thing with SDL2_mixer, following the same instructions as before, as well as copying the DLL into the build folder