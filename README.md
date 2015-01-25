Fairies? Why did it have to be Fairies?!?
=============================================

This game was created for the [Global Game Jam 2015](http://globalgamejam.org), at the [Miami University](http://MiamiOH.edu) site. Visit the official [GGJ game site](http://globalgamejam.org/2015/games/fairies-why-did-it-have-be-fairies) for more info.

The intention was to create a simple game that demonstrates the capabilities of Google Project Tango.

Background
---------

You are a mighty barbarian warrior, who has just conquered a dragon. A really big one. You got stomped pretty bad. 
You are badly wounded, and out of energy. So it is unlucky that, in taking a load off your feet, you sat on a fairy flower.
As vengeful flower fairies begin to magically appear, intent to take a bite out of you, you think "Oh wonderful. [What do I do now?](https://www.youtube.com/watch?v=N1W5VxdNyNk)"

Gameplay
-------

Look around the room with your Google Project Tango tablet. Eventually fairies (little pink clouds) will start to magically arrive. You have 5 seconds to get close (fairy will turn white) and tap the tablet to "squash" the fairy. When the fairy turns green your only hope is to dodge out of the way.

Technical details
------------------

We use both the location tracking and point cloud technologies of Project Tango to create a game that is immersive and uses the real world in an interesting way.

We use the point cloud to identy flat surfaces, and to detect the extents of the room where you are playing the game. Because of this, fairies should spawn only on top of flat surfaces that you have seen while playing the game.

We use the position tracking to align the point cloud with the world, which allows us to build a heigh map of the real world, to identify possible spawn points for fairies. We also use it to track the player's location in space. This allows us to detect when the user is close enough to a fairy to squash it, and to detect when a fairy is dodged successfully.

Credits
==============

This code started as a fork of [Google's Tango Examples](https://github.com/googlesamples/tango-examples-c) and is used under the Apache License 2.0, and I have chosen to release my code under that license as well.


