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

	QRCode(const uint8_t *, std::size_t, std::size_t);
	~QRCode();

	ErrorCorrection getErrorCorrection() const;
	int getVersion() const;
	std::size_t getWidth() const;

	const uint8_t *getData() const;
	bool getCellColor(std::size_t, std::size_t) const;

private:
	QRcode *code;

	QRCode(const QRCode &);
	QRCode &operator=(const QRCode &);
};

std::size_t getCapacity(int version, QRCode::ErrorCorrection errorCorrection);
std::size_t getMaximumCapacity();
}
}

#endif
