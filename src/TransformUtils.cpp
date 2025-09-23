#include "TransformUtils.h"
#include <cmath>

SDL_Point rotatePoint(SDL_Point p, SDL_Point origin, double angGraus) {
    double ang = angGraus * M_PI / 180.0;
    double x = p.x - origin.x;
    double y = p.y - origin.y;
    double xr = x * cos(ang) - y * sin(ang);
    double yr = x * sin(ang) + y * cos(ang);
    return { (int)std::round(xr + origin.x), (int)std::round(yr + origin.y) };
}
