//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <Noise.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>


namespace ossimplugins
{
static const char NOISE[] = "noise";
static const char NUMBER_OF_NOISE_RECORDS_KW[] = "numberOfNoiseRecords"; 

Noise::Noise():
  _numberOfNoiseRecords(0),
  _tabImageNoise()
{
}

Noise::~Noise()
{
}


Noise::Noise(const Noise& rhs):
  _numberOfNoiseRecords(rhs._numberOfNoiseRecords),
  _tabImageNoise(rhs._tabImageNoise)
{
}

Noise& Noise::operator=(const Noise& rhs)
{
  _numberOfNoiseRecords = rhs._numberOfNoiseRecords;
  _tabImageNoise = rhs._tabImageNoise;
  return *this;
}

bool Noise::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += NOISE;
   std::string s = pfx + "." + NUMBER_OF_NOISE_RECORDS_KW;
   kwl.add(prefix, s.c_str(), _numberOfNoiseRecords);
   for (unsigned int i = 0; i < _tabImageNoise.size(); ++i)
   {
      std::string s2 = pfx + "[" + ossimString::toString(i) + "]";
      _tabImageNoise[i].saveState(kwl, s2.c_str());
   }

   return true;
}

bool Noise::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "Noise::loadState";

   bool result = true;

   std::string pfx("");
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += NOISE;

   ossimString s;
   const char* lookup = 0;
   std::string s1 = pfx + ".";

  lookup = kwl.find(s1.c_str(), NUMBER_OF_NOISE_RECORDS_KW);
  if (lookup)
  {
    s = lookup;
    _numberOfNoiseRecords = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NUMBER_OF_NOISE_RECORDS_KW << "\n";
    result = false;
  }

   _tabImageNoise.clear();
   for (unsigned int i = 0; i < _numberOfNoiseRecords; ++i)
   {
      std::string s2 = pfx + "[" + ossimString::toString(i) + "]";
      ImageNoise in;
      result = in.loadState(kwl, s2.c_str());
      _tabImageNoise.push_back(in);
   }
   if( _numberOfNoiseRecords != _tabImageNoise.size() )
   {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword " << NUMBER_OF_NOISE_RECORDS_KW << " is different with the number of ImageNoise nodes \n";
   }
   return result;
}


std::ostream& Noise::print(std::ostream& out) const
{
   out << setprecision(15) << setiosflags(ios::fixed)
       << "\n Noise class data members:\n";

   const char* prefix = 0;
   ossimKeywordlist kwl;
   ossimString pfx;
   pfx += NOISE;
   std::string s = pfx + "." + NUMBER_OF_NOISE_RECORDS_KW;
   kwl.add(prefix, s.c_str(), _numberOfNoiseRecords);
   for (unsigned int i = 0; i < _tabImageNoise.size(); ++i)
   {
      std::string s2 = pfx + "[" + ossimString::toString(i) + "]";
      _tabImageNoise[i].saveState(kwl, s2.c_str());
   }

   out << kwl;

   return out;
}

}