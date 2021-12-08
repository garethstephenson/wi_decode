# wi_decode

Rust Wrapper library to convert WI images to JPEG.

To compile the libary run:

in `./lib/`

`make all`

In `.` 

`cargo build`



Run `patchelf --set-rpath '$ORIGIN' <your executable>` to set the binary to search for the library in the same diretory as the executable
