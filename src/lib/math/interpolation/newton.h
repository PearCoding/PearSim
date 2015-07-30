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
