//
// Created by pc on 17-12-20.
//

#ifndef BLOCKS_POINT_H
#define BLOCKS_POINT_H

#include <SDL2/SDL.h>

class Point {

  public:
    Point(double x, double y);
    Point(const SDL_Point& point);

                 operator SDL_Point() const;
    Point        operator+=(const Point& other);
    Point        operator-=(const Point& other);
    friend Point operator-(const Point& lhs, const Point& rhs);
    friend Point operator+(const Point& lhs, const Point& rhs);
    double       x() const;
    double       y() const;

  private:
    double m_x;
    double m_y;
};

#endif // BLOCKS_POINT_H
