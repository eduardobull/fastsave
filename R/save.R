# #' @export
# saveLZ4 <- function(object, path, level = 1) {
#   object_comp <- do_lzCompress(object, 1)

#   con <- file(path, "wb")
#   on.exit(close(con))

#   writeBin(object_comp, con)
# }

#' @export
saveLZ4 <- function(object, path, level = 1) {
  object_comp <- do_lzCompress(object, 1)

  con <- file(path, "wb")
  on.exit(close(con))

  writeBin(object_comp, con)
}

#' @export
readLZ4 <- function(path, level = 1) {
  con <- file(path, "rb")
  on.exit(close(con))

  raw <- readBin(con, "raw", n = file.size(path))
  do_lzUncompress(raw)
}

#' @export
saveZSTD <- function(object, path, level = 1) {
  object_comp <- do_zstdCompress(object, 1)

  con <- file(path, "wb")
  on.exit(close(con))

  writeBin(object_comp, con)
}

#' @export
readZSTD <- function(path, level = 1) {
  con <- file(path, "rb")
  on.exit(close(con))

  raw <- readBin(con, "raw", n = file.size(path))
  do_zstdUncompress(raw)
}

#' @export
saveSNP <- function(object, path, level = 1) {
  object_comp <- do_snappyCompress(object, 1)

  con <- file(path, "wb")
  on.exit(close(con))

  writeBin(object_comp, con)
}

#' @export
readSNP <- function(path, level = 1) {
  con <- file(path, "rb")
  on.exit(close(con))

  raw <- readBin(con, "raw", n = file.size(path))
  do_snappyUncompress(raw)
}
