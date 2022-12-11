#import "LIDAppController.h"

int main(int argc, const char* argv[])
{
    @autoreleasepool {
        LIDAppController* appController = [[LIDAppController alloc] init];

        NSApplication* app = [NSApplication sharedApplication];
        [app setDelegate:appController];
        [app run];
    }

    return NSApplicationMain(argc, argv);
}
