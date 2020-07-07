#ifndef _APP_CLOCK_H_
#define _APP_CLOCK_H_

#include "include.h"

class Clock : public QObject
{
  Q_OBJECT

  Q_PROPERTY( int beat READ beat NOTIFY beatChanged )
  Q_PROPERTY( int bpm READ bpm WRITE setBpm NOTIFY bpmChanged )

  /* The timer instance */
  QTimer* m_timer = nullptr;

  /* Flag if clock is running */
  bool m_is_running = false;

  /** The current beat */
  int m_beat = 0;

  /* Number of beats before repeating */
  int m_num_beats = 16;

  public:

  /* beats per minute */
  int m_bpm = 60;

  /* Constructor */
  Clock()
  {
    m_timer = new QTimer( );
    m_timer->setTimerType( Qt::PreciseTimer );
  }

  /* connect to clock */
  template <typename T>
  void registerUpdate( T const* o )
  {
    QObject::connect( m_timer, &QTimer::timeout, o, QOverload<>::of( &T::update ) );
  }

  /* Start the clock */
  void start( bool fromBeginning = true )
  {
    m_beat = fromBeginning ? 0 : m_beat;
    m_timer->start( getInterval() );
    m_is_running = true;

    // qDebug() << "Clock::start";
  }

  int getInterval() const
  {
    /*
     * 60 seconds in a minute
     * 4 16th notes in a beat
     * 1000 milliseconds in a seconds
     */
    return 60.f / m_bpm / 4 * 1000;
    // return 60.f / m_bpm * 1000;
  }

  /* Get the current beat */
  int beat()
  {
    return m_beat;
  }

  /* Get the current bpm */
  int bpm()
  {
    return m_bpm;
  }

  /* Set the BPM */
  void setBpm( int bpm )
  {
    m_bpm = bpm;
    m_timer->setInterval( getInterval() );

    emit bpmChanged( m_bpm );
  }

  /* Stop the clock */
  void stop()
  {
    m_timer->stop();
    m_is_running = false;

    // qDebug() << "Clock::stop";
  }

  /* Returns true iff the clock is running */
  bool isRunning() const
  {
    return m_is_running;
  }

  /* Update the clock. One tick */
  void update()
  {
    // qDebug() << "Clock::update" << m_beat;
    emit beatChanged( m_beat );

    m_beat = m_beat == m_num_beats - 1 ? 0 : m_beat + 1;
  }

  public slots:

  /* Set the number of beats before repeating */
  void setNumBeats( int num )
  {
    m_num_beats = num;
  }

  signals:

  void beatChanged( int beat );
  void bpmChanged( int bpm );

};

#endif
