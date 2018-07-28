# xbevd

xbevd, a simple X bell event daemon (based on xbelld & nxbelld).

Compile with `gcc -Wall -O2 xbevd.c -o xbevd -lX11` and put the executable into your PATH.

The program listens for X bell events and prints DISPLAY and the milliseconds since EPOCH to stdout.

Example usage:

    #!/bin/sh
    
    soundfile="/usr/share/sounds/freedesktop/stereo/complete.oga"
    timeout=500
    
    [ -f "$soundfile" ] || exit 1
    
    lastbell=0
    
    xbevd | while read -r disp time; do
    	if [ $(( time - lastbell )) -ge $timeout ]; then
    		( /usr/bin/ogg123 -d alsa -o dev:hw:0 -q "$soundfile" & )
    		lastbell=$time
    	fi
    done
