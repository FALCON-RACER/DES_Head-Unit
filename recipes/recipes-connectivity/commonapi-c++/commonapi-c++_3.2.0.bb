SUMMARY = "CommonAPI"
SECTION = "libs"
LICENSE = "CLOSED"
# LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "boost"

SRC_URI = "git://github.com/COVESA/capicxx-core-runtime.git;protocol=https;branch=master"
SRCREV = "9eb5d398a0ea10c39b30ebf2789c6ae365c1895e"

S = "${WORKDIR}/git"

inherit cmake

EXTRA_OECMAKE += "-DINSTALL_LIB_DIR:PATH=${baselib} \
                  -DINSTALL_CMAKE_DIR:PATH=${baselib}/cmake/CommonAPI \
                 "

FILES:${PN} += "${libdir}/*.so*"
FILES:${PN}-dev += "${libdir}/cmake/CommonAPI"

do_install:append() {
    # Make sure the CMake config files are installed correctly
    install -d ${D}${libdir}/cmake/CommonAPI
    install -m 644 ${B}/CommonAPIConfig.cmake ${D}${libdir}/cmake/CommonAPI/
    install -m 644 ${B}/CommonAPIConfigVersion.cmake ${D}${libdir}/cmake/CommonAPI/
}

BBCLASSEXTEND = "native nativesdk"