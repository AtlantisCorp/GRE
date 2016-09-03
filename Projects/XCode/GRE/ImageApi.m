//
//  ImageApi.m
//  GRE
//
//  Created by Jacques Tronconi on 01/04/2016.
//
//
/*
#define OSXFUNCTIONS_OBJC
#include "OSXFunctions.h"

void GImageLoad(CFTypeRef* crefreturn, const char* filename)
{
    NSString* nsFilename = [[NSString alloc] initWithUTF8String:filename];
    NSImage* retimage = [[NSImage alloc] initByReferencingFile:nsFilename];
    *crefreturn = CFBridgingRetain(retimage);
    [nsFilename release];
}

void GImageUnload(CFTypeRef* crefreturn)
{
    NSImage* img = (__bridge NSImage*) *crefreturn;
    CFBridgingRelease(*crefreturn);
    
    [img release];
}

int GImageIsEmpty(const CFTypeRef* cref)
{
    //    const NSImage* img = (__bridge const NSImage*) *cref;
    return false;
}

size_t GImageGetWidth(const CFTypeRef* cref)
{
    const NSImage* img = (__bridge const NSImage*) *cref;
    return (size_t) [img size].width;
}

size_t GImageGetHeight(const CFTypeRef* cref)
{
    const NSImage* img = (__bridge const NSImage*) *cref;
    return (size_t) [img size].height;
}

unsigned char* GImageGetData(const CFTypeRef* cref)
{
    const NSImage* img = (__bridge const NSImage*) *cref;
    
    NSBitmapImageRep* bitmap = [NSBitmapImageRep alloc];
    NSSize imgSize = [img size];
    
    [img lockFocus];
    [bitmap initWithFocusedViewRect: NSMakeRect(0.0, 0.0, imgSize.width, imgSize.height)];
    [img unlockFocus];
    
    size_t sz = sizeof(unsigned char) * [bitmap bytesPerPlane];
    unsigned char* data = (unsigned char*) malloc(sz+1);
    memset(data, 0, sz);
    if(data) {
        const unsigned char* datasrc = [bitmap bitmapData];
        memcpy(data, datasrc, sz);
    }
    
    [bitmap release];
    return data;
}

int GImageGetSamples(const CFTypeRef* cref)
{
    const NSImage* img = (__bridge const NSImage*) *cref;
    
    NSBitmapImageRep* bitmap = [NSBitmapImageRep alloc];
    NSSize imgSize = [img size];
    
    [img lockFocus];
    bitmap = [bitmap initWithFocusedViewRect: NSMakeRect(0.0, 0.0, imgSize.width, imgSize.height)];
    [img unlockFocus];
    
    int samples = (int) [bitmap samplesPerPixel];
    [bitmap release];
    return samples;
}
*/