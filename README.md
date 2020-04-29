[![Build Status](https://travis-ci.com/eduardobull/fastsave.svg?branch=master)](https://travis-ci.com/eduardobull/fastsave)

About
-----

Dot not use this package.


Installation
------------

To install development version from GitHub:

``` r
devtools::install_github("eduardobull/fastsave")
```


Usage
-----

``` r
# Load package
library(fastsave)

# Test dataset and temporary file path
data(mtcars)
file_dir <- file.path(tempdir(), "mtcars")

# Package usage
saveZstd(mtcars, file_dir) # or saveLZ4
mtcars2 <- readZstd(file_dir) # or readLZ4

# Test code
stopifnot(identical(mtcars, mtcars2))
```


Links
-----

-   [zstandard official site](http://facebook.github.io/zstd/)
-   [lz4 official site](https://github.com/lz4/lz4/)
