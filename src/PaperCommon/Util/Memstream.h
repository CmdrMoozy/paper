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

#ifndef PAPER_UTIL_MEMSTREAM_H
#define PAPER_UTIL_MEMSTREAM_H

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace paper
{
namespace util
{
/**
 * \brief This class provides an in-memory stream buffer.
 *
 * This is very similar to e.g. std::stringstream, except it is
 * designed to be more convenient to write binary data to, and
 * it provides direct access to its internal buffer pointer.
 *
 * NOTE: In order to prevent users from having to copy our
 * buffer contents, we DO NOT DELETE THE BUFFER IN OUR
 * DESTRUCTOR. This means that it is up to our users to call
 * free() on getBuffer()'s returned pointer
 */
class Memstream
{
public:
	Memstream();
	~Memstream();

	std::size_t write(const uint8_t *, std::size_t);
	void flush();

	std::size_t getSize() const;
	uint8_t *getBuffer();

	FILE *getFile();

private:
	uint8_t *buffer;
	FILE *stream;
	std::size_t size;

	Memstream(const Memstream &);
	Memstream &operator=(const Memstream &);

	void cleanup();
};
}
}

#endif
