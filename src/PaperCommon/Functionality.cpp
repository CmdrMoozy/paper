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

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include <QDir>
#include <QFileInfo>
#include <QString>

#include "PaperCommon/Compression/LZMA.h"
#include "PaperCommon/QR/Coding.h"
#include "PaperCommon/Render/SVG.h"
#include "PaperCommon/Util/FS.h"
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

void renderSVGs(const std::string &p, const std::string &b,
                const std::vector<std::shared_ptr<qr::QRCode>> &codes)
{
	std::string path(util::fs::dirname(p));
	QString pathTemplate(
	        QString::fromStdString(util::fs::appendPath(path, b)) +
	        ".%1.svg");

	auto getOutputPath = [&pathTemplate, &codes](int i) -> QString
	{
		return pathTemplate.arg(i + 1, codes.size(), 10, QChar('0'));
	};

	// Check that our output directory and output files are valid.

	util::fs::mkpath(path);

	for(int i = 0; i < static_cast<int>(codes.size()); ++i)
	{
		std::string outPath(getOutputPath(i).toStdString());
		if(util::fs::exists(outPath))
		{
			throw std::runtime_error("File already exists: " +
			                         outPath);
		}
	}

	// Write each output file.

	for(int i = 0; i < static_cast<int>(codes.size()); ++i)
	{
		render::SVG svg(*codes[static_cast<std::size_t>(i)]);
		std::string outPath(getOutputPath(i).toStdString());
		util::io::writeFile(outPath, svg.getData(), svg.getDataSize());
	}
}
}
