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

#include "IO.h"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * This function returns the size of the file denoted by the given path, in
 * bytes. If the file doesn't exist, or some error occurs, then an exception
 * will be thrown.
 *
 * \param path The path to the file to inspect.
 * \return The size of the given file, in bytes.
 */
std::size_t paper::util::io::filesize(const std::string &path)
{
	struct stat s;
	int r = stat(path.c_str(), &s);

	if(r != 0)
	{
		throw std::runtime_error(strerror(errno));
	}

	return static_cast<size_t>(s.st_size);
}

/**
 * This function loads all of the contents of the file denoted by the given
 * path into memory. The contents will be stored in an array of uint8_t's,
 * which will be placed in the given shared_ptr instance. Any existing contents
 * of the given shared_ptr will be reset().
 *
 * \param buf The shared_ptr to store a pointer to the buffer in.
 * \param path The path to the file to read.
 * \return The size of the loaded buffer, in bytes.
 */
std::size_t paper::util::io::loadFile(std::shared_ptr<uint8_t> &buf,
                                      const std::string &path)
{
	std::basic_ifstream<uint8_t> in(
	        path.c_str(), std::ios_base::binary | std::ios_base::in);

	if(!(in.is_open() && in.good()))
		throw std::runtime_error("Reading file contents failed.");

	std::size_t size = paper::util::io::filesize(path);
	buf.reset(new uint8_t[size], [](uint8_t *p)
	          {
		delete[] p;
	});

	in.read(buf.get(), static_cast<std::streamsize>(size));

	return size;
}
