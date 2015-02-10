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
	/**
	 * This constructor creates a new memory stream instance. This stream
	 * is automatically opened and made ready for writing.
	 */
	Memstream();

	/**
	 * This destructor cleans up our memory stream, freeing all memory.
	 */
	~Memstream();

	/**
	 * This function writes the given data to the buffer.
	 *
	 * \param data The data to write to the buffer.
	 * \param length The length of the data, bytes.
	 * \return The number of bytes written.
	 */
	std::size_t write(const uint8_t *data, std::size_t length);

	/**
	 * This function flushes any outstanding writes to our buffer. If this
	 * results in an error, then an exception will be thrown instead.
	 */
	void flush();

	/**
	 * This function returns the current size of the buffer contents, in
	 * bytes.
	 *
	 * \return The size of the buffer.
	 */
	std::size_t getSize() const;

	/**
	 * This function return a pointer to the buffer which contains all of
	 * the data that has been written so far. This buffer contains
	 * getSize() bytes of data, and it is additionally null terminated.
	 *
	 * Note that there are some caveats about the returned pointer. First,
	 * for the pointer to be valid flush() must be called, which means that
	 * this function must be non-const. Second, the returned pointer is
	 * ONLY valid until the next time data is written to this object, since
	 * adding data to the buffer may cause it to be reallocated.
	 *
	 * \return This memory stream's internal buffer pointer.
	 */
	uint8_t *getBuffer();

	/**
	 * This function returns this memstream's internal file pointer. This
	 * can be used to perform low-level operations with this stream's
	 * buffer.
	 *
	 * \return This memstream's internal file pointer.
	 */
	FILE *getFile();

private:
	uint8_t *buffer;
	FILE *stream;
	std::size_t size;

	Memstream(const Memstream &);
	Memstream &operator=(const Memstream &);
};
}
}

#endif
