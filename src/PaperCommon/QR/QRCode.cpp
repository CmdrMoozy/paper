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

#include "QRCode.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace
{
/**
 * This structure denotes the maximum byte capacity of each version of QR
 * codes, at each possible error correction level.
 */
const std::size_t CAPACITY_MAP[41][4] = {{0, 0, 0, 0},
                                         {17, 14, 11, 7},
                                         {32, 26, 20, 14},
                                         {53, 42, 32, 24},
                                         {78, 62, 46, 34},
                                         {106, 84, 60, 44},
                                         {134, 106, 74, 58},
                                         {154, 122, 86, 64},
                                         {192, 152, 108, 84},
                                         {230, 180, 130, 98},
                                         {271, 213, 151, 119},
                                         {321, 251, 177, 137},
                                         {367, 287, 203, 155},
                                         {425, 331, 241, 177},
                                         {458, 362, 258, 194},
                                         {520, 412, 292, 220},
                                         {586, 450, 322, 250},
                                         {644, 504, 364, 280},
                                         {718, 560, 394, 310},
                                         {792, 624, 442, 338},
                                         {858, 666, 482, 382},
                                         {929, 711, 509, 403},
                                         {1003, 779, 565, 439},
                                         {1091, 857, 611, 461},
                                         {1171, 911, 661, 511},
                                         {1273, 997, 715, 535},
                                         {1367, 1059, 751, 593},
                                         {1465, 1125, 805, 625},
                                         {1528, 1190, 868, 658},
                                         {1628, 1264, 908, 698},
                                         {1732, 1370, 982, 742},
                                         {1840, 1452, 1030, 790},
                                         {1952, 1538, 1112, 842},
                                         {2068, 1628, 1168, 898},
                                         {2188, 1722, 1228, 958},
                                         {2303, 1809, 1283, 983},
                                         {2431, 1911, 1351, 1051},
                                         {2563, 1989, 1423, 1093},
                                         {2699, 2099, 1499, 1139},
                                         {2809, 2213, 1579, 1219},
                                         {2953, 2331, 1663, 1273}};

/**
 * This function returns the minimum QR code version which can store the given
 * amount of bytes. If the given amount of bytes is larger than the maximum
 * possible capacity, an exception will be thrown.
 *
 * \param bytes The amount of bytes you want to store.
 * \param errorCorrection The desired level of error correction.
 * \return The minimum version for the given number of bytes.
 */
int getMinimumVersion(std::size_t bytes,
                      paper::qr::QRCode::ErrorCorrection errorCorrection)
{
	for(int i = 1; i <= 40; ++i)
	{
		if(getCapacity(i, errorCorrection) >= bytes)
			return i;
	}

	throw std::runtime_error("Too much data for one QR code.");
}
}

namespace paper
{
namespace qr
{
QRCode::QRCode(const uint8_t *data, std::size_t offset, std::size_t size)
        : code(nullptr)
{
	int version = getMinimumVersion(size, ErrorCorrection::Low);

	code = QRcode_encodeData(size, data + offset, version,
	                         QRecLevel::QR_ECLEVEL_L);

	if(code == nullptr)
	{
		std::string error(strerror(errno));
		switch(errno)
		{
		case EINVAL:
			error = "Invalid input object.";
			break;
		case ENOMEM:
			error = "Unable to allocate memory for input objects.";
			break;
		case ERANGE:
			error = "Input data is too large.";
			break;
		default:
			error = "Unknown error.";
			break;
		}
		throw std::runtime_error(error);
	}
}

QRCode::~QRCode()
{
	if(code != nullptr)
	{
		QRcode_free(code);
		code = nullptr;
	}
}

QRCode::ErrorCorrection QRCode::getErrorCorrection() const
{
	return ErrorCorrection::Low;
}

int QRCode::getVersion() const
{
	return code->version;
}

std::size_t QRCode::getWidth() const
{
	return static_cast<std::size_t>(std::abs(code->width));
}

const uint8_t *QRCode::getData() const
{
	return static_cast<const uint8_t *>(code->data);
}

bool QRCode::getCellColor(std::size_t x, std::size_t y) const
{
	std::size_t width(getWidth());

	if((x >= width) || (y >= width))
		throw std::runtime_error("Cell index out of bounds.");

	std::size_t i(y * width + x);

	return (getData()[i] & 1) ? true : false;
}

std::size_t getCapacity(int version, QRCode::ErrorCorrection errorCorrection)
{
	if((version < 1) || (version > 40))
		throw std::runtime_error("Invalid QR code version.");

	std::size_t errorCorrectionIdx =
	        static_cast<std::size_t>(errorCorrection);
	return CAPACITY_MAP[version][errorCorrectionIdx];
}

std::size_t getMaximumCapacity()
{
	return getCapacity(40, QRCode::ErrorCorrection::Low);
}
}
}
