#ifndef _PAD_MODEL_H_
#define _PAD_MODEL_H_

#include "include.h"

class Pad
{
  int m_index;
  bool m_engaged = false;
  bool m_playing = false;

  public:
  Pad( int index )
    : m_index( index ),
    m_engaged( false ),
    m_playing( false )
  {}

  int index() const
  {
    return m_index;
  }

  bool engaged() const
  {
    return m_engaged;
  }
  bool playing() const
  {
    return m_playing;
  }

  void setEngaged( bool engaged )
  {
    m_engaged = engaged;
  }

  void setPlaying( bool playing )
  {
    m_playing = playing;
  }

  void reset()
  {
    m_engaged = false;
    m_playing = false;
  }

  void toggleEngaged()
  {
    m_engaged = !m_engaged;
  }
};

class PadModel : public QAbstractListModel
{
  Q_OBJECT

  Q_PROPERTY( int padSize READ padSize WRITE setPadSize NOTIFY padSizeChanged )
  Q_PROPERTY( QString rootNote READ rootNote WRITE setRootNote NOTIFY rootNoteChanged )
  Q_PROPERTY( QString quality READ quality WRITE setQuality NOTIFY qualityChanged )

  static int PADSIZE;
  static constexpr int MAXPADSIZE = 16;

  /* List of Pad states */
  QList<Pad> m_pads;

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

  protected:

  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;

    roles[IndexRole] = "index";
    roles[EngagedRole] = "engaged";
    roles[PlayingRole] = "playing";

    return roles;
  }

  public:

  enum PadRoles {
    IndexRole = Qt::UserRole + 1,
    EngagedRole,
    PlayingRole
  };

  PadModel( QObject *parent = 0 ) : QAbstractListModel( parent )
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
    Mix_AllocateChannels( MAXPADSIZE * MAXPADSIZE * MAXPADSIZE );

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

    newGrid();
    loadChord();
  }

  ~PadModel()
  {
    for ( Mix_Chunk* chunk_ptr : m_wavs )
    {
      Mix_FreeChunk( chunk_ptr );
    }

    Mix_Quit();
    SDL_Quit();
  }

  void addPad( const Pad &pad )
  {
    beginInsertRows( QModelIndex(), rowCount(), rowCount() );
    m_pads << pad;
    endInsertRows();
  }

  int rowCount( QModelIndex const& parent = QModelIndex() ) const
  {
    Q_UNUSED(parent);
    return m_pads.count();
  }

  QVariant data( QModelIndex const& index, int role = Qt::DisplayRole ) const
  {
    if (index.row() < 0 || index.row() >= m_pads.count())
    {
      return QVariant();
    }

    Pad const& pad = m_pads[ index.row() ];

    if ( role == IndexRole )
    {
      return pad.index();
    }
    else if ( role == EngagedRole )
    {
      return pad.engaged();
    }
    else if ( role == PlayingRole )
    {
      return pad.playing();
    }

    return QVariant();
  }

  void update( int col )
  {
    // qDebug() << "PadModel::update" << col;

    /* Play the sounds ASAP */
    for (int i = 0 ; i < PADSIZE ; i += 1)
    {
      if ( m_pads[ col + i * PADSIZE ].engaged() )
      {
        if ( Mix_PlayChannel( -1, m_wavs[ m_chord[ i ] ], 0 ) == -1 )
        {
          qDebug() << Mix_GetError();
        }
      }
    }

    int prev_col = col == 0 ? PADSIZE - 1 : col - 1;
    for ( int i = 0 ; i < PADSIZE ; i += 1 )
    {
      /* Setup current column */
      {
        QModelIndex idx = createIndex( col + i * PADSIZE, 0 );
        Pad& pad = m_pads[ idx.row() ];

        pad.setPlaying( true );
        emit dataChanged( idx, idx, { PlayingRole } );
      }

      /* Clear previous column */
      {
        QModelIndex idx = createIndex( prev_col + i * PADSIZE, 0 );
        m_pads[ idx.row() ].setPlaying( false );
        emit dataChanged(idx, idx, { PlayingRole });
      }
    }
  }

  static int padSize()
  {
    return PADSIZE;
  }

  void setPadSize( int size )
  {
    PADSIZE = size;
    loadChord();

    emit padSizeChanged( PADSIZE );
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

  void toggleEngaged( int index )
  {
    m_pads[ index ].toggleEngaged();

    QModelIndex i = createIndex( index, 0 );
    emit dataChanged(i, i, { EngagedRole });
  }

  void clear()
  {
    for ( int i = 0 ; i < PADSIZE * PADSIZE ; i += 1 )
    {
      QModelIndex idx = createIndex( i, 0 );

      m_pads[ idx.row() ].reset();

      emit dataChanged(idx, idx, { EngagedRole, PlayingRole });
    }
  }

  void newGrid()
  {
    beginRemoveRows( QModelIndex(), 0, rowCount() );
    m_pads.clear();
    endRemoveRows();

    for (int i = 0 ; i < (PADSIZE * PADSIZE) ; i++ )
    {
      addPad( Pad( i ) );
    }

    emit padSizeChanged( PADSIZE );
  }

  void random( QString pattern )
  {
    clear();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib( 0, PADSIZE - 1 );

    for ( int i = 0 ; i < PADSIZE ; i += 1 )
    {
      int numToToggle = pattern[ i % pattern.count() ].digitValue();
      numToToggle = numToToggle > PADSIZE ? PADSIZE : numToToggle;

      std::vector<bool> isSet( PADSIZE, false );

      for ( int j = 0 ; j < numToToggle ; j += 1 )
      {
        int newInt = distrib( gen );
        while ( isSet[ newInt ] )
        {
          newInt = distrib( gen );
        }

        isSet[ newInt ] = true;

        toggleEngaged( i + newInt * PADSIZE );
      }
      QCoreApplication::processEvents();
    }
  }


  signals:
  void padSizeChanged( int size );
  void rootNoteChanged();
  void qualityChanged();
};

#endif
