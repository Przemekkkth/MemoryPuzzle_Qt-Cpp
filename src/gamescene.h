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

signals:
private slots:
    void loop();
private:
    void generateRevealedBoxesData(bool a[Game::BOARD_WIDTH][Game::BOARD_HEIGHT], bool value);
    QVector< QPair<QString, QColor> > getRandomizedBoard();
    bool m_revealedBoxes[Game::BOARD_WIDTH][Game::BOARD_HEIGHT];
    float m_deltaTime, m_loopTime;
    const float m_loopSpeed;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    QVector< QPair<QString, QColor> > m_mainBoard;
    bool m_mouseClicked;
    QPointF m_clickedPos;

    // QGraphicsScene interface
protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
