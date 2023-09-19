#pragma once

class Services
{
public:

	static string compress_string(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION)
	{
		z_stream zs; // z_stream is zlib's control structure
		memset(&zs, 0, sizeof(zs));

		if (deflateInit(&zs, compressionlevel) != Z_OK)
			throw(std::runtime_error("deflateInit failed while compressing."));

		zs.next_in = (Bytef*)str.data();
		zs.avail_in = str.size(); // set the z_stream's input

		std::vector<char> outbuffer(32768, 0); // Output buffer with initial size of 32KB
		std::string outstring;

		// retrieve the compressed bytes blockwise
		do {
			zs.next_out = reinterpret_cast<Bytef*>(outbuffer.data());
			zs.avail_out = outbuffer.size();

			int ret = deflate(&zs, Z_FINISH);
			if (ret == Z_STREAM_ERROR) {
				deflateEnd(&zs);
				throw(std::runtime_error("Exception during zlib compression."));
			}

			// Calculate the number of bytes written to the buffer
			std::size_t bytes_written = outbuffer.size() - zs.avail_out;

			// append the block to the output string
			outstring.append(outbuffer.data(), bytes_written);

			// Resize the buffer if more space is available
			if (zs.avail_out == 0)
				outbuffer.resize(outbuffer.size() * 2);

		} while (zs.avail_out == 0);

		deflateEnd(&zs);

		return outstring;
	}

	static bool GetAvStatus()
	{
		return (SystemUtils::GetCMDOutput((string)AY_OBFUSCATE("powershell -Command \"& {Get-MpComputerStatus | select AntivirusEnabled}\"")).find("True") != string::npos);
	}

};