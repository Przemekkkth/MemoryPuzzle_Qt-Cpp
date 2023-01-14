#ifndef GAME_H
#define GAME_H
#include <QSize>
#include <QColor>
#include <QList>

class Game
{
public:
    Game();
    static const QSize RESOLUTION;
    static const unsigned int FPS;
    static const unsigned int REVEAL_SPEED;
    static const unsigned int BOX_SIZE;
    static const unsigned int GAP_SIZE;
    static constexpr unsigned int BOARD_WIDTH = 10;
    static constexpr unsigned int BOARD_HEIGHT = 7;

    static const int X_MARGIN;
    static const int Y_MARGIN;

    static const QColor GRAY_COLOR;
    static const QColor NAVYBLUE_COLOR;
    static const QColor WHITE_COLOR;
    static const QColor RED_COLOR;
    static const QColor GREEN_COLOR;
    static const QColor BLUE_COLOR;
    static const QColor YELLOW_COLOR;
    static const QColor ORANGE_COLOR;
    static const QColor PURPLE_COLOR;
    static const QColor CYAN_COLOR;
    static const QColor PINK_COLOR;
    static const QColor BLACK_COLOR;

    static const QColor BG_COLOR;
    static const QColor LIGHTBG_COLOR;
    static const QColor BOX_COLOR;
    static const QColor HIGHLIGHT_COLOR;

    static const QString HEART;
    static const QString SQUARE;
    static const QString DIAMOND;
    static const QString TRIANGLE;
    static const QString OVAL;

    static const float ITERATION_VALUE;

    static QList<QColor> ALL_COLORS;
    static QStringList   ALL_SHAPES;

    static constexpr int RENEVAL_ANIM_SPEED = int(120.0f);
    static constexpr int COVER_ANIM_SPEED = int(300.0f);

    static const QString PATH_TO_FONT;
    static const QString PATH_TO_CURSOR_PIXMAP;
    static const QString PATH_TO_ITEMS_PIXMAP;

}
;


#endif // GAME_H
