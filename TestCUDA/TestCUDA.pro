QT += core gui widgets

CONFIG += c++11

TARGET = TestCUDA
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    gl_window.cpp \
    mainwindow.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0




OTHER_FILES += vectorAddition.cu  matrixMultiply.cu


QMAKE_CFLAGS_RELEASE += /MTd
QMAKE_CXXFLAGS_RELEASE += /MTd
QMAKE_CFLAGS_RELEASE -= -MDd
QMAKE_CXXFLAGS_RELEASE -= -MDd

QMAKE_CFLAGS_DEBUG += /MTd
QMAKE_CXXFLAGS_DEBUG += /MTd
QMAKE_CFLAGS_DEBUG -= -MDd
QMAKE_CXXFLAGS_DEBUG -= -MDd

# CUDA settings <-- may change depending on your system
CUDA_SOURCES += vectorAddition.cu   matrixMultiply.cu
#CUDA_SOURCES += v.cu

#CUDA_SDK = "C:/ProgramData/NVIDIA Corporation/NVIDIA GPU Computing SDK 4.2/C"   # Path to cuda SDK install
CUDA_DIR = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0"           # Path to cuda toolkit install
SYSTEM_NAME = x64         # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64            # '32' or '64', depending on your system
CUDA_ARCH = sm_20           # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
NVCC_OPTIONS = --use_fast_math

# include paths
INCLUDEPATH += "$$CUDA_DIR/include"

               #$$CUDA_SDK/common/inc/ \
               #$$CUDA_SDK/../shared/inc/

# library directories
QMAKE_LIBDIR += $$CUDA_DIR/lib/$$SYSTEM_NAME
                #$$CUDA_SDK/common/lib/$$SYSTEM_NAME \
                #$$CUDA_SDK/../shared/lib/$$SYSTEM_NAME


# The following library conflicts with something in Cuda
QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:msvcrt.lib   /NODEFAULTLIB:libcmt.lib
QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:msvcrtd.lib  /NODEFAULTLIB:libcmt.lib
#QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:libcmtd
#QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:

# Add the necessary libraries
CUDA_LIBS = cuda.lib cudart.lib
# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')
#LIBS += $$join(CUDA_LIBS,'.lib ', '', '.lib')
LIBS += $$CUDA_LIBS
#LIBS += $$CUDA_INC #this is stupid since libs shoud not have any include

HIII = $$quote("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/bin/nvcc.exe")
# Configuration of the Cuda compiler
CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc.exe -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC "$$NVCC_LIBS" --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$HIII $$NVCC_OPTIONS $$CUDA_INC "$$NVCC_LIBS" --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}































DISTFILES += \
    vectorAddition.cu \
    matrixMultiply.cu

HEADERS += \
    gl_window.h \
    mainwindow.h \
    visualizerwidget.h \
    vectoraddition.h
