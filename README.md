# SPORTS CAR ON THE LAKEFRONT SCENE
This scene features three animations, one where the viewer pans across the lake alongside an orange sportscar while the car and the city scene behind it can be seen in the water's reflection in the foreground. Another animation features a stationary view in relation to the scene and cars driving by on both sides. The final mode features a closeup of the car on a reflective platform as if the car is on the polished show room floor. Press m to toggle between these animation modes.

In all animations the vehicles' speed can be decreased/increased using s/S. Excessive speeds in the FollowCar mode may lead to run-ins with the law! The posted speed limit is 80 MPH as you can see on the side of the road, but you usually don't get the attention of the police until you exceed this limit by quite a bit.

Part of the challenge for this was making the FollowCar loop imperceptable. In order to do this I had to redraw almost all of my objects since I made some poor choices in how I drew them earlier. I found the most useful thing to be setting a global unit length and then having every dimension of every object (almost every) be in multiples of that unit length. This enabled me to create a cityBlock function that draws a block of 8 buildings, a street and a cross street, and street lights. The water in the foreground is the same length as these city blocks, however I found that I needed to draw all waters AFTER drawing all city blocks or I would have some water sections blocking the reflection of the buildings at far angles. 

Reflections on the water are created by drawing each object that should be reflected once more, but with the y dimension inverted. The water object is slightly opaque making the reflection slightly washed out as it should be.

There is a fog mode available by pressing f. I would like to caveat this with the fact that on my machine (with my measly integrated graphics) the fog destroys my frame rate. I left the fog mode in because I am not sure if this would work with a decent graphics card or whether the problem is in my code. I think it really improves the scene by blending the further objects into the horizon more smoothly. I hope it works well on a machine with better capabilities.


## COMPILE
run: `make -f makefile`

## OPERATE
 *  m          Cycle though animation mode (FollowCar, InspectCar, StayPut)
 *  s/S        Decrease/increase Speed of movement
 *  f/F	       Toggle Fog on/off
 *  [ , ]         Lower/rise light
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  ESC        Exit

## ACKNOWLEDGEMENTS
- All textures that have been borrowed are being used under fair use
- asphalt texture from https://www.123rf.com/photo_97376423_asphalt-background-asphalt-texture-black-road-surface-.html
- sky map from https://learnopengl.com/Advanced-OpenGL/Cubemaps
- sidewalk texture from http://ixiqi.co/image
- car underside texture from https://opengameart.org/node/10581
- glass texture from https://www.pngkey.com/detail/u2q8y3o0a9y3i1r5_transparent-glass-texture-png-index-of-images-pattern/
- tread texture from https://www.michelintruck.com/tires-and-retreads/selector/#!/info/xty2
- wheel texture from https://www.schoolphotoproject.com/cars-motorbikes/car-tire-photo2.html
- building0 from http://www.passingloop.com/?p=410
- building1 from http://softwaremonster.info/?w=dryvit+stores+near
- building2 from http://texturelib.com/#!/category/?path=/Textures/buildings/buildings
- building3 from https://downloadtexture.com/textures-of-facades-of-office-buildings.html
- speed limit sign texture from https://en.wikipedia.org/wiki/Speed_limits_in_the_United_States
- Code for fog borrowed from from https://users.cs.jmu.edu/bernstdh/web/common/lectures/slides_opengl-fog.php
- Reflection idea borrowed from https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/dinoshade.c
- This project borrows from many of the course examples from http://www.prinmath.com/csci5229/Su19/programs/index.html including, but not necessary limited to ex: 10,13,14,15,19


