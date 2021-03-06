
onWindows <- .Platform$OS.type == "windows"

txt <- demo_read_file(system.file("rawdata", "qc.txt", package="rcppstringtest"))

if (!onWindows) expect_true(identical(txt, "Québec\n"))
if (onWindows) expect_false(identical(txt, "Québec\n"))
