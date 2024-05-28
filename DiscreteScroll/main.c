#include <ApplicationServices/ApplicationServices.h>

#define DEFAULT_LINES 3
#define SIGN(x) (((x) > 0) - ((x) < 0))

static int LINES;

CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo)
{
    if (CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous) == 0) {
        int delta = (int)CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1);
        CGEventSetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1, SIGN(delta) * LINES);
    }

    return event;
}

void displayNoticeAndExit(CFStringRef alertHeader)
{
    CFUserNotificationDisplayNotice(
        0, kCFUserNotificationCautionAlertLevel,
        NULL, NULL, NULL,
        alertHeader, NULL, NULL
    );

    exit(EXIT_FAILURE);
}

int main(void)
{
    CFDictionaryRef options = CFDictionaryCreate(
        kCFAllocatorDefault,
        (const void **)&kAXTrustedCheckOptionPrompt, (const void **)&kCFBooleanTrue, 1,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks
    );
    bool trusted = AXIsProcessTrustedWithOptions(options);
    CFRelease(options);
    if (!trusted)
        displayNoticeAndExit(
            CFSTR("Restart DiscreteScroll after granting it access to accessibility features.")
        );

    CFNumberRef value = (CFNumberRef)CFPreferencesCopyAppValue(
        CFSTR("lines"), kCFPreferencesCurrentApplication
    );
    bool got = false;
    if (value) {
        if (CFGetTypeID(value) == CFNumberGetTypeID())
            got = CFNumberGetValue(value, kCFNumberIntType, &LINES);
        CFRelease(value);
    }
    if (!got)
        LINES = DEFAULT_LINES;

    CFMachPortRef tap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventScrollWheel), callback, NULL
    );
    if (!tap)
        displayNoticeAndExit(CFSTR("DiscreteScroll could not create an event tap."));
    CFRunLoopSourceRef source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
    if (!source)
        displayNoticeAndExit(CFSTR("DiscreteScroll could not create a run loop source."));
    CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopDefaultMode);
    CFRelease(tap);
    CFRelease(source);
    CFRunLoopRun();

    return EXIT_SUCCESS;
}
