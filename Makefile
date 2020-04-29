THIRD_PARTY_DIR := src/third_party

define latest_version
$(shell curl --silent "https://api.github.com/repos/$(1)/releases/latest" | grep -Po '"tag_name": "\K.*?(?=")')
endef

default: install

prepare:
	autoconf
	Rscript --vanilla -e "Rcpp::compileAttributes()"

build:
	Rscript --vanilla -e "Rcpp::compileAttributes()"
	Rscript --vanilla -e "devtools::build()"
	Rscript --vanilla -e "devtools::build(binary = TRUE, args = c('--preclean'))"

install:
	autoconf
	Rscript --vanilla -e "Rcpp::compileAttributes()"
	R CMD INSTALL --no-multiarch --strip .

clean:
	find . -regex '.*\.s?o$$' -exec rm -v {} \;
	find . -regex '.*\.a$$' -exec rm -v {} \;
	rm -rfv autom4te.cache configure config.log config.status src/Makevars

update-lz4:
	$(eval lz4_version=$(call latest_version,lz4/lz4))
	rm -rf ${THIRD_PARTY_DIR}/lz4-*
	cd ${THIRD_PARTY_DIR} && \
		curl -L "https://github.com/lz4/lz4/archive/${lz4_version}.tar.gz" | tar xz
	sed -i 's/^lz4_version=.*/lz4_version="$(subst v,,${lz4_version})"/' configure.ac

update-zstd:
	$(eval zstd_version=$(call latest_version,facebook/zstd))
	rm -rf ${THIRD_PARTY_DIR}/zstd-*
	cd ${THIRD_PARTY_DIR} && \
		curl -L "https://github.com/facebook/zstd/archive/${zstd_version}.tar.gz" | tar xz
	sed -i 's/^zstd_version=.*/zstd_version="$(subst v,,${zstd_version})"/' configure.ac

update-snappy:
	$(eval snappy_version=$(call latest_version,google/snappy))
	rm -rf ${THIRD_PARTY_DIR}/snappy-*
	cd ${THIRD_PARTY_DIR} && \
		curl -L "https://github.com/google/snappy/archive/${snappy_version}.tar.gz" | tar xz
	sed -i 's/^snappy_version=.*/snappy_version="$(subst v,,${snappy_version})"/' configure.ac

update-all: update-lz4 update-snappy update-zstd

.PHONY: default build install clean update-all update-lz4 update-zstd update-snappy
