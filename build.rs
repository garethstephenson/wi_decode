use bindgen;

use std::env;
use std::path::PathBuf;


fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();
    // Tell cargo to tell rustc to link the system bzip2
    // shared library.
    println!("cargo:rustc-link-search=native=lib");
    println!("cargo:rustc-link-lib=dylib=samwid");
    // println!("cargo:rustc-cdylib-link-arg=-Wl,-rpath,{}","$ORIGIN");
    // println!("cargo:rustc-link-arg=-Wl,-rpath,{}","\\$ORIGIN");
    // println!("cargo:rustc-flags=-C link-args='Wl,-rpath=$ORIGIN'");

    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let bindings = bindgen::Builder::default()
        // The input header we would like to generate
        // bindings for.
        .header("lib/samwid.h")
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");


    let out_path = PathBuf::from(out_dir.clone());
    // Write the bindings to the $OUT_DIR/bindings.rs file.
    bindings
        .write_to_file(out_path.join("samwid.rs"))
        .expect("Couldn't write bindings!");
}
// patchelf --set-rpath '$ORIGIN' wrapper