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

#include "Coding.h"

#include <algorithm>

namespace paper
{
namespace qr
{
std::vector<std::shared_ptr<QRCode>> encode(uint8_t *data, std::size_t size)
{
	// Determine how many QR codes we'll need, by rounding up division.
	const std::size_t maxCapacity(getMaximumCapacity());
	std::size_t codes = 1 + ((size - 1) / maxCapacity);
	std::vector<std::shared_ptr<QRCode>> ret;

	// Encode each block of data.
	for(std::size_t code = 0; code < codes; ++code)
	{
		std::size_t offset(code * maxCapacity);
		std::size_t singleSize(std::min(size - offset, maxCapacity));

		ret.push_back(std::shared_ptr<QRCode>(
		        new QRCode(data, offset, singleSize)));
	}

	return ret;
}
}
}
