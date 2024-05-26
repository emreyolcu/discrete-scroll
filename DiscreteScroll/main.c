#include <ApplicationServices/ApplicationServices.h>

#define SIGN(x) (((x) > 0) - ((x) < 0))

static int LINES;

CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo)
{
    if (!CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous)) {
        int64_t delta = CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1);
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
    Boolean trusted = AXIsProcessTrustedWithOptions(options);
    CFRelease(options);
    if (!trusted)
        displayNoticeAndExit(
            CFSTR("Restart DiscreteScroll after granting it access to accessibility features.")
        );

    CFStringRef key = CFSTR("lines");
    CFNumberRef value = (CFNumberRef)CFPreferencesCopyAppValue(
        key, kCFPreferencesCurrentApplication
    );
    Boolean got = false;
    if (value) {
        got = CFNumberGetValue(value, kCFNumberIntType, &LINES);
        CFRelease(value);
    }
    if (!got)
        LINES = 3;

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
