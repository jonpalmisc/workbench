#pragma once

#import "product.h"

@interface Store : NSObject

@property (nonatomic, retain) NSNumber *id;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSNumber *capacity;
@property (nonatomic, retain) NSMutableArray *products;

- (id)initWithName:(NSString *)name andCapacity:(NSNumber *)capacity;

- (void)addProduct:(Product *)product;
- (Product *)productAtIndex:(NSUInteger)index;
- (NSUInteger)productCount;

@end
