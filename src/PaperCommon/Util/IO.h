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
std::size_t filesize(const std::string &);
std::size_t loadFile(std::shared_ptr<uint8_t> &, const std::string &);
}
}
}

#endif
