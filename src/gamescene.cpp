#include "gamescene.h"

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_deltaTime(0.0f), m_loopTime(0.0f), m_loopSpeed(Game::ITERATION_VALUE)
{
    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    setBackgroundBrush(QBrush(Game::BG_COLOR));

    generateRevealedBoxesData(m_revealedBoxes, false);

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
