#pragma once

#include "data/data.h"
#include "data/data_utils.h"

template<typename T, typename F, typename DF>
inline Data<T> newton_zero(const Data<T>& start, F f, DF df, T eps)
{
	Data<T> val = start;
	bool enough = false;
	while (!enough)
	{
		Data<T> fd = f(val);
		Data<T> dfd = df(val);
		val = val - fd / dfd;

		enough = (fd.abs() <= eps);
	}
	return val;
}

/**
 * Calculates one coefficient with divided differences.
 * Recursive function!
 *
 * Couldn't use Data<T>::size_type (Why???)
 */
template<typename T, typename S>
inline T newton_delta(S i, S j, const Data<T>& x, const Data<T>& y)
{
	Q_ASSERT(i <= j);
	if (i == j)
	{
		return y.at(i);
	}
	else
	{
		return (newton_delta(i + 1, j, x, y) - newton_delta(i, j - 1, x, y)) / (x.at(j) - x.at(i));
	}
}

template<typename T>
inline Data<T> newton_coeff(const Data<T>& x, const Data<T>& y)
{
	Q_ASSERT(x.hasSameStructure(y));
	Data<T> ret = x;

	for (Data<T>::size_type i = 0; i < x.size(); ++i)
	{
		ret.set(i, newton_delta(1, i, x, y));
	}
}

template<typename T>
inline Data<T> newton_value(const Data<T>& v, const Data<T>& x, const Data<T>& coeff)
{
	Q_ASSERT(x.hasSameStructure(coeff));

	Data<T> ret = v;
	for (Data<T>::size_type i = 0; i < ret.size(); ++i)
	{
		ret.set(i, 0);
	}

	for (Data<T>::size_type m = coeff.size(); m >= 2; --m)
	{
		ret = (v - x.at(m)) * ret + coeff.at(m);// Could be optimized. Always using new/delete is not good.
	}

	return ret;
}