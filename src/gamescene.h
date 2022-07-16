#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include "game.h"
#include <QVector>
#include <QPair>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    void startGameAnimation();
signals:
private slots:
    void loop();
private:
    void generateRevealedBoxesData(bool a[Game::BOARD_WIDTH][Game::BOARD_HEIGHT], bool value);
    QVector<QVector<QPair<QString, QColor> > > getRandomizedBoard();
    bool m_revealedBoxes[Game::BOARD_WIDTH][Game::BOARD_HEIGHT];
    QPointF leftTopCoordsOfBox(QPointF point);

    QVector <QVector<QPoint> > splitIntoGroupsOf(int size, QVector<QPoint> points);
    void drawBoard();
    QPair<QString, QColor> getShapeAndColor(int x, int y);
    void drawIcon(QString shape, QColor color, int x, int y);
    void revealAndCoverBoxesAnimation(QVector<QPoint> boxGroup, int index);
    QTimer m_revealTimer;
    void coverBoxesAnimation(QVector<QPoint> boxGroup);
    QTimer m_coverTimer;
    void drawBoxCovers(QVector<QPoint> boxGroup, int coverage);

    float m_deltaTime, m_loopTime;
    const float m_loopSpeed;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    QVector<QVector<QPair<QString, QColor> > > m_mainBoard;
    bool m_mouseClicked;
    QPointF m_clickedPos;
    int m_boxX, m_boxY;
    QString m_familyName;

    //Start Animation
    QVector<QPoint> m_boxGroup;
    QVector< QVector<QGraphicsRectItem*> >  m_revealCoverAnimRectItems;
    QVector< QVector<QPoint> > m_boxGroups;
    QVector<int> m_revealCoverAnimCoverage;
    QVector<QTimer*> m_revealAnimTimers;
    QVector<QTimer*> m_coverAnimTimers;

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
