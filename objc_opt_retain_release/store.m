#import "store.h"

static unsigned gNextStoreId = 1001;

@implementation Store

@synthesize products;

- (id)initWithName:(NSString *)name andCapacity:(NSNumber *)capacity
{
    self = [super init];
    if (!self)
        return nil;

    self.id = [NSNumber numberWithInt:gNextStoreId++];
    self.name = name;
    self.capacity = capacity;
    self.products = [[NSMutableArray alloc] initWithCapacity:[capacity intValue]];
    return self;
}

- (void)addProduct:(Product *)product
{
    [self.products addObject:product];
}

- (Product *)productAtIndex:(NSUInteger)index
{
    return [self.products objectAtIndex:index];
}

- (NSUInteger)productCount
{
    return self.products.count;
}

@end
