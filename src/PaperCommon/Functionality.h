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

#ifndef PAPER_FUNCTIONALITY_H
#define PAPER_FUNCTIONALITY_H

#include <memory>
#include <string>
#include <vector>

#include "PaperCommon/QR/QRCode.h"

namespace paper
{
/**
 * This function will encode the contents of the given file as a minimal set
 * of QR codes. If some error occurs, an appropriate exception will be thrown.
 *
 * \param path The path to the file to encode.
 * \return The set of QR codes containing the file's data.
 */
std::vector<std::shared_ptr<qr::QRCode>> encode(const std::string &path);

/**
 * This function will render the given QR codes as SVG images, writing the
 * resulting file(s) to the given output directory. The files will be named
 * according to the given base file name.
 *
 * \param path The directory to write output files to.
 * \param baseName The base name for each file.
 * \param codes The set of QR codes to render.
 */
void renderSVGs(const std::string &path, const std::string &baseName,
                const std::vector<std::shared_ptr<qr::QRCode>> &codes);
}

#endif
