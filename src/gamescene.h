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
    void setRevealRectItem(QGraphicsRectItem *newRevealRectItem);

signals:
private slots:
    void loop();
private:
    void loadPixmap();
    QPixmap m_itemsPixmap;
    void generateRevealedBoxesData(bool a[Game::BOARD_WIDTH][Game::BOARD_HEIGHT], bool value);
    QVector<QVector<QPair<QString, QColor> > > getRandomizedBoard();
    bool m_revealedBoxes[Game::BOARD_WIDTH][Game::BOARD_HEIGHT];
    QPointF leftTopCoordsOfBox(QPointF point);
    QPointF getBoxAtPixel(float x, float y);

    QVector <QVector<QPoint> > splitIntoGroupsOf(int size, QVector<QPoint> points);
    void drawBoard();
    void drawText();
    void drawIcon(QString shape, QColor color, int x, int y);
    void drawHighlightBox(float x, float y);
    void drawMoveText();
    QPair<QString, QColor> getShapeAndColor(int x, int y);


    QTimer m_revealTimer;
    void coverBoxesAnimation(QVector<QPoint> boxGroup);
    QTimer m_coverTimer;
    void drawBoxCovers(QVector<QPoint> boxGroup, int coverage);
    QVector<QVector<QPair<QString, QColor> > > m_mainBoard;
    bool m_mouseClicked;
    QPointF m_clickedPos;
    QPointF m_firstSelection;
    int m_boxX, m_boxY;
    QString m_familyName;
    void hideBox(int x1, int y1, int x2, int y2);
    int m_hx1, m_hx2, m_hy1, m_hy2;
    bool m_hide = false;
    int m_countOfMove;
    //frame rate
    float m_deltaTime, m_loopTime;
    const float m_loopSpeed;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;

    //Start Animation
    void revealAndCoverBoxesAnimation(QVector<QPoint> boxGroup, int index);
    QVector<QPoint> m_boxGroup;
    QVector< QVector<QGraphicsRectItem*> >  m_revealCoverAnimRectItems;
    QVector< QVector<QPoint> > m_boxGroups;
    QVector<int> m_revealCoverAnimCoverage;
    QVector<QTimer*> m_revealAnimTimers;
    QVector<QTimer*> m_coverAnimTimers;
    bool m_isStartAnimRunnning;


protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
