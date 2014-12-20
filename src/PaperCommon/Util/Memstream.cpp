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

#include "Memstream.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

/**
 * This constructor creates a new memory stream instance. This stream is
 * automatically opened and made ready for writing.
 */
paper::util::Memstream::Memstream()
	: buffer(nullptr), stream(nullptr), size(0)
{
	static_assert(sizeof(char) == sizeof(uint8_t),
		"For Memstream, char must be 8 bits long.");

	stream = open_memstream(reinterpret_cast<char **>(&buffer), &size);

	if(stream == nullptr)
	{
		cleanup();
		throw std::runtime_error(strerror(errno));
	}
}

/**
 * This destructor cleans up our memory stream, freeing all memory.
 */
paper::util::Memstream::~Memstream()
{
	cleanup();
}

/**
 * This function writes the given data to the buffer.
 *
 * \param data The data to write to the buffer.
 * \param length The length of the data, bytes.
 * \return The number of bytes written.
 */
std::size_t paper::util::Memstream::write(
	const uint8_t *data, std::size_t length)
{
	return fwrite(data, sizeof(uint8_t), length, stream);
}

/**
 * This function flushes any outstanding writes to our buffer. If this results
 * in an error, then an exception will be thrown instead.
 */
void paper::util::Memstream::flush()
{
	int r = fflush(stream);

	if(r != 0)
		throw std::runtime_error(strerror(errno));
}

/**
 * This function returns the current size of the buffer contents, in bytes.
 *
 * \return The size of the buffer.
 */
std::size_t paper::util::Memstream::getSize() const
{
	return size;
}

/**
 * This function return a pointer to the buffer which contains all of the data
 * that has been written so far. This buffer contains getSize() bytes of data,
 * and it is additionally null terminated.
 *
 * Note that the returned pointer is only valid for the life of this object.
 *
 * \return This memory stream's internal buffer pointer.
 */
const uint8_t *paper::util::Memstream::getBuffer() const
{
	return buffer;
}

/**
 * This function cleans up our memory stream by closing any open streams, and
 * by freeing all allocated memory.
 */
void paper::util::Memstream::cleanup()
{
	if(stream != nullptr)
	{
		fclose(stream);
		stream = nullptr;
	}

	if(buffer != nullptr)
	{
		free(buffer);
		buffer = nullptr;
	}
}
