#include <raylib.h>
#include <raymath.h>

#include <debugview.h>
#include <gamestates.h>
#include <ball.h>

// Debug view
bool debugView = false;

/* Function Definitions */

// Toggle debug view
void manageDebugView()
{
    if (gameState != GS_MAIN_GAME || !DEBUG_VIEW_ALLOWED)
        return;
    if (IsKeyPressed(KEY_TAB))
    {
        debugView = !debugView;
    }

    if (debugView)
    {
        if (IsKeyDown(KEY_EQUAL))
        {
            ball.speed.x = ball.speed.x + 10 * (ball.speed.x > 0 ? 1 : -1);
            ball.speed.y = ball.speed.y + 10 * (ball.speed.y > 0 ? 1 : -1);
        }
        else if (IsKeyDown(KEY_MINUS))
        {
            ball.speed.x = ball.speed.x - 10 * (ball.speed.x > 0 ? 1 : -1);
            ball.speed.y = ball.speed.y - 10 * (ball.speed.y > 0 ? 1 : -1);
        }
    }
}
