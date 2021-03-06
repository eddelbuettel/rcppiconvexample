
win1252file <- system.file("rawdata", "windows-1252.txt", package="RcppIconvExample")
win1252txt <- read_file(win1252file, "windows-1252")
utf8file <- system.file("rawdata", "utf8.txt", package="RcppIconvExample")
utf8txt <- read_file(utf8file, "UTF-8")
## test proper conversion of win1252 content to utf8
expect_equal(substr(win1252txt, 1, 62), substr(utf8txt, 1, 62))

## also test without explicitly encoding as UTF-8
txt <- read_file(utf8file)
expect_equal(substr(win1252txt, 1, 62), substr(txt, 1, 62))
