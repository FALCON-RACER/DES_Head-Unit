import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: speedometer
    width: 600
    height: 600

    readonly property real speed: instrumentClusterController.speed
    property real animatedSpeed: 0
    readonly property string colorTheme: instrumentClusterController.ambientLighting
    property var gradient: null
    readonly property real gaugeStartAngle: 162
    readonly property int gaugeRadius: 200
    readonly property int gaugeStrokeWith: 30

    onSpeedChanged: {
        speedAnimation.to = speed;
        speedAnimation.start();
    }

    onColorThemeChanged: {
        updateGradient();
    }

    NumberAnimation {
        id: speedAnimation
        target: speedometer
        property: "animatedSpeed"
        duration: 100 // ms
        easing.type: Easing.InOutQuad

        onRunningChanged: {
            if (running) {
                updateTimer.start();
            } else {
                updateTimer.stop();
                foregroundCanvas.requestPaint();
            }
        }
    }

    Timer {
        id: updateTimer
        interval: 16  // 60fps
        repeat: true
        running: false
        onTriggered: foregroundCanvas.requestPaint()
    }

    Canvas {
        id: backgroundCanvas
        anchors.fill: parent

        onPaint: {
            let ctx = getContext("2d");
            ctx.reset();
            ctx.translate(width / 2, height / 2);
            ctx.clearRect(-width / 2, -height / 2, width, height);

            drawScale(ctx);
        }
    }

    Canvas {
        id: foregroundCanvas
        anchors.fill: parent

        onAvailableChanged: {
            if (available) {
                requestPaint();
            }
        }

        onPaint: {
            let ctx = getContext("2d");
            ctx.reset();
            ctx.translate(width / 2, height / 2);

            drawSpeedGauge(ctx);
        }
    }

    Text {
        id: speedText
        text: Math.round(animatedSpeed).toString()
        font.pixelSize: 70
        font.bold: true
        font.italic: true
        color: colorTheme
        anchors.centerIn: parent
    }

    Text {
        id: unitText
        text: "cm/s"
        font.pixelSize: 30
        font.italic: true
        font.bold: true
        color: colorTheme
        anchors.left: speedText.right
        anchors.bottom: speedText.bottom
        anchors.leftMargin: 3
        anchors.bottomMargin: 10
    }

    function updateGradient() {
        function hexToRgba(hex, alpha) {
            let r = parseInt(hex.slice(1, 3), 16);
            let g = parseInt(hex.slice(3, 5), 16);
            let b = parseInt(hex.slice(5, 7), 16);
            return `rgba(${r}, ${g}, ${b}, ${alpha})`;
        }

        let ctx = foregroundCanvas.getContext("2d");
        gradient = ctx.createRadialGradient(0, 0, gaugeRadius - gaugeStrokeWith, 0, 0, gaugeRadius + gaugeStrokeWith);
        gradient.addColorStop(0, hexToRgba(colorTheme, 0));
        gradient.addColorStop(1, hexToRgba(colorTheme, 1));
    }

    function drawSpeedGauge(ctx) {
        if (animatedSpeed > 0) {
            let gaugeEndAngle = gaugeStartAngle + animatedSpeed * 0.72;

            ctx.strokeStyle = gradient;
            ctx.lineWidth = gaugeStrokeWith * 2;

            ctx.beginPath();
            ctx.arc(0, 0, gaugeRadius, gaugeStartAngle * Math.PI / 180, gaugeEndAngle * Math.PI / 180);
            ctx.stroke();
        }
    }

    function drawScale(ctx) {
        ctx.strokeStyle = "white";
        for (let i = 0; i <= 240; i += 4) {
            ctx.save();
            ctx.rotate((280 + i) * Math.PI / 200);
            if (i % 40 === 0) {
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.moveTo(0, -245);
                ctx.lineTo(0, -280);
                ctx.stroke();
                ctx.translate(0, -107);
            } else if (i % 20 === 0) {
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(0, -255);
                ctx.lineTo(0, -280);
                ctx.stroke();
            } else {
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(0, -265);
                ctx.lineTo(0, -280);
                ctx.stroke();
            }
            ctx.restore();
        }
    }
}
