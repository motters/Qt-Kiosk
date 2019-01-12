import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebEngine 1.4
import QtQuick.Controls 2.3

//Item {
//    id: webEngineContainer

//    width: parent.width
//    height: parent.height

    WebEngineView {
        id: webEngine

        //anchors.fill: parent
         width: parent.width
         height: parent.height

        //url: "http://intranet.chemistry.manchester.ac.uk/electronic_booking/walkup"
        url: "http://google.com"

        settings.pluginsEnabled: true
        settings.fullScreenSupportEnabled: true
        settings.autoLoadImages: true
        settings.javascriptEnabled: true
        settings.errorPageEnabled: true

//        onContextMenuRequested: function(request)
//        {
//            request.accepted = true;

//        }

        onCertificateError: function(request)
        {
            // http://doc.qt.io/qt-5/qml-qtwebengine-webenginecertificateerror.html
        }

        onLoadingChanged: function(request)
        {
            // http://doc.qt.io/qt-5/qml-qtwebengine-webengineloadrequest.html
            console.log(request.url);
            if(request.status === WebEngineLoadRequest.LoadFailedStatus)
            {
                console.log(request.errorDomain);
                console.log(request.errorString);
            }
        }

        onWindowCloseRequested:
        {
            // http://doc.qt.io/qt-5/qml-qtwebengine-webengineview.html
        }

        onRenderProcessTerminated: function(terminationStatus, exitCode)
        {
            // http://doc.qt.io/qt-5/qwebenginepage.html#RenderProcessTerminationStatus-enum
            var status = "";

            switch (terminationStatus)
            {
                case WebEngineView.NormalTerminationStatus:
                    status = "(normal exit)";
                    break;
                case WebEngineView.AbnormalTerminationStatus:
                    status = "(abnormal exit)";
                    break;
                case WebEngineView.CrashedTerminationStatus:
                    status = "(crashed)";
                    break;
                case WebEngineView.KilledTerminationStatus:
                    status = "(killed)";
                    break;
            }

            print("Render process exited with code " + exitCode + " " + status);

            //reloadTimer.running = true;
        }

        Timer {
            id: reloadTimer
            interval: 0
            running: false
            repeat: false
            onTriggered: webEngine.reload()
        }
    }
//}
