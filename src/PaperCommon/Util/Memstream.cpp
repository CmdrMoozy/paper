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

paper::util::Memstream::Memstream() : buffer(nullptr), stream(nullptr), size(0)
{
	static_assert(sizeof(char) == sizeof(uint8_t),
	              "For Memstream, char must be 8 bits long.");

	stream = open_memstream(reinterpret_cast<char **>(&buffer), &size);

	if(stream == nullptr)
		throw std::runtime_error(strerror(errno));
}

paper::util::Memstream::~Memstream()
{
	if(stream != nullptr)
	{
		fclose(stream);
		stream = nullptr;
	}
}

std::size_t paper::util::Memstream::write(const uint8_t *data,
                                          std::size_t length)
{
	return fwrite(data, sizeof(uint8_t), length, stream);
}

void paper::util::Memstream::flush()
{
	int r = fflush(stream);

	if(r != 0)
		throw std::runtime_error(strerror(errno));
}

std::size_t paper::util::Memstream::getSize() const
{
	return size;
}

uint8_t *paper::util::Memstream::getBuffer()
{
	flush();
	return buffer;
}

FILE *paper::util::Memstream::getFile()
{
	return stream;
}
