#pragma once

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

	inline static bool sort_f(const Point& p1, const Point& p2)
	{
		return p1.Pos < p2.Pos;
	}

	inline std::pair<Point, Point> borderPoints(float t) const
	{
		Point last = mPoints.first();
		foreach(Point p, mPoints)
		{
			if (p.Pos > t)
			{
				return std::pair<Point, Point>(last, p);
			}

			last = p;
		}

		return std::pair<Point, Point>(last, mPoints.last());
	}

public:
	inline T value(float t) const
	{
		Q_ASSERT(mPoints.size() >= 2);

		const std::pair<Point, Point> border = borderPoints(t);

		const float nt = (border.first.Pos == border.second.Pos) ? 1 : (t - border.first.Pos) / (border.second.Pos - border.first.Pos);

		return nt*border.second.Value + (1-nt)*border.first.Value;
	}

	inline void add(float t, T val)
	{
		Point p;
		p.Pos = t;
		p.Value = val;
		mPoints.append(p);

		std::sort(mPoints.begin(), mPoints.end(), sort_f);
	}

private:
	QVector<Point> mPoints;
};

class StandardGradient : public MultiLinearGradient<QVector4D>
{
public:
	enum Type
	{
		T_HSV,
		T_Hot,
		T_Cool,
		T_Gray
	};
	StandardGradient(Type type = T_HSV);
};
