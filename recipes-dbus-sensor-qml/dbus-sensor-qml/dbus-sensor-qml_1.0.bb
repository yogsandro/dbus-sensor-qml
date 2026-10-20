SUMMARY = "QML temperature sensor client application"
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "git://github.com/yogsandro/dbus-sensor-qml.git;branch=main;protocol=https" 
SRC_URI += "\
    file://qml-temperature.service \
    file://qml-temperature.conf \
"

SRCREV = "993a43c29c89c5e960eae87a0841ff10de96804c"


inherit systemd pkgconfig qt6-cmake

DEPENDS += "glib-2.0 qtbase qtdeclarative qtdeclarative-native qtquick3d qtquick3d-native qttools qtquicktimeline qtgraphs"


S = "${UNPACKDIR}/${P}"

SYSTEMD_SERVICE:${PN} = "qml-temperature.service"
SYSTEMD_AUTO_ENABLE = "enable"

   
    
do_install() {    
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${UNPACKDIR}/qml-temperature.service \
        ${D}${systemd_system_unitdir}/qml-temperature.service
    install -d ${D}${datadir}/dbus-1/system.d
    install -m 0644 ${UNPACKDIR}/qml-temperature.conf \
        ${D}${datadir}/dbus-1/system.d/    
    install -d ${D}${bindir}
    install -m 0755 ${B}/QMLTemperature ${D}${bindir}/qml-temperature-sensor-app

}

FILES:${PN} += " \
    ${systemd_system_unitdir}/qml-temperature.service \
    ${datadir}/dbus-1/system.d/qml-temperature.conf \
"

