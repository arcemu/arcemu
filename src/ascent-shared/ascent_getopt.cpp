/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#include "Common.h"
#include "ascent_getopt.h"

int arg_counter = 1;
char ascent_optarg[514];
int ascent_getopt_long_only(int ___argc, char *const *___argv, const char *__shortopts, const struct ascent_option *__longopts, int *__longind)
{
	// burlex todo: handle the shortops, at the moment it only works with longopts.

	if (___argc == 1 || arg_counter == ___argc)			// No arguments (apart from filename)
		return -1;

	const char * opt = ___argv[arg_counter];
//	int return_val = 0;

	// if we're not an option, return an error.
	if (strnicmp(opt, "--", 2) != 0)
		return 1;
	else
		opt += 2;


	// parse argument list
	int i = 0;
	for (; __longopts[i].name != 0; ++i)
	{
		if (!strnicmp(__longopts[i].name, opt, strlen(__longopts[i].name)))
		{
			// woot, found a valid argument =)
			char * par = 0;
			if ((arg_counter + 1) != ___argc)
			{
				// grab the parameter from the next argument (if its not another argument)
				if (strnicmp(___argv[arg_counter+1], "--", 2) != 0)
				{
					arg_counter++;		// Trash this next argument, we won't be needing it.
					par = ___argv[arg_counter];
				}
			}			

			// increment the argument for next time
			arg_counter++;

			// determine action based on type
			if (__longopts[i].has_arg == ascent_required_argument)
			{
				if(!par)
					return 1;

				// parameter missing and its a required parameter option
				if(__longopts[i].flag)
				{
					*__longopts[i].flag = atoi(par);
					return 0;
				}
			}

			// store argument in optarg
			if (par)
				strncpy(ascent_optarg, par, 514);

			if (__longopts[i].flag != 0)
			{
				// this is a variable, we have to set it if this argument is found.
				*__longopts[i].flag = 1;
				return 0;
			}
			else
			{
				if (__longopts[i].val == -1 || par == 0)
					return 1;

				return __longopts[i].val;
			}			
			break;
		}
	}

	// return 1 (invalid argument)
	return 1;
}

