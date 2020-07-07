#ifndef _PAD_MODEL_H_
#define _PAD_MODEL_H_

#include "include.h"

class Pad
{
  int m_index = 0;
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

  int GRID_W = 16;
  int GRID_H = 16;

  Q_PROPERTY( int gridWidth READ gridWidth WRITE setGridWidth NOTIFY gridWidthChanged )
  Q_PROPERTY( int gridHeight READ gridHeight WRITE setGridHeight NOTIFY gridHeightChanged )


  /* Maximum supported dimensions of the grid */
  static constexpr int MAXGRID_H = 16;

  /* List of Pad states */
  QList<Pad> m_pads;

  protected:

  /* For using `PadModel` as a model for `GridLayout` */
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;

    roles[IndexRole] = "index";
    roles[EngagedRole] = "engaged";
    roles[PlayingRole] = "playing";

    return roles;
  }

  public:

  /* For using `PadModel` as a model for `GridLayout` */
  enum PadRoles {
    IndexRole = Qt::UserRole + 1,
    EngagedRole,
    PlayingRole
  };

  /* Constructur */
  PadModel( QObject *parent = 0 ) : QAbstractListModel( parent )
  {
    newGrid();
  }

  /* Add a pad to the pool */
  void addPad( const Pad &pad )
  {
    beginInsertRows( QModelIndex(), rowCount(), rowCount() );
    m_pads << pad;
    endInsertRows();
  }

  /* The number of pads */
  int rowCount( QModelIndex const& parent = QModelIndex() ) const
  {
    Q_UNUSED(parent);
    return m_pads.count();
  }

  /* For retrieving values */
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

  /* One tick */
  void update( int col )
  {
    /* Play the sounds ASAP */
    std::vector<int> note_idxs;
    for (int i = 0 ; i < GRID_H ; i += 1)
    {
      if ( m_pads[ col + i * GRID_W ].engaged() )
      {
        note_idxs.push_back( i );
      }
    }
    emit columnEngagedSignal( note_idxs );

    int prev_col = col == 0 ? GRID_W - 1 : col - 1;
    for ( int i = 0 ; i < GRID_H ; i += 1 )
    {
      {
        QModelIndex idx = createIndex( col + i * GRID_W, 0 );
        Pad& pad = m_pads[ idx.row() ];

        if ( pad.engaged() )
        {
          pad.setPlaying( true );
          emit dataChanged( idx, idx, { PlayingRole } );
        }
      }

      {
        QModelIndex idx = createIndex( prev_col + i * GRID_W, 0 );
        Pad& pad = m_pads[ idx.row() ];

        if ( pad.engaged() || pad.playing() )
        {
          pad.setPlaying( false );
          emit dataChanged( idx, idx, { PlayingRole } );
        }
      }
    }
  }

  /* Get the grid size */
  int gridWidth()
  {
    return GRID_W;
  }

  /* Get the grid size */
  int gridHeight()
  {
    return GRID_H;
  }

  /* Set the grid size */
  void setGridWidth( int w )
  {
    GRID_W = w;

    emit gridWidthChanged( GRID_W );
  }

  /* Set the grid size */
  void setGridHeight( int h )
  {
    GRID_H = h;

    emit gridHeightChanged( GRID_H );
  }

  public slots:

  /* Toggle a pad
   * index is into the 1D m_pads array */
  void toggleEngaged( int index )
  {
    m_pads[ index ].toggleEngaged();

    if ( m_pads[ index ].engaged() )
    {
      /* Send an index into the column */
      emit padEngaged( index / GRID_W );
    }

    QModelIndex i = createIndex( index, 0 );
    emit dataChanged(i, i, { EngagedRole });
  }

  bool isEngaged( int index )
  {
    return m_pads[ index ].engaged();
  }

  /* Clear state of all pads */
  void clear()
  {
    for ( int i = 0 ; i < GRID_W * GRID_H ; i += 1 )
    {
      QModelIndex idx = createIndex( i, 0 );

      m_pads[ idx.row() ].reset();

      emit dataChanged( idx, idx, { EngagedRole, PlayingRole } );
    }
  }

  /* Create a new grid */
  void newGrid()
  {
    beginRemoveRows( QModelIndex(), 0, rowCount() );
    m_pads.clear();
    endRemoveRows();

    for (int i = 0 ; i < GRID_W * GRID_H ; i++ )
    {
      addPad( Pad( i ) );
    }

    emit gridHeightChanged( GRID_H );
    emit gridWidthChanged( GRID_W );
  }

  /* Randomize the engaged pads */
  void random( QString pattern )
  {
    clear();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib( 0, GRID_H - 1 );

    for ( int i = 0 ; i < GRID_W ; i += 1 )
    {
      int numToToggle = pattern[ i % pattern.count() ].digitValue();
      numToToggle = numToToggle > GRID_H ? GRID_H : numToToggle;

      std::vector<bool> isSet( GRID_H, false );

      for ( int j = 0 ; j < numToToggle ; j += 1 )
      {
        int newInt = distrib( gen );
        while ( isSet[ newInt ] )
        {
          newInt = distrib( gen );
        }

        isSet[ newInt ] = true;

        toggleEngaged( i + newInt * GRID_W );
      }
      QCoreApplication::processEvents();
    }
  }

  signals:

  void gridWidthChanged( int w );
  void gridHeightChanged( int h );
  void padEngaged( int index );
  void columnEngagedSignal( std::vector<int> notes );
};

#endif
