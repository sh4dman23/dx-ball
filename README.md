# DX-BALL
![Main Menu](./preview-img/main-menu.png)

This project is a recreation of the iconic **DX-BALL** game, originally released in 1996. It was made using [raylib](https://www.raylib.com/) library in C.

## Table of Contents
- [Features](#features)
- [Installation and Usage](#installation-and-usage)
- [Credits](#credits)
- [License](#license)

## Features
### Basic Gameplay
<!-- add basic gameplay gif here (no perks) -->
In DX-BALL, you control the paddle at the bottom of the screen and use it to keep the moving ball from falling off the bottom, while trying to get as much score as possible by breaking the bricks. There are several [perks](#perks) that make the game easier or harder, depending on their effects. Your goal is to get as much points as possible while completing as many levels as you can. There are a total of **(TBD)** levels in the game as is, but it is possible to add upto a total of 1000 maps in the game, using the [map maker](#map-maker).

The ball angle is controlled by where it bounces off of the paddle, depending on the distance of the ball from the center of the paddle. The scoring depends largely on the speed of the ball when it hits a brick.

 The top 10 players with the highest scores can be found in the highscores screen.

### Bricks
There are 5 types of bricks in the game: 3 standard bricks (<img src="./assets/bricks/1.png" width="30" height="10" alt="Standard Brick 1">, <img src="./assets/bricks/2.png" width="30" height="10" alt="Standard Brick 2">, <img src="./assets/bricks/3.png" width="30" height="10" alt="Standard Brick 3">), indestructive bricks (<img src="./assets/bricks/-1.png" width="30" height="10" alt="Indestructive Brick">) and explosive bricks (<img src="./preview-img/explosive-brick.gif" width="30" height="10" alt="Explosive Brick">).

### Perks
<!-- add gameplay gif with perks -->
<!-- NOTE: edit number of perks when new ones added -->

At present, there are 10 different perks in the game with varying effects, listed below in no particular order:
- <img src="./assets/perks/killpaddle.png" width="16" height="15" alt="Kill Paddle"> Kill Paddle
- <img src="./assets/perks/extralife.png" width="16" height="15" alt="Extra Life"> Extra Life
- <img src="./assets/perks/doublepoints.png" width="16" height="15" alt="Double Points"> Double Points
- <img src="./assets/perks/expandpaddle.png" width="16" height="15" alt="Expand Paddle"> Expand Paddle
- <img src="./assets/perks/shrinkpaddle.png" width="16" height="15" alt="Shrink Paddle"> Shrink Paddle
- <img src="./assets/perks/slowball.png" width="16" height="15" alt="Slow Ball"> Slow Ball
- <img src="./assets/perks/fastball.png" width="16" height="15" alt="Fast Ball"> Fast Ball
- <img src="./assets/perks/laserpaddle.png" width="16" height="15" alt="Laser Paddle"> Laser Paddle
- <img src="./assets/perks/shrinkball.png" width="16" height="15" alt="Shrink Ball"> Shrink Ball
- <img src="./assets/perks/megaball.png" width="16" height="15" alt="Mega Ball"> Mega Ball

Perks like <img src="./assets/perks/shrinkpaddle.png" width="16" height="15" alt="Shrink Paddle">, <img src="./assets/perks/fastball.png" width="16" height="15" alt="Fast Ball"> and <img src="./assets/perks/shrinkball.png" width="16" height="15" alt="Shrink Ball"> which increase difficulty, also increase the score modifier in the game making riskier gameplay more rewarding.

### Map Maker

![Map Editor](./preview-img/map-editor.png)
There is an inbuilt map maker/editor in the game where the player can edit any map in the game and create upto 1000 maps, which will load in order when playing the game.

**Map Maker Controls:** For changing single bricks, use LMB and RMB to cycle through the brick types and MMB to reset a single brick. For selecting multiple bricks, hold CTRL and drag mouse cursor while holding LMB to select a region or press LMB while holding CTRL to select bricks one by one. Selected bricks can be modified by scrolling up/down or alternatively, using the up/down arrow keys.

## Installation and Usage

The project contains all library files required to run the game. Simply clone this repository onto your system and then use the build task in [tasks.json](./.vscode/tasks.json) to compile the game.

<!-- add release -->

## Credits and Acknowledgement
<!-- add credits for dev, art, music -->
This project was made by a team of two:

- Shadman Shahab ([Github](https://github.com/sh4dman23/) | [LinkedIn](https://www.linkedin.com/in/shadman-shahab/))
- Jawad Bin Mobin Akib ([Github](https://github.com/jbmakib) | [LinkedIn](https://www.linkedin.com/in/jbmakib/))

The following were used as reference for the game:
- [dx-ball.ru](https://dx-ball.ru/)
- [DX-Ball 2: A Complete Guide to Mechanics, Power-Ups and More](https://steamcommunity.com/sharedfiles/filedetails/?id=1573212121)

The art assets used in this game are credited to the following sources:
- [The Spriters Resource](https://www.spriters-resource.com/pc_computer/dxball)
- [thoth-tech/DXBallGame](https://github.com/thoth-tech/DXBallGame/tree/main/images)

The music used in this game are part of the original sound track of DX-BALL.

## License
