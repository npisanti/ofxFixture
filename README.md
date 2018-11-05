ofxFixtures
=====================================

Addon to use multiple DMX fixtures with stage simulation. Still a work in progress. Check the `example_head_template` to see how to make a class for your own moving head fixtures. 

It includes a `ofx::fixture::Manager` class that controls all the fixtures and has public objects inside to update many fixtures together, to show a really spartan 3d simulation and to store, load and transition between snapshots of the system.

![example](screenshot.jpg?raw=true "example")

### NOTES ON DEFAULT HEAD DEGREES
Default pan range runs from -180° to 180° clockwise as seen looking at the fixture from the bottom of the base towards the yoke, as if the fixture was hanging downwards underneath you. 0° is with the head centered.
Tilt range is -180° to 180°, 0° is with the head pointed directly away from base, positive is tilting towards the head display and negative away from it. So the fixture points fully forward when pan is set to 0° and tilt is set to -90°, if the head display is towards you.

So this kind of calibration works better with the fixture display pointed AWAY from the area you are wanting to cover with the head. The algorhytm for chasing the target tries to keep the tilt constant so when you are going outside the pan degrees it will rotate the head 360° to keep pointing at the target.

### DISCLAIMER: 
actually the chase target function works only with those values:
```
orientation.x = 0° or 180° 
orientation.y = 0° or from 45° to 360°
orientation.z = 0° 
```
It means that you can use it when your fixtures has the base on the floor or on the ceiling, but not fixed on the side. If you want to improve the code for chasing a target, i'm open to contributions.

### CREDITS:
The code for `ofx::fixture::Head` `panAngle()` and `tiltAngle()` is ported to `glm` from Shohei Fujimoto's [oF_movingLightHead_object](https://github.com/sfjmt/oF_movingLightHead_object).

### LICENSE
Nicola Pisanti, MIT License 2018
