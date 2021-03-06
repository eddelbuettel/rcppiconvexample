
onWindows <- .Platform$OS.type == "windows"

win1252file <- system.file("rawdata", "windows-1252.txt", package="rcppstringtest")
win1252txt <- demo_read_file_enc(win1252file, "windows-1252")
utf8file <- system.file("rawdata", "utf8.txt", package="rcppstringtest")
utf8txt <- demo_read_file_enc(utf8file, "UTF-8")

expect_equal(substr(win1252txt, 1, 62), substr(utf8txt, 1, 62))
