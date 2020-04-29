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
library(fastsave)

data(mtcars)
tmp_dir <- tempdir()

saveZstd(mtcars, file.path(tmp_dir, "mtcars"))
mtcars2 <- readZstd(mtcars)

stopifnot(identical(mtcars, mtcars2))
```


Links
-----

-   [zstandard official site](http://facebook.github.io/zstd/)
-   [snappy official site](https://github.com/google/snappy/)
-   [lz4 official site](https://github.com/lz4/lz4/)
