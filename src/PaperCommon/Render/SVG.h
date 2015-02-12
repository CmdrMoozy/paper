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

#include <QByteArray>
#include <QBuffer>

#include <cstddef>

namespace paper
{
namespace qr
{
class QRCode;
}

namespace render
{
/**
 * \brief This class encapsulates an SVG version of a given QR code.
 */
class SVG
{
public:
	/**
	 * Create a new SVG image version of the given raw QR code object.
	 *
	 * \param code The QR code to render.
	 */
	SVG(const qr::QRCode &code);

	/**
	 * This function returns a pointer to the resulting SVG image data.
	 *
	 * \return The given QR code's SVG image data.
	 */
	const char *getData() const;

	/**
	 * This function returns the length, in bytes, of the resulting SVG
	 * image data.
	 *
	 * \return The length of the given QR code's SVG image data.
	 */
	std::size_t getDataSize() const;

private:
	QByteArray data;
	QBuffer buffer;
};
}
}
