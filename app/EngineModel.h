#ifndef _APP_ENGINE_MODEL_H_
#define _APP_ENGINE_MODEL_H_

#include "include.h"
#include "PadModel.h"
#include "Piano.h"
#include "Clock.h"

class EngineModel : public QObject
{
  Q_OBJECT

  /* Clock */
  Clock* m_clock = nullptr;
  /* Grid of pads */
  PadModel* m_pad_model = nullptr;
  /* Piano sounds */
  Piano* m_piano = nullptr;

  /* Engine is running */
  bool m_is_running = false;

  public:

  /* Constructor */
  EngineModel( Clock* clock, PadModel* padModel, Piano* piano )
    : m_clock( clock ), m_pad_model( padModel ), m_piano( piano )
  {
    registerUpdate( this );
    registerUpdate( m_clock );

    m_clock->setNumBeats( m_pad_model->gridWidth() );

    QObject::connect( m_pad_model, &PadModel::gridWidthChanged,
                      m_clock,    &Clock::setNumBeats );
    QObject::connect( m_pad_model, &PadModel::columnEngagedSignal,
                      m_piano,     QOverload<std::vector<int>>::of( &Piano::play ) );
    QObject::connect( m_pad_model, &PadModel::padEngaged,
                      m_piano,     QOverload<int>::of( &Piano::play ) );
  }

  /* Register object with clock */
  template <typename T>
  void registerUpdate( T const* obj )
  {
    m_clock->registerUpdate( obj );
  }

  /* Tick */
  void update()
  {
    // qDebug() << "\n*** EngineModel::update" << m_clock.m_bpm; 
    m_pad_model->update( m_clock->beat() );
  }

  public slots:

  bool isRunning() const
  {
    return m_is_running;
  }

  bool play( bool fromBeginning = true )
  {
    m_clock->start( fromBeginning );
    m_is_running = true;

    return true;
  }

  bool stop()
  {
    m_clock->stop();
    m_is_running = false;

    return false;
  }

  int beatDuration()
  {
    return m_clock->getInterval();
  }

  void setRootNote( QString root )
  {
    m_piano->setRootNote( root );
  }

  void setQuality( QString quality )
  {
    m_piano->setQuality( quality );
  }

  /* Save the pattern to file */
  void saveMidiFile( QString filename )
  {
    int track      = 1;
    int tpq        = 120;     // ticks per quarter
    int tp16th     = tpq / 4; // ticks per sixteenth
    int velocity   = 100;
    int tick_count = 0;

    smf::MidiFile midifile;
    midifile.absoluteTicks();
    midifile.addTrack( track );
    midifile.setTicksPerQuarterNote( tpq );

    for ( int i = 0 ; i < m_pad_model->gridHeight() ; i += 1 ) // for each column
    {
      for ( int j = 0 ; j < m_pad_model->gridHeight() ; j += 1 ) // for each pad
      {
        int pad_idx = i + j * m_pad_model->gridHeight();
        if ( m_pad_model->isEngaged( pad_idx ) )
        {
          int midi_note = m_piano->getMidiNote( j );

          midifile.addNoteOn(  track,          tick_count, track, midi_note, velocity );
          midifile.addNoteOff( track, tick_count + tp16th, track, midi_note );
        }
      }
      tick_count += tp16th;
    }

    midifile.sortTracks();
    midifile.joinTracks();

    std::string path = QUrl( filename  ).path().toStdString();
    path += filename.endsWith( ".mid" ) ? "" : ".mid";
    midifile.write( path );
  }

  signals:

  void bpmChanged();
};


#endif
