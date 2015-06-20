#ifndef GRADIENT_H
#define GRADIENT_H

#include <QVector>
#include <QVector4D>

template <typename T>
class Gradient
{
public:
    virtual T value(float t) const = 0;
};

template <typename T>
class LinearGradient : public Gradient<T>
{
public:
    inline T startValue() const
    {
        return mStart;
    }

    inline void setStartValue(T t)
    {
        mStart = t;
    }

    inline T endValue() const
    {
        return mEnd;
    }

    inline void setEndValue(T t)
    {
        mEnd = t;
    }

    inline T value(float t) const
    {
        return t*mEnd + (1-t)*mStart;
    }

private:
    T mStart;
    T mEnd;
};

template <typename T>
class MultiLinearGradient : public Gradient<T>
{
private:
    struct Point
    {
        T Value;
        float Pos;
    };
    inline Point lowBorderValue(float t) const
    {
        Point point;
        point.Pos = std::numeric_limits<float>::lowest();
        foreach(Point p, mPoints)
        {
           if(p.Pos <= t && point.Pos < p.Pos)
           {
               point = p;
           }
        }

        return point;
    }

    inline Point highBorderValue(float t) const
    {
        Point point;
        point.Pos = std::numeric_limits<float>::max();
        foreach(Point p, mPoints)
        {
           if(p.Pos > t && point.Pos > p.Pos)
           {
               point = p;
           }
        }

        return point;
    }

public:
    inline T value(float t) const
    {
        Q_ASSERT(mPoints.size() >= 2);

        const Point start = lowBorderValue(t);
        const Point end = highBorderValue(t);

        const float nt = (t-start.Pos)/(end.Pos-start.Pos);

        return nt*end.Value + (1-nt)*start.Value;
    }

    inline void add(float t, T val)
    {
        Point p;
        p.Pos = t;
        p.Value = val;
        mPoints.append(p);
    }

private:
    QVector<Point> mPoints;
};

class StandardGradient : public MultiLinearGradient<QVector4D>
{
public:
    StandardGradient();
};

#endif // GRADIENT_H
