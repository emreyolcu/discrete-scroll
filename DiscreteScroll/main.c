#include <ApplicationServices/ApplicationServices.h>

#define DEFAULT_LINES 3
#define SIGN(x) (((x) > 0) - ((x) < 0))

static const CFStringRef AX_NOTIFICATION = CFSTR("com.apple.accessibility.api");
static bool TRUSTED;

static int LINES;

static CGEventRef tapCallback(CGEventTapProxy proxy,
                              CGEventType type, CGEventRef event, void *userInfo)
{
    if (CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous) == 0) {
        int delta = (int)CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1);
        CGEventSetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1, SIGN(delta) * LINES);
    }

    return event;
}

static void displayNoticeAndExit(CFStringRef alertHeader)
{
    CFUserNotificationDisplayNotice(
        0, kCFUserNotificationCautionAlertLevel,
        NULL, NULL, NULL,
        alertHeader, NULL, NULL
    );

    exit(EXIT_FAILURE);
}

static void notificationCallback(CFNotificationCenterRef center, void *observer,
                                 CFNotificationName name, const void *object,
                                 CFDictionaryRef userInfo)
{
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFRunLoopPerformBlock(
        runLoop, kCFRunLoopDefaultMode, ^{
            bool previouslyTrusted = TRUSTED;
            if ((TRUSTED = AXIsProcessTrusted()) && !previouslyTrusted)
                CFRunLoopStop(runLoop);
        }
    );
}

static bool getIntPreference(CFStringRef key, int *valuePtr)
{
    CFNumberRef number = (CFNumberRef)CFPreferencesCopyAppValue(
        key, kCFPreferencesCurrentApplication
    );
    bool got = false;
    if (number) {
        if (CFGetTypeID(number) == CFNumberGetTypeID())
            got = CFNumberGetValue(number, kCFNumberIntType, valuePtr);
        CFRelease(number);
    }

    return got;
}

int main(void)
{
    CFNotificationCenterRef center = CFNotificationCenterGetDistributedCenter();
    char observer;
    CFNotificationCenterAddObserver(
        center, &observer, notificationCallback, AX_NOTIFICATION, NULL,
        CFNotificationSuspensionBehaviorDeliverImmediately
    );
    CFDictionaryRef options = CFDictionaryCreate(
        kCFAllocatorDefault,
        (const void **)&kAXTrustedCheckOptionPrompt, (const void **)&kCFBooleanTrue, 1,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks
    );
    TRUSTED = AXIsProcessTrustedWithOptions(options);
    CFRelease(options);
    if (!TRUSTED)
        CFRunLoopRun();
    CFNotificationCenterRemoveObserver(center, &observer, AX_NOTIFICATION, NULL);

    if (!getIntPreference(CFSTR("lines"), &LINES))
        LINES = DEFAULT_LINES;

    CFMachPortRef tap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,
        CGEventMaskBit(kCGEventScrollWheel), tapCallback, NULL
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
