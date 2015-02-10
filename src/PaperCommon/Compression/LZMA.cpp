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

#include "LZMA.h"

#include <lzma.h>

#include <climits>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

#include "PaperCommon/Util/Memory.h"
#include "PaperCommon/Util/Memstream.h"

namespace
{
constexpr uint64_t BUFFER_SIZE = 8192;

/**
 * This is a small utility function which converts an LZMA return code
 * to a human-readable string.
 *
 * \param code The return code to interpret.
 * \return A human-readable interpretation of the return code.
 */
std::string lzma_error_string(lzma_ret code)
{
	switch(code)
	{
	case LZMA_OK:
		return "";
	case LZMA_STREAM_END:
		return "End of stream was reached.";
	case LZMA_NO_CHECK:
		return "Input stream has no integrity check.";
	case LZMA_UNSUPPORTED_CHECK:
		return "Cannot calculatethe integrity check.";
	case LZMA_GET_CHECK:
		return "Integrity check type is now available.";
	case LZMA_MEM_ERROR:
		return "Cannot allocate memory.";
	case LZMA_MEMLIMIT_ERROR:
		return "Memory usage limit was reached.";
	case LZMA_FORMAT_ERROR:
		return "File format not recognized.";
	case LZMA_OPTIONS_ERROR:
		return "Invalid or unsupported options.";
	case LZMA_DATA_ERROR:
		return "Data is corrupt.";
	case LZMA_BUF_ERROR:
		return "No progress is possible.";
	case LZMA_PROG_ERROR:
		return "Programming error.";
	default:
		return "Unknown LZMA error.";
	}
}

/**
 * This is a simple utility function to fill the given input buffer
 * with data from the given file, for use with the given LZMA stream.
 *
 * This may also update the given action, depending on the state of the
 * given input file.
 *
 * \param inbuf The input buffer to fill using the existing pointer.
 * \param stream The LZMA stream that will use the new input buffer.
 * \param action The next action LZMA should take.
 * \param src The source file to read data from.
 */
void fillInputBuffer(const std::shared_ptr<uint8_t> &inbuf, lzma_stream &stream,
                     lzma_action &action, FILE *src)
{
	if(stream.avail_in == 0 && (feof(src) == 0))
	{
		stream.next_in = inbuf.get();

		std::size_t left = BUFFER_SIZE;
		while(left > 0)
		{
			std::size_t read =
			        fread(&(inbuf.get())[BUFFER_SIZE - left],
			              sizeof(uint8_t), left, src);

			left -= read;

			if(read == 0)
			{
				action = LZMA_FINISH;
				break;
			}
		}

		stream.avail_in = BUFFER_SIZE - left;
	}
}

/**
 * This is a very basic utility function which writes the contents of
 * the given output buffer to the given destination file.
 *
 * \param dst The destination file to write the data to.
 * \param stream The LZMA stream which produced this output.
 * \param outbuf The output buffer containing the data to write.
 */
void writeOutputBuffer(FILE *dst, lzma_stream &stream,
                       const std::shared_ptr<uint8_t> &outbuf)
{
	fwrite(outbuf.get(), BUFFER_SIZE - stream.avail_out, sizeof(uint8_t),
	       dst);
	stream.next_out = outbuf.get();
	stream.avail_out = BUFFER_SIZE;
}

/**
 * This function deals with the result of an lzma_code operation by
 * possibly writing any remaining output, throwing an exception (in
 * case of an error), or simply stopping the coding operation.
 *
 * \param dst The destination file to write output to, if applicable.
 * \param stream The LZMA stream producing output.
 * \param outbuf The output buffer potentially containing data to write.
 * \param src The source file input is being read from.
 * \param ret The LZMA reurn code being handled.
 * \return True if coding should continue, or false otherwise.
 */
bool handleReturnCode(FILE *dst, lzma_stream &stream,
                      const std::shared_ptr<uint8_t> &outbuf, FILE *src,
                      lzma_ret ret)
{
	if(ret != LZMA_OK)
	{
		/*
		 * If the return value indicates that we're
		 * done with LZMA {de,en}coding, we need to
		 * write out any remaining bytes.
		 */

		if(ret != LZMA_NO_CHECK && ret != LZMA_UNSUPPORTED_CHECK)
		{
			writeOutputBuffer(dst, stream, outbuf);
		}

		// Stop if we reached the end of the stream.

		if(ret == LZMA_STREAM_END)
		{
			if((stream.avail_in == 0) && (feof(src) != 0))
			{
				return false;
			}

			/*
			 * Since we've hit LZMA_STREAM_END, but
			 * there is available input and/or we
			 * haven't hit EOF, there is a problem.
			 */

			throw std::runtime_error("LZMA input data error.");
		}

		// Looks like some other problem occurred.

		throw std::runtime_error(lzma_error_string(ret));
	}

	return true;
}

/**
 * This is a fairly low level implementation of LZMA {en,de}coding
 * using basic FILE pointers. This can be used to implement a
 * higher-level LZMA API.
 *
 * \param compress Whether or not we should be in compress mode.
 * \param dst The file to write output data to.
 * \param src The file to read input data from.
 */
void lzmaRaw(bool compress, FILE *dst, FILE *src)
{
	lzma_stream stream = LZMA_STREAM_INIT;
	lzma_filter filters[2];
	lzma_action action = LZMA_RUN;
	lzma_check check = LZMA_CHECK_CRC32;
	lzma_ret ret;

	lzma_options_lzma options;
	lzma_lzma_preset(&options, LZMA_PRESET_EXTREME);

	filters[0].id = LZMA_FILTER_LZMA2;
	filters[0].options = &options;
	filters[1].id = LZMA_VLI_UNKNOWN;

	if(compress)
	{
		ret = lzma_stream_encoder(&stream, filters, check);
	}
	else
	{
		ret = lzma_stream_decoder(&stream, UINT64_MAX,
		                          LZMA_TELL_UNSUPPORTED_CHECK |
		                                  LZMA_CONCATENATED);
	}

	if(ret != LZMA_OK)
	{
		throw std::runtime_error(std::string("LZMA error: ") +
		                         lzma_error_string(ret));
	}

	auto inbuf(paper::util::makeSharedArray<uint8_t>(BUFFER_SIZE));
	memset(inbuf.get(), 0, sizeof(uint8_t) * BUFFER_SIZE);

	auto outbuf(paper::util::makeSharedArray<uint8_t>(BUFFER_SIZE));
	memset(outbuf.get(), 0, sizeof(uint8_t) * BUFFER_SIZE);

	stream.next_out = outbuf.get();
	stream.avail_out = BUFFER_SIZE;

	while(true)
	{
		fillInputBuffer(inbuf, stream, action, src);

		// Let liblzma do the actual work.
		ret = lzma_code(&stream, action);

		// Write the output if the output buffer is full.
		if(stream.avail_out == 0)
			writeOutputBuffer(dst, stream, outbuf);

		// Handle the return code, continuing if appropriate.
		if(!handleReturnCode(dst, stream, outbuf, src, ret))
			break;
	}
}

/**
 * This function performs an LZMA {en,de}coding operation using
 * high-level memory streams and so on. This is a basic utility which
 * can be used to implement either LZMA compression or decompression.
 *
 * \param compress Whether or not we should be in compress mode.
 * \param dst The shared pointer to store the result inside.
 * \param src The buffer containing the data to {en,de}code.
 * \param srcSize The length of the input buffer.
 * \return The size of the result buffer.
 */
std::size_t lzma(bool compress, std::shared_ptr<uint8_t> &dst,
                 const uint8_t *src, std::size_t srcSize)
{
	paper::util::Memstream dstStream;
	FILE *srcFile = fmemopen(const_cast<uint8_t *>(src),
	                         sizeof(uint8_t) * srcSize, "r");

	lzmaRaw(compress, dstStream.getFile(), srcFile);

	dst.reset(dstStream.getBuffer());
	return dstStream.getSize();
}
}

std::size_t paper::compression::lzmaCompress(std::shared_ptr<uint8_t> &dst,
                                             const uint8_t *src,
                                             std::size_t srcSize)
{
	return lzma(true, dst, src, srcSize);
}

std::size_t paper::compression::lzmaDecompress(std::shared_ptr<uint8_t> &dst,
                                               const uint8_t *src,
                                               std::size_t srcSize)
{
	return lzma(false, dst, src, srcSize);
}
