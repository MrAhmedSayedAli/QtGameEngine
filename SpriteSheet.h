#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Grid.h"
#include <QPixmap>

class Node;

/// Represents a sprite sheet.
/// A sprite sheet is a sheet of paper with sprites arranged in a grid like fashion.
/// This is a cheap value class.
/// @author Abdullah Aghazadah
/// @date 3/18/17
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// SpriteSheet spriteSheet("path/to/file.png", 4,5,64,64); // the sprite sheet contains 64x64 tiles, 4 rows, and 5 columns
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class SpriteSheet
{
public:
    SpriteSheet(std::string filepath, int numXTiles, int numYTiles, double tileWidth, double tileHeight);

    QPixmap tileAt(const Node& cell) const;
    std::vector<QPixmap> tilesAt(const Node& topLeft, const Node& bottomRight) const;

    int numXTiles() const;
    int numYTiles() const;
    double tileWidth() const;
    double tileHeight() const;

private:
    Grid grid_;
    QPixmap pixmap_;
};

#endif // SPRITESHEET_H