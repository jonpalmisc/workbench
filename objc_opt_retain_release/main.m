#import "product.h"
#import "store.h"

int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    Store *store = [[Store alloc] initWithName:@"Office Store" andCapacity:@10];
    [store addProduct:[[Product alloc] initWithName:@"Pencil" price:@1.50 stock:@20]];
    [store addProduct:[[Product alloc] initWithName:@"Pen" price:@2.25 stock:@14]];
    [store addProduct:[[Product alloc] initWithName:@"Eraser" price:@0.5 stock:@50]];

    NSLog(@"%@ (%@)", store.name, store.id);
    NSLog(@" ");
    NSLog(@"%s\t%-20s\t%s\t%s", "ID", "Name", "Price", "Stock");
    NSLog(@"------------------------------------------------------------");
    for (NSUInteger i = 0; i < store.productCount; ++i) {
        Product *product = [store productAtIndex:i];
        NSLog(@"%@\t%-20s\t$%.2f\t%@", product.id, product.name.UTF8String,
            product.price.floatValue, product.stock);
    }

    return 0;
}
