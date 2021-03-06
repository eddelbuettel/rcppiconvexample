#include <fstream>
#include <Rcpp.h>
#include <R_ext/Riconv.h>

//' Demo reading a file
//'
//' @seealso https://fishandwhistle.net/post/2021/using-rs-cross-platform-iconv-wrapper-from-cpp11/
//' @param filename [string] A filename
//' @param encoding [string] An encoding
//' @return A string
//' @examples
//' ## example file from package 'uchardet' encoding as windows-1252
//' win1252file <- system.file("rawdata", "windows-1252.txt", package="RcppIconvExample")
//' win1252txt <- demo_read_file_enc(win1252file, "windows-1252")
//' utf8file <- system.file("rawdata", "utf8.txt", package="RcppIconvExample")
//' utf8txt <- demo_read_file_enc(utf8file, "UTF-8")
//' stopifnot(substr(win1252txt, 1, 62) == substr(utf8txt, 1, 62))
//' cat(win1252txt)
// [[Rcpp::export]]
std::string demo_read_file(std::string filename) {
    const int len = 1024;
    char buffer[len];

    std::ifstream file;
    file.open(filename, std::ifstream::binary);

    file.read(buffer, len);
    size_t n_read = file.gcount();
    file.close();

    return std::string(buffer, n_read);
}

//' @rdname demo_read_file
// [[Rcpp::export]]
std::string demo_read_file_enc(std::string filename, std::string encoding) {
    char buffer[1024];

    std::ifstream file;
    file.open(filename, std::ifstream::in | std::ifstream::binary);

    file.read(buffer, 1024);
    size_t n_read = file.gcount();
    file.close();

    std::string str_source(buffer, n_read);

    void* iconv_handle = Riconv_open("UTF-8", encoding.c_str());
    if (iconv_handle == ((void*) -1)) {
        Rcpp::stop("Can't convert from '%s' to 'UTF-8'", encoding.c_str());
    }

    const char* in_buffer = str_source.c_str();
    char utf8_buffer[2048];
    char* utf8_buffer_mut = utf8_buffer;
    size_t in_bytes_left = n_read;
    size_t out_bytes_left = 2048;

    size_t result = Riconv(iconv_handle, &in_buffer, &in_bytes_left, &utf8_buffer_mut, &out_bytes_left);

    Riconv_close(iconv_handle);

    if (result == ((size_t) -1) || (in_bytes_left != 0)) {
        Rcpp::stop("Failed to convert file contents to UTF-8");
    }

    return std::string(utf8_buffer, 2048 - out_bytes_left);
}
