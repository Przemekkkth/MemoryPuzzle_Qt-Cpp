#include "gamescene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <algorithm>
#include <vector>
#include <utility>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_deltaTime(0.0f), m_loopTime(0.0f), m_loopSpeed(Game::ITERATION_VALUE), m_mouseClicked(false)
{
    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    setBackgroundBrush(QBrush(Game::BG_COLOR));

    generateRevealedBoxesData(m_revealedBoxes, false);
    m_mainBoard = getRandomizedBoard();

    startGameAnimation(m_mainBoard);
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(Game::ITERATION_VALUE);
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    if( m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
    }
}

void GameScene::generateRevealedBoxesData(bool tab[Game::BOARD_WIDTH][Game::BOARD_HEIGHT], bool value)
{
    for(unsigned int i = 0; i < Game::BOARD_WIDTH; ++i)
    {
        for(unsigned int j = 0; j < Game::BOARD_HEIGHT; ++j)
        {
            tab[i][j] = value;
        }
    }
}

QVector<QPair<QString, QColor> > GameScene::getRandomizedBoard()
{
    QVector<QPair<QString, QColor> > retValue;

    std::vector<std::pair<QString, QColor> > icons;
    foreach(QString shape, Game::ALL_SHAPES)
    {
        foreach(QColor color, Game::ALL_COLORS)
        {
            icons.push_back(std::pair(shape, color));
        }
    }

    std::random_shuffle(icons.begin(), icons.end());

    foreach(QString shape, Game::ALL_SHAPES)
    {
        foreach(QColor color, Game::ALL_COLORS)
        {
            icons.push_back(std::pair(shape, color));
        }
    }

    std::random_shuffle(icons.begin(), icons.end());

    return retValue;
}

QPointF GameScene::leftTopCoordsOfBox(QPointF point)
{
    //Convert board coordinates to pixel coordinates
    float left = point.x() * (Game::BOX_SIZE + Game::GAP_SIZE) + Game::X_MARGIN;
    float top =  point.y() * (Game::BOX_SIZE + Game::GAP_SIZE) + Game::Y_MARGIN;
    return QPointF(left, top);
}

void GameScene::startGameAnimation(QVector<QPair<QString, QColor> > board)
{
    bool coveredBoxes[Game::BOARD_WIDTH][Game::BOARD_HEIGHT];
    generateRevealedBoxesData(coveredBoxes, false);

    std::vector<QPoint> tmpBoxes;
    for(unsigned int x = 0; x < Game::BOARD_WIDTH; ++x)
    {
        for(unsigned int y = 0; y < Game::BOARD_HEIGHT; ++y)
        {
            tmpBoxes.push_back(QPoint(x,y));
        }
    }

    std::random_shuffle(tmpBoxes.begin(), tmpBoxes.end());
    QVector<QPoint> boxes;
    for(QPoint point : tmpBoxes)
    {
        boxes.push_back(point);
    }
    //qDebug() << "Box " << boxes;
    QVector< QVector<QPoint> > boxGroups = splitIntoGroupsOf(8, boxes);
    //qDebug() << "boxGroup" << boxGroups;
}

QVector<QVector<QPoint> > GameScene::splitIntoGroupsOf(int size, QVector<QPoint> points)
{
    QVector<QPoint> sub;
    QVector< QVector<QPoint> > mainVector;
    for(int i = 0; i < points.length(); i += size)
    {
        sub.clear();
        for(int j = i; j < i + size; ++j)
        {
            sub.push_back(points.at(j));
        }
        mainVector.push_back(sub);
    }
    return mainVector;
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouseClicked = true;
    m_clickedPos = event->scenePos();
    QGraphicsScene::mouseReleaseEvent(event);
}
