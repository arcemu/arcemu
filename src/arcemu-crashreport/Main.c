/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


struct {
	char uptime[256];
	char *revision;
	char *details;
} opts;

/* Sends a crashdump.log to sf.net, using curl
   TODO: wget support?
*/
int sendCrashdump() {
	char cmd[1024];
	int ret;

	snprintf(cmd, 1024, "curl --silent --header \"Expect:\" --form-string group_id=230683 --form-string atid=1081311 --form-string func=postadd --form-string category_id=100 --form-string artifact_group_id=100 --form-string summary=\"ArcEmu crashdump rev%s\" --form-string details=\"%s; Uptime = %s\" --form-string file_description=crashdump --form input_file=@crashdump.log --form-string submit=SUBMIT http://sourceforge.net/tracker/index.php &> /dev/null", opts.revision, opts.details, opts.uptime);
	printf("%s: sending crashdump.. '%s'\n", __FUNCTION__, cmd);
	ret = system(cmd);
	if (ret != 0) {
		fprintf(stderr, "%s: '%s' returned %d\n", __FUNCTION__, cmd, ret);
	}

	return ret;
}

void buildCrashdump(char *filename) {
	char cmd[1024];
	int ret;

	printf("%s: building crashdump from '%s'\n", __FUNCTION__, filename);
	snprintf(cmd, 1024, "gdb --batch --eval-command=\"bt ful\" arcemu-world %s &> crashdump.log", filename);
	ret = system(cmd);
	if (ret == 0) {
		char dstfile[1024];

		ret = sendCrashdump();
		if (ret == 0) {
			snprintf(dstfile, 1024, "sent.%s", filename);
			rename(filename, dstfile);
		}
	} else {
		fprintf(stderr, "%s: '%s' returned %d\n", __FUNCTION__, cmd, ret);
	}
}

int filter(const struct dirent *entry) {
	return strncmp(entry->d_name, "core", 4) == 0;
}

int main(int argc, char *argv[]) {
	struct dirent **list;
	struct tm tm;
	FILE *f;
	time_t t;
	int n, i;

	for (;;) {
		int c = getopt(argc, argv, "r:d:");
		if (c == -1) break;

		switch (c) {
			case 'r': opts.revision = strdup(optarg); break;
			case 'd': opts.details = strdup(optarg); break;
			default: printf("default\n");
		}
	}

	f = fopen("arcemu.uptime", "r");
	if (f == NULL) return 1;

	fscanf(f, "%ld", &t);
	fclose(f);

	gmtime_r(&t, &tm);
	snprintf(opts.uptime, 256, "%u days, %u hours, %u minutes, %u seconds.", tm.tm_yday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	/* Check for uptime, at last 10min, this way we will filter crashes 
	   by missing sql updates/wrong configs/etc */
	if (t < (10*60)) {
		return 0;
	}

	n = scandir(".", &list, filter, NULL);
	if (n != -1) {
		for (i=0; i<n; i++) {
			buildCrashdump(list[i]->d_name);
		}
	}

	return 0;
}

