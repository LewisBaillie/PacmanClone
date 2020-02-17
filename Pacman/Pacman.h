#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

//Included header for input files
#include <fstream>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanRight;
	Rect* _pacmanRightOpen;
	Rect* _pacmanLeft;
	Rect* _pacmanLeftOpen;
	Rect* _pacmanDown;
	Rect* _pacmanDownOpen;
	Rect* _pacmanUp;
	Rect* _pacmanUpOpen;
	Texture2D* _pacmanTexture;
	const float _cPacmanSpeed;

	

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieBigTexture;

	// Position for String
	Vector2* _stringPosition;

	// Data for Pause Screen
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _pauseStringPosition;
	bool _paused;
	bool _pauseKeyDown;

	// Data for Start Screen
	Vector2* _startStringPosition;
	Vector2* _h2pStringPosition;
	Vector2* _playcmdStringPosition;
	bool _started;

	// Data for Game Over Screen
	Vector2* _gameOverStringPosition;
	Vector2* _playAgainStringPosition;
	
	bool _playAgain;

	// Game Logic Variables
	bool _movementAllowed;
	char _direction;
	int _pacmanLeftSide;
	int _pacmanTop;
	int _pacmanRightSide;
	int _pacmanBottom;
	bool _xCollide;
	bool _yCollide;
	bool _isColliding;
	int _score;
	int _pelletsEaten;
	Vector2* _scoreStringPosition;
	bool _levelEnd;
	int _level;
	bool _gameOver;
	Vector2* _levelEndStringPosition;
	int _lives;

	// Data to represent Ghosts
	Vector2* _blueGhostPosition;
	Vector2* _pinkGhostPosition;
	Vector2* _redGhostPosition;
	Vector2* _orangeGhostPosition;
	Vector2* _blueGhostSpawn;
	Vector2* _pinkGhostSpawn;
	Vector2* _redGhostSpawn;
	Vector2* _orangeGhostSpawn;
	Vector2* _topLeftCorner;
	Rect* _blueGhost;
	Rect* _pinkGhost;
	Rect* _redGhost;
	Rect* _orangeGhost;
	Texture2D* _ghostSpriteSheet;
	Texture2D* _ghost2SpriteSheet;
	char _blueDirection;
	char _orangeDirection;
	char _pinkDirection;
	char _redDirection;
	bool _ghostsStarted;
	bool _xCollideGhost;
	bool _yCollideGhost;
	bool _isCollidingGhost = false;
	bool _changeDirection;
	int _orangeSteps;

	// Map Data
	
	char _nextChar;
	ifstream _mapTxt;
	Texture2D* _horizWall;
	Texture2D* _verticWall;
	Texture2D* _cornerWall;
	Texture2D* _wall;
	Texture2D* _gap;
	Texture2D* _cherry;
	Vector2* _pacmanSpawn;
	bool _isCherry;
	Rect* _wallRect;
	char _arrayPos;
	Vector2* _pos;
	Vector2* _center;
	Rect* _cherryRect;
	char _tiles[33][25];

	//Sound Section
	SoundEffect* eatSound;
	

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void virtual _blueAI(int elapsedTime);

	void virtual _redAI(int elapsedTime);

	void virtual _orangeAI(int elapsedTime);

	void virtual _pinkAI(int elapsedTime);

	void virtual _orangeStart(int elapsedTime);

	void virtual _blueStart(int elapsedTime);

	void virtual _pinkStart(int elapsedTime);

	void virtual _redStart(int elapsedTime);

};