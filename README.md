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
saveZstd(mtcars, file_dir)
mtcars2 <- readZstd(file_dir)

# Test code
stopifnot(identical(mtcars, mtcars2))
```


Links
-----

-   [zstandard official site](http://facebook.github.io/zstd/)
-   [snappy official site](https://github.com/google/snappy/)
-   [lz4 official site](https://github.com/lz4/lz4/)
