#include "Pacman.h"
#include <sstream>
#include <iostream>


Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f)
{
	_frameCount = 0;
	_paused = false;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 780, false, 0, 0, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}
Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanRight;
	delete _pacmanRightOpen;
	delete _pacmanLeft;
	delete _pacmanLeftOpen;
	delete _pacmanDown;
	delete _pacmanDownOpen;
	delete _pacmanUp;
	delete _pacmanUpOpen;
	delete _munchieBlueTexture;
	delete _munchieBigTexture;
	delete _munchieRect;
	delete _menuBackground;
	delete _menuRectangle;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(15*32.0f , 14*32.0f);
	_pacmanRight = new Rect(0.0f, 0.0f, 31, 31);
	_pacmanRightOpen = new Rect(32.0f, 0.0f, 31, 31);
	_pacmanLeft = new Rect(0.0f, 64.0f, 31, 31);
	_pacmanLeftOpen = new Rect(32.0f, 64.0f, 31, 31);
	_pacmanDown = new Rect(0.0f, 32.0f, 31, 31);
	_pacmanDownOpen = new Rect(32.0f, 32.0f, 31, 31);
	_pacmanUp = new Rect(0.0f, 96.0f, 31, 31);
	_pacmanUpOpen = new Rect(32.0f, 96.0f, 31, 31);
	_pacmanSpawn = new Vector2(15 * 32.0f, 14 * 32.0f);
	
	// Load Ghosts
	_ghostSpriteSheet = new Texture2D();
	_ghostSpriteSheet->Load("Textures/Ghosts.png", false);
	_ghost2SpriteSheet = new Texture2D();
	_ghost2SpriteSheet->Load("Textures/Ghosts2.png", false);
	_redGhost = new Rect(0.0f, 0.0f, 31, 31);
	_pinkGhost = new Rect(32.0f, 0.0f, 31, 31);
	_orangeGhost = new Rect(0.0f, 32.0f, 31, 31);
	_blueGhost = new Rect(32.0f, 32.0f, 31, 31);
	_redGhostSpawn = new Vector2(14 * 32.0f, 10 * 32.0f);
	_blueGhostSpawn = new Vector2(14 * 32.0f, 12 * 32.0f);
	_pinkGhostSpawn = new Vector2(16 * 32.0f, 10 * 32.0f);
	_orangeGhostSpawn = new Vector2(16 * 32.0f, 12 * 32.0f);
	_redGhostPosition = _redGhostSpawn;
	_blueGhostPosition = _blueGhostSpawn;
	_orangeGhostPosition = _orangeGhostSpawn;
	_pinkGhostPosition = _pinkGhostSpawn;
	_orangeSteps = 0;
	_ghostsStarted = false;
	


	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieBigTexture = new Texture2D();
	_munchieBigTexture->Load("Textures/MunchieBig.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	

	// Set Pause Screen Parameters
	_pauseStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_pauseKeyDown = false;

	// Set Start Screen Parameters
	_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 3.0f, Graphics::GetViewportHeight() / 3.0f);
	_h2pStringPosition = new Vector2(Graphics::GetViewportWidth() / 3.0f, Graphics::GetViewportHeight() / 2.0f);
	_playcmdStringPosition = new Vector2(Graphics::GetViewportWidth() / 3.0f, (Graphics::GetViewportHeight() * 2) / 3.0f);
	_started = false;

	// Set Game Over Screen Parameters
	_gameOverStringPosition = new Vector2(Graphics::GetViewportWidth() / 3.0f, Graphics::GetViewportHeight() / 3.0f);
	_playAgainStringPosition = new Vector2(Graphics::GetViewportWidth() / 3.0f, Graphics::GetViewportHeight() / 2.0f);
	_levelEndStringPosition = new Vector2(Graphics::GetViewportWidth() / 3.0f, Graphics::GetViewportHeight() / 3.0f);
	_playAgain = false;
	_levelEnd = false;
	_level = 1;
	_gameOver = false;

	// Set Game Logic Defaults
	_movementAllowed = true;
	_direction = 'R';
	_isColliding = false;
	_scoreStringPosition = new Vector2((Graphics::GetViewportWidth()/2.0f) - 128.0f, 26.0f);
	_score = 0;
	_lives = 3;



	// Load Map Data
	_wallRect = new Rect(0, 0, 32.0f, 32.0f);
	_cherry = new Texture2D();
	_cherry->Load("CherryImg.png", false);
	_cherryRect = new Rect(0.0f, 0.0f, 31, 31);
	_verticWall = new Texture2D();
	_verticWall->Load("Textures/verticWall.png", false);
	_horizWall = new Texture2D();
	_horizWall->Load("Textures/horizWall.png", false);
	_cornerWall = new Texture2D();
	_cornerWall->Load("Textures/cornerWall.png", false);
	_wall = new Texture2D();
	_center = new Vector2(-8.0f, -8.0f);
	_gap = new Texture2D();
	_gap->Load("Textures/Gap.png", false);
	_topLeftCorner = new Vector2(3 * 32.0f, 3 * 32.0f);
	
	_isCherry = false;

	_mapTxt.open("Maps/firstLevel.txt");



	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j <= 32; j++)
		{
			_mapTxt.get(_nextChar);
			_tiles[j][i] = _nextChar;
		}
	}

	_mapTxt.close();

	// Sound Effect Data
	eatSound = new SoundEffect();
	eatSound->Load("Sounds/nom.wav");

}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	_pacmanBottom = _pacmanPosition->Y + 25.0f;
	_pacmanLeftSide = _pacmanPosition->X + 7.0f;
	_pacmanTop = _pacmanPosition->Y + 7.0f;
	_pacmanRightSide = _pacmanPosition->X + 25.0f;

	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE))
	{
		exit(0);
	}


	//
	// Checks for Game Start
	//

	if (!_started)
	{
		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
			_started = true;
	}

	else if (_levelEnd)
	{
		if (keyboardState->IsKeyDown(Input::Keys::Y))
		{
			Sleep(2000);
			_gameOver = false;
			_started = true;
			_level++;
			_levelEnd = false;
			_pacmanPosition = _pacmanSpawn;

			_mapTxt.open("Maps/firstLevel.txt");

			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j <= 32; j++)
				{
					_mapTxt.get(_nextChar);
					_tiles[j][i] = _nextChar;
				}
			}
			_mapTxt.close();
		}

		else if (keyboardState->IsKeyDown(Input::Keys::N))
		{
			_gameOver = true;
			_levelEnd = false;

		}
	}


	else if (_gameOver)
	{
		if (keyboardState->IsKeyDown(Input::Keys::Y))
		{
			_gameOver = false;
			_started = false;
			_score = 0;
			_lives = 3;

			_mapTxt.open("Maps/firstLevel.txt");

			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j <= 32; j++)
				{
					_mapTxt.get(_nextChar);
					_tiles[j][i] = _nextChar;
				}
			}

			_mapTxt.close();
		}
	}

	else if (_started == true && _levelEnd != true && _gameOver != true)
	{


		//
		// Handles Pause Toggle
		//


		if ((keyboardState->IsKeyDown(Input::Keys::P)) && !_pauseKeyDown)
		{
			_pauseKeyDown = true;
			_paused = !_paused;
		}

		if (keyboardState->IsKeyUp(Input::Keys::P))
			_pauseKeyDown = false;


		//
		// End of Pause Toggle
		//


		// Only allows for game updates when not paused
		if (!_paused)
		{
			//
			// Handles Movement
			//

			_xCollide = (_pacmanLeftSide + 1 <= _blueGhostPosition->X + 31) && (_pacmanRightSide - 1 >= _blueGhostPosition->X + 1);
			_yCollide = (_pacmanTop + 1 <= _blueGhostPosition->Y + 31) && (_pacmanBottom - 1 >= _blueGhostPosition->Y + 1);

			if (((_pacmanLeftSide + 1 <= _blueGhostPosition->X + 31) && (_pacmanRightSide - 1 >= _blueGhostPosition->X + 1) && (_pacmanTop + 1 <= _blueGhostPosition->Y + 31) && (_pacmanBottom - 1 >= _blueGhostPosition->Y + 1)) || ((_pacmanLeftSide + 1 <= _redGhostPosition->X + 31) && (_pacmanRightSide - 1 >= _redGhostPosition->X + 1) && (_pacmanTop + 1 <= _redGhostPosition->Y + 31) && (_pacmanBottom - 1 >= _redGhostPosition->Y + 1)) || ((_pacmanLeftSide + 1 <= _pinkGhostPosition->X + 31) && (_pacmanRightSide - 1 >= _pinkGhostPosition->X + 1) && (_pacmanTop + 1 <= _pinkGhostPosition->Y + 31) && (_pacmanBottom - 1 >= _pinkGhostPosition->Y + 1)) || ((_pacmanLeftSide + 1 <= _orangeGhostPosition->X + 31) && (_pacmanRightSide - 1 >= _orangeGhostPosition->X + 1) && (_pacmanTop + 1 <= _orangeGhostPosition->Y + 31) && (_pacmanBottom - 1 >= _orangeGhostPosition->Y + 1)))
			{
				_lives -= 1;
				_redGhostSpawn = new Vector2(14 * 32.0f, 10 * 32.0f);
				_blueGhostSpawn = new Vector2(14 * 32.0f, 12 * 32.0f);
				_pinkGhostSpawn = new Vector2(16 * 32.0f, 10 * 32.0f);
				_orangeGhostSpawn = new Vector2(16 * 32.0f, 12 * 32.0f);
				_pacmanPosition = _pacmanSpawn;
				_blueGhostPosition = _blueGhostSpawn;
				_redGhostPosition = _redGhostSpawn;
				_pinkGhostPosition = _pinkGhostSpawn;
				_orangeGhostPosition = _orangeGhostSpawn;
				_ghostsStarted = false;
				_orangeSteps = 0;
			}

			if (_lives == 0)
			{
				_gameOver = true;
			}





			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j <= 32; j++)
				{
					if (_tiles[j][i] == '1' || _tiles[j][i] == '2' || _tiles[j][i] == '3' || _tiles[j][i] == '4')
					{
						int _tileLeft = j * 32;
						int _tileTop = i * 32;
						_xCollide = (_pacmanLeftSide <= _tileLeft + 31) && (_pacmanRightSide >= _tileLeft + 1);
						_yCollide = (_pacmanTop <= _tileTop + 31) && (_pacmanBottom - 1 >= _tileTop + 1);
						

						if (_xCollide && _yCollide)
						{
								_isColliding = true;
						}
					}
				}
			}

			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j <= 32; j++)
				{
					if (_tiles[j][i] == 'P')
					{
						int _tileLeft = j * 32;
						int _tileTop = i * 32;
						_xCollide = (_pacmanLeftSide <= _tileLeft + 31) && (_pacmanRightSide >= _tileLeft + 1);
						_yCollide = (_pacmanTop <= _tileTop + 31) && (_pacmanBottom - 1 >= _tileTop + 1);


						if (_xCollide && _yCollide)
						{
							Audio::Play(eatSound);
							_tiles[j][i] = '.';
							_score += 100;
							_pelletsEaten += 1;

							
							if (_pelletsEaten == (199 * (_level - 1) + 99))
							{
								_isCherry = true;
							}

							if (_pelletsEaten == (199 * (_level - 1) + 110))
							{
								_isCherry = false;
							}

							if (_pelletsEaten == 199*_level)
							{
								_levelEnd = true;
							}
						}
					}
				}
			}

			if (_isCherry)
			{
				int _tileLeft = 15 * 32;
				int _tileTop = 14 * 32;
				_xCollide = (_pacmanLeftSide <= _tileLeft + 31) && (_pacmanRightSide >= _tileLeft + 1);
				_yCollide = (_pacmanTop <= _tileTop + 31) && (_pacmanBottom - 1 >= _tileTop + 1);

				if (_xCollide && _yCollide)
				{
					_isCherry = false;
					_score += 200;
				}
			}

			
				if (keyboardState->IsKeyDown(Input::Keys::D))
				{
					if (!_isColliding)
					{
						// Makes Pacman Reset to the Left Side of the screen
						if (_pacmanPosition->X >= Graphics::GetViewportWidth())
							_pacmanPosition->X = -(_pacmanRight->Width);
						else
							_pacmanPosition->X += _cPacmanSpeed * elapsedTime; // Moves Pacman Right
					
					}
					
					

					_direction = 'R';
					
					

				}
					
				else if (keyboardState->IsKeyDown(Input::Keys::RIGHT))
				{
					if (!_isColliding)
					{
						if (_pacmanPosition->X >= Graphics::GetViewportWidth())
							_pacmanPosition->X = -(_pacmanRight->Width);
						else
							_pacmanPosition->X += _cPacmanSpeed * elapsedTime; // Moves Pacman Right
					}

					_direction = 'R';
				}

				else if (keyboardState->IsKeyDown(Input::Keys::A))
				{
					// Resets Pacman to Right Side of the Screen
					if (!_isColliding)
					{


						if (_pacmanPosition->X <= -(_pacmanRight->Width))
							_pacmanPosition->X = 1024.0f;
						else
							_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; // Moves Pacman Left
					}

					_direction = 'L';
				}

				else if (keyboardState->IsKeyDown(Input::Keys::LEFT))
				{
					// Resets Pacman to Right Side of the Screen
					if (!_isColliding)
					{


						if (_pacmanPosition->X <= -(_pacmanRight->Width))
							_pacmanPosition->X = 1024.0f;
						else
							_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; // Moves Pacman Left
					}

					_direction = 'L';
				}

				else if (keyboardState->IsKeyDown(Input::Keys::S))
				{
					// Checks that Pacman Doesn't go off the map
					if (!_isColliding)
					{
							_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; // Moves Pacman Down
					}

					_direction = 'D';
				}


				else if (keyboardState->IsKeyDown(Input::Keys::DOWN)) 
				{
					// Checks that Pacman Doesn't go off the map
					// Checks that Pacman Doesn't go off the map
					if (!_isColliding)
					{
							_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; // Moves Pacman Down
					}

					_direction = 'D';
				}

				else if (keyboardState->IsKeyDown(Input::Keys::W))
				{
					// Checks that Pacman Doesn't go off the map
					if (!_isColliding)
					{


						if (_pacmanPosition->Y > 0.1f)
							_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; // Moves Pacman Up
					}

					_direction = 'U';
				}
					

				else if (keyboardState->IsKeyDown(Input::Keys::UP))
				{
					// Checks that Pacman Doesn't go off the map
					if (!_isColliding)
					{


						if (_pacmanPosition->Y > 0.1f)
							_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; // Moves Pacman Up
					}
					

					_direction = 'U';
				}

				if (_direction == 'R' && _isColliding == true)
				{
					_pacmanPosition->X -= 5.0f;
				}

				else if (_direction == 'L' && _isColliding == true)
				{
					_pacmanPosition->X += 5.0f;
				}

				else if (_direction == 'D' && _isColliding == true)
				{
					_pacmanPosition->Y -= 5.0f;
				}

				else if (_direction == 'U' && _isColliding == true)
				{
					_pacmanPosition->Y += 5.0f;
				}

				_isColliding = false;
			
			
			//
			// End of Movement Input
			//


			// Stops frames from being progressed (stops animation) whilst paused
			_frameCount++;
		}

	}

	//
	// End of Start Check
	//

}

void Pacman::Draw(int elapsedTime)
{
	
	SpriteBatch::BeginDraw(); // Starts Drawing
	std::stringstream scoreStream;

	scoreStream << "SCORE: " << _score << "    Level: " << _level << "    Lives: " << _lives;
	// Draws Start Screen
	if(!_started)
	{
		_pacmanPosition->X = 15 * 32.0f;
		_pacmanPosition->Y = 14 * 32.0f;
		_direction = 'R';
		//Determines Strings to Draw
		std::stringstream startStream, h2pStream, cmdStream;

		startStream << "PACMAN";

		h2pStream << "USE WASD OR THE ARROW KEYS TO MOVE" << endl << "PRESS P TO PAUSE" << endl << "PRESS ESCAPE TO QUIT";

		cmdStream << "PRESS SPACE TO START";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(scoreStream.str().c_str(), _scoreStringPosition, Color::Green);
		SpriteBatch::DrawString(startStream.str().c_str(), _startStringPosition, Color::White);
		SpriteBatch::DrawString(h2pStream.str().c_str(), _h2pStringPosition, Color::White);
		SpriteBatch::DrawString(cmdStream.str().c_str(), _playcmdStringPosition, Color::White);
	}

	else if (_levelEnd)
	{
		std::stringstream levelEndStream, playStream;

		levelEndStream << "CONGRATS, YOU HAVE COMPLETED LEVEL: " << _level;

		playStream << "TO PLAY AGAIN, PRESS Y, IF YOU WISH TO END THE GAME PRESS N";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(levelEndStream.str().c_str(), _levelEndStringPosition, Color::White);
		SpriteBatch::DrawString(playStream.str().c_str(), _playAgainStringPosition, Color::White);
	}


	else if (_gameOver)
	{
		std::stringstream gameOverStream, playAgainStream;

		gameOverStream << "GAME OVER";

		playAgainStream << "TO PLAY AGAIN, PRESS Y" << endl << "OR PRESS ESCAPE TO CLOSE THE GAME";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(scoreStream.str().c_str(), _scoreStringPosition, Color::Green);
		SpriteBatch::DrawString(gameOverStream.str().c_str(), _gameOverStringPosition, Color::White);
		SpriteBatch::DrawString(playAgainStream.str().c_str(), _playAgainStringPosition, Color::White);
	}

	else
	{
		

		if (_isCherry == true)
		{
			SpriteBatch::Draw(_cherry, _pacmanSpawn, _cherryRect);
		}


		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j <= 32; j++)
			{
				_arrayPos = _tiles[j][i];
				_pos = new Vector2(j * 32, i * 32);
				switch (_arrayPos)
				{

				case'.':
					SpriteBatch::Draw(_menuBackground, _pos);
					break;

				case '1':
					SpriteBatch::Draw(_horizWall, _pos, _wallRect);
					break;
				case '2':
					SpriteBatch::Draw(_verticWall, _pos, _wallRect);
					break;
				case '3':
					SpriteBatch::Draw(_cornerWall, _pos, _wallRect);
					break;
				case '4':
					SpriteBatch::Draw(_gap, _pos, _wallRect);
					break;

				}

				


				if (_arrayPos == 'P')
				{
					if (_frameCount > 30)
						SpriteBatch::Draw(_munchieBigTexture, _pos, nullptr, _center, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
					else
						SpriteBatch::Draw(_munchieBlueTexture, _pos, nullptr, _center, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
				}


			}

		}

		if (_ghostsStarted)
		{
			_orangeAI(elapsedTime);
			_blueAI(elapsedTime);
			_pinkAI(elapsedTime);
			_redAI(elapsedTime);
		}
		
		if (!_ghostsStarted)
		{
			_orangeStart(elapsedTime);
			_blueStart(elapsedTime);
			_pinkStart(elapsedTime);
			_redStart(elapsedTime);
		}

		if (_frameCount > 30)
		{
			if (_direction == 'R')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanRight);
			if (_direction == 'L')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanLeft);
			if (_direction == 'D')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanDown);
			if (_direction == 'U')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanUp);


			SpriteBatch::Draw(_ghostSpriteSheet, _blueGhostPosition, _blueGhost);
			SpriteBatch::Draw(_ghostSpriteSheet, _redGhostPosition, _redGhost);
			SpriteBatch::Draw(_ghostSpriteSheet, _pinkGhostPosition, _pinkGhost);
			SpriteBatch::Draw(_ghostSpriteSheet, _orangeGhostPosition, _orangeGhost);

			if (_frameCount >= 60)
				_frameCount = 0;

		}
		else
		{
			if (_direction == 'R')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanRightOpen);
			if (_direction == 'L')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanLeftOpen);
			if (_direction == 'D')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanDownOpen);
			if (_direction == 'U')
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanUpOpen);


			SpriteBatch::Draw(_ghost2SpriteSheet, _blueGhostPosition, _blueGhost);
			SpriteBatch::Draw(_ghost2SpriteSheet, _redGhostPosition, _redGhost);
			SpriteBatch::Draw(_ghost2SpriteSheet, _pinkGhostPosition, _pinkGhost);
			SpriteBatch::Draw(_ghost2SpriteSheet, _orangeGhostPosition, _orangeGhost);

		}

		SpriteBatch::DrawString(scoreStream.str().c_str(), _scoreStringPosition, Color::Green);

		//Draws Pause Menu
		if (_paused)
		{
			//defines and determines the string stream to draw
			std::stringstream pauseStream;
			pauseStream << "PAUSED";

			//Draws the background for the pause screen
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);

			//Draws the PAUSED string in the center of the screen
			SpriteBatch::DrawString(pauseStream.str().c_str(), _pauseStringPosition, Color::Red);
			SpriteBatch::DrawString(scoreStream.str().c_str(), _scoreStringPosition, Color::Green);
		}
	}
	
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::_blueAI(int elapsedTime)
{
	

	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j <= 32; j++)
		{
			if (_tiles[j][i] == '1' || _tiles[j][i] == '2' || _tiles[j][i] == '3')
			{
				int _tileLeft = j * 32;
				int _tileTop = i * 32;
				_xCollide = (_blueGhostPosition->X + 1 <= _tileLeft + 31) && (_blueGhostPosition->X + 31 >= _tileLeft + 1);
				_yCollide = (_blueGhostPosition->Y + 1 <= _tileTop + 31) && (_blueGhostPosition->Y + 31 >= _tileTop + 1);


				if (_xCollide && _yCollide)
				{
					_isCollidingGhost = true;
				}
			}
		}
	}

	if (!_isCollidingGhost)
	{
		switch (_blueDirection)
		{
		case 'R':
			_blueGhostPosition->X += 0.1f * elapsedTime;
			break;
		case 'L':
			_blueGhostPosition->X -= 0.1f * elapsedTime;
			break;
		case 'U':
			_blueGhostPosition->Y -= 0.1f * elapsedTime;
			break;
		case 'D':
			_blueGhostPosition->Y += 0.1f * elapsedTime;
		}
	}

	if (_blueDirection == 'R' && _isCollidingGhost == true)
	{
		_blueGhostPosition->X -= 5.0f;
		_changeDirection = true;
		_blueDirection = 'D';
	}

	else if (_blueDirection == 'L' && _isCollidingGhost == true)
	{
		_blueGhostPosition->X += 5.0f;
		_changeDirection = true;
		_blueDirection = 'U';
	}

	else if (_blueDirection == 'D' && _isCollidingGhost == true)
	{
		_blueGhostPosition->Y -= 5.0f;
		_changeDirection = true;
		_blueDirection == 'L';
	}

	else if (_blueDirection == 'U' && _isCollidingGhost == true)
	{
		_blueGhostPosition->Y += 5.0f;
		_changeDirection = true;
		_blueDirection = 'R';
	}

	_isCollidingGhost = false;
}

void Pacman::_pinkAI(int elapsedTime)
{

}

void Pacman::_orangeAI(int elapsedTime)
{

	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j <= 32; j++)
		{
			if (_tiles[j][i] == '1' || _tiles[j][i] == '2' || _tiles[j][i] == '3')
			{

				int _tileLeft = j * 32;
				int _tileTop = i * 32;

				_xCollide = (_orangeGhostPosition->X + 1 <= _tileLeft + 31) && (_orangeGhostPosition->X + 31 >= _tileLeft + 1);
				_yCollide = (_orangeGhostPosition->Y + 1 <= _tileTop + 31) && (_orangeGhostPosition->Y + 31 >= _tileTop + 1);

				if (_xCollide && _yCollide)
				{
					_isCollidingGhost = true;
				}
			}
		}
	}


	if (_orangeDirection == 'R' && _isCollidingGhost == true)
	{
		_orangeGhostPosition->X -= 5.0f;
		_isCollidingGhost = false;
	}

	else if (_orangeDirection == 'L' && _isCollidingGhost == true)
	{
		_orangeGhostPosition->X += 5.0f;
		_isCollidingGhost = false;
	}

	else if (_orangeDirection == 'U' && _isCollidingGhost == true)
	{
		_orangeGhostPosition->Y += 5.0f;
		_isCollidingGhost = false;
	}

	else if (_orangeDirection == 'D' && _isCollidingGhost == true)
	{
		_orangeGhostPosition->Y -= 5.0f;
		_isCollidingGhost = false;
	}

		if (ceil(_orangeGhostPosition->X) < ceil(_pacmanPosition->X))
		{
			_orangeDirection = 'R';
		}

		else if (ceil(_orangeGhostPosition->X) > ceil(_pacmanPosition->X))
		{
			_orangeDirection = 'L';
		}

		else if (ceil(_orangeGhostPosition->Y) > ceil(_pacmanPosition->Y))
		{
			_orangeDirection = 'U';
		}

		else if (ceil(_orangeGhostPosition->Y) < ceil(_pacmanPosition->Y))
		{
			_orangeDirection = 'D';
		}


	switch (_orangeDirection)
	{
	case 'D':
		_orangeGhostPosition->Y += 0.1f * elapsedTime;
		break;
	case 'U':
		_orangeGhostPosition->Y -= 0.1f * elapsedTime;
		break;
	case 'R':
		_orangeGhostPosition->X += 0.1f * elapsedTime;
		break;
	case 'L':
		_orangeGhostPosition->X -= 0.1f * elapsedTime;
		break;

	};
}

void Pacman::_redAI(int elapsedTime)
{

}

void Pacman::_orangeStart(int elapsedTime)
{
	//Orange Spawn - 16 * 32.0f, 12 * 32.0f
		_orangeGhostPosition->X = 64;
		_orangeGhostPosition->Y = 64;

	_ghostsStarted = true;

	_orangeDirection = 'R';
	
}

void Pacman::_blueStart(int elapsedTime)
{
	_blueGhostPosition->X = 864;
	_blueGhostPosition->Y = 64;
	_blueDirection = 'R';
}

void Pacman::_pinkStart(int elapsedTime)
{
	_pinkGhostPosition->X = 64;
	_pinkGhostPosition->Y = 640;
	_pinkDirection = 'R';
}

void Pacman::_redStart(int elapsedTime)
{
	_redGhostPosition->X = 896;
	_redGhostPosition->Y = 640;
	_redDirection = 'L';
}