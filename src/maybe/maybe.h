#ifndef __MAYBE_H__
#define __MAYBE_H__
//
// implementation of an optional value for c
//
//
// Usage:
// for a given type declare MAYBE_TYPE(<type>) and MAYBE_FAIL(<type>)
// e.g. maby int type
// MAYBE_TYPE(int)
// MAYBE_FAIL(int)
//
// ways to declare optional ints
// MAYBE(int) optionalInt; 
// INIT_NOTHING(int, optInt); // initializes optional int as Nothing
// INIT_SOME(int, optInt, 4711); // initializes optional int with value 4711 
//

#define MAYBE_TYPE(typeval) typedef struct{bool nothing;typeval something;}TMaybe##typeval
#define MAYBE_FAIL(typeval) typeval maybefail##typeval(){typeval x;exit(1); return x;}
#define MAYBE_FAIL_DO(typeval) maybefail##typeval()

#define MAYBE(typeval) TMaybe##typeval
#define INIT_NOTHING(typeval,varname) MAYBE(typeval) varname;{varname.nothing=true;}
#define INIT_SOME(typeval,varname,val) MAYBE(typeval) varname;{varname.nothing=false;varname.something=val;}

#define NOTHING(typeval,varname) {varname.nothing=true;}
#define SOME(typeval,varname,val) {varname.nothing=false;varname.something=val;}

#define IS_NOTHING(typeval,varname) (varname.nothing==true)
// VALUE extractor is dangerous, better unse monadic bind...
#define MAYBE_VALUE(typeval,varname) (varname.nothing==false?varname.something:MAYBE_FAIL_DO(typeval))

#define MAYBE_APPL(varname,fn) (varname.nothing?varname:INIT_SOME(fn(varname.something)))
#define MAYBE_MAP(varname,fn) (varname.nothing?varname:fn(varname.something))
#define MAYBE_JOIN(fn1,fn2,v) (v.nothing?v:MAYBE_MAP(fn1(v.something),fn2))

#endif

