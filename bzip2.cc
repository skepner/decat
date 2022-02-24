// #include <iostream>

// #include "bzip2.hh"

// // ----------------------------------------------------------------------

// namespace bz2_internal
// {
//     constexpr const unsigned char Signature[] = { 'B', 'Z', 'h' };
//     constexpr ssize_t BufSize = 409600;
// }

// // ----------------------------------------------------------------------

// bool BZip2::compressed(std::string_view input)
// {
//     if (input.size() < sizeof(bz2_internal::Signature))
//         return false;
//     return std::memcmp(input.data(), bz2_internal::Signature, sizeof(bz2_internal::Signature)) == 0;
// }

// // ----------------------------------------------------------------------

// BZip2::~BZip2()
// {
//     if (initialized_)
//         BZ2_bzDecompressEnd(&strm_);
// }

// // ----------------------------------------------------------------------

// std::string BZip2::decompress(std::string_view input)
// {
//     if (!initialized_) {
//         strm_.bzalloc = nullptr;
//         strm_.bzfree = nullptr;
//         strm_.opaque = nullptr;
//         if (BZ2_bzDecompressInit(&strm_, 0 /*verbosity*/, 0 /* not small */) != BZ_OK)
//             throw compressor_failed("bz2 decompression failed during initialization");
//         initialized_ = true;
//     }

//     strm_.next_in = const_cast<decltype(strm_.next_in)>(input.data());
//     strm_.avail_in = static_cast<decltype(strm_.avail_in)>(input.size());

//     std::string output;
//     output.resize(bz2_internal::BufSize);
//     ssize_t offset = 0;
//     for (;;) {
//         strm_.next_out = const_cast<decltype(strm_.next_out)>(output.data() + offset);
//         strm_.avail_out = bz2_internal::BufSize;
//         auto const r = BZ2_bzDecompress(&strm_);
//         if (r == BZ_OK) {
//             if (strm_.avail_out > 0) { // done
//                 output.resize(output.size() - strm_.avail_out);
//                 break;
//             }
//             else {
//                 offset += bz2_internal::BufSize;
//                 output.resize(static_cast<size_t>(offset + bz2_internal::BufSize));
//             }
//         }
//         else if (r == BZ_STREAM_END) {
//             output.resize(static_cast<size_t>(offset + bz2_internal::BufSize) - strm_.avail_out);
//             break;
//         }
//         else
//             throw compressor_failed("bz2 decompression failed, code: " + std::to_string(r));
//     }
//     return output;
// }

// // ----------------------------------------------------------------------
