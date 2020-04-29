#include <Rcpp.h>
#include "third_party/lz4/lib/lz4frame.h"

using namespace Rcpp;

// [[Rcpp::export]]
RawVector impl_lz4Compress(RObject object, int level)
{
  if (!is<RawVector>(object))
    stop("object type must be RAWSXP (raw vector), got %s", type2name(object));

  RawVector src = as<RawVector>(object);

  int max_level = LZ4F_compressionLevel_max();
  if (level > max_level)
  {
    warning("compression level should be <= %d, using %d", max_level, max_level);
    level = max_level;
  }

  LZ4F_preferences_t prefs = LZ4F_INIT_PREFERENCES;
  prefs.autoFlush = 1;
  prefs.compressionLevel = level;
  prefs.frameInfo.contentSize = src.size();

  auto buffer_size = LZ4F_compressFrameBound(src.size(), &prefs);
  auto buffer = std::unique_ptr<char[]>(new char[buffer_size]);

  auto output_size = LZ4F_compressFrame(buffer.get(), buffer_size,
                                        src.begin(), src.size(),
                                        &prefs);
  if (LZ4F_isError(output_size))
    stop("internal error %d in LZ4F_compressFrame()", output_size);

  auto output = RawVector(buffer.get(), buffer.get() + output_size / sizeof(*buffer.get()));

  return output;
}

// [[Rcpp::export]]
RawVector impl_lz4Uncompress(RObject raw)
{
  if (!is<RawVector>(raw))
    stop("object type must be RAWSXP (raw vector), got %s", type2name(raw));

  RawVector src = as<RawVector>(raw);

  LZ4F_dctx *dctx;
  LZ4F_errorCode_t err;

  err = LZ4F_createDecompressionContext(&dctx, LZ4F_VERSION);
  if (LZ4F_isError(err))
    stop("internal error %d in LZ4F_createDecompressionContext()", err);

  LZ4F_frameInfo_t frameInfo;
  size_t input_size = src.size();

  err = LZ4F_getFrameInfo(dctx, &frameInfo, src.begin(), &input_size);
  if (LZ4F_isError(err))
    stop("internal error %d in LZ4F_getFrameInfo()", err);

  size_t output_size = frameInfo.contentSize;
  if (output_size == 0)
    stop("lzDecompress currently requires lz4 compressions with --content-size option");

  RawVector output(output_size);

  size_t srcSize = src.size() - input_size;

  auto n = LZ4F_decompress(dctx,
                           output.begin(), &output_size,
                           src.begin() + input_size, &srcSize,
                           NULL);
  if (LZ4F_isError(n))
    stop("internal error %d in LZ4F_decompress()", n);
  if (n > 0)
    stop("lz4 decompression out buffer size mismatch");

  LZ4F_freeDecompressionContext(dctx);

  return output;
}
