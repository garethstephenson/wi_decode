use image::io::Reader as ImageReader;
use image::ImageFormat;
use image::EncodableLayout;
use std::fs::File;
use std::io::Cursor;
use std::io::Write;
mod samwid;

pub fn init_libary() {
    unsafe {samwid::InitLibrary()};
}
pub fn decode_image(data: Vec<u8>) -> Vec<u8>{

    let mut datamut = data.clone();
    let mut cmpimage = unsafe { *samwid::CreateStructures() };

    cmpimage.Size = datamut.len().try_into().unwrap();
    cmpimage.CmpData = datamut.as_mut_ptr();

    println!("{:?}", cmpimage);

    let rawimage = unsafe { *samwid::Decompress(&mut cmpimage) };

    let mut pgmvec: Vec<u8> = vec![];
    if rawimage.Color == 0 {
        let mut bwheader = format!("P5\n{} {}\n255\n", rawimage.Width, rawimage.Height)
            .as_bytes()
            .to_vec();
        pgmvec.append(&mut bwheader);
        let mut bytes = unsafe {
            Vec::from_raw_parts(
                rawimage.Raw,
                (rawimage.Width * rawimage.Height).try_into().unwrap(),
                (rawimage.Width * rawimage.Height).try_into().unwrap(),
            )
        };
        pgmvec.append(&mut bytes);
    } else if rawimage.Color == 1 {
        let mut cheader = format!("P6\n{} {}\n255\n", rawimage.Width, rawimage.Height)
            .as_bytes()
            .to_vec();
        pgmvec.append(&mut cheader);
        let mut bytes = unsafe {
            Vec::from_raw_parts(
                rawimage.Raw,
                (rawimage.Width * rawimage.Height * 3).try_into().unwrap(),
                (rawimage.Width * rawimage.Height * 3).try_into().unwrap(),
            )
        };
        pgmvec.append(&mut bytes);
    } else {
        panic!("Invalid colour");
    }
    let mut pnm = ImageReader::new(Cursor::new(pgmvec));
    pnm.set_format(ImageFormat::Pnm);
    let pnmimg = pnm.decode().expect("");
    let mut jpeg: Vec<u8> = Vec::new();
    pnmimg.write_to(&mut jpeg, image::ImageOutputFormat::Jpeg(180));
    unsafe { samwid::Cleanup() };
    return jpeg;
}
