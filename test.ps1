

$FILENAME = $("tests/" + $args + ".c")

if(-not $(Test-Path -Path test_folder)) {
    New-Item -ItemType Directory test_folder
}

if(-not $(Test-Path -Path test)) {
    New-Item -ItemType Directory test
}

# Compile the test suite
clang -g $FILENAME -I include utf8.obj -nostdlib -mfma -l ciabatta.lib

# If succeeded run the test suite
if($?) {
    ./a.exe
}
