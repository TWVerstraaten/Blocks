//
// Created by pc on 17-12-20.
//

#include "Point.h"

Point::Point(double x, double y) : m_x(x), m_y(y) {
}

Point::Point(const SDL_Point& point) : m_x(point.x), m_y(point.y) {
}

Point::operator SDL_Point() const {
    return SDL_Point{static_cast<int>(std::round(m_x)), static_cast<int>(std::round(m_y))};
}

Point Point::operator+=(const Point& other) {
    m_x += other.m_x;
    m_y += other.m_y;
    return *this;
}

Point Point::operator-=(const Point& other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    return *this;
}

Point operator-(const Point& lhs, const Point& rhs) {
    return {lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y};
}

Point operator+(const Point& lhs, const Point& rhs) {
    return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
}

double Point::x() const {
    return m_x;
}

double Point::y() const {
    return m_y;
}
