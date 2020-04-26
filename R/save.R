#' @export
saveObject <- function(object, path, compressor = c("zstd", "snappy", "lz4"), ...) {
  compressor <-  switch(match.arg(compressor),
                        snappy = compressSNPY,
                        zstd = compressZSTD,
                        lz4 = compressLZ4)

  object_comp <- compressor(serialize(object, connection = NULL), ...)

  con <- file(path, "wb")
  on.exit(close(con))

  writeBin(object_comp, con)
}

#' @export
readObject <- function(path, decompressor = c("zstd", "snappy", "lz4"), ...) {
  decompressor <-  switch(match.arg(decompressor),
                          snappy = decompressSNPY,
                          zstd = decompressZSTD,
                          lz4 = decompressLZ4)

  con <- file(path, "rb")
  on.exit(close(con))

  raw <- readBin(con, "raw", n = file.size(path))
  unserialize(decompressor(raw, ...))
}

#' @export
saveLZ4 <- function(object, path, level = 1) {
  saveObject(object, path, "lz4", level = level)
}

#' @export
readLZ4 <- function(path) {
  readObject(path, "lz4")
}

#' @export
saveZSTD <- function(object, path, level = 1) {
  saveObject(object, path, "zstd", level = level)
}

#' @export
readZSTD <- function(path) {
  readObject(path, "zstd")
}

#' @export
saveSNPY <- function(object, path) {
  saveObject(object, path, "snappy")
}

#' @export
readSNPY <- function(path) {
  readObject(path, "snappy")
}
