//
//  AppMacros.h
//  DotA
//
//  Created by DucHQ on 5/27/14.
//
//

#ifndef DotA_AppMacros_h
#define DotA_AppMacros_h


#define CREATE_SET_GET_FUNC(name , type , variable) \
protected: type variable; \
public: \
void set##name( type pVar ) { variable = pVar; } \
type get##name( ) { return variable; }


#define CREATE_SET_GET_FUNC_ProtectedInt(name , variable) \
protected: ProtectedInt variable; \
public: \
void set##name( int pVar ) { variable = pVar; } \
int get##name( ) { return variable.getValue(); }

#define CREATE_SET_GET_FUNC_ProtectedLong(name , variable) \
protected: ProtectedLong variable; \
public: \
void set##name( long pVar ) { variable = pVar; } \
long get##name( ) { return variable.getValue(); }


#define CREATE_SET_GET_FUNC_ProtectedFloat(name , variable) \
protected: ProtectedFloat variable; \
public: \
void set##name( float pVar ) { variable = pVar; } \
float get##name( ) { return variable.getValue(); }

#define CREATE_SET_GET_FUNC_ProtectedString(name , variable) \
protected: ProtectedString variable; \
public: \
void set##name( const std::string& pVar ) { variable.setValue(pVar); } \
std::string get##name( ) { return variable.getString(); }

#define FLAG_DRAW uint32_t

#define OFFSET_Y_LABEL -6

#endif
