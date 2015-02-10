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

#ifndef PAPER_QR_CODING_H
#define PAPER_QR_CODING_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "PaperCommon/QR/QRCode.h"

namespace paper
{
namespace qr
{
/**
 * This function encodes all of the given data into one or more QR codes. The
 * QR codes produced will be minimal (in number and then in size) in order to
 * store all of the given data.
 *
 * \param data The data to encode.
 * \param size The size of the given data buffer.
 * \return The set of encoded QR codes.
 */
std::vector<std::shared_ptr<QRCode>> encode(uint8_t *data, std::size_t size);
}
}

#endif
