################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CardDB/DbOperator.cpp \
../CardDB/IcCard_DbExtension.cpp \
../CardDB/IcCard_DbFundation.cpp \
../CardDB/IcCard_DbManagment.cpp 

OBJS += \
./CardDB/DbOperator.o \
./CardDB/IcCard_DbExtension.o \
./CardDB/IcCard_DbFundation.o \
./CardDB/IcCard_DbManagment.o 

CPP_DEPS += \
./CardDB/DbOperator.d \
./CardDB/IcCard_DbExtension.d \
./CardDB/IcCard_DbFundation.d \
./CardDB/IcCard_DbManagment.d 


# Each subdirectory must supply rules for building sources it contributes
CardDB/%.o: ../CardDB/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Ipthread -Idl -include"/home/cvecai/workspace/SqliteDataBaseManager/sqlite3/sqlite3.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/CardDB/DbDefine.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/CardDB/DbOperator.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/CardDB/IcCard_DbExtension.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/CardDB/IcCard_DbFundation.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/CardDB/IcCard_DbManagment.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/md5/md5.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/CAutoLock.h" -include"/home/cvecai/workspace/SqliteDataBaseManager/ThreadMutex.h" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


