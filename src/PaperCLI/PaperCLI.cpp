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

#include "PaperCLI.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>

#include "PaperCommon/Functionality.h"
#include "PaperCommon/QR/QRCode.h"

namespace
{
void printGlobalHelp()
{
	std::cout << "Usage: PaperCLI [command] [options]\n\n";

	std::cout << "Commands:\n";
	std::cout << "\texport - Create a QR code containing data.\n";
}

void exportCommand(std::size_t argc, QStringList::const_iterator argit,
                   QStringList::const_iterator)
{
	if(argc < 1)
	{
		std::cout << "Usage: PaperCLI export [file]\n\n";

		std::cout << "Options:\n";
		std::cout << "\t[file] - The path to the file to "
		          << "encode.\n";

		return;
	}

	QString path = *(argit++);

	std::vector<std::shared_ptr<paper::qr::QRCode>> qrcode(
	        paper::encode(path.toStdString()));
}
}

namespace papercli
{
PaperCLI::PaperCLI() : QObject()
{
}

void PaperCLI::run()
{
	try
	{
		QStringList args = qApp->arguments();

		if(args.length() < 2)
		{
			printGlobalHelp();

			Q_EMIT finished();
			return;
		}

		if(args.at(1) == "export")
		{
			exportCommand(static_cast<size_t>(args.length() - 2),
			              args.cbegin() + 2, args.cend());
		}
		else
		{
			printGlobalHelp();
		}
	}
	catch(std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	catch(...)
	{
		std::cerr << "Exception: Unknown error.\n";
	}

	Q_EMIT finished();
	return;
}
}

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	papercli::PaperCLI exe;

	QObject::connect(&exe, SIGNAL(finished()), &app, SLOT(quit()));

	QTimer::singleShot(1, &exe, SLOT(run()));

	return app.exec();
}
