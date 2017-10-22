#import <ApplicationServices/ApplicationServices.h>

#define SIGN(x) (((x) > 0) - ((x) < 0))
#define LINES 3
#define QUICK_SCROLL_LIMITATION 100
#define QUICK_SCROLL_LINES 12

CGEventRef cgEventCallback(CGEventTapProxy proxy, CGEventType type,
                           CGEventRef event, void *refcon)
{
    if (!CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous)) {
        int64_t delta = CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1);
        int64_t lines_to_scroll;

        if (delta < QUICK_SCROLL_LIMITATION && delta > QUICK_SCROLL_LIMITATION * -1) {
            lines_to_scroll = SIGN(delta) * LINES;
        } else {
            lines_to_scroll = SIGN(delta) * QUICK_SCROLL_LINES;
        }
        CGEventSetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1, lines_to_scroll);
    }
    
    return event;
}

int main(void)
{
    CFMachPortRef eventTap;
    CFRunLoopSourceRef runLoopSource;
    
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
                                1 << kCGEventScrollWheel, cgEventCallback, NULL);
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
    
    CFRelease(eventTap);
    CFRelease(runLoopSource);
    
    return 0;
}
