#ifndef _APP_PIANO_H_
#define _APP_PIANO_H_

#include "include.h"

struct Piano : public QObject
{
  Q_OBJECT

  int PADSIZE = 16;
  static constexpr int MAXPADSIZE = 16;

  /* Cache of wav files */
  std::vector<Mix_Chunk*> m_wavs;

  std::vector<const char*> m_wav_filenames =
  {
    "wav/C1.wav",
    "wav/Db1.wav",
    "wav/D1.wav",
    "wav/Eb1.wav",
    "wav/E1.wav",
    "wav/F1.wav",
    "wav/Gb1.wav",
    "wav/G1.wav",
    "wav/Ab1.wav",
    "wav/A1.wav",
    "wav/Bb1.wav",
    "wav/B1.wav",

    "wav/C2.wav",
    "wav/Db2.wav",
    "wav/D2.wav",
    "wav/Eb2.wav",
    "wav/E2.wav",
    "wav/F2.wav",
    "wav/Gb2.wav",
    "wav/G2.wav",
    "wav/Ab2.wav",
    "wav/A2.wav",
    "wav/Bb2.wav",
    "wav/B2.wav",

    "wav/C3.wav",
    "wav/Db3.wav",
    "wav/D3.wav",
    "wav/Eb3.wav",
    "wav/E3.wav",
    "wav/F3.wav",
    "wav/Gb3.wav",
    "wav/G3.wav",
    "wav/Ab3.wav",
    "wav/A3.wav",
    "wav/Bb3.wav",
    "wav/B3.wav",

    "wav/C4.wav",
    "wav/Db4.wav",
    "wav/D4.wav",
    "wav/Eb4.wav",
    "wav/E4.wav",
    "wav/F4.wav",
    "wav/Gb4.wav",
    "wav/G4.wav",
    "wav/Ab4.wav",
    "wav/A4.wav",
    "wav/Bb4.wav",
    "wav/B4.wav",
  };

  /* Maps QString -> int */
  QMap<QString, int> m_note_map =
  {
    { "C" , 0 },
    { "Db" , 1 },
    { "C#" , 1 },
    { "D" , 2 },
    { "Eb" , 3 },
    { "D#" , 3 },
    { "E" , 4 },
    { "F" , 5 },
    { "Gb" , 6 },
    { "F#" , 6 },
    { "G" , 7 },
    { "Ab" , 8 },
    { "G#" , 8 },
    { "A" , 9 },
    { "Bb" , 10 },
    { "A#" , 10 },
    { "B" , 11 }
  };

  /* Maps QString -> List of chord notes */
  QMap<QString, QList<int>> m_quality_map =
  {
    // C  Db D  Eb E  F  Gb G  Ab A  Bb B
    // 00 01 02 03 04 05 06 07 08 09 10 11
    // 12 13 14 15 16 17 18 19 20 21 22 23
    // 24 25 26 27 28 29 30 31 32 33 34 35
    // 36 37 38 39 40 41 42 43 44 45 46 47

    // 1 3 5 maj
    { "Maj7" ,     { 0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35, 36, 40, 43, 47 } },
    // 1 3 5 maj 9
    { "Maj9" ,     { 0, 2, 4, 7,  11, 12, 14, 19, 23, 24, 26, 28, 30, 31, 33, 35 } },
    // 1 3 5 7
    { "7" ,        { 0, 4, 7, 10, 12, 16, 19, 22, 24, 26, 28, 29, 31, 33, 34, 36 } },
    // 1 3 7 9
    { "9" ,        { 0, 2, 4, 7,  10, 12, 14, 16, 19, 22, 24, 26, 28, 29, 31, 33 } },
    // 1 b3 5 7
    { "-7" ,       { 0, 3, 7, 10, 12, 15, 19, 22, 24, 26, 27, 29, 31, 33, 34, 36 } },
    // 1 3 #5 7
    { "+7" ,       { 0, 4, 8, 10, 12, 16, 20, 22, 24, 26, 28, 29, 31, 32, 34, 36 } },
    // 1 b3 b5 7
    { "half dim" , { 0, 3, 6, 10, 12, 15, 18, 22, 24, 25, 27, 29, 30, 32, 34, 36 } },
    // 1 b3 b5 b7
    { "O" ,        { 0, 3, 6, 9,  12, 15, 18, 21, 24, 26, 27, 29, 30, 32, 33, 35 } },
    // 1 b3 5 maj
    { "minMaj" ,   { 0, 3, 7, 11, 12, 15, 19, 23, 24, 26, 27, 29, 31, 33, 35, 36 } },
    // 1 3 #5 maj 1
    { "Maj7#5" ,   { 0, 4, 8, 11, 12, 16, 20, 23, 24, 26, 28, 30, 32, 33, 35, 36 } },
    // 1 3 b5 7 1
    { "7#11" ,     { 0, 3, 6, 10, 12, 15, 18, 22, 24, 26, 28, 30, 31, 33, 34, 36 } },
    // 1 3 5 7 #9
    { "7#9" ,      { 0, 3, 4, 10, 12, 15, 16, 22, 24, 26, 27, 28, 29, 31, 33, 34 } },
    // 1 3 #5 7 #9
    { "7#5#9" ,    { 0, 3, 4, 8,  10, 12, 15, 16, 20, 22, 24, 25, 27, 28, 30, 32 } },
    // 1 3 5 7 b9
    { "7b9" ,      { 0, 1, 4, 7,  10, 12, 13, 16, 29, 22, 24, 25, 26, 28, 29, 31 } },
    // 1 3 5 7 9
    { "-9" ,       { 0, 2, 3, 7,  10, 12, 14, 15, 19, 22, 24, 26, 27, 29, 31, 33 } },
    // 1 b3 5 7 b9
    { "-7b9" ,     { 0, 1, 3, 7,  10, 12, 13, 15, 19, 22, 24, 25, 27, 29, 31, 32 } },
    // 1 b3 5 1
    { "min" ,      { 0, 3, 5, 7,  12, 15, 17, 19, 24, 26, 27, 29, 31, 32, 34, 36 } }
  };

  QString m_root_note = "C";
  QString m_quality = "Maj7";

  QList<int> m_chord;

  void loadChord()
  {
    m_chord.clear();
    m_chord.reserve( MAXPADSIZE );

    QList<int> const& quality = m_quality_map[ m_quality ];
    int root                  = m_note_map[ m_root_note ];

    int a = 0;
    int b = PADSIZE - 1;

    for ( int i = b ; i >= a ; i -= 1 )
    {
      int q = quality[ i ] + root;
      m_chord.push_back( q );
    }
  }

  public:

  Piano()
  {
    // Initialize SDL
    if( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
      qDebug() << "[sdl init]" << SDL_GetError();
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
      qDebug() << "[mix open audio]" << Mix_GetError();
    }

    /* maximum volume on all channels */
    Mix_Volume( -1, 128 );
    /* set number of simultaneous wavs */
    Mix_AllocateChannels( PADSIZE * PADSIZE * PADSIZE );

    m_wavs.reserve( m_wav_filenames.size() );

    /* Load wavs */
    for ( int i = 0 ; i < (int) m_wav_filenames.size() ; i++ )
    {
      auto m = Mix_LoadWAV( m_wav_filenames[ i ] );
      if ( ! m )
      {
        qDebug() << "[mix load mus] " << Mix_GetError();
      }

      m_wavs.push_back( m );
    }

    // newGrid();
    loadChord();
  }

  ~Piano()
  {
    for ( Mix_Chunk* chunk_ptr : m_wavs )
    {
      Mix_FreeChunk( chunk_ptr );
    }

    Mix_Quit();
    SDL_Quit();
  }

  void play( std::vector<int> note_idxs )
  {
    // qDebug() << "Piano::update" << col;

    /* Play the sounds ASAP */
    for (int i = 0 ; i < (int) note_idxs.size() ; i += 1)
    {
      if ( Mix_PlayChannel( -1, m_wavs[ m_chord[ note_idxs[ i ] ] ], 0 ) == -1 )
      {
        qDebug() << Mix_GetError();
      }
    }
  }

  void play( int padNum )
  {
    qDebug() << padNum;
    if ( Mix_PlayChannel( -1, m_wavs[ m_chord[ padNum / PADSIZE ] ], 0 ) == -1 )
    {
      qDebug() << Mix_GetError();
    }
  }

  int padSize()
  {
    return PADSIZE;
  }

  void setPadSize( int size )
  {
    PADSIZE = size;
    loadChord();
  }

  QString quality()
  {
    return m_quality;
  }

  void setQuality( QString quality )
  {
    m_quality = quality;
    loadChord();
  }

  QString& rootNote()
  {
    return m_root_note;
  }

  void setRootNote( QString root )
  {
    m_root_note = root;
    loadChord();
  }

  public slots:

  /* Save the pattern to file */
  /*
  void saveMidiFile( QString filename )
  {
    int track = 1;
    int tpq = 120; // ticks per quarter
    int tp16th = tpq / 4; // ticks per sixteenth
    int midinote_offset = 24;
    int velocity = 100;
    int tick_count = 0;

    smf::MidiFile midifile;
    midifile.absoluteTicks();
    midifile.addTrack( track );
    midifile.setTicksPerQuarterNote( tpq );

    for ( int i = 0 ; i < PADSIZE ; i += 1 ) // for each column
    {
      for ( int j = 0 ; j < PADSIZE ; j += 1 ) // for each pad
      {
        int pad_idx = i + j * PADSIZE;
        if ( m_pads[ pad_idx ].engaged() )
        {
          int midi_note = 47 - m_quality_map[ m_quality ][ j ] + m_note_map[ m_root_note ] + midinote_offset;

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
  */

  signals:

  void rootNoteChanged();
  void qualityChanged();
};

#endif
