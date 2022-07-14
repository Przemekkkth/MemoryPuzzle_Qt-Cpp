#include "gamescene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <algorithm>
#include <vector>
#include <utility>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_deltaTime(0.0f), m_loopTime(0.0f), m_loopSpeed(Game::ITERATION_VALUE), m_mouseClicked(false)
    , m_boxX(0), m_boxY(0)
{

    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    setBackgroundBrush(QBrush(Game::BG_COLOR));

    generateRevealedBoxesData(m_revealedBoxes, true);
    m_mainBoard = getRandomizedBoard();

    m_boxX = m_boxY = 0;
    startGameAnimation();
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

QVector< QVector< QPair<QString, QColor> > > GameScene::getRandomizedBoard()
{
    QVector<QPair<QString, QColor> > icons;


    std::vector<std::pair<QString, QColor> > tmpIcons;
    foreach(QString shape, Game::ALL_SHAPES)
    {
        foreach(QColor color, Game::ALL_COLORS)
        {
            tmpIcons.push_back(std::pair(shape, color));
        }
    }

    std::random_shuffle(tmpIcons.begin(), tmpIcons.end());

    foreach(QString shape, Game::ALL_SHAPES)
    {
        foreach(QColor color, Game::ALL_COLORS)
        {
            tmpIcons.push_back(std::pair(shape, color));
        }
    }

    std::random_shuffle(tmpIcons.begin(), tmpIcons.end());

    for(const std::pair<QString, QColor> &element : tmpIcons)
    {
        QPair<QString, QColor> e;
        e.first = element.first;
        e.second = element.second;
        icons.push_back(e);
    }

    QVector< QVector< QPair<QString, QColor> > > board;
    for(unsigned int x = 0; x < Game::BOARD_WIDTH; ++x)
    {
        QVector< QPair<QString, QColor> > column;
        for(unsigned y = 0; y < Game::BOARD_HEIGHT; ++y)
        {
            column.append(icons[0]);
            icons.removeFirst();
        }
        board.push_back(column);
    }
    return board;
}

QPointF GameScene::leftTopCoordsOfBox(QPointF point)
{
    //Convert board coordinates to pixel coordinates
    float left = point.x() * (Game::BOX_SIZE + Game::GAP_SIZE) + Game::X_MARGIN;
    float top =  point.y() * (Game::BOX_SIZE + Game::GAP_SIZE) + Game::Y_MARGIN;
    return QPointF(left, top);
}

void GameScene::startGameAnimation()
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
    drawBoard();
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
            if(j < points.length())
            {
                sub.push_back(points.at(j));
            }
        }
        mainVector.push_back(sub);
    }
    return mainVector;
}

void GameScene::drawBoard()
{
    for(unsigned int x = 0; x < Game::BOARD_WIDTH; ++x)
      {
          for(unsigned int y = 0; y < Game::BOARD_HEIGHT; ++y)
          {
              QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(x, y));
              if(!m_revealedBoxes[x][y])
              {
                  QGraphicsRectItem *rectItem = addRect(0, 0, Game::BOX_SIZE, Game::BOX_SIZE, QPen(Game::WHITE_COLOR), QBrush(Game::WHITE_COLOR));
                  rectItem->setPos(leftTopPoint);
              }
              else
              {
                QPair<QString, QColor> pair = getShapeAndColor(x, y);
                drawIcon(pair.first, pair.second, x, y);
              }
          }
    }
}

QPair<QString, QColor> GameScene::getShapeAndColor(int x, int y)
{
    return m_mainBoard[x][y];
}

void GameScene::drawIcon(QString shape, QColor color, int x, int y)
{
    int quarter = int(Game::BOX_SIZE * 0.25f);
    int half =    int(Game::BOX_SIZE * 0.5f);

    QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(x, y));

    if(shape == Game::DONUT)
    {
        QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem(0,0, half-5, half -5);
        circle1->setBrush(QBrush(color));
        circle1->setPen(QPen(color));
        circle1->setPos(leftTopPoint.x() + half, leftTopPoint.y() + half);
        addItem(circle1);

        QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem(0,0, quarter - 5, quarter - 5);
        circle2->setBrush(QBrush(Game::BG_COLOR));
        circle2->setPen(QPen(Game::BG_COLOR));
        circle2->setPos(leftTopPoint.x() + half, leftTopPoint.y() + half);
        addItem(circle2);

    }
    else if(shape == Game::SQUARE)
    {
        QGraphicsRectItem* rectItem = new QGraphicsRectItem(0,0, Game::BOX_SIZE - half, Game::BOX_SIZE - half);
        rectItem->setBrush(QBrush(color));
        rectItem->setPen(QPen(color));
        rectItem->setPos(leftTopPoint.x() + quarter, leftTopPoint.y()+quarter);
        addItem(rectItem);
    }
    else if(shape == Game::DIAMOND)
    {
        QGraphicsPolygonItem* polyItem = new QGraphicsPolygonItem();
        QList<QPoint> points = {QPoint(leftTopPoint.x() + half, leftTopPoint.y()), QPoint(leftTopPoint.x() + Game::BOX_SIZE - 1, leftTopPoint.y() + half), QPoint(leftTopPoint.x() + half, leftTopPoint.y() + Game::BOX_SIZE - 1), QPoint(leftTopPoint.x(), leftTopPoint.y() + half)};
        QPolygonF poly = QPolygonF(points);
        polyItem->setPolygon(poly);
        polyItem->setBrush(QBrush(color));
        polyItem->setPen(QPen(color));
        polyItem->setPos(leftTopPoint.x(), leftTopPoint.y());
        addItem(polyItem);
    }
    else if(shape == Game::LINES)
    {
        for(unsigned int i = 0; i < Game::BOX_SIZE; ++i)
        {
            QGraphicsLineItem *lineItem1 = new QGraphicsLineItem();
            lineItem1->setLine(QLine(QPoint(leftTopPoint.x(), leftTopPoint.y() + i), QPoint(leftTopPoint.x()+i, leftTopPoint.y())));
            lineItem1->setPen(QPen(color));
            addItem(lineItem1);

            QGraphicsLineItem *lineItem2 = new QGraphicsLineItem();
            lineItem2->setLine(QLine(QPoint(leftTopPoint.x() + i, leftTopPoint.y() + Game::BOX_SIZE - 1), QPoint(leftTopPoint.x()+ Game::BOX_SIZE - 1, leftTopPoint.y() + 1)));
            lineItem2->setPen(QPen(color));
            addItem(lineItem2);
        }
    }
    else if(shape == Game::OVAL)
    {
        QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0, 0, Game::BOX_SIZE, half);
        ellipseItem->setBrush(QBrush(color));
        ellipseItem->setPen(QPen(color));
        ellipseItem->setPos(leftTopPoint.x(), leftTopPoint.y()+quarter);
        addItem(ellipseItem);
    }
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouseClicked = true;
    m_clickedPos = event->scenePos();
    QGraphicsScene::mouseReleaseEvent(event);
}
