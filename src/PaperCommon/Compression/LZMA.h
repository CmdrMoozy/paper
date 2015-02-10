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

#ifndef PAPER_COMPRESSION_LZMA_H
#define PAPER_COMPRESSION_LZMA_H

#include <cstddef>
#include <cstdint>
#include <memory>

namespace paper
{
namespace compression
{
/**
 * This function compresses the given data, placing the result in the given
 * shared pointer and returning the size of the compressed result.
 *
 * \param dst The shared pointer to store the result inside.
 * \param src The buffer containing the data to compress.
 * \param srcSize The length of the input buffer.
 * \return The size of the result buffer.
 */
std::size_t lzmaCompress(std::shared_ptr<uint8_t> &dst, const uint8_t *src,
                         std::size_t srcSize);

/**
 * This function decompresses the given data, placing the result in the given
 * shared pointer and returning the size of the decompressed result.
 *
 * \param dst The shared pointer to store the result inside.
 * \param src The bufer containing the data to decompress.
 * \param srcSize The length of the input buffer.
 * \return The size of the result buffer.
 */
std::size_t lzmaDecompress(std::shared_ptr<uint8_t> &dst, const uint8_t *src,
                           std::size_t srcSize);
}
}

#endif
