SUMMARY = "CommonAPI SOME/IP"
SECTION = "libs"
LICENSE = "CLOSED"
# LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "boost commonapi-c++ vsomeip"

SRC_URI = "git://github.com/COVESA/capicxx-someip-runtime.git;protocol=https;branch=master"
SRCREV = "86dfd69802e673d00aed0062f41eddea4670b571"

S = "${WORKDIR}/git"

inherit cmake

EXTRA_OECMAKE += "-DUSE_INSTALLED_COMMONAPI=ON \
                  -DCommonAPI_DIR=${STAGING_LIBDIR}/cmake/CommonAPI \
                 "

FILES:${PN} += "${libdir}/*.so*"
FILES:${PN}-dev += "${libdir}/cmake"

RDEPENDS:${PN} += "commonapi-c++ vsomeip"

BBCLASSEXTEND = "native nativesdk"