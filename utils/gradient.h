#ifndef GRADIENT_H
#define GRADIENT_H

#include <QVector>

template <typename T>
class LinearGradient
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
class MultiLinearGradient
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
        point.Pos = std::numeric_limits<T>::min();
        foreach(Point p, mPoints)
        {
           if(p.Pos <= t && point.Pos < p.Pos)
           {
               point = p;
           }
        }

        return point;
    }

    inline T highBorderValue(float t) const
    {
        Point point;
        point.Pos = std::numeric_limits<T>::max();
        foreach(Point p, mPoints)
        {
           if(p.Pos >= t && point.Pos > p.Pos)
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

private:
    QVector<Point> mPoints;
};


#endif // GRADIENT_H
