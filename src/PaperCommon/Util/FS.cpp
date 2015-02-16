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

#include "FS.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <utility>

#include <QDir>
#include <QFileInfo>
#include <QString>

namespace
{
void makeAbsolute(std::string &path)
{
	QFileInfo info(QString::fromStdString(path));
	std::string p(info.absoluteFilePath().toStdString());
	path.assign(std::move(p));
}

void normalizeSeparators(std::string &path)
{
	std::string p(QDir::toNativeSeparators(QString::fromStdString(path))
	                      .toStdString());
	path.assign(std::move(p));
}

void normalize(std::string &path)
{
	makeAbsolute(path);
	normalizeSeparators(path);
	paper::util::fs::trimTrailingSeparator(path);
}
}

namespace paper
{
namespace util
{
namespace fs
{
void trimTrailingSeparator(std::string &path)
{
	normalizeSeparators(path);

	auto predicate = [](const char &c) -> bool
	{
		return c == QDir::separator().toLatin1();
	};

	path.erase(
	        std::find_if_not(path.rbegin(), path.rend(), predicate).base(),
	        path.end());
}

std::string dirname(const std::string &path)
{
	std::string p(path);
	normalize(p);
	QFileInfo info(QString::fromStdString(p));
	std::string ret = (info.isDir() ? info.absoluteFilePath()
	                                : info.absolutePath()).toStdString();
	normalize(ret);
	return ret;
}

std::string filename(const std::string &path)
{
	QFileInfo info(QString::fromStdString(path));

	if(info.isDir())
		throw std::runtime_error("No filename for a directory.");

	return info.fileName().toStdString();
}

std::string appendPath(const std::string &base, const std::string &a)
{
	std::string ret(base);
	normalize(ret);

	QFileInfo info(QString::fromStdString(a));
	if(!info.isRelative())
		throw std::runtime_error("Can't append absolute paths.");

	return ret + QDir::separator().toLatin1() + a;
}

bool exists(const std::string &p)
{
	std::string path(p);
	normalize(path);
	QFileInfo info(QString::fromStdString(path));
	return info.exists();
}

void mkpath(const std::string &p)
{
	std::string path(p);
	normalize(path);

	QFileInfo info(QString::fromStdString(path));
	if(info.exists())
	{
		if(!info.isDir())
		{
			throw std::runtime_error(
			        "Path exists but isn't a directory.");
		}

		return;
	}

	if(!info.dir().mkpath(QString::fromStdString(path)))
		throw std::runtime_error("Creating path failed.");
}
}
}
}
