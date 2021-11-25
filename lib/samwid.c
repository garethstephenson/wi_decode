// bindgen lib/samwid.h --output src/samwid.rs
// cargo build --target i686-unknown-linux-gnu --release
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include "samwid.h"

#include "winnt_types.h"
#include "pe_linker.h"
#include "ntoskernel.h"
#include "util.h"
#include "log.h"
#include "hook.h"

#define byte uint8_t
#define code int
void *someglobalvariable;

void GetCmpImage(WiCmpImage *cmpImage, char *imgFileName);
void SetDecompressionOptions(WiDecmpOptions *decmpOpts);
void FileImage(char *filename, WiRawImage *image);
WiDecmpOptions *reverseCreateDcmpOptions();
WiRawImage *reverseWiCreateRawImage();
WiCmpImage *reverseWiCreateCmpImage();

WiRawImage *__stdcall (*WiCreateRawImagePtr)(void);
WiCmpImage *__stdcall (*WiCreateCmpImagePtr)(void);
WiDecmpOptions *__stdcall (*WiCreateDecmpOptionsPtr)(void);
int __stdcall (*WiDecompressPtr)(WiDecmpOptions *DecmpOptions, WiRawImage *RawImage, WiCmpImage *CmpImage);
int __stdcall (*WiBeginDecompressPtr)(WiDecmpOptions *DecmpOptions, WiRawImage *RawImage, WiCmpImage *CmpImage);
void __stdcall (*WiDestroyRawImagePtr)(WiRawImage *RawImage);
void __stdcall (*WiDestroyCmpImagePtr)(WiCmpImage *CmpImage);
void __stdcall (*WiDestroyDecmpOptionsPtr)(WiDecmpOptions *DecompOptions);
void __stdcall (*WiFreeRawImageDataPtr)(WiRawImage *RawImage);

int __stdcall (*WiDecompressSubHeaderPtr)(
    WiDecmpOptions *DecmpOptions,
    WiRawImage *RawImage,
    WiCmpImage *CmpImage);

int __stdcall (*WiDecompressSubImagePtr)(
    WiDecmpOptions *DecmpOptions,
    WiRawImage *RawImage,
    WiCmpImage *CmpImage);
void __stdcall (*WiEndDecompressPtr)(
    WiDecmpOptions *DecmpOptions,
    WiRawImage *RawImage,
    WiCmpImage *CmpImage);

void GetCmpImage(WiCmpImage *cmpImage, char *cmpImgFileName)
{
  FILE *fp;
  fp = fopen(cmpImgFileName, "rb");
  fseek(fp, 0L, SEEK_END);
  if (fp)
  {
    // printf("%ld",cmpImage->Size);
  }
  cmpImage->Size = ftell(fp);
  rewind(fp);

  cmpImage->CmpData = (unsigned char *)malloc(cmpImage->Size + 1);
  fread(cmpImage->CmpData, 1, cmpImage->Size, fp);
  fclose(fp);
}

void FileImage(char *fileNamePrefix, WiRawImage *image)
{
  FILE *fp;
  char filename[128];

  switch (image->Color)
  {

  case 0: /* 8 bit grayscale image */
    sprintf(filename, "%s.pgm", fileNamePrefix);
    fp = fopen(filename, "wb");
    fprintf(fp, "P5\n%d %d\n255\n", image->Width, image->Height);
    fwrite(image->Raw, 1, image->Width * image->Height, fp);
    fclose(fp);
    break;

  case 1: /* 24 bit color image */
    sprintf(filename, "%s.ppm", fileNamePrefix);
    fp = fopen(filename, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", image->Width, image->Height);
    fwrite(image->Raw, 1, 3 * image->Width * image->Height, fp);
    fclose(fp);
    break;
  }
}


void SetDecompressionOptions(WiDecmpOptions *decmpOpts)
{

  decmpOpts->Smoothing = 1;       /* use wavelet smoothing */
  decmpOpts->Fast = 0;            /* do not use fast decompression method */
  decmpOpts->ReadNextByte = NULL; /* do not use user defined bitstream I/0 */
  decmpOpts->ReadParam = NULL;    /* do not use free variable */
  decmpOpts->Sharpening = 1;      /* wavelet sharpen image */
}

WiRawImage *image;
WiCmpImage *cmpImage;
WiDecmpOptions *decmpOpts;
int errorCode;

void InitLibrary()
{
  struct pe_image peimage = {
      .entry = (PVOID)0x100016d0,
      .name = "swi32.dll",
  };

  if (pe_load_library(peimage.name, &peimage.image, &peimage.size) == false)
  {
    printf("swi32.dll required \n");
    fflush(stdout);
    return 1;
  }

  // Handle relocations, imports, etc.
  link_pe_images(&peimage, 1);
  // Now you can lookup exports...
  if (get_export("WiCreateRawImage", &WiCreateRawImagePtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiCreateRawImage");
  }
  if (get_export("WiCreateCmpImage", &WiCreateCmpImagePtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiCreateCmpImage");
  }

  if (get_export("WiCreateDecmpOptions", &WiCreateDecmpOptionsPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiCreateDecmpOptions");
  }

  if (get_export("WiDecompress", &WiDecompressPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiDecompress");
  }

  if (get_export("WiFreeRawImageData", &WiFreeRawImageDataPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiFreeRawImageData");
  }

  if (get_export("WiDestroyRawImage", &WiDestroyRawImagePtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiDestroyRawImage");
  }

  if (get_export("WiDestroyCmpImage", &WiDestroyCmpImagePtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiDestroyCmpImage");
  }

  if (get_export("WiDestroyDecmpOptions", &WiDestroyDecmpOptionsPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiDestroyDecmpOptions");
  }
  if (get_export("WiBeginDecompress", &WiBeginDecompressPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiBeginDecompress");
  }
  if (get_export("WiEndDecompress", &WiEndDecompressPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiEndDecompress");
  }

  if (get_export("WiDecompressSubHeader", &WiDecompressSubHeaderPtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiDecompressSubHeader");
  }
  if (get_export("WiDecompressSubImage", &WiDecompressSubImagePtr) == -1)
  {
    errx(EXIT_FAILURE, "failed to resolve WiDecompressSubImage");
  }
}

WiCmpImage *CreateStructures()
{
  image = WiCreateRawImagePtr();
  cmpImage = WiCreateCmpImagePtr();
  decmpOpts = WiCreateDecmpOptionsPtr();

  return cmpImage;
}

WiRawImage *Decompress(WiCmpImage *cmpimage)
{
  SetDecompressionOptions(decmpOpts);
  errorCode = WiDecompress(decmpOpts, image, cmpimage);
  return image;
}
void Cleanup()
{
  free(cmpImage->CmpData); 
  WiDestroyRawImagePtr(image);
  WiDestroyCmpImagePtr(cmpImage);
  WiDestroyDecmpOptionsPtr(decmpOpts);
}

int WiDecompress(WiDecmpOptions *DecmpOptions, WiRawImage *RawImage, WiCmpImage *CmpImage)
{
  int iVar1;
  iVar1 = WiBeginDecompressPtr(DecmpOptions, RawImage, CmpImage);
  if (iVar1 == 0)
  {
    iVar1 = WiDecompressSubHeaderPtr(DecmpOptions, RawImage, CmpImage);
    if (iVar1 == 0)
    {
      iVar1 = WiDecompressSubImagePtr(DecmpOptions, RawImage, CmpImage);
    }
    WiEndDecompressPtr(DecmpOptions, RawImage, CmpImage);
  }
  return iVar1;
}
