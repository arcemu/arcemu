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


void sendCrashdump() {
	char cmd[1024];
	int ret;

	snprintf(cmd, 1024, "curl -F crashdump=@crashdump.log http://www.arcemu.org/crashdump.php");
	printf("%s: sending crashdump.. '%s'\n", __FUNCTION__, cmd);
	ret = system(cmd);
	if (ret != 0) {
		fprintf(stderr, "%s: '%s' returned %d\n", __FUNCTION__, cmd, ret);
	}
}

void buildCrashdump(char *filename) {
	char cmd[1024];
	int ret;

	printf("%s: building crashdump from '%s'\n", __FUNCTION__, filename);
	snprintf(cmd, 1024, "gdb --batch --eval-command=\"bt ful\" arcemu-world %s &> crashdump.log", filename);
	ret = system(cmd);
	if (ret == 0) {
		char dstfile[1024];

		sendCrashdump();

		snprintf(dstfile, 1024, "sent.%s", filename);
		rename(filename, dstfile);
	} else {
		fprintf(stderr, "%s: '%s' returned %d\n", __FUNCTION__, cmd, ret);
	}
}

int filter(const struct dirent *entry) {
	return strncmp(entry->d_name, "core.", 5) == 0;
}

int main(int argc, char *argv[]) {
	struct dirent **list;
	int n, i;

	n = scandir(".", &list, filter, NULL);
	if (n != -1) {
		for (i=0; i<n; i++) {
			buildCrashdump(list[i]->d_name);
		}
	}

	return 0;
}

