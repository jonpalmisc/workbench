#pragma once

#import <Foundation/Foundation.h>

@interface Product : NSObject

@property (nonatomic, retain) NSNumber *id;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSNumber *price;
@property (nonatomic, retain) NSNumber *stock;

- (id)initWithName:(NSString *)name price:(NSNumber *)price stock:(NSNumber *)stock;

@end
