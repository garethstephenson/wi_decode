/*=========================================================================
  swi32.h
  Header file for Wavelet Image 32-bit SDK

  Version: 4.00

  Created:  Dec 20, 1994
  Revised:  Jan 17, 1997

  Copyright (c) 1994-1997 by Summus, Ltd. All rights reserved.
==========================================================================*/

#ifndef _SWI32_H_
#define _SWI32_H_
#include <stdint.h>
#include <stddef.h>
/*
** Defines the calling conventions for different platforms. 
*/
#ifndef CALLSPEC
#  if (defined __WIN32__)||(defined _WIN32)||(defined __NT__)||\
      (defined __WINDOWS__)||(defined _Windows)
     /* For MS-Windows Platforms */
#    include <windows.h>
#    define CALLSPEC PASCAL
#  elif (defined __OS2__)                /* For OS/2 Platform */
#    include <os2.h>
#    define CALLSPEC APIENTRY
#  else                                  /* Other platforms(UNIX, MAC...) */
#    define CALLSPEC
#  endif
#endif /* CALLSPEC */

/*
** variable argument list styles
*/
#ifndef ARGSPEC
#  if (defined FUNCPROTO)||(__STDC__)||(defined __cplusplus)||\
      (defined __TURBOC__)     /* ANSI C, C++ */
#    define ARGSPEC(alist) alist
#  else                                                      /* C Classic */
#    define ARGSPEC(alist) ()
#  endif
#endif /* ARGSPEC */


/*
** Progressive
*/
#define PR_NONE   0
#define PR_NORMAL 1
#define PR_FAST   2

/*
** Encoders
*/
#define E_SLOW    0
#define E_NORMAL  1
#define E_FAST    2
#define E_FASTEST 3

/*
** EncodePaths
*/
#define P_PATH1   0
#define P_PATH2   1
#define P_PATH3   2

/*
** Split Block-Size Specifications
*/
#define SP_OFF    0
#define SP_AUTO   (-1)
#define SP_USER   (-2)

/*
** Image Documentation Spec
*/
#define SP_BKGRND  (0)
#define SP_LAST   (-1)
#define SP_ALL    (-2)


/*
** Compression Ratio Specifications
*/
#define CR_QUALITY        0
#define CR_CLOSEST        1
#define CR_FLOOR          2

/*
** Box structure (for focusing box)
*/
typedef struct
{
    int              Left;
    int              Top;
    int              Right;
    int              Bottom;
}WiBox;

/*
** Block structure
*/
typedef struct
{
    int              Page; //7
    int              PageLeft;//8
    int              PageTop;//9
    int              PageWidth;//10
    int              PageHeight;//11

    int              Block;//12
    int              BlockLeft;
    int              BlockTop;
    int              BlockWidth;
    int              BlockHeight;
}WiImageBlock;

/*
** Raw Image
*/
typedef struct
{
    unsigned char    *Raw; //0
    int              Height;
    int              Width; //2
    int              BitsPerPixel;
    int              Color; //4

    int              LevelHeight; //5
    int              LevelWidth;
    void             *AppData; //7

    unsigned char    *Comment;
    int              CommentLength;

    unsigned char    *AppExtension;
    int              AppExtensionLength;
}WiRawImage;

/*
** Compressed Image
*/
typedef struct
{
    unsigned char    *CmpData;
    int              Size;
}WiCmpImage;

/*
** byte I/O function pointer
*/
typedef int (*ByteIOFunc)(void*, int*);

/*
** image row I/O function pointer
*/
typedef int (*ScanlineIOFunc)(void*, WiImageBlock*, int, unsigned char**);

/*
** image extension I/O function pointer
*/
typedef int (*ExtensionIOFunc)(void*, int, unsigned char**, int*);

/*
** Compression options
*/
typedef struct{
    float            Quality;
    float            CmpRatio;
    int              CmpControl;

    int              Encoder;
    int              EncodePath;
    int              Progressive;

    ByteIOFunc       WriteNextByte;
    void             *WriteParam;

    int              Magnification;
    int              EdgeEnhancement;
    int              ContrastEnhancement;

    int              FocusWeight;
    WiBox            *FocusBoxes;
    int              nBoxes;

    int              HighColorQuality;

    ScanlineIOFunc   ReadScanline;
    void             *ReadScanlineParam;

    int              BlockSize;
    WiBox            *Blocks;
    int              nBlocks;
}WiCmpOptions;

/*
** Decompression Options
*/
typedef struct
{
    int              Smoothing; //0
    int              Fast;      //1

    ByteIOFunc       ReadNextByte; //2
    void             *ReadParam;  //3

    int              Sharpening; //4

    ScanlineIOFunc   WriteScanline; //5
    void             *WriteScanlineParam; //6

    WiImageBlock     SubImage; //7 - 16

    int              Magnification; //17

    ExtensionIOFunc  WriteAppExtension; //18
    void             *WriteAppExtensionParam; //19
}WiDecmpOptions;

/*
** Obsolete data structures. They will be removed in future releases.
*/
typedef struct
{
    int              left;
    int              top;
    int              right;
    int              bottom;
}SiBox;

typedef struct
{
    unsigned char    *Raw;
    int              Height;
    int              Width;
    int              BitsPerPixel;
    int              color;

    int              LevelHeight;
    int              LevelWidth;
    void             *AppData;
}SiImageInfo;

typedef struct
{
    unsigned char    *CmpData;
    int              size;
}SiCmpDataInfo;

typedef struct{
    float            Quality;
    float            CmpRatio;
    int              AutoRatio;

    int              Encoder;
    int              EncodePath;
    int              Progressive;

    ByteIOFunc       WriteNextByte;
    void             *WriteParam;

    int              Magnification;
    int              EdgeEnhancement;
    int              ContrastEnhancement;

    int              FocusWeight;
    SiBox            *FocusBoxes;
    int              nBoxes;
}SiCmpOption;

typedef struct
{
    int              Smoothing;
    int              Fast;

    ByteIOFunc       ReadNextByte;
    void             *ReadParam;

    int              Sharpening;

    ScanlineIOFunc   WriteScanline;
    void             *WriteScanlineParam;
}SiDecmpOption;


/*
** entry points
*/
#ifdef __cplusplus
extern "C"{
#endif

#if (defined __MC68K__)||(defined __POWERPC__)
#  if (__MC68K__)||(__POWERPC__) /* For mac platforms */
#    pragma export on
#  endif
#endif

WiRawImage* CALLSPEC WiCreateRawImage ARGSPEC((
        void
    ));

WiCmpImage* CALLSPEC WiCreateCmpImage ARGSPEC((
        void
    ));

WiCmpOptions* CALLSPEC WiCreateCmpOptions ARGSPEC((
        void
    ));

WiDecmpOptions* CALLSPEC WiCreateDecmpOptions ARGSPEC((
        void
    ));

void CALLSPEC WiDestroyRawImage ARGSPEC((
        WiRawImage        *RawImage
    ));

void CALLSPEC WiDestroyCmpImage ARGSPEC((
        WiCmpImage        *CmpImage
    ));

void CALLSPEC WiDestroyCmpOptions ARGSPEC((
        WiCmpOptions      *CmpOptions
    ));

void CALLSPEC WiDestroyDecmpOptions ARGSPEC((
        WiDecmpOptions    *DecompOptions
    ));

int CALLSPEC WiCompress ARGSPEC((
        WiCmpOptions      *CmpOptions,
        WiRawImage        *RawImage,
        WiCmpImage        *CmpImage
    ));

int CALLSPEC WiDecompress ARGSPEC((
        WiDecmpOptions    *DecmpOptions,
        WiRawImage        *RawImage,
        WiCmpImage        *CmpImage
    ));

int CALLSPEC WiBeginDecompress ARGSPEC((
        WiDecmpOptions    *DecmpOptions,
        WiRawImage        *RawImage,
        WiCmpImage        *CmpImage
    ));

void CALLSPEC WiEndDecompress ARGSPEC((
        WiDecmpOptions    *DecmpOptions,
        WiRawImage        *RawImage,
        WiCmpImage        *CmpImage
    ));

int CALLSPEC WiDecompressSubHeader ARGSPEC((
        WiDecmpOptions    *DecmpOptions,
        WiRawImage        *RawImage,
        WiCmpImage        *CmpImage
    ));

int CALLSPEC WiDecompressSubImage ARGSPEC((
        WiDecmpOptions    *DecmpOptions,
        WiRawImage        *RawImage,
        WiCmpImage        *CmpImage
    ));

int CALLSPEC WiGetProgressiveImage ARGSPEC((
        WiRawImage        *RawImage
    ));

void CALLSPEC WiFreeRawImageData ARGSPEC((
        WiRawImage        *RawImage
    ));

void CALLSPEC WiFreeCmpImageData ARGSPEC((
        WiCmpImage        *CmpImage
    ));

int CALLSPEC WiInsertCmpImage ARGSPEC((
        WiImageBlock      *BlockIndex,
        WiCmpImage        *SrcCmpImage,
        WiCmpImage        *InsCmpImage,
        WiCmpImage        *DesCmpImage
    ));

/*
** Obsolete functions. They will be removed in future releases.
*/

int CALLSPEC SiCompress ARGSPEC((
        SiImageInfo       *RawImage,
        SiCmpOption       *CmpOptions,
        SiCmpDataInfo     *CmpImage
    ));

int CALLSPEC SiDecompress ARGSPEC((
        SiCmpDataInfo     *CmpImage,
        SiDecmpOption     *DecmpOptions,
        SiImageInfo       *RawImage
    ));

int CALLSPEC SiGetImageInfo ARGSPEC((
        SiImageInfo       *ImageInfo
    ));

void CALLSPEC SiFreeImageInfo ARGSPEC((
        SiImageInfo       *ImageInfo
    ));

void CALLSPEC SiFreeCmpDataInfo ARGSPEC((
        SiCmpDataInfo     *CmpDataInfo
    ));

void InitLibrary();
WiCmpImage * CreateStructures();
WiRawImage * Decompress(WiCmpImage* cmpimage);
void Cleanup();

#if (defined __MC68K__)||(defined __POWERPC__)
#  if (__MC68K__)||(__POWERPC__)
#    pragma export off
#  endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _SWI32_H_ */
