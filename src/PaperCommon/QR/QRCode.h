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

#ifndef PAPER_QR_QRCODE_H
#define PAPER_QR_QRCODE_H

#include <cstdint>
#include <cstddef>

#include <qrencode.h>

namespace paper
{
namespace qr
{
/**
 * \brief This class denotes a single QR code image.
 */
class QRCode
{
public:
	/**
	 * \brief This enumeration defines the QR code error correction levels.
	 */
	enum class ErrorCorrection
	{
		Low = 0,
		Medium = 1,
		Quartile = 2,
		High = 3
	};

	/**
	 * This constructor creates a new QR code which contains the given
	 * data. The QR code's size will be minimal in order to store the given
	 * data.
	 *
	 * If too much data is given, or some other error occurs, then an
	 * exception will be thrown indicating that object creation has failed.
	 *
	 * \param data The data to encode.
	 * \param offset The offset in the given buffer to read data from.
	 * \param size The size of the data to encode.
	 */
	QRCode(const uint8_t *data, std::size_t offset, std::size_t size);

	/**
	 * This is our object's default destructor, which frees all of this
	 * object's internal resources.
	 */
	~QRCode();

	/**
	 * This function returns the level of error correction this QR code is
	 * encoded with.
	 *
	 * \return The error correction level for this QR code.
	 */
	ErrorCorrection getErrorCorrection() const;

	/**
	 * This function returns the version of this QR code.
	 *
	 * \return This QR code's version.
	 */
	int getVersion() const;

	/**
	 * This function returns the width of this QR code in cells, which is a
	 * function of its version.
	 *
	 * \return This QR code's width.
	 */
	std::size_t getWidth() const;

	/**
	 * This function returns the raw QR code data. This is an array of
	 * bytes, which is getWidth() * getWidth() long. Each byte contains the
	 * following data:
	 *
	 *     MSB 76543210 LSB
	 *         |||||||`- 1=black/0=white
	 *         ||||||`-- data and ecc code area
	 *         |||||`--- format information
	 *         ||||`---- version information
	 *         |||`----- timing pattern
	 *         ||`------ alignment pattern
	 *         |`------- finder pattern and separator
	 *         `-------- non-data modules (format, timing etc.)
	 *
	 * In general, the only bit which is particularly useful is
	 * particularly useful is the least significant bit, which indicates
	 * whether or not the cell is black or white.
	 *
	 * \return This QR code's raw data.
	 */
	const uint8_t *getData() const;

	/**
	 * This function returns the color of the cell at the given position
	 * in this QR code. A black cell is considered "on" or "filled", so we
	 * return true for black and false for whit.
	 *
	 * \param x The x coordinate of the desired cell.
	 * \param y The y coordinate of the desired cell.
	 * \return The color of the specified cell.
	 */
	bool getCellColor(std::size_t x, std::size_t y) const;

private:
	QRcode *code;

	QRCode(const QRCode &);
	QRCode &operator=(const QRCode &);
};

/**
 * This function returns the maximum number of bytes a QR code of the given
 * version and with the given error correction level can store.
 *
 * \param version The QR code version.
 * \param errorCorrection The error correction level.
 * \return The number of bytes the specified QR code can store.
 */
std::size_t getCapacity(int version, QRCode::ErrorCorrection errorCorrection);

/**
 * This function returns the absolute maximum amount of data a single QR code
 * can store.
 *
 * \return The maximum amount of data one QR code can store.
 */
std::size_t getMaximumCapacity();
}
}

#endif
