APPL_COBJS += 

APPL_CXXOBJS +=

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# C++ Version API
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

# C++ Version API Extra
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3ext/Makefile

endif
