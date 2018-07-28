/*
 *  xbevd, a simple X bell event daemon (based on xbelld & nxbelld).
 *
 *  Copyright (C) 2018 Aaron G <mastaag [at] gmail.com>
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but HAVEOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <X11/XKBlib.h>

int main(int argc, char **argv)
{
	int event_code, xkb_error;
	int major = XkbMajorVersion;
	int minor = XkbMinorVersion;
	Display *display = XkbOpenDisplay(NULL, &event_code, NULL,
	                                  &major, &minor, &xkb_error);
	XkbEvent event;

	if (display == NULL)
	{
		switch (xkb_error)
		{
			case XkbOD_BadLibraryVersion:
				fprintf (stderr, "%s: Xkb version %d.%02d is required "
				                 "(got library %d.%02d).\n",
						argv[0], XkbMajorVersion, XkbMinorVersion,
						major, minor);
				break;
			case XkbOD_ConnectionRefused:
				fprintf (stderr, "%s: Cannot open the display: "
				                 "connection refused.\n",
						argv[0]);
				break;
			case XkbOD_NonXkbServer:
				fprintf (stderr, "%s: XKB extension not present.\n",
						argv[0]);
				break;
			case XkbOD_BadServerVersion:
				fprintf (stderr, "%s: Xkb version %d.%02d is required "
				                 "(got server %d.%02d).\n",
						argv[0], XkbMajorVersion, XkbMinorVersion,
						major, minor);
				break;
			default:
				fprintf (stderr, "%s: Unknown error %d from XkbOpenDisplay",
						argv[0], xkb_error);
				break;
		}

		return 1;
	}

	XkbSelectEvents(display, XkbUseCoreKbd,
			XkbBellNotifyMask, XkbBellNotifyMask);

	struct timeval now;
	char *DISPLAY = getenv("DISPLAY");

	while (1) {
		XNextEvent(display, &event.core);

		if (event.type != event_code) {
			continue;
		}

		gettimeofday(&now, NULL);

		fprintf(stdout, "%s %llu\n",
				DISPLAY,
				(unsigned long long)(now.tv_sec) * 1000 +
				(unsigned long long)(now.tv_usec) / 1000);
		fflush(stdout);
	}

	XCloseDisplay(display);
	return 0;
}