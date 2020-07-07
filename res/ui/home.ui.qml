import QtQuick 2.15
import QtQuick.Controls 2.2
// import com.app 1.0 // Wav
import QtGraphicalEffects 1.12 // RadialGradient
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.1


Page {
  function resetRandomOptionsBoxPosition() {
    randomOptionsBox.x = page.width / 2 - randomOptionsBox.width / 2
    randomOptionsBox.y = page.height / 2 - randomOptionsBox.height / 2
  }

  id: page
  onWidthChanged: resetRandomOptionsBoxPosition()
  onHeightChanged: onWidthChanged

  /* top controls */
  GridLayout {
    columns: 5
    anchors.bottom: padGrid.top;
    anchors.horizontalCenter: parent.horizontalCenter;

    /* play/stop */
    Button {
      id: playStop
      property bool isEngaged

      text: isEngaged ? "Stop" : "Play";
      highlighted: isEngaged
      onPressed: isEngaged = isEngaged ? engineModel.stop() : engineModel.play();
    }

    /* tempo */
    SpinBox {
      id: tempo
      value: clock.bpm
      editable: true
      stepSize: 1
      from: 30
      to: 300

      onValueModified: clock.bpm = value;
    }

    /* grid height */
    SpinBox {
      id: gridHeight
      value: padModel.gridHeight
      editable: true
      stepSize: 1
      from: 1
      to: 16

      onValueModified: {
        engineModel.stop();

        playStop.isEngaged = false;
        padModel.gridHeight = value;
      }
    }

    /* grid width */
    SpinBox {
      id: gridWidth
      value: padModel.gridWidth
      editable: true
      stepSize: 16
      from: 1
      to: 256

      onValueModified: {
        engineModel.stop();

        playStop.isEngaged = false;
        padModel.gridWidth = value;
      }
    }

    /* clear */
    Button {
      id: clearButton
      text: "Clear"
      onPressed: {
        engineModel.stop();

        playStop.isEngaged = false;
        padModel.clear();
      }
    }

    /* empty */
    Rectangle {
      Layout.minimumWidth: playStop.width
    }

    Label {
      text: "bpm"
      Layout.minimumWidth: tempo.width
      horizontalAlignment: Text.AlignHCenter
    }

    Label {
      text: "height"
      Layout.minimumWidth: gridHeight.width
      horizontalAlignment: Text.AlignHCenter
    }

    Label {
      text: "width"
      Layout.minimumWidth: gridWidth.width
      horizontalAlignment: Text.AlignHCenter
    }

    /* empty */
    Rectangle {
      Layout.minimumWidth: clearButton.width
    }
  }

  /* pads */
  GridView {
    /* data */
    id: padGrid
    model: padModel

    required model

    /* anchors */
    anchors.verticalCenter: parent.verticalCenter
    x: contentX + width >= page.width ? 0 : page.width / 2 - padGrid.width / 2

    /* visual */
    height: Math.min( page.width, page.height  ) * 0.75
    cellHeight: height / padModel.gridHeight
    cellWidth: cellHeight
    width: cellHeight * padModel.gridWidth

    interactive: false

    delegate: Component {
      Item {
        id: cont
        height: padGrid.cellHeight
        width: height

        Rectangle {
          // required property int index
          // required property bool engaged
          // required property bool playing

          /* data */
          id: delegate

          /* anchors */
          anchors.centerIn: parent
          anchors.fill: parent

          color: "#00000000"

          Rectangle {
            id: bg
            anchors.centerIn: parent
            height: parent.height - 3
            width: height
            radius: 3
            color: engaged ? playing ? '#d0ff7752' : '#80ffffff' : '#20ffffff'
          }

          MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
              padModel.toggleEngaged( index, true )
            }
          }

          Rectangle {
            // required property bool playing
            // required property bool engaged

            /* data */
            id: playingRect
            anchors.centerIn: parent
            visible: playing && engaged

            /* visual */
            height: delegate.height
            width: delegate.height
            radius: 50
            color: "#00000000"

            RadialGradient {
              id: padGradient
              anchors.fill: parent
              gradient: Gradient {
                GradientStop { position: 0.0; color: "#80ffffff" }
                GradientStop { position: 0.5; color: "#00ffffff" }
              }
            }
          } // Rectangle
        } // Button
      } // Item
    } // delegate

    DragHandler {
      yAxis.enabled: false
    }
  } // GridView

  /* bottom controls */
  Row {
    anchors.top: padGrid.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: 2
    Button {
      text: "Random"
      onClicked: randomOptionsBox.visible = ! randomOptionsBox.visible
    }

    ComboBox {
      model: ListModel {
        ListElement { text: "C" }
        ListElement { text: "Db" }
        ListElement { text: "D" }
        ListElement { text: "Eb" }
        ListElement { text: "E" }
        ListElement { text: "F" }
        ListElement { text: "Gb" }
        ListElement { text: "G" }
        ListElement { text: "Ab" }
        ListElement { text: "A" }
        ListElement { text: "Bb" }
        ListElement { text: "B" }
      }
      onActivated: {
        var wasRunning = engineModel.isRunning();
        if ( wasRunning )
        {
          engineModel.stop();
        }

        engineModel.setRootNote( currentValue );

        if ( wasRunning )
        {
          engineModel.play( false );
        }
      }
    }
    ComboBox {
      model: ListModel {
        ListElement { text: "Maj7" }
        ListElement { text: "Maj9" }
        ListElement { text: "7" }
        ListElement { text: "9" }
        ListElement { text: "-7" }
        ListElement { text: "+7" }
        ListElement { text: "half dim" }
        ListElement { text: "O" }
        ListElement { text: "minMaj" }
        ListElement { text: "Maj7#5" }
        ListElement { text: "7#11" }
        ListElement { text: "7#9" }
        ListElement { text: "7#5#9" }
        ListElement { text: "7b9" }
        ListElement { text: "-9" }
        ListElement { text: "-7b9" }
        ListElement { text: "min" }
      }
      onActivated: {
        var wasRunning = engineModel.isRunning();
        if ( wasRunning )
        {
          engineModel.stop();
        }

        engineModel.setQuality( currentValue );

        if ( wasRunning )
        {
          engineModel.play( false );
        }
      }
    }
    Button {
      text: "Save as midi"
      onClicked: fileDialog.open()
    }
  }

  /* Cursor */
  Rectangle {
    id: cursor
    x: padGrid.x - padGrid.contentX + 0 + padGrid.cellWidth * clock.beat
    y: padGrid.y
    width: padGrid.cellWidth
    height: padGrid.height
    color: '#22ffffff'
  }

  /* Random Options Box */
  Rectangle {
    id: randomOptionsBox
    visible: false
    width: 250
    height: 150
    x: page.width / 2 - width / 2
    y: page.height / 2 - height / 2
    color: "#ff303030"

    DragHandler { }

    Column {
      /* Pattern */
      anchors.fill: parent
      padding: 5
      topPadding: 10

      Label {
        text: "Notes per column pattern (e.g. 1212)"
        anchors.horizontalCenter: parent.horizontalCenter
        wrapMode: Label.WordWrap
        width: 200
      }

      TextField {
        id: randomPattern
        anchors.horizontalCenter: parent.horizontalCenter
        width: 175
        text: "31103021"
        horizontalAlignment: Qt.AlignHCenter
      }

      /* Apply/Close */
      Row {
        anchors.horizontalCenter: parent.horizontalCenter
        padding: 5
        Button {
          text: "Apply"

          onClicked: {
            var wasRunning = engineModel.isRunning();
            if ( wasRunning )
            {
              engineModel.stop();
            }

            padModel.random( randomPattern.text );

            if ( wasRunning )
            {
              engineModel.play( false );
            }
          }
        }
        Button {
          text: "Close"

          onPressed: randomOptionsBox.visible = false
        }
      }
    }
  }

  /* Options Box Shadow */
  DropShadow {
    id: randomOptionsBoxShadow
    visible: randomOptionsBox.visible
    anchors.fill: randomOptionsBox
    horizontalOffset: 3
    verticalOffset: 3
    spread: 0.0
    radius: 20.0
    samples: 25
    color: "#80000000"
    source: randomOptionsBox
  }

  /* For save as midi */
  FileDialog {
    id: fileDialog
    title: "Please choose a file"
    // folder: shortcuts.home
    folder: StandardPaths.writableLocation( StandardPaths.HomeLocation )
    fileMode: FileDialog.SaveFile
    visible: false
    onAccepted: {
      engineModel.saveMidiFile( fileDialog.file );
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }
}
