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
#include <QThread>
#include <QFontDatabase>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_mouseClicked(false), m_firstSelection(QPointF(-1, -1)), m_boxX(0), m_boxY(0)
    , m_countOfMove(0), m_deltaTime(0.0f), m_loopTime(0.0f), m_loopSpeed(Game::ITERATION_VALUE),
      m_isStartAnimRunnning(false)
{
    loadPixmap();
    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    setBackgroundBrush(QBrush(Game::BG_COLOR));

    int id = QFontDatabase::addApplicationFont(":/res/college.ttf");
    m_familyName = QFontDatabase::applicationFontFamilies(id).at(0);


    generateRevealedBoxesData(m_revealedBoxes, false);
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
    if( m_loopTime > m_loopSpeed && !m_isStartAnimRunnning)
    {
        m_loopTime -= m_loopSpeed;
        setBackgroundBrush(QBrush(Game::BG_COLOR));
        if(m_hide)
        {
            QThread::currentThread()->msleep(1000);
            m_revealedBoxes[m_hx1][m_hy1] = false;
            m_revealedBoxes[m_hx2][m_hy2] = false;
            m_hide = false;
        }
        clear();

        drawBoard();
        drawText();
        drawMoveText();
        QPointF boardPoint =  getBoxAtPixel(m_clickedPos.x(), m_clickedPos.y());
        if(boardPoint != QPointF(-1,-1))
        {
            if(!m_revealedBoxes[int(boardPoint.x())][int(boardPoint.y())])
            {
                drawHighlightBox(boardPoint.x(), boardPoint.y());
                QThread::currentThread()->msleep(200);
            }
            if(!m_revealedBoxes[int(boardPoint.x())][int(boardPoint.y())] && m_mouseClicked)
            {
                m_revealedBoxes[int(boardPoint.x())][int(boardPoint.y())] = true;
                if(m_firstSelection == QPointF(-1,-1) )
                {
                    m_firstSelection = QPointF(boardPoint);
                }
                else
                {
                    QPair<QString, QColor> field1 = getShapeAndColor(int(m_firstSelection.x()), int(m_firstSelection.y()));
                    QPair<QString, QColor> field2 = getShapeAndColor(int(boardPoint.x()), int(boardPoint.y()));
                    if(field1.first != field2.first || field1.second != field2.second)
                    {
                        hideBox(int(m_firstSelection.x()), int(m_firstSelection.y()), int(boardPoint.x()), int(boardPoint.y()) );
                        m_hide = true;
                    }
                    m_firstSelection = QPointF(-1, -1);
                    ++m_countOfMove;
                }

            }

        }

    }
}

void GameScene::loadPixmap()
{
    m_itemsPixmap.load(Game::PATH_TO_ITEMS_PIXMAP);
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

QPointF GameScene::getBoxAtPixel(float x, float y)
{
    if(m_clickedPos == QPointF())
    {
        return QPointF(-1, -1);
    }
    for(unsigned int boxx = 0; boxx < Game::BOARD_WIDTH; ++boxx)
    {
        for(unsigned int boxy = 0; boxy < Game::BOARD_HEIGHT; ++boxy)
        {
            QPointF leftTop = leftTopCoordsOfBox(QPointF(boxx, boxy));
            QRectF boxRect = QRectF(leftTop, QSizeF(Game::BOX_SIZE, Game::BOX_SIZE));
            if(boxRect.contains(x, y))
            {
                return QPointF(boxx, boxy);
            }
        }
    }
    return QPointF(-1, -1);
}

void GameScene::startGameAnimation()
{
    bool coveredBoxes[Game::BOARD_WIDTH][Game::BOARD_HEIGHT];
    generateRevealedBoxesData(coveredBoxes, false);

    m_isStartAnimRunnning = true;

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
    m_boxGroups = splitIntoGroupsOf(8, boxes);

    drawBoard();
    for(int i = 0; i < m_boxGroups.size(); ++i)
    {
        QVector<QGraphicsRectItem*> items;
        for(int j = 0; j < m_boxGroups[i].size(); ++j)
        {
            QGraphicsRectItem*rectItem = new QGraphicsRectItem();
            addItem(rectItem);
            items.push_back(rectItem);
        }
        m_revealCoverAnimRectItems.push_back(items);
    }
    m_revealCoverAnimCoverage.resize(m_boxGroups.size());

    for(int i = 0; i < m_boxGroups.size(); ++i)
    {
        QTimer* t = new QTimer(this);
        m_revealAnimTimers.push_back(t);
    }

    for(int i = 0; i < m_boxGroups.size(); ++i)
    {
        QTimer* t = new QTimer(this);
        m_coverAnimTimers.push_back(t);
    }

    QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem();
    textItem->setFont(QFont(m_familyName, 30, 50));
    textItem->setText("Memory this");
    textItem->setPos(0.3f*Game::RESOLUTION.width(), 0);
    textItem->setBrush(QBrush(Game::WHITE_COLOR));
    addItem(textItem);

    for(int i = 0; i < m_boxGroups.size(); ++i)
    {
        QTimer::singleShot(i*3200,[i, this](){
            this->revealAndCoverBoxesAnimation(m_boxGroups[i], i);
        });
    }
    QTimer::singleShot(m_boxGroups.size()*3200,[this](){
        this->m_isStartAnimRunnning = false;
    } );

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

void GameScene::drawText()
{
    QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem();
    textItem->setFont(QFont(m_familyName, 30, 80));
    textItem->setText("Memory Puzzle");
    textItem->setPos(0.3f*Game::RESOLUTION.width(), 0);
    textItem->setBrush(QBrush(Game::WHITE_COLOR));
    textItem->setPen(QPen(Game::CYAN_COLOR));
    addItem(textItem);
}

QPair<QString, QColor> GameScene::getShapeAndColor(int x, int y)
{
    return m_mainBoard[x][y];
}

void GameScene::drawIcon(QString shape, QColor color, int x, int y)
{
    QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(x, y));
    QGraphicsPixmapItem* iconItem = new QGraphicsPixmapItem(m_itemsPixmap.copy(Game::ALL_SHAPES.indexOf(shape)*Game::BOX_SIZE,
                                                                                Game::ALL_COLORS.indexOf(color)*Game::BOX_SIZE,
                                                                                Game::BOX_SIZE, Game::BOX_SIZE));
    iconItem->setPos(leftTopPoint.x(), leftTopPoint.y());
    addItem(iconItem);
}

void GameScene::drawHighlightBox(float x, float y)
{
    QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(x, y));
    addRect(leftTopPoint.x() - 5, leftTopPoint.y() - 5, Game::BOX_SIZE+10, Game::BOX_SIZE + 10, QPen(QBrush(Game::HIGHLIGHT_COLOR), 6), QBrush(Qt::transparent));
}

void GameScene::drawMoveText()
{
    QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem();
    textItem->setFont(QFont(m_familyName, 30, 50));
    textItem->setText("Movements: " + QString::number(m_countOfMove));
    textItem->setPos(0.3f*Game::RESOLUTION.width(), 0.65f * Game::RESOLUTION.width());
    textItem->setBrush(QBrush(Game::WHITE_COLOR));
    addItem(textItem);
}

void GameScene::revealAndCoverBoxesAnimation(QVector<QPoint> boxGroup, int index)
{
    drawBoard();
    m_revealCoverAnimCoverage[index] = Game::BOX_SIZE;
    m_boxGroup = boxGroup;

    connect(m_revealAnimTimers[index], &QTimer::timeout, [this,index](){
        int i = 0;
        for(QPoint point : m_boxGroup)
        {
            QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(point));
            addRect(leftTopPoint.x(), leftTopPoint.y(), Game::BOX_SIZE, Game::BOX_SIZE, QPen(Game::BG_COLOR), QBrush(Game::BG_COLOR));
            QPair<QString, QColor> pair = getShapeAndColor(point.x(), point.y());
            drawIcon(pair.first, pair.second, point.x(), point.y());
            if(m_revealCoverAnimCoverage[index] > 0)
            {
                removeItem(m_revealCoverAnimRectItems[index][i]);
                if(m_revealCoverAnimCoverage[index] > int(Game::BOX_SIZE))
                {
                    m_revealCoverAnimCoverage[index] = Game::BOX_SIZE;
                }
                m_revealCoverAnimRectItems[index][i]->setRect(0,0, m_revealCoverAnimCoverage[index]+2, Game::BOX_SIZE+2);
                m_revealCoverAnimRectItems[index][i]->setPen(QPen(Game::BG_COLOR));
                m_revealCoverAnimRectItems[index][i]->setBrush(QBrush(Game::BOX_COLOR));
                m_revealCoverAnimRectItems[index][i]->setPos(leftTopPoint.x()-1, leftTopPoint.y()-1);
                addItem(m_revealCoverAnimRectItems[index][i]);
            }
            i++;
        }
        if(m_revealCoverAnimCoverage[index] < (-int(Game::REVEAL_SPEED)) - 1)
        {
            m_revealAnimTimers[index]->stop();
            m_revealCoverAnimCoverage[index] = 0;
            m_coverAnimTimers[index]->start(Game::COVER_ANIM_SPEED);
            connect(m_coverAnimTimers[index], &QTimer::timeout, [this, index](){
                int i = 0;
                m_revealCoverAnimCoverage[index] += int(Game::REVEAL_SPEED);
                if(m_revealCoverAnimCoverage[index] > int(Game::BOX_SIZE)-int(Game::REVEAL_SPEED))
                {
                    m_coverAnimTimers[index]->stop();
                }
                for(QPoint point : m_boxGroup)
                {
                    QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(point));
                    addRect(leftTopPoint.x(), leftTopPoint.y(), Game::BOX_SIZE, Game::BOX_SIZE, QPen(Game::BG_COLOR), QBrush(Game::BG_COLOR));
                    QPair<QString, QColor> pair = getShapeAndColor(point.x(), point.y());
                    drawIcon(pair.first, pair.second, point.x(), point.y());
                    if(m_revealCoverAnimCoverage[index] > 0)
                    {
                        removeItem(m_revealCoverAnimRectItems[index][i]);
                        if(m_revealCoverAnimCoverage[index] > int(Game::BOX_SIZE))
                        {
                            m_revealCoverAnimCoverage[index] = Game::BOX_SIZE;
                        }
                        m_revealCoverAnimRectItems[index][i]->setRect(0,0, m_revealCoverAnimCoverage[index]+2, Game::BOX_SIZE+2);
                        m_revealCoverAnimRectItems[index][i]->setPen(QPen(Game::BG_COLOR));
                        m_revealCoverAnimRectItems[index][i]->setBrush(QBrush(Game::BOX_COLOR));
                        m_revealCoverAnimRectItems[index][i]->setPos(leftTopPoint.x()-1, leftTopPoint.y()-1);
                        addItem(m_revealCoverAnimRectItems[index][i]);
                    }
                    i++;
                }
            });

        }
        m_revealCoverAnimCoverage[index] -= Game::REVEAL_SPEED;
    }
    );
    m_revealAnimTimers[index]->start(Game::RENEVAL_ANIM_SPEED);
}

void GameScene::coverBoxesAnimation(QVector<QPoint> boxGroup)
{
    for(int coverage = 0; coverage < int(Game::BOX_SIZE)+int(Game::REVEAL_SPEED); coverage += int(Game::REVEAL_SPEED))
    {
        drawBoxCovers(boxGroup, coverage);
    }
}

void GameScene::drawBoxCovers(QVector<QPoint> boxGroup, int coverage)
{

    for(QPoint point : boxGroup)
    {
        QPointF leftTopPoint = leftTopCoordsOfBox(QPointF(point));
        addRect(leftTopPoint.x(), leftTopPoint.y(), Game::BOX_SIZE, Game::BOX_SIZE, QPen(Game::BG_COLOR), QBrush(Game::BG_COLOR));
        QPair<QString, QColor> pair = getShapeAndColor(point.x(), point.y());
        drawIcon(pair.first, pair.second, point.x(), point.y());
        if(coverage > 0)
        {
            QGraphicsRectItem* rectItem = new QGraphicsRectItem(0,0,coverage, Game::BOX_SIZE);
            rectItem->setPen(QPen(Game::BG_COLOR));
            rectItem->setBrush(QBrush(Game::BG_COLOR));
            rectItem->setPos(leftTopPoint.x(), leftTopPoint.y());
            addItem(rectItem);
        }
    }

}

void GameScene::hideBox(int x1, int y1, int x2, int y2)
{
     m_hx1 = x1;
     m_hx2 = x2;
     m_hy1 = y1;
     m_hy2 = y2;
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isStartAnimRunnning)
    {
        return;
    }
    m_mouseClicked = true;
    m_clickedPos = event->scenePos();
    QGraphicsScene::mouseReleaseEvent(event);
}
