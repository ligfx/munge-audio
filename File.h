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

#ifndef __LIBCREATURES_MUNGE_FILE_H__
#define __LIBCREATURES_MUNGE_FILE_H__

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace CreaturesMunge
{

class MungeFile;
class MungeSample;

class MungeFile
{
  public:
    /**
      @param stream
        Stream pointing to MungeFile data
        
      Does not read anything yet!
    */
	  MungeFile (std::istream*);
    virtual ~MungeFile();

    /**
      Reads the stream and fleshes out the struct
      
      @returns Success
    */
    virtual bool Load();
    
    /**
      @returns Munge script
    */
    virtual std::string GetScript();
    
    /**
      @returns Vector of pairs of sample names and sample data
    */
    // We could do a multimap, and have things keyed by name — problem is
    // we lose the order, but that can be regenerated from the script?
    // Unnamed samples will definitely lose order, but I guess it's not
    // our job to keep unnamed samples in line.
    virtual std::multimap<std::string, std::string*> GetSamples(); // <name, data*>
	
  protected:
    virtual bool ReadBytes (int length, std::string &out);
    virtual bool ReadInt (unsigned int &out);
  
    static std::string ScrambleScript (const std::string &script);
    static std::vector<std::string> ParseSampleNames (const std::string &script);
    static void AddWavHeader (std::string *data);
  
    static std::string leuint32_to_bytes (unsigned int integer);
    static unsigned int bytes_to_leuint32 (const std::string &bytes);
  
  protected:
    std::istream *stream;
   	std::string script;
   	std::multimap<std::string, std::string*> samples;
};

} // namespace CreaturesMunge

#endif
