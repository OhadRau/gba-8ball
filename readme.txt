Ball in Hand
============

This game is a simplified implementation of 8ball pool made for a single player.
While there's no AI to play against, you're encouraged to try to win in as few turns as possible and beat your record!

Some of the rules are simplified, for example scratching resets the ball to the original position (rather than ball in hand, ironically).
There are no colors assigned since there's only one player, so you just try to get the most balls in before the eight ball.

Controls
--------

LEFT - Rotate cue clockwise
RIGHT - Rotate cue counterclockwise
A - Hold to pull back the cue, release when you're ready to take your shot
SELECT - Restart the game (works at any point during gameplay)

Features
--------

* Score + turn counters displayed at the top of the screen
* Sprite-based graphics, with affine sprites for dynamic sprite rotation
* Physically accurate dynamic collisions (i.e. balls bounce correctly)
* Randomized racks every time you break