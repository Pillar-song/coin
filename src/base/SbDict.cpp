/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#include <Inventor/SbDict.h>
#include <Inventor/lists/SbPList.h>
#include <assert.h>

/*!
  \class SbDict SbDict.h Inventor/SbDict.h
  \brief The SbDict class organizes a dictionary of keys and values.

  It uses hashing to quickly insert and find entries in the dictionary.
  An entry consists of an unique key and a generic pointer.
*/

//
// internal class
//
class SbDictEntry
{
private:
  SbDictEntry(const unsigned long key, void * const value) {
    this->key = key;
    this->value = value;
  }
private:
  unsigned long key;
  void * value;
  SbDictEntry * next;

  friend class SbDict;
};

//
// default hashing function will just return the key
//
static unsigned long
default_hashfunc(const unsigned long key)
{
  return key;
}

// *************************************************************************

/*!
  Constructor with \a entries specifying the number of buckets
  in the hash list -- so it need to be larger than 0. For best
  performance during dictionary look-ups, \a entries should be a prime.
*/
SbDict::SbDict(const int entries)
{
  assert(entries > 0);
  this->tablesize = entries;
  this->buckets = new SbDictEntry *[this->tablesize];
  this->hashfunc = default_hashfunc;
  for (int i = 0; i < this->tablesize; i++) this->buckets[i] = NULL;
}

/*!
  Copy constructor.
*/
SbDict::SbDict(const SbDict & from)
{
  this->operator=(from);
}

/*!
  Destructor.
*/
SbDict::~SbDict()
{
  this->clear();
  delete [] buckets;
}

/*!
  Make a deep copy of the contents of dictionary \a from into this dictionary.
*/
SbDict &
SbDict::operator=(const SbDict & from)
{
  this->tablesize = from.tablesize;
  this->hashfunc = from.hashfunc;
  this->buckets = new SbDictEntry *[this->tablesize];
  for (int i = 0; i < this->tablesize; i++) this->buckets[i] = NULL;
  from.applyToAll(copyval, this);
  return *this;
}

/*!
  \internal
  Callback for copying values from one SbDict to another.
*/
void
SbDict::copyval(unsigned long key, void * value, void * data)
{
  SbDict * thisp = (SbDict *)data;
  thisp->enter(key, value);
}

/*!
  Clear all entries in the dictionary.
*/
void
SbDict::clear(void)
{
  int i;
  SbDictEntry * entry, * nextEntry;

  for (i = 0; i < this->tablesize; i++) {
    for (entry = buckets[i]; entry != NULL; entry = nextEntry) {
      nextEntry = entry->next;
      delete entry;
    }
    buckets[i] = NULL;
  }
}

/*!
  Inserts a new entry into the dictionary. \a key should be
  a unique number, and \a value is the generic user data.

  \e If \a key does not exist in the dictionary, a new entry
  is created and \c TRUE is returned. Otherwise, the generic user
  data is changed to \a value, and \c FALSE is returned.
*/
SbBool
SbDict::enter(const unsigned long key, void * const value)
{
  const unsigned long bucketnum = this->hashfunc(key) % this->tablesize;
  SbDictEntry *entry = findEntry(key, bucketnum);
  if (entry == NULL) {
    entry = new SbDictEntry(key, value);
    entry->next = this->buckets[bucketnum];
    this->buckets[bucketnum] = entry;
    return TRUE;
  }
  else {
    entry->value = value;
    return FALSE;
  }
}

/*!
  Searches for \a key in the dictionary. If an entry with this
  key exists, \c TRUE is returned and the entry value is returned
  in \a value. Otherwise, \c FALSE is returned.
*/
SbBool
SbDict::find(const unsigned long key, void *& value) const
{
  const unsigned long bucketnum = this->hashfunc(key) % this->tablesize;
  SbDictEntry *entry = findEntry(key, bucketnum);
  if (entry == NULL) {
    value = NULL;
    return FALSE;
  }
  else {
    value = entry->value;
    return TRUE;
  }
}

/*!
  Removes the entry with key \a key. \c TRUE is returned if an entry
  with this key was present, \c FALSE otherwise.
*/
SbBool
SbDict::remove(const unsigned long key)
{
  const unsigned long bucketnum = this->hashfunc(key) % this->tablesize;
  SbDictEntry *prev = NULL;
  SbDictEntry *entry = findEntry(key, bucketnum, &prev);
  if (entry == NULL)
    return FALSE;
  else {
    if (prev) {
      prev->next = entry->next;
    }
    else {
      this->buckets[bucketnum] = entry->next;
    }
    delete entry;
    return TRUE;
  }
}

/*!
  Applies \a rtn to all entries in the dictionary.
*/
void
SbDict::applyToAll(void (* rtn)(unsigned long key, void * value)) const
{
  SbDictEntry * entry;
  int n = this->tablesize;
  for (int i = 0; i < n; i++) {
    entry = this->buckets[i];
    while (entry) {
      rtn(entry->key, entry->value);
      entry = entry->next;
    }
  }
}

/*!
  \overload
*/
void
SbDict::applyToAll(void (* rtn)(unsigned long key, void * value, void * data),
                   void * data) const
{
  SbDictEntry * entry;
  int n = this->tablesize;
  for (int i = 0; i < n; i++) {
    entry = this->buckets[i];
    while (entry) {
      rtn(entry->key, entry->value, data);
      entry = entry->next;
    }
  }
}

/*!
  Creates lists with all entries in the dictionary.
*/
void
SbDict::makePList(SbPList & keys, SbPList & values)
{
  SbDictEntry * entry;
  int n = this->tablesize;
  for (int i = 0; i < n; i++) {
    entry = this->buckets[i];
    while (entry) {
      keys.append((void *)entry->key);
      values.append((void *)entry->value);
      entry = entry->next;
    }
  }
}

SbDictEntry *
SbDict::findEntry(const unsigned long key,
                  const unsigned long bucketnum,
                  SbDictEntry **prev) const
{
  if (prev) *prev = NULL;
  SbDictEntry *entry = buckets[bucketnum];
  while (entry) {
    if (entry->key == key) break;
    if (prev) *prev = entry;
    entry = entry->next;
  }
  return entry;
}

/*!
  Sets a new hashing function for this dictionary. Default
  hashing function just returns the key.

  If you find that items entered into the dictionary seems to make
  clusters in only a few buckets, you should try setting a hashing
  function. If you're for instance using strings, you could use the
  static SbString::hash() function (you'd need to make a static function
  that will cast from unsigned long to char * of course).

  This function is not part of the OIV API.
*/
void
SbDict::setHashingFunction(unsigned long (*func)(const unsigned long key))
{
  this->hashfunc = func;
}


/*
  For debugging
  */
// void
// SbDict::dump(void)
// {
//   int i;
//   SbDictEntry * entry, * nextEntry;

//   for (i = 0; i < this->tablesize; i++) {
//     for (entry = buckets[i]; entry != NULL; entry = nextEntry) {
//       nextEntry = entry->next;
//       printf("entry: '%s' %p\n", entry->key, entry->value);
//     }
//   }
// }

// void SbDict::print_info()
// {
//   int i, cnt;
//   SbDictEntry * entry;

//   printf("---------- dict info ------------------\n");

//   for (i = 0; i < this->tablesize; i++) {
//     entry = buckets[i];
//     cnt = 0;
//     while (entry) {
//       entry = entry->next;
//       cnt++;
//     }
//     printf(" bucket: %d, cnt: %d\n", i, cnt);
//   }
//   printf("\n\n\n");
// }
