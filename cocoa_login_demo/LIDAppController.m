#import "LIDAppController.h"

@implementation LIDAppController

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    self.viewController = [[LIDMainViewController alloc] init];
    self.mainWindow = [NSWindow windowWithContentViewController:self.viewController];

    [self.mainWindow makeKeyAndOrderFront:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
    return YES;
}

@end
