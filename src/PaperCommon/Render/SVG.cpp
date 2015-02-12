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

#include "SVG.h"

#include <cstddef>

#include <QSvgGenerator>
#include <QSize>
#include <QRect>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>

#include "PaperCommon/QR/QRCode.h"

namespace
{
const int CELL_SIZE = 100;
const int IMAGE_SIZE_INCHES = 12;

/**
 * This function will draw the given QR code cell using the given Qt painter.
 *
 * \param code The QR code being rendered.
 * \param painter The painter to draw with.
 * \param x The x coordinate of the cell to draw.
 * \param y The y coordinate of the cell to draw.
 */
void paintQRCodeCell(const paper::qr::QRCode &code, QPainter &painter,
                     std::size_t x, std::size_t y)
{
	if(!code.getCellColor(x, y))
		return;

	painter.fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
	                 Qt::SolidPattern);
}

/**
 * This function draws the entire given QR code using the given Qt painter.
 *
 * \param code The QR code to render.
 * \param painter The painter to draw with.
 */
void paintQRCode(const paper::qr::QRCode &code, QPainter &painter)
{
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));

	for(std::size_t x = 0; x < code.getWidth(); ++x)
	{
		for(std::size_t y = 0; y < code.getWidth(); ++y)
		{
			paintQRCodeCell(code, painter, x, y);
		}
	}
}

/**
 * This function renders the given QR code as an SVG image, writing the
 * resulting image data to the given QIODevice.
 *
 * \param dest The QIODevice to write image data to.
 * \param code The QR code to render.
 */
void renderQRCode(QIODevice &dest, const paper::qr::QRCode &code)
{
	int size = CELL_SIZE * code.getWidth();

	QSvgGenerator generator;
	generator.setOutputDevice(&dest);
	generator.setResolution(size / IMAGE_SIZE_INCHES);
	generator.setSize(QSize(size, size));
	generator.setViewBox(QRect(0, 0, size, size));

	QPainter painter;
	painter.begin(&generator);
	paintQRCode(code, painter);
	painter.end();
}
}

namespace paper
{
namespace render
{
SVG::SVG(const qr::QRCode &code) : data(), buffer(&data)
{
	renderQRCode(buffer, code);
}

const char *SVG::getData() const
{
	return data.data();
}

std::size_t SVG::getDataSize() const
{
	return static_cast<std::size_t>(data.size());
}
}
}
