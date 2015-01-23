################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../parser/head_lexer.cpp \
../parser/lexer.cpp \
../parser/parser_exception.cpp \
../parser/query_parser.cpp \
../parser/rules.cpp 

OBJS += \
./parser/head_lexer.o \
./parser/lexer.o \
./parser/parser_exception.o \
./parser/query_parser.o \
./parser/rules.o 

CPP_DEPS += \
./parser/head_lexer.d \
./parser/lexer.d \
./parser/parser_exception.d \
./parser/query_parser.d \
./parser/rules.d 


# Each subdirectory must supply rules for building sources it contributes
parser/%.o: ../parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


