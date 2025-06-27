# splinekit
Toolkit for generating arbitrary splines with maximum controls

# Features
- Arbitrarily ordered splines (For higher order control)
- Arbitrary parameterization (For controling motion)
- Arbitrary dimensions (For any degree of freedom and auxiliary info)
- *Parameter limits with dimensional filtering (For physically limited systems with non-physical attributes)
- *Adaptive sampling (For consistent detail and higher performance)
- *Spline length estimation
- *Extrema calculations
- *Variable spline duration
- *Spline translator / approximator
- Presets for common and basic splines
- *Inverse parameterization
- *Basic collisions
- *Sample spline editor
- *2D version of all of the above for surfaces

*work in progress

# Building
To use splinekit, either compile everything in "/src" manually or use the commands:
``` bash
make init
make build
```
This will create a lib folder, which will contain splinekit.a. Statically link against this archive.
Include the include folder for header files. Include the header file in code:
``` c++
#include "splinekit/Curve.hpp"
```

# Getting Started


# Examples
