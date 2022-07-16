#include "game.h"

const QSize Game::RESOLUTION = QSize(640, 480);
const unsigned int Game::FPS = 30;
const unsigned int Game::REVEAL_SPEED = 8;
const unsigned int Game::BOX_SIZE = 40;
const unsigned int Game::GAP_SIZE = 10;

const int Game::X_MARGIN = int( (RESOLUTION.width() - (BOARD_WIDTH * (BOX_SIZE + GAP_SIZE) )) / 2);
const int Game::Y_MARGIN = int( (RESOLUTION.height() - (BOARD_HEIGHT * (BOX_SIZE + GAP_SIZE) )) / 2);;

//                                            R    G    B
const QColor Game::GRAY_COLOR      = QColor(100, 100, 100);
const QColor Game::NAVYBLUE_COLOR  = QColor( 60,  60, 100);
const QColor Game::WHITE_COLOR     = QColor(255, 255, 255);
const QColor Game::RED_COLOR       = QColor(255,   0,   0);
const QColor Game::GREEN_COLOR     = QColor(  0, 255,   0);
const QColor Game::BLUE_COLOR      = QColor(  0,   0, 255);
const QColor Game::YELLOW_COLOR    = QColor(255, 255,   0);
const QColor Game::ORANGE_COLOR    = QColor(255, 128,   0);
const QColor Game::PURPLE_COLOR    = QColor(255,   0, 255);
const QColor Game::CYAN_COLOR      = QColor(  0, 255, 255);

const QColor Game::BG_COLOR = NAVYBLUE_COLOR;
const QColor Game::LIGHTBG_COLOR = GRAY_COLOR;
const QColor Game::BOX_COLOR = WHITE_COLOR;
const QColor Game::HIGHLIGHT_COLOR = BLUE_COLOR;

const QString Game::DONUT = "donut";
const QString Game::SQUARE = "square";
const QString Game::DIAMOND = "diamond";
const QString Game::LINES = "lines";
const QString Game::OVAL = "oval";

QList<QColor> Game::ALL_COLORS = {
    GRAY_COLOR, NAVYBLUE_COLOR,
    GREEN_COLOR, YELLOW_COLOR, ORANGE_COLOR,
    PURPLE_COLOR, CYAN_COLOR
};
QStringList   Game::ALL_SHAPES = {
    DONUT, SQUARE, DIAMOND, LINES, OVAL
};

const float Game::ITERATION_VALUE = 1000.0f/(float)FPS;

const QString Game::PATH_TO_FONT = ":/res/college.ttf";
const QString Game::PATH_TO_CURSOR_PIXMAP = ":/res/cursor.png";

Game::Game()
{
    Q_STATIC_ASSERT_X((BOARD_WIDTH * BOARD_HEIGHT) % 2 == 0, "Board needs to have an even number of boxes for pairs of matches.");
}
