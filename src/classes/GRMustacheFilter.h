// The MIT License
// 
// Copyright (c) 2013 Gwendal Roué
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import "GRMustacheAvailabilityMacros.h"


// =============================================================================
#pragma mark - <GRMustacheFilter>


/**
 * The protocol for implementing GRMustache filters.
 *
 * **Companion guide:** https://github.com/groue/GRMustache/blob/master/Guides/runtime/filters.md
 *
 * The responsability of a GRMustacheFilter is to transform a value into
 * another.
 *
 * For example, the tag `{{ uppercase(name) }}` uses a filter object that
 * returns the uppercase version of its input.
 *
 * @since v4.3
 */
@protocol GRMustacheFilter <NSObject>
@required

////////////////////////////////////////////////////////////////////////////////
/// @name Transforming Values
////////////////////////////////////////////////////////////////////////////////

/**
 * Applies some transformation to its input, and returns the transformed value.
 *
 * @param object  An object to be processed by the filter.
 *
 * @return A transformed value.
 *
 * @since v4.3
 */
- (id)transformedValue:(id)object AVAILABLE_GRMUSTACHE_VERSION_6_0_AND_LATER;

@end



// =============================================================================
#pragma mark - GRMustacheFilter

/**
 * The GRMustacheFilter class helps building mustache filters without writing a
 * custom class that conforms to the GRMustacheFilter protocol.
 *
 * **Companion guide:** https://github.com/groue/GRMustache/blob/master/Guides/runtime/filters.md
 *
 * @see GRMustacheFilter protocol
 *
 * @since v4.3
 */ 
@interface GRMustacheFilter : NSObject<GRMustacheFilter>

////////////////////////////////////////////////////////////////////////////////
/// @name Creating Filters
////////////////////////////////////////////////////////////////////////////////

/**
 * Returns a generic filter that executes the provided block when tranforming
 * a value.
 *
 * Should your filter process strings, refrain yourself from casting the
 * `id` value to a string using the [NSObject description] method. Instead, use
 * the stringFilterWithBlock: method.
 *
 * @param block   The block that transforms its input.
 *
 * @return a GRMustacheFilter object.
 *
 * @since v4.3
 *
 * @see stringFilterWithBlock:
 * @see variadicFilterWithBlock:
 */
+ (id<GRMustacheFilter>)filterWithBlock:(id(^)(id value))block AVAILABLE_GRMUSTACHE_VERSION_6_0_AND_LATER;

/**
 * Returns a string-oriented filter that executes the provided block when
 * tranforming a string.
 *
 * Unlike filters returned by the filterWithBlock: method, such a filter is
 * always given a string, even if the filter parameter in the mustache template
 * evaluates to some other kind of object.
 *
 * That string is the rendering of the filter parameter, before any HTML
 * escaping has been applied.
 *
 * For example, consider the `{{ f(x) }}` tag: should `x` evaluate to a number,
 * the `f` filter would be given the rendering of the number.
 *
 * This method is a shortcut. It is equivalent to the following lines of code:
 *
 *     // The string transformation block:
 *
 *     id(^block)(NSString *) = ^(NSString *string) {
 *         return <the transformed string>;
 *     };
 *
 *     // The filter:
 *
 *     id filter = [GRMustacheFilter filterWithBlock:^id(id value) {
 *     
 *         // Our transformation applies to strings, not to objects of type `id`.
 *         //
 *         // So let's transform the *rendering* of the object, not the object itself.
 *         //
 *         // However, we do not have the rendering yet. So we return a rendering
 *         // object that will eventually render the object, and transform the
 *         // rendering.
 *         
 *         return [GRMustache renderingObjectWithBlock:^NSString *(GRMustacheTag *tag, GRMustacheContext *context, BOOL *HTMLSafe, NSError **error) {
 *             
 *             // First render:
 *             
 *             NSString *rendering = [[GRMustache renderingObjectForObject:value] renderForMustacheTag:tag context:context HTMLSafe:HTMLSafe error:error];
 *             
 *             // And then apply our transformation:
 *             
 *             return block(rendering);
 *         }];
 *     }];
 *
 * @param block   The block that transforms its input.
 *
 * @return a GRMustacheFilter object.
 *
 * @since v6.9
 */
+ (id<GRMustacheFilter>)stringFilterWithBlock:(id(^)(NSString *string))block AVAILABLE_GRMUSTACHE_VERSION_6_9_AND_LATER;

/**
 * Returns a GRMustacheFilter object that executes the provided block, given an
 * array of arguments.
 *
 * Those filters can evaluate expressions like `{{ f(a,b) }}`.
 *
 * GRMustache will invoke the filter regardless of the number of arguments in
 * the template: `{{ f(a) }}`, `{{ f(a,b) }}` and `{{ f(a,b,c) }}` will provide
 * arrays of 1, 2, and 3 arguments respectively. It is your responsability to
 * check that you are provided with as many arguments as you expect.
 *
 * @param block   The block that transforms its input.
 *
 * @return a GRMustacheFilter object.
 *
 * @since v5.5
 *
 * @see filterWithBlock:
 */
+ (id<GRMustacheFilter>)variadicFilterWithBlock:(id(^)(NSArray *arguments))block AVAILABLE_GRMUSTACHE_VERSION_6_0_AND_LATER;

@end
