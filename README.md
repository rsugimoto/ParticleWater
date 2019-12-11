# Particle Water
HKUST COMP5411 Advanced Computer Graphics Project

This is my implementation of the Smoothed Particle Hydrodynamics method with OpenGL, for HKUST COMP5411 Advanced Computer Graphics final project. The focus of this project is more on the simulation itself. If time allows, I may work on improving the rendering. The method allows us to compute a numerical solution to the Navier-Stokes equations by discritizing the problem using particles.

## Requirements:
Linux Environment with OpenGL 4.3 or above compatibility, with GLUT installed  
Tested on Ubuntu 18.04 with Nvidia GeForce RTX 2070 with driver version 430

## Compilation
```Bash
> git clone https://github.com/rsugimoto/ParticleWater.git
> cd ParticleWater
> mkdir build
> cd build
> cmake ..
> make
```

## References:
Takahiro Harada, Seiichi Koshizuka, and Yoichiro Kawaguchi. 2007. Smoothed ParticleHydrodynamics on GPUs.Computer Graphics International(01 2007).  
Matthias Müller, David Charypar, and Markus Gross. 2003. Particle-Based FluidSimulation for Interactive Applications.Fluid Dynamics2003, 154–159.  
Solenthaler, Barbara. 2010. Incompressible fluid simulation and advanced surface handling with SPH. 10.5167/uzh-29724. 
