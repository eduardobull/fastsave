#include <Rcpp.h>
#include "zstd.h"
#include "lz4frame.h"
#include "snappy.h"

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

  auto comp_object = RawVector(buffer.get(), buffer.get() + output_size / sizeof(*buffer.get()));

  return comp_object;
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

// [[Rcpp::export]]
RawVector impl_zstdCompress(RObject object, int level)
{
  if (!is<RawVector>(object))
    stop("object type must be RAWSXP (raw vector), got %s", type2name(object));

  RawVector src = as<RawVector>(object);

  int max_level = ZSTD_maxCLevel();
  if (level > max_level)
  {
    warning("level should be <= %d, using %d", max_level, max_level);
    level = max_level;
  }
  else if (level < 1)
  {
    warning("level should be >= 1, using 1");
    level = 1;
  }

  auto buffer_size = ZSTD_compressBound(src.size());
  auto buffer = std::unique_ptr<char[]>(new char[buffer_size]);

  auto output_size = ZSTD_compress(buffer.get(), buffer_size,
                                   src.begin(), src.size(),
                                   level);
  if (ZSTD_isError(output_size))
    stop("internal error %s in ZSTD_compress()", ZSTD_getErrorName(output_size));

  auto comp_object = RawVector(buffer.get(), buffer.get() + output_size / sizeof(*buffer.get()));

  return comp_object;
}

// [[Rcpp::export]]
RawVector impl_zstdUncompress(RObject raw)
{
  if (!is<RawVector>(raw))
    stop("object type must be RAWSXP (raw vector), got %s", type2name(raw));

  RawVector src = as<RawVector>(raw);

  auto output_size = ZSTD_getFrameContentSize(src.begin(), src.size());
  if (output_size == ZSTD_CONTENTSIZE_ERROR)
    stop("internal error in ZSTD_getFrameContentSize(): content size error");
  if (output_size == ZSTD_CONTENTSIZE_UNKNOWN)
    stop("internal error in ZSTD_getFrameContentSize(): size cannot be determined");
  if (output_size == 0)
    return (RawVector());

  RawVector output(output_size);

  auto nb = ZSTD_decompress(output.begin(), output.size(), src.begin(), src.size());
  if (ZSTD_isError(nb))
    stop("internal error in ZSTD_decompress(): %s", ZSTD_getErrorName(nb));

  return output;
}

// [[Rcpp::export]]
RawVector impl_snappyCompress(RObject object)
{
  if (!is<RawVector>(object))
    stop("object type must be RAWSXP (raw vector), got %s", type2name(object));

  RawVector src = as<RawVector>(object);

  auto buffer_size = snappy::MaxCompressedLength(src.size());
  auto buffer = std::unique_ptr<char[]>(new char[buffer_size]);

  size_t output_size;
  snappy::RawCompress((char *)src.begin(), src.size(), buffer.get(), &output_size);
  if (output_size <= 0)
    stop("internal error in snappy::Compress()");

  auto comp_object = RawVector(buffer.get(), buffer.get() + output_size / sizeof(*buffer.get()));

  return comp_object;
}

// [[Rcpp::export]]
RawVector impl_snappyUncompress(RObject raw)
{
  if (!is<RawVector>(raw))
    stop("object type must be RAWSXP (raw vector), got %s", type2name(raw));

  RawVector src = as<RawVector>(raw);

  size_t output_length;
  auto length_ok = snappy::GetUncompressedLength((char *)src.begin(), src.size(), &output_length);
  if (!length_ok)
    stop("internal error in snappy::GetUncompressedLength(): content size error");

  auto output = RawVector(output_length);

  auto ok = snappy::RawUncompress((char *)src.begin(), src.size(), (char *)output.begin());
  if (!ok)
    stop("internal error in snappy::Uncompress()");

  return output;
}
