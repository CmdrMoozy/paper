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

#ifndef PAPER_UTIL_IO_H
#define PAPER_UTIL_IO_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace paper
{
namespace util
{
namespace io
{
/**
 * This function returns the size of the file denoted by the given path, in
 * bytes. If the file doesn't exist, or some error occurs, then an exception
 * will be thrown.
 *
 * \param path The path to the file to inspect.
 * \return The size of the given file, in bytes.
 */
std::size_t filesize(const std::string &path);

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
std::size_t loadFile(std::shared_ptr<uint8_t> &buf, const std::string &path);

/**
 * This function writes all of the given data to the file denoted by the given
 * path. Any existing contents of the given file will be overwritten.
 *
 * \param path The path to the file to write.
 * \param data The data to write to the file.
 * \param size The size of the given data buffer.
 */
void writeFile(const std::string &path, const char *data, std::size_t size);
}
}
}

#endif
