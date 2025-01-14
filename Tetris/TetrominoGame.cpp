#include "TetrominoGame.h"
#include "VarUtils.h"

TetrominoGame::TetrominoGame() {
	tetro = nullptr;
	isStop = false;
	dx = (GAME_WIDTH * SIZE_PIXELS) / 2 - SIZE_PIXELS;
	dy = 0;
	timeCollision = TIME_COLLISION;
	gridGame = nullptr;
}

TetrominoGame::TetrominoGame(GridGame *grid): gridGame(grid) {
	tetro = nullptr;
	isStop = false;
	timeCollision = TIME_COLLISION;
	dx = (GAME_WIDTH * SIZE_PIXELS) / 2 - SIZE_PIXELS;
	dy = 0;
}

void TetrominoGame::changeTetro(Tetromino* tetro_) {
	tetro = tetro_;
	isStop = false;
	dx = (GAME_WIDTH * SIZE_PIXELS)/2 - SIZE_PIXELS;
	dy = 0;
	timeCollision = TIME_COLLISION;
}

void TetrominoGame::rotate() {
	float maxLast = dxMax();
	float minLast = dxMin();
	tetro->rotate();
	float maxAfter = dxMax();
	float minAfter = dxMin();
	if (dx > maxAfter)
		dx = maxAfter;
	else if(dx <= minAfter)
		dx = 0;
	
	if (isEmbedded()) {
		if (tetro->color == 5)
			tetro->deRotate(); // Le tetro "T" n'as pas besoin de verifier les cot�s car il est tourne deja sur son centre
		else {
			bool isStop = false;
			double i = 1;
			while (i >= -1 && !isStop) {
				double dxTmp = dx + i * SIZE_PIXELS;
				if (!isEmbeddedIA(dxTmp, dy)) {
					isStop = true;
					dx = dxTmp;
				}
				--i;
			}
			if (!isStop)
				tetro->deRotate();
			else
				timeCollision = TIME_COLLISION;
		}
	}
	else
		timeCollision = TIME_COLLISION;
}

void TetrominoGame::left() {
	if (!isCollision(0))
		dx -= SIZE_PIXELS;
}

void TetrominoGame::right() {
	if (!isCollision(2))
		dx += SIZE_PIXELS;
}

void TetrominoGame::down(double vitesseDown, double vitesseCollision) {
	if (!isCollision(1))
		dy += vitesseDown;
	else {
		if (timeCollision <= 0)
			isStop = true;
		else
			timeCollision -= vitesseCollision;
	}
	if (!isCollision(1))
		dy += vitesseDown;
}

void TetrominoGame::drop() {
	dy = ghostBlock().y;
	isStop = true;
}

sf::Vector2<double> TetrominoGame::ghostBlock() {
	double y = dy;
	while (y < dyMax() && !isCollisionY(y))
		y++;
	return sf::Vector2<double>(dx, y);
}

void TetrominoGame::transpose() {
	int indiceX = dx / SIZE_PIXELS;
	int indiceY = dy / SIZE_PIXELS;

	array<sf::Vector2i, 4> tetroTmp = tetro->getTetroActual();

	for (int i = 0; i < 4; ++i) {
		gridGame->grid[indiceX + tetroTmp[i].x][indiceY + tetroTmp[i].y] = tetro->color;
	}
}

float TetrominoGame::dxMax() {
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	int indiceMax = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (tmp[i].x > indiceMax) indiceMax = tmp[i].x;
	}
	return SIZE_PIXELS * ( GAME_WIDTH - indiceMax - 1);
}

float TetrominoGame::dyMax() {
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	int indiceMax = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (tmp[i].y > indiceMax) indiceMax = tmp[i].y;
	}
	return SIZE_PIXELS * (GAME_HEIGHT - indiceMax - 1);
}

float TetrominoGame::dxMin() {
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	int indiceMin = 4;
	for (int i = 0; i < 4; ++i)
	{
		if (tmp[i].x < indiceMin) indiceMin = tmp[i].x;
	}
	return SIZE_PIXELS * (indiceMin-1);
}

bool TetrominoGame::isEmbedded() {
	int indiceX = dx / SIZE_PIXELS;
	int indiceY = dy / SIZE_PIXELS;
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	for (int i = 0; i < 4; ++i) {
		if (!gridGame->isEmbedded(indiceX + tmp[i].x, indiceY + tmp[i].y)) {
			return true;
		}
	}
	return false;
}

bool TetrominoGame::isEmbeddedIA(double dx, double dy) {
	int indiceX = dx / SIZE_PIXELS;
	int indiceY = dy / SIZE_PIXELS;
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	for (int i = 0; i < 4; ++i) {
		if (!gridGame->isEmbedded(indiceX + tmp[i].x, indiceY + tmp[i].y)) {
			return true;
		}
	}
	return false;
}

bool TetrominoGame::isCollisionY(float y) {
	int indiceX = dx / SIZE_PIXELS;
	int indiceY = y / SIZE_PIXELS;
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	for (int i = 0; i < 4; ++i)
	{
		if (gridGame->checkNeighbor(indiceX + tmp[i].x, indiceY + tmp[i].y)[1])
			return true;
	}
	return false;
}

bool TetrominoGame::isCollision(int idCollision) {
	int indiceX = dx / SIZE_PIXELS;
	int indiceY = dy / SIZE_PIXELS;
	array<sf::Vector2i, 4> tmp = tetro->getTetroActual();
	for (int i = 0; i < 4; ++i)
	{
		if (gridGame->checkNeighbor(indiceX + tmp[i].x, indiceY + tmp[i].y)[idCollision]) 
			return true;
	}
	return false;
}