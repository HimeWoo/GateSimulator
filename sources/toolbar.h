#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "button.h"

    extern Rectangle toolPanel;
    extern Button **tools;
    extern const int numTools;

    void InitToolbar(void);
    void DrawToolbar(float borderThickness, Color borderColor);

#endif