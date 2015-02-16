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

#ifndef PAPER_UTIL_FS_H
#define PAPER_UTIL_FS_H

#include <string>

namespace paper
{
namespace util
{
namespace fs
{
/**
 * This function will trim the trailing separator (if any) from the given path.
 *
 * \param path The path to process.
 */
void trimTrailingSeparator(std::string &path);

/**
 * This function will return the path to the parent directory of the given
 * file. If the given path already represents a directory, then no action is
 * taken.
 *
 * \param path The path to get a parent directory path from.
 * \return The given file's parent directory's path.
 */
std::string dirname(const std::string &path);

/**
 * This function will return the bare filename, excluding any directories, from
 * the given path. If the given path is a directory, then an exception will be
 * thrown instead.
 *
 * \param path The pat to get a filename from.
 * \return The filename portion of the given path.
 */
std::string filename(const std::string &path);

/**
 * This is a small utility which returns an absolute, normalized path obtained
 * by appending the given new path to the given base path.
 *
 * \param base The base path to append to.
 * \param a The new path portion to append.
 * \return The resulting full path.
 */
std::string appendPath(const std::string &base, const std::string &a);
}
}
}

#endif
