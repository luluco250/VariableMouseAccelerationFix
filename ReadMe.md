# Variable mouse acceleration fix

This is a tiny ASI plugin (injectable DLL) that prevents games from
accidentally enabling mouse acceleration on Windows.

## Why?

Old Windows games (usually pre-Windows 2000) may call a function named
`SystemInformationInfo` to temporarily disable mouse acceleration, so that they
can calculate mouse movement in a linear manner.

Unfortunately with Windows 2000 onwards Microsoft changed the way mouse
acceleration works and introduced a strange quirk in which calling the function
mentioned above *enables* mouse acceleration (known as "Enhance pointer
precision" in Windows' mouse settings).

This causes older games to behave with accelerated aim, hindering accuracy
and precision with quick movements.

Another quirk with this is that if the game crashes before it can restore the
mouse acceleration settings back to normal, the OS will remain with it enabled
until the user manually turns it back off.

## Existing workaround

A famous fix for this is [MarkC's Mouse Acceleration Fix](https://donewmouseaccel.blogspot.com/2010/03/markc-windows-7-mouse-acceleration-fix.html),
which works around this quirk by altering the acceleration curve in the Windows
registry to be linear, thus making so that mouse acceleration being enabled or
not doesn't matter: movement will **always** be linear.

That's a good approach, but it relies on working around the issue with a hack
and naturally requires different configurations for different DPI levels and
operating systems. There's also no guarantee it won't be patched back by a
Windows update and must be re-applied on every OS install.

It's still advantageous in the way that it'll work on every game without
further configuration or installing plugins though.

## How this plugin works

This plugin takes a different approach: it hooks into the function and waits
for it to receive parameters to alter mouse settings, and when it does it
simply does nothing, ignoring the request while telling the program the request
worked as normal.

Because of this, it entirely bypasses the quirk of enabling acceleration, it
will never be enabled to begin with, by simply loading the plugin with a loader
like [ThirteenAG's Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases).

## What about raw input?

If the game offers an option to use raw input, you should use that and don't
need bother with either this plugin nor the previously mentioned fix.

Raw input simply bypasses Windows' mouse acceleration entirely and does not
modify the user's settings in any way.

Games have come a long way since Windows 9X, so these workarounds are only
really needed with older games with no support for raw input, newer games that
use more modern approaches to handling mouse input won't see any differences.
