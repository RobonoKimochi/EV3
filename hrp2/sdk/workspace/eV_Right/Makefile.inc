mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += balancer.o balancer_param.o

APPL_CXXOBJS += \
	BalancingWalker.o \
	LineMonitor.o \
	BalancerCpp.o \
    Starter.o \
    PidController.o \
    Calibration.o \
	LineTracer.o \
    LineTracerWithStarter.o \
	RotaryEnc.o \
	Odmetry.o \
	Mat.o \
	BlueTooth.o \
	UI.o \
	Logger.o \
	Filter.o \
	Rotator.o \
	Remort.o \
	MeasureDistance.o \
	LookUpGate.o \
	AttitudeControl.o \
	TailMotor.o \
	RunManager.o \
    Garage.o \
	Sound.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)app $(mkfile_path)unit $(mkfile_path)device

INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)unit \
	-I$(mkfile_path)device \
