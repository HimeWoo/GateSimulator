#include "toolbar.h"

Rectangle toolPanel;
Button **tools;
const int numTools = 2;

void InitToolbar(void) {
    toolPanel = toolPanel = CLITERAL(Rectangle) {
        0, 0, 
        50, GetScreenHeight()
    };
    tools = malloc(numTools);
}

void DrawToolbar(float borderThickness, Color borderColor) {
    for (int i = 0; i < numTools; i++) {

    }
    DrawRectangleLinesEx(toolPanel, borderThickness, borderColor);
}
