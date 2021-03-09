#include <fstream>
#include <Rcpp.h>
#include <R_ext/Riconv.h>

//' Read an Enconded File, Optionally Converting to Another Encoding
//'
//' This function relies on the 'iconv' facility available with R. Having 'iconv' is optional but
//' likely for most builds of R; see \code{capabilities("iconv")} to verify. Also note that
//' 'iconv', while portable, does not guarantee identical results across implementations and
//' operating systems.
//'
//' @param filename [string] A filename
//' @param encoding [string, optional] An encoding. If present, file content is converted
//' to the given encoding; if missing (as indicated by the default empty string) no conversion
//' is made.
//' @return A string
//' @seealso https://fishandwhistle.net/post/2021/using-rs-cross-platform-iconv-wrapper-from-cpp11/
//' @examples
//' ## example file from package 'uchardet' encoding as windows-1252
//' win1252file <- system.file("rawdata", "windows-1252.txt", package="RcppIconvExample")
//' win1252txt <- read_file(win1252file, "windows-1252")
//' utf8file <- system.file("rawdata", "utf8.txt", package="RcppIconvExample")
//' utf8txt <- read_file(utf8file, "UTF-8")
//' stopifnot(substr(win1252txt, 1, 62) == substr(utf8txt, 1, 62))
//' cat(win1252txt)
// [[Rcpp::export]]
std::string read_file(std::string filename, std::string encoding = "") {
    const int len = 2048;
    char buffer[len/2];

    std::ifstream file;
    file.open(filename, std::ifstream::in | std::ifstream::binary);

    file.read(buffer, len/2);
    size_t n_read = file.gcount();
    file.close();

    if (encoding == "") {       // no encoding given so return 'as is'
        return std::string(buffer, n_read);
    }

    std::string str_source(buffer, n_read);

    void* iconv_handle = Riconv_open("UTF-8", encoding.c_str());
    if (iconv_handle == ((void*) -1)) {
        Rcpp::stop("Can't convert from '%s' to 'UTF-8'", encoding.c_str());
    }

    const char* in_buffer = str_source.c_str();
    char utf8_buffer[len];
    char* utf8_buffer_mut = utf8_buffer;
    size_t in_bytes_left = n_read;
    size_t out_bytes_left = len;

    size_t result = Riconv(iconv_handle, &in_buffer, &in_bytes_left, &utf8_buffer_mut, &out_bytes_left);
    Riconv_close(iconv_handle);

    if (result == ((size_t) -1) || (in_bytes_left != 0)) {
        Rcpp::stop("Failed to convert file contents to UTF-8");
    }

    return std::string(utf8_buffer, len - out_bytes_left);
}
