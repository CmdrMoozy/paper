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
#include <cstdio>
#include <cstring>
#include <stdexcept>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "PaperCommon/Util/Memory.h"

std::size_t paper::util::io::filesize(const std::string &path)
{
	struct stat s;
	int r = stat(path.c_str(), &s);

	if(r != 0)
		throw std::runtime_error(strerror(errno));

	return static_cast<size_t>(s.st_size);
}

std::size_t paper::util::io::loadFile(std::shared_ptr<uint8_t> &buf,
                                      const std::string &path)
{
	FILE *in = fopen(path.c_str(), "rb");
	if(in == nullptr)
		throw std::runtime_error(strerror(errno));

	std::size_t size = filesize(path);
	buf = makeSharedArray<uint8_t>(size);

	std::size_t read = fread(buf.get(), sizeof(uint8_t), size, in);
	if(read != size)
		throw std::runtime_error("Error reading file contents.");

	return size;
}
