ofxFixtures
=====================================

Addon to use multiple DMX fixtures with stage simulation. Currently a work in progress.


NOTES ON DEFAULT HEAD DEGREES

Default pan range runs from -180° to 180° clockwise as seen looking at the fixture from the bottom of the base towards the yoke, as if the fixture was hanging downwards underneath you. 0° is with the head centered.
Tilt range is -180° to 180°, 0° is with the head pointed directly away from base, positive is tilting towards the head display and negative away from it. So the fixture points fully forward when pan is set to 0° and tilt is set to -90°, if the head display is towards you.

So this kind of calibration works better with the fixture display pointed AWAY from the area you are wanting to cover with the head. The algorhytm for chasing the target tries to keep the tilt constant so when you are going outside the pan degrees it will rotate the head 360° to keep pointing at the target.
