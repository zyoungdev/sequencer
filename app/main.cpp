#include "main.h"

int main( int argc, char *argv[] )
{
  PadModel padModel;
  Piano piano;
  EngineModel engineModel{ &padModel, &piano };

  QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );

  QApplication app( argc, argv );

  QQmlApplicationEngine engine;

  // qmlRegisterType<Wav>("com.app", 1, 0, "Wav");

  engine.rootContext()->setContextProperty( "padModel", QVariant::fromValue( &padModel ) );
  engine.rootContext()->setContextProperty( "engineModel", QVariant::fromValue( &engineModel ) );

  engine.load( QUrl( "qrc:ui/main.ui.qml" ) );
  if ( engine.rootObjects().isEmpty() )
      return -1;

  return app.exec();
}
