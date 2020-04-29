#' Alternative serialization Interface for Single Objects
#'
#' Functions to write a single \code{R} object to a compressed file and to restore it.
#'
#' @param object Object to serialize.
#' @param file A connection or the name of the file where the object is saved to or read from.
#' @param level Integer: the level of compression to be used.
#'
#' @details
#'
#' \code{lapply}, \code{sapply}, and \code{apply} are formula compatible versions of \code{base} \code{\link{lapply}}, \code{\link{sapply}} and \code{\link{apply}}
#'
#' @name fast-save
NULL

saveObject <- function(object, file, compressor = c("zstd", "snappy", "lz4"), ...) {
  object.ser <- serialize(object, connection = NULL)
  object.comp <- compress(object.ser, compressor, ...)

  writeBin(object.comp, file)
}

readObject <- function(file, decompressor = c("auto", "zstd", "snappy", "lz4"), ...) {
  object.comp <- readBin(file, "raw", n = file.size(file))
  object.ser <- decompress(object.comp, decompressor, ...)

  unserialize(object.ser)
}

#' @rdname fast-save
#' @export
saveLZ4 <- function(object, file, level = 1) {
  saveObject(object, file, "lz4", level = level)
}

#' @rdname fast-save
#' @export
readLZ4 <- function(file) {
  readObject(file, "lz4")
}

#' @rdname fast-save
#' @export
saveZstd <- function(object, file, level = -1) {
  saveObject(object, file, "zstd", level = level)
}

#' @rdname fast-save
#' @export
readZstd <- function(file) {
  readObject(file, "zstd")
}

#' @rdname fast-save
#' @export
saveSnappy <- function(object, file) {
  saveObject(object, file, "snappy")
}

#' @rdname fast-save
#' @export
readSnappy <- function(file) {
  readObject(file, "snappy")
}
