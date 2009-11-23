/* creatures-munge - manipulate munge files
 * Copyright (C) 2008-2009 Michael Maltese <michael.joseph.maltese@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib> // 'exit'
#include "File.h"
#include <getopt.h>
#include <iostream>
#include <libgen.h> // basename
#include <string>
#include <string.h>
#include <sys/stat.h> // mkdir

#define foreach BOOST_FOREACH

using namespace boost;
using namespace CreaturesMunge;
using namespace std;
namespace fs = boost::filesystem;

/* Prototypes */
void print_usage ();
void print_version ();
char* parse_args (int, char**);
int main (int, char**);

/* Usage information: */
static const char *usage = "Usage: munge-extract [options] INPUT.mng\n"
"Creates directory INPUT containing script from munge file and samples in\n"
"wave format.\n"
"\n"
"    -s, --silent		Suppress informal output.\n"
"    -v, --version		Print version and exit.\n"
"    -h, --help			Print this help screen and exit.\n"
"\n"
"Please report Bugs to: Michael Maltese <michael.joseph.maltese@gmail.com>\n";

void print_usage(void)
{
	std::cerr << usage << std::endl;
	exit(1);
}

/* Version and copyright information: */
static const char *version = ("munge-extract version 0.0.0\n"
"\n"
"Copyright (C) 2008 Michael Maltese <michael.joseph.maltese@gmail.com>\n"
"\n"
"This program is free software; you can redistribute it and/or\n"
"modify it under the terms of the GNU General Public License\n"
"as published by the Free Software Foundation; either version 2\n"
"of the License, or (at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n"
"\n"
);

void print_version(void)
{
	cout << version << endl;
	exit(1);
}

bool SILENT = false;

/* Command line options */
static struct option cl_options[] = {
	{ "silent",		no_argument,	NULL,	's'	},
	{ "version",	no_argument,	NULL,	'v'	},
	{ "help",			no_argument,	NULL,	'h'	},
	{ 0, 0, 0, 0 }
};

/* Parse command-line args and
 * return name of input file */
char *parse_args(int argc, char **argv)
{
    int opt_idx = 0;
    signed char c;
    char *file;

    while ((c = getopt_long(argc, argv, ":svh", cl_options, &opt_idx)) != -1) {
			switch (c) {
				case 's':
				SILENT = true;
				break;

				case 'v':
				print_version();
				break;

				case 'h':
				default:
				std::cout << c << std::endl;
				print_usage();
			}
    }

    /* Now we need the input file */
    if (!(optind < argc)) print_usage();
    file = argv[optind];

    return file;
}

int main (int argc, char **argv)
{
	fs::path filename = parse_args(argc, argv);
	MungeFile file (new fs::ifstream(filename, ios::in | ios::binary));
	
	if (!file.Load())
	  { cerr << "Error reading file '" << filename << "'; aborting." << endl; exit (1); }
	
	multimap<string, string*> samples = file.GetSamples();
	if (!SILENT) cout << "Sample count: " << samples.size() << endl;
	
	fs::path directory = fs::path(filename.filename()).replace_extension("");
	if (directory == filename)
	  { cerr << "Error figuring output directory name; aborting" << endl; exit(1); }
	
	if (!SILENT) cout << "Creating directory " << directory << "..." << endl;
	if (fs::exists(directory))
  	{ cerr << "Directory exists; aborting." << endl; exit(1); }
	if (!fs::create_directory (directory))
	  { cerr << "Error creating directory; aborting" << endl; exit(1);}
	  
  if (!SILENT) cout << "Extracting Script..." << endl;
  fs::ofstream scriptstream(directory / "script", ios::out);
  scriptstream << file.GetScript();
  scriptstream.close ();
		
	int unnamed = 0;
	typedef pair<string, string*> Sample;
	foreach (Sample sample, samples)
	{
	  string name = sample.first;
	  if (name.length() == 0)
	  {
	    name = "Unnamed Sample " + lexical_cast<string>(unnamed);
	    unnamed += 1;
	  }
		if (!SILENT) cout << "Extracting " << name << "... " << endl;
		fs::ofstream samplestream((directory / name).string() + ".wav", ios::out | ios::binary);
		samplestream << *sample.second;
	}
  
  if (!SILENT) cout << "Done!" << endl;

	return 0;
}
