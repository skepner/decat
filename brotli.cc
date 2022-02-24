// #include "brotli.hh"

// // ----------------------------------------------------------------------

// Brotli::~Brotli()
// {
//     if (decoder_)
//         BrotliDecoderDestroyInstance(decoder_);
// }

// // ----------------------------------------------------------------------

// std::string Brotli::decompress_and_check(std::string_view input, check_if_compressed cic)
// {
//     std::string output;
//     if (!decoder_)
//         decoder_ = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);
//     BrotliDecoderResult result = BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT;
//     const uint8_t* next_in = reinterpret_cast<const uint8_t*>(input.data());
//     // const auto* next_in = input.data();
//     size_t available_in = input.size();
//     while (result == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT) {
//         size_t available_out = 0;
//         result = BrotliDecoderDecompressStream(decoder_, &available_in, &next_in, &available_out, nullptr, nullptr);
//         const uint8_t* next_out = BrotliDecoderTakeOutput(decoder_, &available_out);
//         // fmt::print(">>>> result: {} available_out: {}\n", result, available_out);
//         if (available_out != 0)
//             output.insert(output.end(), next_out, next_out + available_out);
//     }
//     if (cic == check_if_compressed::yes && (result == BROTLI_DECODER_RESULT_SUCCESS || result == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT)) {
//         return output;
//     }
//     else if (!available_in && (result == BROTLI_DECODER_RESULT_SUCCESS || result == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT)) {
//         return output;
//     }
//     else
//         throw compressor_failed{"brotli decompression failed: " + std::to_string(result)};
// }

// // ----------------------------------------------------------------------

// bool Brotli::compressed(std::string_view input)
// {
//     try {
//         Brotli brotli;
//         brotli.decompress_and_check(input.substr(0, 100), check_if_compressed::yes);
//         return true;
//     }
//     catch (compressor_failed&) {
//         return false;
//     }
// }

// // ----------------------------------------------------------------------
