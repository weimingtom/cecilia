//20180324
#pragma once

#ifdef macintosh
#define SEP ':'
#define MAXPATHLEN 256
#define DELIM '\n'
#endif

#if !defined(__QNX__)
#if defined(MS_WINDOWS) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__DJGPP__) || defined(PYOS_OS2)
#define SEP '\\'
#define ALTSEP '/'
#define MAXPATHLEN 256
#define DELIM ';'
#endif
#endif

#ifdef RISCOS
#define SEP '.'
#define MAXPATHLEN 256
#define DELIM ','
#endif


#ifndef SEP
#define SEP '/'
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

#ifndef DELIM
#define DELIM ':'
#endif
