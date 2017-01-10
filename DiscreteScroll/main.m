#import <ApplicationServices/ApplicationServices.h>

#define SIGN(x) (((x) > 0) - ((x) < 0))
#define LINES 3

CGEventRef cgEventCallback(CGEventTapProxy proxy, CGEventType type,
                           CGEventRef event, void *refcon)
{
    if (!CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous)) {
        int64_t delta = CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1);
        int64_t sign = SIGN(delta);
        if (delta * sign < LINES) delta = sign * LINES; // clamp scrolling magnitude to a minimum
        
        CGEventSetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1, delta);
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
