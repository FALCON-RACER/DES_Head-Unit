SUMMARY = "Vehicle Service Communication Pattern"
SECTION = "libs"
LICENSE = "CLOSED"
# LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "boost dlt-daemon"

SRC_URI = "git://github.com/COVESA/vsomeip.git;protocol=https;branch=master"
SRCREV = "e95d6031ac106eb7ff9317b194b73d6aaf501910"

S = "${WORKDIR}/git"

inherit cmake

EXTRA_OECMAKE += "-DENABLE_SIGNAL_HANDLING=1 -DDIAGNOSIS_ADDRESS=0x12 -DDEFAULT_CONFIGURATION_FOLDER=${sysconfdir}/vsomeip"

# 기본 파일들
FILES:${PN} += "${libdir}/*.so*"
FILES:${PN}-dev += "${libdir}/cmake"

# 설정 파일들 추가
FILES:${PN} += " \
    ${sysconfdir}/vsomeip \
    ${sysconfdir}/vsomeip/vsomeip*.json \
"

# 설치 경로 수정을 위한 do_install 오버라이드
do_install:append() {
    # /usr/etc를 /etc로 이동
    if [ -d ${D}/usr/etc ]; then
        mv ${D}/usr/etc ${D}/
    fi
}

RDEPENDS:${PN} += "dlt-daemon"

BBCLASSEXTEND = "native nativesdk"