#ifndef COMMON_H
#define COMMON_H

#include <QApplication>

constexpr int DEFAULT_SCREENWIDTH     = 600;
constexpr int DEFAULT_SCREENHEIGHT    = 800;
constexpr double DEFAULT_SCALEFACTOR  = 1.6;

constexpr int GROUP_NEWROUND       = 0;
constexpr int GROUP_GAMEPLAY       = 1;
constexpr int GROUP_ROUNDEND       = 2;
constexpr int GROUP_FIRSTSCREEN    = 3;
constexpr int GROUP_MAX_ITEM_COUNT = 16;

#define PIXMAP_SCALE(pixmap, scaleFactor) pixmap = (pixmap).scaled((pixmap).size() * scaleFactor);
#define POS_Y_LOGO(height) (((height) / 2) - ((height) / 4.5))

#endif // COMMON_H
