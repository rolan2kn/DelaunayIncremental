//
// Created by rkindela on 12-04-2018.
//

#ifndef PMTOOL_POINT_H
#define PMTOOL_POINT_H
# include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <class T>
struct Point {
    T x, y, z;
    explicit Point(T _x = 0, T _y = 0, T _z = 0): x(_x), y(_y), z(_z){};

    template <class R>
    explicit Point(const Point<R>& rp): x(rp.x), y(rp.y), z(rp.z){};

    float distance(const Point<T>& p)
    {
       return sqrt((p.x-x)*(p.x-x) + (p.y-y)*(p.y-y) + (p.z-z)*(p.z-z));
    }

    Point<T>& multiply(float value)
    {
        return *(new Point<T>(T(x*value), T(y*value), T(z*value)));
    }

    T squareDistance(const Point<T>& p)
    {
        return ((p.x-x)*(p.x-x) + (p.y-y)*(p.y-y) + (p.z-z)*(p.z-z));
    }

    T squareDistanceFromOrigin() const
    {
        return ((x)*(x) + (y)*(y) + (z)*(z));
    }

    Point<T> operator+(const Point<T>& pointb)
    {
        return *(new Point<T>(x + pointb.x, y + pointb.y, z + pointb.z));
    }

    Point<T>& operator-(const Point<T>& pointb)
    {
        return *(new Point<T>(x - pointb.x, y - pointb.y, z-pointb.z));
    }

    Point<T>& operator=(const Point<T>& pointb)
    {
        x = pointb.x;
        y = pointb.y;
        z = pointb.z;

        return *this;
    }

    bool operator<(const Point<T>& pointb)
    {
        return this->squareDistanceFromOrigin() < pointb.squareDistanceFromOrigin();
    }

    bool operator>=(const Point<T>& pointb)
    {
        return this->squareDistanceFromOrigin() >= pointb.squareDistanceFromOrigin();
    }

    bool operator==(const Point<T>& pointb)
    {
        return (x == pointb.x && y == pointb.y && z == pointb.z);
    }

    template <class S>
    friend std::ostream& operator<<(std::ostream& os, const Point<S>& p);

    template <class S>
    friend std::ostream& operator<<(std::ostream& os, const Point<S>* p);

};

template<class T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p)
{
//    if (p.z == 0)
//        os<<"("<<p.x<<", "<<p.y<<")";
//    else
    os<<"("<<p.x<<", "<<p.y<<", "<<p.z<<")";

    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Point<T>* p)
{
//    if (p->z == 0)
//        os<<"("<<p->x<<", "<<p->y<<")";
//    else
    if (p == nullptr)
        return os<<" INVALID POINTER";
    os<<"("<<p->x<<", "<<p->y<<", "<<p->z<<")";

    return os;
}

typedef Point<int> POINT;

#endif //PMTOOL_POINT_H
