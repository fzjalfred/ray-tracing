# Compilation
In A1 folder,
```
premake4 gmake
make
```
boot ``` ./A2 ```

# Manual
Follow assignment specifications.

new added files:
Cube.hpp
utils.hpp

For ViewPort mode,

draw a square from top-left to bottom-right to avoid horizontal flip.


For initial configuration,

near plane is at z = 1.0f, maximum at 10.0f, far plane at z = -2.0f, minimum at -20.0f.

The perspective matrix only give the situation when we look down z-axis.