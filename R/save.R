#' @export
saveObject <- function(object, path, compressor = c("zstd", "snappy", "lz4"), ...) {
  object.ser <- serialize(object, connection = NULL)
  object.comp <- compress(object.ser, compressor, ...)

  writeBin(object.comp, path)
}

#' @export
readObject <- function(path, decompressor = c("auto", "zstd", "snappy", "lz4"), ...) {
  object.comp <- readBin(path, "raw", n = file.size(path))
  object.ser <- decompress(object.comp, decompressor, ...)

  unserialize(object.ser)
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
saveZSTD <- function(object, path, level = -1) {
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
