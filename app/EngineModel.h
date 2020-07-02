#ifndef _APP_ENGINE_MODEL_H_
#define _APP_ENGINE_MODEL_H_

#include "include.h"
#include "PadModel.h"
#include "Clock.h"

class EngineModel : public QObject
{
  Q_OBJECT

  Q_PROPERTY( int bpm READ bpm WRITE setBpm NOTIFY bpmChanged )

  Clock m_clock{};
  PadModel* m_pad_model = nullptr;

  bool m_is_running = false;

  public:
  EngineModel( PadModel* padModel )
    : m_pad_model( padModel )
  {
    registerUpdate( this );
    registerUpdate( &m_clock );

    QObject::connect( m_pad_model, &PadModel::padSizeChanged,
                      &m_clock, &Clock::setNumBeats );
  }

  template <typename T>
  void registerUpdate( T const* obj )
  {
    m_clock.registerUpdate( obj );
  }

  void update()
  {
    qDebug() << "\n*** EngineModel::update" << m_clock.m_bpm; 
    m_pad_model->update( m_clock.beat() );
  }

  int bpm() const
  {
    return m_clock.m_bpm;
  }

  void setBpm( int bpm ) 
  {
    m_clock.setBpm( bpm );

    emit bpmChanged();
  }

  public slots:

  bool isRunning() const
  {
    return m_is_running;
  }

  bool play( bool fromBeginning = true )
  {
    m_clock.start( fromBeginning );
    m_is_running = true;

    return true;
  }

  bool stop()
  {
    m_clock.stop();
    m_is_running = false;

    return false;
  }

  int beatDuration()
  {
    return m_clock.getInterval();
  }

  signals:

  void bpmChanged();
};


#endif
