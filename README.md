# wi_decode

Rust Wrapper library to convert WI images to JPEG.

To compile the libary run:

1. Build the C loadlibrary wrapper

in `./lib/`

`make all`

2. Build the rust library

In `.` 

`cargo build --target i686-unknown-linux-gnu`

3. Build the example

To build and run the example:

```sh
cargo build --example witojpeg --target i686-unknown-linux-gnu
cp target/i686-unknown-linux-gnu/debug/examples/witojpeg .
cp lib/swi32.dll .
patchelf --set-rpath '$ORIGIN:$ORIGIN/lib' witojpeg
./witojpeg -i somewifile.wi -o somejpeg.jpeg
```



Run `patchelf --set-rpath '$ORIGIN' <your executable>` to set the binary to search for the library in the same diretory as the executable. Place the dll in the sam directory as the executable.

It is required to build with --target i686-unknown-linux-gnu as the dll is 32 bit and the wrapper library is also 32bit. 
