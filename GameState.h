#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
public:
    enum State {
        MAIN_MENU,
        PLAYING,
        PAUSED,
        LEVEL_TRANSITION,
        GAME_OVER,
        HIGH_SCORES,
        ENTER_NAME
    };
    
private:
    State currentState;
    State previousState;
    
public:
    GameState() : currentState(MAIN_MENU), previousState(MAIN_MENU) {}
    
    void setState(State newState) {
        previousState = currentState;
        currentState = newState;
    }
    
    State getState() const { return currentState; }
    State getPreviousState() const { return previousState; }
    
    inline bool isPlaying() const { return currentState == PLAYING; }
    inline bool isPaused() const { return currentState == PAUSED; }
    inline bool isGameOver() const { return currentState == GAME_OVER; }
    inline bool isMenu() const { return currentState == MAIN_MENU; }
    inline bool isTransitioning() const { return currentState == LEVEL_TRANSITION; }
};

#endif