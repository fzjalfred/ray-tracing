# Compilation
In A3 folder,
```
cd A4
premake4 gmake
make
```

boot 
``` 
cd Assets
../A4 simple.lua 
```

# Manual
Follow assignment specifications.

I implemented mirror reflection as my extra feature.

The mirror reflection and bounding volumes rendering macros are defined in A4.hpp, Mesh.hpp

The background is gradient from top-right to down-left.

In the **root** directory, sample.png, screenshot.png is rendered from Assests/sample.lua with mirror reflection. Sample2.png is rendered from Assests/nonhier.png with mirror reflection. 
They both satisfy the sample requirements including at least one of each required primitive type, at least one point light source, at least one shiny surface, a non-trivial background, and also mirror reflection as extra feature.


In the **Assets** directory,
For simple.png, hier.png, nonhier.png, mirror reflection is on.

For others, mirror reflection is off.

macho-cows-bb.png is provided for RENDER_BOUNDING_VOLUMES.

I have my A4 run in Machine gl47.

Noticed that I made serveral obj file with Blender v3.3.1.

