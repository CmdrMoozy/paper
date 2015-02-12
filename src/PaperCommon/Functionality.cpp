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

#include "Functionality.h"

#include "PaperCommon/Compression/LZMA.h"
#include "PaperCommon/QR/Coding.h"
#include "PaperCommon/Render/SVG.h"
#include "PaperCommon/Util/IO.h"

namespace paper
{
std::vector<std::shared_ptr<qr::QRCode>> encode(const std::string &path)
{
	// Load the contents of the given file.

	std::shared_ptr<uint8_t> buf(nullptr);
	std::size_t bufSize = util::io::loadFile(buf, path);

	// Compress the given file's contents.

	{
		std::shared_ptr<uint8_t> compressed;
		std::size_t compressedSize = compression::lzmaCompress(
		        compressed, buf.get(), bufSize);
		buf = compressed;
		bufSize = compressedSize;
	}

	// Encode some QR codes containing the input data.

	return qr::encode(buf.get(), bufSize);
}

void renderSVGs(const std::string &, const std::string &,
                const std::vector<std::shared_ptr<qr::QRCode>> &codes)
{
	for(std::shared_ptr<qr::QRCode> code : codes)
	{
		render::SVG svg(*code);
	}
}
}
