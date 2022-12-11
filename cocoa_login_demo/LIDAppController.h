#import <Cocoa/Cocoa.h>

#import "LIDMainViewController.h"

NS_ASSUME_NONNULL_BEGIN

@interface LIDAppController : NSResponder<NSApplicationDelegate>

@property (strong) NSWindow* mainWindow;
@property (strong) LIDMainViewController* viewController;

@end

NS_ASSUME_NONNULL_END

