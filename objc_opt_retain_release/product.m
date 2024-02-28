#import "product.h"

static unsigned gNextProductId = 101;

@implementation Product

- (id)initWithName:(NSString *)name price:(NSNumber *)price stock:(NSNumber *)stock
{
    self = [super init];
    if (!self)
        return nil;

    self.id = [NSNumber numberWithInt:gNextProductId++];
    self.name = name;
    self.price = price;
    self.stock = stock;

    return self;
}

@end
