compress <- function(raw, compressor = c("zstd", "snappy", "lz4"), ...) {
  compressor <-  switch(match.arg(compressor),
                        snappy = impl_snappyCompress,
                        zstd = impl_zstdCompress,
                        lz4 = impl_lz4Compress)

  compressor(raw, ...)
}

decompress <- function(raw, decompressor = c("auto", "zstd", "snappy", "lz4"), ...) {
  zstd.sig <- as.raw(c(0x28, 0xb5, 0x2f, 0xfd))
  lz4.sig <- as.raw(c(0x04, 0x22, 0x4d, 0x18))

  file.sig <- head(raw, 4)
  decompressor <-  switch(match.arg(decompressor),
                          snappy = impl_snappyUncompress,
                          zstd = impl_zstdUncompress,
                          lz4 = impl_lz4Uncompress,
                          auto = if (identical(file.sig, zstd.sig))
                            impl_zstdUncompress
                          else if (identical(file.sig, lz4.sig))
                            impl_lz4Uncompress
                          else
                            impl_snappyUncompress)

  decompressor(raw, ...)
}

#' @export
compressLZ4 <- function(raw, level = 1, ...) {
  compress(raw, "lz4", level = level)
}

#' @export
decompressLZ4 <- function(raw) {
  decompress(raw, "lz4")
}

#' @export
compressZstd <- function(raw, level = 1, ...) {
  compress(raw, "zstd", level = level)
}

#' @export
decompressZstd <- function(raw) {
  decompress(raw, "zstd")
}

#' @export
compressSnappy <- function(raw, ...) {
  compress(raw, "snappy")
}

#' @export
decompressSnappy <- function(raw) {
  decompress(raw, "snappy")
}
