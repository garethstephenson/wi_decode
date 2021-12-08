extern crate clap;
use clap::{Arg, App};
use widecode;
use std::fs::File;
use std::io::{Read,Write};
use std::fs::{self};

fn get_file_as_byte_vec(filename: &String) -> Vec<u8> {
    let mut f = File::open(&filename).expect("no file found");
    let metadata = fs::metadata(&filename).expect("unable to read metadata");
    let mut buffer = vec![0; metadata.len() as usize];
    f.read(&mut buffer).expect("buffer overflow");

    buffer
}

fn main() {
    let matches = App::new("WI to JPEG converter")
                          .version("1.0")
                          .author("Gert Dreyer <gertdreyer@gmail.com>")
                          .arg(Arg::with_name("input")
                               .short("i")
                               .long("input")
                               .value_name("FILE")
                               .help("Sets an input file")
                               .takes_value(true)
                               .required(true)).
                          arg(Arg::with_name("output")
                               .short("o")
                               .long("output")
                               .value_name("FILE")
                               .help("Sets an output file")
                               .takes_value(true)
                               .required(true))
                          .get_matches();

    let inputfilename = matches.value_of("input").unwrap();
    let outputfilename = matches.value_of("output").unwrap();

    let inputfile = get_file_as_byte_vec(&inputfilename.to_string());

    widecode::init_libary();
    let jpeg = widecode::decode_image(inputfile);
    
    let mut outputfile = File::create(outputfilename).expect("Failed to create output file");
    outputfile.write_all(&jpeg).expect("Error writing output");
    
}
