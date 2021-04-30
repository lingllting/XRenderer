#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "Graphics.h"

int main(int argc, char** argv)
{
    Graphics graphics;
    Vec2i a[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i b[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i c[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    
    
    // line sweeping
    // graphics.DrawTriangle(a[0], a[1], a[2]);
    // graphics.DrawTriangle(b[0], b[1], b[2]);
    // graphics.DrawTriangle(c[0], c[1], c[2]);
    
    //AABB
    // graphics.DrawTriangle(a, white);
    // graphics.DrawTriangle(b, white);
    // graphics.DrawTriangle(c, white);
    
    Model *model = new Model("obj/african_head.obj");
    graphics.DrawModel(model);
    graphics.End();
    
    delete model;
	return 0;
}

