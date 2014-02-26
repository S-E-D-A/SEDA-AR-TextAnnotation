#include "TextGame.h"

using namespace TooN;
using namespace std;

TextGame::TextGame(const SE3<> se3, string sText)
    : Game(se3), msText(sText)
{
    mbInitialized = false;
    mdScale = 0.1;
}

void TextGame::Draw()
{

}

void TextGame::Reset()
{

}

void TextGame::Init()
{

}
