//
//  DTCompatibility.h
//  DTCoreText
//
//  Created by Oliver Letterer on 09.04.12.
//  Copyright (c) 2012 Drobnik.com. All rights reserved.
//

#if TARGET_ATV
#import "CoreGraphics+utils.h"

@implementation DTColor
@synthesize CGColor = _CGColor;

+ (DTColor *)colorWithCGColor:(CGColorRef)cgColor
{
	return [[self alloc] initWithCGColor:cgColor];
}

- (DTColor *)initWithCGColor:(CGColorRef)cgColor
{
	if ((self = [super init])) {
		_CGColor = cgColor;
		if (_CGColor != NULL)
			CFRetain(_CGColor);
	}
	return self;
}

+ (DTColor *)colorWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha
{
	return [[self alloc] initWithRed:red green:green blue:blue alpha:alpha];
}

- (DTColor *)initWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha
{
	if ((self = [super init])) {
		CGFloat components[4] = { red, green, blue, alpha };
		_CGColor = CGColorCreate(CGColorSpaceDefault(), components);
	}
	return self;
}

- (void)dealloc
{
	if (_CGColor != NULL) {
		CFRelease(_CGColor);
		_CGColor = NULL;
	}
}

@end

@implementation DTImage : NSObject

+ (DTImage *)imageNamed:(NSString *)name
{
	return nil;
}

- (id)initWithContentsOfFile:(NSString *)path
{
	return nil;
}

- (id)initWithData:(NSData *)data
{
	return nil;
}

- (CGSize)size
{
	return CGSizeZero;
}

- (void)drawInRect:(CGRect)rect
{
	// nop
}

@end

@implementation DTFont : NSObject

- (NSString*)familyName
{
	return CFBridgingRelease(CTFontCopyFamilyName(_font));
}

- (NSString*)fontName
{
	return CFBridgingRelease(CTFontCopyFullName(_font));
}

- (CGFloat)pointSize
{
	return CTFontGetSize(_font);
}

- (CGFloat)ascender
{
	return CTFontGetAscent(_font);
}

- (CGFloat)descender
{
	return CTFontGetDescent(_font);
}

- (CGFloat)capHeight
{
	return CTFontGetCapHeight(_font);
}

- (CGFloat)xHeight
{
	return CTFontGetXHeight(_font);
}

- (CGFloat)lineHeight
{
    CGFloat lineHeight = 0.0;
	
    // Get the ascent from the font, already scaled for the font's size
    lineHeight += CTFontGetAscent(_font);
	
    // Get the descent from the font, already scaled for the font's size
    lineHeight += CTFontGetDescent(_font);
	
    // Get the leading from the font, already scaled for the font's size
    lineHeight += CTFontGetLeading(_font);
	
    return lineHeight;
}

- (CGFloat)leading
{
	return CTFontGetLeading(_font);
}

@end
#endif
