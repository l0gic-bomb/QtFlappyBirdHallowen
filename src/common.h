#ifndef COMMON_H
#define COMMON_H

#include <QApplication>

const int DEFAULT_SCREENWIDTH     = 600;
const int DEFAULT_SCREENHEIGHT    = 800;
const double DEFAULT_SCALEFACTOR  = 1.6;

const int GROUP_NEWROUND       = 0;
const int GROUP_GAMEPLAY       = 1;
const int GROUP_ROUNDEND       = 2;
const int GROUP_FIRSTSCREEN    = 3;
const int GROUP_MAX_ITEM_COUNT = 16;

const QString CONFIG_SCREENWIDTH  = "ScreenWidth";
const QString CONFIG_SCREENHEIGHT = "ScreenHeight";

// Картинки
const QString BACKGROUND_NIGHT = ":/graphics/background_night.png";
const QString BIRD_UP = ":/graphics/bird_up.png";
const QString BIRD_MIDDLE  = ":/graphics/bird_middle.png";
const QString BIRD_DOWN = ":/graphics/bird_down.png";
const QString GROUND_SEGMENT = ":/graphics/ground_segment.png";
const QString LOGO = ":/graphics/logo.png";
const QString BUTTON_PLAY = ":/graphics/play.png";
const QString BUTTON_ABOUT = ":/graphics/about.png";
const QString PIPE_DOWN = ":/graphics/pipe_down.png";
const QString PIPE_UP = ":/graphics/pipe_up.png";
const QString GAMEINFO = ":/graphics/gameInfo.png";
const QString GAMEOVER = ":/graphics/gameOver.png";
const QString SCOREBOARD = ":/graphics/scoreBoard.png";
const QString BIGNUMBER_ZERO = ":/graphics/0.png";
const QString BIGNUMBER_GENERIC = ":/graphics/%1.png";
const QString SMALLNUMBER_ZERO = ":/graphics/20.png";
const QString SMALLNUMBER_GENERIC = ":/graphics/2%1.png";


#define PIXMAP_SCALE(pixmap, scaleFactor) pixmap = (pixmap).scaled((pixmap).size() * scaleFactor);
#define POS_Y_LOGO(height) (((height) / 2) - ((height) / 4.5))

#endif // COMMON_H
