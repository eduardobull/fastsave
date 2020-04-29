#include <Rcpp.h>
#include "third_party/snappy/snappy.h"

using namespace Rcpp;

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

    auto output = RawVector(buffer.get(), buffer.get() + output_size / sizeof(*buffer.get()));

    return output;
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
