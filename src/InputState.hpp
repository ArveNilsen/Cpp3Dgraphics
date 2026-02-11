#pragma once

#include <bitset>

struct InputState
{
    enum class KEY { Q, W, S, A, D };
    /*
     * 0: Quit
     * 1: W
     * 2: S
     * 3: A
     * 4: D
     */
    std::bitset<5> keys{};

    void setQ() { keys.set(0); }
    void setW() { keys.set(1); }
    void setS() { keys.set(2); }
    void setA() { keys.set(3); }
    void setD() { keys.set(4); }
    
    void reset() 
    { 
        keys.reset(); 
        mouseDeltaX = 0;
        mouseDeltaY = 0;
        scrollDelta = 0;
    }

    bool closeRequested() const noexcept
    {
        return keys.test(0);
    }

    bool keyDown(KEY key)
    {
        switch (key) {
        case KEY::Q:
            return keys.test(0);
        case KEY::W:
            return keys.test(1);
        case KEY::S:
            return keys.test(2);
        case KEY::A:
            return keys.test(3);
        case KEY::D:
            return keys.test(4);
        default:
            return false;
        }
    }

    // TODO: bool pressed(key) -> toggles
    float mouseDeltaX{ 0 };
    float mouseDeltaY{ 0 };
    float scrollDelta{ 0 };
    // TODO: Clamp deltas to prevent big jumps
};

