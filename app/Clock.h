#ifndef _APP_CLOCK_H_
#define _APP_CLOCK_H_

#include "include.h"

class Clock : public QObject
{
  Q_OBJECT

  QTimer* m_timer = nullptr;
  bool m_is_running = false;

  /** The current beat */
  int m_beat = 0;

  int m_num_beats = 16;

  public:
  int m_bpm = 60;

  Clock()
  {
    m_timer = new QTimer( );
    m_timer->setTimerType( Qt::PreciseTimer );
  }

  template <typename T>
  void registerUpdate( T const* o )
  {
    QObject::connect( m_timer, &QTimer::timeout, o, QOverload<>::of( &T::update ) );
  }

  void start( bool fromBeginning = true )
  {
    m_beat = fromBeginning ? 0 : m_beat;
    m_timer->start( getInterval() );
    m_is_running = true;

    qDebug() << "Clock::start";
  }

  int getInterval() const
  {
    /*
     * 60 seconds in a minute
     * 4 PADSIZEth notes in a beat
     * 1000 milliseconds in a seconds
     */
    return 60.f / m_bpm / 4 * 1000;
    // return 60.f / m_bpm * 1000;
  }

  int beat()
  {
    return m_beat;
  }

  void setBpm( int bpm )
  {
    m_bpm = bpm;
    m_timer->setInterval( getInterval() );
  }

  void stop()
  {
    m_timer->stop();
    m_is_running = false;

    qDebug() << "Clock::stop";
  }

  bool isRunning() const
  {
    return m_is_running;
  }

  void update()
  {
    qDebug() << "Clock::update" << m_beat;
    m_beat = m_beat == m_num_beats - 1 ? 0 : m_beat + 1;
  }

  public slots:

  void setNumBeats( int num )
  {
    m_num_beats = num;
  }

};

#endif
