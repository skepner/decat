// #include "xz.hh"

// // ----------------------------------------------------------------------

// XZ::~XZ()
// {
//     if (initialized_)
//         lzma_end(&strm_);
// }

// // ----------------------------------------------------------------------

// namespace xz_internal
// {
//     const unsigned char Signature[] = {0xFD, '7', 'z', 'X', 'Z', 0x00};
//     constexpr size_t BufSize = 409600;
// }

// bool XZ::compressed(std::string_view input)
// {
//     if (input.size() < sizeof(xz_internal::Signature))
//         return false;
//     return std::memcmp(input.data(), xz_internal::Signature, sizeof(xz_internal::Signature)) == 0;
// }

// // ----------------------------------------------------------------------

// std::string XZ::decompress(std::string_view input)
// {
//     if (!initialized_) {
//         strm_ = LZMA_STREAM_INIT;
//         if (lzma_stream_decoder(&strm_, UINT64_MAX, LZMA_TELL_UNSUPPORTED_CHECK | LZMA_CONCATENATED) != LZMA_OK)
//             throw compressor_failed("lzma decompression failed 1");
//         initialized_ = true;
//     }

//     strm_.next_in = reinterpret_cast<const uint8_t*>(input.data());
//     strm_.avail_in = input.size();
//     std::string output(xz_internal::BufSize, '\x00');
//     ssize_t offset = 0;
//     for (;;) {
//         strm_.next_out = reinterpret_cast<uint8_t*>(&*(output.begin() + offset));
//         strm_.avail_out = xz_internal::BufSize;
//         auto const r = lzma_code(&strm_, LZMA_RUN);
//         if (r == LZMA_STREAM_END) {
//             output.resize(static_cast<size_t>(offset) + xz_internal::BufSize - strm_.avail_out);
//             break;
//         }
//         else if (r == LZMA_OK) {
//             if (strm_.avail_out > 0) {
//                 output.resize(output.size() - strm_.avail_out);
//                 break;
//             }
//             else {
//                 offset += xz_internal::BufSize;
//                 output.resize(static_cast<size_t>(offset) + xz_internal::BufSize);
//             }
//         }
//         else {
//             throw compressor_failed("lzma decompression failed 2");
//         }
//     }
//     return output;
// }

// // ----------------------------------------------------------------------
