#include "config.h"
#include "widgets/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(PS_APP_TITLE);
    a.setApplicationVersion(PS_VERSION_STRING);
    a.setOrganizationName("PearCoding");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setSamples(4);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);

#ifdef PS_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif

    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

	int res = a.exec();

#if defined(PS_DEBUG) && defined(Q_OS_WIN)
	system("PAUSE");
#endif

    return res;
}
