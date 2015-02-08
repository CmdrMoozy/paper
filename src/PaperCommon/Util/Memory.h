/*
 * Paper - An application for storing & loading data using QR codes.
 * Copyright (C) 2014  Axel Rasmussen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PAPER_UTIL_MEMORY_H
#define PAPER_UTIL_MEMORY_H

#include <cstddef>
#include <memory>

namespace paper
{
namespace util
{
/**
 * This is a basic utility which will allocate a new shared
 * array with an appropriate deleter to ensure that the memory
 * is properly freed on destruction.
 *
 * Note: if the caller wants to later reset() this pointer, it
 * is up to that caller to use an appropriate deleter.
 *
 * \param size The size of the array to allocate.
 * \return A shared pointer containing the requested array.
 */
template <typename T> std::shared_ptr<T> makeSharedArray(std::size_t size)
{
	auto deleter = [](const T *p)
	{
		delete[] p;
	};

	return std::shared_ptr<T>(new T[size], deleter);
}
}
}

#endif
