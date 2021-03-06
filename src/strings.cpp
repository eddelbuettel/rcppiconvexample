#include <fstream>
#include <Rcpp.h>

//' Demo reading a file
//'
//' @seealso https://fishandwhistle.net/post/2021/using-rs-cross-platform-iconv-wrapper-from-cpp11/
//' @param filename A filename
//' @return A string
//' @examples
//' txt <- demo_read_file(system.file("rawdata", "qc.txt", package="rcppstringtest"))
//' cat(txt)
//' stopifnot(txt == "Québec\n" || .Platform$OS.type == "windows")
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
