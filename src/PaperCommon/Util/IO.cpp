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

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <functional>
#include <sstream>
#include <stdexcept>

#include <QDir>
#include <QFileInfo>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "PaperCommon/Util/Memory.h"

namespace paper
{
namespace util
{
namespace io
{
std::size_t filesize(const std::string &path)
{
	struct stat s;
	int r = stat(path.c_str(), &s);

	if(r != 0)
		throw std::runtime_error(strerror(errno));

	return static_cast<size_t>(s.st_size);
}

std::size_t loadFile(std::shared_ptr<uint8_t> &buf, const std::string &path)
{
	FILE *in = fopen(path.c_str(), "rb");
	if(in == nullptr)
		throw std::runtime_error(strerror(errno));

	std::size_t size = filesize(path);
	buf = makeSharedArray<uint8_t>(size);

	std::size_t read = fread(buf.get(), sizeof(uint8_t), size, in);
	if(read != size)
		throw std::runtime_error("Error reading file contents.");

	if(fclose(in) != 0)
		throw std::runtime_error(strerror(errno));

	return size;
}

void writeFile(const std::string &path, const char *data, std::size_t size)
{
	FILE *out = fopen(path.c_str(), "wb");
	if(out == nullptr)
		throw std::runtime_error(strerror(errno));

	std::size_t written = fwrite(data, sizeof(char), size, out);
	if((written / sizeof(char)) != size)
		throw std::runtime_error(strerror(errno));

	if(fclose(out) != 0)
		throw std::runtime_error(strerror(errno));
}
}
}
}
