/* libcreatures-munge - manipulate munge files
 * Copyright (C) 2008-2009 Michael Maltese <michael.joseph.maltese@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "AST.h"
#include <boost/foreach.hpp>
#include <cassert>
#include <cstdlib> // abort
#include "File.h"
#include <iostream>
#include "MNGLexer.h"
#include "MNGParser.h"
#include "SampleScanner.h"

#define foreach BOOST_FOREACH

using namespace std;
using namespace CreaturesMunge;

MungeFile::MungeFile (istream *_stream)
{
  assert (_stream->good ());
  stream = _stream;
}

MungeFile::~MungeFile ()
{
  typedef pair<string, string*> Sample;
  foreach (Sample s, samples)
  {
    delete s.second; 
  }
  samples.clear();

  delete stream;
}

multimap<string, string*> MungeFile::GetSamples() { return samples; }
string MungeFile::GetScript() { return script; }

bool MungeFile::Load ()
{
  if (!stream) return true;

  // n means number
  // p means position
  // l means length

  unsigned int nSamples;
  unsigned int pScript;
  unsigned int lScript;


  // Read Header
  if (!ReadInt (nSamples)) return false;
  if (!ReadInt (pScript)) return false;
  if (!ReadInt (lScript)) return false;

  // Read Sample metadata
  vector<int> pSamples, lSamples;
  pSamples.reserve (nSamples);
  lSamples.reserve (nSamples);
  for (unsigned int i = 0; i < nSamples; i++)
  {
    unsigned int p, l;
    if (!ReadInt (p)) return false;
    if (!ReadInt (l)) return false; 
    pSamples.push_back (p);
    lSamples.push_back (l);
  }
  // TODO: pSamples and lSamples should be list - push on end, pop off front
  
  // Read in the samples
  list<string*> sample_data;
  for (unsigned int i = 0; i < nSamples; i++)
  {
    stream->seekg (pSamples.at(i));
    string *data = new string(); // TODO: Delete this eventually
    if (!ReadBytes (lSamples.at(i), *data)) return false;
    AddWavHeader (data);
    sample_data.push_back (data);
  }
  
  // Read in the script
  stream->seekg (pScript);
  if (!ReadBytes (lScript, script)) return false;
  script = ScrambleScript (script);

  // TODO: Parse sample names
  MNGLexer lexer (script.c_str());
  MNGParser parser(&lexer, "<stdio>"); // TODO: filename
  list<FunctionNode> tree;
  if (parser.Parse (&tree))
  {
    SampleScanner scanner;
    scanner.visit (tree);
    list<string> names = scanner.getNames();
    foreach (string name, names)
    {
      samples.insert (make_pair (name, sample_data.front()));
      sample_data.pop_front();
    }
  }
  else;// TODO

  stream = NULL;
  return true;
}

#if 0
string MungeFile::data ()
{
	vector<int> sample_positions;
	int sample_count, script_position, position, i;
	
	sample_count = samples.size();
	
	// Calculate positions
	script_position = 12 + (8 * sample_count);
	position = script_position + script.size();
	
	sample_positions.reserve(sample_count);
	for (i = 0; i < sample_count; i++) {
		sample_positions.push_back(position);
		position += samples[i].size();
	}
	
	// Collect the data
	string data;
	data.reserve(dataSize());

	data += munge_little_uint32_to_bytes(sample_count);
	data += munge_little_uint32_to_bytes(script_position);
	data += munge_little_uint32_to_bytes(script.size());

	for (i = 0; i < sample_count; i++) {
		data += munge_little_uint32_to_bytes(sample_positions[i]);
		data += munge_little_uint32_to_bytes(samples[i].size());
	}
	
	data += scrambleScript(script);;

	for (i = 0; i < sample_count; i++)
		data.append(samples[i]);
	
	return data;
}

int MungeFile::dataSize ()
{
	int sample_count, size;

	sample_count = samples.size();
	
	size = 12 + (sample_count * 8) + script.size();
	while (sample_count--) size += samples[sample_count].size();
	
	return size;
}

#endif

void MungeFile::AddWavHeader (string *sample)
{
	string header;

	header.reserve(16);
	header += "RIFF";
	// size + 8 b/c of the added 'WAVEfmt '
	header += leuint32_to_bytes(sample->size() + 8);
	header += "WAVEfmt ";

	sample->insert(0, header);
}

string MungeFile::ScrambleScript (const string &old)
{
	string script;
	script.reserve(old.size());
	
	unsigned char pad = 5;
	for (unsigned int i = 0; i < old.size(); i++) {
		script += (old.at(i) ^ pad);
		pad += 0xC1;
	}
	return script;
}

vector<string> MungeFile::ParseSampleNames (const string &script)
{
  cerr << "WARNING: Unimplemented MungeFile::parseSampleNames called!" << endl;
  return vector<string>();
}

bool MungeFile::ReadInt (unsigned int &out) {
  string bytes;
  if (!ReadBytes (4, bytes)) return false;
  
  out = bytes_to_leuint32 (bytes);
  return true;
}

bool MungeFile::ReadBytes (int length, string &out) {
	assert (stream != 0);
	assert (length > 0);
	string bytes;
	
	bytes.reserve(length);
	while (length--) {
		bytes += stream->get();
		if (stream->fail ())
		  return false;
	}
	
	out = bytes;
	return true;
}

string MungeFile::leuint32_to_bytes (unsigned int num) {
	string bytes;

	bytes += ((num%(256*256*256))%(256*256))%256;
	bytes += ((num%(256*256*256))%(256*256))/256;
	bytes += (num%(256*256*256))/(256*256);
	bytes += num/(256*256*256);

	return bytes;
}
unsigned int MungeFile::bytes_to_leuint32 (string const &bytes) {
	unsigned int num = 0;

	num += (unsigned char)(bytes.at(0));
	num += (unsigned char)(bytes.at(1))*256;
	num += (unsigned char)(bytes.at(2))*256*256;
	num += (unsigned char)(bytes.at(3))*256*256*256;
	
	return num;
}
