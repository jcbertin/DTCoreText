//
//  DTCompatibility.h
//  DTCoreText
//
//  Created by Oliver Letterer on 09.04.12.
//  Copyright (c) 2012 Drobnik.com. All rights reserved.
//

#pragma mark - iOS

#if TARGET_ATV

	#import "DTWeakSupport.h"

	#define DTCORETEXT_SUPPORT_NS_ATTRIBUTES 0
	#define DTCORETEXT_SUPPORT_NSPARAGRAPHSTYLE_TABS 0
	#define DTCORETEXT_NEEDS_ATTRIBUTE_REPLACEMENT_LEAK_FIX 1
	#define DTCORETEXT_FIX_14684188 0

	#define NSFoundationVersionNumber10_6_8 751.62


	@interface DTColor : NSObject
	{
	@private
		CGColorRef _CGColor;
	}
	@property(nonatomic,readonly) CGColorRef CGColor;

	+ (DTColor *)colorWithCGColor:(CGColorRef)cgColor;
	- (DTColor *)initWithCGColor:(CGColorRef)cgColor;

	+ (DTColor *)colorWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
	- (DTColor *)initWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
	@end

	@interface DTImage : NSObject
	@property(nonatomic,readonly) CGSize             size;             // reflects orientation setting. size is in pixels
	@property(nonatomic,readonly) CGImageRef         CGImage;          // returns underlying CGImageRef or nil if CIImage based
	- (CGImageRef)CGImage NS_RETURNS_INNER_POINTER CF_RETURNS_NOT_RETAINED;
	@property(nonatomic,readonly) NSInteger          imageOrientation; // this will affect how the image is composited
	@property(nonatomic,readonly) CGFloat            scale;

	+ (DTImage *)imageNamed:(NSString *)name;      // load from main bundle
	+ (DTImage *)imageWithCGImage:(CGImageRef)cgImage scale:(CGFloat)scale orientation:(NSInteger)orientation;

	- (id)initWithContentsOfFile:(NSString *)path;
	- (id)initWithData:(NSData *)data;

	- (void)drawInRect:(CGRect)rect;
	@end

	@interface DTFont : NSObject
	{
	@private
		CTFontRef	_font;
	}
	@property(nonatomic,readonly,DT_WEAK_PROPERTY) NSString *familyName;
	@property(nonatomic,readonly,DT_WEAK_PROPERTY) NSString *fontName;
	@property(nonatomic,readonly)      CGFloat   pointSize;
	@property(nonatomic,readonly)      CGFloat   ascender;
	@property(nonatomic,readonly)      CGFloat   descender;
	@property(nonatomic,readonly)      CGFloat   capHeight;
	@property(nonatomic,readonly)      CGFloat   xHeight;
	@property(nonatomic,readonly)      CGFloat   lineHeight;
	@property(nonatomic,readonly)      CGFloat   leading;
	@end

	typedef struct DTEdgeInsets {
		CGFloat top, left, bottom, right;  // specify amount to inset (positive) for each of the edges. values can be negative to 'outset'
	} DTEdgeInsets;

	static inline DTEdgeInsets DTEdgeInsetsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right) {
		DTEdgeInsets insets = {top, left, bottom, right};
		return insets;
	}

	typedef NS_ENUM(NSInteger, NSLineBreakMode) {		/* What to do with long lines */
		NSLineBreakByWordWrapping = 0,     	/* Wrap at word boundaries, default */
		NSLineBreakByCharWrapping,		/* Wrap at character boundaries */
		NSLineBreakByClipping,		/* Simply clip */
		NSLineBreakByTruncatingHead,	/* Truncate at head of line: "...wxyz" */
		NSLineBreakByTruncatingTail,	/* Truncate at tail of line: "abcd..." */
		NSLineBreakByTruncatingMiddle	/* Truncate middle of line:  "ab...yz" */
	};

	/* Values for NSWritingDirection */
	enum {
		NSWritingDirectionNatural       = -1,   // Determines direction using the Unicode Bidi Algorithm rules P2 and P3
		NSWritingDirectionLeftToRight   = 0,    // Left to right writing direction
		NSWritingDirectionRightToLeft   = 1     // Right to left writing direction
	};
	typedef NSInteger NSWritingDirection;

	#define kCTTextAlignmentLeft		kCTLeftTextAlignment
	#define kCTTextAlignmentRight		kCTRightTextAlignment
	#define kCTTextAlignmentCenter		kCTCenterTextAlignment
	#define kCTTextAlignmentJustified	kCTJustifiedTextAlignment
	#define kCTTextAlignmentNatural		kCTNaturalTextAlignment

	// runtime-check if NS-style attributes are allowed
	static inline BOOL DTCoreTextModernAttributesPossible()
	{
		return NO;
	}

	// runtime-check if CoreText draws underlines
	static inline BOOL DTCoreTextDrawsUnderlinesWithGlyphs()
	{
		return NO;
	}

	#define DTNSNumberFromCGFloat(x) [NSNumber numberWithFloat:x]

#elif TARGET_OS_IPHONE

	// Compatibility Aliases
	@compatibility_alias DTColor	UIColor;
	@compatibility_alias DTImage	UIImage;
	@compatibility_alias DTFont		UIFont;

	// Edge Insets
	#define DTEdgeInsets UIEdgeInsets
	#define DTEdgeInsetsMake(top, left, bottom, right) UIEdgeInsetsMake(top, left, bottom, right)

	// NS-style text attributes are possible with iOS SDK 6.0 or higher
	#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_5_1
		#define DTCORETEXT_SUPPORT_NS_ATTRIBUTES 1
	#endif

	// NSParagraphStyle supports tabs as of iOS SDK 7.0 or higher
	#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_6_1
		#define DTCORETEXT_SUPPORT_NSPARAGRAPHSTYLE_TABS 1
	#endif

	// iOS before 5.0 has leak in CoreText replacing attributes
	#if __IPHONE_OS_VERSION_MIN_REQUIRED < __IPHONE_5_0
		#define DTCORETEXT_NEEDS_ATTRIBUTE_REPLACEMENT_LEAK_FIX 1
	#endif

	// iOS 7 bug (rdar://14684188) workaround, can be removed once this bug is fixed
	#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_6_1
		#define DTCORETEXT_FIX_14684188 1
	#endif

	// constant for checking for iOS 6
	#define DTNSFoundationVersionNumber_iOS_6_0  992.00

	// constant for checking for iOS 7
	#define DTNSFoundationVersionNumber_iOS_7_0  1047.00


	// runtime-check if NS-style attributes are allowed
	static inline BOOL DTCoreTextModernAttributesPossible()
	{
#if DTCORETEXT_SUPPORT_NS_ATTRIBUTES
		if (floor(NSFoundationVersionNumber) >= DTNSFoundationVersionNumber_iOS_6_0)
		{
			return YES;
		}
#endif
		return NO;
	}

	// runtime-check if CoreText draws underlines
	static inline BOOL DTCoreTextDrawsUnderlinesWithGlyphs()
	{
		if (floor(NSFoundationVersionNumber) >= DTNSFoundationVersionNumber_iOS_7_0)
		{
			return YES;
		}
	
		return NO;
	}

#if TARGET_CPU_ARM64 || TARGET_CPU_X86_64
	#define DTNSNumberFromCGFloat(x) [NSNumber numberWithDouble:x]
#else
	#define DTNSNumberFromCGFloat(x) [NSNumber numberWithFloat:x]
#endif

#endif


#pragma mark - Mac


#if !TARGET_OS_IPHONE

	// Compatibility Aliases
	@compatibility_alias DTColor	NSColor;
	@compatibility_alias DTImage	NSImage;
	@compatibility_alias DTFont		NSFont;

	// Edge Insets
	#define DTEdgeInsets NSEdgeInsets
	#define DTEdgeInsetsMake(top, left, bottom, right) NSEdgeInsetsMake(top, left, bottom, right)

	// Mac supports NS-Style Text Attributes since 10.0
	#define DTCORETEXT_SUPPORT_NS_ATTRIBUTES 1
	#define DTCORETEXT_SUPPORT_NSPARAGRAPHSTYLE_TABS 1

	// theoretically MacOS before 10.8 might have a leak in CoreText replacing attributes
	#if __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_7
		#define DTCORETEXT_NEEDS_ATTRIBUTE_REPLACEMENT_LEAK_FIX 1
	#endif

	#ifndef COREGRAPHICS_UTILS_H_
	// NSValue has sizeValue on Mac, CGSizeValue on iOS
	#define CGSizeValue sizeValue

	// String functions named differently on Mac
	static inline NSString *NSStringFromCGRect(const CGRect rect)
	{
		return NSStringFromRect(NSRectFromCGRect(rect));
	}

	static inline NSString *NSStringFromCGSize(const CGSize size)
	{
		return NSStringFromSize(NSSizeFromCGSize(size));
	}

	static inline NSString *NSStringFromCGPoint(const CGPoint point)
	{
		return NSStringFromPoint(NSPointFromCGPoint(point));
	}
	#endif

	// runtime-check if NS-style attributes are allowed
	static inline BOOL DTCoreTextModernAttributesPossible()
	{
		return YES;
	}

	// runtime-check if CoreText draws underlines
	static inline BOOL DTCoreTextDrawsUnderlinesWithGlyphs()
	{
		return NO;
	}

	#define DTNSNumberFromCGFloat(x) [NSNumber numberWithDouble:x]
#endif

// this enables generic ceil, floor, abs, round functions that work for 64 and 32 bit
#include <tgmath.h>
