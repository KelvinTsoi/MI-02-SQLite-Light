#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=X86DebugApp
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/jni/CardDB/DbOperator.o \
	${OBJECTDIR}/jni/CardDB/IcCard_DbExtension.o \
	${OBJECTDIR}/jni/CardDB/IcCard_DbFundation.o \
	${OBJECTDIR}/jni/CardDB/IcCard_DbManagment.o \
	${OBJECTDIR}/jni/UtilClass.o \
	${OBJECTDIR}/jni/main.o \
	${OBJECTDIR}/jni/md5/md5.o \
	${OBJECTDIR}/jni/sqlite3/sqlite3.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread -ldl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dbmanager

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dbmanager: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dbmanager ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/jni/CardDB/DbOperator.o: jni/CardDB/DbOperator.cpp 
	${MKDIR} -p ${OBJECTDIR}/jni/CardDB
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/CardDB/DbOperator.o jni/CardDB/DbOperator.cpp

${OBJECTDIR}/jni/CardDB/IcCard_DbExtension.o: jni/CardDB/IcCard_DbExtension.cpp 
	${MKDIR} -p ${OBJECTDIR}/jni/CardDB
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/CardDB/IcCard_DbExtension.o jni/CardDB/IcCard_DbExtension.cpp

${OBJECTDIR}/jni/CardDB/IcCard_DbFundation.o: jni/CardDB/IcCard_DbFundation.cpp 
	${MKDIR} -p ${OBJECTDIR}/jni/CardDB
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/CardDB/IcCard_DbFundation.o jni/CardDB/IcCard_DbFundation.cpp

${OBJECTDIR}/jni/CardDB/IcCard_DbManagment.o: jni/CardDB/IcCard_DbManagment.cpp 
	${MKDIR} -p ${OBJECTDIR}/jni/CardDB
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/CardDB/IcCard_DbManagment.o jni/CardDB/IcCard_DbManagment.cpp

${OBJECTDIR}/jni/UtilClass.o: jni/UtilClass.cpp 
	${MKDIR} -p ${OBJECTDIR}/jni
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/UtilClass.o jni/UtilClass.cpp

${OBJECTDIR}/jni/main.o: jni/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/jni
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DDEBUG -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DRECORD_G711_AUDIO -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/main.o jni/main.cpp

${OBJECTDIR}/jni/md5/md5.o: jni/md5/md5.c 
	${MKDIR} -p ${OBJECTDIR}/jni/md5
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/md5/md5.o jni/md5/md5.c

${OBJECTDIR}/jni/sqlite3/sqlite3.o: jni/sqlite3/sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}/jni/sqlite3
	${RM} "$@.d"
	$(COMPILE.c) -g -DCURLDEBUG -DCURL_DISABLE_LDAP -DHAVE_FCNTL_H -DHAVE_PTHREAD -DHAVE_SEMAPHORE_H -DHAVE_SOCKET -DHAVE_STRUCT_TIMEVAL -DHAVE_SYS_TIME_H -DUSE_OPENSSL -DUSE_SSLEAY -DX86DEBUG -D__SETUP_ONCE_H -Ijni -Ijni/md5 -Ijni/sqlite3 -Ijni/CardDB -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/jni/sqlite3/sqlite3.o jni/sqlite3/sqlite3.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dbmanager

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
